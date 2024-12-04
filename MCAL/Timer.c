/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer.c
 *
 * Description: source file for the Timer AVR driver
 *
 * Author: Doaa Said
 *
 *******************************************************************************/
#include "Timer.h"
#include "../imp_files/common_macros.h"
#include <avr/interrupt.h>
#include "../imp_files/std_types.h"

// Global variables to track total ticks and individual ticks
volatile uint8 g_total_ticks = 0; // Total ticks
volatile static uint8 g_ticks = 0; // Current tick count

// Function pointers for timer callbacks
static volatile void (*g_Timer0_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer1_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer2_callBackPtr)(void) = NULL_PTR;

// Function to initialize the timer based on the provided configuration
void Timer_init(const Timer_ConfigType *Config_Ptr) {
    // Check if the timer ID is valid
    if (Config_Ptr->timer_ID >= NUM_OF_Timers) {
        /* Do Nothing */
    } else {
        switch (Config_Ptr->timer_ID) {
        case TIMER0_ID:
            TCCR0_REG.Bits.FOC0_Bit = LOGIC_HIGH; // Force Output Compare
            TCCR0_REG.Bits.WGM00_Bit = GET_BIT(Config_Ptr->timer_mode, 0); // Set mode bits
            TCCR0_REG.Bits.WGM01_Bit = GET_BIT(Config_Ptr->timer_mode, 1);
            TCCR0_REG.Bits.CS00_Bit = GET_BIT(Config_Ptr->timer_clock, 0); // Set clock source
            TCCR0_REG.Bits.CS01_Bit = GET_BIT(Config_Ptr->timer_clock, 1);
            TCCR0_REG.Bits.CS02_Bit = GET_BIT(Config_Ptr->timer_clock, 2);
            TCNT0_REG.Byte = Config_Ptr->timer_InitialValue; // Set initial value
            // Configure compare match mode or normal mode
            if (Config_Ptr->timer_mode == CTC_0_OR_2) {
                TCCR0_REG.Bits.COM00_Bit = LOGIC_LOW; // Set output mode
                TCCR0_REG.Bits.COM01_Bit = LOGIC_LOW;
                OCR0_REG.Byte = Config_Ptr->timer_compare_MatchValue; // Set compare value
                TIMSK_REG.Bits.OCIE0_Bit = LOGIC_HIGH; // Enable compare interrupt
            } else if (Config_Ptr->timer_mode == NORMAL) {
                TIMSK_REG.Bits.TOIE0_Bit = LOGIC_HIGH; // Enable overflow interrupt
            }
            break;

        case TIMER1_ID:
            // Configure Timer1
            TCCR1A_REG.Bits.FOC1A_Bit = LOGIC_HIGH; // Force Output Compare
            TCCR1A_REG.Bits.WGM10_Bit = GET_BIT(Config_Ptr->timer_mode, 0); // Set mode bits
            TCCR1A_REG.Bits.WGM11_Bit = GET_BIT(Config_Ptr->timer_mode, 1);
            TCCR1B_REG.Bits.WGM12_Bit = GET_BIT(Config_Ptr->timer_mode, 2);
            TCCR1B_REG.Bits.WGM13_Bit = GET_BIT(Config_Ptr->timer_mode, 3);
            TCCR1B_REG.Bits.CS10_Bit = GET_BIT(Config_Ptr->timer_clock, 0); // Set clock source
            TCCR1B_REG.Bits.CS11_Bit = GET_BIT(Config_Ptr->timer_clock, 1);
            TCCR1B_REG.Bits.CS12_Bit = GET_BIT(Config_Ptr->timer_clock, 2);
            TCNT1_REG.TwoBytes = Config_Ptr->timer_InitialValue; // Set initial value
            // Configure compare match mode or normal mode
            if (Config_Ptr->timer_mode == CTC_1) {
                TCCR1A_REG.Bits.COM1A0_Bit = LOGIC_LOW; // Set output mode
                TCCR1A_REG.Bits.COM1A1_Bit = LOGIC_LOW;
                TCCR1A_REG.Bits.COM1B0_Bit = LOGIC_LOW;
                TCCR1A_REG.Bits.COM1B1_Bit = LOGIC_LOW;
                OCR1A_REG.TwoBytes = Config_Ptr->timer_compare_MatchValue; // Set compare value
                TIMSK_REG.Bits.OCIE1A_Bit = LOGIC_HIGH; // Enable compare interrupt
            } else if (Config_Ptr->timer_mode == NORMAL) {
                TIMSK_REG.Bits.TOIE1_Bit = LOGIC_HIGH; // Enable overflow interrupt
            }
            break;

        case TIMER2_ID: {
            // Configure Timer2
            TCCR2_REG.Bits.FOC2_Bit = LOGIC_HIGH; // Force Output Compare
            TCCR2_REG.Bits.WGM20_Bit = GET_BIT(Config_Ptr->timer_mode, 0); // Set mode bits
            TCCR2_REG.Bits.WGM21_Bit = GET_BIT(Config_Ptr->timer_mode, 1);
            TCCR2_REG.Bits.CS20_Bit = GET_BIT(Config_Ptr->timer_clock, 0); // Set clock source
            TCCR2_REG.Bits.CS21_Bit = GET_BIT(Config_Ptr->timer_clock, 1);
            TCCR2_REG.Bits.CS22_Bit = GET_BIT(Config_Ptr->timer_clock, 2);
            TCNT2_REG.Byte = Config_Ptr->timer_InitialValue; // Set initial value
            // Configure compare match mode or normal mode
            if (Config_Ptr->timer_mode == CTC_0_OR_2) {
                TCCR2_REG.Bits.COM20_Bit = LOGIC_LOW; // Set output mode
                TCCR2_REG.Bits.COM21_Bit = LOGIC_LOW;
                OCR2_REG.Byte = Config_Ptr->timer_compare_MatchValue; // Set compare value
                TIMSK_REG.Bits.OCIE2_Bit = LOGIC_HIGH; // Enable compare interrupt
            } else if (Config_Ptr->timer_mode == NORMAL) {
                TIMSK_REG.Bits.TOIE2_Bit = LOGIC_HIGH; // Enable overflow interrupt
            }
            break;
        }
        }
    }
}

