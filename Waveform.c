#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Waveform.h"
#include "io.h"

double rms_voltage(double *sample){
    double temp = 0;


    for (int i = 0; i < 1000; i++) {
        temp = temp + pow(*(sample + (8*i)), 2);
    }

    return sqrt(temp/1000);
}