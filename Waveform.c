#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Waveform.h"
#include "io.h"

void rms_voltage(EightStruct *WaveformSample) {
    //creating pointers for each rms voltage, so they are not cleared after the function ends
    double *rms_phase_A = calloc(1000, sizeof(double));
    double *rms_phase_B = calloc(1000, sizeof(double));
    double *rms_phase_C = calloc(1000, sizeof(double));

    //temp values for calculating rms
    double temp = 0;
    double temp2 = 0;
    double temp3 = 0;

    for (int i = 0; i < 1000; i++) {
        temp = temp + pow(WaveformSample[i].phase_A_voltage,2);
        temp2 = temp2 + pow(WaveformSample[i].phase_B_voltage,2);
        temp3 = temp3 + pow(WaveformSample[i].phase_C_voltage,2);
    }

    //calculates rms voltage
    *rms_phase_A = sqrt(temp/1000);
    *rms_phase_B = sqrt(temp2/1000);
    *rms_phase_C = sqrt(temp3/1000);

    printf("%lf\n", *rms_phase_A);
    printf("%lf\n", *rms_phase_B);
    printf("%lf\n", *rms_phase_C);
}