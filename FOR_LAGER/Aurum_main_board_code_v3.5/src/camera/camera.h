#pragma once
#include <project_config.h>
#include <pins_setup.h>
#include <usart2.h>
#include <math.h>
#include <gyroscope.h>

class Camera
{
	public:
		Camera(Pin &tx, Pin &rx);
		void readData();
		int16_t getYellowGLX(Gyroscope &gyro);
		int16_t getYellowGLY(Gyroscope &gyro);
		int16_t getBlueGLX(Gyroscope &gyro);
		int16_t getBlueAngle();
		int16_t getYellowAngle();
		int16_t getBlueGLY(Gyroscope &gyro);
		bool getYellowLine();
		bool getBlueLine();
		int8_t getYellowSide();
		int8_t getBlueSide();
		int16_t getYellowDistance();
		int16_t getBlueDistance();
		//int8_t getLine();
		int8_t getSide();
	private:
		char m_temp;
		char m_str;
		int8_t koef;
		bool gotSomething;
		int8_t sign;
		Pin m_rx, m_tx;
		int8_t m_line;
		int16_t m_blue_angle;
		int16_t m_blue_distance;
		int16_t m_yellow_angle;
		int16_t m_yellow_distance;
		int8_t m_side;
};
