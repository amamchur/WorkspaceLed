
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
		if (length > middle) {
			timeout.schedule(0, go);
			return;
		}

		FL::clear();

		int i = middle;
		for (int k = length; k > 0 && i < FL::PixelCount; k--, i++) {
			FL::renderBuffer[i].green = 1;
			FL::renderBuffer[i].red = 1;
			FL::renderBuffer[i].blue = 1;
			FL::renderBuffer[i].white = 1;
		}

		i = middle;
		for (int k = length; k > 0 && i >= 0; k--, i--) {
			FL::renderBuffer[i].green = 1;
			FL::renderBuffer[i].red = 1;
			FL::renderBuffer[i].blue = 1;
			FL::renderBuffer[i].white = 1;
		}

		FL::display();
		timeout.schedule(0, startupAnimation, length + 1);
	}
};

#endif
