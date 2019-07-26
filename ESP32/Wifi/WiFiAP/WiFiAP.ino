# include <WiFi.h>
# include <WiFiClient.h>
# include <WiFiAP.h>

const char * ssid = "ztc's_ESP32";
const char * passwd = "1qaz2wsx";

WiFiServer server(80);

void setup(){
    pinMode(25,OUTPUT);
    pinMode(26,OUTPUT);
    Serial.begin(115200);
    Serial.println("Start Configuring...");

    WiFi.softAP(ssid, passwd);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP Address:");
    Serial.println(myIP);
    Serial.print("Local IP");
    Serial.println(WiFi.localIP());

    server.begin();
    Serial.println("Server Started...");
}

void loop(){

    WiFiClient client = server.available();

    if (client){
        Serial.println("New Client..");
        String line;
        while(client.connected()){
            if (client.available()){
                char c = client.read();
                if (c == '!'){
                    client.print("S.H.I.T");
                }
                Serial.print("Received: ");
                Serial.println(c);
            }
        }

        client.stop();
        Serial.println("Client DisConnected...");

    }
}