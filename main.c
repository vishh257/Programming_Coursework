#include <stdio.h>
#include <stdlib.h>
#include "Waveform.h"
#include "io.h"

int main(void) {

    // all the metrics
    /*double rms_A, rms_B, rms_C, p2p_A, p2p_B, p2p_C, mean_A, mean_B, mean_C;
    double var_A, var_B, var_C, sd_A, sd_B, sd_C;
    int clipping;*/


    EightStruct *WaveformSample = calloc(1000, sizeof(EightStruct));
    metrics *output = calloc(1, sizeof(metrics));

    //creating an array of struct

    load_value("power_quality_log.csv", WaveformSample); //calling the function to load data

    //calling functions to calculate rms voltage for each phase
    output->rms_A = rms_voltage(&(WaveformSample[0].phase_A_voltage), &(output->tolerance));
    output->rms_B = rms_voltage(&(WaveformSample[0].phase_B_voltage), &(output->tolerance));
    output->rms_C = rms_voltage(&(WaveformSample[0].phase_C_voltage), &(output->tolerance));

    //testing sort
    /*for (int t = 0; t < 1000; t++) {
        double test123 = WaveformSample[t].phase_C_voltage;
        printf("%lf\n", test123);
    }*/

    analysis(&(WaveformSample[0].phase_A_voltage), &output->p2p_A, &output->mean_A, &output->clipping);
    analysis(&(WaveformSample[0].phase_B_voltage), &output->p2p_B, &output->mean_B, &output->clipping);
    analysis(&(WaveformSample[0].phase_C_voltage), &output->p2p_C, &output->mean_C, &output->clipping);

    variance(&(WaveformSample[0].phase_A_voltage), output->mean_A, &output->var_A, &output->sd_A);
    variance(&(WaveformSample[0].phase_B_voltage), output->mean_B, &output->var_B, &output->sd_B);
    variance(&(WaveformSample[0].phase_C_voltage), output->mean_C, &output->var_C, &output->sd_C);

    //calling sort function
    sort(WaveformSample, 'A');
    sort(WaveformSample, 'B');
    sort(WaveformSample, 'C');

    output->frequency = range(&(WaveformSample[0].frequency));
    output->power_factor = range(&(WaveformSample[0].power_factor));
    output->thd_percent = range(&(WaveformSample[0].thd_percent));

    results(output);

    free(WaveformSample);
    free(output);


    return 0;
}