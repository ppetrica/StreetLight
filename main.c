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
	/**
	 * Parameters - Retrieve the current settings of the microcontroller
	 * { PARAMETERS }
	 *
	 * Response:
	 * { mode, time_slot, luminosity, alive_time }
	 * MODE - 1 byte
	 * time_slot - { s_hour, s_minute, e_hour, e_minute } - 4 bytes
	 * luminosity - 1 byte
	 * alive_time - 1 byte
	 */
	PARAMETERS = 'p'
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
uint8_t last_mode = 4;

uint8_t luminosity_measured;

uint8_t message[64];


int in_time_slot(const XMC_RTC_TIME_t *t) {
	uint8_t hour = t->hours;
	uint8_t minute = t->minutes;

	int16_t minutes = hour * 60 + minute;

	int16_t s_minutes = time_slot.s_hour * 60 + time_slot.s_minute;

	int16_t e_minutes = time_slot.e_hour * 60 + time_slot.e_minute;

	return (s_minutes < e_minutes)
			? (minutes - s_minutes > 0 && minutes - e_minutes < 0)
			: (minutes > s_minutes || minutes < e_minutes);
}


#define turn_off(bulb) DIGITAL_IO_SetOutputLow(&bulb)

#define turn_on(bulb) DIGITAL_IO_SetOutputHigh(&bulb)


enum mode1_state {
	MODE1_INITIAL = 0,
	MODE1_MAIN
};


int main(void) {
	DAVE_STATUS_t status;

	status = DAVE_Init(); /* Initialization of DAVE APPs */

	if (status == DAVE_STATUS_FAILURE) {
		XMC_DEBUG(("DAVE Apps initialization failed with status %d\n", status));
		while (1U) {
		}
	}

	XMC_RTC_TIME_t current_time;

	UART_Receive(&BLT, message, 1);

	int state = 0;
	uint8_t last_motion = 0;

	BUS_IO_Write(&LEDS, ~(1 << mode));
	turn_off(LBULB);

	while (1U) {
		RTC_GetTime(&current_time);

		if (last_mode != mode) {
			switch(mode) {
			case ON:
				turn_on(LBULB);
				break;
			case OFF:
				turn_off(LBULB);
				break;
			}
			state = 0;
			if (last_mode == LUMINOSITY_CTRL) {
				TIMER_Stop(&TIMER_ADC_SAMPLE);
			}
			last_mode = mode;
			BUS_IO_Write(&LEDS, ~(1 << mode));
		}

		switch (mode) {
		case TIME_CTRL:
			if (in_time_slot(&current_time)) {
				turn_on(LBULB);
			} else {
				turn_off(LBULB);
			}
			break;
		case LUMINOSITY_CTRL:
			switch (state) {
			case MODE1_INITIAL:
			    TIMER_Start(&TIMER_ADC_SAMPLE);
				ADC_MEASUREMENT_StartConversion(&ADC_LUMINOSITY);
				state = MODE1_MAIN;
				break;
			case MODE1_MAIN:
				if (in_time_slot(&current_time) && luminosity_measured < luminosity) {
					turn_on(LBULB);
				} else {
					turn_off(LBULB);
				}
				break;
			}
			break;
		case MOTION_CTRL:;
			uint8_t motion = DIGITAL_IO_GetInput(&MOTION_INPUT);
			if (last_motion == 0 && motion == 1) {
				turn_on(LBULB);
				TIMER_Stop(&TIMER_ALIVE);
			}
			else if (last_motion == 1 && motion == 0) {
				TIMER_Start(&TIMER_ALIVE);
			}
			last_motion = motion;
			if (motion == 0)
				DIGITAL_IO_SetOutputLow(&LED_MOTION);
			else
				DIGITAL_IO_SetOutputHigh(&LED_MOTION);
			break;
		}
	}

	return 1;
}


/** BLUETOOTH Communication */

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
		case PARAMETERS:
			message[0] = mode;
			message[1] = time_slot.s_hour;
			message[2] = time_slot.s_minute;
			message[3] = time_slot.e_hour;
			message[4] = time_slot.e_minute;
			message[5] = luminosity;
			message[6] = alive_time;
			UART_Transmit(&BLT, message, 7);
			message_type = 0;
			return;
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
			uint32_t useconds = (alive_time - 3) * 1000000;
			TIMER_SetTimeInterval(&TIMER_ALIVE, useconds);
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
			last_mode = mode;
			mode = message[1];
			break;
		}

		message_type = 0;
		UART_Transmit(&BLT, &A, 1);
	}
}


/** ADC luminosity */

void EndofMeasure() {
	// ADC uses 8 bits for conversion => 0 <= result <= 255
	// 0V    ... 0
	// 3.3V  ... 255
	// 2.1V  ... 163

	// 0V <= solar panel output <= 2.1V --> 0 <= lightLevel <=  163
	luminosity_measured = (uint16_t)ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_A) * 100 / 163;
}

/** ADC Timer */

void TimerADCSampleInterrupt() {
	TIMER_ClearEvent(&TIMER_ADC_SAMPLE);
	DIGITAL_IO_ToggleOutput(&TIMER_ADC_LED);
	ADC_MEASUREMENT_StartConversion(&ADC_LUMINOSITY);
}

/** Alive Timer **/

void TimerAliveExpire() {
	TIMER_ClearEvent(&TIMER_ALIVE);
	TIMER_Stop(&TIMER_ALIVE);
	turn_off(LBULB);
}
