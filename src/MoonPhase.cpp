#include "MoonPhase.h"
#include "Globals.h"
#include <time.h>

MoonPhase_ MoonPhase;

void MoonPhase_::setup() {
    DEBUG_PRINTLN("MoonPhase initialized");
}

void MoonPhase_::tick() {
    // Actualizar una vez por hora es suficiente
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 3600000 || lastUpdate == 0) {
        calculateCurrent();
        lastUpdate = millis();
    }
}

void MoonPhase_::calculateCurrent() {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    calculate(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
}

void MoonPhase_::calculate(int year, int month, int day) {
    // Calcular fecha juliana
    double jd = getJulianDate(year, month, day);

    // Referencia: Luna nueva conocida - 6 de enero del 2000
    double referenceJD = 2451550.1;

    // Dias desde la luna nueva de referencia
    double daysSinceNew = jd - referenceJD;

    // Numero de lunas nuevas desde referencia
    double newMoons = daysSinceNew / LUNAR_CYCLE;

    // Fase actual (0-29.53)
    double phaseDay = (newMoons - floor(newMoons)) * LUNAR_CYCLE;
    dayOfCycle = (int)round(phaseDay);

    // Calcular iluminacion (aproximada)
    // 0 y 29.5 = 0%, 14.75 = 100%
    double cyclePosition = phaseDay / LUNAR_CYCLE;
    if (cyclePosition <= 0.5) {
        illumination = cyclePosition * 2 * 100;
    } else {
        illumination = (1 - cyclePosition) * 2 * 100;
    }

    // Determinar fase
    if (dayOfCycle <= 1 || dayOfCycle >= 29) {
        phase = MOON_NEW;
    } else if (dayOfCycle <= 6) {
        phase = MOON_WAXING_CRESCENT;
    } else if (dayOfCycle <= 8) {
        phase = MOON_FIRST_QUARTER;
    } else if (dayOfCycle <= 13) {
        phase = MOON_WAXING_GIBBOUS;
    } else if (dayOfCycle <= 16) {
        phase = MOON_FULL;
    } else if (dayOfCycle <= 21) {
        phase = MOON_WANING_GIBBOUS;
    } else if (dayOfCycle <= 23) {
        phase = MOON_LAST_QUARTER;
    } else {
        phase = MOON_WANING_CRESCENT;
    }

    // Calcular dias hasta proximos eventos
    if (dayOfCycle < 15) {
        daysToFullMoon = 15 - dayOfCycle;
        daysToNewMoon = 30 - dayOfCycle;
    } else {
        daysToFullMoon = 45 - dayOfCycle;  // Proxima luna llena
        daysToNewMoon = 30 - dayOfCycle;
    }

    updatePhaseInfo();

    DEBUG_PRINTF("Moon: Day %d, Phase: %s, Illumination: %.1f%%",
                 dayOfCycle, phaseName.c_str(), illumination);
}

double MoonPhase_::getJulianDate(int year, int month, int day) {
    // Algoritmo para calcular fecha juliana
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;

    double jd = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;

    return jd;
}

void MoonPhase_::updatePhaseInfo() {
    switch (phase) {
        case MOON_NEW:
            phaseName = "Luna Nueva";
            phaseEmoji = "🌑";
            break;
        case MOON_WAXING_CRESCENT:
            phaseName = "Creciente";
            phaseEmoji = "🌒";
            break;
        case MOON_FIRST_QUARTER:
            phaseName = "Cuarto Creciente";
            phaseEmoji = "🌓";
            break;
        case MOON_WAXING_GIBBOUS:
            phaseName = "Gibosa Creciente";
            phaseEmoji = "🌔";
            break;
        case MOON_FULL:
            phaseName = "Luna Llena";
            phaseEmoji = "🌕";
            break;
        case MOON_WANING_GIBBOUS:
            phaseName = "Gibosa Menguante";
            phaseEmoji = "🌖";
            break;
        case MOON_LAST_QUARTER:
            phaseName = "Cuarto Menguante";
            phaseEmoji = "🌗";
            break;
        case MOON_WANING_CRESCENT:
            phaseName = "Menguante";
            phaseEmoji = "🌘";
            break;
    }
}

String MoonPhase_::getIconName() {
    // Nombres de iconos para el filesystem
    switch (phase) {
        case MOON_NEW:             return "moon_new";
        case MOON_WAXING_CRESCENT: return "moon_waxing_crescent";
        case MOON_FIRST_QUARTER:   return "moon_first_quarter";
        case MOON_WAXING_GIBBOUS:  return "moon_waxing_gibbous";
        case MOON_FULL:            return "moon_full";
        case MOON_WANING_GIBBOUS:  return "moon_waning_gibbous";
        case MOON_LAST_QUARTER:    return "moon_last_quarter";
        case MOON_WANING_CRESCENT: return "moon_waning_crescent";
        default:                   return "moon_unknown";
    }
}

uint32_t MoonPhase_::getPhaseColor() {
    // Color basado en iluminacion
    // Luna nueva = azul oscuro, Luna llena = amarillo brillante
    if (illumination < 25) {
        return 0x1A1A2E;  // Azul muy oscuro
    } else if (illumination < 50) {
        return 0x4A4A5E;  // Gris azulado
    } else if (illumination < 75) {
        return 0xC0C0C0;  // Plateado
    } else {
        return 0xFFFFAA;  // Amarillo claro (luna llena)
    }
}
