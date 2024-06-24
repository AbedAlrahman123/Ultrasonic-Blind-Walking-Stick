#define _XTAL_FREQ 4000000
#include <xc.h>
#include <pic16f877a.h>

// BEGIN CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
// END CONFIG

// Define Trigger and Echo pins for two sensors
#define Trigger1 PORTBbits.RB1 // Define Trigger pin for sensor 1
#define Echo1 PORTBbits.RB2    // Define Echo pin for sensor 1
#define Trigger2 PORTBbits.RB4 // Define Trigger pin for sensor 2
#define Echo2 PORTBbits.RB5    // Define Echo pin for sensor 2

void main()
{ 
    int time_taken1, time_taken2;
    int distance1, distance2;

    // Configure ports
    TRISD = 0x00;           // PORTD declared as output for interfacing LED/buzzer
    TRISBbits.TRISB1 = 0;   // Trigger pin of US sensor 1 set as output
    TRISBbits.TRISB2 = 1;   // Echo pin of US sensor 1 set as input
    TRISBbits.TRISB3 = 0;   // RB3 is output pin for LED/buzzer control for sensor 1
    TRISBbits.TRISB4 = 0;   // Trigger pin of US sensor 2 set as output
    TRISBbits.TRISB5 = 1;   // Echo pin of US sensor 2 set as input
    TRISDbits.TRISD2 = 0;   // RD2 is output pin for LED/buzzer control for sensor 2
    
    // Initialize Timer1
    T1CON = 0x10;           // Timer1 with prescaler 1:2, internal clock (Fosc/4)

    __delay_ms(2000);       // Initial delay for sensor stabilization

    while(1)
    { 
        // Sensor 1 trigger
        TMR1H = 0;          // Clear the Timer1 high byte
        TMR1L = 0;          // Clear the Timer1 low byte

        Trigger1 = 1; 
        __delay_us(10);           
        Trigger1 = 0;  

        // Wait for Echo1 to go HIGH
        while (Echo1 == 0);

        // Start Timer1
        TMR1ON = 1;

        // Wait for Echo1 to go LOW
        while (Echo1 == 1);

        // Stop Timer1
        TMR1ON = 0;

        // Calculate time taken for sensor 1
        time_taken1 = (TMR1L | (TMR1H << 8)); 

        // Calculate distance in cm for sensor 1
        distance1 = (0.0272 * time_taken1) / 2;

        // Control the buzzer based on distance for sensor 1
        if (distance1 <= 5)
            PORTBbits.RB3 = 1;  // Turn on Buzzer
        else
            PORTBbits.RB3 = 0;  // Turn off Buzzer

        // Sensor 2 trigger
        TMR1H = 0;          // Clear the Timer1 high byte
        TMR1L = 0;          // Clear the Timer1 low byte

        Trigger2 = 1; 
        __delay_us(10);           
        Trigger2 = 0;  

        // Wait for Echo2 to go HIGH
        while (Echo2 == 0);

        // Start Timer1
        TMR1ON = 1;

        // Wait for Echo2 to go LOW
        while (Echo2 == 1);

        // Stop Timer1
        TMR1ON = 0;

        // Calculate time taken for sensor 2
        time_taken2 = (TMR1L | (TMR1H << 8)); 

        // Calculate distance in cm for sensor 2
        distance2 = (0.0272 * time_taken2) / 2;

        // Control the buzzer based on distance for sensor 2
        if (distance2 <= 5)
            PORTDbits.RD1 = 1;  // Turn on Buzzer
        else
            PORTDbits.RD1 = 0;  // Turn off Buzzer

        __delay_ms(100); // Short delay between sensor measurements
    }
}
