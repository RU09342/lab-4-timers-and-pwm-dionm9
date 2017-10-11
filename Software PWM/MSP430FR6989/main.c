#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
//set device proper functionality
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode

//LED initialization
    P1DIR|=0x01; //sets p1 bit 0 as output
    P1SEL0|=0x00; //sets p1 bit 0 as I/O
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

//timer A0 initialization
    TA0CCTL0 = CCIE;//enables inerterupts
    TA0CCR0=500;//sets timer to 1 kHz
    TA0CTL= (MC_1 + TASSEL_2 +ID_3);//set timer to up and SMCLK and internal divider to 8
    TA0CCTL1=CCIE;
    TA0CCR1=500;

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts
}
//TA0 interupt service vector
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    if(TA0CCR1==500){//checks if ccr1
           TA0CCTL0&=~BIT0;//clears flags
            //this prevents the led from turning on when ccr1 is 500
       }

       else{
            P1OUT|=BIT0;//sets led to turn off
            TA0CCTL0&=~BIT0;//clears flags
      }
}
//TA0CCR1 interupt service vector
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR (void)
{
        P1OUT&=~BIT0;//CCR1 interupts
        TA0CCTL1&=~BIT0;    //clears flags
}
//TA0 interupt service vector
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (TA0CCR1==500){
        TA0CCR1=50;
    }
    else if(TA0CCR1==499){
        TA0CCR1=500;
    }
    else if(TA0CCR1==450){
            TA0CCR1=499;
        }

    else{
        TA0CCR1=TA0CCR1+50;
    }
    P1IFG&=~BIT1;//clears flags
}
