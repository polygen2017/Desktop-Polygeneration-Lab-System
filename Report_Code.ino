//link to the arduino libraries
#include <Wire.h>
#include <Adafruit_INA219.h>
 
//Create an instance/constructor for each device (sensors)
Adafruit_INA219 ina219_A;
Adafruit_INA219 ina219_D(0x41);

//define the ports on the Arduino to which the relays are connected.
  int relay1 = 2;
  int relay2 = 4;


void setup(void) 
{
  Serial.begin(115200);
  while (!Serial) {
      delay(1);
  }
 
  uint32_t currentFrequency;
  
  // Initialize the INA219 current sensors.
  ina219_A.begin();
  ina219_D.begin();
 
 
  Serial.println("Measuring voltage and current with INA219 ...");
 
  //set's up the initial states for the relays. LOW refers to open, as HIGH         refers to close
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, LOW);
  
  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, LOW);
}
 
void loop(void) 
{

//gives an initial reference – 0 – for each loop and for the different electrical properties

  float shuntvoltage_A = 0;
  float busvoltage_A = 0;
  float current_mA_A = 0;
  float loadvoltage_A = 0;
  float P_A = 0;
 
//measures the voltage on the sensor
  shuntvoltage_A = ina219_A.getShuntVoltage_mV();
//measures the bus voltage
  busvoltage_A = ina219_A.getBusVoltage_V();
//measures the current
  current_mA_A = ina219_A.getCurrent_mA();
//calculates the total voltage based on the previous measurements
  loadvoltage_A = busvoltage_A + (shuntvoltage_A / 1000);
//calculates the power based on the previous measurements
  P_A = current_mA_A*loadvoltage_A;
  
  float shuntvoltage_D = 0;
  float busvoltage_D = 0;
  float current_mA_D = 0;
  float loadvoltage_D = 0;
  float P_D = 0;
 
  shuntvoltage_D = ina219_D.getShuntVoltage_mV();
  busvoltage_D = ina219_D.getBusVoltage_V();
  current_mA_D = ina219_D.getCurrent_mA();
  loadvoltage_D = busvoltage_D + (shuntvoltage_D / 1000);
  P_D = current_mA_D*loadvoltage_D;
  
//presents the different measured and calculated properties in the monitor
  Serial.print("Bus Voltage A:   "); Serial.print(busvoltage_A); Serial.println(" V");

  Serial.print("Shunt Voltage A: "); Serial.print(shuntvoltage_A); Serial.println(" mV");

  Serial.print("Load Voltage A:  "); Serial.print(loadvoltage_A); Serial.println(" V");

  Serial.print("Current A:       "); Serial.print(current_mA_A); Serial.println(" mA");

  Serial.print("Power A:         "); Serial.print(P_A); Serial.println(" mW");

  Serial.println("");

  Serial.print("Bus Voltage D:   "); Serial.print(busvoltage_D); Serial.println(" V");

  Serial.print("Shunt Voltage D: "); Serial.print(shuntvoltage_D); Serial.println(" mV");

  Serial.print("Load Voltage D:  "); Serial.print(loadvoltage_D); Serial.println(" V");

  Serial.print("Current D:       "); Serial.print(current_mA_D); Serial.println(" mA");

  Serial.print("Power D:         "); Serial.print(P_D); Serial.println(" mW");

  Serial.println("");
 

// surplus power, capacitor not full charged, CHARGE CAPACITOR
  if (P_A > 17 && loadvoltage_D < 1.9){
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, LOW);
  Serial.print("CHARGING");
  Serial.println("");
  }
 
// power deficit, capacitor partially charged, USE CAPACITOR POWER - DSICHARGE
  else if (P_A < 9 && loadvoltage_D > 1.3){
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, HIGH);
  Serial.print("DISCHARGING");
  Serial.println("");
  }
 
  else {
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  Serial.print("GENERATOR ONLY. CAP DISCONNECTED");
  Serial.println("");
}
  
// Delay of half a second before the loop is carried out again
  delay(500);
}

