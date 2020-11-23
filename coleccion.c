/*******************            coleccion.c            ************************************
 * 
 *  Fichero     coleccion.c
 * 
 *  Resumen     Methods releated with the collection
 * 
 *  Descripción This class has the methods releated with the collection.
 *			  Each time it is needed to modify the collection data, it
 *			  will be needed to use a method of this class.
 * 
 *  Autor       Francisco Atalaya and Jose María de La Cruz
 * 
 *  Copyright   [Copyright (c) 2012 Universidad Carlos III de Madrid
 * 
 *  Se concede, sin necesidad de acuerdo por escrito y sin ningún tipo de derechos
 *  de autor, permiso para utilizar, copiar, modificar y distribuir este programa
 *  y su documentación para cualquier propósito siempre y cuando esta advertencia
 *  y los siguientes dos párrafos aparezcan en las copias.
 * 
 *  EN NINGÚN CASO SE RECONOCERÁ A LA UNIVERSIDAD CARLOS III DE MADRID RESPONSABLE
 *  DIRECTA, INDIRECTA, ESPECIAL, O SUBSIDIARIA DE LOS POSIBLES DAÑOS Y PERJUICIOS
 *  QUE PUEDAN DERIVARSE DEL USO DE ESTE PROGRAMA Y DE SU DOCUMENTACIÓN, INCLUSO
 *  EN EL CASO DE QUE LA UNIVERSIDAD CARLOS III DE MADRID HAYA SIDO ADVERTIDA DE
 *  TALES DAÑOS Y PERJUICIOS.
 * 
 *  LA UNIVERSIDAD CARLOS III DE MADRID ESPECÍFICAMENTE SE DESENTIENDE DE TODO
 *  TIPO DE GARANTÍAS INCLUYENDO, PERO NO LIMITANDOLAS A, LAS GARANTIAS IMPLÍCITAS
 *  DE LA COMERCIALIZACIÓN Y ADECUACIÓN PARA CUALQUIER PROPÓSITO EN PARTICULAR. EL
 *  PROGRAMA SE PROVEE TAL CUAL Y LA UNIVERSIDAD CARLOS III DE MADRID NO TIENE
 *  OBLIGACIÓN ALGUNA DE OFRECER MANTENIMIENTO, SOPORTE, ACTUALIZACIONES, MEJORAS
 *  O MODIFICACIONES.]
 * 
 ******************************************************************************/
/*------------INCLUDES-----------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
/*------------OTHER CLASSES INCLUDES-----------*/
#include "node.h"
#include "coleccion.h"
#include "menu.h"
#include "error.h"
#include "list.h"
#include "util.h"
#include "bmp.h"
#include "operations.h"
#include "enhanced.h"
/*---------------------------------------------*/

struct options* opt;
/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/


struct list initialize_collection(struct list *my_collection, int *error){
    my_collection = newList(error);	//creates a new list which will be collection. Currently is unused
    return *my_collection;
}

void info_all(struct list* my_collection, int *error){
    
    if(my_collection->size==0){	//case collection is empty
        *error=EMPTY_COL;
        
    }else{  					//collection not empty
        printf("There are %d items in the loaded collection\n", my_collection->size);
        #ifdef DEBUG
        printf("DEBUG: Program introducding inprintList method \n");
        #endif
        
        printList(my_collection); //Method in list.c that prints a list
        
        printf("\n");
    }
    
}

struct node* load(struct list *my_collection, int type, char *nombre,int *error){ //opt->sort_type 
    //case 1 in switch of main.c
    //nombre is introduce by stdin in get_string(free required)
		#ifdef DEBUG
        printf("DEBUG: Program introducding load method \n");
        #endif
    
     int (*compare_type) (struct node *, struct node *); //call the pinter to function method 
    //checking the type of sorting introduced in int type
    if (type == 1) {
       
        compare_type= compare_size;
        
    } else if (type == 2) {
      
        compare_type = compare_name;
        
    } else if (type== 3) {
   
        compare_type = compare_time;
        
    } else if (type == 0) {
      
        compare_type = compare_id;
        
    } else {   
        compare_type = compare_id;
    }
    
