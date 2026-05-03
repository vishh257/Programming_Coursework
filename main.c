#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Waveform.h"
#include "io.h"

int main(void) {

    //creating an array like data structure to store the output metrics
    metrics *output = calloc(1, sizeof(metrics));
    //function to calculate the no of rows in .csv file
    output->rows = rows("power_quality_log.csv");
    //creating an array like data structure to store the input metrics
    EightStruct *WaveformSample = calloc(output->rows, sizeof(EightStruct));


    int i = load_value("power_quality_log.csv", WaveformSample, output->rows); //calling the function to load data
    if (i == 1) {
        free(output);
        free(WaveformSample);
        return -1;
    }

    //calling functions to calculate rms voltage for each phase
    output->phase[0].rms = rms_voltage(&(WaveformSample[0].phase_A_voltage), output, 1);
    output->phase[1].rms = rms_voltage(&(WaveformSample[0].phase_B_voltage), output, 2);
    output->phase[2].rms = rms_voltage(&(WaveformSample[0].phase_C_voltage), output, 3);

    //calling functions to calculate p2p and mean and detecting clipping for each phase
    analysis(&(WaveformSample[0].phase_A_voltage), output, 1);
    analysis(&(WaveformSample[0].phase_B_voltage), output, 2);
    analysis(&(WaveformSample[0].phase_C_voltage), output, 3);

    //calling functions to calculate variance and standard deviation for each phase
    variance(&(WaveformSample[0].phase_A_voltage), output->phase[0].mean, &output->phase[0].var, &output->phase[0].sd, output->rows);
    variance(&(WaveformSample[0].phase_B_voltage), output->phase[1].mean, &output->phase[1].var, &output->phase[1].sd, output->rows);
    variance(&(WaveformSample[0].phase_C_voltage), output->phase[2].mean, &output->phase[2].var, &output->phase[2].sd, output->rows);

    //calling sort function
    sort(WaveformSample, 'A', output->rows);
    sort(WaveformSample, 'B', output->rows);
    sort(WaveformSample, 'C', output->rows);

    //calling range function to calculate the range for the other parameters
    output->frequency = range(&(WaveformSample[0].frequency), output->rows);
    output->power_factor = range(&(WaveformSample[0].power_factor), output->rows);
    output->thd_percent = range(&(WaveformSample[0].thd_percent), output->rows);

    //using all the calculated metrics to detect tolerance and flag bits
    bitcheck(output);

    //printing the results file
    results(output);

    free(WaveformSample);
    free(output);
    
    return 0;
}