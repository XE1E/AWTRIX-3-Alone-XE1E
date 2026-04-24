# AWTRIX 3 - Guía de Instalación y Desarrollo

## Repositorio
- **Tu Fork:** https://github.com/XE1E/awtrix3-XE1E
- **Original:** https://github.com/Blueforcer/awtrix3
- **Documentación:** https://blueforcer.github.io/awtrix3/

---

# PARTE 1: INSTALACIÓN CON ESP32 + MATRIZ WS2812B 32x8

## Materiales necesarios

| Componente | Descripción | Notas |
|------------|-------------|-------|
| ESP32 | ESP32-WROOM, NodeMCU-32S, o ESP32 D1 Mini | Cualquier variante funciona |
| Matriz LED | WS2812B 32x8 (256 LEDs) | Flexible o rígida |
| Fuente 5V | 5V 4A mínimo | Los LEDs consumen ~60mA cada uno a brillo máximo |
| Cables | Dupont o soldados | 3 cables: VCC, GND, DATA |
| Capacitor | 1000µF 6.3V (opcional) | Protege los LEDs de picos |
| Resistencia | 330Ω (opcional) | En línea de datos, protege el primer LED |

## Diagrama de conexión

```
                    ESP32                         MATRIZ WS2812B 32x8
                 +---------+                      +------------------+
                 |         |                      |                  |
    USB -------->|   USB   |                      |   32 x 8 LEDs    |
                 |         |                      |                  |
                 |  GPIO32 |---[330Ω]------------>| DIN (Data In)    |
                 |         |                      |                  |
                 |    GND  |--------------------->| GND              |
                 |         |                      |                  |
                 |    5V   |--------------------->| 5V / VCC         |
                 +---------+                      +------------------+
                      |                                   |
                      +----------- GND común -------------+
                                      |
                               [1000µF cap]
                                      |
                              Fuente 5V 4A
```

## Conexiones pin a pin

| ESP32 GPIO | Conectar a | Descripción |
|------------|------------|-------------|
| **GPIO32** | DIN de la matriz | Datos de los LEDs (obligatorio) |
| **GND** | GND de la matriz Y fuente | Tierra común (obligatorio) |
| **5V/VIN** | 5V de la fuente | Alimentación (obligatorio) |

### Conexiones opcionales (mejoran la experiencia)

| ESP32 GPIO | Componente | Descripción |
|------------|------------|-------------|
| GPIO35 | LDR (fotoresistor GL5516) | Brillo automático según luz ambiente |
| GPIO26 | Botón izquierdo | Navegación menú |
| GPIO27 | Botón central | Seleccionar |
| GPIO14 | Botón derecho | Navegación menú |
| GPIO15 | Buzzer pasivo | Sonidos y alarmas |
| GPIO21 (SDA) | Sensor temp (BME280/SHT31) | Temperatura y humedad |
| GPIO22 (SCL) | Sensor temp (BME280/SHT31) | I2C Clock |

## Paso 1: Preparar el hardware

1. **Conecta la matriz al ESP32:**
   - GPIO32 → DIN de la matriz (con resistencia 330Ω opcional)
   - GND del ESP32 → GND de la matriz
   
2. **Conecta la fuente de alimentación:**
   - 5V de la fuente → 5V de la matriz
   - GND de la fuente → GND de la matriz Y GND del ESP32
   - (El ESP32 se puede alimentar por USB o por la fuente)

3. **Importante:** GND debe ser común entre ESP32, matriz y fuente.

## Paso 2: Flashear el firmware

### Opción A: Flasher online (más fácil)

1. Abre Google Chrome o Microsoft Edge
2. Ve a: https://blueforcer.github.io/awtrix3/#/flasher
3. Conecta el ESP32 por USB
4. Selecciona el puerto COM
5. Marca "Erase" si es la primera vez
6. Haz clic en "Flash"

### Opción B: Compilar desde código (para desarrollo)

