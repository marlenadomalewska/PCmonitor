/**
 * @file HWMonitor. h
 * @brief Hardware Monitor Protocol Parser Library
 * @version 2.0 - 16-bit sensor IDs
 * @author Auto-generated
 *
 * Universal library for parsing PC hardware sensor data.
 * Compatible with:  Arduino, ESP32, ESP8266, STM32, Teensy, etc.
 *
 * Protocol v2 changes:
 *   - Sensor IDs are now 16-bit (2 bytes) instead of 8-bit
 *   - Reserved bytes (0xAA, 0x55) cannot be part of sensor ID
 *   - Packet structure:  [START][VERSION][COUNT][ID_HI][ID_LO][VALUE x4].. .[CRC16][END]
 *
 * Usage:
 *   1. Create instance:  HWMonitor monitor;
 *   2. In setup(): monitor.begin();
 *   3. In loop(): monitor.update(Serial);  // or any Stream
 *   4. Read values: float temp = monitor.get(SENSOR_CPU_TEMP);
 */

#ifndef HW_MONITOR_H
#define HW_MONITOR_H

#include <Arduino.h>

/*===========================================================================*/
/*  CONFIGURATION                                                            */
/*===========================================================================*/

#ifndef HW_MAX_SENSORS
#define HW_MAX_SENSORS 250
#endif

#ifndef HW_RX_BUFFER_SIZE
#define HW_RX_BUFFER_SIZE 2048
#endif

#ifndef HW_TIMEOUT_MS
#define HW_TIMEOUT_MS 5000
#endif

/*===========================================================================*/
/*  PROTOCOL CONSTANTS                                                       */
/*===========================================================================*/

#define HW_PROTO_START 0xAA
#define HW_PROTO_END 0x55
#define HW_PROTO_VERSION 0x02 // Protocol v2: 16-bit sensor IDs

// Reserved bytes - cannot be part of sensor ID
#define HW_RESERVED_START 0xAA
#define HW_RESERVED_END 0x55

// Sensor data size:  2 bytes ID + 4 bytes float
#define HW_SENSOR_DATA_SIZE 6

/*===========================================================================*/
/*  SENSOR IDs (16-bit)                                                      */
/*===========================================================================*/

// CPU Sensors (0x0001 - 0x000F)
#define SENSOR_CPU_TEMP 0x0001
#define SENSOR_CPU_LOAD 0x0002
#define SENSOR_CPU_CLOCK 0x0003
#define SENSOR_CPU_POWER 0x0004
#define SENSOR_CPU_TEMP_CORE 0x0005
#define SENSOR_CPU_LOAD_CORE 0x0006
#define SENSOR_CPU_POWER_CORE 0x0007
#define SENSOR_CPU_TEMP_CCD 0x0008
#define SENSOR_CPU_VOLTAGE 0x0009

// GPU Sensors (0x0010 - 0x001F)
#define SENSOR_GPU_TEMP 0x0010
#define SENSOR_GPU_LOAD 0x0011
#define SENSOR_GPU_CLOCK 0x0012
#define SENSOR_GPU_CLOCK_MEM 0x0013
#define SENSOR_GPU_POWER 0x0014
#define SENSOR_GPU_LOAD_MEM 0x0015
#define SENSOR_GPU_FAN 0x0016
#define SENSOR_GPU_TEMP_MEM 0x0017
#define SENSOR_GPU_HOTSPOT 0x0018
#define SENSOR_GPU_LOAD_VIDEO 0x0019

// RAM Sensors (0x0020 - 0x002F)
#define SENSOR_RAM_USED 0x0020
#define SENSOR_RAM_AVAILABLE 0x0021
#define SENSOR_RAM_LOAD 0x0022

// Disk Sensors (0x0030 - 0x003F)
#define SENSOR_DISK_TEMP 0x0030
#define SENSOR_DISK_LOAD 0x0031
#define SENSOR_DISK_READ 0x0032
#define SENSOR_DISK_WRITE 0x0033

// Network Sensors (0x0040 - 0x004F)
#define SENSOR_NET_UP 0x0040
#define SENSOR_NET_DOWN 0x0041

