/*
 * @file:			UART.c
 * @company:			ITESO
 * @Engineer Team:	D.F.R. / R.G.P.
 */

#include "UART_driver.h"

 uart_mail_box_t g_mail_box_uart_0;

void UART0_RX_TX_IRQHandler( )
{
	while (!(UART0->S1 & UART_S1_RDRF_MASK))
	{
		// Codigo Bloqueante
	}
	g_mail_box_uart_0.mailBox = UART0->D;
	g_mail_box_uart_0.flag = 1;
}

void UART_init(uart_channel_t uart_channel, uint32_t system_clk, uart_baud_rate_t baud_rate)
{
	if (UART_0 == uart_channel) {
		SIM->SCGC4 |= 0x0400;
		UART0->C2  &= ~(UART_C2_TE_MASK);
		UART0->C2  &= ~(UART_C2_RE_MASK);
		UART0->BDH |= 0x00;
		UART0->BDL  = 0x0B; //seleccion de baudrate con el uso de las tablas
		UART0->C4 |= 0x0D;
		UART0->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK; //encendido para transmitir y recibir
	}
	if (UART_1 == uart_channel) {}
	if (UART_2 == uart_channel) {}
	if (UART_3 == uart_channel) {}
	if (UART_4 == uart_channel) {}
}

void UART_interrupt_enable(uart_channel_t uart_channel)
{
	if (UART_0 == uart_channel) {
		UART0->C2 |= UART_C2_RIE_MASK;//habilitacion de interrupcion
	}
	if (UART_1 == uart_channel) {}
	if (UART_2 == uart_channel) {}
	if (UART_3 == uart_channel) {}
	if (UART_4 == uart_channel) {}
}

void UART_put_char (uart_channel_t uart_channel, uint8_t character)
{
	if (UART_0 == uart_channel) {
		while (!(UART0->S1 & UART_S1_TC_MASK))//utilizamos el codigo bloqueante para que  termine de transmitir
		{
			// Codigo Bloqueante
		}
		UART0->D = character;

	}
	if (UART_1 == uart_channel) {}
	if (UART_2 == uart_channel) {}
	if (UART_3 == uart_channel) {}
	if (UART_4 == uart_channel) {}
}
void UART_put_string(uart_channel_t uart_channel, uint8_t* string)
{
	uint8_t contador = 0;
	if (UART_0 == uart_channel) {
		while (1) {
			if (string[contador] == '\0') //enviado de caracter  con variable contador qque ayuda a movernos sobre el string
			{
				break;
			}
			UART_put_char(uart_channel, string[contador]);
			contador++;
		}
	}
		if (UART_1 == uart_channel) {}
		if (UART_2 == uart_channel) {}
		if (UART_3 == uart_channel) {}
		if (UART_4 == uart_channel) {}
}

