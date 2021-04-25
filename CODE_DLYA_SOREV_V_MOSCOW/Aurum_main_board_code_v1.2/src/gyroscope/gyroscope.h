#pragma once 
#include <project_config.h> 
#include <pins_setup.h>
#include "usart6.h"
class Gyroscope
{
	public:
		Gyroscope(Pin &tx, Pin &rx);
		void setAngle(int16_t angle);
		int32_t getImpact();
		int16_t getCurAngle();
		int16_t getChangedAngle();
		void calcStartingAngle();
		void resetGyro();
		int16_t getStartingAngle();
		//int16_t getStartAngle();
	private:
		Pin m_tx;
		Pin m_rx;
		volatile int16_t m_yaw, m_err, m_errOld, m_startYaw, m_yawC;
		float m_p, m_i, m_d, m_u, kp, kd, ki, tu, ku;
			
};