// Motherboard Sensors (0x0050 - 0x005F)
#define SENSOR_MB_TEMP 0x0050
#define SENSOR_MB_FAN1 0x0051
#define SENSOR_MB_FAN2 0x0052
#define SENSOR_MB_FAN3 0x0053
#define SENSOR_MB_FAN4 0x0054
#define SENSOR_MB_VOLTAGE 0x0056 // Changed from 0x55 (reserved!)

// Battery Sensors (0x0060 - 0x006F)
#define SENSOR_BATTERY_LEVEL 0x0060
#define SENSOR_BATTERY_VOLTAGE 0x0061
#define SENSOR_BATTERY_RATE 0x0062

// Invalid/Unknown
#define SENSOR_UNKNOWN 0xFFFF

/*===========================================================================*/
/*  DATA STRUCTURES                                                          */
/*===========================================================================*/

/**
 * @brief Single sensor data
 */
struct HWSensor
{
    uint16_t id; // 16-bit sensor ID
    float value;
    bool valid;
    uint32_t timestamp;
};

/**
 * @brief Parser state machine states
 */
enum HWParserState
{
    HW_STATE_IDLE,
    HW_STATE_VERSION,
    HW_STATE_COUNT,
    HW_STATE_ID_HIGH, // Reading high byte of sensor ID
    HW_STATE_ID_LOW,  // Reading low byte of sensor ID
    HW_STATE_VALUE,   // Reading 4-byte float value
    HW_STATE_CRC_LOW,
    HW_STATE_CRC_HIGH,
    HW_STATE_END
};

/**
 * @brief Callback function type for new packet
 */
typedef void (*HWPacketCallback)(uint8_t sensorCount);

/**
 * @brief Callback function type for sensor update
 */
typedef void (*HWSensorCallback)(uint16_t id, float value);

/*===========================================================================*/
/*  MAIN CLASS                                                               */
/*===========================================================================*/

class HWMonitor
{
public:
    /**
     * @brief Constructor
     */
    HWMonitor();

    /**
     * @brief Initialize the monitor
     */
    void begin();

    /**
     * @brief Reset all data
     */
    void reset();

    /**
     * @brief Update from a Stream (Serial, etc.)
     * @param stream Reference to input stream
     * @return true if a complete packet was parsed
     */
    bool update(Stream &stream);

    /**
     * @brief Process a single byte
     * @param byte Incoming byte
     * @return true if a complete packet was parsed
     */
    bool processByte(uint8_t byte);

    /**
     * @brief Parse a complete buffer
     * @param data Pointer to data
     * @param len Length of data
     * @return true if valid packet was parsed
     */
    bool parse(const uint8_t *data, size_t len);

    /**
     * @brief Get sensor value by ID
     * @param id 16-bit Sensor ID
     * @param defaultValue Value to return if sensor not found
     * @return Sensor value or defaultValue
     */
    float get(uint16_t id, float defaultValue = -999.0f) const;

    /**
     * @brief Check if sensor has valid data
     * @param id 16-bit Sensor ID
     * @return true if sensor data is valid
     */
    bool isValid(uint16_t id) const;

    /**
     * @brief Get sensor by index
     * @param index Sensor index (0 to sensorCount-1)
     * @return Pointer to sensor or nullptr
     */
    const HWSensor *getSensorByIndex(uint8_t index) const;

    /**
     * @brief Find sensor by ID
     * @param id 16-bit Sensor ID
     * @return Pointer to sensor or nullptr
     */
    const HWSensor *findSensor(uint16_t id) const;

    /**
     * @brief Invalidate all sensors (call on timeout)
     */
    void invalidateAll();

    /**
     * @brief Check if data is stale
     * @param timeoutMs Timeout in milliseconds
     * @return true if last update was more than timeoutMs ago
     */
    bool isStale(uint32_t timeoutMs = HW_TIMEOUT_MS) const;

    /**
     * @brief Get time since last update
     * @return Milliseconds since last valid packet
     */
    uint32_t getAge() const;

    /**
     * @brief Set callback for new packet
     * @param callback Function to call when packet is received
     */
    void onPacket(HWPacketCallback callback);

    /**
     * @brief Set callback for sensor update
     * @param callback Function to call for each sensor
     */
    void onSensor(HWSensorCallback callback);

    /**
     * @brief Check if sensor ID contains reserved bytes
     * @param id 16-bit Sensor ID
     * @return true if ID is invalid (contains 0xAA or 0x55)
     */
    static bool isReservedId(uint16_t id);

