/**
 * @file HWMonitor. cpp
 * @brief Hardware Monitor Protocol Parser Implementation
 * @version 2.0 - 16-bit sensor IDs
 */

#include "HWMonitor.h"

/*===========================================================================*/
/*  CONSTRUCTOR & INITIALIZATION                                             */
/*===========================================================================*/

HWMonitor::HWMonitor()
    : packetsOK(0), packetsError(0), sensorCount(0), lastUpdate(0), _state(HW_STATE_IDLE), _expectedCount(0), _currentSensor(0), _byteInValue(0), _tempId(0), _tempIdHigh(0), _crcLow(0), _crcHigh(0), _packetCallback(nullptr), _sensorCallback(nullptr)
{
}

void HWMonitor::begin()
{
    reset();
}

void HWMonitor::reset()
{
    for (uint8_t i = 0; i < HW_MAX_SENSORS; i++)
    {
        _sensors[i].id = SENSOR_UNKNOWN;
        _sensors[i].value = -999.0f;
        _sensors[i].valid = false;
        _sensors[i].timestamp = 0;
    }

    _state = HW_STATE_IDLE;
    _expectedCount = 0;
    _currentSensor = 0;
    _byteInValue = 0;
    _tempId = 0;
    _tempIdHigh = 0;
    sensorCount = 0;
    packetsOK = 0;
    packetsError = 0;
    lastUpdate = 0;
}

/*===========================================================================*/
/*  RESERVED ID CHECK                                                        */
/*===========================================================================*/

bool HWMonitor::isReservedId(uint16_t id)
{
    uint8_t high = (uint8_t)(id >> 8);
    uint8_t low = (uint8_t)(id & 0xFF);
    return high == HW_RESERVED_START || high == HW_RESERVED_END ||
           low == HW_RESERVED_START || low == HW_RESERVED_END;
}

/*===========================================================================*/
/*  STREAM UPDATE                                                            */
/*===========================================================================*/

bool HWMonitor::update(Stream &stream)
{
    bool packetReceived = false;

    while (stream.available())
    {
        if (processByte(stream.read()))
        {
            packetReceived = true;
        }
    }

    return packetReceived;
}

/*===========================================================================*/
/*  BYTE-BY-BYTE PARSER (Protocol v2 - 16-bit IDs)                           */
/*===========================================================================*/

bool HWMonitor::processByte(uint8_t byte)
{
    switch (_state)
    {
    case HW_STATE_IDLE:
        if (byte == HW_PROTO_START)
        {
            _state = HW_STATE_VERSION;
        }
        break;

    case HW_STATE_VERSION:
        if (byte == HW_PROTO_VERSION)
        {
            _state = HW_STATE_COUNT;
        }
        else
        {
            // Wrong version - could be old protocol or error
            _state = HW_STATE_IDLE;
            packetsError++;
        }
        break;

    case HW_STATE_COUNT:
        _expectedCount = byte;
        _currentSensor = 0;
        _byteInValue = 0;

        if (byte > 0 && byte <= HW_MAX_SENSORS)
        {
            _state = HW_STATE_ID_HIGH;
        }
        else
        {
            _state = HW_STATE_IDLE;
            packetsError++;
        }
        break;

    case HW_STATE_ID_HIGH:
        // First byte of 16-bit sensor ID (high byte)
        _tempIdHigh = byte;
        _state = HW_STATE_ID_LOW;
        break;

    case HW_STATE_ID_LOW:
        // Second byte of 16-bit sensor ID (low byte)
        _tempId = ((uint16_t)_tempIdHigh << 8) | byte;
        _byteInValue = 0;
        _state = HW_STATE_VALUE;
        break;

    case HW_STATE_VALUE:
        // Reading 4-byte float value (little-endian)
        _tempValue[_byteInValue] = byte;
        _byteInValue++;

        if (_byteInValue >= 4)
        {
            // Complete sensor data received
            _storeSensor();
            _currentSensor++;
            _byteInValue = 0;

            if (_currentSensor >= _expectedCount)
            {
                // All sensors received, expect CRC
                _state = HW_STATE_CRC_LOW;
            }
            else
            {
                // More sensors to read
                _state = HW_STATE_ID_HIGH;
            }
        }
        break;

    case HW_STATE_CRC_LOW:
        _crcLow = byte;
        _state = HW_STATE_CRC_HIGH;
        break;

    case HW_STATE_CRC_HIGH:
        _crcHigh = byte;
        _state = HW_STATE_END;
        break;

    case HW_STATE_END:
        _state = HW_STATE_IDLE;

        if (byte == HW_PROTO_END)
        {
            return _finalizePacket();
        }
        else
        {
            packetsError++;
        }
        break;
    }

    return false;
}

