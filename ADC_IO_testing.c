//#############################################################################
//
// FILE: ADC_IO_testing.c
//
// TITLE: ADC Testing Implementation for F2800132
//
// DESCRIPTION:.
// This file implements functions for initializing the device, configuring
// ADC pins, and testing them .
//It specifically handles the setup for three distinct ADC channels, triggers
// conversions on these channels, and stores the results.
// AUTHOR: [Kenneth TANFA ]
//
//#############################################################################


// Included Files
//
#include <ADC_IO_testing.h>
#include "f28x_project.h"

//
// Defines
//
#define RESULTS_BUFFER_SIZE     512

//
// Globals
//
uint16_t array_IN_ADC_500VAC[RESULTS_BUFFER_SIZE];
uint16_t array_IN_CP_ADC[RESULTS_BUFFER_SIZE];
uint16_t array_IN_CP_BORNE[RESULTS_BUFFER_SIZE];

// Buffer for results
uint16_t index;                              // Index into result buffer
uint16_t index2;
volatile uint16_t bufferFull;                // Flag to indicate buffer is full


//__interrupt void cpuTimer2ISR(void);
__interrupt void adcA1ISR(void);


//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    InitSysCtrl();

    //
    // Initialize GPIO
    //
    InitGpio();
    //Enable analog capability of the pin A3 -> IN_CP_500VAC and pin A2 -> IN_CP_ADC
    GPIO242_mux_IN_ADC_500VAC();
    GPIO224_mux_IN_CP_ADC();

    //
    // Disable CPU interrupts
    //
    DINT;

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    //
    InitPieCtrl();

    //
    // Disable CPU interrupts and clear all CPU interrupt flags:
    //
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    InitPieVectTable();

    // Map ISR functions
    //
    EALLOW;
   // PieVectTable.TIMER2_INT = &cpuTimer2ISR;
    PieVectTable.ADCA1_INT = &adcA1ISR;     // Function for ADCA interrupt 1

    EDIS;
    //
    // Initialize the Device Peripheral. For this example, only initialize the
    // Cpu Timers.
    //
    //InitCpuTimers();

    //
    // Configure CPU-Timer 0 to interrupt every second:
    // 100MHz CPU Freq, 1 second Period (in uSeconds)
    //
    //ConfigCpuTimer(&CpuTimer2, 100, 100);

    //
    // To ensure precise timing, use write-only instructions to write to the
    // entire register. Therefore, if any of the configuration bits are changed
    // in ConfigCpuTimer and InitCpuTimers, the below settings must also be
    // be updated.
    //
    //CpuTimer2Regs.TCR.all = 0x4000;

     //
     // Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
     // which is connected to CPU-Timer 1, and CPU int 14, which is connected
     // to CPU-Timer 2
     //
    IER |= M_INT1;  // Enable group 1 interrupts
    //IER |= M_INT14;

     //
     // Enable TINT0 in the PIE: Group 1 interrupt 7
     //
     PieCtrlRegs.PIEIER1.bit.INTx1 = 1;

     //
     // Enable global Interrupts and higher priority real-time debug events
     //
     EINT;
     ERTM;


     //
     // Sync ePWM
     //
     EALLOW;
     CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;

    //
    // Configure the ADC and power it up
    //
    initADC_A();



    //
    // Setup the ADC for software triggered conversions
    //
    init_IN_ADC_500VAC();
    init_IN_CP_ADC();
    init_IN_CP_Borne_ADC();


    //
    // Initialize results buffer
    //
    for(index = 0; index < RESULTS_BUFFER_SIZE; index++)
    {
        
        array_IN_ADC_500VAC[index] = 0;
        array_IN_CP_ADC[index] = 0; 
        array_IN_CP_BORNE[index] = 0;
    }

    index = 0;
    index2 = 0;
    bufferFull = 0;





    //
    // Take conversions indefinitely in loop
    //
    start_EPWM1();
    while(1)
    {
     

        //
        // Wait while ePWM causes ADC conversions, which then cause interrupts,
        // which fill the results buffer, eventually setting the bufferFull
        // flag
        //
        
        //start_read_adc();

        //
        // Software breakpoint. At this point, conversion results are stored in
        // adcAResults.
        //
        // Hit run again to get updated conversions.
        //
        //ESTOP0;
        
    }
}

//
// initADC - Function to configure and power up ADCA.
//
void initADC_A(void)
{
    //
    // Setup VREF as internal
    //
    SetVREF(ADC_ADCA, ADC_INTERNAL, ADC_VREF3P3);

    EALLOW;

    //
    // Set ADCCLK divider to /4
    //
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6;

    //
    // Set pulse positions to late
    //
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //
    // Power up the ADC and then delay for 1 ms
    //
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    EDIS;

    DELAY_US(1000);
}

// COnfigure the GPIO in order to enable their analago capabilities 

// 
//  GPIO242_mux_IN_ADC_500VAC
//
void GPIO242_mux_IN_ADC_500VAC(void){

    EALLOW;
    AnalogSubsysRegs.AGPIOCTRLH.bit.GPIO242 = 1;// enable analog capability of the pin
    GpioCtrlRegs.GPHAMSEL.bit.GPIO242 = 1; // enable analog capability of the pin
    EDIS;

}

// 
//  GPIO224_mux_IN_ADC_500VAC
//
void GPIO224_mux_IN_CP_ADC(void){

    EALLOW;
    AnalogSubsysRegs.AGPIOCTRLH.bit.GPIO224 = 1;// enable analog capability of the pin
    GpioCtrlRegs.GPHAMSEL.bit.GPIO224 = 1; // enable analog capability of the pin
    EDIS;

}
// initEPWM - Function to configure ePWM1 to generate the SOC.
//

