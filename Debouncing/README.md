# Software Debouncing
The program worked by first initalizing the device, initializing the leds, then initializing the buttons. One timer was set up to run in up mode, with an interupt frequency of 1 kHz on CCR0. The program first triggered an interupt when the button was pressed. This condition then changes the state and startst the counting. Once the delay of 50ms is finished, it toggles the LED. The program then waits for the button to be released. Once the button is released, it triggers an interupt. The interupt then start the timer count. This waits for 50ms to toggle the LED. The program then waits for the button to be pressed again. This then repeats until the program is stopped.

## Extra Work
### Double the fun
I was able to use 1 timer to control 2 LEDs with 2 different buttons. The program worked by having 2 different count values and 2 different button status numbers. The timer consistently interupted at a rate of 1 kHz. This allowed for the count values to set a delay. The count values would only change when the corresponding interupt has occured. 
