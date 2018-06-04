/*
	datasheet.h	- Contains any EPS data specifications for use external to the EPS.

	These data are for the ExAlta2 mission.

	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						WARNING!
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	This is currently in a prototyping phase and all of the values herein are temporary.
	They should not be recorded anywhere that is not equally temporary.
	These are currently merely example data.
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	AlbertaSat Project
*/
#ifndef DATASHEET_H
#define DATASHEET_H



// Any items that do not have a specified value should be considered undefined.





enum ECommand	// These are given by ground control and are from ground control's perspective.
{
	CMD_NOP = 0,	// No operation, ignore.

	CMD_READ_VARIABLE = 1,
	CMD_WRITE_VARIABLE = 2,

	CMD_REQUEST_LOGS,
};

// Variables and their default values
enum EVar
{
	VAR_SENSORLOG_INTERVAL = 0,	// time in ms between routine logging
#define DEF_SENSORLOG_INTERVAL		30000

	VAR_MIN_BATT_TEMP,		// temp in deg C at or below which battery cells should start being heated
#define DEF_MIN_BATT_TEMP		-3.0
	VAR_MAX_BATT_TEMP,		// temp in deg C at or above which battery cells should stop being heated
#define DEF_MAX_BATT_TEMP		0.0

	VAR_MIN_BATT_V,			// voltage in V at or below which battery cells should start being charged
#define DEF_MIN_BATT_V			(3.6*4)
	VAR_MAX_BATT_V,			// voltage in V at or above which battery cells should stop being charged
#define DEF_MAX_BATT_V			(3.9*4)

VAR_MAX
};


enum EEvent
{
	EVENT_BOOT = 0,

	EVENT_CLOCK_OVERFLOW,


	// Error events
	ERROR_BAD_VARIDX,
};


#endif // ndef DATASHEET_H