//
// init_IN_ADC_500VAC - Function to configure ADCA's SOC0 to be triggered by ePWM1.
//
void init_IN_ADC_500VAC(void)
{
    //
    // Select the channels to convert and the end of conversion flag
    //
    EALLOW;

    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 3;     // SOC0 will convert pin A3
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 9;     // Sample window is 10 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5;   // EPWM1 SOCA

    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; // End of SOC0 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   // Enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; // Make sure INT1 flag is cleared

    EDIS;
}

//
// init_IN_ADC_500VAC - Function to configure ADCA's SOC0 to be triggered by ePWM1.
//
void init_IN_CP_ADC(void)
{
    //
    // Select the channels to convert and the end of conversion flag
    //
    EALLOW;

    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 2;     // SOC0 will convert pin A2
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = 9;     // Sample window is 10 SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 0;   // Software only

    AdcaRegs.ADCINTSEL1N2.bit.INT2SEL = 1; // End of SOC1 will set INT2 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT2E = 1;   // Enable INT2 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; // Make sure INT2 flag is cleared

    EDIS;
}

//
// init_IN_ADC_500VAC - Function to configure ADCA's SOC0 to be triggered by ePWM1.
//
void init_IN_CP_Borne_ADC(void)
{
    //
    // Select the channels to convert and the end of conversion flag
    //
    EALLOW;

    AdcaRegs.ADCSOC2CTL.bit.CHSEL = 11;     // SOC0 will convert pin A11
                                           // 0:A0  1:A1  2:A2  3:A3
                                           // 4:A4   5:A5   6:A6   7:A7
                                           // 8:A8   9:A9   A:A10  B:A11
                                           // C:A12  D:A13  E:A14  F:A15

    AdcaRegs.ADCSOC2CTL.bit.ACQPS = 9;     // Sample window is 10 SYSCLK cycles
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 0;   // Software only

    AdcaRegs.ADCINTSEL3N4.bit.INT3SEL = 2; // End of SOC2 will set INT3 flag
    AdcaRegs.ADCINTSEL3N4.bit.INT3E = 1;   // Enable INT3 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; // Make sure INT3 flag is cleared

    EDIS;
}

// Function to read ADC values sequentially
void start_read_adc(void) {

    while(index < RESULTS_BUFFER_SIZE) {
        // Trigger and wait for the first conversion (ADC0)
        AdcaRegs.ADCSOCFRC1.bit.SOC0  = 1; // Force SOC0 conversion
        while(!AdcaRegs.ADCINTFLG.bit.ADCINT1); // Wait for completion
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; // Clear flag
        array_IN_ADC_500VAC[index] = AdcaResultRegs.ADCRESULT0; // Store result

        // Trigger and wait for the second conversion (ADC1)
        AdcaRegs.ADCSOCFRC1.bit.SOC1 = 1; // Force SOC1 conversion
        while(!AdcaRegs.ADCINTFLG.bit.ADCINT2); // Wait for completion
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; // Clear flag
        array_IN_CP_ADC[index] = AdcaResultRegs.ADCRESULT1; // Store result

        // Trigger and wait for the third conversion (ADC2)
        AdcaRegs.ADCSOCFRC1.bit.SOC2 = 1; // Force SOC2 conversion
        while(!AdcaRegs.ADCINTFLG.bit.ADCINT3); // Wait for completion
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; // Clear flag
        array_IN_CP_BORNE[index] = AdcaResultRegs.ADCRESULT2; // Store result

        index++; // Increment index

        if(index >= RESULTS_BUFFER_SIZE) {
            bufferFull = 1; // Set buffer full flag
            break;
        }
    }
}

void init_EPWM1(void)
{
    EALLOW;

    EPwm1Regs.ETSEL.bit.SOCAEN = 0;     // Disable SOC on A group
    EPwm1Regs.ETSEL.bit.SOCASEL = 4;    // Select SOC on up-count
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;     // Generate pulse on 1st event

    EPwm1Regs.CMPA.bit.CMPA = 0x0800;   // Set compare A value to 2048 counts
    EPwm1Regs.TBPRD = 0x1000;           // Set period to 4096 counts


    EDIS;
}

void start_EPWM1(void){
               // Start ePWM
               //
               EPwm1Regs.ETSEL.bit.SOCAEN = 1;    // Enable SOCA
               EPwm1Regs.TBCTL.bit.CTRMODE = 0;   // Unfreeze, and enter up count mode
}

void stop_EPWM1(void){

               // Stop ePWM
    EPwm1Regs.ETSEL.bit.SOCAEN = 0;    // Disable SOCA
    EPwm1Regs.TBCTL.bit.CTRMODE = 3;   // Freeze counter
}
__interrupt void adcA1ISR(void)
{
    //
    // Add the latest result to the buffer
    // ADCRESULT0 is the result register of SOC0
    array_IN_ADC_500VAC[index++] = AdcaResultRegs.ADCRESULT0;

    //
    // Set the bufferFull flag if the buffer is full
    //
    if(RESULTS_BUFFER_SIZE <= index)
    {
        index = 0;
        bufferFull = 1;
    }

    //
    // Clear the interrupt flag
    //
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    //
    // Check if overflow has occurred
    //
    if(1 == AdcaRegs.ADCINTOVF.bit.ADCINT1)
    {
        AdcaRegs.ADCINTOVFCLR.bit.ADCINT1 = 1; //clear INT1 overflow flag
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    }

    //
    // Acknowledge the interrupt
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


//
// End of File
//
