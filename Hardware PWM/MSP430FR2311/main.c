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
    PM5CTL0 &= ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode

//LED initialization
    P2DIR|=BIT0; //sets p1 bit 0 as output
    P2SEL0|=BIT0; //sets p1 bit 0 as I/O
    P2SEL1|=0x00; //sets p1 bit 0 as I/O

//button initialization
    P1DIR&=~BIT1; //sets p1 bit 1 as input
    P1SEL0&=~BIT1; //sets p1 bit 1 for I/O
    P1SEL1&=~BIT1;//sets p1 bit 1 for I/O
    P1REN=BIT1;//enables p1 bit 1 resistor
    P1OUT|=BIT1; //sets p1 bit 1 resistor as pull up
    P1IE=BIT1;//enable interupt detection
    P1IES&=~BIT1;//flags interupt on negative
    P1IFG =0;//clears p2 flag registe

    TB1CCR0=1000;//sets timer to 1 KHz
    TB1CTL= (MC_1 + TBSSEL__SMCLK +ID_1);//set timer to up and SMCLK and internal divider to 1
    TB1CCTL1=(OUTMOD_7);//sets timer to output toggle/reset
    TB1CCR1=0; //sets

    _BIS_SR(LPM4_bits + GIE);//enters low power mode 4 with interupts
}
//button 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(TB1CCR1==1000){
        TB1CCR1=0;//resets duty cycle back to 0 if 100%
    }
    else{
        TB1CCR1+=100;//iterates duty cycle by 10%
    }
    P1IFG&=~BIT1;//clears flags
}
