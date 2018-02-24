#ifndef BACK_LEDS_HPP
#define BACK_LEDS_HPP

#include <MCURDK/Utils/Prescalers.hpp>

#include <stdint.h>
#include <avr/eeprom.h>

template<class T, class U>
struct isSameTimer {
	static constexpr bool Value =  false;
};

template<class T>
struct isSameTimer<T, T> {
	static constexpr bool Value =  true;
};

template<class T, bool init>
struct TimerInitializer {
	typedef typename MCURDK::Utils::PrescalerLE<T, 1>::Result TimerPrescaler;

	static void initialize() {
		T::reset();
		T::template mode<MCURDK::Periph::TimerMode::FastPWM8bit>();
		T::template selectClockSource<TimerPrescaler>();
	}
};

template<class T>
struct TimerInitializer<T, false> {
	static void initialize() {
	}
};

template<class SegL, class SegM, class SegR, void * BackLedsMemOffset = (void *) 0x00>
class BackLeds {
public:
	template <uint8_t index, class DUMMY = void>
	class Segment {
	};

	template <class DUMMY>
	class Segment<0, DUMMY> : public SegL {
	};

	template <class DUMMY>
	class Segment<1, DUMMY> : public SegM {
	};

	template <class DUMMY>
	class Segment<2, DUMMY> : public SegR {
	};

	static void begin() {
		constexpr bool LeM = isSameTimer<typename SegL::Timer, typename SegM::Timer>::Value;
		constexpr bool MeR = isSameTimer<typename SegM::Timer, typename SegR::Timer>::Value;
		constexpr bool ReL = isSameTimer<typename SegR::Timer, typename SegL::Timer>::Value;

		TimerInitializer<typename SegL::Timer, true>::initialize();
		TimerInitializer<typename SegM::Timer, !LeM>::initialize();
		TimerInitializer<typename SegR::Timer, !MeR & !ReL>::initialize();

		SegL::on();
		SegM::on();
		SegR::on();

		SegL::value(0);
		SegM::value(0);
		SegR::value(0);
	}

	static void saveState() {
		uint8_t data[3] = {
				(uint8_t) SegL::value(),
				(uint8_t) SegM::value(),
				(uint8_t) SegR::value()
		};

		eeprom_write_block(data, BackLedsMemOffset, sizeof(data));
	}

	static void readState() {
		uint8_t data[3];
		eeprom_read_block(data, BackLedsMemOffset, sizeof(data));

		SegL::value(data[0]);
		SegM::value(data[1]);
		SegR::value(data[2]);
	}

	static uint8_t nextValue(uint8_t current) {
		uint8_t next = PwdMode[PwdModeCount - 1];
		for (int i = PwdModeCount - 1; i >= 0; i--) {
			if (PwdMode[i] < current) {
				next = PwdMode[i];
				break;
			}
		}
		return next;
	}

	template<uint8_t segment>
	static void nextValue() {
		auto current = Segment<segment>::value();
		auto next = nextValue(current);
		Segment<segment>::value(next);
		saveState();
	}
private:
	static const uint8_t PwdMode[];
	static const uint8_t PwdModeCount;
};

template<class SegL, class SegM, class SegR, void * BackLedsMemOffset>
const uint8_t BackLeds<SegL, SegM, SegR, BackLedsMemOffset>::PwdMode[] = { 0, 5, 10, 30, 70, 150, 255 };

template<class SegL, class SegM, class SegR, void * BackLedsMemOffset>
const uint8_t BackLeds<SegL, SegM, SegR, BackLedsMemOffset>::PwdModeCount = sizeof(BackLeds<SegL, SegM, SegR>::PwdMode) / sizeof(BackLeds<SegL, SegM, SegR>::PwdMode[0]);

#endif
