#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MPPT_STEP       1
#define MAX_ARRAY_SIZE  45
#define MAX_BUFFER_SIZE 1024

//GLOBALS
float current, voltage;
static float power0 = 0;
float voltin;
int step_up = 0;

//SIMULATED VALUES
float arr_curr[MAX_ARRAY_SIZE], arr_volt[MAX_ARRAY_SIZE];

void readfromfile(const char* filename);
void po();

int main() {


    readfromfile("mppt_data.csv");

    for(int i = 1; i < 100; i++) {
        current = arr_curr[i];
        voltage = arr_volt[i];

        po(i);

        printf("%f -- %f\n", power0, voltin);


    }
/*
    for(int i = 0; i < MAX_ARRAY_SIZE; i++) {
        printf("%f\n", arr_curr[i]);
    }

    for(int i = 0; i < MAX_ARRAY_SIZE; i++) {
        printf("%f\n", arr_volt[i]);
    }*/

    return 0;
}

void readfromfile(const char* filename) {
    FILE *filePtr;

    if((filePtr = fopen(filename, "r")) == NULL) {
        printf("Could not open file!\n");

        exit(1);
    }
    char buffer[MAX_ARRAY_SIZE+1];
    float curr, volt;
    int index = 0;
    char curr_dest[MAX_ARRAY_SIZE+1];
    char volt_sub[MAX_ARRAY_SIZE+1];
    char volt_dest[MAX_ARRAY_SIZE+1];


    while(!feof(filePtr)) {
        fgets(buffer, MAX_ARRAY_SIZE, filePtr);

        strncpy(curr_dest, buffer, 4);        
        arr_curr[index] = atof(curr_dest);

        int length = strlen(buffer) - 5;
        if(length >= 0) {
            strncpy(volt_sub, buffer+5, length);
            
            volt_dest[length] = '\0';
            strncpy(volt_dest, volt_dest, 4);
        }
        strncpy(volt_dest, volt_sub, 4);
        arr_volt[index] = atof(volt_dest);

        index += 1;
    }

    fclose(filePtr);

}

void readCurrent() {

}

void readVoltage() {

}

void po(int index) {

    float power = current * voltage;

    if(power == power0) {
        //step_up ^= 1;
    } else {
        if(power > power0) {
            if(voltage < arr_volt[index-1]) {
                voltin += MPPT_STEP;
            } else {
                voltin -= MPPT_STEP;
            }
        } else {
            if(voltage > arr_volt[index-1]) {
                voltin += MPPT_STEP;
            } else {
                voltin -= MPPT_STEP;
            }
        }
    }
/*
    if(step_up) {
        voltin += MPPT_STEP;
    } else {
        voltin -= MPPT_STEP;
    }*/

    power0 = power;

}

