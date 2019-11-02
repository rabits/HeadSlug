// This #include statement was automatically added by the Particle IDE.
#include "battery.h"

/**
 * BrainSlug alien logic
 */

SYSTEM_THREAD(ENABLED);

#include "common.h"
#include "http.h"
#include "wifi.h"
#include "servo.h"
#include "buttons.h"
#include "voice.h"

/**
 * Global vars to track the state
 */
uint32_t headslug_mood_time = 0;
int8_t headslug_mood = 0;
bool headslug_on_head = false;
bool headslug_silent = false;
bool headslug_battery_low = false;
uint32_t headslug_charging_time = 0;
uint32_t headslug_last_task_time = 0;

/**
 * Event function
 */
void eventAction() {
    digitalWrite(D7, HIGH);
    delay(50);
    digitalWrite(D7, LOW);
    delay(200);

    // Battery low - need to complain about it
    if( headslug_battery_low ) {
        if( random(0, 5) == 1 )
            sayBatteryLow();
        return;
    }
    digitalWrite(D7, HIGH);
    delay(50);
    digitalWrite(D7, LOW);
    delay(200);

    // Do nothing during charging
    if( headslug_charging_time > 0 ) {
        if( headslug_charging_time < Time.now() )
            headslug_charging_time = 0; // Charging completed
        return;
    }
    digitalWrite(D7, HIGH);
    delay(50);
    digitalWrite(D7, LOW);
    delay(200);

    if( random(0, 15+5*headslug_mood) == 4 ) {
        rotateServo();
        delay(1000*random(2, 5));
        stopServo();
    }

    // Silent mode
    if( headslug_silent ) {
        return;
    }
    digitalWrite(D7, HIGH);
    delay(50);
    digitalWrite(D7, LOW);
    delay(200);

    // Decrease the mood
    /*if( headslug_mood > 0 ) {
        if( headslug_mood_time + headslug_mood * 30 < Time.now() ) {
            headslug_mood--;
            headslug_mood_time = Time.now();
            sayMood();
            return;
        }
    }*/
    digitalWrite(D7, HIGH);
    delay(50);
    digitalWrite(D7, LOW);
    delay(200);

    // If not on the head - complain about it
    if( ! headslug_on_head ) {
        if( random(0, 7) == 3 )
            sayPickUp();
        return;
    }
    digitalWrite(D7, HIGH);
    delay(50);
    digitalWrite(D7, LOW);
    delay(200);

    // Check if someone is around and play in "find him"
    /*if( random(0, 20) == 15 && headslug_last_task_time + 180 < Time.now() ) {
        // Check the other headslug is located
        if( wifiCheck() ) {
            headslug_last_task_time = Time.now();
            // TODO!
        }
    }*/

    // It's time to talk with the carrier and give him some tasks
    if( random(0, 20) == 2 && headslug_last_task_time + 180 + 180*headslug_mood < Time.now() ) {
        headslug_last_task_time = Time.now();
        rotateServo();

        sayTaskIntro();
        voiceWait();

        stopServo();

        int8_t steps = random(3,20);
        for( int8_t step = 0; step < steps; step++ ) {
            if( ! headslug_on_head )
                break;
            rotateServo();
            sayTask();
            voiceWait();
            stopServo();
            delay(3000);
        }

        rotateServo();

        sayTaskEnd();
        voiceWait();

        stopServo();
        return;
    }

    // Just say some random stuff
    if( random(0, 15+5*headslug_mood) == 11 ) {
        rotateServo();

        sayRandom();
        voiceWait();

        stopServo();
        return;
    }

    digitalWrite(D7, HIGH);
    delay(50);
    digitalWrite(D7, LOW);
}

/**
 * Button events
 */
void buttonOnHeadPressed() {
    digitalWrite(D7, HIGH);
    delay(20);
    digitalWrite(D7, LOW);
    headslug_on_head = true;
    headslug_charging_time = 0;
    voiceSetVolume();
    voiceStop();
    sayHello();
    wifiScanStart();
}

void buttonOnHeadReleased() {
    digitalWrite(D7, HIGH);
    delay(20);
    digitalWrite(D7, LOW);
    headslug_on_head = false;
    voiceSetVolume();
    voiceStop();
    sayPutBack();
    wifiScanStop();
}

void buttonTouchShort() {
    digitalWrite(D7, HIGH);
    delay(20);
    digitalWrite(D7, LOW);
    headslug_mood_time = Time.now();
    sayTouch();
}

void buttonTouchLong() {
    digitalWrite(D7, HIGH);
    delay(20);
    digitalWrite(D7, LOW);
    headslug_mood_time = Time.now();
    headslug_mood = headslug_mood < 2 ? (headslug_mood + 1) : 0;
    if( headslug_mood == 0 ) { // Crossing all the moods switching silent mode
        headslug_silent = !headslug_silent;
        saySilent();
    } else
        sayMood();
}

void onBatteryLowAction() {
    sayBatteryLow();
}

void onBatteryOkAction() {
    headslug_charging_time = Time.now()+3600*2; // A couple of hours of sleep
    sayBatteryCharging();
}

/**
 * Init and main loop
 */
Timer event_timer(5000, eventAction);

void setup() {
    Serial.begin(9600); // Debug messages

    servoSetup();
    buttonsSetup();
    batterySetup();
    voiceSetup();

    sayWakeUp();

    pinMode(D7, OUTPUT); // DEBUG LED

    event_timer.start();
}

void loop() {
    wifiListen();

    buttonsCheck();
    batteryCheck();

    delay(5);
}
