#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Waveform.h"
#include "io.h"

double rms_voltage(double *sample){
    double temp = 0; //temporary variable to store values

    //loop to access all 1000 elements of each phase
    for (int i = 0; i < 1000; i++) {
        temp = temp + pow(*(sample + (8*i)), 2);
        //adding 8 to access the next value of the phase voltage
    }

    return sqrt(temp/1000);
}

void analysis(double *sample, double *p2p, double *mean){

    double high = 0, low = 0, temp_mean = 0;
    double tempH = (*sample);
    double tempL = (*sample);

    fabs(tempH) >= 324.9 ? printf("\nClipping Detected at 1st Value\n") : (void)0;

    for (int i = 0; i < 1000; i++) {

       temp_mean += *(sample + (8*i));

       if (fabs(*(sample + (8*i))) >= 324.9 ) {
           double cuttoff_value = *(sample + (8*i));
           printf("Clipping Detected at value %d\nThe value is %.16lf\n\n", (i + 2), cuttoff_value);
       }

       if (*(sample + (8*i)) > tempH) tempH = *(sample + (8*i));
       if (*(sample + (8*i)) < tempL) tempL = *(sample + (8*i));
   }
    high = tempH;
    low = tempL;

    *p2p = high - low;
    *mean = temp_mean/1000;
}

void sort(EightStruct *WaveformSample, char phase) {

    double tempsort;

    for (int i = 1; i < 1000; i++) {
        //checking user input of which phase
        if (phase == 'A')          tempsort = WaveformSample[i].phase_A_voltage;

        else if (phase == 'B')     tempsort = WaveformSample[i].phase_B_voltage;

        else if (phase == 'C')     tempsort = WaveformSample[i].phase_C_voltage;

        else {
            printf("***Invalid Phase***\n");
            return;
        }


            int x = i - 1; //to access and compare element to the left

        if (phase == 'A') {
            while (x >= 0 && (fabs(WaveformSample[x].phase_A_voltage) > fabs(tempsort))) {
                WaveformSample[x + 1].phase_A_voltage = WaveformSample[x].phase_A_voltage;
                x--;
                //stores the value we are comparing in tempsort
                //if the value to the left is less than the value we are comparing...
                //...it moves the value to the left to the right
                //checks condition again, if not bigger it inserts the tempvalue where the value...
                //to the left sat originally
            }
            WaveformSample[x + 1].phase_A_voltage = tempsort;
        }
        else if (phase == 'B') {
            while (x >= 0 && (fabs(WaveformSample[x].phase_B_voltage) > fabs(tempsort))) {
                WaveformSample[x + 1].phase_B_voltage = WaveformSample[x].phase_B_voltage;
                x--;
            }
            WaveformSample[x + 1].phase_B_voltage = tempsort;
        }
        else if (phase == 'C') {
            while (x >= 0 && (fabs(WaveformSample[x].phase_C_voltage) > fabs(tempsort))) {
                WaveformSample[x + 1].phase_C_voltage = WaveformSample[x].phase_C_voltage;
                x--;
            }
            WaveformSample[x + 1].phase_C_voltage = tempsort;
        }
        else return;

        }

}