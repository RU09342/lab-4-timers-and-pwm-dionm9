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
    WDTCTL = WDTPW | WDTHOLD;    // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode
//initilize LEDs
    P1DIR |= BIT0+BIT1; // sets P1.0 and P1.1 direction to 1 for output
    P1SEL1&=(BIT1+BIT0);// sets P1.0 select1 to 0 for I/O
    P1SEL0&=(BIT1+BIT0);//sets P1.0 select 2 to 0 for I/O
    P1OUT&=(BIT1+BIT0); //Sets P1.0 output to 0
//intialize button 1
    P5REN|=BIT6; //enables resistor for P5.6
    P5DIR&=~BIT6; // sets P5.6 for input
    P5SEL1&=~BIT6; //sets P5.6 for I/O
    P5SEL0&=~BIT6; //sets P5.6 for I/O
    P5OUT|=BIT6; // sets P5.6 resistor for pull up
    P5IES|=BIT6;//flags interupt on negative
    P5IE|=BIT6;//enable interupt detection
    P5IFG&=BIT5;//clears P5.6 flag register
//initialize button 2
    P5REN|=BIT5; //sets P5.5 REN to 1 to enable resistor
    P5DIR&=~BIT5; // sets P5.5 direction to 0 for input
    P5SEL1&=~BIT5; //sets P5.5 select1 to 0 for I/O
    P5SEL0&=~BIT5; //sets P5.5 select0 to 0 for I/O
    P5OUT|=BIT5; // sets P5.5 to 1 to set resistor to pull up
    P5IES|=BIT5;//flags interupt on negative
    P5IE|=BIT5;//enable interupt detection
    P5IFG&=BIT5;//clears P5.5 flag register

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
        P5IE|=BIT6;//enables button interupts
    }
    if((count1>50)&(b1==-2))//checks if count1 is greater than 50 and b1 is -2
    {
        P1OUT^=BIT0;//toggles led1 output
        b1=0;//sets b1 to 0
        P5IE|=BIT6;//enables button1 interupts
    }
    if((count2>50)&(b2==1))//checks if count2 is greater than 50 and b2 is 1
    {
        b2=-1;//sets b2 to negative 1
        P1OUT^=BIT1;//toggles led output
        P5IE|=BIT5;//enables button interupts
    }
     if((count2>50)&(b2==-2))//checks if count2 is greater than 50 and b1 is -2
    {
        P1OUT^=BIT1;//toggles led2 output
        b2=0;//sets b2 to 0
        P5IE|=BIT5;//enables button1 interupts
    }
}
#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
{
    if(((~P5IN&BIT6)==BIT6)&(b1==0))//checks if button 1 was pressed initiallb1
    {
        count1=0;
        b1=1;//set button identifier to 1
        P5IE&=~BIT6;//detects first bounce when released
        P5IES^=BIT6;//detects first bounce when released
        P5IFG=~BIT6;//resets p1 bit 1 flag
    }
    if(((P5IN&BIT6)==BIT6)&(b1==-1)) //checks if button 1 was released
    {
        b1=-2;//sets button 1 identifier to -1 to indicate not pressed
        count1=0;//resets count variable before use
       P5IE&=~BIT6;//disable interupts while debouncing
        P5IES^=BIT6;//detects first bounce when pressed
        P5IFG&=~BIT6;//resets p1 bit 1 flag

    }
    if(((~P5IN&BIT5)==BIT5)&(b2==0))//checks if button 1 was pressed initiallb1
    {
        count2=0;
        b2=1;//set button identifier to 1
        P5IE&=~BIT5;//detects first bounce when released
        P5IES^=BIT5;//detects first bounce when released
        P5IFG=~BIT5;//resets p1 bit 1 flag
    }
    if(((P5IN&BIT5)==BIT5)&(b2==-1)) //checks if button 1 was released
    {
        b2=-2;//sets button 1 identifier to -1 to indicate not pressed
        count2=0;//resets count variable before use
        P5IE&=~BIT5;//disable interupts while debouncing
        P5IES^=BIT5;//detects first bounce when pressed
        P5IFG&=~BIT5;//resets p1 bit 1 flag

    }
}
