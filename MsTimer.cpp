#include <MCURDK/Utils/Prescalers.hpp>
#include "Config.hpp"

volatile uint32_t Milliseconds = 0;

typedef MCURDK::Board::Timer0 MsTimer;
typedef MCURDK::Utils::PrescalerLE<MsTimer, 64>::Result MsPrescaler;
typedef Counter::IRQHandler<F_CPU, MsPrescaler::Value, MsTimer> IRQHandler;

ISR(TIMER0_OVF_vect) {
	IRQHandler::handleTimerOverflow();
}

void initializeMillisecondsTimer() {
	MsTimer::reset();
	MsTimer::mode<MCURDK::Periph::TimerMode::FastPWM8bit>();
	MsTimer::selectClockSource<MsPrescaler>();
	MsTimer::enableOverflowInterrupt();
}
