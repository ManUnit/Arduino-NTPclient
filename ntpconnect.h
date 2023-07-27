#ifndef NTPCONNECT_H
#define NTPCONNECT_H

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>

class NtpConnect {
public:
  NtpConnect(const char* ssid, const char* password);
  void setup();
  void update();
  time_t getEpochTime();
  void printDateTime(time_t epochTime);

private:
  const char* ssid_;
  const char* password_;
  WiFiUDP ntpUDP;
  NTPClient timeClient;
  String weekDays[7];
  String months[12];
  void calculateDateTime(time_t epochTime, unsigned long &year, unsigned long &month, unsigned long &day, unsigned long &currentHours, unsigned long &currentMinutes, unsigned long &currentSeconds);
};

#endif // NTPCONNECT_H
