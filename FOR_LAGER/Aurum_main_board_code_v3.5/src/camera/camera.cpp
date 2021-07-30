#include "camera.h"

Camera::Camera(Pin &tx, Pin &rx):m_tx(tx), m_rx(rx)
{
	m_tx.pinInit();
	m_rx.pinInit();
	Usart2::uartInit();
	sign = 1;
	koef = 1;
	gotSomething = false;
}

void Camera::readData()
{
	if(Usart2::available()>0)
	{
		m_temp = Usart2::read();
		if(m_temp == '-')
		{
			sign = -1;
		}
		else if (m_temp == '$' && gotSomething == true)
		{
			if(m_str * sign != -1)
			{
				m_yellow_angle = m_str * sign;
			}
			sign = 1;
			m_str = 0;
			koef = 1;
			gotSomething = false;
		}
		else if (m_temp == '%' && gotSomething == true)
		{
			if(m_str * sign != -1)
			{
				m_yellow_distance = m_str * sign;
			}
			sign = 1;
			m_str = 0;
			koef = 1;
			gotSomething = false;
		}
		else if (m_temp == '@' && gotSomething == true)
		{
			if(m_str * sign != -1)
			{
				m_blue_angle = m_str * sign;
			}
			sign = 1;
			m_str = 0;
			koef = 1;
			gotSomething = false;
		}
		else if (m_temp == '&' && gotSomething == true)
		{
			if(m_str * sign != -1)
			{
				m_blue_distance = m_str * sign;
			}
			sign = 1;
			m_str = 0;
			koef = 1;
			gotSomething = false;
		}
		else if (m_temp == '*')
		{
			m_side = m_str;
			sign = 1;
			m_str = 0;
			koef = 1;
			gotSomething = false;
		}
		else if (m_temp == '^')
		{
			m_line = m_str;
			sign = 1;
			m_str = 0;
			koef = 1;
			gotSomething = false;
		}
		else
		{
			m_str += (m_temp - '0') * koef;
			koef *= 10;
			gotSomething = true;
		}
	}	
}

int16_t Camera::getBlueAngle()
{
		return m_blue_angle;
}
int16_t Camera::getYellowAngle()
{
		return m_yellow_angle;
}
int16_t Camera::getYellowDistance()
{
	return m_yellow_distance;
}
int16_t Camera::getBlueDistance()
{
	return m_blue_distance;
}
//int8_t Camera::getLine()
//{
//	return m_line;
//}
int16_t Camera::getBlueGLX(Gyroscope &gyro)
{
	return m_blue_distance * sin(float(m_blue_angle + gyro.getChangedAngle()));
}
int16_t Camera::getBlueGLY(Gyroscope &gyro)
{
	return m_blue_distance * cos(float(m_blue_angle + gyro.getChangedAngle()));
}
int16_t Camera::getYellowGLX(Gyroscope &gyro)
{
	return m_yellow_distance * sin(float(m_yellow_angle + gyro.getChangedAngle()));
}
int16_t Camera::getYellowGLY(Gyroscope &gyro)
{
	return m_yellow_distance * cos(float(m_yellow_angle + gyro.getChangedAngle()));
}

int8_t Camera::getSide()
{
	return m_side;
}
//int8_t Camera::getSide()
//{
//	if(getBlueDistance() < getYellowDistance())
//	{
//		if(getBlueGLX() > 0)
//		{
//			return 1;
//		}
//		else
//		{
//			return -1;
//		}
//	}
//	else
//	{
//		if(getYellowGLX() > 0)
//		{
//			return -1;
//		}
//		else
//		{
//			return 1;
//		}
//	}
//}
