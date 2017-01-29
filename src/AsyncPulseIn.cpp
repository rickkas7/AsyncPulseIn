#include "Particle.h"

#include "AsyncPulseIn.h"
#include "RingBuffer.h"

static const size_t PULSEINFO_SIZE = 32;
static AsyncPulseInPulseInfo pulseInfoBuffer[PULSEINFO_SIZE];
static RingBuffer<AsyncPulseInPulseInfo> ringBuffer(pulseInfoBuffer, PULSEINFO_SIZE);

AsyncPulseIn::AsyncPulseIn(int pin, int pulseType) : pin(pin), pulseType(pulseType) {
	attachInterrupt(pin, &AsyncPulseIn::interruptHandler, this, CHANGE);
}

AsyncPulseIn::~AsyncPulseIn() {

}

void AsyncPulseIn::clear() {
	ringBuffer.readClear();
}

bool AsyncPulseIn::getNextPulse(AsyncPulseInPulseInfo &pulseInfo) {
	AsyncPulseInPulseInfo *temp = ringBuffer.preRead();
	if (temp != NULL) {
		pulseInfo = *temp;
		ringBuffer.postRead();
		return true;
	}
	else {
		return false;
	}
}

void AsyncPulseIn::interruptHandler() {
	int state = pinReadFast(pin);

	if (state == pulseType) {
		// Start of pulse
		startMillis = millis();
		startMicros = micros();
	}
	else {
		// End of pulse
		unsigned long widthMicros = micros() - startMicros;
		AsyncPulseInPulseInfo *temp = ringBuffer.preWrite();
		if (temp != NULL) {
			temp->startMillis = startMillis;
			temp->widthMicros = widthMicros;
			ringBuffer.postWrite();
		}
	}
}

