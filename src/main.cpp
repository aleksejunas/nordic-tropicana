/*********** 
 * Welcome to Nordic Tropicana's system for keeping track of and growing bananas and 
 * pineapples in greenhouses in Norway. 
 * 
 * Code commenting in English and documentation in Norwegian, since I'm just so used
 * to commenting code in English and I think it's good practice since you never know 
 * who you are going to work with on the code.
 ***********/

// TODO: Move the connection-secrets to a .env file and use the dotenv library to read the file


#include "common.h"

#define DHT11_PIN 5
#define RED_PIN 11
#define GREEN_PIN 10

// Commenting out code since button is sending device in infite reboot loop
// #define BUTTON_PIN 9
// int BUTTON_STATE = 0;



// OFFLINE/ONLINE LED SYSTEM (Buggy:  )

unsigned long previousMillis = 0;
const long interval = 1000;

void updateLed()
{

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    {
      previousMillis = currentMillis;
      if (WiFi.status() == WL_CONNECTED)
      {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, HIGH);
        analogWrite(GREEN_PIN, 255);
      }
      else if (WiFi.status() != WL_CONNECTED)
      {
        // WiFi.disconnect();
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(RED_PIN, HIGH);
        analogWrite(RED_PIN, 255);
      }
    }
  }
}


// Commented out since not working

/* void rebootButton() {

    BUTTON_STATE = digitalRead(BUTTON_PIN);

    if (BUTTON_STATE != HIGH) {
      Serial.println("Button Pressed, resetting...");
      delay(1000);
      Serial.println("5");
      delay(1000);
      Serial.println("Resetting in 5...");
      delay(1000);
      Serial.println("Resetting in 4...");
      delay(1000);
      Serial.println("Resetting in 3...");
      delay(1000);
      Serial.println("Resetting in 2...");
      delay(1000);
      Serial.println("Resetting in 1...");
      delay(1000);
      ESP.restart();
    }
} */

void setup()
{

  // Open serial for debugging
  Serial.begin(115200);
  delay(1500);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  // pinMode(BUTTON_PIN, INPUT_PULLUP);

  connectToWifi();
  rtcInit();
  ltrSensorInit();
  settingUpThings();
  // pushsaferSystemIsOnlineMessage();
}

void loop()
{

  updateLed();
  dht11Controller();
  lightController();
  updateTimeClient();
  // rebootButton();
  thing.handle();

  int chk = DHT11.read(DHT11_PIN);
}
