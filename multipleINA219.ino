#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;
Adafruit_INA219 second_ina219(0x41);

void setup(void) 
{
  Serial.begin(115200);
  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }
  Serial.print line(F(" Compiled " __DATE__ " at " __TIME__ "File: " __FILE__ ));
  //uint32_t currentFrequency;
    
  //Serial.println("Hello!");
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find first INA219 chip");
    while (1) { delay(10); }
  }

  if (! second_ina219.begin() ) {
    Serial.println("Failed to find first INA219 chip");
    while (1) { delay(10); }      
  }
  
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  Serial.println("Measuring voltage and current with INA219 ...");
}

void measure_firstINA219() {
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("First Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("First Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("First Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("First Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("First Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println();
  Serial.println();

  delay(5000);  
}

void measure_secondINA219() {
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = second_ina219.getShuntVoltage_mV();
  busvoltage = second_ina219.getBusVoltage_V();
  current_mA = second_ina219.getCurrent_mA();
  power_mW = second_ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Second Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Second Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Second Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Second Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Second Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println();
  Serial.println();

  delay(5000);  
}

void loop(void) 
{
  measure_firstINA219();
  measure_secondINA219();
}
