/******************************************************************************
 * File Name: main.c
 *
 * Description: source file for the HMI_ECU application
 *
 * Author: Doaa Said
 *
 *******************************************************************************/
#include"../imp_files/std_types.h"
#include"../HAL_Drivers/LCD.h"
#include"../HAL_Drivers/keypad.h"
#include"../MCAL_Drivers/UART.h"
#include"../MCAL_Drivers/Timer.h"
#include"util/delay.h"
#include"main.h"

/*Timer configuration for Timer1 in CTC mode
 ** F_CPU = 8MHz, prescaler = 256
 ** For a timer to generate an interrupt every second:
 ** OCR1A=1s/(256/8000000)=31250;
 ** Therefore, we set OCR1A to 31250
 */
Timer_ConfigType Timer_config = { 0, 31250, TIMER1_ID, CLK_OVER_256, CTC_1 };

// Counters for failed password attempts
uint8 num_of_fault_in_pass_when_open_door = 0;
uint8 num_of_fault_in_pass_when_change_pass = 0;

int main(void) {
	// Enable global interrupts
	SREG_REG.Bits.I_Bit = LOGIC_HIGH;

	// Initialize LCD
	LCD_init();

	// UART configuration and initialization
	UART_ConfigType config = { EIGHT_BITS, DISABLED, one_bit, 9600 };
	UART_init(&config);

	// Start the first step of the process
	step1();

	for (;;) {

	}
}

// Function to display alarm message
void Alarm_message() {
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 1, "System locked");
	LCD_displayStringRowColumn(1, 0, "wait for 1 min");
}

// Function to handle password change
void pass_change() {
	UART_sendByte(HMI_ready);
	while (UART_recieveByte() != CONTROL_ready)
		; // Wait for control acknowledgment

	uint8 is_match = UART_recieveByte(); // Receive match status
	if (is_match) {
		num_of_fault_in_pass_when_change_pass = 0; // Reset fault counter
		LCD_clearScreen();
		step1(); // Proceed to step 1
	} else {
		num_of_fault_in_pass_when_change_pass++;
		if (num_of_fault_in_pass_when_change_pass < 3) {
			LCD_clearScreen();
			step3(); // Prompt for password again
			pass_change(); // Retry password change
		} else {
			num_of_fault_in_pass_when_change_pass = 0;
			UART_sendByte(Alarm); // Trigger alarm
			Alarm_message(); // Show alarm message
			g_total_ticks = 60; /* Set timer duration to call call-
			 back function after 60s(interrupt after 1s)*/
			Timer_init(&Timer_config); // Initialize timer
			Timer_setCallBack(step2, TIMER1_ID); // Set callback for step2

		}
	}
}

// Function to display Door Unlocking if passwords matched
void Door_unlocking() {
	UART_sendByte(HMI_ready);
	while (UART_recieveByte() != CONTROL_ready)
		; // Wait for control acknowledgment

	uint8 is_match = UART_recieveByte(); // Receive match status
	if (is_match) {
		num_of_fault_in_pass_when_open_door = 0; // Reset fault counter
		UART_sendByte(OPEN_DOOR); // Send open door command
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Door Unlocking");
		LCD_displayStringRowColumn(1, 3, "please wait..");
		g_total_ticks = 15; /*Set timer duration to call call-
		 back function after 15s(interrupt after 1s)*/
		Timer_init(&Timer_config); // Initialize timer
		Timer_setCallBack(display_wait, TIMER1_ID); // Set display wait callback

	} else {
		num_of_fault_in_pass_when_open_door++;
		if (num_of_fault_in_pass_when_open_door < 3) {
			LCD_clearScreen();
			step3(); // Prompt for password again
			Door_unlocking(); // Retry door unlocking
		} else {
			num_of_fault_in_pass_when_open_door = 0;
			UART_sendByte(Alarm); // Trigger alarm
			Alarm_message(); // Show alarm message
			g_total_ticks = 60; /*Set timer duration to call call-
			 back function after 60s(interrupt after 1s)*/
			Timer_init(&Timer_config); // Initialize timer
			Timer_setCallBack(step2, TIMER1_ID); // Set callback for step2

		}
	}
}

