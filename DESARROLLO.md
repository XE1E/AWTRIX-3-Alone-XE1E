# AWTRIX 3 Alone - Guia de Desarrollo XE1E

## Repositorio
- **GitHub:** https://github.com/XE1E/AWTRIX-3-Alone-XE1E
- **Local:** `D:\Documents\Raspi\AWTRIX-3-Alone-XE1E`
- **Basado en:** AWTRIX 3 por Blueforcer

---

## Objetivo del Proyecto

Crear una version de AWTRIX 3 que sea 100% standalone:
- Todo corre en el ESP32
- Sin necesidad de Node-RED, Home Assistant, ni servidores externos
- Solo requiere WiFi y API key de OpenWeather (gratis)

---

## Funcionalidades a Implementar

| Funcion | Modulo | Estado | Fuente de datos |
|---------|--------|--------|-----------------|
| Reloj | Ya existe | Listo | NTP |
| Fecha/Calendario | Ya existe | Listo | Local |
| Temperatura local | Ya existe | Listo | Sensor BME280/SHT31 |
| Humedad local | Ya existe | Listo | Sensor BME280/SHT31 |
| **Clima online** | WeatherManager | Creado | OpenWeather API |
| **ICA/AQI** | WeatherManager | Creado | OpenWeather API |
| **Fases lunares** | MoonPhase | Creado | Calculo local |
| Animaciones/GIFs | Ya existe | Listo | Filesystem |

---

## Hardware Soportado

| Dispositivo | Estado |
|-------------|--------|
| Ulanzi TC001 | Soportado |
| ESP32 + Matriz WS2812B custom | Soportado |

## Conexiones (Build Custom)

```
                    ESP32                         MATRIZ WS2812B 32x8
                 +---------+                      +------------------+
                 |         |                      |                  |
    USB -------->|   USB   |                      |   32 x 8 LEDs    |
                 |         |                      |                  |
                 |  GPIO32 |---[330R]------------>| DIN (Data In)    |
                 |         |                      |                  |
                 |    GND  |--------------------->| GND              |
                 |         |                      |                  |
                 |    5V   |--------------------->| 5V / VCC         |
                 +---------+                      +------------------+
                      |                                   |
                      +----------- GND comun -------------+
                                      |
                               [1000uF cap]
                                      |
                              Fuente 5V 4A
```

### Conexiones opcionales

| ESP32 GPIO | Componente | Descripcion |
|------------|------------|-------------|
| GPIO35 | LDR (fotoresistor) | Brillo automatico |
| GPIO26 | Boton izquierdo | Navegacion |
| GPIO27 | Boton central | Seleccionar |
| GPIO14 | Boton derecho | Navegacion |
| GPIO21 (SDA) | Sensor temp | BME280/SHT31 |
| GPIO22 (SCL) | Sensor temp | I2C Clock |

---

## Archivos Nuevos Creados

### src/WeatherManager.h / .cpp
Cliente HTTP para OpenWeather API.

**Datos que obtiene:**
- Temperatura actual y sensacion termica
- Humedad, presion, viento
- Condicion del clima (soleado, nublado, lluvia, etc.)
- PM2.5, PM10, O3, NO2, CO (contaminantes)
- Calculo de AQI segun escala US EPA

**Uso:**
```cpp
#include "WeatherManager.h"

// En setup()
WeatherManager.apiKey = "tu_api_key";
WeatherManager.latitude = "19.4326";
WeatherManager.longitude = "-99.1332";
WeatherManager.setup();

// En loop()
WeatherManager.tick();  // Actualiza cada 15 min automaticamente

// Acceder a datos
float temp = WeatherManager.temperature;
int aqi = WeatherManager.aqi;
uint32_t color = WeatherManager.getAQIColor();
```

### src/MoonPhase.h / .cpp
Calculo matematico de fases lunares (sin API externa).

**Datos que calcula:**
- Fase actual (nueva, creciente, llena, menguante, etc.)
- Dia del ciclo (0-29)
- Porcentaje de iluminacion
- Dias hasta proxima luna llena/nueva

