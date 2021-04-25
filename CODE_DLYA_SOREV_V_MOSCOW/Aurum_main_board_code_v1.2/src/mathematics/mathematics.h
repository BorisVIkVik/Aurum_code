#pragma once
#include <project_config.h>
#include <IRlocator.h>

#define IRDATCHSENSOR 16

class mathematics//class for mathematics 
{
    public:
        mathematics(IRlocator &locator);
				void calculateSpeed(int16_t angle, int32_t maxSpeed, int32_t &sp1, int32_t &sp2, int32_t &sp3, int32_t &sp4);//function for calculating speed
				uint16_t calculateAngle();//function for calculating angle
    private:
        int32_t speed[4];
				int32_t mAngle[4];
        IRlocator m_locator;
        bool m_valueIR[IRDATCHSENSOR];
};