```bash
# 1. Instalar PlatformIO en VS Code
#    - Abre VS Code
#    - Ve a Extensions (Ctrl+Shift+X)
#    - Busca "PlatformIO IDE"
#    - Instalar

# 2. Abrir el proyecto
#    - File > Open Folder > D:\Documents\Raspi\awtrix\awtrix3

# 3. Compilar (desde terminal de PlatformIO)
cd D:\Documents\Raspi\awtrix\awtrix3
pio run -e ulanzi

# 4. Subir al ESP32 (conectado por USB)
pio run -e ulanzi --target upload

# 5. Ver logs de debug
pio device monitor
```

## Paso 3: Configurar WiFi

1. Después de flashear, el ESP32 crea una red WiFi:
   - **Nombre:** `awtrix_XXXXX`
   - **Contraseña:** `12345678`

2. Conéctate a esa red desde tu celular o PC

3. Abre el navegador y ve a: `http://192.168.4.1`

4. Ingresa los datos de tu WiFi y guarda

5. El dispositivo se reinicia y se conecta a tu red

## Paso 4: Acceder a la interfaz web

1. Busca la IP del dispositivo (aparece en la matriz al conectar)
2. Abre `http://[IP-DEL-DISPOSITIVO]` en tu navegador
3. Configura MQTT, apps, y otras opciones

## Solución de problemas

### La matriz muestra caracteres raros o invertidos

Crea un archivo `dev.json` en el administrador de archivos web con:

```json
{
  "matrix": 2
}
```

Prueba con valores 0, 1, o 2 hasta que se vea correctamente.

### No enciende ningún LED

- Verifica que GPIO32 esté conectado a DIN
- Verifica que GND sea común
- Verifica que la fuente tenga suficiente corriente (4A)

### El ESP32 se reinicia constantemente

- La fuente no tiene suficiente corriente
- Baja el brillo en la configuración

---

# PARTE 2: DESARROLLO Y MODIFICACIÓN

## Estructura del código fuente

```
src/
├── main.cpp              # Punto de entrada, setup() y loop()
├── DisplayManager.cpp    # Control matriz LED (archivo principal)
├── Apps.cpp              # Sistema de aplicaciones
├── MQTTManager.cpp       # Integración MQTT / Home Assistant
├── ServerManager.cpp     # API HTTP REST
├── PeripheryManager.cpp  # Sensores, botones, buzzer
├── MenuManager.cpp       # Menú en pantalla
├── Overlays.cpp          # Notificaciones superpuestas
├── effects.cpp           # Efectos visuales
├── GifPlayer.h           # Reproductor de GIFs
└── Globals.cpp           # Variables globales
```

## Archivos clave para modificar

| Archivo | Para qué modificarlo |
|---------|---------------------|
| `Apps.cpp` | Crear nuevas aplicaciones |
| `effects.cpp` | Agregar efectos visuales |
| `DisplayManager.cpp` | Cambiar comportamiento de pantalla |
| `PeripheryManager.cpp` | Agregar sensores o botones |
| `Globals.cpp` | Nuevas configuraciones globales |

## Comandos de compilación

```bash
# Compilar sin subir
pio run -e ulanzi

# Compilar y subir
pio run -e ulanzi --target upload

# Solo subir (si ya compilaste)
pio run -e ulanzi --target uploadfs

# Monitor serial (115200 baud)
pio device monitor

# Limpiar build
pio run -e ulanzi --target clean
```

## Sincronizar con el repositorio original

```bash
# Obtener cambios del repo original
git fetch upstream

# Fusionar a tu rama
git checkout main
git merge upstream/main

# Subir a tu fork
git push origin main
```

## Librerías principales

| Librería | Uso |
|----------|-----|
| FastLED | Control de LEDs WS2812B |
| FastLED NeoMatrix | Gestión de matriz |
| ArduinoJson | Parsing JSON |
| PubSubClient | Cliente MQTT |
| EasyButton | Manejo de botones |
| Adafruit BME280/SHT31 | Sensores de temperatura |