void HWMonitor::_storeSensor()
{
    if (_currentSensor >= HW_MAX_SENSORS)
        return;

    // Skip sensors with reserved IDs
    if (isReservedId(_tempId))
    {
        return;
    }

    // Convert bytes to float (little-endian)
    union
    {
        float f;
        uint8_t b[4];
    } converter;

    converter.b[0] = _tempValue[0];
    converter.b[1] = _tempValue[1];
    converter.b[2] = _tempValue[2];
    converter.b[3] = _tempValue[3];

    _sensors[_currentSensor].id = _tempId;
    _sensors[_currentSensor].value = converter.f;
    _sensors[_currentSensor].valid = true;
    _sensors[_currentSensor].timestamp = millis();

    // Call sensor callback if set
    if (_sensorCallback)
    {
        _sensorCallback(_tempId, converter.f);
    }
}

bool HWMonitor::_finalizePacket()
{
    sensorCount = _currentSensor; // Actual count (may differ if reserved IDs skipped)
    lastUpdate = millis();
    packetsOK++;

    // Call packet callback if set
    if (_packetCallback)
    {
        _packetCallback(sensorCount);
    }

    return true;
}

/*===========================================================================*/
/*  BUFFER PARSER                                                            */
/*===========================================================================*/

bool HWMonitor::parse(const uint8_t *data, size_t len)
{
    if (!data || len < 6)
        return false;

    // Find start byte
    size_t startIdx = 0;
    while (startIdx < len && data[startIdx] != HW_PROTO_START)
    {
        startIdx++;
    }

    if (startIdx >= len)
        return false;

    const uint8_t *pkt = data + startIdx;
    size_t remaining = len - startIdx;

    // Check header
    if (remaining < 3)
        return false;
    if (pkt[1] != HW_PROTO_VERSION)
    {
        packetsError++;
        return false;
    }

    uint8_t count = pkt[2];
    // Protocol v2: 6 bytes per sensor (2 ID + 4 value)
    size_t expectedLen = 3 + (count * HW_SENSOR_DATA_SIZE) + 3;

    if (remaining < expectedLen)
    {
        packetsError++;
        return false;
    }

    if (pkt[expectedLen - 1] != HW_PROTO_END)
    {
        packetsError++;
        return false;
    }

    // Parse sensor data
    size_t offset = 3;
    uint8_t validSensors = 0;

    for (uint8_t i = 0; i < count && validSensors < HW_MAX_SENSORS; i++)
    {
        // Read 16-bit ID (big-endian in packet)
        uint16_t id = ((uint16_t)pkt[offset] << 8) | pkt[offset + 1];

        // Skip reserved IDs
        if (isReservedId(id))
        {
            offset += HW_SENSOR_DATA_SIZE;
            continue;
        }

        union
        {
            float f;
            uint8_t b[4];
        } converter;

        converter.b[0] = pkt[offset + 2];
        converter.b[1] = pkt[offset + 3];
        converter.b[2] = pkt[offset + 4];
        converter.b[3] = pkt[offset + 5];

        _sensors[validSensors].id = id;
        _sensors[validSensors].value = converter.f;
        _sensors[validSensors].valid = true;
        _sensors[validSensors].timestamp = millis();

        if (_sensorCallback)
        {
            _sensorCallback(id, converter.f);
        }

        validSensors++;
        offset += HW_SENSOR_DATA_SIZE;
    }

    sensorCount = validSensors;
    lastUpdate = millis();
    packetsOK++;

    if (_packetCallback)
    {
        _packetCallback(sensorCount);
    }

    return true;
}

