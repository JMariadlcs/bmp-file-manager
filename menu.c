/**CHeaderFile*****************************************************************

  Fichero     menu.c

  Resumen     All method related with list

  Descripción This class has the methods releated with the menu.
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
/*------------INCLUDES-----------*/
#include <stdio.h>
#include <stdlib.h>
/*------------OTHER CLASSES INCLUDES-----------*/
#include "menu.h"

/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/

void short_menu(){
	
	#ifdef DEBUG
		printf("DEBUG: Method used to print the menu");
	#endif

	printf("Valid commands: load, delete, info, infoall, savecol, loadcol, \naddop, help_ops, logon, logoff, help, exit.\n\n");	
	
}
void extended_menu(){
	
	#ifdef DEBUG
		printf("DEBUG: Method used to print the menu");
	#endif

	printf("./saucem_enhanced is a file manager developed by SAUCEM S.L.\nthat offers the following options:\n\n");
	printf("load file_name              - adds the file file_name into the colletion\n");
	printf("delete id                   - removes the node with identifier id from the collection\n");
	printf("                              without erasing the file from the file system\n");
	printf("info id                     - shows information of the node with identifier id\n");
	printf("infoall                     - shows the information of every node in the collection\n");
	printf("savecol file_name           - stores the current collection in file file_name.\n");
	printf("loadcol file_name           - loads the collection stored in file file_name\n");
	printf("                              into the program (this operation destroys the\n");
	printf("                              current collection).\n");
	printf("addop id op                 - applies an operation to a given id\n");
	printf("help_ops                    - shows the supported operations\n");
	printf("logon                       - option for administrators: prints on screen messages\n");
	printf("                              being stored in the log file\n");
	printf("logoff                      - option for administrators: stop printing on screen\n");
	printf("                              messages above. A logoff without a previous logon doesn't\n");
	printf("                              have any effect\n");
	printf("help                        - shows this text\n");
	printf("exit                        - quits the program and stores the collection into the\n");
	printf("                              persistence file\n");
	printf("- If CTRL+D is pressed in the main menu, the program quits storing (and merging) the\ncurrent collection into the persistence file.\n");
	printf("- If CTRL+D is pressed in any sub-menu, the program goes back to the previous menu.\n\n");
	printf("The program captures the following signals:\n\n");
	printf("SIGINT:  if CTRL+C is pressed, the program quits safely\n");
	printf("SIGTSTP: if CTRL+Z is pressed, the program changes the internal order of the\n");
	printf("         collection to 'by filename'\n");
	printf("SIGUSR1: if SIGUSR1 is captured, the program changes all the BMP images to grayscale\n");
	printf("         using its luminance\n");
	
}
void menuOperations(){
    
    printf("\n");
    printf("1. Histogram filename\n");
    printf("2. Gray Scale [r|g|b|y]\n");
    printf("3. Bitonal dark light threshold\n");
    printf("4. Rotate [r|l] [90|180|270]\n");
    printf("5. Only One [r|g|b]\n");
    printf("6. Black and white\n");
    printf("7. Mirror [h|v]\n");
    printf("\n");
    
}

