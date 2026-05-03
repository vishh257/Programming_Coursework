#ifndef PROGRAMMING_COURSEWORK_EIGHTSTRUCT_H
#define PROGRAMMING_COURSEWORK_EIGHTSTRUCT_H
typedef struct{
    double timestamp, phase_A_voltage, phase_B_voltage,
    phase_C_voltage, line_current, frequency, power_factor, thd_percent;

}EightStruct;

typedef struct {
    double rms, p2p, mean, var, sd;
    uint8_t status;
}PhaseMetrics;

typedef struct {
    PhaseMetrics phase[3];
    double frequency, power_factor, thd_percent;
    int clipping, rows;
}metrics;

double rms_voltage(double *sample, metrics *output, int phaseno);

void analysis(double *sample, metrics *output, int phaseno);

void variance(double *sample, double mean, double *variance, double *std_deviation, int rows);

double range(double *sample, int rows);

void bitcheck(metrics *output);

void sort(EightStruct *WaveformSample, char phase, int rows);

#endif //PROGRAMMING_COURSEWORK_EIGHTSTRUCT_H
