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

#define POS_MIN 0
#define POS_MAX 254
#define POS_FLOAT 255

#define RES_INF -1
#define RES_OVF -2
#define RES_UNF -3

typedef struct {
    int leftMin;
    int leftMax;
    int rightMin;
    int rightMax;
} calib_t;

typedef struct {
    int left;
    int right;
} adc_value_t;

class c_SoftPotMagic {
public:
    c_SoftPotMagic(void);
    void begin(int adcLeft, int adcRight);
    void update(void);

    uint8_t pos1(void);
    uint8_t pos2(void);

    uint8_t blobPos(void);
    uint8_t blobSize(void);

    int leftADC(void);
    int rightADC(void);

    //float leftMillimeter(void);
    //float rightMillimeter(void);

    void setCalib(const calib_t *calib);
    void setCalib(float rTestL, float rTestR, float rSoftPot, int adcMax);
    calib_t *getCalib(void) const;
    bool autoCalibLeft(void);
    bool autoCalibRight(void);

    void setBlobThreshold(int threshold);
    int getBlobThreshold(void);
private:
    adc_value_t _adcPins;
    adc_value_t _adcValues;
    adc_value_t _res;
    calib_t _calib;
    int _blobThreshold;
    inline int _leftADC(void);
    inline int _rightADC(void);
    inline uint8_t _pos1(void);
    inline uint8_t _pos2(void);
};

extern c_SoftPotMagic SoftPotMagic;