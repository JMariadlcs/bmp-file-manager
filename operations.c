
/*********         operation.c          *****************************

  Fichero     operation.c

  Resumen     contains the method that manage every error

  Descripción The method inside has an error array and prints
			  the error needed

  Autor       Francisco Atalaya , Jose María de La Cruz, Iosif Mondoc, Patricia Paredes Martín

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
  * 
  *******************************************************************************/
  
  /*------------INCLUDES-----------*/
#include <string.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/*------------OTHER CLASSES INCLUDES-----------*/
#include "util.h"
#include "error.h"
#include "list.h"
#include "bmp.h"
#include "operations.h"

/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/
//POINTER TO FUNCTIONS
BMPFILE* recorrer_array (BMPFILE* bmp_pic, struct parameters* parametros, int* error, PIXELS (*elegir_function)(PIXELS , struct parameters*  , int* )){
	int i;
		for (i = 0; i<(bmp_pic->ih.biWidth * bmp_pic->ih.biHeight); i++){ //Bucle for used to go modifying every pixel in the bmp 
			bmp_pic->im[i]=elegir_function(bmp_pic->im[i], parametros, error) ; 
			if(*error != 0){
		return NULL; 	//Check if an error has ocurred
			}
		}
		return bmp_pic; //Returned the modified bmp
	}
	

//HISTOGRAM
char *makeTempCSV(int *error){ //We need to create a temporal file
    
    char template[PATH_MAX]= "./XXXXXX";
	int check = mkstemp(template); //variable 'check' used to check if error has ocurred
   
    if(check  == -1){ //No error has ocurred
        
        *error = errno;
        return NULL;
        
    }
    
    char filename[PATH_MAX];   //We start computing the whole PATH of the filenmae
    strcpy(filename, template);
    strcat(filename, ".csv");
    rename(template, filename);
    
    //filename later used
    return strdup(filename);  
}   

char *makeCSVFILE(BMPFILE *bmp_pic, int *error){
	
	//Here we create the arrays for the values of each color to be filled later
    
	int iRed[256]= {0}; 
	int iGreen[256]= {0};
	int iBlue[256] = {0};
		
    //We calculate the number of pixels that have intensity in red, green and blue
    
    int auxR, auxG, auxB;
    int i;
    
    for(i=0;i<((bmp_pic->ih.biHeight)*(bmp_pic->ih.biWidth));i++){ //Loop for going through the array
        
        auxR=(int)bmp_pic->im[i].r;
        auxG=(int)bmp_pic->im[i].g;
        auxB=(int)bmp_pic->im[i].b;

		iRed[auxR]++;
        iGreen[auxG]++;
        iBlue[auxB]++;
    }
    
    //We need to create a temportal filename
    char* filename = makeTempCSV(error); 
  
	FILE* file_ptr;
	file_ptr = fopen(filename,"w+"); //We open the file
    
    if (file_ptr == NULL){ //We check if an error has ocurred
        *error = errno;
        return NULL;
        
    }
    
	fprintf(file_ptr, "Intensity\tRed\tGreen\tBlue\n"); //For printing the correct format it in the csv
	
	for(i =0; i <256;i++){ //Loop to fill the csv file 
		fprintf(file_ptr,"%d\t ",i);
        fprintf(file_ptr,"%d\t ",iRed[i]);
        fprintf(file_ptr,"%d\t ",iGreen[i]);
        fprintf(file_ptr,"%d\n",iBlue[i]);
	}
	
	fclose(file_ptr); //We close the file
	return filename;  //We return the file (Later used)
}
	
void printCSVFILE(char* filename, char* outputnamepedido, int *error){ 
	
	char *fhist = strdup(outputnamepedido);  //The name of the final histogram file
    
    if(fhist == NULL){ //Check if error
        *error = errno;
        return;
    }
	
	char *auxInput = (char *)calloc((strlen(filename)+13), sizeof(char)); //We reserve space
    
	if(auxInput == NULL){ //Check if error
        
		*error = MALLOC_FAIL;
		return;
	}
	
	strcpy(auxInput, "inputfile='"); //We compute the whole path of our final final of the input file
    strcat(auxInput, filename);      
    strcat(auxInput,"'");
    
    char* auxOutput= (char*)malloc((strlen(fhist)+19)* sizeof(char)); //We reserve space
    
    if(auxOutput == NULL){ //We check if error
        
		*error = MALLOC_FAIL;
		return;
        
	}
    strcpy(auxOutput, "outputfile='"); //We compute the whole path of our final final of the output file
    strcat(auxOutput, fhist);
    strcat(auxOutput, ".png'");
    
	pid_t pid = fork(); //We create a child process 
    
	if(pid == 0){ //Only execute by the child
		
		 char* arguments[] = {"gnuplot", "-e", auxInput, "-e", auxOutput, "pinto_hist.gp", NULL};  //the function execvp requires an array with the parameters used
        
        execvp(arguments[0], arguments);
		
		
	}else if(pid == -1){ //If an error has ocurred during fork
		
		*error=PID_ERROR;
        free(fhist);
        free(auxInput);
        free(auxOutput);
        free(filename);
		return;
		
		
	}else{ //We wait until child has dead
		waitpid(pid, NULL, 0);	
	}
		
	int check_remove = remove(filename); //We remove the temporal file previously created
    
    if(check_remove == -1){ //Check if error
        *error = errno;
    }
    
    //We free the previous reserved space
	free(fhist);
    free(auxInput);
    free(auxOutput);
	free(filename);
	
}
    
