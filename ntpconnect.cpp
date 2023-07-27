#include "ntpconnect.h"

NtpConnect::NtpConnect(const char* ssid, const char* password) : ssid_(ssid), password_(password),
    timeClient(ntpUDP, "pool.ntp.org", 7 * 3600) // Set offset time in seconds (GMT +7)
{
  weekDays[0] = "Sunday";
  weekDays[1] = "Monday";
  weekDays[2] = "Tuesday";
  weekDays[3] = "Wednesday";
  weekDays[4] = "Thursday";
  weekDays[5] = "Friday";
  weekDays[6] = "Saturday";

  months[0] = "January";
  months[1] = "February";
  months[2] = "March";
  months[3] = "April";
  months[4] = "May";
  months[5] = "June";
  months[6] = "July";
  months[7] = "August";
  months[8] = "September";
  months[9] = "October";
  months[10] = "November";
  months[11] = "December";
}

void NtpConnect::setup() {
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid_);
  WiFi.begin(ssid_, password_);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Initialize NTPClient to get time
  timeClient.begin();
}

void NtpConnect::update() {
  timeClient.update();
}

time_t NtpConnect::getEpochTime() {
  return timeClient.getEpochTime();
}

void NtpConnect::calculateDateTime(time_t epochTime, unsigned long &year, unsigned long &month, unsigned long &day, unsigned long &currentHours, unsigned long &currentMinutes, unsigned long &currentSeconds) {
  // Calculate time components from epochTime
  currentSeconds = epochTime % 60;
  epochTime /= 60;
  currentMinutes = epochTime % 60;
  epochTime /= 60;
  currentHours = epochTime % 24;
  epochTime /= 24;

  // Starting date for reference: January 1, 1970 (Unix Epoch)
  year = 1970;
  month = 1;
  day = 1;

  // Calculate the current date
  while (epochTime >= 365) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
      if (epochTime >= 366) {
        epochTime -= 366;
        year++;
      }
    } else {
      epochTime -= 365;
      year++;
    }
  }

  while (epochTime > 0) {
    unsigned long daysInMonth;
    switch (month) {
      case 2:
        daysInMonth = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 29 : 28;
        break;
      case 4:
      case 6:
      case 9:
      case 11:
        daysInMonth = 30;
        break;
      default:
        daysInMonth = 31;
    }

    if (epochTime >= daysInMonth) {
      epochTime -= daysInMonth;
      month++;
      if (month > 12) {
        month = 1;
        year++;
      }
    } else {
      day += epochTime;
      epochTime = 0;
    }
  }
}

void NtpConnect::printDateTime(time_t epochTime) {
  unsigned long year, month, day, currentHours, currentMinutes, currentSeconds;
  calculateDateTime(epochTime, year, month, day, currentHours, currentMinutes, currentSeconds);
  // Print the date in the desired format
  Serial.print(day);
  Serial.print(" ");
  Serial.print(months[month - 1]); // Month array is 0-based, so decrement month by 1
  Serial.print(" ");
  Serial.print(year);
  Serial.print(" , time : ");

  // Print the time in the desired format
  if (currentHours < 10) Serial.print("0");
  Serial.print(currentHours);
  Serial.print(":");
  if (currentMinutes < 10) Serial.print("0");
  Serial.print(currentMinutes);
  Serial.print(":");
  if (currentSeconds < 10) Serial.print("0");
  Serial.println(currentSeconds);
}
