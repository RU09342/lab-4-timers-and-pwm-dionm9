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
    P4DIR|=0x00; //sets p1 bit 0 as output
    P4SEL|=BIT7; //sets p1 bit 0 as I/O
    P4MAP7=PM_TB0CCR0A; //Maps CCR1 out to led

//button initialization
    P2REN|=BIT1; //enables p2 bit 1 resistor for use
    P2DIR&=~BIT1; //sets p2 bit 1 for input
    P2SEL&=~BIT1; //sets p2 bit 1 for I/O
    P2OUT|=BIT1; //sets p2 bit 1 resistor as a pullup resistor
    P2IE=BIT1;//enable interupt detection
    P2IES&=~BIT1;//flags interupt on negative
    P2IFG=0;//clears p2 flag register

    TB0CCR0=1000;//sets timer to 1 KHz
    TB0CTL= (MC_1 + TBSSEL_2 +ID_1);//set timer to up and SMCLK and internal divider to 1
    TB0CCTL1=OUTMOD_7;//sets timer to output toggle/reset
    TB0CCR1=100; //initially sets duty cycle to 0

    _BIS_SR(LPM0_bits + GIE);//enters low power mode 4 with interupts
}
//button 1 ISR
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if(TB0CCR1==1000){
        TB0CCR1=0;//resets duty cycle back to 0 if 100%
    }
    else{
        TB0CCR1+=100;//iterates duty cycle by 10%
    }
    P2IFG&=~BIT1;//clears flags
}
