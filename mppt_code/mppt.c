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


void readfromfile(const char* filename); //Reads in simulated values from csv file.
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
    FILE *fp = fopen(filename,"r");
    char buff[255];
    int index = 0;
    
    while(getc(fp) != EOF) {
        fgets(buff, 255, fp);
        char *token = strtok(buff, ",");
        int count = 0;

        while(token != NULL) {
            switch(count){
                case 0:
                arr_curr[index] = atof(token);
                break;

                case 1:
                arr_volt[index] = atof(token);
                break;

                default:
                break;
            }
            
            token = strtok(NULL, ",");
            count += 1;
        }
        index += 1;
    }

    fclose(fp);
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

