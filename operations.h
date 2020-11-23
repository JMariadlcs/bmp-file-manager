#ifndef OPERATIONS_H
#define OPERATIONS_H
/************         Coleccion.h     *********************************

  Fichero     coleccion.h

  Resumen     All methods releated with the collection

  Descripción This class has the methods releated with the collection.
			  Each time it is needed to modify the collection data, it 
			  will be needed to use a method of this class.


  Autor       Francisco Atalaya, Jose María de La Cruz, Iosif Mondoc, Patricia Paredes Martín

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
/*------------OTHER CLASSES INCLUDES-----------*/
#include "bmp.h"

/*---------------------------------------------------------------------------*/
/* Protitpos de las funciones                                                */
/*---------------------------------------------------------------------------*/
struct parameters{
	
	char* channel;
	int arrayBitonal[7]; 	//[Rd, Gd, Bd, Rl, Gl, Bl, Threshold]
	char* onlyonechannel;
    int total_pixels;
    int level;
    char *histogramaname;
    char *sense;
    char *direction;
    long angle;
    
};




//FUNCTIONS OF HISTOGRAM



/**********    makeTempCSV    ***********************

  Resumen      This function is used to create a temporal file

  Descripción  We use this function to create a temporal file so later we can 'save' the values of the intensity of each r,g,b and then print them in a .png

  Parámetros    int *error. Used to manage our list and the errors, in case we have some.

  Efec. Colat. we have to return a char type
  

**************************************************/
char *makeTempCSV(int *error);

/**********    makeCSVFILE    ***********************

  Resumen      This function is used to create a csv file

  Descripción  We use this function to create a CSV file where the data we got in the makeTempCSV is stored and we can 'save' the values of the intensity of each r,g,b

  Parámetros    BMPFILE* input, int *error. Used to have an image and save its r,g,b intensity channels, and *error  used to manage our list and the errors, in case we have some.

  Efec. Colat. we have to return a char type
  

**************************************************/
char *makeCSVFILE(BMPFILE* input, int*error);
/**********    printCSVFILE   ***********************

  Resumen      This function is used to create a '.png' file

  Descripción  We use this function to create a '.png' file where the graphs with the insity of each color channel is printed

  Parámetros    BMPFILE* input, int *error. Used to have an image and save its r,g,b intensity channels, and *error  used to manage our list and the errors, in case we have some.

  Efec. Colat. we have to return a char type
  

**************************************************/
void printCSVFILE(char* filename, char* filehist, int *error);
/**********    histogram    ***********************

  Resumen      This function is used to call makeCSVFILE and printCSVFILE

  Descripción  We use this function to call the previous functions to complete our histogram

  Parámetros   char* filename, char* filehist, int *error. To know which image we are applying it, its path, and the error in case we need to manage it

  Efec. Colat. No collateral effects

**************************************************/

void histogram(BMPFILE *bmp, char* path, int *error);



    


//FUNCTIONS OF GRAY SCALE


/**********    graychannels    ***********************

  Resumen      This function is used to change the value of r,g,b of bmp file 

  Descripción  Depending on what the user enters. If it enters a 'b', the channels red and green will change its value to the one of the blue one. If the user enters
				'y' it will means use the luminance, calculated as Y=0.212656r + 0.715158g+0.072186b

  Parámetros   BMPFILE file, char option. To change a bmpw file, and option to get the channel that the user enters.

  Efec. Colat. No collateral effects

**************************************************/
PIXELS graychannels(PIXELS pixel, struct parameters* parametros, int *error);





//FUNCTIONS OF BITONAL



/**********    bitonal   ***********************

  Resumen      This function applies the bitonal operation

  DescripciÃ³n  We use this function to apply the bitonal operation to a bmp file
  
  ParÃ¡metros    BMPFILE ->a pointer to the struct that contains the bmp file, int *error-> to control errors
				returns an int: 0->no error -1->error
  Efec. Colat. nothing
  

**************************************************/
PIXELS bitonal(PIXELS pixel, struct parameters* parametros, int *error);
       
    
    
    
//FUNCTIONS OF ROTATE


/*********************** rotateRight ********************************************************************

  Resumen      This function does the rotation of a bmp file.

  Descripción  It makes de rotation of a bmp file in right sense and with an angle of 90º

  Parámetros   It has two parameters:
                    -BMPFILE *bmp_pic : a pointer to the struct that contains the bmp file
                    -int *error:address of an int variable wich returns an error.

  Efec. Colat. No colateral effects

******************************************************************************/

void rotateRight (BMPFILE *bmp_pic, int *error);

/*********************** rotateLeft ********************************************************************

  Resumen      This function does the rotation of a bmp file.

  Descripción  It makes de rotation of a bmp file in left sense and with an angle of 90º

  Parámetros   It has two parameters:
                    -BMPFILE *bmp_pic : a pointer to the struct that contains the bmp file
                    -int *error:address of an int variable wich returns an error.

  Efec. Colat. No colateral effects

******************************************************************************/
void rotateLeft (BMPFILE *bmp_pic, int *error);

/*********************** rotateRight ********************************************************************

  Resumen      This function does the rotation of a bmp file.

  Descripción  It makes de rotation of a bmp file in the sense that the user introduce (right or left) and in the angle that the user also introduce(90,180,270)

  Parámetros   It has four parameters:
                    -BMPFILE *bmp_pic : a pointer to the struct that contains the bmp file
                    -char *sense : its the sense introuced by the user to rotate
                    -long angle: its the angle introduced by the user to rotate
                    -int *error:address of an int variable wich returns an error.

  Efec. Colat. No colateral effects

******************************************************************************/
BMPFILE* rotate(BMPFILE *bmp_pic, char *sense, long angle, int *error);



//FUNCTION TO MOVE THROUGH THE ARRAY OF PIXELS

BMPFILE* recorrer_array (BMPFILE* bmp_pic, struct parameters *parametros, int* error, PIXELS (*elegir_function)(PIXELS , struct parameters * , int* ));

//FUNCTIONS OF ONLY ONE

PIXELS onlyone(PIXELS pixel, struct parameters* parametros, int *error);

//FUNCTIONS OF BLACK AND WHITE

PIXELS otsu_b_w(PIXELS pixel, struct parameters* parametros, int *error);

struct parameters* PixelsB_W(BMPFILE *bmp_pic,struct parameters *parametros, int* error);

//FUNCTIONS OF MIRROR

BMPFILE *mirror(BMPFILE *bmp_pic, char *direction, int *error);

#endif