/*===========================================================================*/
/*  DATA ACCESS                                                              */
/*===========================================================================*/

float HWMonitor::get(uint16_t id, float defaultValue) const
{
    for (uint8_t i = 0; i < sensorCount; i++)
    {
        if (_sensors[i].id == id && _sensors[i].valid)
        {
            return _sensors[i].value;
        }
    }
    return defaultValue;
}

bool HWMonitor::isValid(uint16_t id) const
{
    for (uint8_t i = 0; i < sensorCount; i++)
    {
        if (_sensors[i].id == id)
        {
            return _sensors[i].valid;
        }
    }
    return false;
}

const HWSensor *HWMonitor::getSensorByIndex(uint8_t index) const
{
    if (index < sensorCount)
    {
        return &_sensors[index];
    }
    return nullptr;
}

const HWSensor *HWMonitor::findSensor(uint16_t id) const
{
    for (uint8_t i = 0; i < sensorCount; i++)
    {
        if (_sensors[i].id == id)
        {
            return &_sensors[i];
        }
    }
    return nullptr;
}

void HWMonitor::invalidateAll()
{
    for (uint8_t i = 0; i < HW_MAX_SENSORS; i++)
    {
        _sensors[i].valid = false;
    }
}

bool HWMonitor::isStale(uint32_t timeoutMs) const
{
    if (lastUpdate == 0)
        return true;
    return (millis() - lastUpdate) > timeoutMs;
}

uint32_t HWMonitor::getAge() const
{
    if (lastUpdate == 0)
        return UINT32_MAX;
    return millis() - lastUpdate;
}

void HWMonitor::onPacket(HWPacketCallback callback)
{
    _packetCallback = callback;
}

void HWMonitor::onSensor(HWSensorCallback callback)
{
    _sensorCallback = callback;
}

/*===========================================================================*/
/*  CRC CALCULATION                                                          */
/*===========================================================================*/

uint16_t HWMonitor::_calculateCRC(const uint8_t *data, size_t len) const
{
    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc = (crc >> 1) ^ 0xA001;
            }
            else
            {
                crc = crc >> 1;
            }
        }
    }

    return crc;
}

/*===========================================================================*/
/*  UTILITY FUNCTIONS                                                        */
/*===========================================================================*/

const char *hwGetSensorName(uint16_t id)
{
    switch (id)
    {
    // CPU
    case SENSOR_CPU_TEMP:
        return "CPU Temperature";
    case SENSOR_CPU_LOAD:
        return "CPU Load";
    case SENSOR_CPU_CLOCK:
        return "CPU Clock";
    case SENSOR_CPU_POWER:
        return "CPU Power";
    case SENSOR_CPU_TEMP_CORE:
        return "CPU Core Temp";
    case SENSOR_CPU_LOAD_CORE:
        return "CPU Core Load";
    case SENSOR_CPU_POWER_CORE:
        return "CPU Core Power";
    case SENSOR_CPU_TEMP_CCD:
        return "CPU CCD Temp";
    case SENSOR_CPU_VOLTAGE:
        return "CPU Voltage";

    // GPU
    case SENSOR_GPU_TEMP:
        return "GPU Temperature";
    case SENSOR_GPU_LOAD:
        return "GPU Load";
    case SENSOR_GPU_CLOCK:
        return "GPU Clock";
    case SENSOR_GPU_CLOCK_MEM:
        return "GPU Memory Clock";
    case SENSOR_GPU_POWER:
        return "GPU Power";
    case SENSOR_GPU_LOAD_MEM:
        return "GPU Memory Load";
    case SENSOR_GPU_FAN:
        return "GPU Fan";
    case SENSOR_GPU_TEMP_MEM:
        return "GPU Memory Temp";
    case SENSOR_GPU_HOTSPOT:
        return "GPU Hotspot";
    case SENSOR_GPU_LOAD_VIDEO:
        return "GPU Video Load";

    // RAM
    case SENSOR_RAM_USED:
        return "RAM Used";
    case SENSOR_RAM_AVAILABLE:
        return "RAM Available";
    case SENSOR_RAM_LOAD:
        return "RAM Load";

    // Disk
    case SENSOR_DISK_TEMP:
        return "Disk Temperature";
    case SENSOR_DISK_LOAD:
        return "Disk Load";
    case SENSOR_DISK_READ:
        return "Disk Read";
    case SENSOR_DISK_WRITE:
        return "Disk Write";

    // Network
    case SENSOR_NET_UP:
        return "Network Upload";
    case SENSOR_NET_DOWN:
        return "Network Download";

    // Motherboard
    case SENSOR_MB_TEMP:
        return "Motherboard Temp";
    case SENSOR_MB_FAN1:
        return "System Fan 1";
    case SENSOR_MB_FAN2:
        return "System Fan 2";
    case SENSOR_MB_FAN3:
        return "System Fan 3";
    case SENSOR_MB_FAN4:
        return "System Fan 4";
    case SENSOR_MB_VOLTAGE:
        return "System Voltage";

    // Battery
    case SENSOR_BATTERY_LEVEL:
        return "Battery Level";
    case SENSOR_BATTERY_VOLTAGE:
        return "Battery Voltage";
    case SENSOR_BATTERY_RATE:
        return "Battery Rate";

    default:
        return "Unknown";
    }
}

