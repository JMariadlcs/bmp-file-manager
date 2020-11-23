/**CFile***********************************************************************

  Fichero     node.c

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
/*------------INCLUDES-----------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/*------------OTHER CLASSES INCLUDES-----------*/
#include "node.h"
#include "coleccion.h"
#include "error.h"
#include "bmp.h"
#include "operations.h"

/*---------------------------------------------------------------------------*/
/* Declaración de variables                                                  */
/*---------------------------------------------------------------------------*/
int counterID=0;

/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/


struct node * createNode(char * name, int* error){
	#ifdef DEBUG
		printf("DEBUG: We create a node");
	#endif
	struct node *nodo = (struct node*)malloc(sizeof(struct node)); //We reserve needed space 
	
	if(nodo == NULL){ 												//If malloc has failed
		*error = MALLOC_FAIL;
	    
	}
	nodo->name=strdup(name); 									 //We copy the name of the node in the node
	if(nodo->name == NULL){										//We see if its has failed
		*error=MALLOC_FAIL;
		nodo = NULL;
	}
	
	nodo->prev = NULL;											//We initializate all the rest of the elements of the list
	nodo->next = NULL;
	nodo->id=counterID;
	counterID++;												//We add ID=ID+1, to indicate that the node has been succesfully created
	return nodo;												//We return nodo in order to use it in other functions
	
}
void printNode(struct node * nodo){      //**********
	#ifdef DEBUG
		printf("DEBUG: Method used to print the info of a node");
	#endif
	printf("Id\tName\tTamaño\tW\tH\tbpp\tFecha de Modificación\n");
	//We print the id, name, size and last modification date using ctime function
	printf("%d\t%s\t%d\t%d\t%d\t%d\t%s\n",					//we print the information needed
	        nodo->id,   nodo->name,(int)nodo->size, nodo->biWidth,  nodo->biHeight,  nodo->biBitCount, ctime(&(nodo->mtime)));
}

void info(struct list *my_collection, int id, int *error){
	#ifdef DEBUG
		printf("DEBUG: Method used to print the info of a node (calls printNode)");
	#endif
	
	if(inListId(my_collection,id)==NULL){  //If the searched ID is not found in the collection
		*error= ID_NOT_FOUND;				//We manage error of ID NOT FOUND
	}else{                                 //FOUND ID in collection
		printNode(inListId(my_collection,id)); //We print its information. inListId in list.c
	}	
}
