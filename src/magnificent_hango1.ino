#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buzzerPin = 8;
const int setButtonPin = 6;
const int adjustButtonPin = 7;

// Time variables (Starts at 07:29:50 to test the alarm quickly)
int hours = 7;
int minutes = 29;
int seconds = 50;

// Alarm variables
int alarmHour = 7;
int alarmMinute = 30;
bool alarmEnabled = true;
bool alarmTriggered = false;

unsigned long previousMillis = 0;
const long interval = 1000; // 1 second interval

void setup() {
  lcd.init();                      
  lcd.backlight();
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(setButtonPin, INPUT_PULLUP);
  pinMode(adjustButtonPin, INPUT_PULLUP);
  
  lcd.setCursor(0, 0);
  lcd.print("Virtual Clock");
  lcd.setCursor(0, 1);
  lcd.print("Ready!");
  delay(1500);
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();

  // Handle time ticking every 1 second without blocking code
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    seconds++;
    
    if (seconds >= 60) {
      seconds = 0;
      minutes++;
      if (minutes >= 60) {
        minutes = 0;
        hours = (hours + 1) % 24;
      }
    }
  }

  // Display Time on Row 0
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  if (hours < 10) lcd.print("0");
  lcd.print(hours);
  lcd.print(":");
  if (minutes < 10) lcd.print("0");
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10) lcd.print("0");
  lcd.print(seconds);

  // Display Alarm Status on Row 1
  lcd.setCursor(0, 1);
  lcd.print("Alrm:");
  if (alarmHour < 10) lcd.print("0");
  lcd.print(alarmHour);
  lcd.print(":");
  if (alarmMinute < 10) lcd.print("0");
  lcd.print(alarmMinute);
  lcd.print(alarmEnabled ? " ON " : " OFF");

  // Button 1: Toggle Alarm ON/OFF
  if (digitalRead(setButtonPin) == LOW) {
    alarmEnabled = !alarmEnabled;
    delay(300); // Debounce
  }

  // Button 2: Manually adjust minutes forward (for testing or setting time)
  if (digitalRead(adjustButtonPin) == LOW) {
    alarmMinute += 1;
    if (alarmMinute >= 60) alarmMinute = 0;
    delay(300); // Debounce
  }

  // Alarm Logic
  if (alarmEnabled && hours == alarmHour && minutes == alarmMinute && !alarmTriggered) {
    soundAlarm();
    alarmTriggered = true;
  } else if (minutes != alarmMinute) {
    alarmTriggered = false;
  }
}

void soundAlarm() {
  for (int i = 0; i < 3; i++) {
    tone(buzzerPin, 1000);
    delay(200);
    noTone(buzzerPin);
    delay(200);
  }
}