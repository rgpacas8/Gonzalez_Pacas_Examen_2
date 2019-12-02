/*
 * @file:			TeraTerm.c
 * @company:		ITESO
 * @Engineer Team:	D.F.R. / R.G.P.
 */

#include "TeraTerm.h"
#include "RGB.h"

#include "PIT.h"

#define ARRAY_mod_hours   &modified_hours[0]	//Direccion del arreglo de Horas
#define ARRAY_mod_minutes &modified_minutes[0]	//Direccion del arreglo de Minutos
#define ARRAY_mod_seconds &modified_seconds[0]	//Direccion del arreglo de Segundos

#define ARRAY_mod_Date  &modified_Date[0]	//Direccion del arreglo de Dia
#define ARRAY_mod_Month &modified_Month[0]	//Direccion del arreglo de Mes
#define ARRAY_mod_Year  &modified_Year[0]	//Direccion del arreglo de Anio

static FSM_flags_t g_FSM_status_flags = {0};	//** Banderas de activacion de cada submenu */

/**This is mail box to received the information from the serial port*/
extern uart_mail_box_t g_mail_box_uart_0;

uint8_t g_key_UART0_PC  = '\0'; //** From TeraTerm in PC */
uint8_t g_key_UART4_CEL = '\0'; //** From Bluetooth in CELLPHONE */

uint8_t g_new_key_UART0 = FALSE;
static uint8_t UART0_key = '\0';	//** Cuenta el numero de presiones dentro de cada Submenu*/
static uint8_t i_sub1 = 0;	//** Indice de array en submenu 1*/
static uint8_t i_sub2 = 0;	//** Indice de array en submenu 2*/
static uint8_t i_sub6 = 0;	//** Indice de array en submenu 6*/

static uint8_t instruction_1[] = "\033[0;35;43m";
static uint8_t mensaje_1[] = "1) ";
static uint8_t mensaje_2[] = "2) ";
static uint8_t mensaje_3[] = "3) ";
static uint8_t mensaje_4[] = "4) ";
static uint8_t mensaje_5[] = "5) ";

static uint8_t terminal_1[] = "Terminal 1: ";
static uint8_t terminal_2[] = "Terminal 2: ";

//** String that will be printed in the TeraTerm*/
uint8_t string_hours[4]    = "HH:"; /*! Hours  */
uint8_t string_minutes[4]  = "MM:"; /*! Minutes*/
uint8_t string_seconds[3] = "SS "; /*! Seconds*/

uint8_t string_Date[4]  = "DD/"; /*! Day  */
uint8_t string_Month[4] = "MM/"; /*! Month*/
uint8_t string_Year[3]  = "AA "; /*! Year */

//** Propose to modify the TIME or the DATE actual*/
//** In the beginning will start in the last day of this year (2019)*/
uint8_t modified_hours[4] 	= {'2', '3'};
uint8_t modified_minutes[4] = {'5', '9'};
uint8_t modified_seconds[3] = {'0', '0'};

uint8_t modified_Date[4]  = {'3', '1'};
uint8_t modified_Month[4] = {'1', '2'};
uint8_t modified_Year[3]  = {'1', '9'};

uint8_t matrix_ON[3] 	= {'n', 'o'};

void TeraTerm_Menu(void) {
	/**The following sentences send strings to PC using the UART_put_string function. Also, the string
	 * is coded with terminal code*/
	/** VT100 command for text in red and background in cyan*/
	UART_put_string(UART_0, instruction_1);
	/*VT100 command for clearing the screen*/
	UART_put_string(UART_0, "\033[2J");
	/** VT100 command for text in red and background in green*/
	//UART_put_string(UART_0,"\033[0;32;41m");
	/** ( 20,10 ) = ( X,Y ) = ( dezp. fila, dezp. columna ) de la terminal*/
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0, "\033[10;10H");
	UART_put_string(UART_0, "COLOR TIEMPO\r");


}

void TeraTerm_Echo_UART0(void) {

	/** Modulo de codice que hace la funcion de echo para recibir de
	 *  la terminal de TeraTerm que entra cuando escribes y envia el
	 *  valor ASCII de la tecla que presionaste en TeraTerm */

	if (g_mail_box_uart_0.flag) {
		/**Sends to the PCA the received data in the mailbox*/
		UART_put_char(UART_0, g_mail_box_uart_0.mailBox);

		g_key_UART0_PC = g_mail_box_uart_0.mailBox; //** GET THE ASCII PRESSED */

		if (g_FSM_status_flags.submenu_1 == TRUE){
			g_new_key_UART0 = TRUE;	//** Bandera para detectar una presion de tecla*/
		}

		if (g_FSM_status_flags.submenu_2 == TRUE){
			g_new_key_UART0 = TRUE;	//** Bandera para detectar una presion de tecla*/
		}

		if (g_FSM_status_flags.submenu_6 == TRUE){
			g_new_key_UART0 = TRUE;	//** Bandera para detectar una presion de tecla*/
		}

		/**clear the reception flag*/
		g_mail_box_uart_0.flag = 0;
	}
}


void TeraTerm_MENU_FSM(void) {

/*	static State_t current_state = MENU_INICIAL; //** Estado Inicial del Sistema */
	static uint8_t index = 0;
	static uint8_t index_colores = 0;
	static uint8_t colores_valores[10] ={0};
	uint8_t estado = 0;

	if(index<=10 && g_mail_box_uart_0.flag==TRUE )
	{
	TeraTerm_Echo_UART0();	//** HERE I GET THE ASCII THAT WAS PRESSED */
	colores_valores[index] = g_key_UART0_PC; // Actualizo mi current State segun la tecla presionada
	index++;
	}
	//delay(500);	//** Evitar demasiado refresh en TeraTerm mejora
					// 	 la calidad del usuario con la interfaz menu */
	if (index == 10) {
		if (colores_valores[index_colores] == 'r') {
			encender_LED(RED_ON);
		}
		if (colores_valores[index_colores] == 'a') {
			encender_LED(BLUE_ON);
		}
		if (colores_valores[index_colores] == 'g') {
			encender_LED(GREEN_ON);
		}
		if (colores_valores[index_colores] == 'b') {
			encender_LED(GREEN_ON);
			encender_LED(RED_ON);
			encender_LED(BLUE_ON);
		}
		if (colores_valores[index_colores] == 'c') {
			encender_LED(GREEN_ON);
			encender_LED(BLUE_ON);
		}
	}					//end if
}

void delay(uint16_t delay) {
	volatile uint16_t j, i;

	for (j = delay; j > 0; j--) {
		for (i = 0; i < 1000; ++i) {
			__asm("nop");
		}

	}
}

