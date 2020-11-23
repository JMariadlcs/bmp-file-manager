/**CHeaderFile*****************************************************************

  Fichero     node.h

  Resumen     All method related with node

  Descripción This class has the methods releated with the node.
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

#ifndef NODE_H
#define NODE_H
/*------------INCLUDES-----------*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
/*------------OTHER CLASSES INCLUDES-----------*/
#include "coleccion.h"
#include "list.h"
#include "bmp.h"

/*---------------------------------------------------------------------------*/
/* Declaración de estructuras                                                */
/*---------------------------------------------------------------------------*/
struct node{
	
	//struct stat infoFile;
	int id;
	char* name;
	
	off_t size;
	time_t mtime;
    
    pid_t pid;
    int p[2];
    int p2[2];
	
	int  biWidth; // width in pixels
	int  biHeight; //height in pixels
	short  biBitCount; // number of bits per pixel
	
	struct node *next;
	struct node *prev;
	
	
};

/*---------------------------------------------------------------------------*/
/* Protitpos de las funciones                                                */
/*---------------------------------------------------------------------------*/


//Function description is everytime before declaration of the function itself

/**createNode********************************************************************

  Resumen      This function is used to create a new node and update our list when we add an item

  Descripción  When we want to add a new item we call to this function to create a new node in order 
				to insert it in the collection

  Parámetros   char * name, int* error. Name to create the node by its name, error to manage it in case we have to  

  Efec. Colat. We have to return a struct node* type


******************************************************************************/

struct node * createNode(char * name, int* error);

/**info********************************************************************

  Resumen      Function used to print the information of a single node

  Descripción  When we want to print the information of a single node in the list, we call to this function

  Parámetros   struct list *my_collection, int id, int*error. My_collection to have access to the elements in our collection,
				id to know what element we want to delet, and error to manage it in case we have to

  Efec. Colat. No collateral errors

******************************************************************************/
void info(struct list *my_collection, int id, int*error);

/**printNode********************************************************************

  Resumen      Function used inside 'info'

  Descripción  This function is called inside info, just to print the information of a single node

  Parámetros   struct node * nodo. To have acces to the node we want to print information of 

  Efec. Colat. No collateral effects
  * 
******************************************************************************/
void printNode(struct node * nodo);
#endif
