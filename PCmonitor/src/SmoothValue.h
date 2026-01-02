/**
 * @file SmoothValue.h
 * @brief Klasa do wygładzania wartości sensorów (EMA - Exponential Moving Average)
 *
 * Zapewnia płynne przejścia między wartościami poprzez interpolację
 */

#ifndef SMOOTH_VALUE_H
#define SMOOTH_VALUE_H

#include <Arduino.h>

/**
 * @brief Pojedyncza wygładzona wartość
 */
class SmoothValue
{
public:
    SmoothValue(float smoothing = 0.15f)
        : m_current(0.0f), m_target(0.0f), m_smoothing(smoothing), m_initialized(false)
    {
    }

    /**
     * @brief Ustaw nową wartość docelową
     */
    void setTarget(float value)
    {
        if (!m_initialized)
        {
            m_current = value;
            m_initialized = true;
        }
        m_target = value;
    }

    /**
     * @brief Aktualizuj wartość bieżącą (interpolacja w kierunku celu)
     * @return Aktualna wygładzona wartość
     */
    float update()
    {
        if (!m_initialized)
            return 0.0f;
        m_current += (m_target - m_current) * m_smoothing;
        return m_current;
    }

    /**
     * @brief Pobierz aktualną wygładzoną wartość bez aktualizacji
     */
    float get() const { return m_current; }

    /**
     * @brief Pobierz wartość docelową
     */
    float getTarget() const { return m_target; }

    /**
     * @brief Zmień współczynnik wygładzania
     * @param smoothing Wartość 0.0-1.0 (mniejsza = wolniejsze przejścia)
     */
    void setSmoothing(float smoothing)
    {
        m_smoothing = constrain(smoothing, 0.01f, 1.0f);
    }

    /**
     * @brief Reset do stanu początkowego
     */
    void reset()
    {
        m_current = 0.0f;
        m_target = 0.0f;
        m_initialized = false;
    }

    /**
     * @brief Natychmiast ustaw wartość (bez interpolacji)
     */
    void setImmediate(float value)
    {
        m_current = value;
        m_target = value;
        m_initialized = true;
    }

private:
    float m_current;
    float m_target;
    float m_smoothing;
    bool m_initialized;
};

/**
 * @brief Manager wygładzonych wartości dla wielu sensorów
 */
template <size_t MAX_SENSORS = 32>
class SmoothValueManager
{
public:
    SmoothValueManager(float defaultSmoothing = 0.15f)
        : m_defaultSmoothing(defaultSmoothing), m_count(0)
    {
        for (size_t i = 0; i < MAX_SENSORS; i++)
        {
            m_ids[i] = 0xFF;
        }
    }

    /**
     * @brief Ustaw wartość docelową dla sensora
     */
    void setTarget(uint8_t sensorId, float value)
    {
        int idx = findOrCreate(sensorId);
        if (idx >= 0)
        {
            m_values[idx].setTarget(value);
        }
    }

    /**
     * @brief Aktualizuj wszystkie wartości (interpolacja)
     */
    void updateAll()
    {
        for (size_t i = 0; i < m_count; i++)
        {
            m_values[i].update();
        }
    }

    /**
     * @brief Pobierz wygładzoną wartość dla sensora
     */
    float get(uint8_t sensorId, float defaultValue = 0.0f) const
    {
        int idx = find(sensorId);
        if (idx >= 0)
        {
            return m_values[idx].get();
        }
        return defaultValue;
    }

    /**
     * @brief Pobierz wygładzoną wartość jako int
     */
    int getInt(uint8_t sensorId, int defaultValue = 0) const
    {
        int idx = find(sensorId);
        if (idx >= 0)
        {
            return static_cast<int>(std::round(m_values[idx].get()));
        }
        return defaultValue;
    }

    /**
     * @brief Zmień współczynnik wygładzania dla sensora
     */
    void setSmoothing(uint8_t sensorId, float smoothing)
    {
        int idx = find(sensorId);
        if (idx >= 0)
        {
            m_values[idx].setSmoothing(smoothing);
        }
    }

    /**
     * @brief Zmień domyślny współczynnik wygładzania
     */
    void setDefaultSmoothing(float smoothing)
    {
        m_defaultSmoothing = constrain(smoothing, 0.01f, 1.0f);
    }

    /**
     * @brief Reset wszystkich wartości
     */
    void reset()
    {
        for (size_t i = 0; i < m_count; i++)
        {
            m_values[i].reset();
            m_ids[i] = 0xFF;
        }
        m_count = 0;
    }

private:
    int find(uint8_t sensorId) const
    {
        for (size_t i = 0; i < m_count; i++)
        {
            if (m_ids[i] == sensorId)
                return i;
        }
        return -1;
    }

    int findOrCreate(uint8_t sensorId)
    {
        int idx = find(sensorId);
        if (idx >= 0)
            return idx;

        if (m_count < MAX_SENSORS)
        {
            m_ids[m_count] = sensorId;
            m_values[m_count] = SmoothValue(m_defaultSmoothing);
            return m_count++;
        }
        return -1;
    }

    SmoothValue m_values[MAX_SENSORS];
    uint8_t m_ids[MAX_SENSORS];
    float m_defaultSmoothing;
    size_t m_count;
};

#endif // SMOOTH_VALUE_H