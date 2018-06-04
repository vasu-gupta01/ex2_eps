/*
	hw_sim.c	- Minimal simulation of EPS hardware, for development and unit tests.

	Michael Devine
	AlbertaSat Project
*/

#include "hw.h"

#include <stdio.h>

#define DEBUG( f )    f       // Enable debugging, do f
#define DEBUGPR( ... )	printf( "====hw: " __VA_ARGS__ )
//#define DEBUG( f )              // Ignore debugging code f



#define DEFAULT_BATT_V	3.7f	// Fake battery voltage


// Allocate space for logs
#define EVENTLOG_MAX	64
#define SENSORLOG_MAX	256
struct CEventLog g_pEventLog[EVENTLOG_MAX];
struct CSensorLog g_pSensorLog[SENSORLOG_MAX];
struct CLogs g_logs =
{
	{ EVENTLOG_MAX, 0, 0 },
	{ SENSORLOG_MAX, 0, 0 },
};


TIME_t GetUptime( )
{
	static s_nUptime = 0;

	// Just fake it completely for now, a varying (though repetetive) increase in time...
	s_nUptime += ((s_nUptime & 0x02) >> 1) + 1;
	return s_nUptime;
}

struct
{
	float	fBattV;
} Sensors;

void InitHW( )
{
	Sensors.fBattV = DEFAULT_BATT_V;
}

float ReadBattV( )
{
	DEBUGPR( "Reading battery at time %d\n", GetUptime( ) );
	return Sensors.fBattV;
}


void Idle( )
{
#ifndef ARDUINO
	sleep( 1 );
#endif
}

