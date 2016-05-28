#ifndef __ASYNCPULSEIN_H
#define __ASYNCPULSEIN_H

/**
 * Information about the pulse is returned in this object
 */
typedef struct {
	unsigned long startMillis;
	unsigned long widthMicros;
} AsyncPulseInPulseInfo;


/**
 * Non-blocking, asynchronous version of pulseIn() that uses interrupts
 */
class AsyncPulseIn {
public:
	/**
	 * Initializes the object to monitor a given pin and state. The pin must be compatible with attachInterrupt()
	 *
	 * Photon: All pins with the exception of D0 and A5 (since at present Mode Button external interrupt(EXTI)
	 * line is shared with D0, A5). Also please note following are the pins for which EXTI lines are shared
	 * so only one can work at a time:
	 * D1, A4
	 * D2, A0, A3
	 * D3, DAC
	 * D4, A1
	 * https://docs.particle.io/reference/firmware/photon/#interrupts
	 *
	 * The pulseType must be LOW or HIGH. If HIGH, then measuring starts a the LOW to HIGH transition and
	 * widthMicros is the width of the HIGH portion in microseconds, for example.
	 */
	AsyncPulseIn(int pin, int pulseType);
	virtual ~AsyncPulseIn();

	/**
	 * Clear and outstanding pulse width information that has been saved in the buffer
	 */
	void clear();

	/**
	 * Non-blocking version to get the length of the pulse. This is different than pulseIn() because the
	 * code is always remembering the latest pulse widths in the background using interrupts. You call this
	 * method out of loop to get the most recent pulse widths. They're stored in a circular buffer, first-in,
	 * first-out.
	 */
	bool getNextPulse(AsyncPulseInPulseInfo &pulse);

protected:
	void interruptHandler();

	int pin;
	int pulseType;

	unsigned long startMillis = 0;
	unsigned long startMicros = 0;
};


#endif /* __ASYNCPULSEIN_H */

