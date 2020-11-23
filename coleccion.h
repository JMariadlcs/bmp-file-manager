#ifndef COLECCION_H
#define COLECCION_H

/************         Coleccion.h     *********************************

  Fichero     coleccion.h

  Resumen     All methods releated with the collection

  Descripción This class has the methods releated with the collection.
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
/*---------------------------------------------------------------------------*/
/* DEFINICION DE METODOS DE COLECCION.C                                      */
/*---------------------------------------------------------------------------*/
#include "node.h"
#include "list.h"

/*---------------------------------------------------------------------------*/
/* Protitpos de las funciones                                                */
/*---------------------------------------------------------------------------*/
//Function description is everything before declaration of the function itself

/**********    Initialize_collection     ***********************

  Resumen      This function is used to create a list for our collection to be managed later in other functions

  Descripción  We call to 'newList' method to create the list of your collection

  Parámetros   struct list *my_collection, int *error. Used to manage our list and the errors, in case we have some.

  Efec. Colat. we have to return a struct list type
  

**************************************************/
struct list initialize_collection(struct list *my_collection, int *error);


/*****************    Delete    ***********************

  Resumen      Method used to delete an item thats being already added to our collection

  Descripción  Elementos already added to our collection are being deleted using this method

  Parámetros   struct list* my_collection, int id, int*error. Used to manage our list, id used to detect the item we 
				want to delete, the error used to manage some erros, if we have.

  Efec. Colat. No collateral effects


*****************************************************/

void Delete(struct list* my_collection, int id, int*error);

/*********************    info_all    ********************

  Resumen      info_all function is used to print every element previously added to our collection

  Descripción  Every element of our collection is printed using this functionallity

  Parámetros   struct list* my_collection, int *error. My_collection used to have access to the items 
				previously added to our collection, the error used to managed it in case we have to

  Efec. Colat. No collateral effects

  

*********************************************************/

void info_all(struct list* my_collection, int *error);

/*****************    load     ***********************

  Resumen      This function is used to add a file in the collection and manage all the child proccesses.

  Descripción  Every item added to the collection manually is being added using this function with an ording criteria
				Also, a loop while(1) makes every child proccess wait and execute the operation given by the parent proccess through a pipe,

  Parámetros   struct list *my_collection, char *nombre, int type, int *error. My_collection is used to 
				access the elements in our list, name is used to add a file in the collectin by its name,
				type is the sorting criteria introduced by the user in the first parameters and error
				is used to manage it in case we have to  

  Efec. Colat. No collateral effects

**************************/

struct node* load(struct list *my_collection,int type,  char *nombre, int *error);

/*************************  free_coleccion     *******************

  Resumen      Function used to 'free' space when we finish executing the program

  Descripción  We call to this function when we finish executing the program in order to avoid memory leaks

  Parámetros   struct list *my_collection. Used to have access to previously requiered memory. 

  Efec. Colat. We have to call it everytime we finish our program

 

*************************************************/

void free_coleccion(struct list *my_collection);

/***************   save_coleccion    ***********************

  Resumen      Function used to save our program collection in a file in the directory

  Descripción  This function is used to save our program collection in a file in the directory 
				just in case that we wanted to load this collection in another time

  Parámetros   struct list *my_collection,char *nombre, int *error. My_collection to have access 
				to the elements in the collection, nombre used to save the items by its name and 
				error used to manage it in case we have to  

  Efec. Colat. We have to be sure to close the file (done inside the method) before finishing programming this method

  

***************************************************/

void save_coleccion(struct list *my_collection,char *nombre, int *error);

/*******************   load_coleccion     ***********************

  Resumen      Function used to load a previously saved collection in our program

  Descripción  If we have saved a file containing a previously collection, we can load it into our 
				new program using this function

  Parámetros   struct list* my_collection, char* filename, int* error. My_collection to have acces 
				to the items in our collection and be able to add new more, filename to know where we have our 
				previously saved collection and error used to managed it in case we have  

  Efec. Colat. We have to close the file (done inside the method) and be sure that we have done the 
			   necessaries 'frees' to avoid memory leakds

 
***************************************/

void load_coleccion(struct list* my_collection, int type,char* filename, int* error);



#endif
