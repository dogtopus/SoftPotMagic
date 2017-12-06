# SoftPotMagic

SoftPot multitouch (up to 2 positions) or blob detection library for Arduino.

## Usage

### Selecting Test Resistors

Two test resistors are required in order to make the circuit working properly.
They need to be chosen specifically for the application.
The resistance of the test resistors needs to be large enough
so that the SoftPot will not overheat when one or both of the ends are touched
(creates a short circuit between the center pin and one or two side pins) and
small enough so that the voltage difference measured from SoftPot is large
enough to calculate the position.

If in doubt and the total resistance of your SoftPot is 10Kohm or 20Kohm (they
are the most common type and sold on SparkFun, Adafruit, Digikey, Mouser, etc.
for a small quantity), use test resistors with resistance of 10Kohm.

**WARNING**: Bad choice of test resistors can cause inaccurate measurement or
permanent damage to the SoftPot/Arduino.

### Wiring

Connect your SoftPot and test resistors to the Arduino as follows:

![wiring](./wiring.png)

`Vref` is the reference voltage of the analog pins of your Arduino (5V pin for
5V Arduino boards and 3.3V/3V3 for 3.3V Arduino boards).

`ADC_LEFT` and `ADC_RIGHT` are the analog pins of your choice.

`R1` and `R2` are the test resistors and `RV1` is the SoftPot.

### Library

To use the library, you must initialize it first with
`SoftPotMagic.begin(<left_pin>, <right_pin>)` in the `setup()` function where
`<left_pin>` and `<right_pin>` are the analog pins of your choice.

A calibration also needs to be performed either by calling
`SoftPotMagic.autoCalibLeft()` when you
press and hold the left end of the SoftPot and `SoftPotMagic.autoCalibRight()`
for the right end, or by telling the resistance of your test resistors and the
total resistance of your SoftPot to the library by calling
`SoftPotMagic.setCalib(<RTestL>, <RTestR>, <RSoftPot>, <ADCMax>)`. (ADCMax is
the maximum value `analogRead()` can return (typically 1023 for Arduino)).
The calibration data can be backed up on a persistent storage (e.g. EEPROM).
You can get the pointer of the calibration data by using
`calib_t *calib = SoftPotMagic.getCalib()` and save it. Use
`SoftPotMagic.setCalib(calib)` to load calibration data to the library.

Use `SoftPotMagic.update()` to get the latest update for the positions (it
should be called periodically). You can then use `SoftPotMagic.pos1()` and
`SoftPotMagic.pos2()` to get the positions. In the case of blob
tracking/detection, use `SoftPotMagic.blobPos()` for the position of blob and
`SoftPotMagic.blSize()` for the size of it.
The returned value of these functions range from 0 (`POS_MIN`) to 254
(`POS_MAX`), 255 or `POS_FLOAT` is returned if no touch/blob was detected.

Refer to the example for more information.

## License

GPLv3+
