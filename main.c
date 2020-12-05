#include <DAVE.h>

uint8_t message[256];

int main(void) {
	DAVE_STATUS_t status;

	status = DAVE_Init(); /* Initialization of DAVE APPs */

	if (status == DAVE_STATUS_FAILURE) {
		XMC_DEBUG(("DAVE Apps initialization failed with status %d\n", status));
		while (1U) {
		}
	}

	UART_Receive(&UART_0, message, 1); //Transmit the string "Infineon Technologies".

	while (1U) {
	}

	return 1;
}

void EndofTransmit() {
	UART_Receive(&UART_0, message, 1);
}

uint8_t A = 'A';
uint8_t E = 'E';

uint8_t message_type;

void EndofReceive() //Callback function for "End of receive" event.
{
	if (message_type == 0) {
		message_type = message[0];
		int message_length;
		switch (message_type) {
		default:
			UART_Transmit(&UART_0, &E, 1);
			message_type = 0;
			return;
		}

		UART_Receive(&UART_0, message + 1, message_length);
	} else {
		switch (message_type) {
		}

		message_type = 0;
		UART_Transmit(&UART_0, &A, 1);
	}
}

