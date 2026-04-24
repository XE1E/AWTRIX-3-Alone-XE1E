# Guía AWTRIX 2 - Instalación y Reinstalación

**Fecha:** 2026-04-24  
**Sistema:** Windows 11  
**Hardware:** ESP8266EX (Wemos D1 mini) + Matriz LED WS2812B

---

## Resumen del Sistema AWTRIX 2

AWTRIX 2 tiene dos componentes:

| Componente | Ubicación | Función |
|------------|-----------|---------|
| **Servidor Java** | PC/Raspberry Pi | Procesa apps, clima, luna, etc. |
| **Firmware ESP8266** | Microcontrolador | Recibe comandos y controla LEDs |

```
[Apps .jar] → [awtrix.jar servidor] → [WiFi] → [ESP8266] → [Matriz LED]
```

---

## Tu Hardware

- **Chip:** ESP8266EX
- **MAC:** b4:e6:2d:69:7a:4b
- **Puerto USB:** COM5 (USB-SERIAL CH340)
- **Versión servidor:** AWTRIX 2045

---

## Archivos Importantes

```
D:\Documents\Raspi\awtrix\
├── awtrix.jar                    # Servidor Java (ejecutable)
├── backup_esp32_awtrix.bin       # ⭐ BACKUP del firmware ESP8266 (4MB)
├── Apps\                         # Aplicaciones Java
│   ├── OpenWeather.jar
│   ├── Moon.jar
│   ├── AQI.jar
│   ├── News.jar
│   ├── Date.jar
│   ├── Time.jar
│   └── ...
├── config\                       # Configuración del servidor
├── www\                          # Interfaz web
├── logs\                         # Logs del servidor
└── tools\
    └── esptool-win64\            # Herramienta para flashear ESP
        └── esptool.exe
```

---

## PARTE 1: Cómo Iniciar AWTRIX 2 (Uso Normal)

### Paso 1: Iniciar el servidor Java

```powershell
cd D:\Documents\Raspi\awtrix
java -jar awtrix.jar
```

O doble clic en `awtrix.jar` si Java está asociado.

### Paso 2: Acceder a la interfaz web

Abre en tu navegador:
```
http://localhost:7000
```

### Paso 3: Verificar conexión del ESP8266

- El ESP8266 debe estar encendido y conectado a la misma red WiFi
- En la interfaz web verás "Matrix connected" cuando esté enlazado

---

## PARTE 2: Reinstalar Firmware en el ESP8266

### Cuándo reinstalar:
- El ESP8266 no responde
- Se borró el firmware accidentalmente
- Quieres restaurar desde el backup

### Requisitos:
- ESP8266 conectado por USB (COM5)
- esptool instalado en `D:\Documents\Raspi\awtrix\tools\esptool-win64\`

### Comando para restaurar el backup:

```powershell
# Abrir PowerShell como administrador

# Restaurar firmware completo desde backup
D:\Documents\Raspi\awtrix\tools\esptool-win64\esptool.exe --port COM5 --baud 921600 write_flash 0 D:\Documents\Raspi\awtrix\backup_esp32_awtrix.bin
```

### Proceso completo paso a paso:

1. **Conectar el ESP8266 por USB**
   - Verificar que aparece en COM5 (o buscar en Administrador de dispositivos)

2. **Poner ESP8266 en modo flash (si es necesario):**
   - Mantener presionado el botón FLASH/BOOT
   - Presionar y soltar RESET
   - Soltar FLASH/BOOT
   - (En muchos Wemos D1 mini esto es automático)

3. **Ejecutar el comando de restauración:**
   ```powershell
   D:\Documents\Raspi\awtrix\tools\esptool-win64\esptool.exe --port COM5 --baud 921600 write_flash 0 D:\Documents\Raspi\awtrix\backup_esp32_awtrix.bin
   ```

4. **Esperar a que termine** (toma ~1 minuto)

5. **Reiniciar el ESP8266:**
   - Desconectar y reconectar USB, o
   - Presionar botón RESET

6. **Configurar WiFi del ESP8266:**
   - Si el ESP no recuerda la red WiFi, creará un hotspot
   - Conectarse al hotspot desde el celular
   - Configurar la red WiFi de tu casa
   - El ESP se reiniciará y conectará

---

## PARTE 3: Configurar WiFi del ESP8266

### Si el ESP8266 pierde la configuración WiFi:

1. **El ESP8266 crea un Access Point:**
   - Nombre: `AWTRIX_XXXX` o `awtrix` (varía)
   - Contraseña: generalmente `12345678` o sin contraseña

2. **Conectarse desde el celular:**
   - Ir a configuración WiFi
   - Conectar a la red del AWTRIX

3. **Abrir el portal de configuración:**
   - El celular debería abrir automáticamente una página
   - Si no, ir a `http://192.168.4.1`

