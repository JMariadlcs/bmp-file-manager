#ifndef ENHANCED_H
#define ENHANCED_H

/**enhanced.h*****************************************************************

  Fichero     enhanced.h

  Resumen     All method related with enhanced version

  Descripción This class contains all method releated with the enhanced
			  version. The managing of complex orders and commands are
			  done in this methods

  Autor       Francisco Atalaya, Jose María de La Cruz,
			  Patricia Paredes, Iosif Mondoc
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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

/*---------------------------------------------------------------------------*/
/* Declaración de estructuras                                                */
/*---------------------------------------------------------------------------*/
struct options{
	int help;
	
	int sort_type; //0-Default 1-size 2-name 3-time
	
	int l;
	char* logfile;
	
	int i;
	char *initfile;
	
	int o;
	char* savefile;
	
	int clean;
	
	int no_save;	
};
/*---------------------------------------------------------------------------*/
/* Protitpos de las funciones                                                */
/*---------------------------------------------------------------------------*/
/**printParamMenu********************************************************************

  Resumen      This function is used to print the parameters availables

  Descripción  The function prints a long menu of the parameters available and its options
				

  Parámetros   nothing

  Efec. Colat. nothing


******************************************************************************/
void printParamMenu();
/**printParam********************************************************************

  Resumen      This function is used to print the parameters chosen

  Descripción  The function prints the parameters that the user has chosen. In case
  * 			of --help, it calls the function printParamMenu
				

  Parámetros   nothing

  Efec. Colat. nothing


******************************************************************************/
void printParam(struct options * opt);
/**fillStructParam********************************************************************

  Resumen      This function is used to fill the struct created in the main

  Descripción  The function fills the struct created in the main with the options
  * 			introduced by the users
				

  Parámetros    int argc-> number of elemtents in the array argv
  * 			char* argv[]-> the command line introduced by the user
				struct options* opt->the struct to fill
				int* error->in to manage the error;
  Efec. Colat. nothing


******************************************************************************/
struct options* fillStructOpt(int argc,char *argv[], struct options* opt,int *error);

/**free_opt********************************************************************

  Resumen      This function is used to free the struct created in the main

  Descripción  The function free the struct created in the main w
				

  Parámetros   struct option* opt->the struct to free
  Efec. Colat. nothing


******************************************************************************/

void free_opt(struct options * opt);

#endif
