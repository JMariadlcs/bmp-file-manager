
/**Main.c***********************************************************************

  Fichero     main.c

  Resumen     In this class is where the program runs. All the principal methods
			  are called here.

  Descripción A while loop is running and calling the methods that the user order
			  untill an exit condition is fulfilled

  Autor       Francisco Atalaya, Jose María de La Cruz, Iosif Mondoc and Patricia Paredes


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
#include <errno.h>
#include <string.h>
#include <signal.h>

/*------------OTHER CLASSES INCLUDES-----------*/

#include "node.h"
#include "coleccion.h"
#include "menu.h"
#include "util.h"
#include "error.h"
#include "operations.h"
#include "bmp.h"
#include "enhanced.h"


/*---------------------------------------------------------------------------*/
/* Declaración de estructuras                                                */
/*---------------------------------------------------------------------------*/


struct list my_collection={NULL,NULL,NULL,0};
struct node* nodo=NULL;
struct options* opt=NULL;

/*---------------------------------------------------------------------------*/
/* Declaración de variables                                                  */
/*---------------------------------------------------------------------------*/

char *string;

/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/


void signal_handlers(int senhal){ //is function is the one used to manage the singals

    
    if(senhal == SIGINT){ 		             //CONTROL+C we go out from the program
    
        printf("\nCTRL+C pressed\n");
        free_coleccion(&my_collection); //free the collection before it exits
        free_opt(opt);                  //free the struct options
        if(string != NULL){         
            free(string);                //free the string
        }
        
        exit (0);						//program finished
        
    }
    
    if(senhal == SIGTERM) 		             //SIGTERM signal 
    {
        printf("\nChild was killed\n");    
        if(string!=NULL){
            free(string);                   //free the string 
        }
        exit (0);						//program finished
    }
    
    if(senhal == SIGTSTP){
        
        printf("Collection sorted by the name of the file");
        
        //Funcion sort not implemented
        
        
        printf("\nEnter an option (type help for help): ");
        printf("\n");
    }
    
    if(senhal == SIGUSR1){
        
        printf("Todos los hijos con grayscale\n");
        
        struct node *current = nodo;
        
        
        char *cadena = "grayscale y";
        
        while (current != NULL){
            
            //nodo->checksenhal=1;
            close(current->p[0]);
            write(current->p[1], cadena, (strlen(cadena)+1));
            
            
            
            current = current->prev;
            
        }
        
        
        
        printf("\nEnter an option (type help for help): ");
        printf("\n");
        
    }
    
}

