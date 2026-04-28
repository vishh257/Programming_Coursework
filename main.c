#include <stdio.h>
#include <stdlib.h>
#include "Waveform.h"
#include "io.h"

int main(void) {

    metrics *output = calloc(1, sizeof(metrics));

    output->rows = rows("power_quality_log.csv");

    EightStruct *WaveformSample = calloc(1000, sizeof(EightStruct));



    load_value("power_quality_log.csv", WaveformSample, output->rows); //calling the function to load data

    //calling functions to calculate rms voltage for each phase
    output->rms_A = rms_voltage(&(WaveformSample[0].phase_A_voltage), output);
    output->rms_B = rms_voltage(&(WaveformSample[0].phase_B_voltage), output);
    output->rms_C = rms_voltage(&(WaveformSample[0].phase_C_voltage), output);

    analysis(&(WaveformSample[0].phase_A_voltage), &output->p2p_A, &output->mean_A, &output->clipping, output->rows);
    analysis(&(WaveformSample[0].phase_B_voltage), &output->p2p_B, &output->mean_B, &output->clipping, output->rows);
    analysis(&(WaveformSample[0].phase_C_voltage), &output->p2p_C, &output->mean_C, &output->clipping, output->rows);

    variance(&(WaveformSample[0].phase_A_voltage), output->mean_A, &output->var_A, &output->sd_A, output->rows);
    variance(&(WaveformSample[0].phase_B_voltage), output->mean_B, &output->var_B, &output->sd_B, output->rows);
    variance(&(WaveformSample[0].phase_C_voltage), output->mean_C, &output->var_C, &output->sd_C, output->rows);

    //calling sort function
    sort(WaveformSample, 'A', output->rows);
    sort(WaveformSample, 'B', output->rows);
    sort(WaveformSample, 'C', output->rows);

    output->frequency = range(&(WaveformSample[0].frequency), output->rows);
    output->power_factor = range(&(WaveformSample[0].power_factor), output->rows);
    output->thd_percent = range(&(WaveformSample[0].thd_percent), output->rows);

    results(output);

    free(WaveformSample);
    free(output);


    return 0;
}