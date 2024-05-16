#ifndef _ADC_IO_testing_h
#define _ADC_IO_testing_h

// things

void initADC_A(void);
void GPIO242_mux_IN_ADC_500VAC(void);
void GPIO224_mux_IN_CP_ADC(void);


void init_IN_ADC_500VAC(void);
void init_IN_CP_ADC(void);
void init_IN_CP_Borne_ADC(void);


//
void init_EPWM1(void);
void init_EPWM2(void);
void init_EPWM4(void);


//
__interrupt void adcA1ISR(void);
__interrupt void adcA2ISR(void);
__interrupt void adcA3ISR(void);

void start_read_adc(void);
void start_EPWM1(void);
void start_EPWM2(void);
void start_EPWM4(void);

void stop_EPWM1(void);
void stop_EPWM2(void);
void stop_EPWM4(void);



#endif
