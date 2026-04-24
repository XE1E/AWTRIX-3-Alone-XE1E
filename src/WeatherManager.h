#ifndef WEATHER_MANAGER_H
#define WEATHER_MANAGER_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Codigos de condicion del clima (OpenWeather)
enum WeatherCondition {
    WEATHER_CLEAR = 0,
    WEATHER_CLOUDS = 1,
    WEATHER_RAIN = 2,
    WEATHER_DRIZZLE = 3,
    WEATHER_THUNDERSTORM = 4,
    WEATHER_SNOW = 5,
    WEATHER_MIST = 6,
    WEATHER_UNKNOWN = 99
};

// Niveles de AQI
enum AQILevel {
    AQI_GOOD = 0,           // 0-50: Verde
    AQI_MODERATE = 1,       // 51-100: Amarillo
    AQI_SENSITIVE = 2,      // 101-150: Naranja
    AQI_UNHEALTHY = 3,      // 151-200: Rojo
    AQI_VERY_UNHEALTHY = 4, // 201-300: Morado
    AQI_HAZARDOUS = 5       // 301+: Marron
};

class WeatherManager_ {
public:
    void setup();
    void tick();

    // Forzar actualizacion
    void fetchWeather();
    void fetchAQI();

    // Datos del clima
    float temperature = 0;
    float feelsLike = 0;
    int humidity = 0;
    float windSpeed = 0;
    int pressure = 0;
    int clouds = 0;
    int weatherCode = 0;
    WeatherCondition condition = WEATHER_UNKNOWN;
    String description = "";
    String icon = "";

    // Datos de AQI
    int aqi = 0;
    AQILevel aqiLevel = AQI_GOOD;
    float pm25 = 0;
    float pm10 = 0;
    float o3 = 0;
    float no2 = 0;
    float co = 0;

    // Estado
    bool weatherValid = false;
    bool aqiValid = false;
    unsigned long lastWeatherUpdate = 0;
    unsigned long lastAQIUpdate = 0;

    // Configuracion (se carga desde Globals)
    String apiKey = "";
    String latitude = "";
    String longitude = "";
    String units = "metric";  // metric, imperial
    String lang = "es";

    // Helpers
    uint32_t getAQIColor();
    String getAQIText();
    String getWeatherIconName();

private:
    static const unsigned long WEATHER_UPDATE_INTERVAL = 900000;  // 15 minutos
    static const unsigned long AQI_UPDATE_INTERVAL = 900000;      // 15 minutos

    void parseWeatherResponse(String &response);
    void parseAQIResponse(String &response);
    int calculateAQI(float pm25);
    WeatherCondition codeToCondition(int code);
};

extern WeatherManager_ WeatherManager;

#endif
