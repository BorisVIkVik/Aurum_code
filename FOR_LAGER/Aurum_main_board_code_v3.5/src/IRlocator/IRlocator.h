#pragma once
#include <project_config.h>
#include <pins_setup.h>
#include <softI2C.h>
class IRlocator
{
	public:
		IRlocator(softI2C &irI2C, uint32_t addres);
		int16_t getAngle();
		int32_t getDistance();
	private:
		softI2C m_irI2C;
		uint32_t m_addres;
};
