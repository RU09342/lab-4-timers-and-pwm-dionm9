#include <msp430.h> 


/**
 * main.c
 * FR6989 PWM
 * UP
 * Aclk
 * Power effiency
 * 3:20 currently
 * need to make sure PM5 is working
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

/* //button initialization
    P5REN|=BIT5; //enables resistor for p1 bit 5
    P5DIR&=~BIT5; // sets p1 bit 5 for input
    P5SEL1&=~BIT5; //sets p1 bit 5 for I/O
    P5SEL0&=~BIT5; //sets p1 bit 5 for I/O
    P5OUT|=BIT5; // sets p1 bit 5 resistor for pull up
    P5IE=BIT5;//enable interupt detection
    P5IES&=~BIT5;//flags interupt on negative
    P5IFG =0;//clears p2 flag registe
*/
//timer A0 initialization
    //TA0CCTL0 = CCIE;//enables inerterupts
    TA0CCR0=499;//sets timer to 1 kHz
    TA0CTL= (MC_1 + TASSEL_1);//set timer to up and SMCLK and internal divider to 1
    TA0CCTL1=(OUTMOD_7);//+CCIE);
    TA0CCR1=249;

    _BIS_SR(LPM4_bits + GIE);//enters low power mode 4 with interupts
    PM5CTL0 |= LOCKLPM5;  // Enables the GPIO power-on default high-impedance mode
}
/*#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
{
    if(TA0CCR1==0){
        TA0CCR1=TA0CCR1+9;
    }
    else if(TA0CCR1>98){
        TA0CCR1=0;
    }
    else{
        TA0CCR1=TA0CCR1+10;
    }
    P5IFG&=~BIT1;//clears flags
}
*/
