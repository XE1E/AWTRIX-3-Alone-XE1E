# Plan: AWTRIX ESP32 Standalone (Sin Servidor Externo)

## Resumen del Proyecto

Migrar todas las funcionalidades que actualmente dependen del servidor Java (AWTRIX2) 
o de Node-RED a código nativo que corra directamente en el ESP32.

---

## Estado Actual

### AWTRIX2 (lo que usas actualmente)
- **Arquitectura:** ESP8266/ESP32 (cliente) + Raspberry Pi/PC (servidor Java)
- **Servidor:** `awtrix.jar` - procesa todo y envía comandos al ESP
- **Apps (.jar):** OpenWeather, News, Moon, AQI, Calendar, etc.
- **Problema:** El proyecto fue descontinuado por Blueforcer, difícil reinstalar

### AWTRIX3 (alternativa moderna)
- **Arquitectura:** Todo corre en el ESP32, no necesita servidor externo
- **Ya incluye:** Reloj, fecha, temperatura, humedad, GIFs, MQTT
- **Falta implementar:** Clima online, Luna, ICA/AQI

---

## Funcionalidades a Implementar

### 1. Clima (WeatherApp)
**Fuente de datos:** OpenWeather API (gratis hasta 1000 llamadas/día)

```
Endpoint: api.openweathermap.org/data/2.5/weather
Parámetros: lat, lon, appid, units=metric, lang=es
Frecuencia: cada 15 minutos
```

**Datos a obtener:**
- Temperatura actual
- Condición (soleado, nublado, lluvia, etc.)
- Icono del clima
- Humedad
- Viento

**Implementación:**
```cpp
// WeatherManager.h
class WeatherManager_ {
public:
    void setup();
    void tick();  // Llamar en loop()
    void fetchWeather();
    
    float temperature;
    int humidity;
    String condition;
    String icon;
    int weatherCode;
    
private:
    unsigned long lastUpdate = 0;
    const unsigned long UPDATE_INTERVAL = 900000; // 15 min
};
```

### 2. Índice de Calidad del Aire (AQIApp)
**Fuente de datos:** OpenWeather Air Pollution API (mismo API key)

```
Endpoint: api.openweathermap.org/data/2.5/air_pollution
Parámetros: lat, lon, appid
Frecuencia: cada 15 minutos
```

**Datos a obtener:**
- PM2.5, PM10
- Calcular AQI según escala US EPA
- Color según nivel (verde/amarillo/naranja/rojo/morado)

**Escala AQI:**
| AQI | Nivel | Color | PM2.5 (µg/m³) |
|-----|-------|-------|---------------|
| 0-50 | Bueno | Verde #00E400 | 0-12 |
| 51-100 | Moderado | Amarillo #FFFF00 | 12.1-35.4 |
| 101-150 | Insalubre (sensibles) | Naranja #FF7E00 | 35.5-55.4 |
| 151-200 | Insalubre | Rojo #FF0000 | 55.5-150.4 |
| 201-300 | Muy insalubre | Morado #8F3F97 | 150.5-250.4 |
| 301+ | Peligroso | Marrón #7E0023 | 250.5+ |

### 3. Fase Lunar (MoonApp)
**Sin API necesaria** - cálculo matemático puro

```cpp
// Algoritmo de fase lunar
int getMoonPhase(int year, int month, int day) {
    // Días desde luna nueva conocida (6 enero 2000)
    int y = year;
    int m = month;
    if (m < 3) { y--; m += 12; }
    
    int a = y / 100;
    int b = a / 4;
    int c = 2 - a + b;
    int e = 365.25 * (y + 4716);
    int f = 30.6001 * (m + 1);
    
    double jd = c + day + e + f - 1524.5;
    double daysSinceNew = jd - 2451549.5;
    double newMoons = daysSinceNew / 29.53059;
    double phase = (newMoons - floor(newMoons)) * 29.53059;
    
    return (int)phase; // 0-29: 0=nueva, 7=cuarto creciente, 15=llena, 22=cuarto menguante
}
```

**Iconos para cada fase:**
| Días | Fase | Icono |
|------|------|-------|
| 0-1 | Luna nueva | 🌑 |
| 2-6 | Creciente | 🌒 |
| 7-8 | Cuarto creciente | 🌓 |
| 9-13 | Gibosa creciente | 🌔 |
| 14-16 | Luna llena | 🌕 |
| 17-21 | Gibosa menguante | 🌖 |
| 22-23 | Cuarto menguante | 🌗 |
| 24-29 | Menguante | 🌘 |

### 4. Calendario Mejorado
**Ya existe DateApp**, pero se puede mejorar para mostrar:
- Próximos eventos (almacenados localmente en SPIFFS)
- Días festivos
- Formato configurable

