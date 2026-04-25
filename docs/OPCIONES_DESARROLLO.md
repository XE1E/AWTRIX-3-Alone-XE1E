# Opciones de Desarrollo para Proyectos AWTRIX

Comparativa de IDEs y herramientas para desarrollar los proyectos AWTRIX.

---

## Comparativa General de IDEs

| IDE | Velocidad | Facilidad | Funciones | Recomendado para |
|-----|-----------|-----------|-----------|------------------|
| **PlatformIO + VS Code** | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | Proyectos serios ESP32 |
| **Arduino IDE 2.x** | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ | Principiantes |
| **ESP-IDF** | ⭐⭐⭐⭐ | ⭐⭐ | ⭐⭐⭐⭐⭐ | Bajo nivel/Pro |
| **VS Code + Arduino** | ⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ | Término medio |
| **B4J** | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ | Apps Java AWTRIX 2 |
| **IntelliJ IDEA** | ⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ | Java puro |

---

## AWTRIX-3-Alone-XE1E (ESP32)

### Recomendado: PlatformIO + VS Code

**Por qué:**
1. El proyecto ya tiene `platformio.ini` configurado
2. Compilación más rápida con caché inteligente
3. Gestión de librerías automática
4. Autocompletado real (IntelliSense)
5. Múltiples targets (Ulanzi, custom build)
6. Debug integrado y serial monitor

### Instalación

```powershell
# 1. Instalar VS Code (si no lo tienes)
winget install Microsoft.VisualStudioCode

# 2. Abrir VS Code
# 3. Instalar extensión PlatformIO:
#    - Ctrl+Shift+X
#    - Buscar "PlatformIO IDE"
#    - Click en Install
#    - Reiniciar VS Code

# 4. Abrir el proyecto
#    - File → Open Folder
#    - Seleccionar: D:\Documents\Raspi\AWTRIX-3-Alone-XE1E
```

### Comandos de PlatformIO

| Acción | Comando Terminal | Atajo VS Code |
|--------|------------------|---------------|
| Compilar | `pio run -e ulanzi` | Ctrl+Alt+B |
| Subir firmware | `pio run -e ulanzi -t upload` | Ctrl+Alt+U |
| Monitor serial | `pio device monitor` | Ctrl+Alt+S |
| Limpiar build | `pio run -e ulanzi -t clean` | - |
| Listar puertos | `pio device list` | - |

### Estructura del proyecto

```
AWTRIX-3-Alone-XE1E/
├── platformio.ini          # Configuración de PlatformIO
├── src/                    # Código fuente
│   ├── main.cpp
│   ├── WeatherManager.cpp  # Nuevo módulo clima
│   ├── MoonPhase.cpp       # Nuevo módulo luna
│   └── ...
├── lib/                    # Librerías locales
├── include/                # Headers
└── docs/                   # Documentación
```

### Targets disponibles

```ini
# En platformio.ini:

[env:ulanzi]        # Para reloj Ulanzi TC001
[env:awtrix2_upgrade]  # Para upgrade de AWTRIX 2 hardware
```

Compilar para target específico:
```bash
pio run -e ulanzi           # Ulanzi TC001
pio run -e awtrix2_upgrade  # Hardware AWTRIX 2 con ESP32
```

---

## AWTRIX-2-XE1E (Servidor Java)

### Para crear Apps (.jar)

#### Opción 1: B4J (Recomendado)

**Ventajas:**
- Es lo que usó el autor original
- Sintaxis tipo Visual Basic (más fácil)
- Genera .jar compatibles directamente
- IDE visual con diseñador

**Descargar:** https://www.b4x.com/b4j.html (gratis)

**Ejemplo de código B4J:**
```basic
Sub Process_Globals
    Private Timer1 As Timer
End Sub

Sub AppStart
    Timer1.Initialize("Timer1", 1000)
    Timer1.Enabled = True
End Sub

Sub Timer1_Tick
    ' Actualizar datos
End Sub

Sub Draw As Object()
    Dim result(3) As Object
    result(0) = "25°C"      ' Texto
    result(1) = 1234        ' Icono
    result(2) = "#FFFFFF"   ' Color
    Return result
End Sub
```

#### Opción 2: IntelliJ IDEA / Eclipse

**Ventajas:**
- IDE profesional para Java
- Mejor para proyectos grandes
- Debugging avanzado

**Descargar:** 
- IntelliJ: https://www.jetbrains.com/idea/download/ (Community = gratis)
- Eclipse: https://www.eclipse.org/downloads/

### Para el firmware ESP8266

El firmware ya está compilado (`firmware/awtrix2_esp8266_firmware.bin`).

