#include "wifi.h"
#include <stdint.h>

#include "Particle.h"
#include "common.h"

#include "math.h"

SYSTEM_MODE(MANUAL);

struct SSID {
    char ssid[20];
    int8_t rssi;
    bool active;
};

SSID slug_ssids[] = {
    { "", -100, false },
    { "", -100, false },
    { "", -100, false },
    { "", -100, false },
    { "", -100, false }
};

void scanWifi() {
    for( uint8_t s=0; s<5; s++ )
        slug_ssids[s].active = false;

    WiFiAccessPoint aps[5]; // Max number of group is 5
    int found = WiFi.scan(aps, 5);
    for( uint8_t i=0; i<found; i++ ) {
        if( ! startsWith("Photon-", aps[i].ssid) )
            continue;

        // Find the exact ssid to adjust rssi averenge or replace the lower one
        int8_t current_ssid = -1;
        int8_t lower_rssi = -1;
        for( int8_t s=0; s<5; s++ ) {
            if( strcmp(slug_ssids[s].ssid, aps[i].ssid) != 0 ) {
                if( slug_ssids[s].rssi < slug_ssids[lower_rssi].rssi )
                    lower_rssi = s;
                continue;
            }

            lower_rssi = -1;
            current_ssid = s;
            slug_ssids[s].rssi = roundf(slug_ssids[s].rssi*0.666666 + aps[i].rssi*0.333333);
            Serial.printlnf("Updated SSID: %s", slug_ssids[current_ssid].ssid);
            slug_ssids[s].active = true;
            break;
        }
        if( lower_rssi > -1 ) {
            current_ssid = lower_rssi;
            memcpy(slug_ssids[current_ssid].ssid, aps[i].ssid, 20);
            slug_ssids[current_ssid].rssi = -100; // Start with not a good signal
            slug_ssids[current_ssid].active = true;
            Serial.printlnf("Found new SSID: %s", slug_ssids[current_ssid].ssid);
        }
        Serial.printlnf("SSID: '%s' RSSI_AVG: %i", aps[i].ssid, slug_ssids[current_ssid].rssi);
    }

    // Reducing signal for inactive ssids
    for( uint8_t s=0; s<5; s++ ) {
        if( ! slug_ssids[s].active and slug_ssids[s].rssi > -100 )
            slug_ssids[s].rssi = roundf(slug_ssids[s].rssi*0.666666 + -33.333333);
    }

    Serial.printlnf("Listening: %s", WiFi.listening() ? "True" : "Flase");
}

Timer scan_wifi_timer(2000, scanWifi);

void wifiScanStart() {
    scan_wifi_timer.start();
}

void wifiScanStop() {
    scan_wifi_timer.stop();
}

void wifiListen() {
    if( ! WiFi.listening() ) {
        Serial.println("Start listening");
        WiFi.listen();
    }
}
