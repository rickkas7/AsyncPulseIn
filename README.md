# AsyncPulseIn

This is an asynchronous version of pulseIn. It’s probably not exactly right for all use cases, but it seems to work.

The annoying thing about pulseIn(), a function inherited from Arduino, is that it’s blocking. Also, the current Particle implementation doesn’t allow you to set the timeout, currently 3 seconds. But even with a configurable timeout, I’m not fond of it being blocking.

What this code does is use an interrupt to monitor the pin. It can catch exceptionally narrow pulses that way, pretty accurately.

The big question is: what do you do with those pulse width measurements? The problem with an interrupt handler is that there are huge number of restrictions of what you can do from your interrupt handler, so you want to minimize what you do there. What I  decided on was a circular buffer (FIFO) that remembers the recent pulse widths. This can handle the simple case where you trigger the sensor and measure one pulse, but also more complicated cases where you have multiple pulses coming in.

You typically create a global object like this:
```cpp
// Initialize the AsyncPulseIn object to listen on D2 and measure the width of the HIGH part of the pulse
AsyncPulseIn asyncPulse(D2, HIGH);
```

Note that since it uses interrupts, there are some restrictions on what pins you can use. See:
https://docs.particle.io/reference/firmware/photon/#attachinterrupt-

This is the code example to just print out all of the pulse info. This code was in `loop();`:
```cpp
AsyncPulseInPulseInfo pulseInfo;
while(asyncPulse.getNextPulse(pulseInfo)) {
    // We have a saved pulse
    Serial.printlnf("millis=%lu width=%lu", pulseInfo.startMillis, pulseInfo.widthMicros);
}
```

The main thing is that you call `getNextPulse()` out of loop and it returns the previously measured pulses. It does not block waiting for new pulses. This will require some restructuring over code ported directly from Arduino, but it will make your code much more efficient and predicable, not blocking loop to measure pulses.

The code is quite simple so feel free to just copy and paste the stuff directly into your code as it’s a reasonable template for a pulse-detecting interrupt service routine.
