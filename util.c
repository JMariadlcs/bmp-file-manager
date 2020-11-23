
#include "util.h"
#include "list.h"
#include "operations.h"
#include "error.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*---------------------------------------------------------------------------*/
/* Declaración de constantes                                                 */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Declaración de tipos                                                      */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Declaración de estructuras                                                */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Declaración de variables                                                  */
/*---------------------------------------------------------------------------*/
extern char *string;

/*---------------------------------------------------------------------------*/
/* Declaración de macros                                                     */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Prototipos de las funciones estáticas                                     */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/

char *get_string(int *error){
    
    /*
     * We declare the variable read_bytes of type ssize_t that can be a negative
     * number in order to indicate the error of the size, and the variable 
     * number_bytes of type size_t that only can indicate teh size.
     * Also, ssize_t y size_t are synonims of unsigned int
     */
    
    ssize_t read_bytes;                 
    size_t number_bytes;                
   

    /*
     * 1ª WAY : We can enter to the get_line the initialized arguments 
     *number_bytes = MAX_SIZE;
     *string = (char *) malloc (number_bytes + 1);
     *read_bytes = getline(&string, &number_bytes, stdin);
     */
    
    
    /*
     * 2º WAY AND THE ONE WE HAVE IMPLEMENTED: We put the number_bytes to 0
     * and the string to NULL because get_line will do the reservation of memory
     */
  
    number_bytes = 0;
    string = NULL;
    
   /*
    * It stores the word introduced by the user, into the pointer string
    */
    read_bytes = getline(&string, &number_bytes, stdin);    
    
    /*
     * Checking getline error (When getline has an error it returns -1), 
     * this means, the user has pressed CNTRL+D.
     */
    
    if (read_bytes == -1){                      
        
        *error = ERR_CTRLD_INPUT;
        clearerr(stdin);          
        
    } else {   //Getline has no error
        
        string[strlen(string)-1]= '\0';
        
        if (strcmp(string, "" ) == 0){           //Checks if the string introduced is null
            
            *error = ERR_EMPTY_LINE;
            
        } else {
        
            *error = 0; 
        
            /*
            * In order to remove the final blank of the string obtaines with getline 
            */
            
        }
    }
    
    return string;
    
}


long int get_number(int *error,int base){
    
    char *string;
    char *ptr;
    long number;
    string = get_string(error);             
    
    if (*error !=0){                         //Checks the if getstring has had error
        
        if (string){                         //If the string is not null, we do a free, 
                                             //if it's NULL no free is needed.
            free(string);
        }
        
        return 0;
        
    } else {
        
        errno = 0;
        
        /*
         * In order to convert the string introduced by the user into a long variable we use
         * the function strtol, that also has some internal errors.
         */
        
        number = strtol(string,&ptr,base);                

        if(errno == ERANGE && number == LONG_MAX){      //Checking the strtol errors
            
            *error = ERR_TOO_MUCH_NUMBERS;
            
        } else if ((errno == ERANGE && number == LONG_MIN) || number<0){
            
            *error = ERR_NEG_NUMBER;
            
        } else if (errno != 0 && number == 0){
            
            *error = ERR_ONLY_CHARS;
            
        } else {
            
            /*
             * Checks if there are more characters after the numbers. 
             */
            
            if (*ptr != '\0') {                                                  
                
                *error = ERR_FURTHER_CHARS_INPUT;  
                
            /*
             * Checks if the string introduced is equals the remaining string after using strtol, 
             * if it is the case it will handle an error called ERR_NO_NUMBERS_INPUT because
             * the user has introduce only chars.
             */
                
            } else if (string == ptr){               
                
                *error = ERR_NO_NUMBERS_INPUT;          
                
            }
            
            free(string);           //Free the string if there's no error of the strtol
            return number;                              
            
        }
        
        free(string);              //Freeing the string if there's an error of the strtol
        return 0;
        
    }
    
    free(string);                 //Free the string of the getstring.
    return 0;
    
}

