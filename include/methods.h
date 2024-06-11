#include <Adafruit_LTR329_LTR303.h>
#include "connection_secrets.h"
#include <Pushsafer.h>
#include <WiFiClient.h>
#include <RTClib.h>
#include "dht11.h"

dht11 DHT11;
RTC_DS3231 RTC;
Adafruit_LTR329 LTR = Adafruit_LTR329();


/****** 
KEEPING TRACK OF TIME 
******/

String formattingTime(time_t t)
{
  char buf[9];
  sprintf(buf, "%02d:%02d:%02d", hour(t), minute(t), second(t));
  return String(buf);
}

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000); // Update interval of 1 minute

TimeChangeRule CET = {"CET", Last, Sun, Mar, 2, 60};    // UTC+1
TimeChangeRule CEST = {"CEST", Last, Sun, Oct, 3, 120}; // UTC+2
Timezone osloTime(CET, CEST);

/******* 
END OF KEEPING TRACK OF TIME 
******/


/******
 Since the values from the sensors are almost always too high
 or too low, the pushsafer methods are commented out to avoid
 being spammed to death with notifications.
******/

/******** 
 * A COLLECTION OF METHODS USED IN MAIN 
 * ******/

void pushsaferMessageWhenHumidityValuesAreTooLow()
{
    struct PushSaferInput input;
    input.message = "Humidity values are too low!";
    input.title = "Warning!";
    input.sound = "8";
    input.vibration = "1";
    input.icon = "1";
    input.iconcolor = "FFCCCCC";
    input.priority = "1";
    input.device = "a";
    input.url = "https://www.pushsafer.com";
    input.urlTitle = "Open Pushsafer.com";
    input.picture = "";
    input.picture2 = "";
    input.picture3 = "";
    input.time2live = "";
    input.retry = "";
    input.expire = "";
    input.confirm = "";
    input.answer = "";
    input.answeroptions = "";
    input.answerforce = "";

    Serial.println(pushsafer.sendEvent(input));
    Serial.println("Sent");
}

void pushsaferMessageWhenHumidityValuesAreTooHigh()
{
    struct PushSaferInput input;
    input.message = "Humidity values are too low!";
    input.title = "Warning!";
    input.sound = "8";
    input.vibration = "1";
    input.icon = "1";
    input.iconcolor = "FFCCCCC";
    input.priority = "1";
    input.device = "a";
    input.url = "https://www.pushsafer.com";
    input.urlTitle = "Open Pushsafer.com";
    input.picture = "";
    input.picture2 = "";
    input.picture3 = "";
    input.time2live = "";
    input.retry = "";
    input.expire = "";
    input.confirm = "";
    input.answer = "";
    input.answeroptions = "";
    input.answerforce = "";

    Serial.println(pushsafer.sendEvent(input));
    Serial.println("Sent");
}

void pushsaferMessageWhenTemperatureValuesAreTooHigh()
{
    struct PushSaferInput input;
    input.message = "Temperature values are too high!";
    input.title = "Warning!";
    input.sound = "8";
    input.vibration = "1";
    input.icon = "1";
    input.iconcolor = "FFCCCCC";
    input.priority = "1";
    input.device = "a";
    input.url = "https://www.pushsafer.com";
    input.urlTitle = "Open Pushsafer.com";
    input.picture = "";
    input.picture2 = "";
    input.picture3 = "";
    input.time2live = "";
    input.retry = "";
    input.expire = "";
    input.confirm = "";
    input.answer = "";
    input.answeroptions = "";
    input.answerforce = "";

    Serial.println(pushsafer.sendEvent(input));
    Serial.println("Sent");
}

void pushsaferMessageWhenTemperatureValuesAreTooLow()
{
    struct PushSaferInput input;
    input.message = "Temperature values are too low!";
    input.title = "Warning!";
    input.sound = "8";
    input.vibration = "1";
    input.icon = "1";
    input.iconcolor = "FFCCCCC";
    input.priority = "1";
    input.device = "a";
    input.url = "https://www.pushsafer.com";
    input.urlTitle = "Open Pushsafer.com";
    input.picture = "";
    input.picture2 = "";
    input.picture3 = "";
    input.time2live = "";
    input.retry = "";
    input.expire = "";
    input.confirm = "";
    input.answer = "";
    input.answeroptions = "";
    input.answerforce = "";

    Serial.println(pushsafer.sendEvent(input));
    Serial.println("Sent");
}

