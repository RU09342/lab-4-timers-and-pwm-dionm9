#include <msp430.h> 


/**
 * main.c
 * problems with interupt during debounce
 */

int count1=0;//number of timer interupts for button 1
int count2=0;//number of timer interupts for button 2
int b1=0;//number to determine button 1 state
int b2=0;//number to determine button 2 state

int main(void)
{
//initialize device
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
//configure LED 1
    P1DIR=BIT0; //sets p1 bit 0 to 1 for output
    P1SEL=0x00; //sets p1 bit 0 to 0 for I/O
    P1OUT=0x00; //Turns led off
//configure LED 2
    P4DIR=BIT7;//sets p4 bit 7 for output
    P4SEL=0x00;//sets p4 bit 7 for I/O
    P4OUT=0x00;//clears p4 before use

//configure buttons
    P1REN|=BIT1; //enables resistor for p1 bit 5
    P1DIR&=~BIT1; // sets P1 bit 5 for input
    P1SEL&=~BIT1; //sets P1 bit 5 for I/O
    P1OUT|=BIT1; // sets P1 bit 5 resistor for pull up
    P1IES|=BIT1;//flags interupt on first press bounce
    P1IE|=BIT1;//enable interupt detection
    P1IFG&=~BIT1;//clears P1 flag register

    P2REN|=BIT1; //enables p2 bit 1 resistor for use
    P2DIR&=~BIT1; //sets p2 bit 1 for input
    P2SEL&=~BIT1; //sets p2 bit 1 for I/O
    P2OUT|=BIT1; //sets p2 bit 1 resistor as a pullup resistor
    P2IE=BIT1;//enable interupt detection
    P2IES|=BIT1;//flags interupt on negative
    P2IFG =0;//clears p2 flag register


    TA0CCR0=1000;//sets timer0 to 1 Khz

    TA0CCR0=1000;//sets timer0 to 1 Khz
    TA0CCTL0=CCIE;//enable CCR0 interupts
    TA0CTL= (MC_1 + TASSEL__SMCLK+TACLR);//set timer0 to up, SMCLK, no division

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    if(count1<60)//checks if count1 is less than 60
        count1+=1;//increments count1 by 1
    if(count2<60)//checks if count2 is less than 60
        count2+=1;//increments count2 by 1
    if((count1>50)&(b1==1))//checks if count is greater than 50 and b1 is 1
    {
        b1=-1;//sets b1 to negative
        P1OUT^=BIT0;//toggles led output
        P2IE|=BIT1;//enables button interupts
    }
    if((count1>50)&(b1==-2))//checks if count1 is greater than 50 and b1 is -2
    {
        P1OUT^=BIT0;//toggles led1 output
        b1=0;//sets b1 to 0
        P2IE|=BIT1;//enables button1 interupts
    }
    if((count2>50)&(b2==1))//checks if count2 is greater than 50 and b2 is 1
    {
        b2=-1;//sets b2 to negative 1
        P4OUT^=BIT7;//toggles led output
        P1IE|=BIT1;//enables button interupts
    }
     if((count2>50)&(b2==-2))//checks if count2 is greater than 50 and b1 is -2
    {
        P4OUT^=BIT7;//toggles led2 outpu
        b2=0;//sets b2 to 0
        P1IE|=BIT1;//enables button1 interupts
    }
}
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if(((~P2IN&BIT1)==BIT1)&(b1==0))//checks if button 1 was pressed initiallb1
    {
        count1=0;
        b1=1;//set button identifier to 1
        P2IE&=~BIT1;//detects first bounce when released
        P2IES^=BIT1;//detects first bounce when released
        P2IFG=~BIT1;//resets p1 bit 1 flag
    }
    if(((P2IN&BIT1)==BIT1)&(b1==-1)) //checks if button 1 was released
    {
        b1=-2;//sets button 1 identifier to -1 to indicate not pressed
        count1=0;//resets count variable before use
        P2IE&=~BIT1;//disable interupts while debouncing
        P2IES^=BIT1;//detects first bounce when pressed
        P2IFG&=~BIT1;//resets p1 bit 1 flag

    }
}
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(((~P1IN&BIT1)==BIT1)&(b2==0))//checks if button 1 was pressed initiallb1
    {
        count2=0;
        b2=1;//set button identifier to 1
        P1IE&=~BIT1;//detects first bounce when released
        P1IES^=BIT1;//detects first bounce when released
        P1IFG=~BIT1;//resets p1 bit 1 flag
    }
    if(((P1IN&BIT1)==BIT1)&(b2==-1)) //checks if button 1 was released
    {
        b2=-2;//sets button 1 identifier to -1 to indicate not pressed
        count2=0;//resets count variable before use
        P1IE&=~BIT1;//disable interupts while debouncing
        P1IES^=BIT1;//detects first bounce when pressed
        P1IFG&=~BIT1;//resets p1 bit 1 flag

    }
}
