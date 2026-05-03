#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "Waveform.h"

int rows(const char *filename) {

    int count = 0;
    FILE *pCSV = fopen(filename, "r"); //opening csv file

    //checking for errors
    if(pCSV == NULL){
        printf("Could not open file");  //indicating the user if opening the file fails
        return 1;
    }


    char buffer[256]; //buffer to store values from fgets

    //skipping the header
    fgets(buffer, sizeof(buffer), pCSV);

    while (fgets(buffer, sizeof(buffer), pCSV) != NULL) {
        count++; //after fgets hits a newline it stores it into buffer
    }

    fclose(pCSV);
    return count;
}

int load_value(const char *filename, EightStruct *WaveformSample, int rows) {

    int i = 0;
    FILE *pCSV = fopen(filename, "r"); //opening csv file

    //skipping the header
    if(pCSV == NULL){
        printf("Could not open file");  //indicating the user if opening the file fails
        return 1;
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

    return 0;
}

void results(metrics *output) {
    //creating a file called output.txt and writing to it
    FILE *results = fopen("output.txt", "w");

    //error checking
    if (results == NULL) {
        printf("\n\nFile does not open");
        return; }

    //writing rms, p2p and mean for each phase
    fprintf(results, "\nPhase A rms is: %lf\n", output->phase[0].rms);
    fprintf(results, "Phase A peak to peak is: %lf\n", output->phase[0].p2p);
    fprintf(results, "Phase A DC offset is: %.16lf\n\n", output->phase[0].mean);

    fprintf(results, "\nPhase B rms is: %lf\n", output->phase[1].rms);
    fprintf(results, "Phase B peak to peak is: %lf\n", output->phase[1].p2p);
    fprintf(results, "Phase B DC offset is: %.16lf\n\n", output->phase[1].mean);

    fprintf(results, "\nPhase C rms is: %lf\n", output->phase[2].rms);
    fprintf(results, "Phase C peak to peak is: %lf\n", output->phase[2].p2p);
    fprintf(results, "Phase C DC offset is: %.16lf\n\n", output->phase[2].mean);

    //writing the no of clipped samples
    fprintf(results, "Total clipped samples is %d\n\n", output->clipping);

    //writing variance and standard deviation for each phase
    fprintf(results, "Phase A Variance is: %lf\n", output->phase[0].var);
    fprintf(results, "Phase A Standard Deviation is: %lf\n\n", output->phase[0].sd);

    fprintf(results, "Phase B Variance is: %lf\n", output->phase[1].var);
    fprintf(results, "Phase B Standard Deviation is: %lf\n\n", output->phase[1].sd);

    fprintf(results, "Phase C Variance is: %lf\n", output->phase[2].var);
    fprintf(results, "Phase C Standard Deviation is: %lf\n\n", output->phase[2].sd);

    //wirting the other parameters
    fprintf(results, "Frequency Range: %lf Hz\n\n", output->frequency);
    fprintf(results, "Power factor Range is: %lf\n\n", output->power_factor);
    fprintf(results, "Thd Percent Range is: %lf\n\n", output->thd_percent);

    //printing the bit status flags
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