    struct stat file;	//catch the info of the file searched with lstat
    struct node* nodo = NULL;	//node to save in the collection
    if(lstat(nombre,&file)==-1){  //checks if the file exists
        *error=FILE_NOT_FOUND;    //if it does not exist
        
        #ifdef DEBUG
        printf("DEBUG: File does not exist in the folder\n");
        #endif
        return NULL;
    } else if(checkifBMP(nombre, error) == -1){	//checking if the file introduced is a bmpfile
        return NULL;
          
    } else if(access(nombre, R_OK) != 0){  //BMP file can be readable and writtable
        *error = PERMISSIONS_ERROR;
        return NULL;
        
    } else {  						//if it exists
        
           
        #ifdef DEBUG
        printf("DEBUG: File exists in the folder. Cheking if it exists in the collection\n");
        #endif
        
        if(inListName(my_collection,nombre)==NULL){ //check if it is already in the collection
            //case it is not int the collection. It is added
            
            #ifdef DEBUG
            printf("DEBUG: File does not exist in the collection. Program introducing into InsertAtTail method\n");
            #endif
            
            nodo = createNode(nombre, error);
            
            //initialating the node pipes for communications and checking if they had been init correctly
            if(pipe(nodo->p) < 0 ){		//pipe comm: parent->child
                *error = PIPE_ERROR;
                return NULL;
            }
            
            if(pipe(nodo->p2) < 0 ){ 		//pipe comm: child->parent
                *error = PIPE_ERROR;
                return NULL;
            }
            
            //creating the proccess child with fork
            nodo->pid = fork();
            if(nodo->pid != 0){			
                if (nodo->pid == -1){	//checking if fork has been done correctly
                    *error = PID_ERROR;
                    return NULL;		//in case of error load returns a null
                    
                }
            }
            
            if(nodo->pid == 0){				//condition to stackchild proccesses in a while
                
                signal(SIGTSTP,SIG_IGN);
                signal(SIGUSR1,SIG_IGN);
                
				//free_opt(opt);				//only the child proccess arrive here
                BMPFILE* bmp_pic;			//BMPFILE to save the bmp file introduced
                int error=0;
                char receivedstring[1024];	//string to save the information given by the parent proccess
                char *resto;				//string to save the rest of the introduce string in the strtok_r
                char *operation;			//string to save the first strtok called which returns the operation as a char*
                char *aux;					//used to save some parameters in operations with strtok
                int op, bytes;				//the operation parsed into an int; the bytes read/written in the pipes
                int totalWords;				//once the strok_r is used for cut the operation selected, it is needed to count
											//the number of words the string rest have
                
                //Communication Chanels 1
                int in = nodo->p[0]; 	//read
                int  out = nodo->p[1];	//write
                
                //Communication Channels 2
                int in2 = nodo->p2[0];	//read
                int  out2 = nodo->p2[1];//write
                
                struct parameters *parametros;	//declaring the parameters used in the operations methods
                //Pointer to function
                PIXELS (*elegir_function)(PIXELS , struct parameters*  , int* );	//declaring pointer to function methods
                
                
                while(1){
                    
                    
                    /*if(nodo->checksenhal==1){
                        close(out);													//need to read-> closing write
                        bytes = read(in, &receivedstring, sizeof(receivedstring)); //reading the pipe and saving in the receivedstring
                    }*/
                    
                    close(out);													//need to read-> closing write
                    bytes = read(in, &receivedstring, sizeof(receivedstring)); //reading the pipe and saving in the receivedstring
                    
                    if(bytes == -1){	//if int return by read(bytes) is -1->error                     
                        error = errno;
                        
                    } else {
                       
                        operation = strtok_r(receivedstring," ", &resto);	//cutting the operation selected
																			//the rest of the command is saved in resto
                        
                        op = compare_operation(operation, &error);			//parsing the operation (Ex:histogram) into an int(Ex:1)
                        
                        if (op == -1){		//if compareo_peration has returned an error
							
                            //sending the error from child to parent
                            close(in2);										//closing the read
                            bytes = write(out2, &error, sizeof(&error));	//writing in the pipe
                            
                            if(bytes == -1){		//checking the write error
                                error = errno;
                            }
                            
                            error=0;				//cleaning th error
                            receivedstring[0] = 0;	//cleaning th received string
                            
                        } else {//in case of no error in compare_operation
                            
                            totalWords = contar_palabras(resto);	//we count the words in the string rest
                            
                            switch(op){	//switch with the operation selected(the parsed one)
                                
                                case 1:
                                    
                                    //Histogram
                                    
                                    if(totalWords == 1 && strcmp(resto,"")!=0){	//only one more word(filename) it is not empty
                                        
                                        parametros = (struct parameters*)malloc(sizeof(struct parameters)); //declaring the struct where the parameters of the operations methods are saved
                                        
                                        parametros->histogramaname = strdup(resto);	//as resto is directly the filename we introduce it into struct parameters
                                        
                                        bmp_pic = load_BMP(nodo->name, &error);	//charging the bmp file introduce in the struct BMPFILE
                                        
                                        
                                        if(bmp_pic != NULL && error == 0){	//case of error in load_BMP
                                            
                                            histogram(bmp_pic, parametros->histogramaname ,&error);	//compute the operation
                                        }
                                        
                                        free(parametros->histogramaname);	//free the string inside the struct parameters because of strdup
                                        freeBMP(bmp_pic);					//free the struct BMPFILE
                                        free(parametros);					//free the struct parameters
                                        
                                    } else {	//in case of rest contains more than one word
                                        
                                        error = EXCESS_ARGUMENTS;
                                        
                                    }
                                    
                                    break;
                                    
                                case 2:
                                    
                                    //Gray Scale
                                    
                                    if(totalWords == 1 && strcmp(resto,"")!=0){ //only one more word(filename) and it is not empty
                                        
                                        parametros = (struct parameters*)malloc(sizeof(struct parameters)); //declaring the struct where the parameters of the operations methods are saved
                                        
                                        if ((strcmp(resto, "r") == 0) || (strcmp(resto, "g") == 0) || (strcmp(resto, "b") == 0) || (strcmp(resto, "y") == 0)){	//checking if the parameter is correct
                                            
                                            parametros->channel = strdup(resto);	//as resto is directly the type we introduce it into struct parameters
                                            
                                            bmp_pic = load_BMP(nodo->name, &error);	//charging the bmp file introduce in the struct BMPFILE
                                            
                                            if(bmp_pic!= NULL && error == 0){		//no error
                                                
                                                elegir_function=graychannels;		//initialating the pointer to function
                                                
                                                bmp_pic=recorrer_array(bmp_pic, parametros, &error, elegir_function);//compute the operation
                                                
                                                
                                                if(bmp_pic!=NULL){							//if operation applying has ben done correctly
                                                    
                                                    save_BMP(bmp_pic,nodo->name, &error);	//saving the BMPFILE struct in the bmp file
                                                    
                                                } else {	//case of error in load_BMP
                                                    
                                                    free(bmp_pic);	//free the BMPFILE
                                                }
                                            }
                                            
                                            free(parametros->channel);	//free the string channel in the struct parameters
                                            
                                            
                                        } else {	//case wrong parameter introduced
                                            
                                            error = GRAY_ERROR;
                                            
                                        }
                                        
                                        free(parametros); //free the struct parameters
                                        
                                    } else {	//case more than one parameter introduced in the command
                                        error = EXCESS_ARGUMENTS;
                                        
                                    }
                                    
                                    break;
                                    
                                case 3:
                                    
                                    
                                    // Bitonal
                                    if(totalWords == 7){ //it is needed seven words because bitonal use seven parameters
                                        
                                        parametros = (struct parameters*)malloc(sizeof(struct parameters)); //declaring the struct where the parameters of the operations methods are saved
                                        //the array of bitonal follows the next schema: [Rd, Gd, Bd, Rl, Gl, Bl, Threshold]
                                        
                                        int i;
                                        
                                        //for first case because of strtol(needs the string)
                                        aux=strtok(resto, " ");	//first strtok to give it the string resto. It returns the first parameter of bitonal(red dark)
                                        
                                        int check= transformarstrtol(aux, 16, &error);	//parsing the parameter into an int
                                        
                                        if(check != -1 && check < 256){	//checking the error of transformarstrtol and if the parametrs is less than 255
                                            
                                            parametros->arrayBitonal[0]=check;	//saving the parameter into the bitonal array
                                            
                                        } else {
                                            
                                            error = BITONAL_ERROR;
                                            
                                        }
                                        
                                        if (error == 0){
                                            //for cases[1,7]
                                            for(i = 1; i < 6; i++){
                                                
                                                if(error == 0){
                                                    
                                                    aux = strtok(NULL, " ");	//now we can call strtok with NULL
                                                    
                                                    
                                                    int check = transformarstrtol(aux, 16, &error);	//parsing the parameter into an int
                                                    
                                                    if(error == 0){
                                                    
														if(check != -1 && check < 256){	//checking the error
															
															parametros->arrayBitonal[i]=check;	//saving in the corresponding index of the bitonal array
															
														} else {
															
															error = BITONAL_ERROR;
															
														}
                                                    }
                                                }
                                                
                                            }
                                            
                                        }
                                        
                                        //threshold done apart because the function transformarstrtol is called with decimal base
                                        if(error == 0){//in case of error before
                                                    
											aux=strtok(NULL, " ");//cutting the threshold
                                                   
                                            int check= transformarstrtol(aux, 10, &error);//parsing the parameter into an int
                                            
                                            if(check != -1 && check < 256){	//checking the error
                                                        
												parametros->arrayBitonal[6]=check;	//saving in the corresponding index of the bitonal array
                                                        
                                                } else {
                                                        
													error = BITONAL_ERROR;
                                                        
                                                }
											}
                                        
                                        
                                        if(error == 0){//if there are not any error in while filling the bitonal array
                                            bmp_pic = load_BMP(nodo->name, &error);//charging the bmp file introduce in the struct BMPFILE
                                            
                                            if(bmp_pic!=NULL && error == 0){	//if load_BMP hasn't an error
                                                
                                                elegir_function = bitonal;		//initialize the pointer to function
                                                bmp_pic = recorrer_array(bmp_pic, parametros, &error, elegir_function);	//compute the operation
                                                
                                                if(bmp_pic!=NULL){	//in case of no error while doin ghte operation
                                                    
                                                    save_BMP(bmp_pic,nodo->name, &error);
                                                    
                                                } else {	//if error while doing the poeration->free the BMPFILE struct
                                                    
                                                    free(bmp_pic);
                                                }
                                                
                                            }
                                            
                                            
                                        }
                                        
                                        
                                        free(parametros);		//free the struct parameters
                                        
                                        
                                        
                                    } else {
                                        
                                        error = EXCESS_ARGUMENTS;
                                        
                                    }
                                    
                                    break;
                                    
                                    
                                    case 4:
                                        
                                        //rotate
                                        
                                        
                                        if(totalWords == 2){	//rotate needs two arguments (direction and angle)
                                            
                                            parametros = (struct parameters*)malloc(sizeof(struct parameters)); //declaring the struct where the parameters of the operations methods are saved
                                            
                                            
                                            //for first case because of strtol(needs the string)
                                            aux=strtok(resto, " ");	//first strtok to give it the string resto. It returns the first parameter of rotate(direction)
                                            
                                            if((strcmp(aux, "r") == 0) || (strcmp(aux, "l") == 0)){	//checking if the first parameter is correct
                                                
                                                parametros->sense = strdup(aux);	//as resto is directly the filename we introduce it into struct parameters
                                                
                                                aux=strtok(NULL, " ");				//taking the last parameter of the command
                                                
                                                int angle = transformarstrtol(aux, 10, &error);	//parsing the last parameter
                                                
                                                if(angle == 90 || angle == 180 || angle == 270){	//checking if the second parameter is correct
                                                    
                                                    parametros->angle = angle;	//saving the parameter in the struct if it is correct
                                                    
                                                } else {	//the second parameter is not correct->error
                                                    
                                                    error = ROTATE_ERROR;
                                                    
                                                }
                                                
                                            } else {	//the first parameter is not correct
                                                
                                                error = ROTATE_ERROR;
                                            }
                                            
                                            if (error == 0){
                                                
                                                bmp_pic = load_BMP(nodo->name, &error);//charging the bmp file introduce in the struct BMPFILE
                                                
                                                if(bmp_pic!=NULL && error == 0){	//checking if error in load_BMP
                                                    
                                                    bmp_pic = rotate(bmp_pic, parametros->sense, parametros->angle, &error); //compute the operation
                                                    
                                                    if(bmp_pic!=NULL){//checking if error in the operation
                                                        
                                                        save_BMP(bmp_pic, nodo->name, &error);	//savinf the BMPFILE struct into the bmp file
                                                        
                                                    }else {
                                                        free(bmp_pic);	//in case of error: free needed
                                                    }
                                                    
                                                }
                                            }
                                           				
                                            free(parametros->sense);	//free of the parameter in struct parameters
                                            free(parametros);			//free the struct parameters
                                            
                                            
                                        } else {
                                            
                                            error = EXCESS_ARGUMENTS;
                                            
                                        }
                                        
                                        break;
                                        
                                        case 5:
                                            
                                            //only one
                                            if(totalWords == 1 && strcmp(resto,"")!=0){ //only one more word(filename) it is not empty
                                                
                                                parametros = (struct parameters*)malloc(sizeof(struct parameters)); //declaring the struct where the parameters of the operations methods are saved
                                                
                                                if ((strcmp(resto, "r") == 0) || (strcmp(resto, "g") == 0) || (strcmp(resto, "b") == 0) || (strcmp(resto, "y") == 0)){	//checking if the parameter is correct
                                                    
                                                    parametros->onlyonechannel = strdup(resto);	//saving the parameter in the struct
                                                    
                                                    bmp_pic = load_BMP(nodo->name, &error);		//charging the bmp file introduce in the struct BMPFILE
                                                    
                                                    if(bmp_pic!= NULL && error == 0){	//no error in load_BMP
                                                        
                                                        elegir_function=onlyone;		//initialating pointer to function
                                                        
                                                        bmp_pic=recorrer_array(bmp_pic, parametros, &error, elegir_function); //compute the operation
                                                        
                                                        
                                                        if(bmp_pic!=NULL){	//if no error while doing the operation
                                                            
                                                            save_BMP(bmp_pic,nodo->name, &error);	//save the BMPFILE struct in the bmp file
                                                            
                                                        }else {	//in case of error-> free needed
                                                            free(bmp_pic);
                                                        }
                                                    }
                                                    
                                                    free(parametros->onlyonechannel);	//free of the parameter svaed in the struct parameters
                                                    
                                                    
                                                    
                                                    
                                                } else {	//error: parameter not correct
                                                    
                                                    error = ONLYONE_ERROR;
                                                    
                                                }
                                                
                                                free(parametros);	//free of the struct parameters
                                                
                                            } else {//error: not enought parameters (or more than needed)
                                                
                                                error = EXCESS_ARGUMENTS;
                                                
                                            }
                                            
                                            break;
                                            
                                        case 6:
                                            
                                            //Black and white
                                            
                                            if(totalWords == 1 && strcmp(resto,"")==0){ //only one more word(filename) it is not empty
                                                
                                                parametros = (struct parameters*)malloc(sizeof(struct parameters)); //declaring the struct where the parameters of the operations methods are saved
                                                
                                                bmp_pic = load_BMP(nodo->name, &error);	//charging the bmp file introduce in the struct BMPFILE
                                                
                                                if(bmp_pic != NULL && error == 0){	// no error while load_BMP
                                                    
                                                    parametros->channel =strdup( "y");	//saving the parameter i the struct
                                                    
                                                    elegir_function=graychannels; //initialating pointer to fucntion
                                                    bmp_pic = recorrer_array(bmp_pic, parametros, &error, elegir_function); //compute the operation
                                                    
                                                    parametros->total_pixels = bmp_pic->ih.biHeight * bmp_pic->ih.biWidth; //total number of pixels in the image
                                                    
                                                    if(bmp_pic!=NULL && error == 0){	//if errorwhile computing the operation
                                                        
                                                        parametros = PixelsB_W(bmp_pic, parametros, &error);	
                                                        
                                                        if(parametros!=NULL && error == 0){
                                                            
                                                            elegir_function = otsu_b_w;
                                                            bmp_pic=recorrer_array(bmp_pic, parametros, &error, elegir_function); //compute the operation
                                                            
                                                            if(bmp_pic != NULL){
                                                                save_BMP(bmp_pic, nodo->name, &error);
                                                                
                                                            }else {
                                                                free(bmp_pic);
                                                            }
                                                            
                                                            
                                                        }
                                                        
                                                    }
                                                    
                                                    
                                                }
                                                
                                                free(parametros->channel);
                                                free(parametros);
                                                
                                                
                                            } else {
                                                
                                                error = EXCESS_ARGUMENTS;
                                                
                                            }
                                            
                                            break;
                                            
                                            case 7:
                                                
                                                //Mirror
                                                
                                                if(totalWords == 1 && strcmp(resto,"")!=0){ //only one more word(filename) it is not empty
                                                    
                                                    parametros = (struct parameters*)malloc(sizeof(struct parameters)); //declaring the struct where the parameters of the operations methods are saved
                                                    
                                                    if((strcmp(resto, "h") == 0) || (strcmp(resto, "v") == 0)){
                                                        
                                                        parametros->direction = strdup(resto);	//as resto is directly the type we introduce it into struct parameters
                                                        
                                                        bmp_pic = load_BMP(nodo->name, &error);	//charging the bmp file introduce in the struct BMPFILE
                                                        
                                                        if(bmp_pic!=NULL && error == 0){
                                                            
                                                            bmp_pic = mirror(bmp_pic, parametros->direction, &error); //compute the operation
                                                            
                                                            if(bmp_pic!=NULL && error == 0){
                                                                
                                                                save_BMP(bmp_pic, nodo->name, &error);
                                                                
                                                            }else {
                                                                free(bmp_pic);
                                                            }
                                                            
                                                        }
                                                        
                                                        free(parametros->direction);
                                                        
                                                        
                                                        
                                                    } else {
                                                        
                                                        error = MIRROR_ERROR;
                                                    }
                                                    
                                                    free(parametros);
                                                    
                                                } else {
                                                    
                                                    error = EXCESS_ARGUMENTS;
                                                    
                                                }
                                                
                                                break;
											
										
                            }
                            
                            close(in2);
                            bytes = write(out2, &error, sizeof(&error));
                            
                            if(bytes == -1){
                                
                                error = errno;
                                
                            }
                            
                            
                            error=0;
                            receivedstring[0] = 0;
                        }
                    }
                }
            
            }
            
            //createNode creates a new struct node gicen its name and returns a pointer to it. InsertAtTail insert at the
            //end of the list the pointer to a node
             
            nodo->size = file.st_size;	//taking this values for the info
            nodo->mtime = file.st_mtime;
           
            
            FILE *file_ptr = fopen(nombre, "rb"); //open reading in binary the file
            
            if (!file_ptr) {
                *error = errno;
                return NULL;        //We check if there is image or not
                
            }
            
            int check;
            check= fseek(file_ptr, 18, SEEK_CUR);  //jump 18 to reach biWidth
            
            if(check == -1){//checking fseek error
                *error = errno;
                fclose(file_ptr);
                return NULL;
            }
            
            check = fread(&(nodo->biWidth), sizeof(int), 1, file_ptr); //saving Width
            
            if (check != 1){  //checking fread error
                
                *error = FREAD_ERROR;
                fclose(file_ptr);
                return NULL;
                
            }
            check = fread(&(nodo->biHeight), sizeof(int), 1, file_ptr); //saving Height
            
            if (check != 1){//checking fread error
                
                *error = FREAD_ERROR;
                
                fclose(file_ptr);
                return NULL;
                
            }
            
            check= fseek(file_ptr, 2, SEEK_CUR);  //jump 2 bytes(short  biPlanes)to reach biBitCount
            
            if(check == -1){//checking fseek error
                *error = errno;
                fclose(file_ptr);
                return NULL;
            }
            
            check = fread(&(nodo->biBitCount), sizeof(short), 1, file_ptr);
            
            if (check != 1){//checking fread error
                *error = FREAD_ERROR;
                fclose(file_ptr);
                return NULL;
                
            }
            
            InsertAtPosition(my_collection, nodo, error, compare_type);
            fclose(file_ptr);
            
           }else{
            
            
            
            //case it is already in the collection. It is printed
            #ifdef DEBUG
            printf("DEBUG: File exist in the collection. Program introducing into info method\n");
            #endif
            
            printf("The file is in the collection\n");
            printNode(inListName(my_collection,nombre)); //method in node.c and list.c
            
            //inListName search for the node given its name and returns a pointer to that node
            //printNodeprints the information(name and parameters of struct stat)
            
            
            return NULL;
        }
        return nodo;
    }
    
}




