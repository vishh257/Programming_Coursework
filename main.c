#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Waveform.h"
#include "io.h"

int main(void) {

    int i = 0;
    EightStruct *WaveformSample = calloc(1000, sizeof(EightStruct));
    //creating an array of struct

    load_value("power_quality_log.csv", WaveformSample);


    free(WaveformSample);

    return 0;
}
