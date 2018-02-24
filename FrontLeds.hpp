#ifndef FRONT_LEDS_HPP
#define FRONT_LEDS_HPP

#include <MCURDK/IC/WS2812.hpp>
#include <string.h>

typedef struct Pixel {
	uint8_t green;
	uint8_t red;
	uint8_t blue;
	uint8_t white;
} Pixel;

template<class DIN, class MCU, class Tools, uint8_t Count>
class FrontLeds {
public:
	static constexpr uint8_t PixelCount = Count;

	typedef Pixel PixelType;
	typedef MCURDK::IC::TimingSK6812 Timing;
	typedef MCURDK::IC::TransmitterWS2812<DIN, MCU::Frequency, Timing> Transmitter;
	typedef MCURDK::IC::WS2812<Transmitter, typename Tools::Delay> IC;

	static void begin() {
		ic.begin();
	}

	static void clear() {
		memset(renderBuffer, 0, sizeof(renderBuffer));
	}

	static void display() {
		ic.send(renderBuffer, sizeof(renderBuffer));
	}

	static IC ic;
	static Pixel renderBuffer[PixelCount];
};

template<class DIN, class MCU, class Tools, uint8_t PixelCount>
Pixel FrontLeds<DIN, MCU, Tools, PixelCount>::renderBuffer[PixelCount];

#endif
