#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR int stepCount = 0;
RTC_DATA_ATTR int test[] = {};

/*
  Method to print the reason by which ESP32
  has been awaken from sleep
*/

void setup() {
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

  /*
    Next we decide what all peripherals to shut down/keep on
    By default, ESP32 will automatically power down the peripherals
    not needed by the wakeup source, but if you want to be a poweruser
    this is for you. Read in detail at the API docs
    http://esp-idf.readthedocs.io/en/latest/api-reference/system/deep_sleep.html
    Left the line commented as an example of how to configure peripherals.
    The line below turns off all RTC peripherals in deep sleep.
  */
  //esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  //Serial.println("Configured all RTC Peripherals to be powered down in sleep");

  /*
    Now that we have setup a wake cause and if needed setup the
    peripherals state in deep sleep, we can now start going to
    deep sleep.
    In the case that no wake up sources were provided but deep
    sleep was started, it will sleep forever unless hardware
    reset occurs.
  */

  ++stepCount;
  readWeight(stepCount);
  if (stepCount == 10) {
    stepCount = 0;
  }

  Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void readWeight(int stepCount) {
  Serial.println("reading weight + stepCount " + String(stepCount));
  test[stepCount] = random(25);
  Serial.println("Weigt recorded: " + String(test[stepCount]));

  if (stepCount == 10) {
    sendData();
  }
}

void sendData() {
  Serial.println("sending data to user");
  Serial.println("------------------------------------------------------------------------------------");
  Serial.println("------------------------------------------------------------------------------------");
  Serial.println("------------------------------------------------------------------------------------");
  Serial.println("------------------------------------------------------------------------------------");

  for(int i = 0; i <= stepCount; i++){
    Serial.println("Random number: " + String(i) + " with value: " + String(test[i]));
  }
}


void loop() {
  //This is not going to be called
}
