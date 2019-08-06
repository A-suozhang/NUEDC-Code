// ------------ Board Platform: MSP430FR5594 ----------------
// ----------- Code Composer With Energia (Board V.1.0.5) ---


# include <msp430.h>


bool state = false;
bool state1 = false;
int cnt = 0;
int cnt2 = 0;

void setup() {
  // put your setup code here, to run once:
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED,OUTPUT);
    pinMode(PUSH1,INPUT_PULLUP);
//    TA0CTL=0b0000001011010010;
    TA0CTL = 0b000000001010110010;
    // Configuration word
    // Bits 15-10: Unused
    // Bits 9-8: Clock source select: set to SMCLK (16MHz)
    // Bits 7-6: Input divider: set to 8
    // Bits 5-4: Mode control: Count up to TACCRO and reset
    // Bit 3: Unused
    // Bits 2: TACLR : set to initially clear timer system
    // Bit 1: Enable interrupts from TA0
    // Bit 0: Interrupt (pending) flag : set to zero (initially)
    TA0CCR0 = 2000;   // Set TACCR0 = 2000 to generate a 1ms timebase @ 16MHz with a divisor of 8
    TA0CCR1 = 1000;
    TA0CCTL0 = CCIE;  // Enable interrupts when TAR = TACCR0
    TA0CCTL1 = CCIE;
    attachInterrupt(PUSH1,blink_intr,FALLING);

}

void loop() {
  // put your main code here, to run repeatedly:

//    for (int i=0;i<5;i++){
//        digitalWrite(RED_LED,HIGH);
//        sleep(200);
//        digitalWrite(RED_LED,LOW);
//        sleep(1000);
//    }
//    suspend();  // Enter LPM4
}

void blink_intr(){
    // Used For LPM (None Of Business With Current Design)
    wakeup();
}



// !: VERY IMPORTANT
// The CCR0 Overflow & CCR1 & CCR2 Share An Interrupt Vector (Must Choose One)
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TimerA1ISR(){
    switch( TA0IV )
       {
           case  2: {
               cnt2 ++;
                   if (cnt2 >= 1000){
                       cnt2 = 0;
                       digitalWrite(RED_LED,state1);
                       state1 = !state1;
                   }
           }                        // CCR1  used
           case  4: break;                          // CCR2 not used
           case 10: break;                  // overflow
           break;
       }

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TimerA0ISR(){
    cnt ++;
    if (cnt >= 1000){
        cnt = 0;
        digitalWrite(GREEN_LED,state);
        state = !state;
    }
}

