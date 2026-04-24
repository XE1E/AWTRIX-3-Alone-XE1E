# AWTRIX 3 Alone - XE1E Edition

Fork personalizado de AWTRIX 3 con funcionalidades standalone - todo corre en el ESP32 sin necesidad de servidores externos, Node-RED, ni Home Assistant.

## Objetivo

Crear un reloj LED inteligente 100% autónomo que solo necesita:
- ESP32
- Matriz LED WS2812B 32x8
- Conexión WiFi
- API key de OpenWeather (gratis)

## Funcionalidades

### Incluidas de AWTRIX 3 base:
- [x] Reloj con sincronización NTP
- [x] Fecha/Calendario
- [x] Temperatura y humedad (sensores locales)
- [x] Notificaciones via API
- [x] GIFs y animaciones
- [x] MQTT (opcional)
- [x] Interfaz web de configuración

### Nuevas funcionalidades standalone:
- [ ] **Clima Online** - Datos de OpenWeather directamente en el ESP32
- [ ] **Indice de Calidad del Aire (ICA/AQI)** - De OpenWeather Air Pollution API
- [ ] **Fases Lunares** - Calculo matematico local (sin API)
- [ ] **Calendario mejorado** - Eventos locales
- [ ] **Iconos de clima** - Integrados en el firmware

## Hardware Soportado

| Dispositivo | Estado |
|-------------|--------|
| Ulanzi TC001 | Soportado |
| ESP32 + Matriz WS2812B custom | Soportado |
| ESP32-S3 | En pruebas |

## Conexiones (Build Custom)

```
ESP32                    Matriz WS2812B 32x8
GPIO32  --[330R]------►  DIN
GND     --------------►  GND
5V      --------------►  VCC

Opcional:
GPIO35  ◄--------------  LDR (auto-brillo)
GPIO26  ◄--------------  Boton izquierdo
GPIO27  ◄--------------  Boton central
GPIO14  ◄--------------  Boton derecho
GPIO21  ◄--------------  SDA (sensor temp)
GPIO22  ◄--------------  SCL (sensor temp)
```

## Instalacion

### Opcion 1: Compilar desde codigo

```bash
# Clonar repositorio
git clone https://github.com/XE1E/AWTRIX-3-Alone-XE1E.git
cd AWTRIX-3-Alone-XE1E

# Compilar con PlatformIO
pio run -e ulanzi

# Subir al ESP32
pio run -e ulanzi --target upload
```

## Configuracion

### Primera vez:
1. El ESP32 crea red WiFi: `awtrix_XXXXX`
2. Conectar con password: `12345678`
3. Abrir `http://192.168.4.1`
4. Configurar WiFi

### Configurar APIs:
1. Acceder a `http://[IP-del-ESP32]`
2. Ir a Settings
3. Ingresar:
   - OpenWeather API Key
   - Latitud y Longitud
   - Zona horaria

## API Key de OpenWeather

1. Registrarse en: https://openweathermap.org/api
2. Ir a "API Keys" en tu perfil
3. Copiar el key (tarda ~10 min en activarse)
4. Plan gratuito: 1000 llamadas/dia (suficiente)

## Estructura del Proyecto

```
src/
├── main.cpp              # Entrada principal
├── DisplayManager.cpp    # Control de matriz LED
├── Apps.cpp              # Sistema de aplicaciones
├── WeatherManager.cpp    # Cliente HTTP para clima/AQI
├── WeatherManager.h
├── MoonPhase.cpp         # Calculo de fases lunares
├── MoonPhase.h
├── Globals.cpp           # Variables globales
└── ...
```

## Apps Incluidas

| App | Descripcion | Fuente de datos |
|-----|-------------|-----------------|
| Reloj | Hora actual | NTP |
| Fecha | Dia, mes, año | Local |
| Clima | Temperatura, condicion | OpenWeather API |
| ICA | Indice calidad aire | OpenWeather API |
| Luna | Fase lunar actual | Calculo local |
| Temperatura | Temp ambiente | Sensor BME280/SHT31 |
| Humedad | Humedad relativa | Sensor BME280/SHT31 |

## Desarrollo

### Requisitos:
- VS Code + PlatformIO
- ESP32 conectado por USB

### Comandos utiles:

```bash
# Compilar
pio run -e ulanzi

# Compilar y subir
pio run -e ulanzi --target upload

# Monitor serial
pio device monitor

# Limpiar build
pio run -e ulanzi --target clean
```

## Creditos

- **AWTRIX 3 Original:** [Blueforcer](https://github.com/Blueforcer/awtrix3)
- **Este Fork:** [XE1E](https://github.com/XE1E)

## Licencia

Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International
(Misma licencia que AWTRIX 3 original)

---

## TODO

- [ ] Implementar WeatherManager
- [ ] Implementar MoonPhase
- [ ] Crear WeatherApp
- [ ] Crear AQIApp  
- [ ] Crear MoonApp
- [ ] Agregar iconos de clima al filesystem
- [ ] Agregar configuracion de API en interfaz web
- [ ] Documentar API endpoints
- [ ] Probar en hardware
