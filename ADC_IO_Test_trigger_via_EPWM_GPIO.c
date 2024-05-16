//#############################################################################
//
// FILE:   adc_ex1_soc_epwm.c
//
// TITLE:  ADC ePWM Triggering
//
//! \addtogroup bitfield_example_list
//! <h1>ADC ePWM Triggering</h1>
//!
//! This example demonstrates how to set up multiple ePWM modules to periodically
//! trigger conversions on the ADCA module, and how to manage the results in
//! different buffers.
//!
//! The setup includes:
//!  - ePWM1 triggering conversions on ADCA channel A3 (connected to signal IN_CP_500VAC).
//!  - ePWM2 triggering conversions on ADCA channel A2 (connected to signal IN_CP_ADC).
//!  - ePWM4 triggering conversions on ADCA channel A11 (connected to signal IN_CP_BORNE).
//!
//! The code uses the following steps:
//!  1. Initialize system control and GPIO for analog inputs.
//!  2. Configure and power up the ADCA module.
//!  3. Set up ePWM modules to trigger ADC conversions.
//!  4. Configure ADC channels to be triggered by corresponding ePWM modules.
//!  5. Enable interrupts for each ADC channel.
//!  6. Implement interrupt service routines (ISRs) to store conversion results in buffers.
//!  7. Monitor buffer status in the main loop to determine when buffers are full.
//!
//! When a buffer is full, its corresponding flag is set and the main loop waits
//! until all buffers are full before resetting the flags and continuing the process.
//!
//! \b External \b Connections \n
//!  - A3 (GPIO242) should be connected to the signal to be converted by ePWM1 (IN_CP_500VAC).
//!  - A2 (GPIO224) should be connected to the signal to be converted by ePWM2 (IN_CP_ADC).
//!  - A11 should be connected to the signal to be converted by ePWM4 (IN_CP_BORNE).
//!
//! \b Watch \b Variables \n
//!  - \b array_IN_ADC_500VAC - Buffer storing ADC conversion samples from channel A3.
//!  - \b array_IN_CP_ADC - Buffer storing ADC conversion samples from channel A2.
//!  - \b array_IN_CP_BORNE - Buffer storing ADC conversion samples from channel A11.
//!  - \b array_IN_ADC_500VAC_bufferFull - Flag indicating when the buffer for channel A3 is full.
//!  - \b array_IN_CP_ADC_bufferFull - Flag indicating when the buffer for channel A2 is full.
//!  - \b array_IN_CP_BORNE_ADC_bufferFull - Flag indicating when the buffer for channel A11 is full.
//!
//! The main loop waits for all buffers to be filled before resetting the buffer full flags
//! and continuing the sampling process. This ensures synchronized data acquisition from
//! multiple channels triggered by different ePWM modules.
//
//#############################################################################
//
// $Copyright:
// Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the
//   distribution.
//
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//#############################################################################


//
// Included Files
//
#include "f28x_project.h"
#include "ADC_IO_testing.h"
#include "Test_GPIO.h"

//
// Defines
//
#define RESULTS_BUFFER_SIZE  2000
#define PERIODE_10u 625
#define CMPA_      312

//
// Globals
//
uint16_t array_IN_ADC_500VAC[RESULTS_BUFFER_SIZE];
uint16_t array_IN_CP_ADC[RESULTS_BUFFER_SIZE];
uint16_t array_IN_CP_BORNE[RESULTS_BUFFER_SIZE];
uint16_t index_1;                              // Index into result buffer
uint16_t index_2;                              // Index into result buffer
uint16_t index_3;                              // Index into result buffer


