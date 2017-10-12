#include <msp430.h> 


/**
 * main.c
 * problems with interupt during debounce
 */

int count1=0;//number of timer interupts for button 1
int b1=0;//number to determine button 1 state

int main(void)
{
//initialize device
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode

//configure LED 1
    P1DIR|=BIT0; // sets p1 bit 0 and 1 for output
    P1SEL1=0x00;// sets p1 bit 0 to I/O
    P1SEL0=0x00;//sets p1 bit 0 to I/O
    P1OUT=0x00; //Turns led off

//initialize button
    P1OUT|=BIT1; //selected Vcc for pull up resistor on bit 1
    P1REN|=BIT1; //enabled pull up resistor on bit 1
    P1DIR&=~BIT1; //set p1 for input on bit 1
    P1SEL1&=~BIT1; //sets p1 for I/O
    P1IE|=BIT1;//enable interupt detection
    P1IES|=BIT1;//flags interupt on negative
    P1IFG&=~BIT1;//clears p2 flag register

    TB0CCR0=1000;//sets timer0 to 1 Khz
    TB0CCTL0=CCIE;//enable CCR0 interupts
    TB0CTL= (MC_1 + TBSSEL__SMCLK+TBCLR);//set timer0 to up, SMCLK, no division

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR (void)
{
    if(count1<60)//checks if count is less than 60
        count1+=1;//increments count by 1
    if((count1>50)&(b1==1))//checks if count is greater than 50 and b1 is 1
    {
        b1=-1;//sets b1 to negative 1
        P1OUT^=BIT0;//toggles led output
        P1IE|=BIT1;//enables button interupts
    }
    if((count1>50)&(b1==-2))//checks if count is greater than 50 and b1 is -2
    {
        P1OUT^=BIT0;//toggles led output
        b1=0;//sets b1 to 0
        P1IE|=BIT1;//enables button interupts
    }
}
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(((~P1IN&BIT1)==BIT1)&(b1==0))//checks if button 1 was pressed initiallb1
    {
        count1=0;//resets count1 variable
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
}
