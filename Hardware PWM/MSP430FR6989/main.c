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
    P1DIR|=0x01; //sets p1 bit 0 as output
    P1SEL0|=0x01; //sets p1 bit 0 as I/O
    P1SEL1|=0x00; //sets p1 bit 0 as I/O
    P1OUT=0x00; //clears p1out before use

//button initialization
    P1DIR&=~BIT1; //sets p1 bit 1 as input
    P1SEL0&=~BIT1; //sets p1 bit 1 for I/O
    P1SEL1&=~BIT1;//sets p1 bit 1 for I/O
    P1REN=BIT1;//enables p1 bit 1 resistor
    P1OUT|=BIT1; //sets p1 bit 1 resistor as pull up
    P1IE=BIT1;//enable interupt detection
    P1IES&=~BIT1;//flags interupt on negative
    P1IFG =0;//clears p2 flag registe

    TA0CCR0=1000;//sets timer to 1 KHz
    TA0CTL= (MC_1 + TASSEL_2 +ID_1);//set timer to up and SMCLK and internal divider to 1
    TA0CCTL1=(OUTMOD_7);//sets timer to output toggle/reset
    TA0CCR1=0; //sets

    _BIS_SR(LPM4_bits + GIE);//enters low power mode 4 with interupts
}
//button 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(TA0CCR1==1000){
        TA0CCR1=0;//resets duty cycle back to 0 if 100%
    }
    else{
        TA0CCR1+=100;//iterates duty cycle by 10%
    }
    P1IFG&=~BIT1;//clears flags
}
