#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <MQUnifiedsensor.h>
//#include <Buzzer.h>




/************************Hardware Related Macros************************************/
#define         Board                   ("Adafruit QT Py ESP32-S3")
#define         Pin                     (A0)  //Analog input 3 of your Arduino

/***********************Software Related Macros************************************/
#define         Type                    ("MQ-2") //MQ2
#define         Voltage_Resolution      (3.3)
#define         ADC_Bit_Resolution      (10)
#define         RatioMQ2CleanAir        (9.83)

MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

float temperature;
float humidity;

#define MQ2_PIN A0 // Analog pin for MQ2
//#define BUZZER_PIN  3// Digital pin for Buzzer

Adafruit_BME280 bme; // I2C

//Buzzer buzzer(16);

void setup() {
  Serial.begin(115200); 

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Initialize BME280
  if (!bme.begin(0x76)) {
    Serial.println("Error. Check BME280 Connections");
    while (1);
  }

  // Initialize MQ-2 Gas Sensor
  Serial.println("MQ-2 warming up!");
  delay(20000);

  MQ2.setRegressionMethod(1);
  MQ2.setA(574.25);
  MQ2.setB(-2.222);

  MQ2.init();

  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ2.update();
    calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
    Serial.print(".");
  }
  MQ2.setR0(calcR0 / 10);
  Serial.println("  done!.");

  if (isinf(calcR0)) { Serial.println("Warning: Connection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while (1); }
  if (calcR0 == 0) { Serial.println("Warning: Connection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while (1); }

  MQ2.serialDebug(true);

  // Initialize Buzzer
  //pinMode(9, OUTPUT);
  //buzzer.begin(0);
}
void getTemperature() {
  temperature = bme.readTemperature();
  String temperatureString = String(temperature, 1) + " C";
  Serial.print("Temperature: ");
  Serial.println(temperatureString);
  display.print("Temp: ");
  display.println(temperatureString);
}

void getHumidity() {
  humidity = bme.readHumidity();
  String humidityString = String(humidity, 1) + " %";
  Serial.print("Humidity: ");
  Serial.println(humidityString);
  display.print("Hum: ");
  display.println(humidityString);
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);

  // Read and display BME280 data
  getTemperature();
  getHumidity();

  // Read MQ-2 Sensor
  int sensorValue = analogRead(MQ2_PIN);
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
 


  MQ2.update();
  MQ2.readSensor();
  MQ2.serialDebug();

  // Activate reading if smoke/alcohol detected
  if (sensorValue > 2800) {
    //digitalWrite(9, HIGH);
    Serial.println("Smoke/Alcohol Detected!");
    display.println("Detected!");

    // Buzzer code here
    //int time = 500;

    //buzzer.sound(NOTE_A3, 500); 
    //buzzer.sound(NOTE_A3, 500);
    //buzzer.sound(NOTE_A3, 500);
    //buzzer.sound(NOTE_F3, 375);
    //buzzer.sound(NOTE_C4, 125);

    
    
    //buzzer.end(2000);
  } else {
    int aaaaa=0 ;
    //digitalWrite(9, LOW);
  }

  display.display();
  delay(1000);
}


