# Arduino Code
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
OneWire oneWire(2);
DallasTemperature temp_sensor(&oneWire);
float calibration_value = 21.34;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;
void setup()
{
  Serial.begin(9600);
  temp_sensor.begin();
}
StaticJsonBuffer<1000> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
void loop() {
  for (int i = 0; i < 10; i++)
  {
    buffer_arr[i] = analogRead(A0);
    delay(30);
  }
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++)
    avgval += buffer_arr[i];
  float volt = (float)avgval * 5.0 / 1024 / 6;
  float ph_act = -5.70 * volt + calibration_value;
  temp_sensor.requestTemperatures();
  int moisture_analog=analogRead(A1);
  int moist_act=map(moisture_analog,0,1023,100,0);
  root["a1"] = ph_act;
  root["a2"] = temp_sensor.getTempCByIndex(0);
  root["a3"] = moist_act;
  root.printTo(Serial);
  Serial.println("");
}
 
