//#############################################################################
//
// FILE: GPIO_testing.c
//
// TITLE: GPIO Testing Implementation for F2800132
//
// DESCRIPTION:
// This file implements functions for initializing the device, configuring
// GPIO pins, and testing their outputs.
//
// AUTHOR: [Kenneth TANFA ]
//
//#############################################################################

//
// Included Files
//
#include "f28x_project.h"
#include "Test_GPIO.h"

//
// Function Prototypes
//

//
// Main
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

    while(!GpioDataRegs.GPADAT.bit.GPIO16 == 0 && !GpioDataRegs.GPBDAT.bit.GPIO33 == 0 )
    GpioDataRegs.GPADAT.bit.GPIO24 = 1;


}

void select_CP_6V(void){

    GpioDataRegs.GPADAT.bit.GPIO24 = 0; // CP_9V
    GpioDataRegs.GPBDAT.bit.GPIO33 = 0; // CP_3V3

    while(!GpioDataRegs.GPADAT.bit.GPIO24 == 0 && !GpioDataRegs.GPBDAT.bit.GPIO33 == 0 )
    GpioDataRegs.GPADAT.bit.GPIO16 = 1;


}

void select_CP_3V3(void){

    GpioDataRegs.GPADAT.bit.GPIO16 = 0; // CP_6V
    GpioDataRegs.GPADAT.bit.GPIO24 = 0; // CP_9V

    while(!GpioDataRegs.GPADAT.bit.GPIO24 == 0 && !GpioDataRegs.GPADAT.bit.GPIO16 == 0 )
    GpioDataRegs.GPBDAT.bit.GPIO33 = 1;


}

// Select resistance