Solo necesitas flashearlo:
```powershell
.\tools\esptool-win64\esptool.exe --port COM5 --baud 921600 write_flash 0 firmware\awtrix2_esp8266_firmware.bin
```

---

## Resumen de Recomendaciones

| Proyecto | Qué desarrollar | IDE Recomendado |
|----------|-----------------|-----------------|
| **AWTRIX-3-Alone-XE1E** | Firmware ESP32 completo | **PlatformIO + VS Code** |
| **AWTRIX-2-XE1E** | Apps .jar nuevas | **B4J** |
| **AWTRIX-2-XE1E** | Apps .jar (Java puro) | **IntelliJ IDEA** |
| **AWTRIX-2-XE1E** | Servidor (modificar) | **IntelliJ IDEA** |

---

## Arduino IDE - Cuándo usarlo

**Usar para:**
- Pruebas rápidas de 5 minutos
- Código simple de un archivo
- Enseñar a alguien que empieza

**NO recomendado para AWTRIX porque:**
- Proyecto tiene 30+ archivos fuente
- Dependencias complejas de librerías
- Configuración específica de particiones
- Sin autocompletado real
- Compilación lenta

---

## AWTRIX 2 vs AWTRIX 3 para Desarrollo Standalone

| Aspecto | AWTRIX 2 | AWTRIX 3 |
|---------|----------|----------|
| Arquitectura | ESP8266 + Servidor Java | **Todo en ESP32** |
| Dependencias | Necesita PC/Raspberry siempre | **Solo WiFi** |
| Desarrollo | Java (B4J) - proyecto muerto | **C++ activo** |
| Comunidad | Abandonada | **5000+ en Discord** |
| Actualizaciones | Ninguna desde 2022 | **Activas** |
| Hardware | ESP8266 (limitado) | **ESP32 (potente)** |

**Conclusion:** Desarrollar en AWTRIX 3 es mejor porque el proyecto esta vivo y todo corre en el ESP32.

---

## ESP32 - Cual Modelo Usar

### Modelos Recomendados (probados con AWTRIX 3)

| Modelo | Precio | Notas | Recomendacion |
|--------|--------|-------|---------------|
| **ESP32-WROOM-32** | ~$5 | El clasico, funciona perfecto | Mejor opcion generica |
| **ESP32 D1 Mini** | ~$4 | Compacto, compatible con placa AWTRIX 2 | Si tienes hardware AWTRIX 2 |
| **NodeMCU-32S** | ~$5 | Facil de usar, muchos pines | Buena opcion |
| **Ulanzi TC001** | ~$50 | Todo integrado (ESP32 + matriz + case) | Plug & play |

### Funcionan pero no ideales

| Modelo | Notas |
|--------|-------|
| ESP32-S2 | Sin Bluetooth, menos RAM |
| ESP32-C3 | RISC-V, menos compatible |
| ESP32-S3 | Funciona pero overkill |

### NO usar

| Modelo | Por que |
|--------|---------|
| ESP8266 | Sin suficiente RAM/Flash para AWTRIX 3 |
| ESP32-C2 | Muy limitado |

### Especificaciones Minimas

```
CPU:        240 MHz dual-core (ESP32 estandar)
Flash:      4 MB minimo
RAM:        520 KB (SRAM)
WiFi:       802.11 b/g/n
GPIO libre: 1 pin para datos de LEDs (GPIO32 recomendado)
```

### Recomendacion de Compra

**Para empezar desarrollo (economico):**

```
ESP32-WROOM-32 DevKit     ~$5 (AliExpress)
Matriz WS2812B 32x8       ~$10-15
Fuente 5V 4A              ~$5
─────────────────────────────────
Total:                    ~$20-25
```

**Si quieres todo listo:**

```
Ulanzi TC001              ~$50
(Ya tiene ESP32 + matriz + case + sensores)
```

---

## Ulanzi TC001 - Hardware Oficial AWTRIX 3

El Ulanzi TC001 es el hardware para el que AWTRIX 3 fue disenado. Es **100% hackeable** y perfecto para desarrollo.

### Que incluye

| Componente | Incluido |
|------------|----------|
| ESP32-WROOM-32 | Si (programable) |
| Matriz LED 32x8 | Si (WS2812B) |
| Sensor temp/humedad | Si |
| LDR (auto-brillo) | Si |
| Buzzer | Si |
| 3 Botones | Si |
| Case/carcasa | Si |
| USB-C | Si (datos + alimentacion) |

### Que puedes hacer con el TC001

| Accion | Posible |
|--------|---------|
| Flashear firmware custom | Si |
| Modificar codigo AWTRIX 3 | Si |
| Agregar WeatherManager/MoonPhase | Si |
| Crear nuevas apps | Si |
| Volver al firmware original | Si |
| Usar con Home Assistant | Si |
| Acceder a sensores | Si |

