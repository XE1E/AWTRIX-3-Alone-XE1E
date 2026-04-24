#include "WeatherManager.h"
#include "Globals.h"

WeatherManager_ WeatherManager;

void WeatherManager_::setup() {
    // Cargar configuracion desde Globals
    // apiKey = WEATHER_API_KEY;
    // latitude = WEATHER_LAT;
    // longitude = WEATHER_LON;

    DEBUG_PRINTLN("WeatherManager initialized");
}

void WeatherManager_::tick() {
    unsigned long now = millis();

    // Verificar si hay API key configurada
    if (apiKey.length() == 0) {
        return;
    }

    // Actualizar clima cada 15 minutos
    if (now - lastWeatherUpdate > WEATHER_UPDATE_INTERVAL || lastWeatherUpdate == 0) {
        fetchWeather();
    }

    // Actualizar AQI cada 15 minutos
    if (now - lastAQIUpdate > AQI_UPDATE_INTERVAL || lastAQIUpdate == 0) {
        fetchAQI();
    }
}

void WeatherManager_::fetchWeather() {
    if (apiKey.length() == 0 || latitude.length() == 0 || longitude.length() == 0) {
        DEBUG_PRINTLN("Weather: Missing API key or coordinates");
        return;
    }

    HTTPClient http;
    String url = "http://api.openweathermap.org/data/2.5/weather?lat=" + latitude +
                 "&lon=" + longitude +
                 "&appid=" + apiKey +
                 "&units=" + units +
                 "&lang=" + lang;

    DEBUG_PRINTLN("Fetching weather from: " + url);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        parseWeatherResponse(response);
        weatherValid = true;
        lastWeatherUpdate = millis();
        DEBUG_PRINTLN("Weather updated successfully");
    } else {
        DEBUG_PRINTF("Weather HTTP error: %d", httpCode);
        weatherValid = false;
    }

    http.end();
}

void WeatherManager_::fetchAQI() {
    if (apiKey.length() == 0 || latitude.length() == 0 || longitude.length() == 0) {
        DEBUG_PRINTLN("AQI: Missing API key or coordinates");
        return;
    }

    HTTPClient http;
    String url = "http://api.openweathermap.org/data/2.5/air_pollution?lat=" + latitude +
                 "&lon=" + longitude +
                 "&appid=" + apiKey;

    DEBUG_PRINTLN("Fetching AQI from: " + url);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        parseAQIResponse(response);
        aqiValid = true;
        lastAQIUpdate = millis();
        DEBUG_PRINTLN("AQI updated successfully");
    } else {
        DEBUG_PRINTF("AQI HTTP error: %d", httpCode);
        aqiValid = false;
    }

    http.end();
}

void WeatherManager_::parseWeatherResponse(String &response) {
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
        DEBUG_PRINTLN("Weather JSON parse error");
        return;
    }

    // Extraer datos principales
    temperature = doc["main"]["temp"] | 0.0;
    feelsLike = doc["main"]["feels_like"] | 0.0;
    humidity = doc["main"]["humidity"] | 0;
    pressure = doc["main"]["pressure"] | 0;
    windSpeed = doc["wind"]["speed"] | 0.0;
    clouds = doc["clouds"]["all"] | 0;

    // Extraer condicion del clima
    JsonArray weather = doc["weather"];
    if (weather.size() > 0) {
        weatherCode = weather[0]["id"] | 0;
        description = weather[0]["description"].as<String>();
        icon = weather[0]["icon"].as<String>();
        condition = codeToCondition(weatherCode);
    }
}

void WeatherManager_::parseAQIResponse(String &response) {
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
        DEBUG_PRINTLN("AQI JSON parse error");
        return;
    }

    JsonArray list = doc["list"];
    if (list.size() > 0) {
        JsonObject components = list[0]["components"];

        pm25 = components["pm2_5"] | 0.0;
        pm10 = components["pm10"] | 0.0;
        o3 = components["o3"] | 0.0;
        no2 = components["no2"] | 0.0;
        co = components["co"] | 0.0;

        // Calcular AQI basado en PM2.5 (escala US EPA)
        aqi = calculateAQI(pm25);

        // Determinar nivel
        if (aqi <= 50) aqiLevel = AQI_GOOD;
        else if (aqi <= 100) aqiLevel = AQI_MODERATE;
        else if (aqi <= 150) aqiLevel = AQI_SENSITIVE;
        else if (aqi <= 200) aqiLevel = AQI_UNHEALTHY;
        else if (aqi <= 300) aqiLevel = AQI_VERY_UNHEALTHY;
        else aqiLevel = AQI_HAZARDOUS;
    }
}