// Function to handle the first step of password creating
void step1(void) {
	uint8 real_password[PASS_SIZE], counter = 0, confirmed_password[PASS_SIZE];
	LCD_displayStringRowColumn(0, 0, "plz enter pass: ");
	LCD_moveCursor(1, 0);

	// Get real password from user input
	for (counter = 0; counter < PASS_SIZE; counter++) {
		uint8 read = KEYPAD_getPressedKey();

		if (read >= 0 && read <= 9) { // Check if the key is a valid digit
			real_password[counter] = read;
			// store read in password
			_delay_ms(300); //Small delay for stability
			LCD_displayCharacter('*'); // Display asterisk for security
		} else {
			counter--; // Decrement counter if invalid key is pressed
		}
	}

	while (KEYPAD_getPressedKey() != Enter)
		; // Wait for Enter key
	_delay_ms(200); //Small delay for stability
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "plz re-enter the");
	LCD_displayStringRowColumn(1, 0, "same pass: ");

	// Get confirmed password from user input
	for (counter = 0; counter < PASS_SIZE; counter++) {
		uint8 read = KEYPAD_getPressedKey();

		if (read >= 0 && read <= 9) { //Check if the key is a valid digit
			confirmed_password[counter] = read;
			// store read in confirmed password
			_delay_ms(300); //Small delay for stability
			LCD_displayCharacter('*'); // Display asterisk for security
		} else {
			counter--; // Decrement counter if invalid key is pressed
		}
	}

	while (KEYPAD_getPressedKey() != Enter)
		; // Wait for Enter key
	_delay_ms(200);

	// Send password confirmation request
	UART_sendByte(SAVE_PASS_and_confirm);
	UART_sendByte(HMI_ready);
	while (UART_recieveByte() != CONTROL_ready)
		; // Wait for control acknowledgment

	// Send both passwords
	UART_sendString(real_password);
	UART_sendByte('#');
	UART_sendString(confirmed_password);
	UART_sendByte('#');

	UART_sendByte(HMI_ready);
	while (UART_recieveByte() != CONTROL_ready)
		; // Wait for control acknowledgment
	uint8 result = UART_recieveByte(); // Get match result
	if (result == matched) {
		LCD_clearScreen();
		step2(); // Proceed to step 2
	} else {
		LCD_clearScreen();
		step1(); // Retry password input
	}
}
// Function to handle step 2 of the system and show main options
void step2() {
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+ : Open Door");
	LCD_displayStringRowColumn(1, 0, "- : Change Pass");
	Timer_deInit(TIMER1_ID); // Deinitialize timer

	// Check user input for actions
	uint8 read = KEYPAD_getPressedKey();

	while (read != '+' && read != '-') {
		read = KEYPAD_getPressedKey();
	}
	if (read == '+') {
		_delay_ms(300);
		step3(); // Prompt for password to open door
		Door_unlocking(); // Attempt to unlock door
		;
	} else if (read == '-') {
		_delay_ms(300);
		step3(); // Prompt for password to change
		pass_change(); // Attempt to change password

	}
}

// Function to handle waiting during door unlocking
void display_wait() {
	LCD_clearScreen();
	UART_sendByte(HMI_ready);
	if (UART_recieveByte() == people_detected) {
		LCD_displayStringRowColumn(0, 0, "wait for people");
		LCD_displayStringRowColumn(1, 2, "to enter");
		Timer_deInit(TIMER1_ID); // Deinitialize timer

		// Wait for people to exit detection
		while (UART_recieveByte() != people_notdetected)
			;
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 2, "Door locking");
		g_total_ticks = 15; /*Set timer duration to call call-
		 back function after 15s(interrupt after 1s)*/
		Timer_init(&Timer_config); // Initialize timer
		Timer_setCallBack(step2, TIMER1_ID); // Set callback for step 2

	}
}

// Function to handle password entry for step 3
void step3(void) {
	uint8 counter = 0, Entered_pass[6];
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "enter door pass: ");
	LCD_moveCursor(1, 0);

	// Get entered password from user input
	for (counter = 0; counter < PASS_SIZE; counter++) {
		uint8 read = KEYPAD_getPressedKey();
		if (read >= 0 && read <= 9) { // Check if the key is a valid digit
			Entered_pass[counter] = read; //store read in Entered_pass
			_delay_ms(300); //small delay for stability
			LCD_displayCharacter('*'); // Display asterisk for security
		} else {
			counter--; // Decrement counter if invalid key is pressed
		}
	}

	Entered_pass[counter] = '\0'; // Null-terminate the string
	while (KEYPAD_getPressedKey() != Enter)
		; // Wait for Enter key
	_delay_ms(300);

	UART_sendByte(CHECK_PASS); // Send check password command
	while (UART_recieveByte() != CONTROL_ready)
		; // Wait for control acknowledgment
	UART_sendString(Entered_pass); // Send entered password
	UART_sendByte('#'); // End of string
}

