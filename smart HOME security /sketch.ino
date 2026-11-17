#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <ESP32Servo.h>

// PIN KOMPONEN
#define PIR 27
#define LED 4
#define BUZZER 19
#define SERVO_PIN 18

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// SERVO
Servo kunci;

// KEYPAD 4x4
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {13, 14, 16, 17};
byte colPins[COLS] = {25, 32, 33, 23};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// PENGATURAN SISTEM
String password = "1234";
String inputPIN = "";

bool armed = true;
bool alarmActive = false;

void setup() {
  Serial.begin(115200);

  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();

  kunci.setPeriodHertz(50);
  kunci.attach(SERVO_PIN, 500, 2400);
  kunci.write(0);

  lcd.setCursor(0, 0);
  lcd.print("SMART SECURITY");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM READY");

  delay(2000);
  lcd.clear();
}

void loop() {

  char key = keypad.getKey();

  // SENSOR GERAK
  if (armed && digitalRead(PIR) == HIGH) {
    alarmActive = true;
  }

  // KEYPAD
  if (key) {

    if (key == '*') {
      inputPIN = "";
      lcd.clear();
    }

    else if (key == '#') {
      armed = true;
      alarmActive = false;
      inputPIN = "";

      kunci.write(0);
      noTone(BUZZER);
      lcd.clear();
    }

    else if (key >= '0' && key <= '9') {
      inputPIN += key;
    }

    if (inputPIN.length() == 4) {

      if (inputPIN == password) {

        armed = false;
        alarmActive = false;

        kunci.write(90);
        noTone(BUZZER);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PIN BENAR!");
        lcd.setCursor(0, 1);
        lcd.print("PINTU TERBUKA");

        delay(2000);
        inputPIN = "";
        lcd.clear();

      } else {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AKSES DITOLAK!");
  lcd.setCursor(0, 1);
  lcd.print("PIN SALAH!");

  for (int i = 0; i < 10; i++) {
    digitalWrite(LED, HIGH);
    tone(BUZZER, 1800);
    delay(250);

    digitalWrite(LED, LOW);
    tone(BUZZER, 800);
    delay(250);
  }

  noTone(BUZZER);
  digitalWrite(LED, LOW);

  inputPIN = "";
  lcd.clear();
}
    }
  }

  // KONDISI ALARM
  if (alarmActive) {

    digitalWrite(LED, HIGH);
    tone(BUZZER, 1000);

    lcd.setCursor(0, 0);
    lcd.print("GERAKAN TERDETEK");
    lcd.setCursor(0, 1);
    lcd.print("MASUKKAN PIN");

  }

  else {

    noTone(BUZZER);
    digitalWrite(LED, armed ? LOW : HIGH);

    lcd.setCursor(0, 0);
    lcd.print(armed ? "SYSTEM ARMED    " : "SYSTEM DISARMED ");

    lcd.setCursor(0, 1);
    lcd.print("PIN: ");
    lcd.print(inputPIN);
    lcd.print("            ");
  }

  delay(50);
}
