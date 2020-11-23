/**CHeaderFile*****************************************************************

  Fichero     menu.h

  Resumen     All method related with list

  Descripción This class has the methods releated with the menu.
			  Each time it is needed to modify the collection data, it 
			  will be needed to use a method of this class.

  Autor       Francisco Atalaya and Jose María de La Cruz, Iosif Mondoc, Patricia Paredes Martín

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

#ifndef MENU_H
#define MENU_H

/*--------------------------------------------------------------------*/
/*Function we are calling to print the menu                           */
/*--------------------------------------------------------------------*/

/**short_menu********************************************************************

  Resumen      Used to print the 'short menu'

  Descripción  When we call this function when the user types 'help'

  Parámetros   No parameters  

  Efec. Colat. No collateral effects

******************************************************************************/

void short_menu();

/**extended_menu********************************************************************

  Resumen      Used to print the 'extended' menu

  Descripción  When the user types 'help', short menu will be printed and then the user will be asked if he wants more information, if so extended_menu will be printed

  Parámetros   No parameters  

  Efec. Colat. No collateral effects

******************************************************************************/

void extended_menu();

/**menuOperations********************************************************************

  Resumen      Used to print the menu of perations 

  Descripción  When the user types help_ops, this menu will be printed

  Parámetros   No parameters  

  Efec. Colat. No collateral effects

******************************************************************************/

void menuOperations();

#endif
