#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eightstruct.h"

int main(void) {

    int i = 0;
    EightStruct *WaveformSample = calloc(1000, sizeof(EightStruct));
    //creating an array of struct

    FILE *pCSV = fopen("power_quality_log.csv", "r");

    if(pCSV == NULL){
        printf("Could not open file");  //indicating the user if opening the file fails
        free(WaveformSample);
        return 1;
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), pCSV); //skipping the header
    while ((fgets(buffer, sizeof(buffer), pCSV) != NULL) && i < 1000) {

        char *token = strtok(buffer, ",");
        WaveformSample[i].timestamp  = atof(token);

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

      i++;
    }

    //printf("%.16f\n", WaveformSample[50].phase_C_voltage);
    //printf("%.16f\n", WaveformSample[79].phase_A_voltage);
    //printf("%.16f\n", WaveformSample[2].thd_percent);

    fclose(pCSV);
    free(WaveformSample);

    return 0;
}
