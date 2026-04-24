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

## Links de Descarga

| Herramienta | URL |
|-------------|-----|
| VS Code | https://code.visualstudio.com/ |
| PlatformIO (extensión) | Instalar desde VS Code |
| B4J | https://www.b4x.com/b4j.html |
| IntelliJ IDEA | https://www.jetbrains.com/idea/download/ |
| Arduino IDE | https://www.arduino.cc/en/software |

---

*Documento creado: 2026-04-24*