### 5. Animaciones
**Ya soportado** - subir GIFs al filesystem del ESP32 via interfaz web.

---

## Archivos a Crear/Modificar

### Nuevos archivos:
```
src/
├── WeatherManager.cpp    # Cliente HTTP para clima y AQI
├── WeatherManager.h
├── MoonPhase.cpp         # Cálculo de fase lunar
├── MoonPhase.h
```

### Archivos a modificar:
```
src/
├── main.cpp              # Agregar setup/tick de WeatherManager
├── Apps.cpp              # Agregar WeatherApp, AQIApp, MoonApp
├── Apps.h                # Declaraciones
├── Globals.cpp           # Variables de configuración (API key, coords)
├── Globals.h             # Declaraciones
├── ServerManager.cpp     # Endpoints para configurar API key
```

### Configuración nueva (en interfaz web):
```json
{
  "weather_api_key": "tu_api_key_openweather",
  "weather_lat": "19.4326",
  "weather_lon": "-99.1332",
  "weather_units": "metric",
  "weather_lang": "es"
}
```

---

## Recursos de OpenWeather

### Obtener API Key (gratis):
1. Registrarse en: https://openweathermap.org/api
2. Ir a "API Keys" en tu perfil
3. Copiar el key (tarda ~10 min en activarse)

### Límites plan gratuito:
- 1,000 llamadas/día
- 60 llamadas/minuto
- Suficiente para actualizar cada 15 min (96 llamadas/día para clima + AQI)

---

## Sobre AWTRIX2

### Archivos que tienes:
- `awtrix.jar` - Servidor Java (versión Aug 2022)
- `Apps/*.jar` - Aplicaciones: OpenWeather, News, Moon, AQI, etc.
- `www/` - Interfaz web
- `config/` - Configuración

### El proyecto fue descontinuado:
- Repositorio original archivado
- No hay actualizaciones desde 2022
- Blueforcer migró todo el esfuerzo a AWTRIX3

### Para seguir usando AWTRIX2:
1. **Servidor Java:** Ya tienes `awtrix.jar` - funciona en cualquier PC/Raspberry con Java
2. **Firmware ESP:** El firmware original de AWTRIX2 era para ESP8266, no ESP32
   - Si tienes ESP32, necesitas el firmware de AWTRIX3 en modo compatibilidad
   - O usar ESPHome con integración MQTT

### Alternativas si no puedes reinstalar:
1. **Migrar a AWTRIX3** - Todo standalone en ESP32
2. **ESPHome + Home Assistant** - Muy flexible, comunidad activa
3. **WLED** - Solo para efectos de luz, no texto/apps

---

## Flujo de Trabajo Propuesto

### Fase 1: Backup (ANTES de modificar)
```bash
# Hacer backup del firmware actual del ESP32
esptool.py --port COM3 read_flash 0 0x400000 backup_firmware.bin

# Guardar configuración
# (copiar archivos de config/ y cualquier JSON del ESP)
```

### Fase 2: Desarrollo
1. Crear rama de desarrollo en el repo de AWTRIX3
2. Implementar WeatherManager
3. Implementar MoonPhase
4. Agregar apps a la rotación
5. Probar en ESP32 de desarrollo (si tienes uno extra)

### Fase 3: Migración
1. Flashear AWTRIX3 modificado al ESP32 principal
2. Configurar WiFi, API keys, coordenadas
3. Verificar funcionamiento

---

## Próximos Pasos

1. [ ] Obtener API key de OpenWeather
2. [ ] Decidir si usar ESP32 adicional para desarrollo o modificar el actual
3. [ ] Hacer backup del firmware actual
4. [ ] Implementar WeatherManager (clima + AQI)
5. [ ] Implementar MoonPhase
6. [ ] Crear iconos para clima y luna
7. [ ] Probar y ajustar
8. [ ] Documentar configuración final

---

## Notas Adicionales

### Hardware requerido:
- ESP32 (ya lo tienes)
- Matriz LED WS2812B (ya conectada)
- Conexión WiFi estable (para APIs)

### Memoria ESP32:
- Flash: ~4MB (suficiente para firmware + iconos)
- RAM: ~320KB (suficiente para JSON parsing)
- SPIFFS/LittleFS: ~1.5MB para iconos y GIFs

### Dependencias ya incluidas en AWTRIX3:
- ArduinoJson (parsing de APIs)
- HTTPClient (llamadas HTTP)
- WiFiClientSecure (HTTPS)
- FastLED (control de LEDs)

---

*Documento creado: 2026-04-24*
*Para desarrollo futuro - no modificar código hasta tener backup*
