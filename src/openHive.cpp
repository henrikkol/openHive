#define TINY_GSM_MODEM_SIM800 // Modem is SIM800
//#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

#define LOADCELL_DOUT_PIN 2
#define LOADCELL_SCK_PIN 3

#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 60          /* Time ESP32 will go to sleep (in seconds) */

#include <Arduino.h>
#include <HX711.h>
#include <TinyGsmClient.h>

const long LOADCELL_OFFSET = 50682624;
const long LOADCELL_DIVIDER = 5895655;

HX711 scale;

RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR int stepCount = 0;
RTC_DATA_ATTR int hiveWeight[] = {};

void sendData()
{
    for (int i = 0; i <= stepCount; i++)
    {
        Serial.println("Hive weight:");
        Serial.println(stepCount + ": " + String(hiveWeight[i]) + "kg.");
    }
}

void readWeight(int stepCount)
{
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.power_up();
    delay(10);
    if (scale.is_ready())
    {
        scale.set_scale(LOADCELL_DIVIDER);
        scale.set_offset(LOADCELL_OFFSET);
        hiveWeight[stepCount] = int(scale.get_units());
    }
    else
    {
        /* could not read, possible hardware failure */
    }

    if (stepCount == 10)
    {
        sendData();
    }
}

void setup()
{
    Serial.begin(115200);

    delay(1000); //Take some time to open up the Serial Monitor

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
