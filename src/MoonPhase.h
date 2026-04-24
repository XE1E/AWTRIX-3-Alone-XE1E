#ifndef MOON_PHASE_H
#define MOON_PHASE_H

#include <Arduino.h>

// Fases de la luna
enum MoonPhaseType {
    MOON_NEW = 0,           // Luna nueva
    MOON_WAXING_CRESCENT,   // Creciente
    MOON_FIRST_QUARTER,     // Cuarto creciente
    MOON_WAXING_GIBBOUS,    // Gibosa creciente
    MOON_FULL,              // Luna llena
    MOON_WANING_GIBBOUS,    // Gibosa menguante
    MOON_LAST_QUARTER,      // Cuarto menguante
    MOON_WANING_CRESCENT    // Menguante
};

class MoonPhase_ {
public:
    void setup();
    void tick();

    // Calcular fase para una fecha especifica
    void calculate(int year, int month, int day);

    // Calcular para fecha actual (usa NTP time)
    void calculateCurrent();

    // Datos de la fase
    MoonPhaseType phase = MOON_NEW;
    int dayOfCycle = 0;         // Dia del ciclo (0-29)
    float illumination = 0;      // Porcentaje iluminado (0-100)
    String phaseName = "";       // Nombre en español
    String phaseEmoji = "";      // Emoji de la fase

    // Proximos eventos
    int daysToFullMoon = 0;
    int daysToNewMoon = 0;

    // Helpers
    String getIconName();
    uint32_t getPhaseColor();

private:
    static constexpr double LUNAR_CYCLE = 29.53059;  // Dias del ciclo lunar

    double getJulianDate(int year, int month, int day);
    void updatePhaseInfo();
};

extern MoonPhase_ MoonPhase;

#endif
