#ifndef PROGRAMMING_COURSEWORK_EIGHTSTRUCT_H
#define PROGRAMMING_COURSEWORK_EIGHTSTRUCT_H
typedef struct{
    double timestamp, phase_A_voltage, phase_B_voltage,
    phase_C_voltage, line_current, frequency, power_factor, thd_percent;

}EightStruct;

void rms_voltage(EightStruct *WaveformSample);
#endif //PROGRAMMING_COURSEWORK_EIGHTSTRUCT_H
