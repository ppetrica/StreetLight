#include <DAVE.h>


enum message_type {
	/**
	 * Time slot in which the bulb can turn on
	 * { TIME_SLOT, s_hour, s_minute, e_hour, e_minute }
	 * s_hour - 1 byte
	 * s_minute - 1 byte
	 * e_hour - 1 byte
	 * e_minute - 1 byte
	 */
	TIME_SLOT = 't',
	/**
	 * Light intensity in lumins
	 * { LUMINOSITY, l_value }
	 * l_value - 1 byte
	 */
	LUMINOSITY = 'l',
	/**
	 * Alive time.
	 * How much will the bulb keep itself on if no
	 * movement is detected in seconds
	 * { ALIVE_TIME, l_time }
	 * l_time = 1 byte
	 */
	ALIVE_TIME = 'a',
	/**
	 * GMT Time value to synchronize time slot with
	 * { GMT_TIME, seconds, minutes, hours, days, month, year }
	 * seconds - 1 byte
	 * minutes - 1 byte
	 * hours - 1 byte
	 * days - 1 byte
	 * month - 1 byte
	 * year - 2 bytes
	 */
	GMT_TIME = 'g',
	/**
	 * Mode
	 * { MODE, mode }
	 * mode - 0 -> The bulb only lights up in time slot interval
	 *        1 -> All of the above and luminosity under LUMINOSITY value
	 *        2 -> All of the above and motion is detected
	 *        3 -> The bulb will stay ON
	 *        4 -> The bulb will stay OFF (default)
	 */
	MODE = 'm',
};


enum mode {
	TIME_CTRL,
	LUMINOSITY_CTRL,
	MOTION_CTRL,
	ON,
	OFF
};


typedef struct time_slot {
	uint8_t s_hour;
	uint8_t s_minute;
	uint8_t e_hour;
	uint8_t e_minute;
} time_slot_t;


time_slot_t time_slot = {17, 00, 05, 00};

uint8_t luminosity = 0;

uint8_t alive_time = 15;

XMC_RTC_TIME_t gmt_time;

uint8_t mode = 4;


uint8_t message[64];


int main(void) {
	DAVE_STATUS_t status;

	status = DAVE_Init(); /* Initialization of DAVE APPs */

	if (status == DAVE_STATUS_FAILURE) {
		XMC_DEBUG(("DAVE Apps initialization failed with status %d\n", status));
		while (1U) {
		}
	}

	XMC_RTC_TIME_t current_time;

	UART_Receive(&BLT, message, 1); //Transmit the string "Infineon Technologies".

	while (1U) {
		RTC_GetTime(&current_time);
		BUS_IO_Write(&LEDS, ~(1 << mode));

		switch (mode) {
		case TIME_CTRL:
			break;
		case LUMINOSITY_CTRL:
			break;
		case MOTION_CTRL:
			break;
		case ON:
			DIGITAL_IO_SetOutputLow(&LBULB);
			break;
		case OFF:
		default:
			DIGITAL_IO_SetOutputHigh(&LBULB);
			break;
		}
	}

	return 1;
}

void EndofTransmit() {
	UART_Receive(&BLT, message, 1);
}

uint8_t A = 'A';
uint8_t E = 'E';

uint8_t message_type = 0;

void EndofReceive() //Callback function for "End of receive" event.
{
	if (message_type == 0) {
		message_type = message[0];
		int message_length;
		switch (message_type) {
		case TIME_SLOT:
			message_length = sizeof(time_slot_t);
			break;
		case LUMINOSITY:
			message_length = 1;
			break;
		case ALIVE_TIME:
			message_length = 1;
			break;
		case GMT_TIME:
			message_length = 7;
			break;
		case MODE:
			message_length = 1;
			break;
		default:
			UART_Transmit(&BLT, &E, 1);
			message_type = 0;
			return;
		}

		UART_Receive(&BLT, message + 1, message_length);
	} else {
		switch (message_type) {
		case TIME_SLOT:
			time_slot.s_hour = message[1];
			time_slot.s_minute = message[2];
			time_slot.e_hour = message[3];
			time_slot.e_minute = message[4];
			break;
		case LUMINOSITY:
			luminosity = message[1];
			break;
		case ALIVE_TIME:
			alive_time = message[1];
			break;
		case GMT_TIME:
			gmt_time.seconds = message[1];
			gmt_time.minutes = message[2];
			gmt_time.hours = message[3];
			gmt_time.days = message[4];
			gmt_time.month = message[5];
			gmt_time.year = (message[6] << 8) | message[7];

			RTC_SetTime(&gmt_time);
			break;
		case MODE:
			mode = message[1];
			break;
		}

		message_type = 0;
		UART_Transmit(&BLT, &A, 1);
	}
}

