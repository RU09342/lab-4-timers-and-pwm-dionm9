#include <msp430.h> 


/**
 * main.c
 * problems with interupt during debounce
 */

int count1=0;//number of timer interupts for button 1
int count2=0;//number of timer interupts for button 2
int b1=0;
int b2=0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode

//configure LED 1
    P1DIR|=BIT0; // sets p1 bit 0 and 1 for output
    P1SEL1=0x00;// sets p1 bit 0 to I/O
    P1SEL0=0x00;//sets p1 bit 0 to I/O
    P1OUT=0x00; //Turns led off
//configure LED 2
    P9DIR=BIT7;//sets p9 bit 7 as an output
    P9SEL0=0x00;//sets p9 bit 7 for I/O
    P9SEL1=0x00;//sets p9 bit 7 for I/O
    P9OUT&=~BIT7; //Turns led off

//configure buttons
    P1REN|=BIT1+BIT2; //enables resistor for p1 bit 5
    P1DIR&=~(BIT1+BIT2); // sets P1 bit 5 for input
    P1SEL1&=~(BIT1+BIT2); //sets P1 bit 5 for I/O
    P1SEL0&=~(BIT1+BIT2); //sets P1 bit 5 for I/O
    P1OUT|=(BIT1+BIT2); // sets P1 bit 5 resistor for pull up
    P1IES|=(BIT1+BIT2);//flags interupt on first press bounce
    P1IE|=(BIT1+BIT2);//enable interupt detection
    P1IFG&=~(BIT1+BIT2);//clears P1 flag register

    TA0CCR0=1000;//sets timer0 to 1 Khz
    TA0CCTL0=CCIE;
    TA0CTL= (MC_1 + TASSEL__SMCLK+TACLR);//set timer0 to up, SMCLK, no division

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    if(count1<60)
        count1+=1;
    if(count2<60)
        count2+=1;
    if((count1>50)&(b1==1))
    {
        b1=-1;
        P1OUT^=BIT0;
        P1IE|=BIT1;
    }
    if((count1>50)&(b1==-2))
    {
        P1OUT^=BIT0;
        b1=0;
        P1IE|=BIT1;
    }
    if((count2>50)&(b2==1))
    {
        b2=-1;
        P9OUT^=BIT7;
        P1IE|=BIT2;
    }
     if((count2>50)&(b2==-2))
    {
        P9OUT^=BIT7;
        b2=0;
        P1IE|=BIT2;
    }
}
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(((~P1IN&BIT1)==BIT1)&(b1==0))//checks if button 1 was pressed initiallb1
    {
        count1=0;
        b1=1;//set button identifier to 1
        P1IE&=~BIT1;//detects first bounce when released
        P1IES^=BIT1;//detects first bounce when released
        P1IFG=~BIT1;//resets p1 bit 1 flag
    }
    if(((P1IN&BIT1)==BIT1)&(b1==-1)) //checks if button 1 was released
    {
        b1=-2;//sets button 1 identifier to -1 to indicate not pressed
        count1=0;//resets count variable before use
       P1IE&=~BIT1;//disable interupts while debouncing
        P1IES^=BIT1;//detects first bounce when pressed
        P1IFG&=~BIT1;//resets p1 bit 1 flag

    }
    if(((~P1IN&BIT2)==BIT2)&(b2==0))//checks if button 1 was pressed initiallb1
    {
        count2=0;
        b2=1;//set button identifier to 1
        P1IE&=~BIT2;//detects first bounce when released
        P1IES^=BIT2;//detects first bounce when released
        P1IFG=~BIT2;//resets p1 bit 1 flag
    }
    if(((P1IN&BIT2)==BIT2)&(b2==-1)) //checks if button 1 was released
    {
        b2=-2;//sets button 1 identifier to -1 to indicate not pressed
        count2=0;//resets count variable before use
        P1IE&=~BIT2;//disable interupts while debouncing
        P1IES^=BIT2;//detects first bounce when pressed
        P1IFG&=~BIT2;//resets p1 bit 1 flag

    }
}
