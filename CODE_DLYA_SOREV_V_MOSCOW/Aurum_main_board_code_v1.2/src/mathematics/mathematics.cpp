#include "mathematics.h"
#include <math.h>

#define MOTORANGLE 30
#define IRDATCHSENSOR 16
#define MOTOR1 0
#define MOTOR2 1
#define MOTOR3 2
#define MOTOR4 3 
#define DEGREE_TO_RADIAN 57.3

mathematics::mathematics(IRlocator &locator):m_locator(locator)
{
    mAngle[0] = 45;
		mAngle[1] = -45;
		mAngle[2] = -135;
		mAngle[3] = 135;
}

void mathematics::calculateSpeed(int16_t angle, int32_t maxSpeed, int32_t &sp1, int32_t &sp2, int32_t &sp3, int32_t &sp4)
{
	sp4 = cos((angle - mAngle[0])/DEGREE_TO_RADIAN) * maxSpeed;
	sp2 = cos((angle - mAngle[1])/DEGREE_TO_RADIAN) * maxSpeed;
	sp1 = cos((angle - mAngle[2])/DEGREE_TO_RADIAN) * maxSpeed;
	sp3 = cos((angle - mAngle[3])/DEGREE_TO_RADIAN) * maxSpeed;
}
