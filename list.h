/*******************            coleccion.c            ************************************

  Fichero     list.h

  Resumen     Methods releated with the collection

  Descripción This class has the methods releated with the list.
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
#ifndef LIST_H
#define LIST_H

/*------------OTHER CLASSES INCLUDES-----------*/
#include "node.h"

/*---------------------------------------------------------------------------*/
/* Declaración de estructuras                                                */
/*---------------------------------------------------------------------------*/

struct list{ //collection will be al list instead of a struct collection
	
	struct node *head;
	struct node *tail;
	struct node *current;
	int size;
		
	};

struct list *newList(int *error);

/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/

//Function description is everytime before declaration of the function itself

/**InsertAtHead********************************************************************

  Resumen      Used to insert a node when we create it, in its position, at first.

  Descripción  When we add an item, we have to insert it in our collection so we call to this method

  Parámetros   struct list *lista, struct node* nodo ,int *error . lista to have access to the insertable positions
				node we want to insert, error to manage it 

  Efec. Colat. No collateral effects


******************************************************************************/
void InsertAtHead(struct list *lista, struct node* nodo ,int *error );

/**InsertAtTail********************************************************************

 
  Resumen      Used to insert a node when we create it, in its position, at tail.

  Descripción  When we add an item, we have to insert it in our collection so we call to this method

  Parámetros   struct list *lista, struct node* nodo ,int *error . lista to have access to the insertable positions
				node we want to insert, error to manage it 

  Efec. Colat. No collateral effects

******************************************************************************/
void InsertAtTail(struct list *lista, struct node* nodo, int *error);

/**delete_node********************************************************************

  Resumen      Function used to delete a node of the collection

  Descripción  When we want to delent a node of our list, we will call to this method

  Parámetros   struct list *lista, struct node *nodo, int* error. lista to have access to the insertable positions
				node we want to insert, error to manage it  

  Efec. Colat. No collateral effects

******************************************************************************/
void delete_node (struct list *lista, struct node *nodo, int* error);

/**printList********************************************************************

  Resumen      Method used to print the complete list. Used in 'infoall'

  Descripción  When we want to print every item in the collection, we call to this method

  Parámetros   struct list* lista to have acces to the list  

  Efec. Colat. No collateral effects

******************************************************************************/
void printList(struct list* lista);

/**inListId********************************************************************

  Resumen      Used to know if an items already exists in the collection by its ID

  Descripción  This method is used for example when we have to delete an item and we have to search	
				if its already in the collection.

  Parámetros   struct list* lista, int id. To have acces to the list, and its id to search for it

  Efec. Colat. No collateral effects

******************************************************************************/
struct node* inListId(struct list* lista, int id);

/**inListName********************************************************************

  Resumen       Used to know if an items already exists in the collection by its name

  Descripción  This method is used for example when we have to delete an item and we have to search	
				if its already in the collection.

  Parámetros   struct list* lista, int id. To have acces to the list, and its id to search for it

  Efec. Colat. No collateral effects

******************************************************************************/
struct node* inListName(struct list* lista, char* name);

/**InsertAtPosition********************************************************************

  Resumen       Used to insert into a specific position of the collection

  Descripción  Used to insert into a specific position of the collection

  Parámetros   struct list* lista, struct node *nodo, its the node to be inserted, type of sort used to insert it
  Efec. Colat. No collateral effects

******************************************************************************/

void InsertAtPosition(struct list *lista, struct node *nodo, int *error, int (*compare_type) (struct node *, struct node *));

/**compare_id;********************************************************************

  Resumen       Used to compare two nodes by comparing its id

  Descripción   Used to compare two nodes in order to insert them in a correct way. 
                Returns 1 if the first node has an id bigger than the second,
                -1 if it is smaller and 0 if they have the same id (not common).

  Parámetros   Boths nodes to be compared
  Efec. Colat. No collateral effects

******************************************************************************/

int compare_id(struct node *first, struct node* second);

/**compare_size;********************************************************************

  Resumen       Used to compare two nodes by comparing its size

  Descripción   Used to compare two nodes in order to insert them in a correct way. 
                Returns 1 if the first node has a bigger size, -1 if smaller and 0
                if both have the same file size.

  Parámetros   Boths nodes to be compared
  Efec. Colat. No collateral effects

******************************************************************************/

int compare_size(struct node *first, struct node* second);

/**compare_name;********************************************************************

  Resumen       Used to compare two nodes by comparing its name

  Descripción   Used to compare two nodes in order to insert them in a correct way. 
                Returns 1 if the first node is alphabetically earlier than the second,
                -1 if older and 0 if they start with the same character. In the last case,
                another comparation is made. So, the function only checks up to 2 equal
                characters.

  Parámetros   Boths nodes to be compared
  Efec. Colat. No collateral effects

******************************************************************************/

int compare_name(struct node *first, struct node* second);

/**compare_time;********************************************************************

  Resumen       Used to compare two nodes by comparing its time of insertion

  Descripción   Used to compare two nodes in order to insert them in a correct way. 
                Returns 1 if the first node has a more recent modification time,
                -1 if older and 0 if they have the same modification time.

  Parámetros   Boths nodes to be compared
  Efec. Colat. No collateral effects

******************************************************************************/

int compare_time(struct node *first, struct node* second);

 

#endif

