/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer.h
 *
 * Description: header file for the Timer AVR driver
 *
 * Author: Doaa Said
 *
 *******************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

#include "../imp_files/std_types.h"

/*******************************************************************************
 *                      Definitions                                            *
 *******************************************************************************/
// Define the total number of timers available
#define NUM_OF_Timers          3

// Global variable to keep track of total ticks
extern volatile uint8 g_total_ticks;

/*********************************** Timers Registers Definitions ******************************/
// Define memory-mapped registers for timer interrupt flags
#define TIFR_REG      (*(volatile Timers_TIFR_Type*)0x58) // Timer Interrupt Flag Register
#define TIMSK_REG     (*(volatile Timers_TIMSK_Type*)0x59) // Timer Interrupt Mask Register

/*********************************** Timer0 Registers Definitions ******************************/
// Define memory-mapped registers for Timer0
#define TCNT0_REG    (*(volatile Timer0_TCNT0_Type*)0x52) // Timer/Counter Register
#define TCCR0_REG    (*(volatile Timer0_TCCR0_Type*)0x53) // Timer/Counter Control Register
#define OCR0_REG     (*(volatile Timer0_OCR0_Type*)0x5C) // Output Compare Register

/*********************************** Timer1 Registers Definitions ******************************/
// Define memory-mapped registers for Timer1
#define TCNT1_REG     (*(volatile Timer1_TCNT1_Type*)0x4C) // Timer/Counter Register
#define TCCR1A_REG    (*(volatile Timer1_TCCR1A_Type*)0x4F) // Timer/Counter Control Register A
#define TCCR1B_REG    (*(volatile Timer1_TCCR1B_Type*)0x4E) // Timer/Counter Control Register B
#define OCR1A_REG     (*(volatile Timer1_OCR1A_Type*)0x4A) // Output Compare Register A
#define OCR1B_REG     (*(volatile Timer1_OCR1B_Type*)0x48) // Output Compare Register B
#define ICR1_REG      (*(volatile Timer1_ICR1_Type*)0x46)  // Input Capture Register

/*********************************** Timer2 Registers Definitions ******************************/
// Define memory-mapped registers for Timer2
#define TCNT2_REG     (*(volatile Timer2_TCNT2_Type*)0x44) // Timer/Counter Register
#define TCCR2_REG     (*(volatile Timer2_TCCR2_Type*)0x45) // Timer/Counter Control Register
#define OCR2_REG      (*(volatile Timer2_OCR2_Type*)0x43)  // Output Compare Register

/*******************************************************************************
 *                      Types Declaration                                      *
 *******************************************************************************/
// Enum for identifying timers
typedef enum {
	TIMER0_ID, TIMER1_ID, TIMER2_ID
} Timer_ID_Type;

// Enum for timer clock sources
typedef enum {
	No_CLK_SOURCE,
	No_PRESCALING,
	CLK_OVER_8,
	CLK_OVER_64,
	CLK_OVER_256,
	CLK_OVER_1024
} Timer_ClockType;

// Enum for timer modes
typedef enum {
	NORMAL, CTC_0_OR_2 = 2, CTC_1 = 4
} Timer_ModeType;

/*************************Timers Registers type structure declarations ************************/
// Structure for Timer Interrupt Mask Register
typedef union {
	uint8 Byte;
	struct {
		uint8 TOIE0_Bit :1;  // Timer0 Overflow Interrupt Enable
		uint8 OCIE0_Bit :1;  // Timer0 Output Compare Interrupt Enable
		uint8 TOIE1_Bit :1;  // Timer1 Overflow Interrupt Enable
		uint8 OCIE1B_Bit :1; // Timer1 Output Compare B Interrupt Enable
		uint8 OCIE1A_Bit :1; // Timer1 Output Compare A Interrupt Enable
		uint8 TICIE1_Bit :1; // Timer1 Input Capture Interrupt Enable
		uint8 TOIE2_Bit :1;  // Timer2 Overflow Interrupt Enable
		uint8 OCIE2_Bit :1;  // Timer2 Output Compare Interrupt Enable
	} Bits;
} Timers_TIMSK_Type;

// Structure for Timer Interrupt Flag Register
typedef union {
	uint8 Byte;
	struct {
		uint8 TOV0_Bit :1;   // Timer0 Overflow Flag
		uint8 OCF0_Bit :1;   // Timer0 Output Compare Flag
		uint8 TOV1_Bit :1;   // Timer1 Overflow Flag
		uint8 OCF1B_Bit :1;  // Timer1 Output Compare B Flag
		uint8 OCF1A_Bit :1;  // Timer1 Output Compare A Flag
		uint8 ICF1_Bit :1;   // Timer1 Input Capture Flag
		uint8 TOV2_Bit :1;   // Timer2 Overflow Flag
		uint8 OCF2_Bit :1;   // Timer2 Output Compare Flag
	} Bits;
} Timers_TIFR_Type;

