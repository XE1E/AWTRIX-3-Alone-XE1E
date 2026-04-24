# Flows para AWTRIX 3

Colección de flows y scripts para enviar datos a AWTRIX 3.

## ICA / AQI - Índice de Calidad del Aire

Muestra el Índice de Calidad del Aire basado en datos de OpenWeather.

### Archivos

| Archivo | Descripción |
|---------|-------------|
| `aqi-flow.json` | Flow para Node-RED |
| `test-aqi.ps1` | Script PowerShell (Windows) |
| `test-aqi.sh` | Script Bash (Linux/Mac) |

### Configuración con Variables de Entorno

Los scripts usan variables de entorno para mayor seguridad (no exponer API keys en código).

#### Windows (PowerShell)

```powershell
# Configurar para la sesión actual
$env:OPENWEATHER_API_KEY = "tu_api_key"
$env:AWTRIX_IP = "192.168.1.108"
$env:AWTRIX_LAT = "19.4326"
$env:AWTRIX_LON = "-99.1332"

# Configurar permanentemente (ejecutar como Admin)
[System.Environment]::SetEnvironmentVariable("OPENWEATHER_API_KEY", "tu_api_key", "User")
[System.Environment]::SetEnvironmentVariable("AWTRIX_IP", "192.168.1.108", "User")
[System.Environment]::SetEnvironmentVariable("AWTRIX_LAT", "19.4326", "User")
[System.Environment]::SetEnvironmentVariable("AWTRIX_LON", "-99.1332", "User")
```

#### Linux / Raspberry Pi (Bash)

```bash
# Configurar para la sesión actual
export OPENWEATHER_API_KEY="tu_api_key"
export AWTRIX_IP="192.168.1.108"
export AWTRIX_LAT="19.4326"
export AWTRIX_LON="-99.1332"

# Configurar permanentemente (agregar a ~/.bashrc)
echo 'export OPENWEATHER_API_KEY="tu_api_key"' >> ~/.bashrc
echo 'export AWTRIX_IP="192.168.1.108"' >> ~/.bashrc
echo 'export AWTRIX_LAT="19.4326"' >> ~/.bashrc
echo 'export AWTRIX_LON="-99.1332"' >> ~/.bashrc
source ~/.bashrc
```

#### Variables disponibles

| Variable | Descripción | Ejemplo |
|----------|-------------|---------|
| `OPENWEATHER_API_KEY` | API key de OpenWeather (requerida) | `e6d10b15...` |
| `AWTRIX_IP` | IP de tu AWTRIX 3 | `192.168.1.108` |
| `AWTRIX_LAT` | Latitud de tu ubicación | `19.4326` |
| `AWTRIX_LON` | Longitud de tu ubicación | `-99.1332` |

**Obtener coordenadas:** Abre Google Maps, haz clic derecho en tu ubicación, copia las coordenadas.

**API Key:** Crea una gratis en https://openweathermap.org/api

### Uso

#### Opción 1: Script PowerShell (Windows)

```powershell
cd D:\Documents\Raspi\awtrix\awtrix3\flows
.\test-aqi.ps1
```

#### Opción 2: Script Bash (Linux/Raspberry Pi)

```bash
cd /path/to/awtrix3/flows
chmod +x test-aqi.sh
./test-aqi.sh
```

#### Opción 3: Node-RED (recomendado para automatización)

1. Abre Node-RED: `http://localhost:1880`
2. Menú → Import → Clipboard
3. Pega el contenido de `aqi-flow.json`
4. Edita el nodo "Config" con tus datos
5. Deploy

### Escala de colores ICA

| ICA | Estado | Color | Significado |
|-----|--------|-------|-------------|
| 0-50 | Bueno | Verde | Calidad satisfactoria |
| 51-100 | Moderado | Amarillo | Aceptable |
| 101-150 | Insalubre sensibles | Naranja | Grupos sensibles pueden tener efectos |
| 151-200 | Insalubre | Rojo | Todos pueden tener efectos |
| 201-300 | Muy insalubre | Morado | Alerta de salud |
| 301+ | Peligroso | Marrón | Emergencia |

### Cómo funciona

1. Obtiene datos de [OpenWeather Air Pollution API](https://openweathermap.org/api/air-pollution)
2. Extrae PM2.5 (partículas finas)
3. Calcula ICA usando escala US EPA
4. Envía a AWTRIX 3 via HTTP API con color según nivel

### Automatizar con Task Scheduler (Windows)

1. Abre "Programador de tareas"
2. Crear tarea básica
3. Nombre: "AWTRIX AQI"
4. Trigger: Repetir cada 10 minutos
5. Acción: Iniciar programa
   - Programa: `powershell.exe`
   - Argumentos: `-ExecutionPolicy Bypass -File "D:\Documents\Raspi\awtrix\awtrix3\flows\test-aqi.ps1"`

### Automatizar con cron (Linux)

```bash
# Editar crontab
crontab -e

# Agregar línea (cada 10 minutos)
*/10 * * * * /path/to/awtrix3/flows/test-aqi.sh
```

### Solución de problemas

**"No se recibieron datos"**
- Verifica tu API key de OpenWeather
- Verifica las coordenadas

**"Error al enviar"**
- Verifica la IP de AWTRIX 3
- Asegúrate que AWTRIX esté encendido y en la misma red

**El script no ejecuta (Windows)**
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```
