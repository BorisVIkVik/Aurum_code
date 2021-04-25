#pragma once 
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

class Pin
{
    public:
        Pin(uint16_t pinNumber,
            GPIO_TypeDef* GPIOx,
            GPIOMode_TypeDef GPIO_Mode,
						TIM_TypeDef* timer1,
						uint32_t channel1,
						uint32_t tim1,
						uint16_t GPIO_PinSource,
						uint32_t period,
						uint32_t prescaler);     
						void pinInit();
						GPIO_TypeDef* getGPIOx();
						uint16_t getPinNumber();
						void pwm(uint32_t pulse);
						void pwmInit();
						void setBit();
						void resetBit();
    private:
        uint16_t m_pinNumber;
        GPIO_TypeDef * m_GPIOx;
        GPIOMode_TypeDef m_mode;
				TIM_TypeDef* m_timer1;
				uint32_t m_channel1;
				uint32_t m_tim1;
				uint32_t m_period;
		    uint16_t m_GPIO_PinSource;
				uint8_t m_GPIO_AF_TIM;
				uint32_t m_prescaler;
};
