/*
 * PIT.c
 *
 *  Created on: 16 sep 2019
 *      Author: Lanix
 */

#include "MK64F12.h"
#include "PIT.h"
#include "Bits.h"
#define SYSTEM_CLOCK (21000000U)
#define DELAY (0.00050)			// (0.01F)
								//  0.025	seg
								//  0.01785	seg
								//  0.00050	seg

static void (*PIT_0_callback)(void) = 0;	// Inicializamos apuntadores a función
static void (*PIT_1_callback)(void) = 0;
static void (*PIT_2_callback)(void) = 0;
static void (*PIT_3_callback)(void) = 0;

static PIT_interrupt_flags_t g_intrPIT_status_flag = {0};

void PIT_callback_init(PIT_timer_t pit_timer, void (*handler)(void)) {
	if (PIT_0 == pit_timer) {
		PIT_0_callback = handler;
	}
	if (PIT_1 == pit_timer) {
		PIT_1_callback = handler;
	}
	if (PIT_2 == pit_timer) {
		PIT_2_callback = handler;
	}
	if (PIT_3 == pit_timer) {
		PIT_3_callback = handler;
	}
}
/*
void PIT0_IRQHandler(void)			// ESTE ES MI VECTOR DE INTERRUPCIÓN
{
	if (PIT_0_callback) {	// Si ya se inicializó el apuntador a función
		PIT_0_callback( );	// con el Handler del CallBack... LLAMADA A FUNCIÓN: PIT_delay();
	}

	PIT_clear_interrupt(PIT_0);				// Apago interrupción por HW
}
*/

void PIT0_IRQHandler(void)							// ESTE ES MI VECTOR DE INTERRUPCIÓN
{
	g_intrPIT_status_flag.flag_PIT_channel_0 = TRUE;// Enciendo bandera por Software
	PIT_clear_interrupt(PIT_0);				// Apago interrupción por HW
}

void PIT1_IRQHandler(void)
{
	g_intrPIT_status_flag.flag_PIT_channel_1 = TRUE;// Enciendo bandera por Software
	PIT_clear_interrupt(PIT_1);				// Apago interrupción por HW
}

void PIT2_IRQHandler(void)
{
	g_intrPIT_status_flag.flag_PIT_channel_2 = TRUE;// Enciendo bandera por Software
	PIT_clear_interrupt(PIT_2);				// Apago interrupción por HW
}

void PIT_clock_gating(void)
{
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;	// Set bit 23 - enable module PIT
}

void PIT_enable(void)
{
	PIT->MCR &= ~(0x02);	//PIT_MCR_MDIS_MASK;		// bit 1 - MDIS enable PIT section
}

void FRZ_enable(void)
{
	PIT->MCR |= PIT_MCR_FRZ_MASK;		// bit 0 - FRZ enable Mode Debug
}

void PIT_delayInteger(PIT_timer_t pit_timer, uint32_t system_clock,uint32_t delay)
{

	My_float_pit_t clock_PIT;		/*! This variable hold the PIT clock	  	*/
	My_float_pit_t period_PIT;		/*! This variable hold the PIT period	  	*/
	uint32_t cycles_number;			/*! This variable is the cycles to spend	*/

	clock_PIT = system_clock / 2;
	period_PIT = (My_float_pit_t)(1 / clock_PIT);

	cycles_number = (int)(delay / period_PIT);
	PIT->CHANNEL[pit_timer].LDVAL = cycles_number - 1; /** Load of number of cycles */
	PIT->CHANNEL[pit_timer].TCTRL |= PIT_TCTRL_TIE_MASK;// set TIE - enable interrupts Timer
	PIT->CHANNEL[pit_timer].TCTRL |= PIT_TCTRL_TEN_MASK;// set TEN - start Timer
}

