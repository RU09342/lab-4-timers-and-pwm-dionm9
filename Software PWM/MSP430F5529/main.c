#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
//set device proper functionality
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//initialize LED
    P1DIR=0x01; //sets P1.0 Direction to 1 for output
    P1SEL&=~BIT0; //sets P1.0 select to 0 for I/O
//intialize button 1
    P2REN|=BIT1; //enables P2.1 resistor for use
    P2DIR&=~BIT1; //sets P2.1 for input
    P2SEL&=~BIT1; //sets P2.1 for I/O
    P2OUT|=BIT1; //sets P2.1 resistor as a pullup resistor
    P2IE=BIT1;//enable interupt detection
    P2IES&=~BIT1;//flags interupt on negative
    P2IFG&=~BIT1;//clears p5 flag register

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
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
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
    P2IFG&=~BIT1;//clears flags
}
