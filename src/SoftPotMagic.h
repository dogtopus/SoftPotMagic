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

#pragma once

#include "Arduino.h"

const int POS_MIN = 0;
const int POS_MAX = 254;
const int POS_FLOAT = 255;

const int RES_INF = -1;
const int RES_OVF = -2;
const int RES_UNF = -3;

typedef struct {
    int leftMin;
    int leftMax;
    int rightMin;
    int rightMax;
    int zeroLevel;
} calib_t;

typedef struct {
    int left;
    int right;
} adc_value_t;

class c_SoftPotMagic {
public:
    c_SoftPotMagic(void);
    void begin(int adcLeft, int adcRight, int (*adcf)(uint8_t) = analogRead);
    void update(void);

    uint8_t pos1(void);
    uint8_t pos2(void);

    uint8_t gapCenter(void);
    uint8_t gapSize(void);

    int leftADC(void);
    int rightADC(void);

    //float leftMillimeter(void);
    //float rightMillimeter(void);

    void setCalib(const calib_t *calib);
    void setCalib(float rTestL, float rTestR, float rSoftPot, int adcMax, int adcZero);
    const calib_t *getCalib(void) const;
    bool autoCalibLeft(void);
    bool autoCalibRight(void);
    bool autoCalibZero(bool start = true);

    void setMinGapRatio(float ratio);
    float getMinGapRatio(void);
    int getMinGapRes(void);
private:
    adc_value_t _adcPins;
    adc_value_t _adcValues;
    adc_value_t _res;
    calib_t _calib;
    float _gapRatio;
    int __gapRatioRes;
    int (*_adcf)(uint8_t pin);
    inline int _leftADC(void);
    inline int _rightADC(void);
    inline bool _autoCalib(int &ladcMap, int &radcMap);
    inline uint8_t _pos1(void);
    inline uint8_t _pos2(void);
    inline void _gapRatio2Res(void);
};

extern c_SoftPotMagic SoftPotMagic;
