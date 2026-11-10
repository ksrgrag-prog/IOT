#define TRIG1 5
#define ECHO1 18

#define TRIG2 17
#define ECHO2 16

#define TOMBOL 21
#define LED 4
#define BUZZER 19

bool sistemAktif = false;
bool tombolSebelumnya = HIGH;

unsigned long waktuTekan = 0;

long bacaJarak(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long durasi = pulseIn(echo, HIGH, 30000);

  if (durasi == 0) return 0;

  return durasi * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);

  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  pinMode(TOMBOL, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  bool tombolSekarang = digitalRead(TOMBOL);

  if (tombolSebelumnya == HIGH &&
      tombolSekarang == LOW &&
      millis() - waktuTekan > 250) {

    sistemAktif = !sistemAktif;
    waktuTekan = millis();

    Serial.println(sistemAktif ? "SISTEM AKTIF" : "SISTEM MATI");
  }

  tombolSebelumnya = tombolSekarang;

  if (sistemAktif) {
    long jarak1 = bacaJarak(TRIG1, ECHO1);
    long jarak2 = bacaJarak(TRIG2, ECHO2);

    bool dekat =
      (jarak1 > 0 && jarak1 <= 30) ||
      (jarak2 > 0 && jarak2 <= 30);

    bool sangatDekat =
      (jarak1 > 0 && jarak1 <= 10) ||
      (jarak2 > 0 && jarak2 <= 10);

    if (dekat) {
      digitalWrite(LED, HIGH);
      delay(200);
      digitalWrite(LED, LOW);
      delay(200);
    } else {
      digitalWrite(LED, LOW);
    }

    if (sangatDekat) {
      tone(BUZZER, 1000);
    } else {
      noTone(BUZZER);
    }
  } else {
    digitalWrite(LED, LOW);
    noTone(BUZZER);
  }

  delay(50);
}
