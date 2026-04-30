#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "Waveform.h"

int rows(const char *filename) {

    int count = 0;
    FILE *pCSV = fopen(filename, "r"); //opening csv file

    if(pCSV == NULL){
        printf("Could not open file");  //indicating the user if opening the file fails
        return 1;
    }

    char buffer[256]; //buffer to store values from fgets

    fgets(buffer, sizeof(buffer), pCSV);

    while (fgets(buffer, sizeof(buffer), pCSV) != NULL) {
        count++;
    }

    fclose(pCSV);
    return count;
}

void load_value(const char *filename, EightStruct *WaveformSample, int rows) {

    int i = 0;
    FILE *pCSV = fopen(filename, "r"); //opening csv file

    if(pCSV == NULL){
        printf("Could not open file");  //indicating the user if opening the file fails
        free(WaveformSample);
        return;
    }

    char buffer[256]; //buffer to store values from fgets

    fgets(buffer, sizeof(buffer), pCSV); //skipping the header

    while ((fgets(buffer, sizeof(buffer), pCSV) != NULL) && i < rows) {

        char *token = strtok(buffer, ","); //temp array like datastructure to store string from strtok
        WaveformSample[i].timestamp  = atof(token); //converting string into float to store into struct

        token = strtok(NULL, ",");
        WaveformSample[i].phase_A_voltage  = atof(token);

        token = strtok(NULL, ",");
        WaveformSample[i].phase_B_voltage  = atof(token);

        token = strtok(NULL, ",");
        WaveformSample[i].phase_C_voltage  = atof(token);

        token = strtok(NULL, ",");
        WaveformSample[i].line_current  = atof(token);

        token = strtok(NULL, ",");
        WaveformSample[i].frequency  = atof(token);

        token = strtok(NULL, ",");
        WaveformSample[i].power_factor  = atof(token);

        token = strtok(NULL, ",");
        WaveformSample[i].thd_percent  = atof(token);

        i++; //increment i to store it into next row in the struct
    }

    fclose(pCSV);
}

void results(metrics *output) {

    FILE *results = fopen("output.txt", "w");

    if (results == NULL) printf("\n\nFile does not open");

    fprintf(results, "\nPhase A rms is: %lf\n", output->phase[0].rms);
    fprintf(results, "Phase A peak to peak is: %lf\n", output->phase[0].p2p);
    fprintf(results, "Phase A DC offset is: %.16lf\n\n", output->phase[0].mean);

    fprintf(results, "\nPhase B rms is: %lf\n", output->phase[1].rms);
    fprintf(results, "Phase B peak to peak is: %lf\n", output->phase[1].p2p);
    fprintf(results, "Phase B DC offset is: %.16lf\n\n", output->phase[1].mean);

    fprintf(results, "\nPhase C rms is: %lf\n", output->phase[2].mean);
    fprintf(results, "Phase C peak to peak is: %lf\n", output->phase[2].p2p);
    fprintf(results, "Phase C DC offset is: %.16lf\n\n", output->phase[2].mean);

    fprintf(results, "Total clipped samples is %d\n\n", output->clipping);

    switch (output->tolerance) {
        case 0:
            fprintf(results, "All phases are not in tolerance\n\n");
        case 1:
            fprintf(results, "Phase A is within tolerance. Phase B and C are not\n\n");
        case 2:
            fprintf(results, "Phase A and B are within tolerance. Phase C is not\n\n");
        case 3:
            fprintf(results, "All phases are in tolerance\n\n");
    }

    fprintf(results, "Phase A Variance is: %lf\n", output->phase[0].var);
    fprintf(results, "Phase A Standard Deviation is: %lf\n\n", output->phase[0].sd);

    fprintf(results, "Phase B Variance is: %lf\n", output->phase[1].var);
    fprintf(results, "Phase B Standard Deviation is: %lf\n\n", output->phase[1].sd);

    fprintf(results, "Phase C Variance is: %lf\n", output->phase[2].var);
    fprintf(results, "Phase C Standard Deviation is: %lf\n\n", output->phase[2].sd);

    fprintf(results, "Frequency Range: %lf\n\nHz", output->frequency);
    fprintf(results, "Power factor is: %lf\n\n", output->power_factor);
    fprintf(results, "Power factor is: %lf\n\n", output->thd_percent);

    fprintf(results, "Phase A bitstatus:\n");
    for (int i = 7; i >= 0; i--) {
        fprintf(results, "%d", (output->phase[0].status >> i) & 1);
    }
    fprintf(results, "\n\n");

    fprintf(results, "Phase B bitstatus:\n");
    for (int i = 7; i >= 0; i--) {
        fprintf(results, "%d", (output->phase[1].status >> i) & 1);
    }
    fprintf(results, "\n\n");

    fprintf(results, "Phase C bitstatus:\n");
    for (int i = 7; i >= 0; i--) {
        fprintf(results, "%d", (output->phase[2].status >> i) & 1);
    }
    fprintf(results, "\n\n");

    printf("\n");


    fclose(results);
}