**Uso:**
```cpp
#include "MoonPhase.h"

// En setup()
MoonPhase.setup();

// En loop()
MoonPhase.tick();  // Actualiza cada hora

// Acceder a datos
String fase = MoonPhase.phaseName;  // "Luna Llena"
int illumination = MoonPhase.illumination;  // 0-100%
String icono = MoonPhase.getIconName();  // "moon_full"
```

---

## Pasos de Integracion

### Paso 1: Agregar variables de configuracion

**Archivo:** `src/Globals.h`

Agregar despues de las otras variables externas:
```cpp
// Weather API Configuration
extern String WEATHER_API_KEY;
extern String WEATHER_LAT;
extern String WEATHER_LON;
extern String WEATHER_UNITS;
extern String WEATHER_LANG;
extern bool SHOW_WEATHER;
extern bool SHOW_AQI;
extern bool SHOW_MOON;
```

**Archivo:** `src/Globals.cpp`

Agregar las definiciones:
```cpp
// Weather API Configuration
String WEATHER_API_KEY = "";
String WEATHER_LAT = "19.4326";
String WEATHER_LON = "-99.1332";
String WEATHER_UNITS = "metric";
String WEATHER_LANG = "es";
bool SHOW_WEATHER = true;
bool SHOW_AQI = true;
bool SHOW_MOON = true;
```

### Paso 2: Integrar en main.cpp

**Archivo:** `src/main.cpp`

Agregar includes:
```cpp
#include "WeatherManager.h"
#include "MoonPhase.h"
```

En `setup()`, despues de `ServerManager.setup()`:
```cpp
// Configurar Weather Manager
WeatherManager.apiKey = WEATHER_API_KEY;
WeatherManager.latitude = WEATHER_LAT;
WeatherManager.longitude = WEATHER_LON;
WeatherManager.units = WEATHER_UNITS;
WeatherManager.lang = WEATHER_LANG;
WeatherManager.setup();

// Configurar Moon Phase
MoonPhase.setup();
```

En `loop()`:
```cpp
WeatherManager.tick();
MoonPhase.tick();
```

### Paso 3: Crear las Apps visuales

**Archivo:** `src/Apps.h`

Agregar declaraciones:
```cpp
void WeatherApp(FastLED_NeoMatrix *matrix, MatrixDisplayUiState *state, int16_t x, int16_t y, GifPlayer *gifPlayer);
void AQIApp(FastLED_NeoMatrix *matrix, MatrixDisplayUiState *state, int16_t x, int16_t y, GifPlayer *gifPlayer);
void MoonApp(FastLED_NeoMatrix *matrix, MatrixDisplayUiState *state, int16_t x, int16_t y, GifPlayer *gifPlayer);
```

**Archivo:** `src/Apps.cpp`

Agregar includes:
```cpp
#include "WeatherManager.h"
#include "MoonPhase.h"
```

Agregar las funciones de las apps:
```cpp
void WeatherApp(FastLED_NeoMatrix *matrix, MatrixDisplayUiState *state, int16_t x, int16_t y, GifPlayer *gifPlayer)
{
    if (!WeatherManager.weatherValid) {
        DisplayManager.printText(x + 7, y + 6, "Weather...", TEXT_COLOR, false);
        return;
    }

    // Mostrar icono del clima (posicion x, y)
    // TODO: Cargar icono desde filesystem

    // Mostrar temperatura
    String tempStr = String((int)round(WeatherManager.temperature)) + "C";
    DisplayManager.printText(x + 12, y + 6, tempStr.c_str(), TEXT_COLOR, false);
}

void AQIApp(FastLED_NeoMatrix *matrix, MatrixDisplayUiState *state, int16_t x, int16_t y, GifPlayer *gifPlayer)
{
    if (!WeatherManager.aqiValid) {
        DisplayManager.printText(x + 7, y + 6, "AQI...", TEXT_COLOR, false);
        return;
    }

    // Mostrar AQI con color segun nivel
    String aqiStr = "ICA " + String(WeatherManager.aqi);
    uint32_t color = WeatherManager.getAQIColor();
    DisplayManager.printText(x + 7, y + 6, aqiStr.c_str(), color, false);
}

void MoonApp(FastLED_NeoMatrix *matrix, MatrixDisplayUiState *state, int16_t x, int16_t y, GifPlayer *gifPlayer)
{
    // Mostrar icono de la luna
    // TODO: Cargar icono desde filesystem

    // Mostrar nombre de la fase
    DisplayManager.printText(x + 12, y + 6, MoonPhase.phaseName.c_str(), MoonPhase.getPhaseColor(), false);
}
```