// Flags to indicate buffer is full
volatile uint16_t array_IN_ADC_500VAC_bufferFull = 0;
volatile uint16_t array_IN_CP_ADC_bufferFull = 0;
volatile uint16_t array_IN_CP_BORNE_ADC_bufferFull = 0;


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

    // COnfigure GPIo
    Configure_GPIO();
    select_CP_9V();


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

    //
    // Map ISR functions
    //
    EALLOW;
    PieVectTable.ADCA1_INT = &adcA1ISR;     // Function for ADCA interrupt 1
    PieVectTable.ADCA2_INT = &adcA2ISR;     // Function for ADCA interrupt 2
    PieVectTable.ADCA3_INT = &adcA3ISR;     // Function for ADCA interrupt 3
    EDIS;

    //
    // Configure the ADC and power it up
    //
    initADC_A();

    //
    // Setup the ADC for ePWM triggered conversions on channel 1
    //

    // Init the ADC channels
    init_IN_ADC_500VAC();
    init_IN_CP_ADC();
    init_IN_CP_Borne_ADC();

    //
    // Configure the ePWM
    //
    init_EPWM1();
    init_EPWM2();
    init_EPWM4();



    //
    // Enable global Interrupts and higher priority real-time debug events:
    //
    IER |= M_INT1;  // Enable group 1 interrupts
    IER |= M_INT10; // Enable group 10 interrupts


    EINT;           // Enable Global interrupt INTM
    ERTM;           // Enable Global realtime interrupt DBGM

    //
    // Initialize results buffer
    //
    for(index_1 = 0; index_1 < RESULTS_BUFFER_SIZE; index_1++)
    {
        array_IN_ADC_500VAC[index_1] = 0;
        array_IN_CP_ADC[index_1] = 0;
        array_IN_CP_BORNE[index_1] = 0;
    }

    index_1 = 0;
    index_2 = 0;
    index_3 = 0;

    array_IN_ADC_500VAC_bufferFull = 0;
    array_IN_CP_ADC_bufferFull = 0;
    array_IN_CP_BORNE_ADC_bufferFull = 0;
    //
    // Enable PIE interrupt individually
    //
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1; // // enable interrupt x1 within the group 1
    PieCtrlRegs.PIEIER10.bit.INTx2 = 1; // enable interrupt x2 within the group 10
    PieCtrlRegs.PIEIER10.bit.INTx3 = 1; // enable interrupt x3 within the group 10
    //
    // Sync ePWM
    //
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;

    //
    //Start the EPWMS
    //
    start_EPWM1();
    start_EPWM2();
    start_EPWM4();



    //
    // Take conversions indefinitely in loop
    //


    while(1)
    {



        //
        // Wait while ePWM causes ADC conversions, which then cause interrupts,
        // which fill the results buffer, eventually setting the bufferFull
        // flag
        //
        while((!array_IN_ADC_500VAC_bufferFull) && (!array_IN_CP_ADC_bufferFull) && (!array_IN_CP_BORNE_ADC_bufferFull) )
        {}

        array_IN_ADC_500VAC_bufferFull= 0; //clear the buffer full flag
        array_IN_CP_ADC_bufferFull= 0; //clear the buffer full flag
        array_IN_CP_BORNE_ADC_bufferFull= 0; //clear the buffer full flag


        // Software breakpoint. At this point, conversion results are stored in
        // adcAResults.
        //
        // Hit run again to get updated conversions.
        //
        //ESTOP0;
    }

    //
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

//
// init_EPWM1 - Function to configure ePWM1 to generate the SOC.
//
void init_EPWM1(void)
{
    EALLOW;

    EPwm1Regs.ETSEL.bit.SOCAEN = 0;     // Disable SOC on A group
    EPwm1Regs.ETSEL.bit.SOCASEL = 4;    // Select SOC on up-count
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;     // Generate pulse on 1st event

    EPwm1Regs.CMPA.bit.CMPA = CMPA_;   // Set compare A value to 2048 counts
    EPwm1Regs.TBPRD = PERIODE_10u;           // Set period to 4096 counts

    EPwm1Regs.TBCTL.bit.CTRMODE = 3;    // Freeze counter

    EDIS;
}



//
// init_EPWM2 - Function to configure ePWM1 to generate the SOC.
//
void init_EPWM2(void)
{
    EALLOW;

    EPwm2Regs.ETSEL.bit.SOCAEN = 0;     // Disable SOC on A group
    EPwm2Regs.ETSEL.bit.SOCASEL = 4;    // Select SOC on up-count
    EPwm2Regs.ETPS.bit.SOCAPRD = 1;     // Generate pulse on 1st event

    EPwm2Regs.CMPA.bit.CMPA = CMPA_;   // Set compare A value to 2048 counts
    EPwm2Regs.TBPRD = PERIODE_10u;           // Set period to 4096 counts

    EPwm2Regs.TBCTL.bit.CTRMODE = 3;    // Freeze counter

    EDIS;
}


