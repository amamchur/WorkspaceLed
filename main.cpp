#include <MCURDK.h>
#include <MCURDK/Board/ArduinoNano.hpp>
#include <MCURDK/IO/Button.hpp>
#include <MCURDK/IC/WS2812.hpp>
#include <MCURDK/Utils/ToolSet.hpp>
#include <MCURDK/Utils/MillisecondsCounter.hpp>

#include <stdio.h>

#include "Config.hpp"
#include "BackLeds.hpp"
#include "FrontLeds.hpp"
#include "StartupAnimator.hpp"

using namespace MCURDK::Board;
using namespace MCURDK::IO;
using namespace MCURDK::IC;
using namespace MCURDK::GPIO;

void launch(int);

typedef typename Tools::template FunctionTimeout<8, int> Timeout;
Timeout timeout;

typedef BD09PWM SegL;
typedef BD10PWM SegM;
typedef BD11PWM SegR;
typedef BackLeds<SegL, SegM, SegR> Back;
typedef FrontLeds<BD05, MCURDK::Board::MCU, Tools, 134> Front;
typedef StartupAnimator<Front, Timeout, timeout, launch> Startup;

Button<BA03, Counter> button1;
Button<BA04, Counter> button2;
Button<BA00, Counter> button3;
Button<BA02, Counter> button4;
Button<BA05, Counter> button5;
Button<BA01, Counter> button6;

void initializeButtons() {
	button1.begin();
	button2.begin();
	button3.begin();
	button4.begin();
	button5.begin();
	button6.begin();
}

void launch(int) {
	Back::readState();
	Front::clear();
	Front::display();
}

void ledOn(int);

void ledOff(int) {
	BD13::low();
	timeout.schedule(500, ledOn);
}

void ledOn(int) {
	BD13::high();
	timeout.schedule(500, ledOff);
}

void setup() {
	initializeMillisecondsTimer();
	initializeUSART();
	initializeButtons();

	Back::begin();
	Front::begin();

	MCURDK::Utils::Interrupts::on();

	Startup::start();
	BD13::mode<PinMode::Output>();
	timeout.schedule(500, ledOn);
}

template<uint8_t no>
class ButtonHandler {
private:
	static void printButtonEvent(ButtonEvent event) {
		switch (event) {
		case ButtonEventDown:
			cout << "ButtonEventDown\r\n";
			break;
		case ButtonEventPress:
			cout << "ButtonEventPress\r\n";
			break;
		case ButtonEventUp:
			cout << "ButtonEventUp\r\n";
			break;
		}
	}
public:
	static void handle(ButtonEvent event) {
		cout << no << ": ";
		printButtonEvent(event);
	}
};

template<uint8_t no>
class LedModeHandler {
public:
	static void handle(ButtonEvent event) {
		if (event == ButtonEventPress) {
			Back::template nextValue<no>();
		}
	}
};

void handleButtons() {
	button1.handle(ButtonHandler<1>::handle);
	button2.handle(ButtonHandler<2>::handle);
	button3.handle(ButtonHandler<3>::handle);

	button4.handle(LedModeHandler<0>::handle);
	button5.handle(LedModeHandler<1>::handle);
	button6.handle(LedModeHandler<2>::handle);
}

void loop() {
	handleButtons();
	timeout.handle();
}

int main() {
	setup();

	cout << "Start" << endl;

	while (1) {
		loop();
	}
	return 0;
}