int WeatherManager_::calculateAQI(float pm25_value) {
    // Breakpoints de PM2.5 a AQI (escala US EPA)
    struct Breakpoint {
        float pmLow, pmHigh;
        int aqiLow, aqiHigh;
    };

    Breakpoint breakpoints[] = {
        {0.0, 12.0, 0, 50},
        {12.1, 35.4, 51, 100},
        {35.5, 55.4, 101, 150},
        {55.5, 150.4, 151, 200},
        {150.5, 250.4, 201, 300},
        {250.5, 500.4, 301, 500}
    };

    for (int i = 0; i < 6; i++) {
        if (pm25_value >= breakpoints[i].pmLow && pm25_value <= breakpoints[i].pmHigh) {
            float aqi_value = ((float)(breakpoints[i].aqiHigh - breakpoints[i].aqiLow) /
                              (breakpoints[i].pmHigh - breakpoints[i].pmLow)) *
                             (pm25_value - breakpoints[i].pmLow) + breakpoints[i].aqiLow;
            return (int)round(aqi_value);
        }
    }

    return 500;  // Fuera de escala
}

WeatherCondition WeatherManager_::codeToCondition(int code) {
    // OpenWeather condition codes: https://openweathermap.org/weather-conditions
    if (code >= 200 && code < 300) return WEATHER_THUNDERSTORM;
    if (code >= 300 && code < 400) return WEATHER_DRIZZLE;
    if (code >= 500 && code < 600) return WEATHER_RAIN;
    if (code >= 600 && code < 700) return WEATHER_SNOW;
    if (code >= 700 && code < 800) return WEATHER_MIST;
    if (code == 800) return WEATHER_CLEAR;
    if (code > 800) return WEATHER_CLOUDS;
    return WEATHER_UNKNOWN;
}

uint32_t WeatherManager_::getAQIColor() {
    switch (aqiLevel) {
        case AQI_GOOD:          return 0x00E400;  // Verde
        case AQI_MODERATE:      return 0xFFFF00;  // Amarillo
        case AQI_SENSITIVE:     return 0xFF7E00;  // Naranja
        case AQI_UNHEALTHY:     return 0xFF0000;  // Rojo
        case AQI_VERY_UNHEALTHY: return 0x8F3F97; // Morado
        case AQI_HAZARDOUS:     return 0x7E0023;  // Marron
        default:                return 0xFFFFFF;  // Blanco
    }
}

String WeatherManager_::getAQIText() {
    switch (aqiLevel) {
        case AQI_GOOD:          return "Bueno";
        case AQI_MODERATE:      return "Moderado";
        case AQI_SENSITIVE:     return "Sensibles";
        case AQI_UNHEALTHY:     return "Insalubre";
        case AQI_VERY_UNHEALTHY: return "Muy insalubre";
        case AQI_HAZARDOUS:     return "Peligroso";
        default:                return "Desconocido";
    }
}

String WeatherManager_::getWeatherIconName() {
    // Mapear iconos de OpenWeather a nombres de archivo local
    // Los iconos se guardaran en el filesystem del ESP32
    switch (condition) {
        case WEATHER_CLEAR:       return "weather_clear";
        case WEATHER_CLOUDS:      return "weather_clouds";
        case WEATHER_RAIN:        return "weather_rain";
        case WEATHER_DRIZZLE:     return "weather_drizzle";
        case WEATHER_THUNDERSTORM: return "weather_storm";
        case WEATHER_SNOW:        return "weather_snow";
        case WEATHER_MIST:        return "weather_mist";
        default:                  return "weather_unknown";
    }
}
