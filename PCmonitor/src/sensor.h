#ifndef SENSOR_H
#define SENSOR_H
#include <ArduinoJson.h>

const char *GPU_HARDWARE = "AMD Radeon RX 6800";
const char *CPU_HARDWARE = "AMD Ryzen 5 5600X";
const char *RAM_HARDWARE = "Generic Memory";

const char *TEMPERATURE_TYPE = "Temperature";
const char *CONTROL_TYPE = "Control";
const char *LOAD_TYPE = "Load";
const char *POWER_TYPE = "Power";
const char *MEMORY_TYPE = "Memory";
const char *DATA_TYPE = "Data";

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
const int MAX_SENSORS = 20;
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
    {
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

#endif