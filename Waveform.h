#ifndef PROGRAMMING_COURSEWORK_EIGHTSTRUCT_H
#define PROGRAMMING_COURSEWORK_EIGHTSTRUCT_H
typedef struct{
    double timestamp, phase_A_voltage, phase_B_voltage,
    phase_C_voltage, line_current, frequency, power_factor, thd_percent;

}EightStruct;

double rms_voltage(double *sample);
void analysis(double *sample, double *p2p, double *mean);
void sort(EightStruct *WaveformSample, char phase);
#endif //PROGRAMMING_COURSEWORK_EIGHTSTRUCT_H