## Notas técnicas

- CPU: ESP32 a 240MHz
- Framework: Arduino
- Tamaño máximo paquete MQTT: 8192 bytes
- Puerto serial: 115200 baud
- Pin matriz LED: GPIO32 (hardcoded en DisplayManager.cpp línea 40)

---

# PARTE 3: FLOWS - APLICACIONES EXTERNAS

## Qué son los Flows

Los **Flows** son automatizaciones externas que envían contenido a AWTRIX 3 mediante MQTT o HTTP.
No corren en el ESP32, sino en sistemas externos como Node-RED, Home Assistant, etc.

```
[Fuente de datos]  →  [Node-RED/HA]  →  [MQTT/HTTP]  →  [AWTRIX 3]
   (API externa)       (procesa)        (envía JSON)    (muestra en matriz)
```

## Hub de Flows comunitarios

**https://flows.blueforcer.de/**

- Descargar flows listos para usar
- Subir tus propios flows
- Compartir iconos incluidos
- No requiere login

## Plataformas soportadas

| Plataforma | Descripción | Instalación |
|------------|-------------|-------------|
| **Node-RED** | Visual, ideal para principiantes | `npm install -g node-red` |
| **Home Assistant** | Blueprints y automations | Addon de HA |
| **N8N** | Alternativa a Node-RED | Docker o npm |
| **ioBroker** | Smarthome | Instalador propio |
| **FHEM** | Perl-based | apt install |
| **Domoticz** | Domótica ligera | apt install |

## API para Custom Apps

### Enviar texto via HTTP

```bash
# Crear/actualizar una app personalizada
curl -X POST http://[IP-AWTRIX]/api/custom?name=miapp \
  -H "Content-Type: application/json" \
  -d '{"text":"Hola mundo", "icon": 1234, "color":"#FF0000"}'

# Eliminar una app
curl -X POST http://[IP-AWTRIX]/api/custom?name=miapp \
  -d ''
```

### Enviar texto via MQTT

```
Topic: awtrix/custom/miapp
Payload: {"text":"Hola mundo", "icon": 1234, "color":"#FF0000"}
```

### Enviar notificación (temporal, no se guarda en loop)

```bash
# HTTP
curl -X POST http://[IP-AWTRIX]/api/notify \
  -H "Content-Type: application/json" \
  -d '{"text":"Alerta!", "icon": 555, "duration": 10}'

# MQTT
Topic: awtrix/notify
Payload: {"text":"Alerta!", "icon": 555, "duration": 10}
```

## Propiedades JSON disponibles

| Propiedad | Tipo | Descripción | Ejemplo |
|-----------|------|-------------|---------|
| `text` | string | Texto a mostrar | `"Hola"` |
| `icon` | number | ID del icono (de LaMetric o subido) | `1234` |
| `color` | string/array | Color del texto | `"#FF0000"` o `[255,0,0]` |
| `background` | string/array | Color de fondo | `"#000000"` |
| `duration` | number | Duración en segundos | `10` |
| `scroll` | bool | Activar scroll | `true` |
| `scrollSpeed` | number | Velocidad de scroll (ms) | `100` |
| `effect` | string | Efecto de fondo | `"Fade"` |
| `progress` | number | Barra de progreso (0-100) | `75` |
| `progressC` | string | Color de la barra | `"#00FF00"` |
| `repeat` | number | Repeticiones (notificaciones) | `3` |
| `sound` | string | Sonido a reproducir | `"alarm"` |
| `rtttl` | string | Melodía RTTTL | `"melody:d=4,o=5..."` |
| `pushIcon` | number | Animación del icono (0,1,2) | `1` |
| `lifetime` | number | Tiempo de vida en segundos | `3600` |
| `noScroll` | bool | Desactivar scroll | `true` |
| `center` | bool | Centrar texto | `true` |

## Ejemplo: Flow Node-RED para YouTube

Este flow obtiene suscriptores de YouTube y los muestra en AWTRIX:

