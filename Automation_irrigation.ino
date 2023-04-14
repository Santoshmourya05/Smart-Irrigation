#include <NTPClient.h>
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

/*const char ssid     = "OPPO A53s 5G";;
const char password = "1234567890";*/

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);




//Initialize the LCD display
//LiquidCrystal_I2C lcd(0x27, 16, 2);
char auth[] = "HiYHCUajk0XD717lchsDE9ICk9P1qP2o";
char ssid[] = "OPPO A53s 5G";                     //Enter your WIFI name
char pass[] = "1234567890";                       //Enter your WIFI password


BlynkTimer timer;
bool Relay = 0;

//Define component pins
#define sensor A0
#define waterPump D3
int TS;
int clk = timeClient.getHours();

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  pinMode(sensor, INPUT_PULLUP);
  Serial.begin(115200);

  WiFi.begin("OPPO A53s 5G", "1234567890");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  timeClient.begin();
  timer.setInterval(100L, soilMoistureSensor);
}

//Get the button value
BLYNK_WRITE(V1) {
  Relay = param.asInt();
  if (Relay == 1) {
    digitalWrite(waterPump, LOW);
    //lcd.setCursor(0, 1);
    //lcd.print("Motor is ON ");
  } else {
    digitalWrite(waterPump, HIGH);
    //lcd.setCursor(0, 1);
    //lcd.print("Motor is OFF");
  }
}
BLYNK_WRITE(V2) {
  TS = param.asInt();
}
//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V0, value);
  /*lcd.setCursor(0, 0);
  lcd.print("Moisture :");
  lcd.print(value);
  lcd.print(" ");*/
}
void timedRelay() {
  //clock.getTime();
  if (sensor == 1) {
    if(TS == 0) {
      digitalWrite(Relay, LOW);
    }
    else {
      switch (clk) {
        case 6:                       //when the clock reads 6 AM seconds (Once every 24 hours)
          digitalWrite(Relay, HIGH);  // turn the Relay on (HIGH is the voltage level)
          delay(20000);               // wait for 20 second
          digitalWrite(Relay, LOW);   // turn the Relay off by making the voltage LOW
          delay(3600000);             // wait for 3600 second
      }

      switch (clk) {
        case 20:                      //when the clock reads 8 PM seconds (Once every 24 hours)
          digitalWrite(Relay, HIGH);  // turn the Relay on (HIGH is the voltage level)
          delay(20000);               // wait for 20 second
          digitalWrite(Relay, LOW);   // turn the Relay off by making the voltage LOW
          delay(3600000);             // wait for 3600 second
      }
    }
  }else{
    digitalWrite(Relay,HIGH);
  }
}


void loop() {
  timeClient.update();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  delay(5000);

  //Serial.println(timeClient.getFormattedTime());
  delay(1000);
  Blynk.run();  //Run the Blynk library
  timer.run();  //Run the Blynk timer
}

