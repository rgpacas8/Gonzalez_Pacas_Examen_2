/**
	\file
	\brief
		This is the source file for the GPIO device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
		i.e., this is the application programming interface (API) for the GPIO peripheral.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	18/02/2019
	    Interrupts are not implemented in this API implementation.
 */

#include "MK64F12.h"
#include "GPIO.h"
#include "bits.h"
#include <stdio.h>	// DEBUG teclado matricial

static void (*gpio_B_callback)  (void) = 0; // teclado


static gpio_interrupt_flags_t g_intr_status_flag = {0};

/*! This variable reads the full port	  */
uint32_t port_readValue;

/*! This variable reads the specific pin  */
uint8_t pin_readValue;

void GPIO_callback_init(gpio_port_name_t port_name,void (*handler)(void))
{
	if(GPIO_B == port_name)
	{
		if(gpio_B_callback == 0)
			gpio_B_callback = handler;	// Inicializa función del Teclado
	}
}

uint8_t GPIO_clock_gating(gpio_port_name_t port_name)	// f(x) #2		done jlpe! + good perform
{
	switch (port_name)/** Selecting the GPIO for clock enabling*/
	{
	case GPIO_A: /** GPIO A is selected*/
		SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTA; /** Bit 9 of SIM_SCGC5 is  set*/
		break;
	case GPIO_B: /** GPIO B is selected*/
		SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTB; /** Bit 10 of SIM_SCGC5 is set*/
		break;
	case GPIO_C: /** GPIO C is selected*/
		SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTC; /** Bit 11 of SIM_SCGC5 is set*/
		break;
	case GPIO_D: /** GPIO D is selected*/
		SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTD; /** Bit 12 of SIM_SCGC5 is set*/
		break;
	case GPIO_E: /** GPIO E is selected*/
		SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTE; /** Bit 13 of SIM_SCGC5 is set*/
		break;
	default: /**If doesn't exist the option*/
		return (FALSE);
	}		// end switch
	/**Successful configuration*/
	return (TRUE);
}		// end function

														// f(x) #3		done jlpe! + good perform
uint8_t GPIO_pin_control_register(gpio_port_name_t port_name, uint8_t pin,
		const gpio_pin_control_register_t* pin_control_register) {
	switch (port_name) {
	case GPIO_A:/** GPIO A is selected*/
		PORTA->PCR[pin] = *pin_control_register;
		break;
	case GPIO_B:/** GPIO B is selected*/
		PORTB->PCR[pin] = *pin_control_register;
		break;
	case GPIO_C:/** GPIO C is selected*/
		PORTC->PCR[pin] = *pin_control_register;
		break;
	case GPIO_D:/** GPIO D is selected*/
		PORTD->PCR[pin] = *pin_control_register;
		break;
	case GPIO_E: /** GPIO E is selected*/
		PORTE->PCR[pin] = *pin_control_register;
	default:/**If doesn't exist the option*/
		return (FALSE);
		break;
	}
	/**Successful configuration*/
	return (TRUE);
}

void PORTA_IRQHandler(void)
{
	g_intr_status_flag.flag_port_a = TRUE; // bandera de SW de SW3
	GPIO_clear_interrupt(GPIO_A);
}
void PORTB_IRQHandler(void)
{
	if(gpio_B_callback)
	{
		gpio_B_callback();	// Función del teclado
	}

	GPIO_clear_interrupt(GPIO_B);
}

