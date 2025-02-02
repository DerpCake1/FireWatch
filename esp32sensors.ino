/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-temperature-humidity-sensor
 */

#include <DHT.h>
#define DHT_SENSOR_PIN 5 // ESP32 pin GPIO21 connected to DHT11 sensor
#define DHT_SENSOR_TYPE DHT11
#define ENCODER_SW 18
#define ENCODER_DT 19
#define ENCODER_CLK 21

volatile int encoderPos = 0; // Current position of the encoder
volatile bool encoderSwitchPressed = false; // Encoder switch state

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

void IRAM_ATTR encoderISR() {
  static bool lastCLK = LOW;
  bool currentCLK = digitalRead(ENCODER_CLK);

  // Check if the encoder is rotating
  if (lastCLK == LOW && currentCLK == HIGH) {
    if (digitalRead(ENCODER_DT) == HIGH) {
      encoderPos++;
    } else {
      encoderPos--;
    }
  }

  lastCLK = currentCLK;
}

void IRAM_ATTR switchISR() {
  encoderSwitchPressed = !encoderSwitchPressed;
}

void setup() {
  Serial.begin(9600);
  
  dht_sensor.begin(); // initialize the DHT sensor
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(ENCODER_SW, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), encoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_SW), switchISR, FALLING);

  Serial.println("Rotary Encoder Test");
}

void humid()
{
// read humidity
  float humi  = dht_sensor.readHumidity();
  // read temperature in Celsius
  float tempC = dht_sensor.readTemperature();
  // read temperature in Fahrenheit
  float tempF = dht_sensor.readTemperature(true);

  // check whether the reading is successful or not
  
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C  ~  ");
    Serial.print(tempF);
    Serial.println("°F");
  

  // wait a 2 seconds between readings
}

void rotary()
{
  // Print encoder position
  Serial.print("Encoder Position: ");
  Serial.println(encoderPos);

  // Print switch status
  if (encoderSwitchPressed) {
    Serial.println("Switch Pressed");
  } else {
    Serial.println("Switch Not Pressed");
  }

}
void loop() {
  humid();
  rotary();
  delay(2000);

}