const char *hwGetSensorUnit(uint16_t id)
{
    switch (id)
    {
    // Temperatures
    case SENSOR_CPU_TEMP:
    case SENSOR_CPU_TEMP_CORE:
    case SENSOR_CPU_TEMP_CCD:
    case SENSOR_GPU_TEMP:
    case SENSOR_GPU_TEMP_MEM:
    case SENSOR_GPU_HOTSPOT:
    case SENSOR_DISK_TEMP:
    case SENSOR_MB_TEMP:
        return "°C";

    // Loads/Percentages
    case SENSOR_CPU_LOAD:
    case SENSOR_CPU_LOAD_CORE:
    case SENSOR_GPU_LOAD:
    case SENSOR_GPU_LOAD_MEM:
    case SENSOR_GPU_LOAD_VIDEO:
    case SENSOR_RAM_LOAD:
    case SENSOR_DISK_LOAD:
    case SENSOR_BATTERY_LEVEL:
        return "%";

    // Clocks
    case SENSOR_CPU_CLOCK:
    case SENSOR_GPU_CLOCK:
    case SENSOR_GPU_CLOCK_MEM:
        return "MHz";

    // Power
    case SENSOR_CPU_POWER:
    case SENSOR_CPU_POWER_CORE:
    case SENSOR_GPU_POWER:
    case SENSOR_BATTERY_RATE:
        return "W";

    // Voltage
    case SENSOR_CPU_VOLTAGE:
    case SENSOR_MB_VOLTAGE:
    case SENSOR_BATTERY_VOLTAGE:
        return "V";

    // Fan
    case SENSOR_GPU_FAN:
    case SENSOR_MB_FAN1:
    case SENSOR_MB_FAN2:
    case SENSOR_MB_FAN3:
    case SENSOR_MB_FAN4:
        return "RPM";

    // Data sizes
    case SENSOR_RAM_USED:
    case SENSOR_RAM_AVAILABLE:
        return "GB";

    // Throughput
    case SENSOR_DISK_READ:
    case SENSOR_DISK_WRITE:
    case SENSOR_NET_UP:
    case SENSOR_NET_DOWN:
        return "MB/s";

    default:
        return "";
    }
}

const char *hwGetSensorCategory(uint16_t id)
{
    // Extract category from high nibble of low byte (for standard IDs)
    // or check ranges for 16-bit IDs

    if (id >= 0x0001 && id <= 0x000F)
        return "CPU";
    if (id >= 0x0010 && id <= 0x001F)
        return "GPU";
    if (id >= 0x0020 && id <= 0x002F)
        return "RAM";
    if (id >= 0x0030 && id <= 0x003F)
        return "Disk";
    if (id >= 0x0040 && id <= 0x004F)
        return "Network";
    if (id >= 0x0050 && id <= 0x005F)
        return "Motherboard";
    if (id >= 0x0060 && id <= 0x006F)
        return "Battery";
    if (id >= 0x0080 && id <= 0xFFFD)
        return "Custom";

    return "Unknown";
}