/************************* Timer0 Registers type structure declarations ************************/
typedef union {
	uint8 Byte;
	struct {
		uint8 CS00_Bit :1;   // Clock Select Bit 0
		uint8 CS01_Bit :1;   // Clock Select Bit 1
		uint8 CS02_Bit :1;   // Clock Select Bit 2
		uint8 WGM01_Bit :1;   // Waveform Generation Mode Bit 1
		uint8 COM00_Bit :1;   // Compare Match Output Mode Bit 0
		uint8 COM01_Bit :1;   // Compare Match Output Mode Bit 1
		uint8 WGM00_Bit :1;   // Waveform Generation Mode Bit 0
		uint8 FOC0_Bit :1;    // Force Output Compare
	} Bits;
} Timer0_TCCR0_Type;

typedef union {
	uint8 Byte;
	struct {
		uint16 Bit0 :1;     // Bit 0
		uint16 Bit1 :1;     // Bit 1
		uint16 Bit2 :1;     // Bit 2
		uint16 Bit3 :1;     // Bit 3
		uint16 Bit4 :1;     // Bit 4
		uint16 Bit5 :1;     // Bit 5
		uint16 Bit6 :1;     // Bit 6
		uint16 Bit7 :1;     // Bit 7
	} Bits;
} Timer0_TCNT0_Type;

typedef union {
	uint8 Byte;
	struct {
		uint16 Bit0 :1;     // Bit 0
		uint16 Bit1 :1;     // Bit 1
		uint16 Bit2 :1;     // Bit 2
		uint16 Bit3 :1;     // Bit 3
		uint16 Bit4 :1;     // Bit 4
		uint16 Bit5 :1;     // Bit 5
		uint16 Bit6 :1;     // Bit 6
		uint16 Bit7 :1;     // Bit 7
	} Bits;
} Timer0_OCR0_Type;

/************************* Timer1 Registers type structure declarations ************************/
typedef union {
	uint8 Byte;
	struct {
		uint8 WGM10_Bit :1;  // Waveform Generation Mode Bit 0
		uint8 WGM11_Bit :1;  // Waveform Generation Mode Bit 1
		uint8 FOC1A_Bit :1;  // Force Output Compare A
		uint8 FOC1B_Bit :1;  // Force Output Compare B
		uint8 COM1B0_Bit :1; // Compare Match Output Mode B Bit 0
		uint8 COM1B1_Bit :1; // Compare Match Output Mode B Bit 1
		uint8 COM1A0_Bit :1; // Compare Match Output Mode A Bit 0
		uint8 COM1A1_Bit :1; // Compare Match Output Mode A Bit 1
	} Bits;
} Timer1_TCCR1A_Type;

typedef union {
	uint8 Byte;
	struct {
		uint8 CS10_Bit :1;   // Clock Select Bit 0
		uint8 CS11_Bit :1;   // Clock Select Bit 1
		uint8 CS12_Bit :1;   // Clock Select Bit 2
		uint8 WGM12_Bit :1;   // Waveform Generation Mode Bit 2
		uint8 WGM13_Bit :1;   // Waveform Generation Mode Bit 3
		uint8 :1;             // Reserved
		uint8 ICES1_Bit :1;   // Input Capture Edge Select
		uint8 ICNC1_Bit :1;   // Input Capture Noise Canceler
	} Bits;
} Timer1_TCCR1B_Type;

typedef union {
	uint16 TwoBytes;
	struct {
		uint16 Bit0 :1;      // Bit 0
		uint16 Bit1 :1;      // Bit 1
		uint16 Bit2 :1;      // Bit 2
		uint16 Bit3 :1;      // Bit 3
		uint16 Bit :1;       // Bit 4
		uint16 Bit5 :1;      // Bit 5
		uint16 Bit6 :1;      // Bit 6
		uint16 Bit7 :1;      // Bit 7
		uint16 Bit8 :1;      // Bit 8
		uint16 Bit9 :1;      // Bit 9
		uint16 Bit10 :1;     // Bit 10
		uint16 Bit11 :1;     // Bit 11
		uint16 Bit12 :1;     // Bit 12
		uint16 Bit13 :1;     // Bit 13
		uint16 Bit14 :1;     // Bit 14
		uint16 Bit15 :1;     // Bit 15
	} Bits;
} Timer1_TCNT1_Type;

typedef union {
	uint16 TwoBytes;
	struct {
		uint16 Bit0 :1;      // Bit 0
		uint16 Bit1 :1;      // Bit 1
		uint16 Bit2 :1;      // Bit 2
		uint16 Bit3 :1;      // Bit 3
		uint16 Bit :1;       // Bit 4
		uint16 Bit5 :1;      // Bit 5
		uint16 Bit6 :1;      // Bit 6
		uint16 Bit7 :1;      // Bit 7
		uint16 Bit8 :1;      // Bit 8
		uint16 Bit9 :1;      // Bit 9
		uint16 Bit10 :1;     // Bit 10
		uint16 Bit11 :1;     // Bit 11
		uint16 Bit12 :1;     // Bit 12
		uint16 Bit13 :1;     // Bit 13
		uint16 Bit14 :1;     // Bit 14
		uint16 Bit15 :1;     // Bit 15
	} Bits;
} Timer1_OCR1A_Type;

