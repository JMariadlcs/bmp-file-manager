/*******************            coleccion.c            ************************************
 * 
 F i*chero     list.c
 
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

/*------------INCLUDES-----------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
/*------------OTHER CLASSES INCLUDES-----------*/
#include "node.h"
#include "list.h"
#include "error.h"
#include "bmp.h"


/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/

struct list *newList(int *error){
    
    #ifdef DEBUG
    printf("DEBUG: Method used to create a new list");
    #endif
    
    struct list *newList = (struct list*)malloc(sizeof(struct list));  //frees in freeCollection
    if(newList == NULL){
        *error = MALLOC_FAIL;
    }else{
        newList->head = NULL;
        newList->tail = NULL;
        newList->current = NULL;
        newList->size = 0;
    }
    return newList;
}


//Inserts a Node at head of doubly linked list
void InsertAtHead(struct list *lista, struct node* nodo , int *error ) {
    
    #ifdef DEBUG
    printf("DEBUG: Method used to insert a Node at head of the list");
    #endif
    
    if(lista->head == NULL) {						//case in which the list is empty
        lista->head = nodo;
        lista->tail = nodo;
        lista->current = nodo;
        lista->size++;
        
    }else{
        lista->head->prev = nodo;				//The previous of the head is the new node
        nodo->next = lista->head; 				//The next of the new is the current head
        lista->head = nodo;						//The newNode is now the Head of the list
        lista->size++;
        
    }
}

//Inserts a Node at tail of Doubly linked list
void InsertAtTail(struct list *lista, struct node *nodo, int *error) {
    
    #ifdef DEBUG
    printf("DEBUG: Method used to insert a Node at tail of the list");
    #endif
    
    if(lista->head == NULL) {							//case in which the list is empty
        lista->head = nodo;
        lista->tail = nodo;
        lista->current = nodo;
        lista->size++;
        
    }else{
        lista->tail->next =nodo;						//The next of the tail is the new node
        nodo->prev = lista->tail;						//The previous of the new node is the current tail
        lista->tail = nodo;							    //The newNode is now the Tail of the list
        lista->size++; 
        
    }						
}


//Deletes the node passed through parameter
void delete_node(struct list *lista, struct node *nodo, int* error){
    
    #ifdef DEBUG
    printf("DEBUG: Method used to delete a Node");
    #endif
    
    if(lista->head == NULL){
        *error=EMPTY_COL;						//The list is empty
        
    }
    if(nodo->next == NULL && nodo->prev == NULL){  //the node to delete is the only node in the list
        lista->head = NULL;
        lista->tail = NULL;
        lista->current = NULL;
        lista->size--;
    } else if(lista->head == nodo){					//if the node you want to delete is the first one.	
        if(lista->current == lista->head){
            lista->current = NULL;
        }
        nodo->next->prev = NULL;								//The previous of the second node is now NULL
        lista->head = nodo->next;								//The second node is now the new Head
        lista->size--;									
        
    } else if( nodo->next == NULL){					//the node is the last one
        if(lista->current == lista->tail){
            lista->current = lista->current->prev;
            
        }
        nodo->prev->next = NULL;								//The next of the penultimate node is now NULL
        lista->tail = nodo->prev;								//The penultimate node is now the new Tail
        lista->size--;
    } else {										//It's an intermedium node				
        if(lista->current == nodo){
            lista->current = lista->current->prev;
            
        }
        nodo->prev->next = nodo->next;					
        nodo->next->prev = nodo->prev;
        lista->size--;
    }
    
    free(nodo->name);
    free(nodo);
    
    
}

void printList(struct list* lista){
    
    #ifdef DEBUG
    printf("DEBUG: Method used to print a Node at head of the list");
    #endif
    
    struct node * tmp=lista->head; 						//Temporal node pointing the head of the list
    printf("Id\tName\t\tTamaño\t\tW\tH\tbpp\tFecha de Modificación\n");
    for(int i=0; i<lista->size; i++){
        printf("%d\t%s\t%d\t%d\t%d\t%d\t%s\n",					//we print the information needed
               tmp->id,   tmp->name,(int)tmp->size, tmp->biWidth,tmp->biHeight,tmp->biBitCount, ctime(&(tmp->mtime)));
        tmp=tmp->next;
    }
}



