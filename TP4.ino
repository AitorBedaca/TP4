#include <Wire.h>
#include <Preferences.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include<DHT.h>

Preferences preferences;

#define DHTTYPE DHT11
#define DHTPIN 23

const int BTN1_PIN = 34;
const int BTN2_PIN = 35;

#define LED 25

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

int estado = 1;

int estado_boton1 = 1;
int estado_boton2 = 1;

int umbral = 30 ;
float temperatura;
bool btn1_state = LOW;
bool btn2_state = LOW;



DHT dht(DHTPIN, DHTTYPE);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

unsigned int umbral_g;

void setup() {
  preferences.begin("my-app", false);
  Serial.begin(115200);
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.display();
  pinMode(LED, OUTPUT);
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  umbral_g = preferences.getUInt("counter", 0);

}

void loop() {
  Serial.print("Current threshold value:");
  Serial.println (umbral_g);
  float temperatura = dht.readTemperature();
  int estado_btn1 = digitalRead (BTN1_PIN);
  int estado_btn2 = digitalRead (BTN2_PIN); 
  switch (estado) {
    case 1:
      if ((estado_btn1 == 0) && (estado_btn2 == 0)) {
        estado = 3;
      }
      umbral_g = umbral;
      preferences.putUInt("counter", umbral_g);
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.print("T actual: ");
      display.print(temperatura);
      display.setCursor(0, 20);
      display.print("T TH: ");
      display.print(umbral);
      display.display();

      break;
    case 2:
      if (estado_btn1 == 0 && estado_btn2 == 0) {
        estado = 4;
      }
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.print(umbral);
      display.display();
      switch (estado_boton1) {
        case 1:
          if (estado_btn1 == 0) {
            estado_boton1 = 2;
          }
          break;
        case 2:
          if (estado_btn1 == 0) {

          } else if (estado_btn1 == 1) {
            estado_boton1 = 1;
            umbral++;
          }
          break;
      }
      switch (estado_boton2) {
        case 1:
          if (estado_btn2 == 0) {
            estado_boton2 = 2;
          }
          break;
        case 2:
          if (estado_btn2 == 0) {

          } else if (estado_btn2 == 1) {
            estado_boton2 = 1;
            umbral--;
          }
          break;
      }
      break;
    case 3:
      if (estado_btn1 == 0 && estado_btn2 == 0) {

      } else if (estado_btn1 == 1 && estado_btn2 == 1) {
        estado = 2;
      }
      break;
    case 4:
      if (estado_btn1 == 0 && estado_btn2 == 0) {

      } else if (estado_btn1 == 1 && estado_btn2 == 1) {
        estado = 1;
      }
      break;
  }

  if (temperatura >= umbral) {
    digitalWrite(LED, HIGH);
    //Serial.println("PASA");
  }
  else {
    //Serial.println(" no PASA");
    digitalWrite(LED , LOW);
  }
  Serial.println(estado);
}
