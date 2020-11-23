/*********         error.c          *****************************

  Fichero     error.c

  Resumen     contains the method that manage every error

  Descripción The method inside has an error array and prints
			  the error needed

  Autor       Francisco Atalaya and Jose María de La Cruz, Patricia Paredes Martin, Iosif Mondoc

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

/*------------INCLUDES-----------*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
/*------------OTHER CLASSES INCLUDES-----------*/
#include "error.h"

/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/
 
char *msg_error [NUM_ERRORS + 1] = 
{   "Success ! ",                                                                              //ERR_SUCCESS_INPUT 0
    "ERROR: No valid input",                                                  				   //ERR_NO_NUMBERS_INPUT -1
    "ERROR: You have written characters",                                                      //ERR_FURTHER_CHARS_INPUT -2
    "You have selected CTRL+D",                                    							   //ERR_CTRLD_INPUT -3
    "ERROR: You have introduced more than one number",                                         //ERR_TOO_MUCH_NUMBERS -4
    "ERROR: You have itroduced a negative number",                                             //ERR_NEG_NUMBER -5
    "ERROR: You have written only characters",                                                 //ERR_ONLY_CHARS -6           
    "ERROR: You have pressed the enter key",                                                   //ERR_EMPTY_LINE -7
    "ERROR: The collection is empty",                                                          //EMPTY_COL -8
    "ERROR: Element already in the collection",                                                //ELEMENT_IN_COL -9
    "ERROR: ID not found",                                                                     //ID_NOT_FOUND -10
    "ERROR: Malloc can't allocate it, insufficient memory",                                    //ERR_MALLOC_FAIL -11
    "ERROR: File doesn't exist",                                                               //FILE_NOT_FOUND -12
    "ERROR: Child process not created",                                                        //PID_ERROR -13
    "ERROR: It's not a BMP file",                                                              //NOT_BMP_ERROR -14
    "ERROR: It can't be read",                                                                 //FREAD_ERROR -15
    "ERROR: It can't be written",                                                              //FWRITE_ERROR -16
    "ERROR: Invalid rotation argument",                                                        //ROTATE_ERROR -17
    "ERROR: Permission denied",                                                                //PERMISSIONS_ERROR -18
    "ERROR: Invalid gray scale argument",                                                      //GRAY_ERROR -19
    "ERROR: Communication has failled",                                                        //PIPE_ERROR -20
    "ERROR: Invalid mirror argument",                                                          //MIRROR_ERROR -21
    "ERROR: You have written a WRONG number of arguments",                                     //EXCESS_ARGUMENTS -22
    "ERROR: Invalid bitonal argument",                                                         //BITONAL_ERROR -23
    "ERROR: Invalid only one argument",                                                        //ONLYONE_ERROR -24
    "ERROR: Invalid or missing sort type",								                       //ERR_SORT_ARG -25
    "ERROR: Missing log filename",										                       //ERR_LOGFILE -26
    "ERROR: Missing initial collection filename",						                       //ERR_INIT_FILE -27
    "ERROR: Missing save filename",										                       //ERR_SAVE_FILE -28
    "ERROR: Invalid command",											                       //ERR_INVALID_COMMAND -29
    "ERROR: Contradictory commands"										                       //ERR_CONTR_COMMAND -30
}; 


char *errormanage(int error){
    
    /*
     * If the error is positive its means that we have an error in errno.
     */
    
    if (error > 0) {                
        
        return "Errno error \n" ;
        
    /*
     * If the error is a negative one but its a number less than the total of
     * error that we have, its an undefined error
     */
        
    } else if (error <= -NUM_ERRORS) {
        
        return "Undefined error \n"; 
        
    /*
     * If the error is a negative one and corresponds to the ones defined in 
     * util.h we use the pointer of chars that we have created before.
     */
        
    } else {
        
        return msg_error[-error];
    }
    
}
	
	
	
	
	