void PORTC_IRQHandler(void) {

		g_intr_status_flag.flag_port_c = TRUE;	// bandera de SW de SW2
		GPIO_clear_interrupt(GPIO_C);
}
void GPIO_clear_interrupt(gpio_port_name_t port_name)
{
	switch(port_name)/** Selecting the GPIO for clock enabling*/
	{
		case GPIO_A: /** GPIO A is selected*/
			PORTA->ISFR=0xFFFFFFFF;
			break;
		case GPIO_B: /** GPIO B is selected*/
			PORTB->ISFR=0xFFFFFFFF;
			break;
		case GPIO_C: /** GPIO C is selected*/
			PORTC->ISFR = 0xFFFFFFFF;
			break;
		case GPIO_D: /** GPIO D is selected*/
			PORTD->ISFR=0xFFFFFFFF;
			break;
		default: /** GPIO E is selected*/
			PORTE->ISFR=0xFFFFFFFF;
			break;

	}// end switch
}

uint8_t GPIO_get_irq_status(gpio_port_name_t gpio)	// flag SW
{
	uint8_t status = 0;

	if (GPIO_A == gpio) {
		status = g_intr_status_flag.flag_port_a;
	} else {
		status = g_intr_status_flag.flag_port_c;
	}

	return (status);
}

void GPIO_clear_irq_status(gpio_port_name_t gpio)	// flag SW
{
	if(GPIO_A == gpio)
	{
		g_intr_status_flag.flag_port_a = FALSE;
	}
	else
	{
		g_intr_status_flag.flag_port_c = FALSE;
	}
}