int main(int argc, char **argv){
    
    int error = 0; 					//the error managed in funcion get_number
    
    //initialitation of signal handlers
    signal(SIGINT,signal_handlers);
    signal(SIGTERM,signal_handlers); 
    signal(SIGTSTP,signal_handlers); 
    signal(SIGUSR1,signal_handlers); 
    
    //controling parameters introduced by the user
    opt = (struct options*)malloc(sizeof(struct options));	
    opt = fillStructOpt(argc,argv,opt,&error);			      //filling the struct with the parameters introduced
    
    if(error != 0){				//in case of error introducing command
        
        printf("%s \n", errormanage(error));
        free_opt(opt);                       //free the struct options
        return 0;
        
    } else if(opt->help == 1){		//in case of introducing --help
        
        printParamMenu();
        free_opt(opt);             //free the struct options
        return 0;
        
    } else {
        printParam(opt);		//no exit -> printing parameters introduced by the user
    }
    
    if(opt->i == 1){				//initial collection selected
        
        load_coleccion(&my_collection, opt->sort_type, opt->initfile, &error);
        
        if(error != 0)
            printf("%s \n", errormanage(error));
        
    }
    
    
    printf("\nEnter an option (type help for help): ");
    
    char* option_str;
    char* resto=NULL;
    
    int option=0;   //the associated number of the option(load = 1)
    
    //We ask a string with the options and arguments (whole command)
    char* total_option = get_string(&error);	
    
    if(error == 0){ 
        
        option_str=strtok_r(total_option," ",&resto);  //we extract the name of the option(load, info, infoall...)
        
        option= compare_option(option_str,&error);   //returns the number of the options or -1 in case is wrong written
        
    }
    
    while (option !=0 || (option == 0 && error != 0)){   //while loop for the options and CTRLD INPUT
        
        int nWords=contar_palabras(resto);  //returns the number of arguments after the options
        
        if (error == 0){  // for the options
            
            
            #ifdef DEBUG
            printf("DEBUG: It introduce to the selected option\n");
            #endif
            
            
            switch(option){
                
                case 1:{	//Load
                    
                    if(nWords == 1 && strcmp(resto,"") != 0){ //there are one argument and is not empty
                        
                        char* name = strtok_r(NULL," ",&resto);     //we store the name of the file
                        
                        #ifdef DEBUG
                        printf("DEBUG: It introduce to load method\n");
                        #endif
                        
                        nodo =  load(&my_collection,opt->sort_type ,name, &error); //from coleccion.c (load the file if it is not yet)
                        
                        printf("%s \n", errormanage(error));    
                        
                        
                    } else {   //there are more or less arguments than the needed
                        
                        error=EXCESS_ARGUMENTS;
                        printf("%s \n", errormanage(error));
                        
                    }
                    
                    error = 0;
                    
                    break;
                    
                    
                }
                
                case 2:{	//Delete
                    
                    if(nWords == 1 && strcmp(resto,"") != 0){    //there are one argument and is not empty
                        
                        char* id_str = strtok_r(NULL," ",&resto);    //we store the id
                        int id = transformarstrtol(id_str, 10, &error);	//we check if it is a number or it handles the error
                        
                        #ifdef DEBUG
                        printf("DEBUG: It introduce to delete method\n");
                        #endif
                        
                        if(error != 0){
                            
                            printf("%s \n", errormanage(error));
                            
                        } else {
                            
                            Delete(&my_collection,id, &error);    //from coleccion.c (deletes the node of the collection by id introduced)
                            printf("%s \n", errormanage(error));
                            
                        }
                        
                    } else {      //there are more or less arguments than the needed
                        
                        error = EXCESS_ARGUMENTS;
                        printf("%s \n", errormanage(error));
                        
                    }
                    break;
                }
                
                
                case 3:{	//Info
                    
                    if(nWords == 1 && strcmp(resto,"") != 0){        //there are one argument and is not empty
                        
                        char* id2_str = strtok_r(NULL," ",&resto);           //we store the id
                        int id2 = transformarstrtol(id2_str, 10, &error);	//we check if it is a number or it handles the error
                        
                        if(error != 0){
                            
                            printf("%s \n", errormanage(error));
                            
                        } else {
                            
                            #ifdef DEBUG
                            printf("DEBUG: It introduce to info method\n");
                            #endif
                            
                            info(&my_collection,id2,&error); //from node.c (print the information of a single node by its id)
                            
                            if(error!=0){
                                printf("%s \n", errormanage(error));
                            }
                        }
                        
                    } else {       //there are more or less arguments than the needed
                        
                        error = EXCESS_ARGUMENTS;
                        printf("%s \n", errormanage(error));
                        
                    }
                    break;
                }
                
                
                case 4:{	//Infoall
                    
                    if(nWords == 1 && strcmp(resto,"") == 0){       //there are no arguments and we check that is empty
                        
                        error = 0;
                        
                        #ifdef DEBUG
                        printf("DEBUG: It introduce to infoall method\n");
                        #endif
                        
                        info_all(&my_collection,&error); //from coleccion.c (print the information of every nodes in the list)
                        
                        if(error != 0){
                            
                            printf("%s \n", errormanage(error));
                            
                        }
                        
                    } else {    //there are more or less arguments than the needed
                        
                        error=EXCESS_ARGUMENTS;
                        printf("%s \n", errormanage(error));
                        
                    }
                    
                    break;
                }
                
                
                case 5:{	//savecol
                    
                    if(nWords == 1 && strcmp(resto,"") != 0){      //there are one argument and is not empty    
                        
                        char* option = NULL;
                        char* name = strtok_r(NULL," ",&resto);   //we store the name of the file
                        
                        if(error == 0){
                            
                            if(!access(name, F_OK )){		//if file exists
                                //case: file exists
                                
                                if(!access( name, W_OK )){	//if file can be written
                                    //case: file can be written
                                    
                                    printf("The File %s was found\n",name);
                                    printf("Do you want it to be overwritten? (yes/no): ");
                                    
                                    option =get_string(&error);		//gets the yes or no option
                                    
                                    if(strcmp(option, "yes")==0){
                                        //case:yes, the file is overwritten
                                        
                                        printf("Overwritting...Done!\n");
                                        
                                        #ifdef DEBUG
                                        printf("DEBUG: File exists. Yes introduced. Programm introduce to save collection method\n");
                                        #endif
                                        save_coleccion(&my_collection, name, &error); //from coleccion.c (save the current collection in a file)
                                        
                                        
                                    } else if(strcmp(option, "no") == 0){
                                        //case: no, the file is not overwritten
                                        
                                        #ifdef DEBUG
                                        printf("DEBUG: No introduced\n");
                                        #endif
                                        printf("Operation cancelled\n");
                                        
                                    } else {
                                        //case: input error
                                        
                                        #ifdef DEBUG
                                        printf("DEBUG: Invalid input\n");
                                        #endif
                                        error=ERR_NO_NUMBERS_INPUT;//an invalid input has been introduced
                                        printf("%s \n", errormanage(error));
                                        
                                    }
                                }
                                
                            } else {
                                //case: file does not exists, it creates it
                                
                                #ifdef DEBUG
                                printf("DEBUG: File doesnt exists. Introducing to save collection\n");
                                #endif
                                
                                printf("That file does not exist already. Creating the file...Done!\n");
                                save_coleccion(&my_collection, name, &error);
                                
                            }
                            
                        } else {   //if error != 0
                            
                            printf("\n");
                            printf("%s \n", errormanage(error));
                            
                        }
                        
                        free(option);        //free teh string yes or no
                        
                    } else {              //there are more or less arguments than the needed
                        
                        error=EXCESS_ARGUMENTS;
                        printf("%s \n", errormanage(error));
                        
                    }
                    
                    break;
                }
                
                
                case 6:{	//loadcol
                    
                    if(nWords == 1 && strcmp(resto,"") != 0){    //there are one argument and is not empty
                        
                        char* collection_file = strtok_r(NULL," ",&resto);    // we store the name of the file
                        
                        //the collection
                            #ifdef DEBUG
                            printf("DEBUG: It introduce to load coleccion method\n");
                            #endif
                            
                            load_coleccion(&my_collection,opt->sort_type,collection_file,&error); //from coleccion.c(load a collection from a file)
                            printf("%s \n", errormanage(error));
                        
                        
                    } else {    //there are more or less arguments than the needed
                        
                        error=EXCESS_ARGUMENTS;
                        printf("%s \n", errormanage(error));
                        
                    }
                    
                    break;
                }
                
                
                case 7:{	//help_ops
                    
                    if(nWords == 1 && strcmp(resto,"") == 0){   //there are one argument and is not empty
                        
                        menuOperations();   //prints the small menu of the operations with its respectively arguments expected
                        
                    } else {      //there are more or less arguments than the needed
                        
                        error=EXCESS_ARGUMENTS;
                        printf("%s \n", errormanage(error));
                        
                    }
                    break;
                }
                
                
                case 8:{	//addop
                    
                    if(strcmp(resto,"")!=0){        //if the arguments are not empty
                        
                        int in, out, in2,out2;      //declaration of the channels of communitacion
                        
                        char* id3_str = strtok_r(NULL," ",&resto);    // we store the id of the nodo
                        
                        int id3 = transformarstrtol(id3_str, 10, &error);	//we check if it is a number or it handles the error
                        
                        if(error == 0){  //if the id is right written 
                            
                            struct node *aux = inListId(&my_collection, id3);  //search in the list that id
                            
                            if(aux == NULL){   //if the id is not in the collection
                                
                                error = ID_NOT_FOUND;
                                printf("%s \n", errormanage(error));
                                
                            } else {       //the id is in the collection
                                
                                
                                in = aux->p[0]; //read
                                out = aux->p[1];  //write
                                
                                in2 = aux->p2[0]; //read
                                out2 = aux->p2[1];  //write
                                
                                char *cadena = strdup(resto);  //copy all the arguments and the name of the operation in the string
                                
                                if(error == 0){ 
                                    
                                    //CONNECTION FATHER-CHILD
                                    close(in);    //close the read side of the pipe 
                                    int nbytes;
                                    nbytes = write(out, cadena, (strlen(cadena)+1));  //we send the string to the child process
                                    
                                    if(nbytes == -1){  //we check if write has an error
                                        
                                        error = errno;
                                        free(cadena);           //free the string of the arguments and name of the operation
                                        
                                    } else {
                                        
                                        free(cadena);          //free the string of the arguments and name of the operation
                                        
                                        //CONNECTION CHILD-FATHER
                                        close(out2);   //close the write side of the pipe
                                        nbytes = read(in2,&error, sizeof(&error));    //we read the error sended by the child process
                                        
                                        if(nbytes == -1){    //we check if read has an error
                                            
                                            error = errno;
                                            
                                        } 
                                        
                                        printf("%s \n", errormanage(error));
                                    }
                                    
                                } else {
                                    
                                    free(cadena);
                                    printf("%s \n", errormanage(error));
                                    
                                }
                                
                            }
                            
                        } else {
                            
                            printf("%s \n", errormanage(error));
                        }
                        
                    } else {    //there is no arguments
                        
                        error = EXCESS_ARGUMENTS;
                        printf("%s \n", errormanage(error));
                        
                    }
                    
                    break;
                }
                
                case 9:{	//logon
                    
                    if(nWords == 1 && strcmp(resto,"") == 0){   //we check there is no arguments
                        
                        printf("Logon available very soon!\n");
                        
                    } else {    //there are more or less arguments
                        
                        error = EXCESS_ARGUMENTS;
                        printf("%s \n", errormanage(error));
                        
                    }
                    break;
                }
                
                case 10:{	//logoff
                    
                    if(nWords == 1 && strcmp(resto,"") == 0){     //we check there is no arguments
                        
                        printf("Logoff available very soon!");
                        
                    } else {     //there are more or less arguments
                        
                        error = EXCESS_ARGUMENTS;
                        printf("%s \n", errormanage(error));
                        
                    }
                    break;
                }
                
                case 11:{	//help
                    
                    if(nWords == 1 && strcmp(resto,"") == 0){    //we check there is no arguments
                        
                        short_menu();      //we print the options availables
                        
                        printf("More information (Y/N): ");
                        
                        char* opt = get_string(&error);   //the user introduces Y or N
                        
                        if(error == 0){
                            
                            if(strcmp(opt,"Y") == 0){
                                
                                extended_menu();    //if yes the extended menu is printed
                                
                            } else if(strcmp(opt,"N") == 0){
                                
                            } else { //if Y or N is not introduced
                                
                                error=ERR_NO_NUMBERS_INPUT;
                                printf("%s \n", errormanage(error));
                                
                            }
                            
                        } else {   //string written is not right
                            
                            printf("%s \n", errormanage(error));
                            
                        }
                        
                        free(opt);   //free of the struct options
                        
                    } else {   //there is more or less arguments than needed
                        
                        error = EXCESS_ARGUMENTS;
                        printf("%s \n", errormanage(error));
                        
                    }
                    
                    break;
                }
                
                default:{      //if the options is not one of the availables
                    
                    printf("\nWrong command!\n");
                    
                    break;
                }
                
            }    //end of swich of options
            
            
        } else {                     //if the string of the beginning has an error 
            
            if(error == ERR_CTRLD_INPUT){  //if CTRLD is introduced, the option is 0
                
                printf("%s \n", errormanage(error));
                
                char* save = NULL;        
                
                #ifdef DEBUG
                printf("DEBUG: CTRLD has been introduced\n");
                #endif
                
                if(opt->no_save == 1){	//if it is introduced in commands
                    
                    printf("Exit without saving the collection\n");
                    
                } else {
                    
                    while(error!=0){
                        
                        error=0;
                        printf("Do you want to save the collection? (yes/no): ");
                        save = get_string(&error);   //the user intorudce yes or no
                        
                        if(error==0){
                            
                            if(strcmp(save, "yes") == 0){
                                
                                
                                //save the collection(same structure of save_coleccion, case 5 in the switch avobe)
                                
                                char* option = NULL;
                                char* name;
                                
                                if(opt->o == 1){		//name introduced in commands
                                    
                                    name=strdup(opt->savefile);
                                    
                                } else {
                                    
                                    printf("Please, introduce a FILE NAME: ");
                                    name =get_string(&error);  //gets the filename from stdin
                                    
                                }
                                
                                if(!access(name, F_OK )){        //if file exists
                                    //case: file exists
                                    
                                    if(!access( name, W_OK )){	//if file can be written
                                        
                                        //case: file can be written
                                        printf("The File %s was found\n",name);
                                        printf("Do you want it to be overwritten? (yes/no): ");
                                        
                                        option =get_string(&error);		//gets the yes or no option
                                        
                                        if(strcmp(option, "yes") == 0){
                                            //case:yes, the file is overwritten
                                            
                                            #ifdef DEBUG
                                            printf("DEBUG: File exists. Yes introduced. Programm introduce to save collection method\n");
                                            #endif
                                            
                                            printf("Overwritting...Done!\n");
                                            save_coleccion(&my_collection, name, &error);//from coleccion.c (save the current collection in a file)
                                            
                                            
                                        } else if(strcmp(option, "no")==0){
                                            //case: no, the file is not overwritten
                                            #ifdef DEBUG
                                            printf("DEBUG: File exists. No introduced. Programm exits\n");
                                            #endif
                                            printf("Exit without saving\n");
                                            
                                        } else {
                                            //case: input error
                                            #ifdef DEBUG
                                            printf("DEBUG: Invalid input\n");
                                            #endif
                                            error=ERR_NO_NUMBERS_INPUT;
                                            printf("%s \n", errormanage(error));
                                            //ERROR
                                        }
                                    }
                                    
                                } else {
                                    //case: file does not exists, it creates it
                                    printf("The file %s does not already exist . Creating the file...Done!\n", name);
                                    #ifdef DEBUG
                                    printf("DEBUG: File doesnt exists. Introducing to save collection\n");
                                    #endif
                                    save_coleccion(&my_collection, name, &error);
                                }
                                
                                //both frees because colateral effects of get_string
                                free(name);
                                free(option);
                                
                            } else if(strcmp(save, "no") == 0){
                                
                                printf("Exit without saving\n");
                                
                            } else {
                                
                                error = ERR_NO_NUMBERS_INPUT;
                                printf("%s \n", errormanage(error));
                                //ERROR
                            }
                        } else {
                            
                            if(error == ERR_CTRLD_INPUT){
                                printf("%s \n", errormanage(error));
                                error=0;
                            }
                        }
                        
                        free(save);	//colateral effect of get_string
                        
                    }
                    
                }
                
                //frees needed
                free(total_option);
                free_coleccion(&my_collection);
                free_opt(opt);
                
                return 0;
                
            }
            
            printf("%s \n", errormanage(error));
            
        }
        
        free(total_option);
        resto = NULL;
        option = 0;
        
        printf("Enter an option (type help for help): ");
        total_option = get_string(&error);	//asking for whole command
        
        if(error == 0){    //total_option is correct
            
            option_str = strtok_r(total_option," ",&resto);  //extract the name of the option
            
            option= compare_option(option_str,&error);    //returns the number of the operation or -1 if is not an option
                
            
        }
        
    }
    
    
    if(option == 0 && error == 0){     //When exit is introduced
        
        
        printf("Deleting all data stored. \n");
        
        #ifdef DEBUG
        printf("DEBUG: Free collection before exit\n");
        #endif
        
        free(total_option);
        free_opt(opt);
        free_coleccion(&my_collection);
        
        return 0;
        
    }
}