void dht11Controller()
{
    /******
     * Controls the automatic
     *  temperature system
     *                      ******/

    if (DHT11.temperature < 24)
    {
        // Serial.println();
        Serial.println("Warning: Temperature values too low, increasing temperature");
        // Serial.println();
        // pushsaferMessageWhenTemperatureValuesAreTooLow();
    }
    else if (DHT11.temperature > 29)
    {
        // Serial.println();
        Serial.println("Warning: Temperature values are too high, decreasing temperature");
        // Serial.println();
        // pushsaferMessageWhenTemperatureValuesAreTooHigh();
    }

    /******
     * Controls the automatic
     *  humidity system
     *                      ******/

    if (DHT11.humidity < 60)
    {
        // Serial.println();
        Serial.println("Warning: Humidity values too low, increasing ventalition");
        // Serial.println();
        // pushsaferMessageWhenHumidityValuesAreTooLow();
    }
    else if (DHT11.humidity > 70)
    {
        Serial.println();
        Serial.println("Warning: Humidity values are too high, increasing ventilation");
        Serial.println();
        // pushsaferMessageWhenHumidityValuesAreTooHigh();
    }
}

void lightController()
{
    uint16_t visible_ir = 0, infrared = 0;

    if (LTR.readBothChannels(visible_ir, infrared))
    {
        if (visible_ir < 2000)
        {
            // Serial.println();
            Serial.println("Warning: Light values too low, increasing values");
            // Serial.println();
        }
        else if (visible_ir > 4000)
        {
            // Serial.println();
            Serial.println("Warning: Light values too high, decreasing values");
            // Serial.println();
        }
    }
}

void connectToWifi()
{
    WiFi.disconnect();

    int retries = 0;
    int max_retries = 2;
    int delay_time = 1000;

    while (WiFi.status() != WL_CONNECTED && retries < max_retries)
    {
        Serial.println("Connecting to WiFi...");
        WiFi.begin(ssid, password);

        delay(delay_time);

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("WiFi connected");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
        }
        else
        {
            retries++;
            delay_time *= 2; // Double the delay time for each retry
            Serial.println("Connection attempt: ");
            Serial.print(retries);
            Serial.println("\n");
        }
    }

    if (retries >= max_retries)
    {
        Serial.println("Failed to connect to WiFi");
    }
}

void pushsaferSystemIsOnlineMessage()
{
    pushsafer.debug = true;

    struct PushSaferInput input;
    input.message = "Nordic Tropicana System is Online!";
    input.title = "SYSTEM IS ONLINE!";
    input.sound = "8";
    input.vibration = "1";
    input.icon = "1";
    input.iconcolor = "FFCCCCC";
    input.priority = "1";
    input.device = "a";
    input.url = "https://www.pushsafer.com";
    input.urlTitle = "Open Pushsafer.com";

    Serial.println(pushsafer.sendEvent(input));
    Serial.println("Sent");
}

void rtcInit()
{

    // ***** RTC *****
    if (!RTC.begin())
    {
        Serial.println("RTC not found!");
        while (1)
            ;
    }

    // Initialize the RTC module
    Wire.begin();
    RTC.begin();
    // Set the RTC to the current time
    RTC.adjust(DateTime(__TIME__, __DATE__));
}

void ltrSensorInit()
{
    if (!LTR.begin())
    {
        Serial.println("Couldn't find LTR sensor!");
        while (1)
            delay(10);
    }

    // Setup LTR sensor
    Serial.println("Found LTR sensor!");
    LTR.setGain(LTR3XX_GAIN_4);
    LTR.setIntegrationTime(LTR3XX_INTEGTIME_50);
    LTR.setMeasurementRate(LTR3XX_MEASRATE_50);
}

void settingUpThings()
{

    /******
     *  THINGS
     *
     *  (things that are
     *  connected to the
     *  thinger.io service)
     *
     *      ******/

    // Digital pin control example (i.e. turning on/off a light, a relay, configuring a paramater, etc)
    // thing["GPIO_16"] << digitalPin(16);

    thing["light"] >> [](pson &out)
    {
        uint16_t visible_plus_ir;
        uint16_t infrared;
        if (LTR.readBothChannels(visible_plus_ir, infrared))
        {
            out["visible_plus_ir"] = visible_plus_ir;
            out["infrared"] = infrared;
        }
    };

    thing["temperature"] >> [](pson &out)
    {
        out = DHT11.temperature;
    };

    thing["humidity"] >> [](pson &out)
    {
        out = DHT11.humidity;
    };
}

 void updateTimeClient() {
    timeClient.update();
  time_t localTime = osloTime.toLocal(timeClient.getEpochTime());
  Serial.print("Time: ");
  Serial.println(formattingTime(localTime));

  delay(10000);
  }

  