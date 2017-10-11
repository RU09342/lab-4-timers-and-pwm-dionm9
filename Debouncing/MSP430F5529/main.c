#include <msp430.h> 


/**
 * main.c
 * problems with interupt during debounce
 */

int x = 0;//debounce vote
int y = 0;//button identifier
int count=0;//number of timer interupts


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//configure LED 1
    P1DIR=BIT0; //sets p1 bit 0 to 1 for output
    P1SEL=0x00; //sets p1 bit 0 to 0 for I/O
    P1OUT=0x00; //Turns led off

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

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    if(y==11){       //if button 6 was pressed
        count+=1;//counts how many times the interupt runs
        if(count>40){
            if(x<-25){
                y=-11;
                P1IE|=BIT1;
                TA0CTL=TACLR;
                TA0CCTL0&=~CCIE;//enable timer interupts
            }
            else {
                y=-11;
                P1OUT^=BIT0;
                P1IE|=BIT1;
                TA0CTL=TACLR;
                TA0CCTL0&=~CCIE;//enable timer interupts
            }
        }
        else if((P1IN&BIT1)==BIT1){//checks if button is not pressed
            x-=1;//decreased x to represent a button not pressed
        }
        else if((~P1IN&BIT1)==BIT1){//checks if button is not pressed
            x+=1;//decreased x to represent a button not pressed
        }
    }
    if(y==21){       //if button 6 was pressed
        count+=1;//counts how many times the interupt runs
        if(count>40){
            if(x<-25){
                y=-21;
                P2IE|=BIT1;
                TA0CTL=TACLR;
                TA0CCTL0&=~CCIE;//enable timer interupts
            }
            else {
                y=-21;
                P1OUT^=BIT0;
                P2IE|=BIT1;
                TA0CTL=TACLR;
                TA0CCTL0&=~CCIE;//enable timer interupts
            }
        }
        else if((P2IN&BIT1)==BIT1){//checks if button is not pressed
            x-=1;//decreased x to represent a button not pressed
        }
        else if((~P2IN&BIT1)==BIT1){//checks if button is not pressed
            x+=1;//decreased x to represent a button not pressed
        }
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(((~P1IN&BIT1)==BIT1)&(y==0))//checks if button 1 was pressed initially
    {
        TA0CCTL0|=CCIE;//enable timer interupts
        TA0CTL= (MC_1 + TASSEL__SMCLK+TACLR);//set timer0 to up, SMCLK, no division
        P1IE&=~BIT1;//disable button interupts while debouncing
        P1IES&=~BIT1;//detects first bounce when released
        y=11;//set button identifier to 6
        P1IFG=~BIT1;//resets p1 bit 1 flag
    }
    else if(((P1IN&BIT1)==BIT1)&(y==-11)) //checks if button 1 was released
    {
        count=0;
        x=0;//clears debounce counter
        y=0;//resets identifier
        P1IES|=BIT1;//detects first bounce when pressed
        P1IFG&=~BIT1;//resets p1 bit 1 flag

    }
}
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if(((~P2IN&BIT1)==BIT1)&(y==0))//checks if button 1 was pressed initially
    {
        TA0CCTL0|=CCIE;//enable timer interupts
        TA0CTL= (MC_1 + TASSEL__SMCLK+TACLR);//set timer0 to up, SMCLK, no division
        P2IE&=~BIT1;//disable button interupts while debouncing
        P2IES&=~BIT1;//detects first bounce when released
        y=21;//set button identifier to 6
        P2IFG=~BIT1;//resets p1 bit 1 flag
    }
    else if(((P2IN&BIT1)==BIT1)&(y==-21)) //checks if button 1 was released
    {
        count=0;
        x=0;//clears debounce counter
        y=0;//resets identifier
        P2IES|=BIT1;//detects first bounce when pressed
        P2IFG&=~BIT1;//resets p1 bit 1 flag

    }
}
