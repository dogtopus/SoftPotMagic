// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include "Arduino.h"

const int POS_MIN = 0;
const int POS_MAX = 254;
const int POS_FLOAT = 255;

const int RES_INF = -1;
const int RES_OVF = -2;
const int RES_UNF = -3;

// Min and max are in terms of resistances or positions (not ADC values or voltages) so min values are bigger than max
// Also right touch position is reversed (right < left) compare to left touch position (left > right)
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
    bool autoCalibZero(uint16_t samples=1000);

    void setMinGapRatio(float ratio);
    float getMinGapRatio(void);
    void setMinGapRatioInt(uint16_t ratio);
    uint16_t getMinGapRatioInt(void);
    int getMinGapRes(void);
private:
    adc_value_t _adcValues;
    adc_value_t _res;
    calib_t _calib;
    uint8_t _adcLeft, _adcRight;
    uint16_t _gapRatio;
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