void PIT_delayFloat(PIT_timer_t pit_timer, uint32_t system_clock, My_float_pit_t delay)
{

	My_float_pit_t clock_PIT;		/*! This variable hold the PIT clock	  	*/
	My_float_pit_t period_PIT;		/*! This variable hold the PIT period	  	*/
	uint32_t cycles_number;			/*! This variable is the cycles to spend	*/

	clock_PIT = system_clock / 2;
	period_PIT = (My_float_pit_t)(1 / clock_PIT);

	cycles_number = (int)(delay / period_PIT);
	PIT->CHANNEL[pit_timer].LDVAL = cycles_number - 1; /** Load of number of cycles */
	PIT->CHANNEL[pit_timer].TCTRL |= PIT_TCTRL_TIE_MASK;// set TIE - enable interrupts Timer
	PIT->CHANNEL[pit_timer].TCTRL |= PIT_TCTRL_TEN_MASK;// set TEN - start Timer
}

void PIT_stop(PIT_timer_t pit_timer)
{
	if (PIT_0 == pit_timer) {
		PIT->CHANNEL[pit_timer].TCTRL &= ~(PIT_TCTRL_TIE_MASK); /** Apaga bit TIE y se apagan interrupciones */
		PIT->CHANNEL[pit_timer].TCTRL &= ~(PIT_TCTRL_TEN_MASK); /** Apaga bit TEN y se deshabilita el timer  */
		}
		if (PIT_1 == pit_timer) {
		PIT->CHANNEL[pit_timer].TCTRL &= ~(PIT_TCTRL_TIE_MASK);
		PIT->CHANNEL[pit_timer].TCTRL &= ~(PIT_TCTRL_TEN_MASK);

		}
		if (PIT_2 == pit_timer) {
		PIT->CHANNEL[pit_timer].TCTRL &= ~(PIT_TCTRL_TIE_MASK);
		PIT->CHANNEL[pit_timer].TCTRL &= ~(PIT_TCTRL_TEN_MASK);

		}
		if (PIT_3 == pit_timer) {
		PIT->CHANNEL[pit_timer].TCTRL &= ~(PIT_TCTRL_TIE_MASK);
		PIT->CHANNEL[pit_timer].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
		}
}
uint8_t PIT_get_irq_flag_status(PIT_timer_t pit_timer)	// OBTENGO SOFTWARE FLAG
{
	uint8_t status = 0;

		// Esta es Software
	if (PIT_0 == pit_timer) {
		status = g_intrPIT_status_flag.flag_PIT_channel_0;
	}
	if (PIT_1 == pit_timer) {
		status = g_intrPIT_status_flag.flag_PIT_channel_1;
	}
	if (PIT_2 == pit_timer) {
		status = g_intrPIT_status_flag.flag_PIT_channel_2;
	}
	if (PIT_3 == pit_timer) {
		status = g_intrPIT_status_flag.flag_PIT_channel_3;
	}

	return (status);
}

void PIT_clear_irq_flag_status(PIT_timer_t pit_timer)	// LIMPIO FLAG SOFTWARE
{
	if (PIT_0 == pit_timer) {
		g_intrPIT_status_flag.flag_PIT_channel_0 = FALSE;		// Apagamos la bandera de HW
	}
	if (PIT_1 == pit_timer) {
		g_intrPIT_status_flag.flag_PIT_channel_1 = FALSE;
	}
	if (PIT_2 == pit_timer) {
		g_intrPIT_status_flag.flag_PIT_channel_2 = FALSE;
	}
	if (PIT_3 == pit_timer) {
		g_intrPIT_status_flag.flag_PIT_channel_3 = FALSE;
	}
}

void PIT_clear_interrupt(PIT_timer_t pit_timer)	// HARDWARE FLAG
{
	PIT->CHANNEL[pit_timer].TFLG = TRUE;	// Borramos bandera de Hardware
}
void PIT_init(void){
	PIT_clock_gating();		// Habilita modulo PIT
	PIT_enable();			// Habilita PIT Timers
	//PIT_delay(pit_timer, SYSTEM_CLOCK, DELAY);
}
