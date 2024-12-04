/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Doaa Said
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "../imp_files/std_types.h" // Include standard types header

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

// Register definitions for UART control and data
#define UCSRA_REG  (*(volatile  UART_UCSRA_Type*) 0x2B ) // UART Control and Status Register A
#define UCSRB_REG  (*(volatile  UART_UCSRB_Type*) 0x2A) // UART Control and Status Register B
#define UCSRC_REG  (*(volatile  UART_UCSRC_Type*) 0x40) // UART Control and Status Register C
#define UDR_REG    (*(volatile  uint8*) 0x2C)             // UART Data Register

// Uncomment for different receive modes
#define UART_RECIEVE_POLLING // Polling mode for receiving
//#define UART_RECIVE_INTERRUPT // Interrupt mode for receiving

#define UART_UDR_EMPTY_POLLING // Polling mode to check if UDR is empty

#define UBRRL_REG  (*(volatile  uint8*) 0x29) // UART Baud Rate Register Low
#define UBRRH_REG  (*(volatile  uint8*) 0x40) // UART Baud Rate Register High

#define URSEL_bitNum 7 // URSEL bit number in UCSRC

#define UCSZ0_bitNum 1 // UCSZ0 bit number in UCSRC
#define USBS_bitNum 3   // USBS bit number in UCSRC

#define UPM0_bitNum 4  // UPM0 bit number in UCSRC

// Global flags for UART operation
extern uint8 volatile g_UART_recieve_flag; // Flag for UART receive status
extern uint8 volatile g_UART_UDR_EMP_flag; // Flag for UDR empty status
extern uint8 volatile g_UART_UDRvalue; // Value received in UDR

/*******************************************************************************
 *                      Types Declaration                                    *
 *******************************************************************************/

// Enum for UART parity modes
typedef enum {
	DISABLED, RESERVED, EVEN, ODD
} UART_paritymode;

// Enum for UART stop bits configuration
typedef enum {
	one_bit, two_bits
} UART_stopbit;

// Enum for UART character sizes
typedef enum {
	FIVE_BITS, SIX_BITS, SEVEN_BITS, EIGHT_BITS, R1, R2, R3, NINE_BITS
} UART_charsize;

// Configuration structure for UART settings
typedef struct {
	UART_charsize char_size;   // Character size
	UART_paritymode parity_mode; // Parity mode
	UART_stopbit stop_bit;      // Stop bit configuration
	uint32 baud_rate;           // Baud rate
} UART_ConfigType;

// Union for UCSRA register representation
typedef union {
	uint8 Byte; // Represents the entire byte
	struct {
		uint8 MPCM_Bit :1; // Multi-processor communication mode
		uint8 U2X_Bit :1;  // Double the transmission speed
		uint8 PE_Bit :1;   // Parity error flag
		uint8 DOR_Bit :1;  // Data overrun flag
		uint8 FE_Bit :1;   // Frame error flag
		uint8 UDRE_Bit :1; // UART Data Register Empty
		uint8 TXC_Bit :1;  // Transmit complete flag
		uint8 RXC_Bit :1;  // Receive complete flag
	} Bits; // Individual bits
} UART_UCSRA_Type;

// Union for UCSRB register representation
typedef union {
	uint8 Byte; // Represents the entire byte
	struct {
		uint8 TXB8_Bit :1; // Transmit data bit 8
		uint8 RXB8_Bit :1; // Receive data bit 8
		uint8 UCSZ2_Bit :1; // Character size bit 2
		uint8 TXEN_Bit :1;  // Transmitter enable
		uint8 RXEN_Bit :1;  // Receiver enable
		uint8 UDRIE_Bit :1; // UART Data Register Empty Interrupt Enable
		uint8 TXCIE_Bit :1; // Transmit Complete Interrupt Enable
		uint8 RXCIE_Bit :1; // Receive Complete Interrupt Enable
	} Bits; // Individual bits
} UART_UCSRB_Type;

// Union for UCSRC register representation
typedef union {
	uint8 Byte; // Represents the entire byte
	struct {
		uint8 UCPOL_Bit :1; // Clock polarity
		uint8 UCSZ0_Bit :1; // Character size bit 0
		uint8 UCSZ1_Bit :1; // Character size bit 1
		uint8 USBS_Bit :1;  // Stop bit select
		uint8 UPM0_Bit :1;  // Parity mode bit 0
		uint8 UPM1_Bit :1;  // Parity mode bit 1
		uint8 UMSEL_Bit :1; // UART Mode Select
		uint8 URSEL_Bit :1; // Register select
	} Bits; // Individual bits
} UART_UCSRC_Type;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize the UART device by:
 * 1. Setting up the frame format (data bits, parity, stop bits).
 * 2. Enabling the UART.
 * 3. Configuring the UART baud rate.
 */
void UART_init(UART_ConfigType* UART_ConfigType);

/*
 * Description :
 * Send a byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Receive a byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send a string through UART to another device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive a string until the '#' symbol from another UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until '#' symbol

#endif /* UART_H_ */