long int get_op(int *error){
    
    char *string;
    char *ptr;
    long number;
    string = get_string(error);             
    
    if (*error !=0){                         //Checks the if getstring has had error
        
        if (string){                         //If the string is not null, we do a free, 
                                             //if it's NULL no free is needed.
            free(string);
        }
        
        return 0;
        
    } else {
        
        errno = 0;
        
        /*
         * In order to convert the string introduced by the user into a long variable we use
         * the function strtol, that also has some internal errors.
         */
        if(strcmp(string,"?") == 0){
            free(string);
            return 0;
        }else{
            
            number = strtol(string,&ptr,10);                
            
            if(errno == ERANGE && number == LONG_MAX){      //Checking the strtol errors
                
                *error = ERR_TOO_MUCH_NUMBERS;
                
            } else if ((errno == ERANGE && number == LONG_MIN) || number<0){
                
                *error = ERR_NEG_NUMBER;
                
            } else if (errno != 0 && number == 0){
                
                *error = ERR_ONLY_CHARS;
                
            } else {
                
                /*
                 * Checks if there are more characters after the numbers. 
                 */
                
                if (*ptr != '\0') {                                                  
                    
                    *error = ERR_FURTHER_CHARS_INPUT;  
                    
                    /*
                     * Checks if the string introduced is equals the remaining string after using strtol, 
                     * if it is the case it will handle an error called ERR_NO_NUMBERS_INPUT because
                     * the user has introduce only chars.
                     */
                    
                } else if (string == ptr){               
                    
                    *error = ERR_NO_NUMBERS_INPUT;          
                    
                }
                
                free(string);           //Free the string if there's no error of the strtol
                return number;                              
                
            }
        }
        
        free(string);              //Freeing the string if there's an error of the strtol
        return 0;
        
    } 
    
    free(string);                 //Free the string of the getstring.
    return 0;
    
}


int transformarstrtol(char* cadena, int base, int *error){
	int number;
    char *ptr;

    errno = 0;
    
        number = strtol(cadena,&ptr,base);                

        if(errno == ERANGE && number == LONG_MAX){      //Checking the strtol errors
            
            *error = ERR_TOO_MUCH_NUMBERS;
            
        } else if ((errno == ERANGE && number == LONG_MIN) || number<0){
            
            *error = ERR_NEG_NUMBER;
            
        } else if (errno != 0 && number == 0){
            
            *error = ERR_ONLY_CHARS;
            
        } else {
            
            /*
             * Checks if there are more characters after the numbers. 
             */
            
            if (*ptr != '\0') {                                                  
                
                *error = ERR_FURTHER_CHARS_INPUT; 
                return -1;
                
            /*
             * Checks if the string introduced is equals the remaining string after using strtol, 
             * if it is the case it will handle an error called ERR_NO_NUMBERS_INPUT because
             * the user has introduce only chars.
             */
                
            } else if (cadena == ptr){               
                
                *error = ERR_NO_NUMBERS_INPUT;
                return -1;
                
            }
            
                       //Free the string if there's no error of the strtol
            return number;                              
            
        }
        
                    //Freeing the string if there's an error of the strtol
        return -1;
        
    }
    
    

int compare_operation( char *token, int *error){
    
    if(token==NULL){
        *error = ERR_NO_NUMBERS_INPUT;
        return -1;
    } else {
    
    if(strcmp(token, "help") == 0){

        return 0;

    }

    else if(strcmp(token, "histogram") == 0){

        return 1;

    }

    else if(strcmp(token, "grayscale") == 0){

        return 2;

    }

    else if(strcmp(token, "bitonal") == 0){

        return 3;

    }

    else if(strcmp(token, "rotate") == 0){

        return 4;

    }

    else if(strcmp(token, "onlyone") == 0){

        return 5;

    }

    else if(strcmp(token, "blackwhite") == 0){

        return 6;

    }

    else  if(strcmp(token, "mirror") == 0){ 

        return 7;

        

    } else {

        

        *error = ERR_NO_NUMBERS_INPUT;

        return -1;

        

    }
    }

}





int compare_option( char *token, int *error){
    
       if(token==NULL){
        *error = ERR_NO_NUMBERS_INPUT;
        return -1;
    } else {

    if(strcmp(token, "load") == 0){

        return 1;

    }

    else if(strcmp(token, "delete") == 0){

        return 2;

    }

    else if(strcmp(token, "info") == 0){

        return 3;

    }

    else if(strcmp(token, "infoall") == 0){

        return 4;

    }

    else if(strcmp(token, "savecol") == 0){

        return 5;

    }

    else if(strcmp(token, "loadcol") == 0){

        return 6;

    }

    else if(strcmp(token, "help_ops") == 0){ 

        return 7;  

    }  

    else if(strcmp(token, "addop") == 0){

        return 8;

    } 

    else if(strcmp(token, "logon") == 0){

        return 9;

    }

    else if(strcmp(token, "logoff") == 0){

        return 10;

    }

    else if(strcmp(token, "help") == 0){

        return 11;

    }

    else if(strcmp(token, "exit") == 0){

        return 0;

    } else {

        *error = ERR_NO_NUMBERS_INPUT;

        return -1;
    }
    }

}


int contar_palabras(char *cadena){
    
    int p = 0;
    int i;
    
    if(cadena != NULL){
    for(i = 0; i<strlen(cadena); i++){
        
        if(cadena[i] == ' ' && cadena[i+1]!=' '){
            p++;
        }
    
    }
    
    p++;
    
    if(strcmp(cadena, " ") == 0){
        
        p = 0;
        
    }
    
    return p;
    
    } else {
        p = 0;
        return p;
    }
}





