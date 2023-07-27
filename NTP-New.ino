#include <Arduino.h>
#include "ntpconnect.h"

const char *ssid = "Your-SSID";
const char *password = "Your-Password";

NtpConnect ntpClient(ssid, password);

void setup() {
  Serial.begin(115200);

  ntpClient.setup();
}

void loop() {
  ntpClient.update();

  time_t epochTime = ntpClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);

  ntpClient.printDateTime(epochTime);

  delay(10000);
}
