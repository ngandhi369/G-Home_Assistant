
#include <ESP8266WiFi.h>

#include "Adafruit_MQTT.h"

#include "Adafruit_MQTT_Client.h"
#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp8266.h>


#define Relay1            D1



#define WLAN_SSID       "PUT YOUR WIFI NAME HERE"             
// Your SSID

#define WLAN_PASS       "PUT YOUR WIFI PASS HERE"        
// Your password

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "PUT YOUR AUTH TOKEN HERE";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WIFI NAME";
char pass[] = "WIFI PASS";




/************************* Adafruit.io Setup *********************************/


#define AIO_SERVER      "io.adafruit.com"

#define AIO_SERVERPORT  1883                   
// use 8883 for SSL

#define AIO_USERNAME    "Adafruit username"            
// Replace it with your username
#define AIO_KEY         "AdaFruit Key"   
// Replace with your Project Auth Key



/************ Global State (you don't need to change this!) ******************/



// Create an ESP8266 WiFiClient class to connect to the MQTT server.

WiFiClient client;

// or... use WiFiFlientSecure for SSL

//WiFiClientSecure client;



// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


/****************************** Feeds ***************************************/




// Setup a feed called 'onoff' for subscribing to changes.

Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/led1"); 
// FeedName



void MQTT_connect();


void setup() 
{

  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

  Serial.begin(115200);

  
  pinMode(Relay1, OUTPUT);

  

  // Connect to WiFi access point.
  
  Serial.println(); 
  Serial.println();
  
  Serial.print("Connecting to ");
  
  Serial.println(WLAN_SSID);

  

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  
  while (WiFi.status() != WL_CONNECTED) 
  {

        delay(500);

          Serial.print(".");
  
  }
  
  Serial.println();

  

  Serial.println("WiFi connected");
  
  Serial.println("IP address: "); 
  
  Serial.println(WiFi.localIP());
 

  

  // Setup MQTT subscription for onoff feed.
  
  mqtt.subscribe(&Light1);


}



void loop() 
{
 
Blynk.run();  

  MQTT_connect();
  
  
  Adafruit_MQTT_Subscribe *subscription;
  
  while ((subscription = mqtt.readSubscription(5000))) {
    
    if (subscription == &Light1) {
      
            Serial.print(F("Got: "));
      
      Serial.println((char *)Light1.lastread);
      
            int Light1_State = atoi((char *)Light1.lastread);
                digitalWrite(Relay1, !(Light1_State));
      
    
    }
  
  }

}



void MQTT_connect() {
  
  int8_t ret;

  

  // Stop if already connected.
  
  if (mqtt.connected()) {
    
    return;
  
  }

  

  Serial.print("Connecting to MQTT... ");

  

  uint8_t retries = 3;
  
  

  while ((ret = mqtt.connect()) != 0) { 
  // connect will return 0 for connected
    

    Serial.println(mqtt.connectErrorString(ret));
    
    Serial.println("Retrying MQTT connection in 5 seconds...");
            mqtt.disconnect();
    
    delay(2000);  // wait 2 seconds
      
    retries--;
    
    if (retries == 0) {
      
      // basically die and wait for WDT to reset me
      
    while (1);
    
    }
  
  }
 
   Serial.println("MQTT Connected!");
 

}
//void setup() 
//{
  // put your setup code here, to run once:
  // Debug console
  //Serial.begin(9600);

//  Blynk.begin(auth, ssid, pass);

//}

//void loop() {
  // put your main code here, to run repeatedly:
  //Blynk.run();

//}
