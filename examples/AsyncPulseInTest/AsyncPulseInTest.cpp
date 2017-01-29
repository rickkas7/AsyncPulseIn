#include "Particle.h"

#include "AsyncPulseIn.h"

// Initialize the AsyncPulseIn object to listen on D2 and measure the width of the HIGH part of the pulse
AsyncPulseIn asyncPulse(D2, HIGH);

void setup() {
	Serial.begin(9600);

	pinMode(D2, INPUT_PULLUP);

	// Call this to clear any previously saved data
	asyncPulse.clear();
}

void loop() {

	// Unlike pulseIn(), getNextPulse() is non-blocking. It returns the most recent pulses measured
	// and stored in a FIFO (first-in, first-out) buffer. If you have a sensor that you need to trigger,
	// you'd typically call asyncPulse.clear(), trigger the sensor, then you could either wait until
	// getNextPulse() returns true, or return from loop and handle it later.
	AsyncPulseInPulseInfo pulseInfo;
	while(asyncPulse.getNextPulse(pulseInfo)) {
		// We have a saved pulse
		Serial.printlnf("millis=%lu width=%lu", pulseInfo.startMillis, pulseInfo.widthMicros);
	}

}