void GPIO_data_direction_port(gpio_port_name_t port_name, gpio_port_direction_t direction)		   // f(x) #4	done!
{
	switch (port_name) {
	case GPIO_A:/** GPIO A is selected*/
		GPIOA->PDDR = (direction == 1) ? 0xFFFFFFFF : GPIO_INPUT;
		break;
	case GPIO_B:/** GPIO B is selected*/
		GPIOB->PDDR = (direction == 1) ? 0xFFFFFFFF : GPIO_INPUT;
		break;
	case GPIO_C:/** GPIO C is selected*/
		GPIOC->PDDR = (direction == 1) ? 0xFFFFFFFF : GPIO_INPUT;
		break;
	case GPIO_D:/** GPIO D is selected*/
		GPIOD->PDDR = (direction == 1) ? 0xFFFFFFFF : GPIO_INPUT;
		break;
	case GPIO_E: /** GPIO E is selected*/
		GPIOE->PDDR = (direction == 1) ? 0xFFFFFFFF : GPIO_INPUT;
		break;
	default:/**If doesn't exist the option*/

		break;
	}
}
void GPIO_data_direction_pin (gpio_port_name_t port_name, gpio_port_direction_t state, uint8_t pin)// f(x) #5	done + good perform!
{
	switch (port_name) {
	case GPIO_A:/** GPIO A is selected*/
		GPIOA->PDDR |= (state == 1) ? (1 << pin) : (0 << pin);
		break;
	case GPIO_B:/** GPIO B is selected*/
		GPIOB->PDDR |= (state == 1) ? (1 << pin) : (0 << pin);
		break;
	case GPIO_C:/** GPIO C is selected*/
		GPIOC->PDDR |= (state == 1) ? (1 << pin) : (0 << pin);
		break;
	case GPIO_D:/** GPIO D is selected*/
		GPIOD->PDDR |= (state == 1) ? (1 << pin) : (0 << pin);
		break;
	case GPIO_E: /** GPIO E is selected*/
		GPIOE->PDDR |= (state == 1) ? (1 << pin) : (0 << pin);
	default:/**If doesn't exist the option*/

		break;
	}
}
void GPIO_write_port(gpio_port_name_t portName, uint32_t data);// f(x) #6
uint32_t GPIO_read_port(gpio_port_name_t port_name)			   //(f(x) #7	done!
{
	switch (port_name) {
	case GPIO_A:/** GPIO A is selected*/
		port_readValue = GPIOA->PDIR;		 // Lee completo el GPIOA
		break;
	case GPIO_B:/** GPIO B is selected*/
		port_readValue = GPIOB->PDIR;		 // Lee completo el GPIOB
		break;
	case GPIO_C:/** GPIO C is selected*/
		port_readValue = GPIOC->PDIR;		 // Lee completo el GPIOC
		break;
	case GPIO_D:/** GPIO D is selected*/
		port_readValue = GPIOD->PDIR;		 // Lee completo el GPIOD
		break;
	case GPIO_E: /** GPIO E is selected*/
		port_readValue = GPIOE->PDIR;		 // Lee completo el GPIOE
	default:/**If doesn't exist the option*/

		break;
	}// end switch
	/**Successful configuration*/
	return (port_readValue);
}// end function
void GPIO_set_pin(gpio_port_name_t port_name, uint8_t pin)	   // f(x) #8	done! + good perform
{
	switch (port_name) {
	case GPIO_A:
		GPIOA->PSOR = (1<<pin);
		break;
	case GPIO_B:/** GPIO B is selected*/
		GPIOB->PSOR = (1<<pin);

		break;
	case GPIO_C:/** GPIO C is selected*/
		GPIOC->PSOR = (1<<pin);
		break;
	case GPIO_D:/** GPIO D is selected*/
		GPIOD->PSOR = (1<<pin);
		break;
	case GPIO_E:/** GPIO E is selected*/
		GPIOE->PSOR = (1<<pin);
	default:/**If doesn't exist the option*/

		break;
	}
}
uint8_t GPIO_read_pin(gpio_port_name_t port_name, uint8_t pin) // f(x) #9	done!
{
	switch (port_name) {
		case GPIO_A:/** GPIO A is selected*/
			pin_readValue = GPIOA->PDIR;		 // Lee solo el pin de interés
			break;
		case GPIO_B:/** GPIO B is selected*/
			pin_readValue = GPIOB->PDIR;		 // Lee solo el pin de interés
			break;
		case GPIO_C:/** GPIO C is selected*/
			pin_readValue = GPIOC->PDIR;		 // Lee solo el pin de interés
			break;
		case GPIO_D:/** GPIO D is selected*/
			pin_readValue = GPIOD->PDIR;		 // Lee solo el pin de interés
			break;
		case GPIO_E: /** GPIO E is selected*/
			pin_readValue = GPIOE->PDIR;		 // Lee solo el pin de interés
			break;
		default:/**If doesn't exist the option*/

			break;
		}// end switch
		/**Successful configuration*/
		return (pin_readValue & (1 << pin));	 // Mask for read the especific pin
}// end fuction
void GPIO_clear_pin(gpio_port_name_t port_name, uint8_t pin)   // f(x) #10	done! + good perform
{
	switch (port_name) {
	case GPIO_A:
		GPIOA->PCOR = (1<<pin);
		break;
	case GPIO_B:/* GPIO B is selected*/
		GPIOB->PCOR = (1<<pin);
		break;
	case GPIO_C:/* GPIO C is selected*/
		GPIOC->PCOR = (1<<pin);
		break;
	case GPIO_D:/* GPIO D is selected*/
		GPIOD->PCOR = (1<<pin);
		break;
	case GPIO_E: /* GPIO E is selected*/
		GPIOE->PCOR = (1<<pin); // ON - GREEN pin26.
	default:/*If doesn't exist the option*/

		break;
	}
}
void GPIO_toogle_pin(gpio_port_name_t port_name, uint8_t pin)  // f(x) #11	done!
{
	switch (port_name) {
	case GPIO_A:
		GPIOA->PTOR = (1 << pin);
		break;
	case GPIO_B:/** GPIO B is selected*/
		GPIOB->PTOR = (1 << pin);

		break;
	case GPIO_C:/** GPIO C is selected*/
		GPIOC->PTOR = (1 << pin);
		break;
	case GPIO_D:/** GPIO D is selected*/
		GPIOD->PTOR = (1 << pin);
		break;
	case GPIO_E:/** GPIO E is selected*/
		GPIOE->PTOR = (1 << pin);
	default:/**If doesn't exist the option*/

		break;
	}

}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

