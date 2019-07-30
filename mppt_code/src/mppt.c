
#define RUN
//#undef RUN /* -- comment this to run simulation */
#define MPPT_STEP 1
#define MPPT_AVERAGE 20 // number of samples averaged
#define MPPT_INTERVAL 1	// TODO
#define SECOND_COUNT 2	// TODO

#define MPPT_PO			// run perturb and observe algorithm, if defined

#include <stdio.h>

// globals ------------------------------------------------------------------------
long vinref;	// TODO set the current operating voltage for the panel
long curr, volt, power;
// declarations -------------------------------------------------------------------

void init(){

}

void read_current(){

}

void read_voltage(){

}

void po(){
	/*
		Perturb and observe algorithm
		- curr and volt refer to the streamed current and voltage readings.
		- power0 is a static power value calculated to measure power difference
		since the last input stream.
		- assumes curr and volt are assigned values
	*/

	static long power0 = 0;
	long power = curr * volt;
	int updown = 0;

	if (power < power0)
		updown ^= 1;
	
	if (!updown)
		vinref -= MPPT_STEP;
	else
		vinref += MPPT_STEP;

	power0 = power;
}

void inc_cond(){
	long di, dv, dp;
	static long power0 = 0;
	static long curr0 =  0;
	static long volt0 = 0;

	power = curr * volt;
	dp = power - power0;
	di = curr - curr0;
	dv = volt - volt0;

	if (dv){
		if (dp / dv > 0){
			vinref += MPPT_STEP;
		}
		else{
			// TODO what if there is no change in power?
			vinref -= MPPT_STEP;
		}
	}
	else{
		if (di > 0){
			vinref += MPPT_STEP;
		}
		else{
			// TODO what if there is no change in current?
			vinref -= MPPT_STEP;
		}
	}

	power0 = power;
	curr0 = curr;
	volt0 = volt;
}

// main ---------------------------------------------------------------------------
void main(){
	printf("Running MPPT...\n");
	init();

	#ifdef RUN

	int butcnt;
	int track = 0;
	int mppt_calc;
	int second;
	int battery_state;	// TODO define an FSM for the battery

	while (1){
		// read values ---
		read_current();
		read_voltage();

		if (!track){
			// TODO check battery faultage
		}
		else{
			if (mppt_calc < MPPT_AVERAGE){
				// TODO doubts here
			}
			if (!mppt_calc){
				mppt_calc = MPPT_INTERVAL;

				#ifdef MPPT_PO
					po();
				#endif

				#ifdef MPPT_INCCOND
					inc_cond();
				#endif
			}
		}
		if (!second){
			second = SECOND_COUNT;
			if (!track){
				// TODO battery state machine
			}
		}
	}
	printf("Terminating MPPT...\n");
	#endif
}