void Delete(struct list* my_collection, int id, int*error){
    //case 2 in switch of main.c
    //the line below search a node in the list given its ID and return a pointer
    //to it. THen aux is used to delete the node
    
    
    struct node* aux=inListId(my_collection,id);
    #ifdef DEBUG
    printf("DEBUG: Check if the file is in the collection\n");
    #endif
    if(aux==NULL){      //if the id is not in the collection
        #ifdef DEBUG
        printf("DEBUG: the file is in the collection\n");
        #endif
        *error=ID_NOT_FOUND;
    }else{
        #ifdef DEBUG
        printf("DEBUG: the file is not in the collection. Program introduce in delete_node method\n");
        #endif
        
        kill(aux->pid, SIGTERM);    //First we kill the process before deleting the node.
        waitpid(aux->pid,NULL, 0);
        
        delete_node(my_collection,aux, error); //method in list.c
        
        
        
        //given a pointer to a list and a pointer to a node, the method delete the node
        //from thar list
    }
    
    
}

void free_coleccion(struct list* my_collection){
    //used before exit the program
    //With two auxiliar pointers to different nodes we travel the list and free the
    //name of the nodes and the nodes themselves
    #ifdef DEBUG
    printf("DEBUG: Liberating the collection\n");
    #endif
    struct node* current;
    struct node* tmp = my_collection->head;
    
    for(int i=0;i<my_collection->size;i++){
        current=tmp;
        tmp= tmp->next;
        
        kill(current->pid, SIGTERM);    //First we kill the process before deleting the node.
        waitpid(current->pid,NULL, 0);
        
        free(current->name);
        free(current);
        
    }
    
    my_collection->head=NULL;
    my_collection->tail=NULL;
    my_collection->current=NULL;
    my_collection->size=0;
    
}

