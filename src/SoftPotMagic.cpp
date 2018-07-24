/*
 *  This file is part of SoftPortMagic.
 *
 *  SoftPortMagic is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SoftPortMagic is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SoftPortMagic.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "SoftPotMagic.h"

#define __AUTO_CALIB(ladc_map, radc_map) \
    int ladc = _leftADC(); \
    int radc = _rightADC(); \
    if (ladc == 0 || radc == 0) { \
        return false; \
    } \
    ladc_map = ladc; \
    radc_map = radc; \
    return true;

#define __RTOTALLEFT (_calib.leftMin - _calib.leftMax)
#define __RTOTALRIGHT (_calib.rightMin - _calib.rightMax)

// calculate relative resistance from an ADC value
static inline int __adc_to_res(int adc, int adc_minpos, int adc_maxpos, int zero) {
    // max position has the greatest resistance
    if (adc_minpos <= 0 || adc_maxpos <= 0 || adc < zero) {
        // resistance is infinity (i.e. circuit is open, no touch was detected)
        // or cannot be calculated due to lack of calibration data
        return RES_INF;
    } else if (adc < adc_maxpos) {
        // resistance overflows (greater than upper threshold)
        return RES_OVF;
    } else if (adc > adc_minpos) {
        // resistance underflows (less than lower threshold)
        return RES_UNF;
    }

    long tmp = ((long) adc_maxpos * adc_minpos) / adc - adc_maxpos;
    return (int) tmp;
}

// convert relative resistance to a position (0-254)
static inline uint8_t __res_to_pos(int res, int rtotal, bool invert) {
    int pos;
    switch (res) {
        case RES_INF:
            pos = POS_FLOAT;
            break;
        case RES_OVF:
            pos = POS_MAX;
            break;
        case RES_UNF:
            pos = POS_MIN;
            break;
        default:
            if (invert) {
                pos = (int) map(rtotal - res, 0, rtotal, POS_MIN, POS_MAX);
            } else {
                pos = (int) map(res, 0, rtotal, POS_MIN, POS_MAX);
            }
    }
    return pos;
}

c_SoftPotMagic::c_SoftPotMagic(void) : _gapRatio(0.10f) {
    _adcPins = {-1, -1};
    _res = {-1, -1};
    _adcValues = {0, 0};
    _calib = {-1, -1, -1, -1, 0};
}

void c_SoftPotMagic::begin(int adcLeft, int adcRight, int (*adcf)(uint8_t)) {
    _adcPins.left = adcLeft;
    _adcPins.right = adcRight;
    _adcf = adcf;
}

inline int c_SoftPotMagic::_leftADC(void) {
    return _adcf(_adcPins.left);
}

inline int c_SoftPotMagic::_rightADC(void) {
    return _adcf(_adcPins.right);
}

// update/scan values
void c_SoftPotMagic::update(void) {
    // save ADC values just in caser the user needs them
    _adcValues.left = _leftADC();
    _adcValues.right = _rightADC();
    // calculate relative resistance
    _res.left = __adc_to_res(_adcValues.left, _calib.leftMin, _calib.leftMax, _calib.zeroLevel);
    _res.right = __adc_to_res(_adcValues.right, _calib.rightMin, _calib.rightMax, _calib.zeroLevel);
}

inline uint8_t c_SoftPotMagic::_pos1(void) {
    return __res_to_pos(_res.left, __RTOTALLEFT, false);
}

inline uint8_t c_SoftPotMagic::_pos2(void) {
    return __res_to_pos(_res.right, __RTOTALRIGHT, true);
}

inline void c_SoftPotMagic::_gapRatio2Res(void) {
    __gapRatioRes = _gapRatio * __RTOTALRIGHT;
}

// position readers for multitouch mode (0-254, 255 if no touch detected)
uint8_t c_SoftPotMagic::pos1(void) {
    return _pos1();
}

uint8_t c_SoftPotMagic::pos2(void) {
    if (_res.right >= 0 && _res.left >= 0 && abs(__RTOTALRIGHT - _res.right - _res.left) < __gapRatioRes) {
        return POS_FLOAT;
    } else {
        return _pos2();
    }
}

// position and size reader for gap measurement mode
uint8_t c_SoftPotMagic::gapCenter(void) {
    int p1 = _pos1();
    int p2 = _pos2();
    if (p1 != POS_FLOAT && p2 != POS_FLOAT) {
        return (p1 + p2) / 2;
    } else {
        return POS_FLOAT;
    }
}

uint8_t c_SoftPotMagic::gapSize(void) {
    int p1 = _pos1();
    int p2 = _pos2();
    if (p1 != POS_FLOAT && p2 != POS_FLOAT) {
        return abs(p1 - p2);
    } else {
        return POS_FLOAT;
    }
}

// raw ADC readers
int c_SoftPotMagic::leftADC(void) {
    return _adcValues.left;
}

int c_SoftPotMagic::rightADC(void) {
    return _adcValues.right;
}

// import calibration data
void c_SoftPotMagic::setCalib(const calib_t *calib) {
    memcpy((void *) &_calib, (const void *) calib, sizeof(calib_t));
    _gapRatio2Res();
}

// manually calibrate the system with resistance of test resistors and the total resistance of the SoftPot (between pin 1 and pin 3)
void c_SoftPotMagic::setCalib(float rTestL, float rTestR, float rSoftPot, int adcMax, int adcZero) {
    _calib.leftMax = rTestL / (rTestL + rSoftPot) * adcMax;
    _calib.rightMax = rTestR / (rTestR + rSoftPot) * adcMax;
    _calib.leftMin = adcMax;
    _calib.rightMin = adcMax;
    _calib.zeroLevel = adcZero;
    _gapRatio2Res();
}

// export calibration data
const calib_t *c_SoftPotMagic::getCalib(void) const {
    return &_calib;
}

// auto calibration for both sides
bool c_SoftPotMagic::autoCalibLeft(void) {
    __AUTO_CALIB(_calib.leftMin, _calib.rightMax);
}

bool c_SoftPotMagic::autoCalibRight(void) {
    __AUTO_CALIB(_calib.leftMax, _calib.rightMin);
}

bool c_SoftPotMagic::autoCalibZero(bool start) {
    static int max = 0;

    if (start) max = 0;

    int ladc = _leftADC();
    int radc = _rightADC();
    if (ladc > _calib.leftMin || radc > _calib.rightMin) {
        // within the input range or have a really noisy zero
        return false;
    }
    if (ladc > max) max = ladc;
    if (radc > max) max = radc;
    _calib.zeroLevel = max * 2;
    return true;
}

// Minimum gap ratio (between 0 and 1)
void c_SoftPotMagic::setMinGapRatio(float ratio) {
    if (ratio >= 0.0f && ratio <= 1.0f) {
        _gapRatio = ratio;
        _gapRatio2Res();
    }
}

float c_SoftPotMagic::getMinGapRatio(void) {
    return _gapRatio;
}

// For debug purpose
int c_SoftPotMagic::getMinGapRes(void) {
    return __gapRatioRes;
}

c_SoftPotMagic SoftPotMagic;
