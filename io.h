//
// Created by Vishvesh on 04/04/2026.
//

#ifndef PROGRAMMING_COURSEWORK_IO_H
#define PROGRAMMING_COURSEWORK_IO_H
#include "Waveform.h"

int rows(const char *filename);

void load_value(const char *filename, EightStruct *WaveformSample, int rows);

void results(metrics *output);

#endif //PROGRAMMING_COURSEWORK_IO_H
