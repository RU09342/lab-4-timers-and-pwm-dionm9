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
    PM5CTL0 &= ~LOCKLPM5;  // Disable the GPIO power-on default high-impedance mode

//configure LED 1
    P1DIR|=BIT0; // sets p1 bit 0 and 1 for output
    P1SEL1=0x00;// sets p1 bit 0 to I/O
    P1SEL0=0x00;//sets p1 bit 0 to I/O
    P1OUT=0x00; //Turns led off

//configure buttons
    P5REN|=BIT6+BIT5; //enables resistor for p1 bit 5
    P5DIR&=~(BIT6+BIT5); // sets p5 bit 5 for input
    P5SEL1&=~(BIT6+BIT5); //sets p5 bit 5 for I/O
    P5SEL0&=~(BIT6+BIT5); //sets p5 bit 5 for I/O
    P5OUT|=(BIT6+BIT5); // sets p5 bit 5 resistor for pull up
    P5IES|=(BIT6+BIT5);//flags interupt on first press bounce
    P5IE|=(BIT6+BIT5);//enable interupt detection
    P5IFG&=~(BIT6+BIT5);//clears p5 flag register

    TA0CCR0=1000;//sets timer0 to 1 Khz

    _BIS_SR(LPM0_bits + GIE);//enters low power mode with interupts
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    if(y==6){       //if button 6 was pressed
        count+=1;//counts how many times the interupt runs
        if(count>40){
            if(x<-25){
                y=-6;
                P5IE|=BIT6;
                TA0CTL=TACLR;
                TA0CCTL0&=~CCIE;//enable timer interupts
            }
            else {
                y=-6;
                P1OUT^=BIT0;
                P5IE|=BIT6;
                TA0CTL=TACLR;
                TA0CCTL0&=~CCIE;//enable timer interupts
            }
        }
        else if((P5IN&BIT6)==BIT6){//checks if button is not pressed
            x-=1;//decreased x to represent a button not pressed
        }
        else if((~P5IN&BIT6)==BIT6){//checks if button is not pressed
            x+=1;//decreased x to represent a button not pressed
        }
    }
    if(y==5){       //if button 6 was pressed
        count+=1;//counts how many times the interupt runs
        if(count>40){
            if(x<-25){
                y=-5;
                P5IE|=BIT5;
                TA0CTL=TACLR;
                TA0CCTL0&=~CCIE;//enable timer interupts
            }
            else {
                y=-5;
                P1OUT^=BIT0;
                P5IE|=BIT5;
                TA0CTL=TACLR;
                TA0CCTL0&=~CCIE;//enable timer interupts
            }
        }
        else if((P5IN&BIT5)==BIT5){//checks if button is not pressed
            x-=1;//decreased x to represent a button not pressed
        }
        else if((~P5IN&BIT5)==BIT5){//checks if button is not pressed
            x+=1;//decreased x to represent a button not pressed
        }
    }
}

#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
{
    if(((~P5IN&BIT6)==BIT6)&(y==0))//checks if button 1 was pressed initially
    {
        TA0CCTL0|=CCIE;//enable timer interupts
        TA0CTL= (MC_1 + TASSEL__SMCLK+TACLR);//set timer0 to up, SMCLK, no division
        P5IE&=~BIT6;//disable button interupts while debouncing
        P5IES&=~BIT6;//detects first bounce when released
        y=6;//set button identifier to 6
        P5IFG=~BIT6;//resets p1 bit 1 flag
    }
    else if(((P5IN&BIT6)==BIT6)&(y==-6)) //checks if button 1 was released
    {
        count=0;
        x=0;//clears debounce counter
        y=0;//resets identifier
        P5IES|=BIT6;//detects first bounce when pressed
        P5IFG&=~BIT6;//resets p1 bit 1 flag

    }
    if(((~P5IN&BIT5)==BIT5)&(y==0))//checks if button 1 was pressed initially
    {
        TA0CCTL0|=CCIE;//enable timer interupts
        TA0CTL= (MC_1 + TASSEL__SMCLK+TACLR);//set timer0 to up, SMCLK, no division
        P5IE&=~BIT5;//disable button interupts while debouncing
        P5IES&=~BIT5;//detects first bounce when released
        y=5;//set button identifier to 6
        P5IFG=~BIT6;//resets p1 bit 1 flag
    }
    else if(((P5IN&BIT5)==BIT5)&(y==-5)) //checks if button 1 was released
    {
        count=0;
        x=0;//clears debounce counter
        y=0;//resets identifier
        P5IES|=BIT5;//detects first bounce when pressed
        P5IFG&=~BIT5;//resets p1 bit 1 flag

    }
}
