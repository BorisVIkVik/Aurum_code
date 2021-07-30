#pragma once
#include <project_config.h>
#include <pins_setup.h>
#include <usart1.h>
class lowerBoard
{
	public:
		lowerBoard(Pin &tx, Pin &rx);
		void read();
		int8_t getValue();
	private:
		Pin m_tx, m_rx;
		int8_t m_lineState;
};
