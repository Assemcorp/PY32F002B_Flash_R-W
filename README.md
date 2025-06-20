# PY32F002B_Flash_R-W
PY32F002B Flash Write Read Example

Project Overview
This project is an embedded software application running on the PY32F002B microcontroller. Its main purpose is to perform read and write operations on the internal flash memory of the microcontroller. Additionally, it demonstrates basic control of hardware peripherals such as LEDs and buttons.

Main Functions of the Project
Flash Memory Read and Write:

The project can write a specific block of data to the microcontroller’s internal flash memory and later read it back for verification.
These operations are essential for storing persistent data, saving configuration settings, or performing firmware updates in embedded systems.
Data integrity and safety checks are performed during flash operations.
LED Control:

The onboard LED is controlled via GPIO pins.
The software can turn the LED on or off, or make it blink at certain intervals to indicate system status or operation results.
Button Reading:

The user button state is monitored.
When the button is pressed, it can trigger a flash write/read operation or change the LED state as feedback.
Hardware Abstraction Layer (HAL) Usage:

The project uses Puya’s HAL (Hardware Abstraction Layer) drivers to simplify hardware access and improve code portability.
This allows easy management of peripherals like GPIO, Flash, and RCC without dealing directly with hardware registers.
Project Workflow
On startup, the system initializes all necessary hardware peripherals (clock, GPIO, Flash).
In the main loop, the software checks if the user button is pressed.
If the button is pressed, a predefined data block is written to flash memory, then read back and verified.
If the operation is successful, the LED lights up or blinks in a specific pattern; if there is an error, the LED indicates this with a different pattern.
Use Cases
Data Storage: Storing persistent settings or user data.
Firmware Update: Writing new firmware code to flash memory.
Hardware Testing: Verifying the correct operation of Flash, GPIO, and other peripherals.
Summary
This project demonstrates safe and reliable data writing and reading to the flash memory of the PY32F002B1 microcontroller, while also showcasing basic input/output control (LED, button). It serves as a reference for embedded system development and microcontroller programming.
**********************************************************************************************************************
Here is a detailed explanation of what each function does:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
1. static void APP_FlashErase(void);
Purpose:
This function erases a specific region (usually one or more pages/sectors) of the microcontroller’s internal flash memory.

Details:

Before writing new data to flash memory, the target region must be erased, because flash memory cannot be overwritten directly.
The erase operation sets all bits in the specified region to ‘1’ (0xFF).
The function determines the address range to erase and triggers the hardware’s flash erase command.
No other operations should be performed during erasing, as it takes some time and is critical for system stability.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
2. static void APP_FlashProgram(void);
Purpose:
This function writes (programs) new data to a previously erased region of flash memory.

Details:

The target flash region must be erased before programming.
The function specifies the data block to write and the destination flash address.
It uses the hardware’s flash programming command to write the data.
Data is usually written in small blocks (such as 32-bit or 64-bit words).
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
3. static void APP_FlashBlank(void);
Purpose:
This function checks if a specific region of flash memory is completely blank (erased, all bits set to ‘1’).

Details:

Used to verify that the erase operation was successful.
The function reads the specified flash address range and checks if all values are 0xFF (or the hardware’s defined blank value).
If the region is fully blank, the erase was successful.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
4. static void APP_FlashVerify(void);
Purpose:
This function verifies that the data written to flash memory is correct.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Details:
After programming, the function reads back the flash region.
It compares the read data with the expected (written) data.
If all data matches, the programming is considered successful; otherwise, an error is detected.
This ensures data integrity in flash operations.
Summary:
These four functions are used to safely and reliably erase, program, check, and verify data in the microcontroller’s flash memory. They are essential for persistent data storage and firmware updates in embedded systems.
