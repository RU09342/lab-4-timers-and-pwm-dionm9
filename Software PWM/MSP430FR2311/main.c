#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
//initialize device
    WDTCTL = WDTPW | WDTHOLD;    // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode
//initialize LED 1
    P1DIR|=BIT0; //set p1 bit 0 for output
    P1SEL0&=~BIT0; //set p1 bit 0 for I/O
//initialize button
    P1OUT|=BIT1; //selected Vcc for pull up resistor on bit 1
    P1REN|=BIT1; //enabled pull up resistor on bit 1
    P1DIR&=~BIT1; //set p1 for input on bit 1
    P1SEL1&=~BIT1; //sets p1 for I/O
    P1IE|=BIT1;//enable interupt detection
    P1IES&=~BIT1;//flags interupt on negative
    P1IFG&=~BIT1;//clears p2 flag register

//timer A0 initialization
    TB0CCTL0 = CCIE;//enables inerterupts
    TB0CCR0=500;//sets timer to 1 kHz
    TB0CTL= (MC_1 + TBSSEL_2 +ID_3);//set timer to up and SMCLK and internal divider to 8
    TB0CCTL1=CCIE;
    TB0CCR1=500;

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts
}
//TB0 interupt service vector
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR (void)
{
    if(TB0CCR1==500){//checks if ccr1
           TB0CCTL0&=~BIT0;//clears flags
            //this prevents the led from turning on when ccr1 is 500
       }

       else{
            P1OUT|=BIT0;//sets led to turn off
            TB0CCTL0&=~BIT0;//clears flags
      }
}
//TB0CCR1 interupt service vector
#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR (void)
{
        P1OUT&=~BIT0;//CCR1 interupts
        TB0CCTL1&=~BIT0;    //clears flags
}
//TB0 interupt service vector
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (TB0CCR1==500){
        TB0CCR1=50;
    }
    else if(TB0CCR1==499){
        TB0CCR1=500;
    }
    else if(TB0CCR1==450){
            TB0CCR1=499;
        }

    else{
        TB0CCR1=TB0CCR1+50;
    }
    P1IFG&=~BIT1;//clears flags
}
