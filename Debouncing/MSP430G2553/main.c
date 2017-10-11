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
    P1DIR |= 0x01; //sets p1 bit 0 to 1 for output
    P1SEL2|=0x00; //sets p1 bit 0 to 0 for I/O
    P1SEL|=0x00; //sets p1 bit 0 to 0 for I/O
    P1OUT=0x00; //Turns led off

//configure buttons
    P1DIR&=~BIT3; //sets p1 bit 3 to 0 for button Input
    P1SEL&=~BIT3; //sets p1 bit 3 to 0 for I/O
    P1SEL2&=~BIT3; //sets p1 bit 3 to 0 for I/O
    P1REN|=BIT3; //enables p1 bit 3 resistor for use
    P1OUT|=BIT3; //sets p1 bit 3 resistor to pull up resistor
    P1IE=BIT3;//enable interupt detection
    P1IES&=~BIT3;//flags interupt on negative

    P1IFG =0;//clears p2 flag register

    TA0CCR0=1000;//sets timer0 to 1 Khz

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    if(y==13){       //if button 6 was pressed
        count+=1;//counts how many times the interupt runs
        if(count>40){
            if(x<-25){
                y=-13;
                P1IE|=BIT3;
                TA0CTL=TACLR;
                TA0CCTL0&=~CCIE;//enable timer interupts
            }
            else {
                y=-13;
                P1OUT^=BIT0;
                P1IE|=BIT3;
                TA0CTL=TACLR;
                TA0CCTL0&=~CCIE;//enable timer interupts
            }
        }
        else if((P1IN&BIT3)==BIT3){//checks if button is not pressed
            x-=1;//decreased x to represent a button not pressed
        }
        else if((~P1IN&BIT3)==BIT3){//checks if button is not pressed
            x+=1;//decreased x to represent a button not pressed
        }
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(((~P1IN&BIT3)==BIT3)&(y==0))//checks if button 1 was pressed initially
    {
        TA0CCTL0|=CCIE;//enable timer interupts
        TA0CTL= (MC_1 + TASSEL_2 +TACLR);//set timer0 to up, SMCLK, no division
        P1IE&=~BIT3;//disable button interupts while debouncing
        P1IES&=~BIT3;//detects first bounce when released
        y=13;//set button identifier to 6
        P1IFG=~BIT3;//resets p1 bit 1 flag
    }
    else if(((P1IN&BIT3)==BIT3)&(y==-13)) //checks if button 1 was released
    {
        count=0;
        x=0;//clears debounce counter
        y=0;//resets identifier
        P1IES|=BIT3;//detects first bounce when pressed
        P1IFG&=~BIT3;//resets p1 bit 1 flag

    }
}
