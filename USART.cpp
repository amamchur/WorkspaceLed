#include "Config.hpp"

typedef MCURDK::Board::MCU::USARTConfig<F_CPU, 57600> USARTCfg;

MCURDK::IO::OutputStream<USART> cout;

ISR(USART_RX_vect) {
	USART::handleRxIrq();
}

ISR(USART_UDRE_vect) {
	USART::handleTxIrq();
}

void initializeUSART() {
	USART::begin<USARTCfg>();
}