### Paso 4: Registrar las Apps en el sistema

**Archivo:** `src/DisplayManager.cpp`

En la funcion `loadNativeApps()`, agregar:
```cpp
if (SHOW_WEATHER) {
    Apps.push_back(std::make_pair("Weather", WeatherApp));
}
if (SHOW_AQI) {
    Apps.push_back(std::make_pair("AQI", AQIApp));
}
if (SHOW_MOON) {
    Apps.push_back(std::make_pair("Moon", MoonApp));
}
```

### Paso 5: Agregar configuracion en interfaz web

**Archivo:** `src/ServerManager.cpp`

Agregar endpoints para configurar la API key y coordenadas.

En la funcion que maneja `/api/settings` (GET):
```cpp
doc["WEATHER_API_KEY"] = WEATHER_API_KEY;
doc["WEATHER_LAT"] = WEATHER_LAT;
doc["WEATHER_LON"] = WEATHER_LON;
doc["SHOW_WEATHER"] = SHOW_WEATHER;
doc["SHOW_AQI"] = SHOW_AQI;
doc["SHOW_MOON"] = SHOW_MOON;
```

En la funcion que maneja `/api/settings` (POST):
```cpp
if (doc.containsKey("WEATHER_API_KEY")) {
    WEATHER_API_KEY = doc["WEATHER_API_KEY"].as<String>();
    WeatherManager.apiKey = WEATHER_API_KEY;
}
if (doc.containsKey("WEATHER_LAT")) {
    WEATHER_LAT = doc["WEATHER_LAT"].as<String>();
    WeatherManager.latitude = WEATHER_LAT;
}
// ... etc
```

### Paso 6: Persistir configuracion

**Archivo:** `src/Globals.cpp`

En `saveSettings()`:
```cpp
doc["WEATHER_API_KEY"] = WEATHER_API_KEY;
doc["WEATHER_LAT"] = WEATHER_LAT;
doc["WEATHER_LON"] = WEATHER_LON;
doc["SHOW_WEATHER"] = SHOW_WEATHER;
doc["SHOW_AQI"] = SHOW_AQI;
doc["SHOW_MOON"] = SHOW_MOON;
```

En `loadSettings()`:
```cpp
WEATHER_API_KEY = doc["WEATHER_API_KEY"] | "";
WEATHER_LAT = doc["WEATHER_LAT"] | "19.4326";
WEATHER_LON = doc["WEATHER_LON"] | "-99.1332";
SHOW_WEATHER = doc["SHOW_WEATHER"] | true;
SHOW_AQI = doc["SHOW_AQI"] | true;
SHOW_MOON = doc["SHOW_MOON"] | true;
```

---

## Iconos Necesarios

Crear iconos de 8x8 pixeles para el filesystem:

### Clima
| Archivo | Descripcion |
|---------|-------------|
| weather_clear.gif | Sol |
| weather_clouds.gif | Nubes |
| weather_rain.gif | Lluvia |
| weather_drizzle.gif | Llovizna |
| weather_storm.gif | Tormenta |
| weather_snow.gif | Nieve |
| weather_mist.gif | Niebla |

### Luna
| Archivo | Descripcion |
|---------|-------------|
| moon_new.gif | Luna nueva |
| moon_waxing_crescent.gif | Creciente |
| moon_first_quarter.gif | Cuarto creciente |
| moon_waxing_gibbous.gif | Gibosa creciente |
| moon_full.gif | Luna llena |
| moon_waning_gibbous.gif | Gibosa menguante |
| moon_last_quarter.gif | Cuarto menguante |
| moon_waning_crescent.gif | Menguante |

