/******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: gpio.h
 *
 * Description: Header file for the AVR GPIO driver
 *
 * Author: Doaa Said
 *
 *******************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_

#include "../imp_files/std_types.h" // Include standard types header

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define NUM_OF_PORTS           4               // Number of GPIO ports
#define NUM_OF_PINS_PER_PORT   8               // Number of pins per port

// Port IDs for easy reference
#define PORTA_ID               0
#define PORTB_ID               1
#define PORTC_ID               2
#define PORTD_ID               3

// Pin IDs for easy reference
#define PIN0_ID                0
#define PIN1_ID                1
#define PIN2_ID                2
#define PIN3_ID                3
#define PIN4_ID                4
#define PIN5_ID                5
#define PIN6_ID                6
#define PIN7_ID                7

// Register definitions for each port and their data direction
#define PORTA_REG      (*(volatile GPIO_Reg_Type*)0x3B) // Port A Data Register
#define DDRA_REG       (*(volatile GPIO_Reg_Type*)0x3A) // Port A Data Direction Register
#define PINA_REG       (*(volatile GPIO_Reg_Type*)0x39) // Port A Input Pins Register
#define PORTB_REG      (*(volatile GPIO_Reg_Type*)0x38) // Port B Data Register
#define DDRB_REG       (*(volatile GPIO_Reg_Type*)0x37) // Port B Data Direction Register
#define PINB_REG       (*(volatile GPIO_Reg_Type*)0x36) // Port B Input Pins Register
#define PORTC_REG      (*(volatile GPIO_Reg_Type*)0x35) // Port C Data Register
#define DDRC_REG       (*(volatile GPIO_Reg_Type*)0x34) // Port C Data Direction Register
#define PINC_REG       (*(volatile GPIO_Reg_Type*)0x33) // Port C Input Pins Register
#define PORTD_REG      (*(volatile GPIO_Reg_Type*)0x32) // Port D Data Register
#define DDRD_REG       (*(volatile GPIO_Reg_Type*)0x31) // Port D Data Direction Register
#define PIND_REG       (*(volatile GPIO_Reg_Type*)0x30) // Port D Input Pins Register

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum {
	PIN_INPUT,  // Input pin configuration
	PIN_OUTPUT  // Output pin configuration
} GPIO_PinDirectionType;

typedef enum {
	PORT_INPUT,        // All pins as input
	PORT_OUTPUT = 0xFF // All pins as output
} GPIO_PortDirectionType;

// Structure for GPIO register representation
typedef union {
	uint8 Byte; // Represents the entire byte
	struct {
		uint8 Bit0 :1; // Bit 0
		uint8 Bit1 :1; // Bit 1
		uint8 Bit2 :1; // Bit 2
		uint8 Bit3 :1; // Bit 3
		uint8 Bit4 :1; // Bit 4
		uint8 Bit5 :1; // Bit 5
		uint8 Bit6 :1; // Bit 6
		uint8 Bit7 :1; // Bit 7
	} Bits; // Individual bits
} GPIO_Reg_Type;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Set the direction of the specified pin (input/output).
 * If the input port number or pin number is invalid, the function does nothing.
 */
void GPIO_setupPinDirection(uint8 port_num, uint8 pin_num,
		GPIO_PinDirectionType direction);

/*
 * Description :
 * Write Logic High or Logic Low to the specified pin.
 * If the input port number or pin number is invalid, the function does nothing.
 * If the pin is configured as input, this function will enable/disable the internal pull-up resistor.
 */
void GPIO_writePin(uint8 port_num, uint8 pin_num, uint8 value);

/*
 * Description :
 * Read the value of the specified pin (Logic High or Logic Low).
 * If the input port number or pin number is invalid, the function returns Logic Low.
 */
uint8 GPIO_readPin(uint8 port_num, uint8 pin_num);

/*
 * Description :
 * Set the direction of the specified port (all pins as input/output).
 * If the direction value is PORT_INPUT, all pins are configured as input.
 * If the direction value is PORT_OUTPUT, all pins are configured as output.
 * If the input port number is invalid, the function does nothing.
 */
void GPIO_setupPortDirection(uint8 port_num, GPIO_PortDirectionType direction);

/*
 * Description :
 * Write the specified value to the entire port.
 * If any pin in the port is configured as output, the value is written.
 * If any pin in the port is configured as input, this will activate/deactivate the internal pull-up resistor.
 * If the input port number is invalid, the function does nothing.
 */
void GPIO_writePort(uint8 port_num, uint8 value);

/*
 * Description :
 * Read and return the value of the specified port.
 * If the input port number is invalid, the function returns zero.
 */
uint8 GPIO_readPort(uint8 port_num);

#endif /* GPIO_H_ */
