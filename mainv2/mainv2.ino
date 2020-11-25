#include <stdio.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_MCP4725.h>
#include<string.h>
#define FUNCTION 0 // 0=CONTINUOUS; 1=IR Measurement
#define CONT_CURRENT 20 //Continuous discharge current
// IEC current curve definition
#define LOW_TIME 5 
#define LOW_CURRENT 1*4096/5
#define HIGH_TIME 1
#define HIGH_CURRENT 2*4096/5
#define OFF_TIME 9

#define VoltagePin A0 // using voltage
#define CurrentPin A1
#define CoreTempPin A2

//char charRead;
char dataStr[100] = "";
char buffer[7];

int exitFlag = 0;
Adafruit_MCP4725 dac;
//Adafruit_MLX90614 mlx;
// TODO : implement mlx library (check), MCP4725 library (complete, check twice), serial print formatter (check)

void setup() {
  //Adafruit_MLX90614 mlx = Adafruit_MLX90614();
  Serial.begin(9600);
  //Serial.println("Adafruit MLX90614 test");  
  //mlx.begin();
  dac.begin();
  //b'1100010'
  // InterruptTimerBased(__interrupt, 100);
}

void setDischarge(float current){
    // Example - MCP4725.setVoltage(LOW_CURRENT*5/180)
    dac.setVoltage(current, false);
}

void __interrupt() {
  float Voltage = analogRead(A0)/1023.0 * 5;
  float Current = analogRead(A1)/1023.0 * 180.0;
  //float CoreTemp = MappingFunc(AnalogRead(A3))
  //float SurfTemp = i2cRead(MLX90614)
  //float CoreTemp = mlx.readObjectTempC();
  float CoreTemp = 0;
  float SurfTemp = 0;
  //float SurfTemp = mlx.readAmbientTempC();
  //Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  //Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  //Serial.Println("%f, %f, %f, %f" %Voltage %Current %CoreTemp %SurfTemp)
  //convert floats to string and assemble c-type char string for writing:
  /*dtostrf(Voltage, 5, 1, buffer); //5 is minimum width, 1 is precision; float value is copied onto buff
  strcat(dataStr, buffer); //add it to the end
  strcat( dataStr, ", "); //append the delimiter*/
  Serial.print(Voltage);
  Serial.print(", ");
  Serial.println(Current);
 
  //dtostrf(floatVal, minimum width, precision, character array);
  /*dtostrf(Current, 5, 1, buffer);  //5 is minimum width, 1 is precision; float value is copied onto buff
  strcat( dataStr, buffer); //append the converted float
  strcat( dataStr, ", "); //append the delimiter*/
  /*dtostrf(CoreTemp, 5, 1, buffer);  //5 is minimum width, 1 is precision; float value is copied onto buff
  strcat( dataStr, buffer); //append the converted float
  strcat( dataStr, ", "); //append the delimiter

  dtostrf(SurfTemp, 5, 1, buffer);  //5 is minimum width, 1 is precision; float value is copied onto buff
  strcat( dataStr, buffer); //append the converted float
  strcat( dataStr, 0); //terminate correctly 
  
  Serial.println(dataStr);*/
  
    if (Voltage < 2.5){
        Serial.println("Undervoltage, test complete, exiting");
        setDischarge(0);
        //exit(0);
    }
}


void loop(){
    setDischarge(LOW_CURRENT);
    Serial.println("Setting low current");
    for(int i = 0; i < LOW_TIME*10; ++i) {
      __interrupt();
      delay(100);
    }
    // delay(LOW_TIME*1000); //wait for a bit
    setDischarge(HIGH_CURRENT); // Set Idis high
    //delay(HIGH_TIME*1000); 
    Serial.println("Setting high current");
    for(int i = 0; i < HIGH_TIME*10; ++i) {
      __interrupt();
      delay(100);
    }
    setDischarge(0); // Turn Idis off
    //delay(OFF_TIME*1000);
    Serial.println("Setting off current");
    for(int i = 0; i < OFF_TIME*10; ++i) {
      __interrupt();
      delay(100);
    }

}
