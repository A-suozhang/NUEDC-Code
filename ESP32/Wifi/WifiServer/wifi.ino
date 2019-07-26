// -----------------------------------------
// This Code : ESP32 Is The STA(Stationary)
// Connect To The AP(Access Point) which is the 
// mobile-phone hotspot, The ESP32 Acts As Server
// And The Remote(Local Jupyter Notebook) Act As 
// The Client
// -----------------------------------------
# include<WiFi.h>
# include<Arduino.h>

// Set Up The WIFI_ROUTER(AP)  Info
const char* ssid     = "awsl";
const char* password = "1qaz2wsx";

// Set web server port number to 80
WiFiServer server(80);


// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, HIGH);
  digitalWrite(output27, HIGH);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected."); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

// The Client Instance Is Used For The Server(ESP32) To Manipulate The Client :) So We Use The client.write() instead Of server.write()?
// I'm Really Uncertain About WHY The server.write() function has no Use
// But This Code indeed Could Communicate With The PC
bool send_data = true;
void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             
    Serial.println("New Client.");          // print a message out in the serial port
    while (client.connected()) {    // (client.connected() stays true, when there are unread data, even if the client has disconnected)

      // Start Sending Endless ! To Client
      if(send_data == true){
          client.write('!');  // Write Data From Local(ESP32) To The Client(Jupyter Notebook)
      }

      if (client.available()) {             // if there's bytes to read to the client,
        // Read Client's Respond
        char c = client.read();  
        if (c == 'A'){  // If Get 'A' , stop Sending
          send_data = false; 
        }      
        // server.write(c);  // ??? : Server.write Has NO EFFECT HERE ?
        client.write(c);
        Serial.write(c);   

      }                 // print it out the serial monitor
    }

  // After The client disconnected
  client.stop();
  Serial.println("Client disconnected.");
  Serial.println("");
  }
}
