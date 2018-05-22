// Use this with serial plotter to get a nice plot of touch positions

#include "SoftPotMagic.h"


// Change this to set the interval of sampling data (5ms by default)
const int INTERVAL = 5;

// ADC pins used (A0 for left side and A1 for right side)
const int ADC_LEFT = 0;
const int ADC_RIGHT = 1;


void setup() {
  // Set the analog pins that you used to connect to the SoftPot (in this case,
  // A0 and A1)
  SoftPotMagic.begin(ADC_LEFT, ADC_RIGHT);

  Serial.begin(115200);

  // Do a manual calibration with 10kOhm test resistors and 10kOhm SoftPot (the
  // typical setup)
  SoftPotMagic.setCalib(10000.0, 10000.0, 10000.0, 1023);

  // Minimum gap ratio, any gaps detected that is bigger than specified will be
  // treated as multitouch
  SoftPotMagic.setMinGapRatio(0.10f);

}

void loop() {
  // Make sure to run this function to refresh the value
  SoftPotMagic.update();

  // Print positions
  Serial.print(SoftPotMagic.pos1());
  Serial.print(F(" "));
  Serial.println(SoftPotMagic.pos2());

  // Sleep for INTERVAL ms
  delay(INTERVAL);
}
