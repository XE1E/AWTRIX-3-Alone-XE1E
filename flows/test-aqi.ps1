# ==============================================
# Script de prueba para ICA/AQI en AWTRIX 3
# Para Windows PowerShell
# ==============================================

# ========== CONFIGURACIÓN ==========
# Usar variables de entorno o valores por defecto

$LAT = if ($env:AWTRIX_LAT) { $env:AWTRIX_LAT } else { "19.4326" }
$LON = if ($env:AWTRIX_LON) { $env:AWTRIX_LON } else { "-99.1332" }
$API_KEY = if ($env:OPENWEATHER_API_KEY) { $env:OPENWEATHER_API_KEY } else { $null }
$AWTRIX_IP = if ($env:AWTRIX_IP) { $env:AWTRIX_IP } else { "192.168.1.108" }

# ======================================

if (-not $API_KEY) {
    Write-Host "ERROR: Falta la variable de entorno OPENWEATHER_API_KEY" -ForegroundColor Red
    Write-Host ""
    Write-Host "Configurar con:" -ForegroundColor Yellow
    Write-Host '  $env:OPENWEATHER_API_KEY = "tu_api_key"'
    Write-Host '  $env:AWTRIX_IP = "192.168.1.100"'
    Write-Host '  $env:AWTRIX_LAT = "19.4326"'
    Write-Host '  $env:AWTRIX_LON = "-99.1332"'
    Write-Host ""
    Write-Host "O configurar permanentemente en Variables de Entorno del Sistema"
    exit 1
}

Write-Host "Obteniendo datos de calidad del aire..." -ForegroundColor Cyan
Write-Host "Ubicacion: $LAT, $LON"
Write-Host ""

# Obtener datos de OpenWeather Air Pollution API
$url = "http://api.openweathermap.org/data/2.5/air_pollution?lat=$LAT&lon=$LON&appid=$API_KEY"
$response = Invoke-RestMethod -Uri $url -Method Get

# Extraer componentes
$pollution = $response.list[0]
$pm25 = $pollution.components.pm2_5
$pm10 = $pollution.components.pm10
$o3 = $pollution.components.o3
$no2 = $pollution.components.no2
$aqiOW = $pollution.main.aqi

Write-Host "=== Datos de OpenWeather ===" -ForegroundColor Yellow
Write-Host "PM2.5: $pm25 ug/m3"
Write-Host "PM10:  $pm10 ug/m3"
Write-Host "O3:    $o3 ug/m3"
Write-Host "NO2:   $no2 ug/m3"
Write-Host "AQI OpenWeather: $aqiOW (1=Bueno, 5=Muy malo)"
Write-Host ""

# Calcular AQI US EPA basado en PM2.5
function Get-AQI {
    param([double]$pm25)

    $breakpoints = @(
        @{low=0; high=12; aqiLow=0; aqiHigh=50},
        @{low=12.1; high=35.4; aqiLow=51; aqiHigh=100},
        @{low=35.5; high=55.4; aqiLow=101; aqiHigh=150},
        @{low=55.5; high=150.4; aqiLow=151; aqiHigh=200},
        @{low=150.5; high=250.4; aqiLow=201; aqiHigh=300},
        @{low=250.5; high=500.4; aqiLow=301; aqiHigh=500}
    )

    foreach ($bp in $breakpoints) {
        if ($pm25 -ge $bp.low -and $pm25 -le $bp.high) {
            $aqi = (($bp.aqiHigh - $bp.aqiLow) / ($bp.high - $bp.low)) * ($pm25 - $bp.low) + $bp.aqiLow
            return [math]::Round($aqi)
        }
    }
    return 500
}

$aqi = Get-AQI -pm25 $pm25

# Determinar color y estado
if ($aqi -le 50) {
    $color = "#00E400"; $status = "Bueno"
} elseif ($aqi -le 100) {
    $color = "#FFFF00"; $status = "Moderado"
} elseif ($aqi -le 150) {
    $color = "#FF7E00"; $status = "Insalubre sensibles"
} elseif ($aqi -le 200) {
    $color = "#FF0000"; $status = "Insalubre"
} elseif ($aqi -le 300) {
    $color = "#8F3F97"; $status = "Muy insalubre"
} else {
    $color = "#7E0023"; $status = "Peligroso"
}

Write-Host "=== Resultado ICA ===" -ForegroundColor Green
Write-Host "ICA: $aqi"
Write-Host "Estado: $status"
Write-Host "Color: $color"
Write-Host ""

# Enviar a AWTRIX 3
Write-Host "Enviando a AWTRIX 3 ($AWTRIX_IP)..." -ForegroundColor Cyan

$body = @{
    text = "ICA $aqi"
    icon = 4080
    color = $color
    duration = 10
} | ConvertTo-Json

try {
    Invoke-RestMethod -Uri "http://$AWTRIX_IP/api/custom?name=aqi" -Method Post -Body $body -ContentType "application/json"
    Write-Host "Listo! Revisa tu AWTRIX" -ForegroundColor Green
} catch {
    Write-Host "Error al enviar: $_" -ForegroundColor Red
}
