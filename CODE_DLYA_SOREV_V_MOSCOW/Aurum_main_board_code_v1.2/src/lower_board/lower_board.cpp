#include "lower_board.h"

lowerBoard::lowerBoard(Pin &tx, Pin &rx):m_tx(tx),m_rx(rx)
{
	m_tx.pinInit();
	m_rx.pinInit();
	Usart1::uartInit();
}
void lowerBoard::read()
{
	if(Usart1::available()>0)
	{
		m_lineState = Usart1::read();
	}
}

int8_t lowerBoard::getValue()
{
	return m_lineState;
}
