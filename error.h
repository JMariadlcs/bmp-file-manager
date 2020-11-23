#ifndef ERROR_H
#define ERROR_H

/***************         error.h        ***********************************

  Fichero     error.h

  Resumen     error managing is in this class

  Descripción The class contains the method that is used for ocmtroling 
			  every error in the program.


  Autor       Francisco Atalaya and Jose Maria de La Cruz, Patricia Paredes Martin, Iosif Mondoc

  Copyright   [Copyright (c) 2012 Universidad Carlos III de Madrid

  Se concede, sin necesidad de acuerdo por escrito y sin ningún tipo de derechos
  de autor, permiso para utilizar, copiar, modificar y distribuir este programa
  y su documentación para cualquier propósito siempre y cuando esta advertencia
  y los siguientes dos párrafos aparezcan en las copias.

  EN NINGÚN CASO SE RECONOCERÁ A LA UNIVERSIDAD CARLOS III DE MADRID RESPONSABLE
  DIRECTA, INDIRECTA, ESPECIAL, O SUBSIDIARIA DE LOS POSIBLES DAÑOS Y PERJUICIOS
  QUE PUEDAN DERIVARSE DEL USO DE ESTE PROGRAMA Y DE SU DOCUMENTACIÓN, INCLUSO
  EN EL CASO DE QUE LA UNIVERSIDAD CARLOS III DE MADRID HAYA SIDO ADVERTIDA DE
  TALES DAÑOS Y PERJUICIOS.

  LA UNIVERSIDAD CARLOS III DE MADRID ESPECÍFICAMENTE SE DESENTIENDE DE TODO
  TIPO DE GARANTÍAS INCLUYENDO, PERO NO LIMITANDOLAS A, LAS GARANTIAS IMPLÍCITAS
  DE LA COMERCIALIZACIÓN Y ADECUACIÓN PARA CUALQUIER PROPÓSITO EN PARTICULAR. EL
  PROGRAMA SE PROVEE TAL CUAL Y LA UNIVERSIDAD CARLOS III DE MADRID NO TIENE
  OBLIGACIÓN ALGUNA DE OFRECER MANTENIMIENTO, SOPORTE, ACTUALIZACIONES, MEJORAS
  O MODIFICACIONES.]

******************************************************************************/

/*---------------------------------------------------------------------------*/
/* Declaración de constantes                                                 */
/*---------------------------------------------------------------------------*/

#define NUM_ERRORS 31
/*----------------- UTIL.H -------------------------*/
#define SUCCESS_INPUT 0         // Éxito
#define ERR_NO_NUMBERS_INPUT -1 // No hay números
#define ERR_FURTHER_CHARS_INPUT -2 // Hay más caracteres después de los números
#define ERR_CTRLD_INPUT -3      // CTRL+D introducido 
#define ERR_TOO_MUCH_NUMBERS -4
#define ERR_NEG_NUMBER -5
#define ERR_ONLY_CHARS -6
#define ERR_EMPTY_LINE -7
/*---------------------------------------------------*/

/*---------------COLECCION.H-------------------------*/
#define EMPTY_COL -8                 // element already in the collection
#define ELEMENT_IN_COL -9            // empty collection
/*---------------------------------------------------*/

/*---------------NODE.H-------------------------*/
#define ID_NOT_FOUND -10         //id node not found
/*-----------------------------------------------*/

#define MALLOC_FAIL -11          //malloc return null

/*---------------Files-------------------------*/
#define FILE_NOT_FOUND -12         //id node not found
/*-----------------------------------------------*/

/*---------------Fork-------------------------*/
#define PID_ERROR -13   		//child proccess not created
/*-----------------------------------------------*/

/*---------------BMP-------------------------*/
#define NOT_BMP_ERROR -14
#define FREAD_ERROR -15
#define FWRITE_ERROR -16
#define ROTATE_ERROR -17
#define PERMISSIONS_ERROR -18
#define GRAY_ERROR -19
#define PIPE_ERROR -20
#define MIRROR_ERROR -21
#define EXCESS_ARGUMENTS -22
#define BITONAL_ERROR -23
#define ONLYONE_ERROR -24
/*-----------------------------------------------*/
/*---------------ENHANCED-------------------------*/
#define ERR_SORT_ARG -25
#define ERR_LOGFILE -26
#define ERR_INIT_FILE -27
#define ERR_SAVE_FILE -28
#define ERR_INVALID_COMMAND -29
#define ERR_CONTR_COMMAND -30

/*---------------------------------------------------------------------------*/
/* Protitpos de las funciones                                                */
/*---------------------------------------------------------------------------*/
/***********************************************************************/


/*********         Errormanage       ***********************

  Resumen      Function used to manage errors when we have some

  Descripción  We manage every error of our program with this function

  Parámetros   int *error. Used to manage errors  

  Efec. Colat. Nothing

**************************************/

char *errormanage(int error);



#endif
