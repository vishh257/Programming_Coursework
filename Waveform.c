#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "Waveform.h"
#include "io.h"

#define RMS_OUT_OF_TOLERANCE            (1 << 0)
#define P2P_NOT_NORMAL                  (1 << 1)
#define ABNORMAL_DC_OFFSET              (1 << 2)
#define CLIPPING_DETECTED               (1 << 3)
#define ABNORMAL_FREQUENCY_RANGE        (1 << 4)
#define ABNORMAL_POWERFACTOR_RANGE      (1 << 5)
#define ABNORMAL_THD_RANGE              (1 << 6)


double rms_voltage(double *sample, metrics *output, int phaseno){
    double temp = 0; //temporary variable to store values

    //loop to access all 1000 elements of each phase
    for (int i = 0; i < output->rows; i++) {
        temp = temp + pow(*(sample + (8*i)), 2);
        //adding 8 to access the next value of the phase voltage
    }

    if ((sqrt(temp/output->rows)  <= 207 || sqrt(temp/output->rows)  >= 253)) {
        (output->tolerance)++;

        phaseno == 1 ? output->phase[0].status |= RMS_OUT_OF_TOLERANCE : (void)0;
        phaseno == 2 ? output->phase[1].status |= RMS_OUT_OF_TOLERANCE : (void)0;
        phaseno == 3 ? output->phase[2].status |= RMS_OUT_OF_TOLERANCE : (void)0;
    }

    return sqrt(temp/output->rows);
}

void analysis(double *sample, metrics *output, int phaseno){

    double high = 0, low = 0, temp_mean = 0;
    double tempH = (*sample);
    double tempL = (*sample);

    /*if (fabs(tempH) >= 324.9) {
        printf("\nClipping Detected at 1st Value\n");

        phaseno == 1 ? output->phase[0].status |= RMS_OUT_OF_TOLERANCE : (void)0;
        phaseno == 2 ? output->phase[1].status |= RMS_OUT_OF_TOLERANCE : (void)0;
        phaseno == 3 ? output->phase[2].status |= RMS_OUT_OF_TOLERANCE : (void)0;
    }*/

    for (int i = 0; i < output->rows; i++) {

       temp_mean += *(sample + (8*i));

       if (fabs(*(sample + (8*i))) >= 324.9 ) {
           phaseno == 1 ? output->phase[0].status |= CLIPPING_DETECTED : (void)0;
           phaseno == 2 ? output->phase[1].status |= CLIPPING_DETECTED : (void)0;
           phaseno == 3 ? output->phase[2].status |= CLIPPING_DETECTED : (void)0;

           double cuttoff_value = *(sample + (8*i));
           printf("Clipping Detected at value %d\nThe value is %.16lf\n\n", (i + 2), cuttoff_value);
           (output->clipping)++;
       }

       if (*(sample + (8*i)) > tempH) tempH = *(sample + (8*i));
       if (*(sample + (8*i)) < tempL) tempL = *(sample + (8*i));
   }
    high = tempH;
    low = tempL;

    if (phaseno == 1) {
        output->phase[0].p2p = high - low;
        (output->phase[0].p2p <=650 && output->phase[0].p2p >= 649.5) ? (void)0 : (output->phase[0].status |= P2P_NOT_NORMAL);

        output->phase[0].mean = temp_mean/output->rows;
        (output->phase[0].mean < 1e-4) ? (void)0 : (output->phase[0].status |= ABNORMAL_DC_OFFSET);
    }
    if (phaseno == 2) {
        output->phase[1].p2p = high - low;
        (output->phase[1].p2p <=650 && output->phase[1].p2p >= 649.5) ? (void)0 : (output->phase[1].status |= P2P_NOT_NORMAL);

        output->phase[1].mean = temp_mean/output->rows;
        (output->phase[1].mean < 1e-4) ? (void)0 : (output->phase[1].status |= ABNORMAL_DC_OFFSET);
    }
    if (phaseno == 3) {
        output->phase[2].p2p = high - low;
        (output->phase[2].p2p <=650 && output->phase[2].p2p >= 649.5) ? (void)0 : (output->phase[2].status |= P2P_NOT_NORMAL);

        output->phase[2].mean = temp_mean/output->rows;
        (output->phase[2].mean < 1e-4) ? (void)0 : (output->phase[2].status |= ABNORMAL_DC_OFFSET);
    }
}

void variance(double *sample, double mean, double *variance, double *std_deviation, int rows) {

    double temp = 0;

    for (int i = 0; i < rows; i++) {
        temp = temp + pow(*(sample + (8*i)), 2);
        //adding 8 to access the next value of the phase voltage
    }

    temp = temp/rows;

    *variance = temp - pow((mean), 2);
    *std_deviation =  sqrt(temp - pow((mean), 2));
}

double range(double *sample, int rows){

    double high = 0, low = 0, temp_mean = 0;
    double tempH = (*sample);
    double tempL = (*sample);

    for (int i = 0; i < rows; i++) {

        temp_mean += *(sample + (8*i));

        if (*(sample + (8*i)) > tempH) tempH = *(sample + (8*i));
        if (*(sample + (8*i)) < tempL) tempL = *(sample + (8*i));
    }
    high = tempH;
    low = tempL;

    return high - low;
}

void bitcheck(metrics *output) {

    if (output->frequency > 0.048) {
        for (int i = 0; i < 3; i++) {
            output->phase[i].status |= ABNORMAL_FREQUENCY_RANGE;
        }
    }

    if (output->power_factor > 0.012) {
        for (int i = 0; i < 3; i++) {
            output->phase[i].status |= ABNORMAL_POWERFACTOR_RANGE;
        }
    }

    if (output->thd_percent > 0.18) {
        for (int i = 0; i < 3; i++) {
            output->phase[i].status |= ABNORMAL_THD_RANGE;
        }
    }
}

void sort(EightStruct *WaveformSample, char phase, int rows) {

    double tempsort;

    for (int i = 1; i < rows; i++) {
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