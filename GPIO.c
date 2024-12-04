/******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: gpio.c
 *
 * Description: Source file for the AVR GPIO driver
 *
 * Author: Doaa Said
 *
 *******************************************************************************/

#include "GPIO.h" // Include the GPIO header file
#include "../imp_files/common_macros.h" /* Include common macros like SET_BIT */

/*
 * Description:
 * Setup the direction of the required pin as input or output.
 * If the port or pin number is invalid, the function does nothing.
 */
void GPIO_setupPinDirection(uint8 port_num, uint8 pin_num, GPIO_PinDirectionType direction) {
	// Check if the port or pin number is out of valid range
	if ((pin_num >= NUM_OF_PINS_PER_PORT) || (port_num >= NUM_OF_PORTS)) {
		/* Do Nothing */
	} else {
		// Set the direction for the specified pin
		switch (port_num) {
		case PORTA_ID:
			if (direction == PIN_OUTPUT) {
				SET_BIT(DDRA_REG.Byte, pin_num); // Set pin as output
			} else {
				CLEAR_BIT(DDRA_REG.Byte, pin_num); // Set pin as input
			}
			break;
		case PORTB_ID:
			if (direction == PIN_OUTPUT) {
				SET_BIT(DDRB_REG.Byte, pin_num); // Set pin as output
			} else {
				CLEAR_BIT(DDRB_REG.Byte, pin_num); // Set pin as input
			}
			break;
		case PORTC_ID:
			if (direction == PIN_OUTPUT) {
				SET_BIT(DDRC_REG.Byte, pin_num); // Set pin as output
			} else {
				CLEAR_BIT(DDRC_REG.Byte, pin_num); // Set pin as input
			}
			break;
		case PORTD_ID:
			if (direction == PIN_OUTPUT) {
				SET_BIT(DDRD_REG.Byte, pin_num); // Set pin as output
			} else {
				CLEAR_BIT(DDRD_REG.Byte, pin_num); // Set pin as input
			}
			break;
		}
	}
}

/*
 * Description:
 * Write a logic high or low value to the specified pin.
 * If the port or pin number is invalid, the function does nothing.
 * For input pins, this will configure the internal pull-up resistor.
 */
void GPIO_writePin(uint8 port_num, uint8 pin_num, uint8 value) {
	if ((pin_num >= NUM_OF_PINS_PER_PORT) || (port_num >= NUM_OF_PORTS)) {
		/* Do Nothing */
	} else {
		// Write value to the specified pin
		switch (port_num) {
		case PORTA_ID:
			if (value == LOGIC_HIGH) {
				SET_BIT(PORTA_REG.Byte, pin_num); // Set pin high
			} else {
				CLEAR_BIT(PORTA_REG.Byte, pin_num); // Set pin low
			}
			break;
		case PORTB_ID:
			if (value == LOGIC_HIGH) {
				SET_BIT(PORTB_REG.Byte, pin_num); // Set pin high
			} else {
				CLEAR_BIT(PORTB_REG.Byte, pin_num); // Set pin low
			}
			break;
		case PORTC_ID:
			if (value == LOGIC_HIGH) {
				SET_BIT(PORTC_REG.Byte, pin_num); // Set pin high
			} else {
				CLEAR_BIT(PORTC_REG.Byte, pin_num); // Set pin low
			}
			break;
		case PORTD_ID:
			if (value == LOGIC_HIGH) {
				SET_BIT(PORTD_REG.Byte, pin_num); // Set pin high
			} else {
				CLEAR_BIT(PORTD_REG.Byte, pin_num); // Set pin low
			}
			break;
		}
	}
}

/*
 * Description:
 * Read the value of the specified pin (logic high or low).
 * If the port or pin number is invalid, the function returns logic low.
 */
uint8 GPIO_readPin(uint8 port_num, uint8 pin_num) {
	if ((pin_num >= NUM_OF_PINS_PER_PORT) || (port_num >= NUM_OF_PORTS)) {
		return LOGIC_LOW; // Return low if pin/port is invalid
	} else {
		// Read value from the specified pin
		switch (port_num) {
		case PORTA_ID:
			return (GET_BIT(PINA_REG.Byte, pin_num)); // Return pin state
		case PORTB_ID:
			return (GET_BIT(PINB_REG.Byte, pin_num)); // Return pin state
		case PORTC_ID:
			return (GET_BIT(PINC_REG.Byte, pin_num)); // Return pin state
		case PORTD_ID:
			return (GET_BIT(PIND_REG.Byte, pin_num)); // Return pin state
		}
	}
	return 0; // Fallback return
}

/*
 * Description:
 * Setup the direction of all pins in the specified port as input or output.
 * If the direction value is PORT_INPUT, all pins will be input.
 * If PORT_OUTPUT, all pins will be output.
 * If the port number is invalid, the function does nothing.
 */
void GPIO_setupPortDirection(uint8 port_num, GPIO_PortDirectionType direction) {
	// Check if the port number is valid
	if (port_num >= NUM_OF_PORTS) {
		/* Do Nothing */
	} else {
		// Set the direction for the specified port
		switch (port_num) {
		case PORTA_ID:
			DDRA_REG.Byte = direction; // Set port direction
			break;
		case PORTB_ID:
			DDRB_REG.Byte = direction; // Set port direction
			break;
		case PORTC_ID:
			DDRC_REG.Byte = direction; // Set port direction
			break;
		case PORTD_ID:
			DDRD_REG.Byte = direction; // Set port direction
			break;
		}
	}
}

/*
 * Description:
 * Write a value to the specified port.
 * If any pin in the port is an output pin, the value is written directly.
 * For input pins, this will configure the internal pull-up resistor.
 * If the port number is invalid, the function does nothing.
 */
void GPIO_writePort(uint8 port_num, uint8 value) {
	if (port_num >= NUM_OF_PORTS) {
		/* Do Nothing */
	} else {
		// Write value to the specified port
		switch (port_num) {
		case PORTA_ID:
			PORTA_REG.Byte = value; // Write to port A
			break;
		case PORTB_ID:
			PORTB_REG.Byte = value; // Write to port B
			break;
		case PORTC_ID:
			PORTC_REG.Byte = value; // Write to port C
			break;
		case PORTD_ID:
			PORTD_REG.Byte = value; // Write to port D
			break;
		}
	}
}

/*
 * Description:
 * Read the value of the specified port and return it.
 * If the port number is invalid, the function returns zero.
 */
uint8 GPIO_readPort(uint8 port_num) {
	if (port_num >= NUM_OF_PORTS) {
		return LOGIC_LOW; // Return low if port is invalid
	} else {
		// Read value from the specified port
		switch (port_num) {
		case PORTA_ID:
			return (uint8)(PINA_REG.Byte & 0XFF); // Return port A value
		case PORTB_ID:
			return (uint8)(PINB_REG.Byte & 0XFF); // Return port B value
		case PORTC_ID:
			return (uint8)(PINC_REG.Byte & 0XFF); // Return port C value
		case PORTD_ID:
			return (uint8)(PIND_REG.Byte & 0XFF); // Return port D value
		}
	}

	return 0;
}
