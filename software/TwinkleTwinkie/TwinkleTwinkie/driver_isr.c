/**
 * \file
 *
 * \brief Driver ISR.
 *
 (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/*
 * Code generated by START.
 *
 * This file will be overwritten when reconfiguring your START project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include <driver_init.h>
#include <compiler.h>

volatile uint8_t LED_PWM[3] = { 0 };
volatile uint16_t timeTicks = 0;

ISR(RTC_CNT_vect)
{

	/* Insert your RTC Compare interrupt handling code */
	touch_timer_handler();

	/* Compare interrupt flag has to be cleared manually */
	RTC.INTFLAGS = RTC_CMP_bm;
}

ISR(TCA0_OVF_vect){
	
	static uint8_t counter = 0;

	if(counter == 0){
		// Set all LEDs high
		LED1_set_level(true);
		LED2_set_level(true);
		LED3_set_level(true);
	}

	// Determine if each LED needs to turn off
	if(LED_PWM[0] <= counter){
		LED1_set_level(false);
	}
	if(LED_PWM[1] <= counter){
		LED2_set_level(false);
	}
	if(LED_PWM[2] <= counter){
		LED3_set_level(false);
	}
	
	counter++;

	/* The interrupt flag has to be cleared manually */
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

ISR(TCA0_CMP0_vect){

	/* The interrupt flag has to be cleared manually */
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm;
}

