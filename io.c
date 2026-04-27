#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "Waveform.h"

void load_value(const char *filename, EightStruct *WaveformSample) {

    int i = 0;
    FILE *pCSV = fopen(filename, "r"); //opening csv file

    if(pCSV == NULL){
        printf("Could not open file");  //indicating the user if opening the file fails
        free(WaveformSample);
        return;
    }

    char buffer[256]; //buffer to store values from fgets

    fgets(buffer, sizeof(buffer), pCSV); //skipping the header

    while ((fgets(buffer, sizeof(buffer), pCSV) != NULL) && i < 1000) {

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

    //printf("%.16f\n", WaveformSample[50].phase_C_voltage);
    //printf("%.16f\n", WaveformSample[79].phase_A_voltage);
    //printf("%.16f\n", WaveformSample[2].thd_percent);

    fclose(pCSV);
}

/*void results() {

    FILE *results = fopen("output.txt", "w");

    fprintf(results, );


}*/