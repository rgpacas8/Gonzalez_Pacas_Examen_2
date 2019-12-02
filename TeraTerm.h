/*
 * @file:			TeraTerm.h
 * @company:		ITESO
 * @Engineer Team:	D.F.R. / R.G.P.
 */


#ifndef TERATERM_H_
#define TERATERM_H_

#include <stdint.h>
#include "bits.h"


#include "UART_driver.h"
#include "GPIO.h"




//** Macros para los valores ASCII correspondientes de las teclas presionadas */
#define NADA  '\0'
#define ESC   '\e'
#define ENTER '\r'
#define SYSTEM_CLOCK (21000000U)
#define key_barra '/'
#define key_1 '1'
#define key_2 '2'
#define key_3 '3'
#define key_4 '4'
#define key_5 '5'
#define key_6 '6'
#define key_7 '7'
#define key_8 '8'
#define key_9 '9'
#define key_0 '0'

typedef enum {
	MENU_INICIAL,
	sub_1, sub_2, sub_3, sub_4, sub_5, sub_6, sub_7, sub_8,
	CONTROL_FSM
} State_t;

typedef struct {
	uint8_t menu_init :1;
	uint8_t submenu_1 :1;
	uint8_t submenu_2 :1;
							//** submenu_3 Logicamente no se usa por constante refresh */
							//** submenu_4 no se usa por constante refresh */
	uint8_t submenu_5 :1;
	uint8_t submenu_6 :1;
	uint8_t submenu_7 :1;
	uint8_t submenu_8 :1;
} FSM_flags_t;

//** For show the actual Menu for the user */
void TeraTerm_Menu(void);

//** For show the key pressed back to the terminal */
void TeraTerm_Echo_UART0(void);

//** For show the Initial Menus of the Control Panel */
void TeraTerm_MENU_FSM(void);

void RTC_BCD_TIME_Registers(void); //** String de tiempo lista para mandar a TeraTerm*/

void RTC_BCD_DATE_Registers(void); //** String de fecha lista para mandar a TeraTerm */

void submenu1 (void);
void submenu2 (void);
void submenu3 (void);
void submenu4 (void);
void submenu5 (void);
void submenu6 (void);
void submenu7 (void);
void submenu8 (void);


uint8_t* Get_modified_hours(void);
uint8_t* Get_modified_minutes(void);
uint8_t* Get_modified_seconds(void);

uint8_t* Get_modified_date(void);
uint8_t* Get_modified_month(void);
uint8_t* Get_modified_year(void);

void FSM_Submenu_1(void);
void FSM_Submenu_2(void);

#endif /* TERATERM_H_ */