4. **Ingresar datos de tu WiFi:**
   - SSID: nombre de tu red
   - Password: contraseña de tu WiFi
   - Guardar

5. **El ESP8266 se reinicia y conecta a tu red**

---

## PARTE 4: Verificar Conexión

### Desde el servidor AWTRIX:

Los logs muestran el estado:
```
Connecting with WiFi          # Buscando ESP
Matrix connected!             # ✓ Conectado
Matrix connection Timeout!    # ✗ No encuentra ESP
```

### Ver logs en tiempo real:

```powershell
Get-Content D:\Documents\Raspi\awtrix\logs\*.log -Tail 20 -Wait
```

### Buscar el ESP en la red:

El ESP8266 tiene IP asignada por DHCP. Puedes buscarlo en:
- Tu router (lista de dispositivos)
- La interfaz web de AWTRIX muestra la IP cuando conecta

---

## PARTE 5: Hacer un Nuevo Backup

Si modificas algo y quieres guardar el estado actual:

```powershell
# Crear backup con fecha
$fecha = Get-Date -Format "yyyy-MM-dd"
D:\Documents\Raspi\awtrix\tools\esptool-win64\esptool.exe --port COM5 --baud 921600 read_flash 0 0x400000 "D:\Documents\Raspi\awtrix\backup_esp8266_$fecha.bin"
```

---

## PARTE 6: Solución de Problemas

### "Matrix connection Timeout!"
- El ESP8266 no está en la misma red WiFi
- El ESP8266 está apagado
- El firewall bloquea la conexión (abrir puerto 7000 y 5001)

### "Address already in use: bind"
- Otra instancia de AWTRIX está corriendo
- Cerrar todas las ventanas de Java:
  ```powershell
  taskkill /F /IM java.exe
  ```

### El ESP8266 no responde por USB
- Probar otro cable USB (algunos solo cargan, no transmiten datos)
- Verificar drivers CH340 instalados
- Probar otro puerto USB

### No encuentra el puerto COM
- Abrir Administrador de dispositivos
- Buscar en "Puertos (COM y LPT)"
- Si aparece con advertencia, reinstalar drivers CH340

### Drivers CH340 (si es necesario)
- Descargar de: https://www.wch-ic.com/downloads/CH341SER_EXE.html
- Instalar y reiniciar

---

## PARTE 7: Resumen de Comandos Útiles

### Identificar ESP conectado:
```powershell
D:\Documents\Raspi\awtrix\tools\esptool-win64\esptool.exe --port COM5 chip_id
```

### Borrar completamente el ESP (si hay problemas):
```powershell
D:\Documents\Raspi\awtrix\tools\esptool-win64\esptool.exe --port COM5 erase_flash
```
⚠️ **CUIDADO:** Esto borra todo. Luego necesitas restaurar el backup.

### Verificar el backup:
```powershell
# Ver información del archivo
Get-Item D:\Documents\Raspi\awtrix\backup_esp32_awtrix.bin | Select-Object Name, Length, LastWriteTime
```
Debe mostrar: ~4,194,304 bytes (4MB)

### Ver puertos COM disponibles:
```powershell
Get-CimInstance Win32_PnPEntity | Where-Object { $_.Name -match 'COM\d' } | Select-Object Name
```

---

## Lo que Hicimos Hoy (2026-04-24)

1. ✅ Exploramos el proyecto AWTRIX
2. ✅ Descubrimos que tienes ESP8266 (no ESP32)
3. ✅ Descargamos esptool standalone
4. ✅ Hicimos backup del firmware: `backup_esp32_awtrix.bin`
5. ✅ Instalamos Java 17 JRE via winget
6. ✅ Iniciamos el servidor AWTRIX (versión 2045)
7. ✅ Creamos esta documentación

---

## Notas Adicionales

### Diferencia AWTRIX 2 vs AWTRIX 3:

| | AWTRIX 2 | AWTRIX 3 |
|---|----------|----------|
| **ESP** | ESP8266 | ESP32 |
| **Servidor** | Java en PC/Raspberry | Todo en el ESP32 |
| **Apps** | .jar externos | Integradas + API |
| **Estado** | Descontinuado | Activo |

### Si quieres migrar a AWTRIX 3 en el futuro:
- Necesitas ESP32 (no ESP8266)
- El código está en: `D:\Documents\Raspi\awtrix\awtrix3\`
- Ver: `PLAN_ESP32_STANDALONE.md` para el plan de desarrollo

---

*Documento creado: 2026-04-24*
*Mantener actualizado si hay cambios en la configuración*