//
// init_EPWM4 - Function to configure ePWM1 to generate the SOC.
//
void init_EPWM4(void)
{
    EALLOW;

    EPwm4Regs.ETSEL.bit.SOCAEN = 0;     // Disable SOC on A group
    EPwm4Regs.ETSEL.bit.SOCASEL = 4;    // Select SOC on up-count
    EPwm4Regs.ETPS.bit.SOCAPRD = 1;     // Generate pulse on 1st event

    EPwm4Regs.CMPA.bit.CMPA = CMPA_;   // Set compare A value to 2048 counts
    EPwm4Regs.TBPRD = PERIODE_10u;           // Set period to 4096 counts

    EPwm4Regs.TBCTL.bit.CTRMODE = 3;    // Freeze counter

    EDIS;
}
void start_EPWM1(void){

    //
    // Start ePWM
    //
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;    // Enable SOCA
    EPwm1Regs.TBCTL.bit.CTRMODE = 0;   // Unfreeze, and enter up count mode
}


void stop_EPWM1(void){

    //
    // Start ePWM
    //
    EPwm1Regs.ETSEL.bit.SOCAEN = 0 ;    // Disable SOCA
    EPwm1Regs.TBCTL.bit.CTRMODE = 3;   // Unfreeze, and enter up count mode
}


void start_EPWM2(void){

    //
    // Start ePWM
    //
    EPwm2Regs.ETSEL.bit.SOCAEN = 1;    // Enable SOCA
    EPwm2Regs.TBCTL.bit.CTRMODE = 0;   // Unfreeze, and enter up count mode
}


void stop_EPWM2(void){

    //
    // Start ePWM
    //
    EPwm2Regs.ETSEL.bit.SOCAEN = 0;    // Disable SOCA
    EPwm2Regs.TBCTL.bit.CTRMODE = 3;   // Unfreeze, and enter up count mode
}


void start_EPWM4(void){

    //
    // Start ePWM
    //
    EPwm4Regs.ETSEL.bit.SOCAEN = 1;    // Enable SOCA
    EPwm4Regs.TBCTL.bit.CTRMODE = 0;   // Unfreeze, and enter up count mode
}


void stop_EPWM4(void){

    //
    // Start ePWM
    //
    EPwm4Regs.ETSEL.bit.SOCAEN = 0;    // Disable SOCA
    EPwm4Regs.TBCTL.bit.CTRMODE = 3;   // Unfreeze, and enter up count mode
}



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
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5;   // Trigger on ePWM1 SOCA

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
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 7;   // Trigger on ePWM2 SOCA

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
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 11;   // Trigger on ePWM4 SOCA

    AdcaRegs.ADCINTSEL3N4.bit.INT3SEL = 2; // End of SOC2 will set INT3 flag
    AdcaRegs.ADCINTSEL3N4.bit.INT3E = 1;   // Enable INT3 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; // Make sure INT3 flag is cleared

    EDIS;
}

