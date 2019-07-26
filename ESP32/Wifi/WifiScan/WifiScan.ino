#include "WiFi.h"

int Selected_WiFi_Index;

void setup()
{
    Serial.begin(115200);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    Serial.println("Setup done");
}

void loop()
{
    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        Selected_WiFi_Index = 0;
        for (int i = n-1; i >= 0; i--) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);

            if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN){
                    Selected_WiFi_Index = i;
            }

        }
    }
    // The Selcted WiFi Is The Most Significant WiFi Which Is Public
    if (WiFi.encryptionType(Selected_WiFi_Index) == WIFI_AUTH_OPEN){
        Serial.println("The Selcted WiFi Is:");
        Serial.println(WiFi.SSID(Selected_WiFi_Index));
    }

    Serial.println("");

    // Wait a bit before scanning again
    delay(5000);
}