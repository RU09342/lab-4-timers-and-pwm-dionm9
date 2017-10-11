#include <msp430.h> 


/**
 * main.c
 * FR6989 PWM
 * UP
 * SMCLK
 */
//initialize PWM
int main(void)
{
//set device proper functionality
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//LED output initialization
    P1DIR|=BIT6; //sets p1 bit 0 as output
    P1SEL|=BIT6; //sets p1 bit 0 as I/O
    P1SEL2&=~BIT6;

//button initialization
    P1REN|=BIT3; //enables P1 bit 1 resistor for use
    P1DIR&=~BIT3; //sets P1 bit 1 for input
    P1SEL&=~BIT3; //sets P1 bit 1 for I/O
    P1OUT|=BIT3; //sets P1 bit 1 resistor as a pullup resistor
    P1IE|=BIT3;//enable interupt detection
    P1IES&=~BIT3;//flags interupt on negative
    P1IFG&=~BIT3;//clears P1 flag register

    TA0CCR0=1000;//sets timer to 1 KHz
    TA0CTL= (MC_1 + TASSEL_2 +ID_1);//set timer to up and SMCLK and internal divider to 1
    TA0CCTL1=(OUTMOD_7);//sets timer to output toggle/reset
    TA0CCR1=0; //initially sets duty cycle to 0

    _BIS_SR(LPM0_bits + GIE);//enters low power mode 4 with interupts
}
//button 1 ISR

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(TA0CCR1==999){
        TA0CCR1=0;//resets duty cycle back to 0 if 100%
    }
    else if(TA0CCR1==0){
           TA0CCR1=99;//resets duty cycle back to 0 if 100%
       }
    else{
        TA0CCR1+=100;//iterates duty cycle by 10%
    }
    P1IFG&=~BIT3;//clears flags
}