struct node* inListId(struct list* lista, int id){
    
    #ifdef DEBUG
    printf("DEBUG: Method used to print search an item in the list by its Id");
    #endif
    
    struct node * tmp=lista->head;						//Temporal node pointing the head of the list
    for(int i=0; i<lista->size; i++){					//for loop used to travel through all the collection
        if(tmp->id==id){								//We check if its the item that we are searching
            return tmp;
        }
        tmp=tmp->next;
    }
    return NULL;
    
}
struct node* inListName(struct list* lista, char* name){
    
    #ifdef DEBUG
    printf("DEBUG: Method used to print search an item in the list by its name");
    #endif
    
    struct node * tmp=lista->head;					//Temporal node pointing the head of the list
    for(int i=0; i<lista->size; i++){				//for loop used to travel through all the collection
        if(strcmp(tmp->name,name)==0){				//We check if its the item that we are searching
            return tmp;
        }
        tmp=tmp->next;
    }
    return NULL;
    
    
    
}

//Inserts a Node in a sorted way of a Doubly linked list

void InsertAtPosition(struct list *lista, struct node *nodo, int *error, int (*compare_type) (struct node *, struct node *)) {
    
    #ifdef DEBUG
    printf("DEBUG: Method used to insert a Node at tail of the list");
    #endif
    struct node *aux= nodo;
    
    int cond = 0;
    struct node *temp= lista->head;
    
    struct node *temp2;
    
    if(lista->head == NULL) {						//case in which the list is empty
        
        InsertAtHead(lista, nodo , error );
        cond++;
        
    } else {
        
        while (temp != NULL  && cond == 0){ //While loop, used to go through the list
            
            
            if(compare_type(aux, temp)== 1){//If we have to insert at the right
                
                //There are only two posibilities: Insert a new tail, or a node in between of two nodes.
                
                if(temp == lista->tail){ //If we have to insert at the end (tamaño mayor que todos)
                    
                    InsertAtTail(lista,nodo, error);
                    cond++;
                    
                } else if(compare_type(aux, temp) == 1 && compare_type(aux, temp->next) == -1){ //Si el nodo está entre medias
                    
                    temp2 = temp->next; //Auxiliar variable that stores the next node
                    temp->next = aux;
                    temp2->prev = aux;
                    aux->prev = temp;
                    aux->next = temp2;
                    
                    lista->current = aux;
                    lista->size++;
                    cond++;
                    
                }
                
            } else { //We introduce a new head. The nodes has the same size or the new node is lower
                if(cond == 0){
                    
                    lista->head->prev = nodo;				//The previous of the head is the new node
                    nodo->next = lista->head; 				//The next of the new is the current head
                    lista->head = nodo;						//The newNode is now the Head of the list
                    lista->current = aux;
                    lista->size++;
                    cond++;
                }
            }
            temp = temp->next;
        }
    } 
}



int compare_id(struct node *first, struct node* second){
    
    int a = first->id;
    int b = second->id;
    
    if (a > b) {
        
        return 1;
        
    } else if (a == b) {
        
        return 0;
        
    } else {
        
        return -1;
    }
}

int compare_name(struct node *first,  struct node* second){

    if(first->name[0] > second->name[0]){
        
        return 1;
        
    } else if(first->name[0] < second->name[0]){
        
        return -1;
        
    } else { 
        
        if(first->name[1] > second->name[1]){
            
            return 1;
            
        } else if(first->name[1] < second->name[1]){
            
            return -1;
            
        } else { 
            
            return 0;
        }
    }
    
}

int compare_time(struct node *first, struct node* second){
    
    time_t a = first->mtime;
    time_t b = second->mtime;
    
    if ((int)a > (int)b) {
        
        return 1;
        
    } else if ((int)a == (int)b) {
        
        return 0;
        
    } else {
        
        return -1;
        
    }
}

int compare_size(struct node *first, struct node* second){
    
    off_t a = first->size;
    off_t b = second->size;
    
    if (a > b) {
        
        return 1;
        
    } else if (a == b) {
        
        return 0;
        
    } else {
        
        return -1;
        
    }
}