void save_coleccion(struct list* my_collection,char *nombre, int *error){
    //case 5 in switch of main.c
    
    
    FILE *file;								 //create a pointer to a file
    struct node * tmp=my_collection->head;	 //create an auxiliar node to travel the list
    file = fopen(nombre, "w");	//the file is opened if exists, if not it is created
    #ifdef DEBUG
    printf("DEBUG: saving the collection...\n");
    #endif
    //loop that copies the collection (names of the nodes) in the file
    for(int i=0;i<my_collection->size;i++){
        fprintf(file, tmp->name);
        fprintf(file,"\n");
        tmp=tmp->next;
    }
    
    fclose(file); //we close the file
    
    
}

void load_coleccion(struct list* my_collection,int type, char* filename, int* error){
    //case 6 in switch of main.c
    
    size_t buffer_size = 80;  							//used in getline
    char *buffer = malloc(buffer_size * sizeof(char)); 	//string returned by getline
    #ifdef DEBUG
    printf("DEBUG: opening the file\n");
    #endif
    char* option=NULL;				//string thtat saves teh yes/no decition of overwritting the file
    FILE *file=fopen(filename,"r"); //open the file with the filename introduced by get_string in main
    
    if(file==NULL){ //file does not exist in the folder
        #ifdef DEBUG
        printf("DEBUG: the file is not in the folder\n");
        #endif
        
        *error=FILE_NOT_FOUND;
    }else{
        #ifdef DEBUG
        printf("DEBUG: the file is in the folder\n");
        #endif
        if(my_collection->size==0){
            //empty collection: the collection charges directly
            
            //the loop travels the file line by line and introduce each line in the string buffer
            //then a load is used for introduce the file with name buffer in the collection
            
            
            #ifdef DEBUG
            printf("DEBUG: Empty collection. The collection charges directly\n");
            #endif
            while(getline(&buffer, &buffer_size, file)!=-1){
                buffer[strlen(buffer)-1]= '\0';
                load(my_collection, type ,buffer,error);
                
                if(*error!=0){
                    *error=0;
                    printf("The loaded file contains one or more elements that does not exist. The rest are loaded\n");
                }
            }
            
            
        }else{
            
            printf("Do you want to discard the current collection and save the one in the file?(yes/no): ");
            
            option =get_string(error);
            
            if(strcmp(option, "yes")==0){ //Decide sobreescribir
                #ifdef DEBUG
                printf("DEBUG: Overwritting accepted. Liberating the collection and charging the new one\n");
                #endif
                //deletes all the current collection
                free_coleccion(my_collection);
                
                //add filename's nodes as before
                
                while(getline(&buffer, &buffer_size, file)!=-1){
                    buffer[strlen(buffer)-1]= '\0';
                    load(my_collection,type,buffer,error);
                    
                    if(*error<0){
                        *error=0;
                        printf("The loaded file contains one or more elements that does not exist. The rest are loaded\n");
                    }
                    
                    
                }
                
                
            }else if(strcmp(option, "no")==0){
                //it goes to the menu if the user decise not to overwrite
                #ifdef DEBUG
                printf("DEBUG: Overwritting denied.Coming back to the menu\n");
                #endif
                
                
            }else{
                *error=ERR_NO_NUMBERS_INPUT; //despite it says number it is valid for any wrong input
            }
        }
        
        fclose(file); //we close the file opened
    }
    free(option); //from get_string (colateral effects)
    fflush(stdout);
    free(buffer);
    
    
    
}