void histogram(BMPFILE *bmp, char* outputnamepedido, int* error){
	
	//We call to the previously method to execute our histogram function
	char* filename = makeCSVFILE(bmp, error);
    
    printCSVFILE(filename, outputnamepedido, error);
	//free of filename is done inside printCSVFILE
}



//FUNCTIONS OF GRAY SCALE 
PIXELS graychannels(PIXELS pixel, struct parameters* parametros, int *error){
	
	 //For used to travers the full image (every pixel)
        
			
			  int y; //Used to calculate lumitance
			if (strcmp(parametros->channel,"b")==0){ //We put every channel to the value of the blue one
				
					pixel.r = pixel.b;
					pixel.g = pixel.b;
				
			} else if (strcmp(parametros->channel, "g")==0) { //We put every channel to the value of the green one
                
					pixel.r = pixel.g;
					pixel.b =pixel.g;
					
			} else if (strcmp(parametros->channel, "r")==0) { //We put every channel to the value of the red one
            
				pixel.g = pixel.r;
				pixel.b = pixel.r;
				
			
			} else if (strcmp(parametros->channel, "y")==0){ //LUMINANCE: Y=0212656r + 0.715158g + 0.072186b
                
              
                y = 0.212656*pixel.r + 0.715158*pixel.g + 0.072186*pixel.b;
                pixel.r = y;
                pixel.g = y;
                pixel.b = y;
            
		}
		return pixel; //We return the pixel (managed in pointer to functions)
}



//FUNCTIONS OF BITONAL

PIXELS bitonal(PIXELS pixel, struct parameters* parametros, int *error){  //0->Success -1->Error

    //applying bitonal

    //for travels the array comparing the media of each pixel with the threshold
    //and changing by the corresponding pixel (dark/light)

    int media; //To compute the media of the three input parameters

    media = pixel.r + pixel.g + pixel.b;
    media=media/3;
    if(media < parametros->arrayBitonal[6]){ //pixel turns to light pixel
        pixel.r = parametros->arrayBitonal[3];
        pixel.g = parametros->arrayBitonal[4];
        pixel.b = parametros->arrayBitonal[5];
    }
    if(media >= parametros->arrayBitonal[6]){ //pixel turns to dark pixel
        pixel.r = parametros->arrayBitonal[0];
        pixel.g = parametros->arrayBitonal[1];
        pixel.b = parametros->arrayBitonal[2];
    }

    return pixel; //We return the pixel (managed in pointer to functions)
}




