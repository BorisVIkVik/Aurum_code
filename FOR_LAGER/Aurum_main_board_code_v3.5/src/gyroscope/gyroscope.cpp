#include "gyroscope.h"

int8_t sign(int32_t x)
{
	if(x < 0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

Gyroscope::Gyroscope(Pin &tx, Pin &rx):m_tx(tx), m_rx(rx)
{
	m_rx.pinInit();
	m_tx.pinInit();
	Usart6::usartInit();
	tu = 0.525;
	ku = 5.0;
	kp = ku;
	ki = 0.01;
	kd = (kp*8);
	m_yaw = 0;
	m_u = 0;
	m_errOld = 0;
	m_err = 0;
	m_startYaw = 0;
}
void Gyroscope::calcStartingAngle()
{
	bool ready = false;
	while(!ready)
	{
		if (Usart6::read() == 255)
		{
			while (!Usart6::available());
			int32_t tmpYaw = Usart6::read();
			if(tmpYaw <= 180)
			{
				m_startYaw = tmpYaw;
				m_startYaw *= 2;
				m_startYaw-=180;
				ready = true;
			}
    }
	}
}
void Gyroscope::setAngle(int16_t angle)
{
	
		if (Usart6::read() == 255)
    {
      while (!Usart6::available());
			int32_t tmpYaw = Usart6::read();
			if(tmpYaw <= 180)
			{
				m_yaw = tmpYaw;
				m_yaw *= 2;
				m_yaw -= 180;
			}
    }
    m_err = m_yaw + angle - m_startYaw;
		m_yawC = m_yaw;
		if(m_err < 2 && m_err > -2)
		{
			m_u = 0;
			m_i = 0;
		}
		else
		{
			
    while (m_err > 180)
    {
      m_err = m_err - 2*180;
    }
    while (m_err < -180)
    {
      m_err = m_err + 2*180;
    }
		
    m_p = m_err * kp;
		m_i = 0;
    m_d = (m_err - m_errOld) * kd;
    m_u = m_p + m_d + m_i;
		//m_u += sign(m_u) * 50;
    m_errOld = m_err;
	}
}
int32_t Gyroscope::getImpact()
{
	int32_t tmp = m_u;
	if (tmp > 4096) tmp = 4096;
	else if (tmp < -4096) tmp = 4096;
	return tmp;
}

void Gyroscope::resetGyro()
{
	m_p = 0;
	m_i = 0;
	m_d = 0;
	m_u = 0;
	m_errOld = 0;
	m_err = 0;
}

int16_t Gyroscope::getCurAngle()
{
	return m_yaw;
}
int16_t Gyroscope::getStartingAngle()
{
	return m_startYaw;
}
int16_t Gyroscope::getChangedAngle()
{
	return m_yawC;
}