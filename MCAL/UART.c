/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Doaa Said
 *
 *******************************************************************************/

#include "UART.h"                  // Include the UART header file
#include "../imp_files/common_macros.h" // Include common macros
#include <avr/interrupt.h>         // Include AVR interrupt header
#include "../imp_files/std_types.h" // Include standard types

// Global variables for UART status flags
uint8 volatile g_UART_recieve_flag = 0; // Flag for indicating byte received
uint8 volatile g_UART_UDR_EMP_flag = 0;  // Flag for UDR empty status
uint8 volatile g_UART_UDRvalue;           // Value received in UDR

/*******************************************************************************
 * Function: UART_init
 *
 * Description:
 * Initializes the UART device with the specified configuration.
 * Sets up frame format, enables the UART, and configures baud rate.
 *
 * Parameters:
 *  UART_ConfigType* UART_ConfigType - Pointer to the UART configuration structure.
 *******************************************************************************/
void UART_init(UART_ConfigType* UART_ConfigType) {
    // Set double speed mode
    UCSRA_REG.Bits.U2X_Bit = LOGIC_HIGH;

    // Enable receiver and transmitter
    UCSRB_REG.Bits.RXEN_Bit = LOGIC_HIGH;
    UCSRB_REG.Bits.TXEN_Bit = LOGIC_HIGH;

    // Set character size
    UCSRC_REG.Byte |= (1 << URSEL_bitNum) | ((UART_ConfigType->char_size & 0x03) << UCSZ0_bitNum);

    // Set parity mode
    UCSRC_REG.Byte |= (1 << URSEL_bitNum) | ((UART_ConfigType->parity_mode & 0x03) << UPM0_bitNum);

    // Set stop bit
    UCSRC_REG.Byte |= (1 << URSEL_bitNum) | ((UART_ConfigType->stop_bit & 0x01) << USBS_bitNum);

    // Set the stop bit based on the provided configuration
    UCSRC_REG.Bits.USBS_Bit = GET_BIT(UART_ConfigType->stop_bit, 0);

    #ifdef UART_RECIVE_INTERRUPT
    // Enable receive interrupt
    UCSRB_REG.Bits.RXCIE_Bit = LOGIC_HIGH;
    #endif

    #ifdef UART_UDR_EMPTY_INTERRUPT
    // Enable UDR empty interrupt
    UCSRB_REG.Bits.UDRIE_Bit = LOGIC_HIGH;
    #endif

    // Calculate the baud rate register value
    uint16 ubrr_value = (uint16)(((F_CPU / (UART_ConfigType->baud_rate * 8UL))) - 1);
    UBRRL_REG = ubrr_value;    // Set low byte of baud rate
    UBRRH_REG = ubrr_value >> 8; // Set high byte of baud rate
}

/*******************************************************************************
 * Function: UART_sendByte
 *
 * Description:
 * Sends a single byte via UART. Waits until the UDR is empty before sending.
 *
 * Parameters:
 *  const uint8 data - The byte to send.
 *******************************************************************************/
#ifdef UART_UDR_EMPTY_POLLING
void UART_sendByte(const uint8 data) {
    // Wait until UDR is empty
    while (!UCSRA_REG.Bits.UDRE_Bit) {};

    // Load data into UDR for transmission
    UDR_REG = data;
}

/*******************************************************************************
 * Function: UART_sendString
 *
 * Description:
 * Sends a string of characters via UART.
 *
 * Parameters:
 *  const uint8 *Str - Pointer to the string to send.
 *******************************************************************************/
void UART_sendString(const uint8 *Str) {
    // Loop through each character in the string and send it
    while (*Str) {
        UART_sendByte(*Str);
        Str++; // Move to the next character
    }
}
#endif

/*******************************************************************************
 * Function: UART_recieveByte
 *
 * Description:
 * Receives a single byte via UART. Waits until data is received.
 *
 * Returns:
 *  uint8 - The received byte.
 *******************************************************************************/
#ifdef UART_RECIEVE_POLLING
uint8 UART_recieveByte(void) {
    // Wait until data is received
    while (!UCSRA_REG.Bits.RXC_Bit);

    // Return the received byte from UDR
    return UDR_REG;
}

/*******************************************************************************
 * Function: UART_receiveString
 *
 * Description:
 * Receives a string of characters until the '#' symbol is encountered.
 *
 * Parameters:
 *  uint8 *Str - Pointer to the buffer to store the received string.
 *******************************************************************************/
void UART_receiveString(uint8 *Str) {
    uint8 i = 0;

    // Receive the first byte
    Str[i] = UART_recieveByte();

    // Continue receiving until '#' is encountered
    while (Str[i] != '#') {
        i++; // Increment index
        Str[i] = UART_recieveByte(); // Receive next byte
    }

    Str[i] = '\0'; // Null-terminate the string
}
#endif

/*******************************************************************************
 * Interrupt Service Routine: USART_RXC_vect
 *
 * Description:
 * Handles the receive complete interrupt. Sets the receive flag and stores
 * the received byte in a global variable.
 *******************************************************************************/
#ifdef UART_RECIVE_INTERRUPT
ISR(USART_RXC_vect) {
    g_UART_recieve_flag = 1; // Set the receive flag
    g_UART_UDRvalue = UDR_REG; // Store the received byte
}
#endif

/*******************************************************************************
 * Interrupt Service Routine: USART_UDRE_vect
 *
 * Description:
 * Handles the UDR empty interrupt. Sets the UDR empty flag.
 *******************************************************************************/
#ifdef UART_UDR_EMPTY_INTERRUPT
ISR(USART_UDRE_vect) {
    g_UART_UDR_EMP_flag = 1; // Set the UDR empty flag
}
#endif
