#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HX711.h>


//Set your own wifi + mqtt broker here
const char* ssid = "my_ssid";
const char* password = "password";
const char* mqtt_server = "server_ip_adress";
const int mqtt_port = 1234;
const char* mqtt_user = "mqtt_user";
const char* mqtt_password = "mqtt_password";

WiFiClient espClient;
PubSubClient client(espClient);

#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 60          /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR int stepCount = 0;
RTC_DATA_ATTR int test[] = {};

void sendData()
{
  Serial.println("sending data to user");
  Serial.println("------------------------------------------------------------------------------------");
  Serial.println("------------------------------------------------------------------------------------");
  Serial.println("------------------------------------------------------------------------------------");
  Serial.println("------------------------------------------------------------------------------------");

  for (int i = 0; i <= stepCount; i++)
  {
    Serial.println("Random number: " + String(i) + " with value: " + String(test[i]));
  }
}

void readWeight(int stepCount)
{
  Serial.println("reading weight + stepCount " + String(stepCount));
  test[stepCount] = random(25);
  Serial.println("Weigt recorded: " + String(test[stepCount]));

  if (stepCount == 10)
  {
    sendData();
  }
}

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  delay(1000); //Take some time to open up the Serial Monitor
  Serial.println("connection to wifi..");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(10);
    Serial.print(" ... ");
  }
  

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  /*
    First we configure the wake up source
  */
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
                 " Seconds");

  ++stepCount;
  readWeight(stepCount);
  if (stepCount == 10)
  {
    stepCount = 0;
  }

  Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop()
{
  //This is not going to be called
}
