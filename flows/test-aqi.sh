#!/bin/bash
# ==============================================
# Script de prueba para ICA/AQI en AWTRIX 3
# ==============================================

# ========== CONFIGURACIÓN ==========
# Usar variables de entorno o valores por defecto

LAT="${AWTRIX_LAT:-19.4326}"
LON="${AWTRIX_LON:--99.1332}"
API_KEY="${OPENWEATHER_API_KEY:-}"
AWTRIX_IP="${AWTRIX_IP:-192.168.1.108}"

# ======================================

if [ -z "$API_KEY" ]; then
    echo "ERROR: Falta la variable de entorno OPENWEATHER_API_KEY"
    echo ""
    echo "Configurar con:"
    echo '  export OPENWEATHER_API_KEY="tu_api_key"'
    echo '  export AWTRIX_IP="192.168.1.100"'
    echo '  export AWTRIX_LAT="19.4326"'
    echo '  export AWTRIX_LON="-99.1332"'
    echo ""
    echo "O agregar a ~/.bashrc para que sea permanente"
    exit 1
fi

echo "Obteniendo datos de calidad del aire..."
echo "Ubicación: $LAT, $LON"
echo ""

# Obtener datos de OpenWeather Air Pollution API
RESPONSE=$(curl -s "http://api.openweathermap.org/data/2.5/air_pollution?lat=$LAT&lon=$LON&appid=$API_KEY")

# Extraer PM2.5 (requiere jq)
if command -v jq &> /dev/null; then
    PM25=$(echo $RESPONSE | jq -r '.list[0].components.pm2_5')
    PM10=$(echo $RESPONSE | jq -r '.list[0].components.pm10')
    O3=$(echo $RESPONSE | jq -r '.list[0].components.o3')
    NO2=$(echo $RESPONSE | jq -r '.list[0].components.no2')
    AQI_OW=$(echo $RESPONSE | jq -r '.list[0].main.aqi')

    echo "=== Datos de OpenWeather ==="
    echo "PM2.5: $PM25 μg/m³"
    echo "PM10:  $PM10 μg/m³"
    echo "O3:    $O3 μg/m³"
    echo "NO2:   $NO2 μg/m³"
    echo "AQI OpenWeather: $AQI_OW (1=Bueno, 5=Muy malo)"
    echo ""

    # Calcular AQI US EPA basado en PM2.5
    if (( $(echo "$PM25 <= 12" | bc -l) )); then
        AQI=50
        COLOR="#00E400"
        STATUS="Bueno"
    elif (( $(echo "$PM25 <= 35.4" | bc -l) )); then
        AQI=100
        COLOR="#FFFF00"
        STATUS="Moderado"
    elif (( $(echo "$PM25 <= 55.4" | bc -l) )); then
        AQI=150
        COLOR="#FF7E00"
        STATUS="Insalubre sensibles"
    elif (( $(echo "$PM25 <= 150.4" | bc -l) )); then
        AQI=200
        COLOR="#FF0000"
        STATUS="Insalubre"
    elif (( $(echo "$PM25 <= 250.4" | bc -l) )); then
        AQI=300
        COLOR="#8F3F97"
        STATUS="Muy insalubre"
    else
        AQI=500
        COLOR="#7E0023"
        STATUS="Peligroso"
    fi

    echo "=== Resultado ICA ==="
    echo "ICA: $AQI"
    echo "Estado: $STATUS"
    echo "Color: $COLOR"
    echo ""

    # Enviar a AWTRIX 3
    echo "Enviando a AWTRIX 3 ($AWTRIX_IP)..."

    curl -X POST "http://$AWTRIX_IP/api/custom?name=aqi" \
        -H "Content-Type: application/json" \
        -d "{\"text\":\"ICA $AQI\",\"icon\":4080,\"color\":\"$COLOR\",\"duration\":10}"

    echo ""
    echo "Listo!"

else
    echo "Error: Se requiere 'jq' para parsear JSON"
    echo "Instalar con: sudo apt install jq"
    echo ""
    echo "Respuesta raw de API:"
    echo $RESPONSE
fi
