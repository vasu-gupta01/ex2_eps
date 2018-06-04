/*
	eps.c	- Base code for the Electrical Power System module.

	AlbertaSat Project
	Michael Devine, 2018
*/


#include <stdio.h>
#include <stdarg.h>

#include "datasheet.h"
#include "hw.h"

#ifdef MONOLITHIC
	// Monolithic source file... Combine all the source into one so that gcc can optimize across the HAL.
#	include "hw_sim.c"
#	warning "hw_sim configuration is selected... is that what you wanted?"

#else
	/*
	// Enable debugging
#	define DEBUG( f )	f
#	define DEBUGPR( ... )  printf( "====eps: " __VA_ARGS__ )
	*/

	// Ignore debugging
#	define DEBUG( f )
#	define DEBUGPR( ... )

#endif // MONOLITHIC



#define STUB	// Placeholder for incomplete functionality


#define MODULE_COMMS	1
#define MODULE_OBC	2
#define MODULE_PAYLOAD	3




// Global data
float configVariables[VAR_MAX];

struct {
	TIME_t tFrameUptime;	// Uptime of the cpu, as recorded for the current frame of the main loop.
} cpu;





void LogEvent( enum EEvent nEventID );




// Util functions -------------------------------------------------------------------------

// Gets the next free spot to use in the ring buffer, advancing the indices appropriately.
int GetNextFreeIdx( struct CRingBufferIndex *pIndex )
{
	int nNextFree = pIndex->nNextFreeIdx;
	pIndex->nNextFreeIdx = (nNextFree + 1) % pIndex->nBufferSize;
	if (pIndex->nNextUnreadIdx == nNextFree)
	{
		// This unread one will be overwritten, so advance unread index too.
		pIndex->nNextUnreadIdx = pIndex->nNextFreeIdx;
	}

	return nNextFree;
}


// Returns the indexed stored data.
float GetVar( enum EVar nVarIdx )
{
	if (nVarIdx < 0  ||  nVarIdx >= VAR_MAX)
	{
		LogEvent( ERROR_BAD_VARIDX );
		return 0.0f;
	}
	return configVariables[nVarIdx];
}

// EPS functions ---------------------------------------------------------------------------


//	Stores logging info on the occurrence of an event, for later retrieval.
void LogEvent( enum EEvent nEventID )
{
	int nIdx = GetNextFreeIdx( &(g_logs.cEventLogsIndex) );
	struct CEventLog *pLog = &(g_pEventLog[nIdx]);

	pLog->tEventTime = GetUptime( );
	pLog->nEventID = nEventID;
	pLog->nData = 0;	STUB // Nothing needs data yet...
}

void LogSensors( )
{
	STUB
}

//	Returns the length of time since the last time this was called, in units defined by TIME_t
TIME_t GetFrameTime( )
{
	TIME_t tLastFrameUptime = cpu.tFrameUptime;
	cpu.tFrameUptime = GetUptime( );

	if (cpu.tFrameUptime < tLastFrameUptime)
	{
		LogEvent( EVENT_CLOCK_OVERFLOW );	// Eg. if 32 bit times in milliseconds are used, times will overflow in 49.71 days.
		// Subject to design specs, time overflows may be simply ignored and ground control can keep track of overflows and adjust accordingly.
	}
	return cpu.tFrameUptime - tLastFrameUptime;	// CPU should make this a sane value even in case of overflow.
}


/*	Do a system check. Takes whatever pre-programmed actions when necessary.
	Logs status when applicable.
*/
void CheckPowerSystem( )
{
	STUB
}

// Powers up and initializes EPS settings for an external hardware module.
void StartMod( int nModuleID )
{
	STUB
}

void StartExternalModules( )
{
	// ??? Will the order be configurable? I think yes.
	StartMod( MODULE_COMMS );
	StartMod( MODULE_OBC );
	StartMod( MODULE_PAYLOAD );
}


void ProcessCommands( )
{
	STUB
}

void UpdateBattery( )
{
	float fVolts = ReadBattV( );

	DEBUGPR( "read %f volts\n", fVolts );

	// deprecated... Log( "BatteryV: %f", fVolts );
}

void HandleBoot( )
{
	// Mark beginning of uptime
	GetFrameTime( );

	// Do absolute critical hardware initialization first
	InitHW( );
	CheckPowerSystem( );

	LogEvent( EVENT_BOOT );

	// At this point EPS is expected to know which modules are to be brought online, and in which order.
	StartExternalModules( );
}



#ifdef ARDUINO
void setup( )	// Arduino doesn't accept a function called "main" that doesn't return int
#else
void main( )
#endif
{
	// Initial entry point when the power module (and typically entire satellite) is booted.
	HandleBoot( );

	TIME_t tTimeSinceLastLog = 0;

	// Idle loop
	while( 1 ) {
		ProcessCommands( );

		CheckPowerSystem( );

		UpdateBattery( );


		TIME_t tFrameTime = GetFrameTime( );

		// Do intermittent housekeeping logging
		tTimeSinceLastLog += tFrameTime;
		if (tTimeSinceLastLog >= GetVar( VAR_SENSORLOG_INTERVAL ))
		{
			// tTimeSinceLastLog -= GetVar( VAR_SENSORLOG_INTERVAL ); // Requires signed TIME_t! Do this for consistent average number of logs per time period.
			tTimeSinceLastLog = 0;

			LogSensors( );
		}

		Idle( );
	}

}
