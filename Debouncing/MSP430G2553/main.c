#include <msp430.h> 


/**
 * main.c
 * problems with interupt during debounce
 */

int count1=0;//number of timer interupts for button 1
int b1=0;

int main(void)
{
//initialize device
    WDTCTL = WDTPW | WDTHOLD;    // stop watchdog timer
//initialize LED 1
    P1DIR |= 0x01; //sets p1 bit 0 as an output
    P1SEL2|=0x00; //sets p1 bit 0 for I/O
    P1SEL|=0x00;//sets p1 bit 0 for I/O
    P1OUT&=~BIT0;//set output to 0 initially
//initialize button
    P1DIR&=~BIT3; //sets p1 bit 3 to 0 for button Input
    P1SEL&=~BIT3; //sets p1 bit 3 to 0 for I/O
    P1SEL2&=~BIT3; //sets p1 bit 3 to 0 for I/O
    P1REN|=BIT3; //enables p1 bit 3 resistor for use
    P1OUT|=BIT3; //sets p1 bit 3 resistor to pull up resistor
    P1IE=BIT3;//enable interupt detection
    P1IES|=BIT3;//flags interupt on negative

    TA0CCR0=1000;//sets timer0 to 1 Khz
    TA0CCTL0=CCIE;
    TA0CTL= (MC_1 + TASSEL_2+TACLR);//set timer0 to up, SMCLK, no division

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    if(count1<60)//checks if count is less than 60
        count1+=1;//increments count
    if((count1>50)&(b1==1))//checks if count is greater than 50 and b1 is 1
    {
        b1=-1;//sets b1 to negative
        P1OUT^=BIT0;//toggles led output
        P1IE|=BIT3;//enables button interupts
    }
    if((count1>50)&(b1==-2))//checks if count is greater than 50 and b1 is 1
    {
        P1OUT^=BIT0;//toggles led output
        b1=0;//sets b1 to 0
        P1IE|=BIT3;//enables button interupts
    }
}
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(((~P1IN&BIT3)==BIT3)&(b1==0))//checks if button 1 was pressed initiallb1
    {
        count1=0;//resets count1 variable
        b1=1;//set button identifier to 1
        P1IE&=~BIT3;//detects first bounce when released
        P1IES^=BIT3;//detects first bounce when released
        P1IFG=~BIT3;//resets p1 bit 1 flag
    }
    if(((P1IN&BIT1)==BIT1)&(b1==-1)) //checks if button 1 was released
    {
        b1=-2;//sets button 1 identifier to -1 to indicate not pressed
        count1=0;//resets count variable before use
        P1IE&=~BIT3;//disable interupts while debouncing
        P1IES^=BIT3;//detects first bounce when pressed
        P1IFG&=~BIT3;//resets p1 bit 1 flag

    }
}
