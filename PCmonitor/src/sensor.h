#ifndef SENSOR_H
#define SENSOR_H
#include <ArduinoJson.h>

const char *HARDWARE_GPU = "AMD Radeon RX 6800";
const char *HARDWARE_CPU = "AMD Ryzen 5 5600";
const char *HARDWARE_RAM = "Generic Memory";
const char *HARDWARE_MOBO = "ASUS TUF GAMING B550-PLUS";

const char *TYPE_TEMPERATURE = "Temperature";
const char *TYPE_CONTROL = "Control";
const char *TYPE_LOAD = "Load";
const char *TYPE_POWER = "Power";
const char *TYPE_MEMORY = "Memory";
const char *TYPE_DATA = "Data";

// CPU
const char *ITEM_CPU_TOTAL = "CPU Total";
const char *ITEM_CPU_CORE_MAX = "CPU Core Max";
const char *ITEM_CPU_PACKAGE = "Package";
const char *ITEM_CPU_TEMPERATURE = "Core (Tctl/Tdie)";

// GPU
const char *ITEM_GPU_CORE = "GPU Core";
const char *ITEM_GPU_HOT_SPOT = "GPU Hot Spot";
const char *ITEM_GPU_FAN = "GPU Fan";
const char *ITEM_GPU_MEMORY = "GPU Memory";
const char *ITEM_GPU_PACKAGE = "GPU Package";
// RAM
const char *ITEM_MEMORY_USED = "Memory Used";
const char *ITEM_MEMORY_AVAILABLE = "Memory Available";
const char *ITEM_VIRTUAL_MEMORY_USED = "Virtual Memory Used";
const char *ITEM_VIRTUAL_MEMORY_AVAILABLE = "Virtual Memory Available";
const char *ITEM_MEMORY = "Memory";
const char *ITEM_VIRTUAL_MEMORY = "Virtual Memory";

enum Tabs
{
    ALL,
    MOBO,
    CPU,
    GPU,
    RAM
};

struct SensorData
{
    String id;
    String name;
    String hardware;
    String type;
    float value;
    String unit;
};
// Maksymalna liczba sensorów
const int MAX_SENSORS = 30;
SensorData sensors[MAX_SENSORS];
int sensorCount = 0;

// Znajdź sensor po nazwie
SensorData *findSensorByName(const char *name)
{
    for (int i = 0; i < sensorCount; i++)
    {
        if (sensors[i].name.equalsIgnoreCase(name))
        {
            return &sensors[i];
        }
    }
    return nullptr;
}

SensorData *findSensorByName(const char *name, SensorData *filteredData[], int dataSize)
{
    for (int i = 0; i < dataSize; i++)
    {
        if (filteredData[i]->name.equalsIgnoreCase(name))
        {
            return filteredData[i];
        }
    }
    return nullptr;
}

// Znajdź sensor po typie (zwraca pierwszy znaleziony)
SensorData *findSensorByType(const char *type)
{
    for (int i = 0; i < sensorCount; i++)
    {
        if (sensors[i].type.equalsIgnoreCase(type))
        {
            return &sensors[i];
        }
    }
    return nullptr;
}

SensorData *findSensor(const char *name, const char *type, const char *hardware)
{
    for (int i = 0; i < sensorCount; i++)
    { // invert logic and check for null
        if (sensors[i].type.equalsIgnoreCase(type) && sensors[i].hardware.equalsIgnoreCase(hardware) && sensors[i].name.equalsIgnoreCase(name))
        {
            return &sensors[i];
        }
    }
    return nullptr;
}

// Pobierz wszystkie sensory danego typu
int getSensorsByType(const char *type, SensorData *result[], int maxResults)
{
    int count = 0;
    for (int i = 0; i < sensorCount && count < maxResults; i++)
    {
        if (sensors[i].type.equalsIgnoreCase(type))
        {
            result[count++] = &sensors[i];
        }
    }
    return count;
}

int getSensorsByTypeAndHardware(const char *type, const char *hardware, SensorData *result[], int maxResults)
{
    int count = 0;
    for (int i = 0; i < sensorCount && count < maxResults; i++)
    {
        if (sensors[i].type.equalsIgnoreCase(type) && sensors[i].hardware.equalsIgnoreCase(hardware))
        {
            result[count++] = &sensors[i];
        }
    }
    return count;
}
// Pobierz wartość sensora po nazwie (zwraca -999 jeśli nie znaleziono)
float getSensorValue(const char *name)
{
    SensorData *sensor = findSensorByName(name);
    return sensor ? sensor->value : -999.0f;
}

const char *getSensorUnit(const char *name)
{
    SensorData *sensor = findSensorByName(name);
    return sensor ? sensor->unit.c_str() : "";
}

String sensorToString(const SensorData *sensor)
{
    return "ID: " + sensor->id + ", Name: " + sensor->name + ", Hardware: " + sensor->hardware + ", Type: " + sensor->type + ", Value: " + String(sensor->value) + ", Unit: " + sensor->unit;
}

#endif