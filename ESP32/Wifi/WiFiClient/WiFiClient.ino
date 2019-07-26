# include <WiFi.h>
# include <Arduino.h>

const char * ssid = "awsl";
const char * passwd = "1qaz2wsx";

const char * host = "192.168.43.22";    // My PC's IP Address under The WiFi
const  int port = 100;

WiFiClient client;

void setup(){
    pinMode(25,OUTPUT);
    pinMode(26,OUTPUT);

    Serial.begin(115200);
    Serial.println("Connecting To...");

    WiFi.begin(ssid, passwd);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Successfully Connected To AP:");
    Serial.print("The IP Address Is:");
    Serial.println(WiFi.localIP());

    
}

void loop() {
    digitalWrite(25,HIGH);
    digitalWrite(26,HIGH);

    // client.print("Hello Server!");
    Serial.println("Connecting To HOST...");
    while (!client.connect(host, port)){    // Trying Util Connected
        delay(200);
        Serial.println("Connection Failed");
    }
    Serial.println("Successfully Connected To Server");

    while (client.connected()){
        // ? delay(1);  
        // ? I don't know why If Empty Here, My Net Debug Software(Script Communicator will freeze after sending The 1st Message)
        // ! It's My Debug Software's Bug, When not disconnected(client.stop()) from that term but reprogram
        // ! Needed To Reopen The TCPServer 
        // ! Must Use client.close()

        if (client.available()){

            // float f = client.parseFloat();
            // Serial.println(f);
            // client.println(f);

            String s = client.readStringUntil('!'); 
            // Read String Until Means Reading Until The '!' happens
            // Does Not Block If not get '!'
            Serial.println(s);
            client.println(s);

            // char c = client.read();
        
            // if (c == '!'){
            //     client.println("GOT !");
            // }
            // for (int i = 0;i<3;i++){
            //     client.write(c);
            // }

        }

    }
    // String Line = client.readStringUntil('A');
    // client.println(Line);

    
    delay(1000);
    client.stop();
    Serial.println("Closing Connection...");


}