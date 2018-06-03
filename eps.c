/*
	eps.c	- Base code for the Electrical Power System module.

	AlbertaSat Project
	Michael Devine, 2018
*/


#include <stdio.h>
#include <stdarg.h>

#include "datasheet.h"
#include "hw.h"


/*
// Enable debugging
#define DEBUG( f )	f
#define DEBUGPR( ... )  printf( "====eps: " __VA_ARGS__ )
*/

// Ignore debugging
#define DEBUG( f )
#define DEBUGPR( ... )

#define STUB	// Placeholder for incomplete functionality


DEBUG( test );

//#define IDLE	STUB
//#define IDLE	STUB sleep( 1 );


#define MODULE_COMMS	1
#define MODULE_OBC	2
#define MODULE_PAYLOAD	3




// Global data
float ConfigVariables[VAR_MAX];

struct {
	TIME_t nFrameUptime;	// Uptime of the cpu, as recorded for the current frame of the main loop.
} cpu;






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


float GetVar( enum EVar nVarIdx )
{
}

// EPS functions ---------------------------------------------------------------------------


//	Logs a test message
#define MAX_MESSAGE_SIZE 256
void Log( char *sMessage, ... )
{
	char sOut[MAX_MESSAGE_SIZE];	// Must use carefully to avoid overrun
	va_list argptr;

	va_start( argptr, sMessage );
	vsprintf( sOut, sMessage, argptr );
	va_end( argptr );

	// Test functionality, just print it
	STUB
	printf( "%s\n", sOut );
}

//	Stores logging info on the occurrence of an event, for later retrieval.
void LogEvent( enum EEvent nEventID )
{
	int nIdx = GetNextFreeIdx( &(g_logs.cEventLogsIndex) );
	struct CEventLog *pLog = &(g_pEventLog[nIdx]);

	pLog->nEventTime = GetUptime( );
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
	TIME_t nLastFrameUptime = cpu.nFrameUptime;
	cpu.nFrameUptime = GetUptime( );

	if (cpu.nFrameUptime < nLastFrameUptime)
	{
		LogEvent( EVENT_CLOCK_OVERFLOW );	// Eg. if 32 bit times in milliseconds are used, times will overflow in 49.71 days.
		// Subject to design specs, time overflows may be simply ignored and ground control can keep track of overflows and adjust accordingly.
	}
	return cpu.nFrameUptime - nLastFrameUptime;	// CPU should make this a sane value even in case of overflow.
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

	Log( "BatteryV: %f", fVolts );
}

void HandleBoot( )
{
	// Mark beginning of uptime
	GetFrameTime( );

	// remove... trying to avoid allocating memory; prefer having it static and available right at the start
	//logs.pEventLogs = AllocateRingBuffer( &(logs.cEventLogsIndex), sizeof( struct CEventLog ) );
	//logs.pDataLogs = AllocateRingBuffer( &(logs.cSensorLogsIndex), sizeof( struct CSensorLog ) );

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

	TIME_t nTimeSinceLastLog = 0;

	// Idle loop
	while( 1 ) {
		ProcessCommands( );

		CheckPowerSystem( );

		UpdateBattery( );


		TIME_t nFrameTime = GetFrameTime( );

		// Do intermittent housekeeping logging
		nTimeSinceLastLog += nFrameTime;
		if (nTimeSinceLastLog >= GetVar( VAR_SENSORLOG_INTERVAL ))
		{
			// nTimeSinceLastLog -= GetVar( VAR_SENSORLOG_INTERVAL ); // Requires signed TIME_t! Do this for consistent average number of logs per time period.
			nTimeSinceLastLog = 0;

			LogSensors( );
		}

		Idle( );
	}

}
