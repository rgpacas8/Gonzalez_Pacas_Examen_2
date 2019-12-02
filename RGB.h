/*
 * RGB.h
 *
 *  Created on: 10 sep 2019
 *      Author: Ricardo Pacas
 */

#ifndef RGB_H_
#define RGB_H_

#include "stdint.h"

#define SET_RED_ON	  0x00400000;
#define SET_GREEN_ON  0x04000000;
#define SET_BLUE_ON   0x00200000;

#define SET_RED_OFF   0x00400000;
#define SET_GREEN_OFF 0x04000000;
#define SET_BLUE_OFF  0x00200000;

typedef enum {
	RED_ON,
	GREEN_ON,
	BLUE_ON,
	PURPLE_ON,
	YELLOW_ON,
} color_ON;

typedef enum {
	RED_OFF,
	GREEN_OFF,
	BLUE_OFF,
	RGB_OFF,
} color_OFF;


/** Inicializa los pines del LED RGB */
void RGB_init(void);


/** Inicializa los sw2 y sw3 como entrada y Interrupción */
void SW_init(void);





/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	  This function turn on a specific color: RED, GREEN or BLUE.

 	 \param[in]  Color of the led.
 	 \return 	 FALSE o TRUE
 */
uint8_t encender_LED(color_ON color_RGB);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function turn off a specific color: RED, GREEN or BLUE.

 	 \param[in]  Color of the led.
 	 \return 	 FALSE o TRUE
 */
uint8_t apagar_LED(color_OFF color_RGB);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function spend time in nothing.

 	 \param[in]  Valor para gastar tiempo
 	 \return void
 */
void delay_rgb(uint16_t delay);


#endif /* RGB_H_ */
