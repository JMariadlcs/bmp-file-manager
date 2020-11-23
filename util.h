/**CHeaderFile*****************************************************************

  Fichero     util.h

  Resumen     All method related with list

  Descripción This class has the methods releated with the util.
			  Each time it is needed to modify the collection data, it 
			  will be needed to use a method of this class.

  Autor       Francisco Atalaya and Jose María de La Cruz

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

#ifndef UTIL_H
#define UTIL_H


/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/
//Function description is everytime before declaration of the function itself
 

/**get_number********************************************************************

  Resumen      This function is used to get what we have introduced by stdin and convert it into a number if we have to do it

  Descripción  We are calling this function everytime we want the user to enter something by stdin; 
				every time we have to select an option for example

  Parámetros   int base, int *error. Used to set the base and manage the rror  

  Efec. Colat. No collateral efects

  Ver también  [opcional]

******************************************************************************/
long get_number( int *error, int base);

/**get_string********************************************************************

  Resumen      This function is used to get what we have introduced by stdin

  Descripción  We are calling this function everytime we want the user to enter something by stdin; 
				for example when we have to introduce a name. get_string is called by get_number

  Parámetros   int *error. To manage it  

  Efec. Colat. We have to be aware of doing a free to the return value of get_string

******************************************************************************/
char *get_string(int *error);

long int get_op(int *error);

int transformarstrtol(char* cadena, int base, int *error);

int compare_operation( char *token, int *error);

int compare_option( char *token, int *error);

int contar_palabras(char *cadena);


#endif