### Estructura del flow

```
[Inject cada 1h] → [Function: API key] → [HTTP Request] → [Function: Parser] → [MQTT Out]
```

### Código del nodo "Data" (Function)

```javascript
msg.payload = {
  "id": "TU_CHANNEL_ID",
  "key": "TU_API_KEY",
  "part": "statistics"
};
return msg;
```

### Código del nodo "Parser" (Function)

```javascript
var json = msg.payload;
var subscribers = json.items[0].statistics.subscriberCount;

msg.payload = {
  "text": subscribers,
  "icon": 5029
};
return msg;
```

### Configuración MQTT Out

```
Topic: awtrix/custom/youtube
Broker: localhost:1883
```

## Instalar Node-RED

### En Windows

```bash
# Instalar Node.js primero desde https://nodejs.org

# Instalar Node-RED
npm install -g node-red

# Ejecutar
node-red

# Abrir en navegador
http://localhost:1880
```

### En Raspberry Pi

```bash
# Script de instalación oficial
bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)

# Habilitar como servicio
sudo systemctl enable nodered
sudo systemctl start nodered

# Abrir en navegador
http://[IP-RASPBERRY]:1880
```

### Nodos útiles para instalar

En Node-RED, ve a Menu → Manage Palette → Install:

| Nodo | Para qué |
|------|----------|
| `node-red-contrib-home-assistant` | Integrar Home Assistant |
| `node-red-dashboard` | Crear dashboards web |
| `node-red-contrib-influxdb` | Base de datos de métricas |
| `node-red-contrib-telegrambot` | Enviar/recibir Telegram |

## Flows populares disponibles

| Flow | Descripción | Plataforma |
|------|-------------|------------|
| YouTube Subscribers | Muestra suscriptores | Node-RED |
| Instagram Followers | Seguidores de IG | Node-RED |
| OpenWeather | Clima actual y pronóstico | Node-RED / HA |
| SpeedTest | Velocidad de internet | Node-RED |
| WooCommerce | Notifica pedidos | N8N |
| Spotify Now Playing | Canción actual | Home Assistant |
| Crypto Prices | Bitcoin, ETH, etc. | Node-RED |
| Calendar Events | Próximos eventos | Home Assistant |
| Printer Status | Estado impresora 3D | Node-RED |
| Energy Monitor | Consumo eléctrico | Home Assistant |

## Otros endpoints útiles de la API

| Endpoint | Método | Descripción |
|----------|--------|-------------|
| `/api/stats` | GET | Estado del dispositivo |
| `/api/screen` | GET | Captura de pantalla actual |
| `/api/power` | POST | Encender/apagar matriz |
| `/api/indicator1` | POST | LED indicador esquina |
| `/api/effects` | GET | Lista de efectos |
| `/api/transitions` | GET | Lista de transiciones |
| `/api/loop` | GET | Apps en rotación |
| `/api/reboot` | POST | Reiniciar dispositivo |
| `/api/sound` | POST | Reproducir sonido |
| `/api/moodlight` | POST | Luz ambiente |

## Ejemplo: Indicadores de color

```bash
# Indicador rojo parpadeante (esquina superior derecha)
curl -X POST http://[IP]/api/indicator1 \
  -d '{"color":"#FF0000", "blink": 500}'

# Indicador verde fijo (lado derecho)
curl -X POST http://[IP]/api/indicator2 \
  -d '{"color":[0,255,0]}'

# Apagar indicador
curl -X POST http://[IP]/api/indicator1 \
  -d '{"color":"0"}'
```

## Ejemplo: Control de energía

```bash
# Apagar matriz (pantalla negra, ESP32 sigue funcionando)
curl -X POST http://[IP]/api/power -d '{"power": false}'

# Encender matriz
curl -X POST http://[IP]/api/power -d '{"power": true}'

# Modo sleep por 1 hora (3600 segundos)
curl -X POST http://[IP]/api/sleep -d '{"sleep": 3600}'
```
