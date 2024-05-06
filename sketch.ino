#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>

#define dht_pin  8

#define LDR_PIN  2

dht my_dht;

int LED_VERMELHA = 11;
int LED_AMARELA = 12;
int LED_VERDE = 13;
int som = 4;
int temperatura = 0x00;
int umidade = 0x00;
const float GAMMA = 0.7;
const float RL10 = 50;

LiquidCrystal_I2C lcd(0x27, 2, 1);

void setup(){
  pinMode(LED_VERMELHA,OUTPUT);
  pinMode(LED_AMARELA,OUTPUT);
  pinMode(LED_VERDE,OUTPUT);
  pinMode(som,OUTPUT);
  pinMode(A0,INPUT);
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  lcd.backlight();
}

void loop(){
  int analogValue = analogRead(A0);
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  
  my_dht.read22(dht_pin);
  temperatura = my_dht.temperature;
  
  umidade = my_dht.humidity;
  lcd.clear();
  lcd.print(temperatura);
  lcd.print(" C ");
  noTone(som);
  Serial.println(lux);
  if (temperatura >= 10 && temperatura <= 15){
    lcd.print("temp. ok");
  }else{
    lcd.print("temp. bad");
  }


  if (lux < 800){
    digitalWrite(LED_VERMELHA,LOW);
    digitalWrite(LED_AMARELA,LOW);
    digitalWrite(LED_VERDE,HIGH);
    lcd.setCursor(0,1);
    lcd.print(umidade);
    lcd.print("%");
    lcd.print(" de umidade");
  }
  else if (800 <= lux && lux <= 950){
    digitalWrite(LED_VERDE,LOW);
    digitalWrite(LED_VERMELHA,LOW);
    digitalWrite(LED_AMARELA,HIGH);
    lcd.setCursor(0,1);
    lcd.print("Amb. a meia luz");
  }else{
    digitalWrite(LED_VERDE,LOW);
    digitalWrite(LED_AMARELA,LOW);
    digitalWrite(LED_VERMELHA,HIGH);
    lcd.setCursor(0,1);
    lcd.print("Amb. muito claro");
    tone(som,1000);
  }
  delay(1000);
}
