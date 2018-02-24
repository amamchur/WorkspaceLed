#ifndef WORKSPACE_LED_CONFIG_HPP
#define WORKSPACE_LED_CONFIG_HPP

#include <stdint.h>

#include <MCURDK.h>
#include <MCURDK/Board/ArduinoNano.hpp>
#include <MCURDK/IO/OutputStream.hpp>
#include <MCURDK/Utils/ToolSet.hpp>
#include <MCURDK/Utils/MillisecondsCounter.hpp>

extern volatile uint32_t Milliseconds;

typedef MCURDK::Utils::MillisecondsCounter<decltype(Milliseconds), &Milliseconds> Counter;
typedef MCURDK::Utils::ToolSet<MCURDK::Board::MCU, Counter> Tools;
typedef Tools::Delay Delay;

typedef MCURDK::Board::MCU::HardwareUSART0<16, 8> USART;

extern MCURDK::IO::OutputStream<USART> cout;

void initializeMillisecondsTimer();
void initializeUSART();

#endif