//FUNCTIONS OF ROTATE 
void rotateRight (BMPFILE *bmp_pic, int *error){
    
    //Define the new width and heigh of the image
    int oldHeigh = bmp_pic->ih.biHeight;            
    int oldWidth = bmp_pic->ih.biWidth;
    
    bmp_pic->ih.biHeight = oldWidth;
    bmp_pic->ih.biWidth = oldHeigh;
    
    int rotHeigh = bmp_pic->ih.biHeight;
    int rotWidth = bmp_pic->ih.biWidth;
    
    //Change the coordinates X and Y
    int oldXPels = bmp_pic->ih.biXPelsPerMeter;
    int oldYPels = bmp_pic->ih.biYPelsPerMeter;
    
    bmp_pic->ih.biXPelsPerMeter = oldYPels;
    bmp_pic->ih.biYPelsPerMeter = oldXPels;
    
    //Calculation of the new padding of the bmp file
    if ((bmp_pic->ih.biWidth * 3) % 4 != 0) {
      
      bmp_pic->padding = 4 - (bmp_pic->ih.biWidth * 3) % 4;
      
   } else {
       
       bmp_pic->padding = 0;
       
   }
   
   //Creation of an auxilary PIXEL in order to do the rotations and then copy the result in the orginial PIXEL which is im
   
   PIXELS *aux = (PIXELS*)malloc(sizeof(PIXELS)*rotHeigh*rotWidth);
   
   //Checking the malloc
   
   if(aux == NULL){ //If error
       
      *error = MALLOC_FAIL;
      
  } else {
      
      
      //By using some formulas, we interchange the pixels with aux[] and bmp_pic->im[]
      
      int i,j, index1, index2, w, h;
      
      for(i=0; i<oldHeigh; i++){ 
          
          for(j = 0; j<oldWidth; j++){ //For loops to go throw every pixel
              
              //Formulas used to compute rotate
              w = i;
              h = rotHeigh-1-j;
              index1 = rotWidth*h + w;
              index2 = oldWidth*i + j;
              aux[index1] = bmp_pic->im[index2];
      
          }
      }
      
  }
  
  //Finally, we do the corresponding frees
  
  free(bmp_pic->im);
  bmp_pic->im = NULL;
  bmp_pic->im = aux;

}
      
  
  void rotateLeft (BMPFILE *bmp_pic, int *error){
    
    //Define the new width and heigh of the image
    int oldHeigh = bmp_pic->ih.biHeight;
    int oldWidth = bmp_pic->ih.biWidth;
    
    bmp_pic->ih.biHeight = oldWidth;
    bmp_pic->ih.biWidth = oldHeigh;
    
    int rotHeigh = bmp_pic->ih.biHeight;
    int rotWidth = bmp_pic->ih.biWidth;
    
    //Change the coordinates X and Y
    int oldXPels = bmp_pic->ih.biXPelsPerMeter;
    int oldYPels = bmp_pic->ih.biYPelsPerMeter;
    
    bmp_pic->ih.biXPelsPerMeter = oldYPels;
    bmp_pic->ih.biYPelsPerMeter = oldXPels;
    
    //Calculation of the new padding of the bmp file
    
    if ((bmp_pic->ih.biWidth * 3) % 4 != 0) {
      
      bmp_pic->padding = 4 - (bmp_pic->ih.biWidth * 3) % 4;
      
   } else {
       
       bmp_pic->padding = 0;
       
   }
   
   //Creation of an auxilary PIXEL in order to do the rotations and then copy the result in the orginial PIXEL which is im
   PIXELS *aux = (PIXELS*)malloc(sizeof(PIXELS)*rotHeigh*rotWidth);
   
   //Checking the malloc
   
   if(aux == NULL){
       
      *error = MALLOC_FAIL; //If error
      
  } else {
      
      //By using some formulas, we interchange the pixels with aux[] and bmp_pic->im[]
      
      int i,j, index1, index2, w, h;
      
      for(i=0; i<oldHeigh; i++){
          
          for(j = 0; j<oldWidth; j++){ //For loops to go throw every pixel
              
              //Formulas used to rotate
              w = rotWidth-1-i;
              h = j;
              index1 = rotWidth*h + w;
              index2 = oldWidth*i + j;
              aux[index1] = bmp_pic->im[index2];
      
          }
      }
      
  }
  
  //Finally, we do the corresponding frees
  
  free(bmp_pic->im);
  bmp_pic->im = NULL;
  bmp_pic->im = aux;

}


BMPFILE *rotate(BMPFILE *bmp_pic, char *sense, long angle, int *error){
    
    int total = angle/90; //To have a single loop degree
    int i;
    
        //We check what is the case introduced by the user to compute the correct rotate option
        if((strcmp(sense, "r")== 0 && angle == 90) || (strcmp(sense, "l")== 0 && angle == 270)){
            
            rotateRight(bmp_pic, error);
               
        } else if((strcmp(sense, "r")== 0 && angle == 270) || (strcmp(sense, "l")== 0 && angle == 90)){
            
            rotateLeft(bmp_pic, error);
             
        } else if((strcmp(sense, "r")== 0 && angle == 180) || (strcmp(sense, "l")== 0 && angle == 180)){
            
            for(i = 0; i < total; i++){
                rotateRight(bmp_pic, error);
            }
        }
        
    
    //Introducing the new sides
    bmp_pic->fh.bfSize = ((bmp_pic->ih.biWidth*3 + bmp_pic->padding)*bmp_pic->ih.biHeight) + bmp_pic->fh.bfOffBits;
    bmp_pic->ih.biSizeImage = (bmp_pic->ih.biWidth*3 + bmp_pic->padding)*bmp_pic->ih.biHeight;
    
    
    return bmp_pic; //Return modified the bmp_pic
    
}

