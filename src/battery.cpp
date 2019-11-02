#include "battery.h"
#include "Particle.h"
#include "common.h"

extern bool headslug_battery_low;

extern void onBatteryLowAction();
extern void onBatteryOkAction();

uint32_t battery_last_low_state = 0;

void batterySetup() {
    pinMode(LOWBATTERY_PIN, INPUT_PULLUP); // On battery low
}

// We need to wait 30 sec of stable low battery to
// make sure it's not just high load voltage drop
void batteryCheck() {
    if( digitalRead(LOWBATTERY_PIN) == LOW ) {
        if( headslug_battery_low )
            return;
        if( battery_last_low_state == 0 )
            battery_last_low_state = Time.now();
        if( battery_last_low_state + 30 > Time.now() )
            return;
        headslug_battery_low = true;
        onBatteryLowAction();
    } else {
        if( battery_last_low_state != 0 )
            battery_last_low_state = 0;
        if( ! headslug_battery_low )
            return;
        headslug_battery_low = false;
        onBatteryOkAction();
    }
}