---

## Comandos de Compilacion

```bash
# Compilar
pio run -e ulanzi

# Compilar y subir
pio run -e ulanzi --target upload

# Monitor serial
pio device monitor

# Limpiar
pio run -e ulanzi --target clean
```

---

## API de OpenWeather

### Obtener API Key (gratis)
1. Registrarse en: https://openweathermap.org/api
2. Ir a "API Keys" en tu perfil
3. El key tarda ~10 minutos en activarse

### Limites del plan gratuito
- 1,000 llamadas/dia
- 60 llamadas/minuto
- Con actualizacion cada 15 min: ~96 llamadas/dia (sobra mucho)

### Endpoints usados
```
# Clima actual
GET api.openweathermap.org/data/2.5/weather?lat={lat}&lon={lon}&appid={key}&units=metric&lang=es

# Calidad del aire
GET api.openweathermap.org/data/2.5/air_pollution?lat={lat}&lon={lon}&appid={key}
```

---

## Escala AQI (US EPA)

| AQI | Nivel | Color | PM2.5 (ug/m3) |
|-----|-------|-------|---------------|
| 0-50 | Bueno | #00E400 (Verde) | 0-12 |
| 51-100 | Moderado | #FFFF00 (Amarillo) | 12.1-35.4 |
| 101-150 | Insalubre (sensibles) | #FF7E00 (Naranja) | 35.5-55.4 |
| 151-200 | Insalubre | #FF0000 (Rojo) | 55.5-150.4 |
| 201-300 | Muy insalubre | #8F3F97 (Morado) | 150.5-250.4 |
| 301+ | Peligroso | #7E0023 (Marron) | 250.5+ |

---

## Fases Lunares

| Dias del ciclo | Fase | Emoji |
|----------------|------|-------|
| 0-1, 29 | Luna nueva | new |
| 2-6 | Creciente | waxing_crescent |
| 7-8 | Cuarto creciente | first_quarter |
| 9-13 | Gibosa creciente | waxing_gibbous |
| 14-16 | Luna llena | full |
| 17-21 | Gibosa menguante | waning_gibbous |
| 22-23 | Cuarto menguante | last_quarter |
| 24-28 | Menguante | waning_crescent |

Ciclo lunar completo: 29.53 dias

---

## Historial de Cambios

### 2026-04-24 - Creacion del proyecto
- Fork de AWTRIX 3
- Creado WeatherManager.cpp/h
- Creado MoonPhase.cpp/h
- Documentacion inicial
- Repositorio en GitHub: https://github.com/XE1E/AWTRIX-3-Alone-XE1E

---

## Checklist de Desarrollo

### Modulos base
- [x] Crear WeatherManager.cpp/h
- [x] Crear MoonPhase.cpp/h
- [x] Documentar estructura

### Integracion
- [ ] Agregar variables en Globals.h
- [ ] Agregar variables en Globals.cpp
- [ ] Integrar en main.cpp setup()
- [ ] Integrar en main.cpp loop()

### Apps visuales
- [ ] Crear WeatherApp en Apps.cpp
- [ ] Crear AQIApp en Apps.cpp
- [ ] Crear MoonApp en Apps.cpp
- [ ] Registrar apps en DisplayManager.cpp

### Configuracion web
- [ ] Agregar GET en ServerManager.cpp
- [ ] Agregar POST en ServerManager.cpp
- [ ] Persistir en saveSettings()
- [ ] Cargar en loadSettings()

### Iconos
- [ ] Crear iconos de clima (8x8 gif)
- [ ] Crear iconos de luna (8x8 gif)
- [ ] Subir al filesystem

### Pruebas
- [ ] Compilar sin errores
- [ ] Probar en hardware
- [ ] Verificar llamadas a API
- [ ] Ajustar posiciones visuales

---

*Documento creado: 2026-04-24*
*Mantener actualizado con cada cambio*