//FUNCTIONS OF ONLYONE
PIXELS onlyone(PIXELS pixel, struct parameters* parametros, int *error){
	
	 //For used to travers the full image (every pixel)
        
			if (strcmp(parametros->onlyonechannel,"b")==0){ //We put every channel to the value of the blue one
				
					pixel.r = 0;
					pixel.g = 0;
				
			} else if (strcmp(parametros->onlyonechannel, "g")==0) { //We put every channel to the value of the green one
                
					pixel.r = 0;
					pixel.b =0;
					
			} else if (strcmp(parametros->onlyonechannel, "r")==0) { //We put every channel to the value of the red one
            
				pixel.g = 0;
				pixel.b = 0;
				
			} 
			
		return pixel;
}


//FUNCTIONS OF BLACK AND WHITE
struct parameters* PixelsB_W(BMPFILE *bmp_pic, struct parameters *parametros, int* error){

    int top = 256; //Max number of pixel used 
    

    int histogramCounts[top];
    
    int total = parametros->total_pixels;
    
    int sumB = 0;
    int sum1 = 0;
    int maximum = 0;
    int wB = 0;
    int wF, mF, val;
    int level = 0;
    int i = 0;
    
    
    for (i = 0; i<top; i++){
        histogramCounts[i] = 0; //We initialize the array to 0
    }
    
    for (i = 0; i<total; i++) {
    histogramCounts[bmp_pic->im[i].g]++;
    }

    
    for (i = 0; i<top; i++) {
        sum1 += i*histogramCounts[i]; //scalar product, stored in sum1 variable
    }
    
    for (i = 0; i<top; i++) {
        
        wF = total - wB;
        
        if ((wB > 0) && (wF > 0)){
            mF = (sum1 - sumB) / wF;
            val = wB * wF * ((sumB / wB) - mF) * ((sumB / wB) - mF);
            
            
            if (val >= maximum){
                level = i;   //THE THRESHOLD!!!!!
                maximum = val;
            }
        }
        wB = wB + histogramCounts[i];
        sumB = sumB + i * histogramCounts[i];
        
    }
    
    parametros->level = level;
    return parametros; //We return the modified parametros
    
}

    
    
    
PIXELS otsu_b_w(PIXELS pixel, struct parameters* parametros, int *error){ 
        
       
    if (pixel.r > parametros->level){ //We modified every pixel, using the return parametros->level 
                                      //from the previous threshold we have obtained  with the otsu
            
        pixel.r = 255;
        pixel.g = 255;
        pixel.b = 255;
            
    } else {
            
        pixel.r = 0;
        pixel.g = 0;
        pixel.b = 0;
    }
    return pixel; //no error
}




//FUNCTIONS OF MIRROR

BMPFILE *mirror(BMPFILE *bmp_pic, char *direction, int *error){
   
    //We declare some auxiliar variables in order to do the operations easier
    
    int auxHeight = bmp_pic->ih.biHeight;
    int auxWidth = bmp_pic->ih.biWidth;
    
    int totalDimension = auxHeight*auxWidth;
    
    PIXELS aux;
    
    int i;
    int j = 0;
    
    if(strcmp(direction, "h") == 0){ 
        
        for (i = 0; i < (totalDimension/2); i++){
            
            aux = bmp_pic->im[i];
            bmp_pic->im[i] = bmp_pic->im[totalDimension-i-1];
            bmp_pic->im[totalDimension-i-1] = aux;
            
        }
        
        for(i = 0; i < auxHeight; i++){
            
            while(((auxWidth/2)-j) > 0){
                
                aux = bmp_pic->im[(i*auxWidth)+j];
                bmp_pic->im[(i*auxWidth)+j] = bmp_pic->im[((i+1)*auxWidth)-1-j];
                bmp_pic->im[((i+1)*auxWidth)-1-j] = aux;
                j++;
            }
            
            j = 0;
            
        }
                
        
    }
    
    if(strcmp(direction, "v") == 0){
        
        for(i = 0; i < auxHeight; i++){
            
            while(((auxWidth/2)-j)>0){
                
                aux = bmp_pic->im[(i*auxWidth)+j];
                bmp_pic->im[(i*auxWidth)+j] = bmp_pic->im[((i+1)*auxWidth)-1-j];
                bmp_pic->im[((i+1)*auxWidth)-1-j] = aux;
                j++;
            }
            j = 0;
        }
        
    }
       
    return bmp_pic;
     
}





            
            
