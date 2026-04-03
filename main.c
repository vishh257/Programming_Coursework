#include <stdio.h>
#include <stdlib.h>
#include "eightstruct.h"

int main(void) {

    float *WaveformSample = calloc(1000, sizeof(float));

    free(WaveformSample);
    return 0;
}