typedef union {
	uint16 TwoBytes;
	struct {
		uint16 Bit0 :1;      // Bit 0
		uint16 Bit1 :1;      // Bit 1
		uint16 Bit2 :1;      // Bit 2
		uint16 Bit3 :1;      // Bit 3
		uint16 Bit :1;       // Bit 4
		uint16 Bit5 :1;      // Bit 5
		uint16 Bit6 :1;      // Bit 6
		uint16 Bit7 :1;      // Bit 7
		uint16 Bit8 :1;      // Bit 8
		uint16 Bit9 :1;      // Bit 9
		uint16 Bit10 :1;     // Bit 10
		uint16 Bit11 :1;     // Bit 11
		uint16 Bit12 :1;     // Bit 12
		uint16 Bit13 :1;     // Bit 13
		uint16 Bit14 :1;     // Bit 14
		uint16 Bit15 :1;     // Bit 15
	} Bits;
} Timer1_OCR1B_Type;

typedef union {
	uint16 TwoBytes;
	struct {
		uint16 Bit0 :1;      // Bit 0
		uint16 Bit1 :1;      // Bit 1
		uint16 Bit2 :1;      // Bit 2
		uint16 Bit3 :1;      // Bit 3
		uint16 Bit :1;       // Bit 4
		uint16 Bit5 :1;      // Bit 5
		uint16 Bit6 :1;      // Bit 6
		uint16 Bit7 :1;      // Bit 7
		uint16 Bit8 :1;      // Bit 8
		uint16 Bit9 :1;      // Bit 9
		uint16 Bit10 :1;     // Bit 10
		uint16 Bit11 :1;     // Bit 11
		uint16 Bit12 :1;     // Bit 12
		uint16 Bit13 :1;     // Bit 13
		uint16 Bit14 :1;     // Bit 14
		uint16 Bit15 :1;     // Bit 15
	} Bits;
} Timer1_ICR1_Type;

/************************* Timer2 Registers type structure declarations ************************/
typedef union {
	uint8 Byte;
	struct {
		uint8 CS20_Bit :1;   // Clock Select Bit 0
		uint8 CS21_Bit :1;   // Clock Select Bit 1
		uint8 CS22_Bit :1;   // Clock Select Bit 2
		uint8 WGM21_Bit :1;   // Waveform Generation Mode Bit 1
		uint8 COM20_Bit :1;   // Compare Match Output Mode Bit 0
		uint8 COM21_Bit :1;   // Compare Match Output Mode Bit 1
		uint8 WGM20_Bit :1;   // Waveform Generation Mode Bit 0
		uint8 FOC2_Bit :1;    // Force Output Compare
	} Bits;
} Timer2_TCCR2_Type;

typedef union {
	uint8 Byte;
	struct {
		uint16 Bit0 :1;      // Bit 0
		uint16 Bit1 :1;      // Bit 1
		uint16 Bit2 :1;      // Bit 2
		uint16 Bit3 :1;      // Bit 3
		uint16 Bit4 :1;      // Bit 4
		uint16 Bit5 :1;      // Bit 5
		uint16 Bit6 :1;      // Bit 6
		uint16 Bit7 :1;      // Bit 7
	} Bits;
} Timer2_TCNT2_Type;

typedef union {
	uint8 Byte;
	struct {
		uint16 Bit0 :1;      // Bit 0
		uint16 Bit1 :1;      // Bit 1
		uint16 Bit2 :1;      // Bit 2
		uint16 Bit3 :1;      // Bit 3
		uint16 Bit4 :1;      // Bit 4
		uint16 Bit5 :1;      // Bit 5
		uint16 Bit6 :1;      // Bit 6
		uint16 Bit7 :1;      // Bit 7
	} Bits;
} Timer2_OCR2_Type;

/****************************************************************************************************/
// Structure for timer configuration parameters
typedef struct {
	uint16 timer_InitialValue;          // Initial value for the timer
	uint16 timer_compare_MatchValue;    // Compare match value (used in compare mode)
	Timer_ID_Type timer_ID;              // Identifier for the timer
	Timer_ClockType timer_clock;         // Clock source for the timer
	Timer_ModeType timer_mode;           // Mode of operation for the timer
} Timer_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Initializes the Timer driver with the provided configuration.
 */
void Timer_init(const Timer_ConfigType *Config_Ptr);

/*
 * Description :
 * Disables the specified Timer identified by Timer_ID.
 */
void Timer_deInit(Timer_ID_Type timer_type);

/*
 * Description :
 * Sets the callback function address for the specified Timer.
 */
void Timer_setCallBack(void (*a_ptr)(void), Timer_ID_Type a_timer_ID);

#endif /* TIMER_H_ */
