/*
	hw.h	- Abstract definition of EPS hardware components and sensors.
	Different hardware configurations (including simulations and mock functions for unit testing) each have separate implementations of these functions.

	Michael Devine
	AlbertaSat Project
*/


#include <inttypes.h>

#include "datasheet.h"


typedef uint32_t TIME_t;	// Time in milliseconds, maximum 49.71 days.


// Log memory is made hardware-dependent to handle differing memory sizes.
struct CRingBufferIndex {
        int nBufferSize;
        int nNextFreeIdx;
        int nNextUnreadIdx;
};
// Allocates some memory for a ring buffer, initializing the given index and returning a pointer to the memory
// Not ideal... void *AllocateRingBuffer( struct CRingBufferIndex *pIndex, int nElementSize );
struct CEventLog {
        TIME_t          nEventTime;
        enum EEvent     nEventID;
        int             nData;
};
struct CSensorLog {
        TIME_t          nTime;
        float           fBattV;
        float           fBattTemp;
        float           fAmperage;
        float           fIntervalWatts;
};
struct CLogs {
        struct CRingBufferIndex cEventLogsIndex;
        //struct CEventLog *pEventLogs;

        struct CRingBufferIndex cSensorLogsIndex;
        //struct CSensorLog *pDataLogs;
};
extern struct CLogs g_logs;
extern struct CEventLog g_pEventLog[];
extern struct CSensorLog pSensorLog[];

// Initializes any hardware. Expected to be called before any components are used or measured.
void InitHW( );


// ----	CPU

// Get time since boot, in units defined by TIME_t
TIME_t GetUptime( );

// Do nothing for a short time
void Idle( );

// ---- Hardware


// ---- Sensors

// Returns voltage (in volts) of an onboard battery.
float ReadBattV( );