    // Convenience getters
    inline float getCpuTemp() const { return get(SENSOR_CPU_TEMP); }
    inline float getCpuLoad() const { return get(SENSOR_CPU_LOAD); }
    inline float getCpuClock() const { return get(SENSOR_CPU_CLOCK); }
    inline float getCpuPower() const { return get(SENSOR_CPU_POWER); }
    inline float getCpuVoltage() const { return get(SENSOR_CPU_VOLTAGE); }

    inline float getGpuTemp() const { return get(SENSOR_GPU_TEMP); }
    inline float getGpuLoad() const { return get(SENSOR_GPU_LOAD); }
    inline float getGpuClock() const { return get(SENSOR_GPU_CLOCK); }
    inline float getGpuPower() const { return get(SENSOR_GPU_POWER); }
    inline float getGpuFan() const { return get(SENSOR_GPU_FAN); }
    inline float getGpuHotspot() const { return get(SENSOR_GPU_HOTSPOT); }
    inline float getGpuMemLoad() const { return get(SENSOR_GPU_LOAD_MEM); }
    inline float getGpuMemTemp() const { return get(SENSOR_GPU_TEMP_MEM); }

    inline float getRamUsed() const { return get(SENSOR_RAM_USED); }
    inline float getRamAvailable() const { return get(SENSOR_RAM_AVAILABLE); }
    inline float getRamLoad() const { return get(SENSOR_RAM_LOAD); }

    inline float getDiskTemp() const { return get(SENSOR_DISK_TEMP); }
    inline float getDiskLoad() const { return get(SENSOR_DISK_LOAD); }
    inline float getDiskRead() const { return get(SENSOR_DISK_READ); }
    inline float getDiskWrite() const { return get(SENSOR_DISK_WRITE); }

    inline float getNetUp() const { return get(SENSOR_NET_UP); }
    inline float getNetDown() const { return get(SENSOR_NET_DOWN); }

    inline float getMbTemp() const { return get(SENSOR_MB_TEMP); }

    inline float getBatteryLevel() const { return get(SENSOR_BATTERY_LEVEL); }

    // Statistics
    uint32_t packetsOK;
    uint32_t packetsError;
    uint8_t sensorCount;
    uint32_t lastUpdate;

private:
    HWSensor _sensors[HW_MAX_SENSORS];
    HWParserState _state;
    uint8_t _expectedCount;
    uint8_t _currentSensor;
    uint8_t _byteInValue;  // Byte counter within 4-byte float value
    uint16_t _tempId;      // 16-bit sensor ID being parsed
    uint8_t _tempIdHigh;   // High byte of sensor ID
    uint8_t _tempValue[4]; // 4-byte float value buffer
    uint8_t _crcLow;
    uint8_t _crcHigh;

    HWPacketCallback _packetCallback;
    HWSensorCallback _sensorCallback;

    void _storeSensor();
    bool _finalizePacket();
    uint16_t _calculateCRC(const uint8_t *data, size_t len) const;
};

/*===========================================================================*/
/*  UTILITY FUNCTIONS                                                        */
/*===========================================================================*/

/**
 * @brief Get sensor name string
 * @param id 16-bit Sensor ID
 * @return Human-readable sensor name
 */
const char *hwGetSensorName(uint16_t id);

/**
 * @brief Get sensor unit string
 * @param id 16-bit Sensor ID
 * @return Unit string (°C, %, MHz, etc.)
 */
const char *hwGetSensorUnit(uint16_t id);

/**
 * @brief Get sensor category
 * @param id 16-bit Sensor ID
 * @return Category string (CPU, GPU, RAM, etc.)
 */
const char *hwGetSensorCategory(uint16_t id);

/**
 * @brief Check if sensor ID contains reserved bytes
 * @param id 16-bit Sensor ID
 * @return true if ID is invalid
 */
inline bool hwIsReservedId(uint16_t id)
{
    uint8_t high = (uint8_t)(id >> 8);
    uint8_t low = (uint8_t)(id & 0xFF);
    return high == HW_RESERVED_START || high == HW_RESERVED_END ||
           low == HW_RESERVED_START || low == HW_RESERVED_END;
}

#endif // HW_MONITOR_H