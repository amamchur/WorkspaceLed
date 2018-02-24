
#ifndef STARTUP_ANIMATOR_HPP
#define STARTUP_ANIMATOR_HPP

template<class FL, class T, T& timeout, void (*go)(typename T::Token)>
class StartupAnimator {
public:
	static void start() {
		timeout.schedule(0, startupAnimation, 1);
	}

	static void startupAnimation(int length) {
		const uint8_t middle = FL::PixelCount / 2;
		const uint8_t brightness = 5;

		if (length > middle) {
			timeout.schedule(0, go);
			return;
		}

		FL::clear();

		auto lengthInBytes = length * sizeof(typename FL::PixelType);
		auto offset = FL::renderBuffer + middle;
		memset(offset, brightness, lengthInBytes);

		offset = FL::renderBuffer + middle - length;
		memset(offset, brightness, lengthInBytes);

		FL::display();
		timeout.schedule(0, startupAnimation, length + 1);
	}
};

#endif
