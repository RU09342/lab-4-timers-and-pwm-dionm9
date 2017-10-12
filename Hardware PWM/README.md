# Hardware PWM
Not all processors had a hardware PWM option. The 5529 lacked the ability to connect TAoutmode to an LED register. The other programs initialized to have Ax.y (Bx.y for the 2311) as outputs directly. X and y were whatever the option the LED output had avliable. The timer initalized was then TimerAx and TAxCCRy. Each of these was set on a button interupt that incremented the pwm by 10%. The PWM ran at a set frequency of 1 kHz. 

## Extra Work
### Ultra Low Power
Using ACLk, enablining HIGHZ GPIO pins on unused pins, disconnecting unused jumpers, and Low power mode 4, I was able to achieve a time of 7 minutes and 40 seconds. I'm sure this could be improved with more optimizations of clock dividers and PWM frequency. 