// Function to deinitialize the specified timer
void Timer_deInit(Timer_ID_Type timer_ID) {
    // Check if the timer ID is valid
    if (timer_ID >= NUM_OF_Timers) {
        /* Do Nothing */
    } else {
        switch (timer_ID) {
        case TIMER0_ID:
            // Reset Timer0 registers
            TCCR0_REG.Byte = LOGIC_LOW;
            OCR0_REG.Byte = LOGIC_LOW;
            TCNT0_REG.Byte = LOGIC_LOW;
            TIMSK_REG.Bits.TOIE0_Bit = LOGIC_LOW; // Disable overflow interrupt
            TIMSK_REG.Bits.OCIE0_Bit = LOGIC_LOW; // Disable compare interrupt
            g_Timer0_callBackPtr = NULL_PTR; // Clear callback pointer
            break;

        case TIMER1_ID:
            // Reset Timer1 registers
            TCCR1A_REG.Byte = LOGIC_LOW;
            TCCR1B_REG.Byte = LOGIC_LOW;
            OCR1A_REG.TwoBytes = LOGIC_LOW;
            OCR1B_REG.TwoBytes = LOGIC_LOW;
            TCNT1_REG.TwoBytes = LOGIC_LOW;
            TIMSK_REG.Bits.TOIE1_Bit = LOGIC_LOW; // Disable overflow interrupt
            TIMSK_REG.Bits.OCIE1A_Bit = LOGIC_LOW; // Disable compare interrupt
            TIMSK_REG.Bits.OCIE1B_Bit = LOGIC_LOW; // Disable compare interrupt
            g_Timer1_callBackPtr = NULL_PTR; // Clear callback pointer
            break;

        case TIMER2_ID:
            // Reset Timer2 registers
            TCCR2_REG.Byte = LOGIC_LOW;
            OCR2_REG.Byte = LOGIC_LOW;
            TCNT2_REG.Byte = LOGIC_LOW;
            TIMSK_REG.Bits.TOIE2_Bit = LOGIC_LOW; // Disable overflow interrupt
            TIMSK_REG.Bits.OCIE2_Bit = LOGIC_LOW; // Disable compare interrupt
            g_Timer2_callBackPtr = NULL_PTR; // Clear callback pointer
            break;
        }
    }
}

// Function to set a callback function for a specific timer
void Timer_setCallBack(void (*a_ptr)(void), Timer_ID_Type a_timer_ID) {
    // Check if the timer ID is valid
    if (a_timer_ID >= NUM_OF_Timers) {
        /* Do Nothing */
    } else {
        switch (a_timer_ID) {
        case TIMER0_ID:
            g_Timer0_callBackPtr = a_ptr; // Set callback for Timer0
            break;
        case TIMER1_ID:
            g_Timer1_callBackPtr = a_ptr; // Set callback for Timer1
            break;
        case TIMER2_ID:
            g_Timer2_callBackPtr = a_ptr; // Set callback for Timer2
            break;
        }
    }
}

// Timer0 overflow interrupt service routine
ISR(TIMER0_OVF_vect) {
    if (g_Timer0_callBackPtr != NULL_PTR) {
        (void) (*g_Timer0_callBackPtr)(); // Call the registered callback
    }
}

// Timer0 compare match interrupt service routine
ISR(TIMER0_COMP_vect) {
    if (g_Timer0_callBackPtr != NULL_PTR) {
        (void) (*g_Timer0_callBackPtr)(); // Call the registered callback
    }
}

// Timer2 compare match interrupt service routine
ISR(TIMER2_COMP_vect) {
    if (g_Timer2_callBackPtr != NULL_PTR) {
        (void) (*g_Timer2_callBackPtr)(); // Call the registered callback
    }
}

// Timer2 overflow interrupt service routine
ISR(TIMER2_OVF_vect) {
    if (g_Timer2_callBackPtr != NULL_PTR) {
        (void) (*g_Timer2_callBackPtr)(); // Call the registered callback
    }
}

// Timer1 overflow interrupt service routine
ISR(TIMER1_OVF_vect) {
    if (g_Timer1_callBackPtr != NULL_PTR) {
        (void) (*g_Timer1_callBackPtr)(); // Call the registered callback
    }
}

// Timer1 compare match interrupt service routine
ISR(TIMER1_COMPA_vect) {
    g_ticks++; // Increment tick count

    // Call the registered callback when total ticks are reached
    if (g_Timer1_callBackPtr != NULL_PTR && (g_ticks == g_total_ticks)) {
        g_ticks = 0; // Reset tick count
        g_Timer1_callBackPtr(); // Call the registered callback
    }
}
