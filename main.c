#include <stdio.h>
#include <stdlib.h>
#include "Waveform.h"
#include "io.h"

int main(void) {

    int i = 0;
    double rms_A, rms_B, rms_C;

    EightStruct *WaveformSample = calloc(1000, sizeof(EightStruct));
    //creating an array of struct

    load_value("power_quality_log.csv", WaveformSample); //calling the function to load data

    //calling functions to calculate rms voltage for each phase
    rms_A = rms_voltage(&(WaveformSample[0].phase_A_voltage));
    rms_B = rms_voltage(&(WaveformSample[0].phase_B_voltage));
    rms_C = rms_voltage(&(WaveformSample[0].phase_C_voltage));

    sort(WaveformSample, 'C');

    //testing sort
    for (int t = 0; t < 1000; t++) {
        double test123 = WaveformSample[t].phase_C_voltage;
        printf("%lf\n", test123);
    }
    free(WaveformSample);

    return 0;
}