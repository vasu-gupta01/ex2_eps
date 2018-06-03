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


// #define RING_BUFFER_HEAP_SIZE 4096	// bytes

#define DEFAULT_BATT_V	3.7f


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

/* Deprecated...
// Allocates some memory for a ring buffer, initializing the given index and returning a pointer to the memory
// Use a small fixed amount of space to simulate limited microcontroller memory.
void *AllocateRingBuffer( struct CRingBufferIndex *pIndex, int nElementSize )
{
	static char pRingBufferData[RING_BUFFER_HEAP_SIZE];
	static char *pFreeRingBufferData = pRingBufferData;

	// Just as an example, allocate only up to half the remaining free data, meaning subsequent allocations are appox half the size of the last.
	int nUsedSpace = pFreeRingBufferData - pRingBufferData;
	int nAvailableSpace = (RING_BUFFER_HEAP_SIZE - nUsedSpace) >> 1;
	int nAllocatedEntries = nAvailableSpace / nElementSize;

	pIndex->nBufferSize = nAllocatedEntries;
	pIndex->nNextFreeIdx = 0;
	pIndex->nNextUnreadIdx = 0;

	void *pRet = pFreeRingBufferData;
	pFreeRingBufferData += nAllocatedEntries * nElementSize;
	DEBUG( printf( "AllocateRingBuffer: Allocated %d bytes for %d elements.\n", nAllocatedEntries * nElementSize, nAllocatedEntries ) );
	return pRet;
}
*/

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

