/******************************************************************************
 * File Name: main.h
 *
 * Description: Header file for the HMI_ECU application
 *
 * Author: Doaa Said
 *
 *******************************************************************************/
#ifndef MAIN_H_
#define MAIN_H_
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
// Register definition for the status register (SREG)
#define SREG_REG      (*(volatile SREG_Type*)0x5F)

// size of the password
#define PASS_SIZE 5

// Status constants for password matching
#define matched 1
#define unmatched 0

// Communication codes between HMI and Control
#define HMI_ready 0x01
#define CONTROL_ready 0x02
#define OPEN_DOOR 0X03
#define CHANGE_PASS 0X04
#define SAVE_PASS_and_confirm 0X05
#define CHECK_PASS 0X06
#define recieve_result 0x44

// Motion detection status
#define people_detected 1
#define people_notdetected 0

// Alarm signal code
#define Alarm 0x55

// Key code for Enter key
#define Enter 13

/*******************************************************************************
 *                      Types Declaration                                    *
 *******************************************************************************/

// Structure to represent the bits of the status register (SREG)
typedef union {
	uint8 Byte; // Byte representation
	struct {
		uint8 C_Bit :1; // Carry flag
		uint8 Z_Bit :1; // Zero flag
		uint8 N_Bit :1; // Negative flag
		uint8 V_Bit :1; // Overflow flag
		uint8 S_Bit :1; // Sign flag
		uint8 H_Bit :1; // Half-carry flag
		uint8 T_Bit :1; // T flag
		uint8 I_Bit :1; // Interrupt flag
	} Bits; // Individual bits
} SREG_Type;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/* Function to handle the first step of password creating*/
void step1(void);
/* Function to handle step 2 of the system and show main options*/
void step2(void);
/* Function to manage waiting during door unlocking*/
void display_wait(void);
/* Function to prompt for password entry*/
void step3(void);
/* Function to display Door Unlocking if passwords matched*/
void Door_unlocking(void);
/* Function to handle password change*/
void pass_change(void);
/* Function to display alarm messages*/
void Alarm_message(void);

#endif /* MAIN_H_ */