//
// adcA1ISR - ADC A Interrupt 1 ISR
//
__interrupt void adcA1ISR(void)
{
    //
    // Add the latest result to the buffer
    // ADCRESULT0 is the result register of SOC0
    array_IN_ADC_500VAC[index_1++] = AdcaResultRegs.ADCRESULT0;

    //
    // Set the bufferFull flag if the buffer is full
    //
    if(RESULTS_BUFFER_SIZE <= index_1)
    {
        index_1 = 0;
        array_IN_ADC_500VAC_bufferFull = 1;
        //stop_EPWM1();
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
// adcA1ISR - ADC A Interrupt 1 ISR
//
__interrupt void adcA2ISR(void)
{
    //
    // Add the latest result to the buffer
    // ADCRESULT0 is the result register of SOC0
    array_IN_CP_ADC[index_2++] = AdcaResultRegs.ADCRESULT1;

    //
    // Set the bufferFull flag if the buffer is full
    //
    if(RESULTS_BUFFER_SIZE <= index_2)

        // Enter function here
        // Stop EPWM
        // call the function
        // Start the EPWM

    {
        index_2 = 0;
        array_IN_CP_ADC_bufferFull = 1;
    }

    //
    // Clear the interrupt flag
    //
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;

    //
    // Check if overflow has occurred
    //
    if(1 == AdcaRegs.ADCINTOVF.bit.ADCINT2)
    {
        AdcaRegs.ADCINTOVFCLR.bit.ADCINT2 = 1; //clear INT3 overflow flag
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear INT3 flag
    }

    //
    // Acknowledge the interrupt
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

}

//
// adcA3ISR - ADC A Interrupt 1 ISR
//
__interrupt void adcA3ISR(void)
{
    //
    // Add the latest result to the buffer
    // ADCRESULT0 is the result register of SOC0
    array_IN_CP_BORNE[index_3++] = AdcaResultRegs.ADCRESULT2;

    //
    // Set the bufferFull flag if the buffer is full
    //
    if(RESULTS_BUFFER_SIZE <= index_3)
    {
        index_3 = 0;
        array_IN_CP_BORNE_ADC_bufferFull = 1;
    }

    //
    // Clear the interrupt flag
    //
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1;

    //
    // Check if overflow has occurred
    //
    if(1 == AdcaRegs.ADCINTOVF.bit.ADCINT3)
    {
        AdcaRegs.ADCINTOVFCLR.bit.ADCINT3 = 1; //clear INT3 overflow flag
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //clear INT3 flag
    }

    //
    // Acknowledge the interrupt
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

}

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
//
// End of File
//

void Configure_GPIO(void){
    // Sélection des niveaux de tensions CP
    GPIO_SetupPinMux(24U, GPIO_MUX_CPU1, 0); // OUT_CP_9V
    GPIO_SetupPinOptions(24U, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(16U, GPIO_MUX_CPU1, 0); // OUT_CP_6V
    GPIO_SetupPinOptions(16U, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(33U, GPIO_MUX_CPU1, 0); // OUT_CP_3V3
    GPIO_SetupPinOptions(33U, GPIO_OUTPUT, GPIO_PUSHPULL);

    // Sélection des niveau de résistances

    GPIO_SetupPinMux(7U, GPIO_MUX_CPU1, 0); // OUT_PP_680R
    GPIO_SetupPinOptions(7U, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(0U, GPIO_MUX_CPU1, 0); // OUT_PP_220R
    GPIO_SetupPinOptions(0U, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(1U, GPIO_MUX_CPU1, 0); // OUT_PP_1500R
    GPIO_SetupPinOptions(1U, GPIO_OUTPUT, GPIO_PUSHPULL);

    // CP enabled for 12 V level adaptator and CP modified

    GPIO_SetupPinMux(03U, GPIO_MUX_CPU1, 0); // CP enabled
    GPIO_SetupPinOptions(03U, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(04U, GPIO_MUX_CPU1, 0); // CP_modifie
    GPIO_SetupPinOptions(04U, GPIO_OUTPUT, GPIO_PUSHPULL);
}

void select_CP_9V(void){

    GpioDataRegs.GPADAT.bit.GPIO16 = 0; // CP_6V
    GpioDataRegs.GPBDAT.bit.GPIO33 = 0; // CP_3V3

    //while(GpioDataRegs.GPADAT.bit.GPIO16 == 0 && GpioDataRegs.GPBDAT.bit.GPIO33 == 0 )
    GpioDataRegs.GPADAT.bit.GPIO24 = 1;


}

void select_CP_6V(void){

    GpioDataRegs.GPADAT.bit.GPIO24 = 0; // CP_9V
    GpioDataRegs.GPBDAT.bit.GPIO33 = 0; // CP_3V3

    //while(GpioDataRegs.GPADAT.bit.GPIO24 == 0 && GpioDataRegs.GPBDAT.bit.GPIO33 == 0 )
    GpioDataRegs.GPADAT.bit.GPIO16 = 1;


}

void select_CP_3V3(void){

    GpioDataRegs.GPADAT.bit.GPIO16 = 0; // CP_6V
    GpioDataRegs.GPADAT.bit.GPIO24 = 0; // CP_9V

    //while(!GpioDataRegs.GPADAT.bit.GPIO24 == 0 && !GpioDataRegs.GPADAT.bit.GPIO16 == 0 )
    GpioDataRegs.GPBDAT.bit.GPIO33 = 1;


}
