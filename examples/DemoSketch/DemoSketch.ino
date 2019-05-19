// SoftPotMagic Demo Sketch

#include "SoftPotMagic.h"

#define PAUSE() while (!Serial.available()); while (Serial.available()) Serial.read();


// ADC pins used (A0 for left side and A1 for right side)
const int ADC_LEFT = A0;
const int ADC_RIGHT = A1;


void autoCalib() {
  Serial.println(F("Running calibration routine"));
  // Do calibration for the left side
  while (1) {
    Serial.println(F("Press and hold the leftmost side of the SoftPot, then press Enter..."));
    PAUSE();
    // Do calibration. If there is an error, do it again until no error has been
    // reported
    if (SoftPotMagic.autoCalibLeft()) break;
  }
  // Same for the right side.
  // Note that you need to calibrate both sides in order to make the detection
  // properly working
  while (1) {
    Serial.println(F("Press and hold the rightmost side of the SoftPot, then press Enter..."));
    PAUSE();
    if (SoftPotMagic.autoCalibRight()) break;
  }

  // Find touch release threshold (might be necessary on some boards)
  Serial.println(F("Release all fingers and press Enter..."));
  PAUSE();
  SoftPotMagic.autoCalibZero();
  Serial.println(F("Calibration completed"));

  // This prints out the calibration data
  const calib_t *c = SoftPotMagic.getCalib();
  Serial.print(F("leftMin="));
  Serial.println(c->leftMin);
  Serial.print(F("leftMax="));
  Serial.println(c->leftMax);
  Serial.print(F("rightMin="));
  Serial.println(c->rightMin);
  Serial.print(F("rightMax="));
  Serial.println(c->rightMax);
}

void setup() {
  // Set the analog pins that you used to connect to the SoftPot (in this case,
  // A0 and A1)
  SoftPotMagic.begin(ADC_LEFT, ADC_RIGHT);

  Serial.begin(115200);

  // Run the calibration routine once
  // You can also load an already saved calibration data from, for example,
  // an EEPROM
  autoCalib();

  // You can also manually calibrate it by specifying the resistance of your 2
  // test resistors and the total resistance of your SoftPot
  //SoftPotMagic.setCalib(10000.0, 10000.0, 10000.0, 1023);

  // Minimum gap ratio, any gaps detected that is bigger than specified will be
  // treated as multitouch
  SoftPotMagic.setMinGapRatio(0.10f);
}

void loop() {
  // Make sure to run this function to refresh the value
  SoftPotMagic.update();

  // Print positions
  Serial.print(F("pos1="));
  Serial.print(SoftPotMagic.pos1());
  Serial.print(F(" pos2="));
  Serial.println(SoftPotMagic.pos2());

  // Print gap
  Serial.print(F("gapCenter="));
  Serial.print(SoftPotMagic.gapCenter());
  Serial.print(F(" gapSize="));
  Serial.println(SoftPotMagic.gapSize());

  // Print raw values returned from internal analogRead() that were used to
  // calculate the positions
  Serial.print(F("ADCL="));
  Serial.print(SoftPotMagic.leftADC());
  Serial.print(F(" ADCR="));
  Serial.println(SoftPotMagic.rightADC());

  delay(1000);
}
