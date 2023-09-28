/*
 * MIT License
 * Copyright (c) 2023 Francesco Cavina <francescocavina98@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or, sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of he Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAS PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY. WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE
 */

/*
 * @file:    test_FSA8S_driver_UAI.c
 * @date:    26/09/2023
 * @author:  Francesco Cavina <francescocavina98@gmail.com>
 * @version: v1.0.0
 *
 * @brief:   This is the unit test of the file "FS-A8S_driver_UAI.c" which is part of the driver of
 *           the radio control receiver (FlySky model FS-A8S).
 */

/* --- Headers files inclusions ---------------------------------------------------------------- */
#include "unity.h"
#include "FS_A8S_driver_UAI.h"
#include "mock_FS_A8S_driver_HWI.h"

/* --- Macros definitions ---------------------------------------------------------------------- */

/* --- Private data type declarations ---------------------------------------------------------- */

/* --- Private variable declarations ----------------------------------------------------------- */
static UART_HandleTypeDef huart1;      // UART handle structure for UART1
static UART_HandleTypeDef huart2;      // UART handle structure for UART2
IBUS_HandleTypeDef_t * rc_controller1; // IBUS handle for device 1
IBUS_HandleTypeDef_t * rc_controller2; // IBUS handle for device 2
uint8_t data[32] = {
    0x20, 0x40, 0xDB, 0x05, 0xDC, 0x05, 0x54, 0x05, 0xDC, 0x05, 0xE8,
    0x03, 0xD0, 0x07, 0xD2, 0x05, 0xE8, 0x03, 0xDC, 0x05, 0xDC, 0x05,
    0xDC, 0x05, 0xDC, 0x05, 0xDC, 0x05, 0xDC, 0x05, 0xDA, 0xF3}; // Simulated received data

/* --- Private function declarations ----------------------------------------------------------- */

/* --- Public variable definitions ------------------------------------------------------------- */

/* --- Private variable definitions ------------------------------------------------------------ */

/* --- Private function implementation --------------------------------------------------------- */
/*
 * @brief  This is a mock to replace the original function defined in HWI file.
 * @param  iBus handle structure
 * @retval true: always
 */
bool_t auxiliar_IBUS_Init(IBUS_HandleTypeDef_t * hibus) {
    return true;
}

/* --- Public function implementation ---------------------------------------------------------- */
void setUp(void) {

    /* Define mock for external function */
    IBUS_Init_fake.custom_fake = auxiliar_IBUS_Init;
}

/*
 * TEST CASE NO. 1: Initialize one device with wrong UART handle.
 */
void test_initialize_one_device_with_wrong_uart_handle(void) {

    /* Initialize device */
    rc_controller1 = FSA8S_RC_Init(NULL);

    /* Test that device couldn't be initialized */
    TEST_ASSERT_NULL(rc_controller1);
}

/*
 * TEST CASE NO. 2: Initialize first device with proper UART handle.
 * Note: Driver allows only one initialization.
 */
void test_initialize_first_device_with_proper_uart_handle(void) {

    /* Initialize first device */
    rc_controller1 = FSA8S_RC_Init(&huart1);

    /* Test if first device was correctly initialized */
    TEST_ASSERT_NOT_NULL(rc_controller1);
}

/*
 * TEST CASE NO. 3: Initialize second device with proper UART handle.
 * Note: Driver allows only one initialization.
 */
void test_initialize_second_device_with_proper_uart_handle(void) {

    /* Initialize second device */
    rc_controller2 = FSA8S_RC_Init(&huart2);

    /* Test if second device couldn't be initialized */
    TEST_ASSERT_NULL(rc_controller2);
}

/* TEST CASE NO. 4: Read Channel 1 and 14 with proper iBus handle.
 * Note: Internal buffer is filled automatically through Direct Memory Access (DMA) with a circular
 *       FIFO. Therefore, in order to simulate the DMA, buffer is filled inside the test function.
 */
void test_read_channel_1_and_14_with_proper_ibus_handle(void) {

    /* Define variables for channels numbers */
    uint8_t channel1 = CHANNEL_1;
    uint8_t channel14 = CHANNEL_14;

    /* Define variables for channels values */
    uint16_t channelValue1;
    uint16_t channelValue14;

    /* Simulate data reception through DMA */
    for (uint8_t i = 0; i < 32; i++) {
        *((rc_controller1->buffer) + i) = data[i];
    }

    /* Read channel 1 */
    channelValue1 = FSA8S_RC_ReadChannel(rc_controller1, channel1);

    /* Read channel 14*/
    channelValue14 = FSA8S_RC_ReadChannel(rc_controller1, channel14);

    /* Test if channel values are correct */
    TEST_ASSERT_EQUAL(499, channelValue1);
    TEST_ASSERT_EQUAL(500, channelValue14);
}

/* TEST CASE NO. 5: Read Channel 0 and 15 (out of limits) with proper iBus handle.
 * Note: Internal buffer is filled automatically through Direct Memory Access (DMA) with a circular
 *       FIFO. Therefore, in order to simulate the DMA, buffer is filled inside the test function.
 */
void test_read_channel_0_and_15_with_proper_ibus_handle(void) {

    /* Define variables for channels numbers */
    uint8_t channel0 = 0;
    uint8_t channel15 = 15;

    /* Define variables for channels values */
    uint16_t channelValue0;
    uint16_t channelValue15;

    /* Simulate data reception through DMA */
    for (uint8_t i = 0; i < 32; i++) {
        *((rc_controller1->buffer) + i) = data[i];
    }

    /* Read channel 0 */
    channelValue0 = FSA8S_RC_ReadChannel(rc_controller1, channel0);

    /* Read channel 15 */
    channelValue15 = FSA8S_RC_ReadChannel(rc_controller1, channel15);

    /* Test if channels values are 0 */
    TEST_ASSERT_EQUAL(0, channelValue0);
    TEST_ASSERT_EQUAL(0, channelValue15);
}

/* TEST CASE NO. 6: Read Channel 1 with wrong iBus handle.
 * Note: Internal buffer is filled automatically through Direct Memory Access (DMA) with a circular
 *       FIFO. Therefore, in order to simulate the DMA, buffer is filled inside the test function.
 */
void test_read_channel_1_with_wrong_ibus_handle(void) {

    /* Define variable for channel number */
    uint8_t channel = CHANNEL_1;

    /* Define variable for channel value */
    uint16_t channelValue;

    /* Simulate data reception through DMA */
    for (uint8_t i = 0; i < 32; i++) {
        *((rc_controller1->buffer) + i) = data[i];
    }

    /* Read channel 1 */
    channelValue = FSA8S_RC_ReadChannel(NULL, channel);

    /* Test if channel value is 0 */
    TEST_ASSERT_EQUAL(0, channelValue);
}

/* --- End of file ----------------------------------------------------------------------------- */
