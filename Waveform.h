#ifndef PROGRAMMING_COURSEWORK_EIGHTSTRUCT_H
#define PROGRAMMING_COURSEWORK_EIGHTSTRUCT_H
typedef struct{
    double timestamp, phase_A_voltage, phase_B_voltage,
    phase_C_voltage, line_current, frequency, power_factor, thd_percent;

}EightStruct;

typedef struct {
    double rms_A, rms_B, rms_C, p2p_A, p2p_B, p2p_C, mean_A, mean_B, mean_C;
    double var_A, var_B, var_C, sd_A, sd_B, sd_C;
    int clipping;
}metrics;

double rms_voltage(double *sample);

void analysis(double *sample, double *p2p, double *mean, int *clipping);

void variance(double *sample, double mean, double *variance, double *std_deviation);

void sort(EightStruct *WaveformSample, char phase);

#endif //PROGRAMMING_COURSEWORK_EIGHTSTRUCT_H
