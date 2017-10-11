#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
//set device proper functionality
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode

//initilize LEDs
    P1DIR |= BIT0+BIT1; // sets P1.0 and P1.1 direction to 1 for output
    P1SEL1&=BIT0;// sets P1.0 select1 to 0 for I/O
    P1SEL0&=BIT0;//sets P1.0 select 2 to 0 for I/O
    P1OUT&=BIT0; //Sets P1.0 output to 0

//intialize button 1
    P5REN|=BIT6; //enables resistor for P5.6
    P5DIR&=~BIT6; // sets P5.6 for input
    P5SEL1&=~BIT6; //sets P5.6 for I/O
    P5SEL0&=~BIT6; //sets P5.6 for I/O
    P5OUT|=BIT6; // sets P5.6 resistor for pull up
    P5IES&=~BIT6;//flags interupt on negative
    P5IE|=BIT6;//enable interupt detection
    P5IFG&=BIT6;//clears P5.6 flag register

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
#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
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
    P5IFG&=~BIT6;//clears flags
}