### Arquitectura interna

```
Ulanzi TC001
     │
     ├── ESP32-WROOM-32 ◄── Programable 100%
     ├── Matriz WS2812B 32x8
     ├── Sensor temperatura/humedad
     ├── LDR (luz ambiente)
     ├── Buzzer
     ├── 3 Botones
     └── USB-C (datos + alimentacion)
```

### Flujo de desarrollo con TC001

```
1. Conectar TC001 por USB-C
2. Abrir proyecto AWTRIX-3-Alone-XE1E en VS Code
3. Modificar codigo (WeatherManager, MoonPhase, etc.)
4. Compilar: pio run -e ulanzi
5. Flashear: pio run -e ulanzi -t upload
6. Ver cambios en el reloj
7. Repetir
```

### Target en platformio.ini

```ini
[env:ulanzi]
platform = espressif32@6.3.0
board = esp32dev
build_flags = -DULANZI
```

### Donde comprar

| Tienda | Precio aprox |
|--------|--------------|
| AliExpress | ~$45-55 USD |
| Amazon | ~$50-60 USD |
| Ulanzi.com | ~$50 USD |

**Buscar:** "Ulanzi TC001 Smart Pixel Clock"

---

### Conexion Basica ESP32 Generico

```
ESP32                Matriz WS2812B
─────                ─────────────
GPIO32  ────────────► DIN
GND     ────────────► GND
5V (externa) ───────► VCC

Nota: Alimentar matriz con fuente externa 5V 4A
      No usar el 5V del ESP32 (no tiene suficiente corriente)
```

---

## Compatibilidad ESP8266 vs ESP32

**AWTRIX 3 NO es compatible con ESP8266.** Razones:

| Recurso | ESP8266 | ESP32 | AWTRIX 3 necesita |
|---------|---------|-------|-------------------|
| RAM | 80 KB | 520 KB | ~300 KB |
| CPU | 1 core 80MHz | 2 cores 240MHz | Dual core |
| Flash | 4 MB | 4 MB | OK |
| WiFi | Si | Si | OK |
| Bluetooth | No | Si | Opcional |

**Conclusion:** Si tienes ESP8266, solo sirve para AWTRIX 2 (con servidor Java).

---

## Desarrollo sin Hardware (mientras consigues matriz)

### Lo que puedes hacer SIN matriz LED

| Tarea | Requiere matriz |
|-------|-----------------|
| Instalar PlatformIO + VS Code | No |
| Compilar codigo | No |
| Verificar errores de compilacion | No |
| Integrar nuevos modulos | No |
| Modificar y probar logica | No |
| Flashear al ESP32 | No (solo ESP32 por USB) |
| Ver resultado en LEDs | **Si** |
| Probar apps visuales | **Si** |

### Setup de desarrollo paralelo

```
ESP8266 NodeMCU v3              ESP32 30-pin USB-C
       │                               │
       ▼                               ▼
┌─────────────────┐            ┌─────────────────┐
│   AWTRIX 2      │            │   AWTRIX 3      │
│   (produccion)  │            │   (desarrollo)  │
│                 │            │                 │
│ Sigue corriendo │            │ Pruebas nuevas  │
│ con tu matriz   │            │ funciones       │
└─────────────────┘            └─────────────────┘
```

Esto permite desarrollar sin interrumpir tu reloj actual.

### Donde comprar matriz WS2812B 32x8

| Tienda | Tiempo envio | Precio aprox |
|--------|--------------|--------------|
| AliExpress | 2-3 semanas | ~$8-12 USD |
| Amazon | 1-2 dias | ~$15-20 USD |
| MercadoLibre | Variable | ~$200-400 MXN |

**Buscar:** "WS2812B 32x8 flexible" o "WS2812B 8x32 panel"

---

## Links de Descarga

| Herramienta | URL |
|-------------|-----|
| VS Code | https://code.visualstudio.com/ |
| PlatformIO (extension) | Instalar desde VS Code |
| B4J | https://www.b4x.com/b4j.html |
| IntelliJ IDEA | https://www.jetbrains.com/idea/download/ |
| Arduino IDE | https://www.arduino.cc/en/software |

---

## Links de Compra Hardware

| Componente | Donde comprar |
|------------|---------------|
| ESP32-WROOM-32 | AliExpress, Amazon, MercadoLibre |
| Matriz WS2812B 32x8 | AliExpress, Amazon |
| Ulanzi TC001 | Amazon, Ulanzi.com, AliExpress |
| Fuente 5V 4A | Amazon, tiendas electronica |

---

*Documento creado: 2026-04-24*
