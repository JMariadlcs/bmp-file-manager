#ifndef BMP_H
#define BMP_H
/************       bmp.h        *************************************************
 * 
 * Fichero      bmp.h
 * 
 *  Resumen     All method related with the manage of a BMP file.
 * 
 *  Descripción This file has the methods releated with the node.
 *			    Each time it is needed to modify the collection data, it 
 *			    will be needed to use a method of this class.
 *			    Also,a possible definition of data types for both BMP headers
 *			    and for the whole image is presented.
 * 
 *  Autor       Francisco Atalaya, Jose María de La Cruz, Iosif Mondoc, Patricia Paredes Martín
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

/*---------------------------------------------------------------------------*/
/* Declaración de constantes                                                 */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Declaración de tipos                                                      */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Declaración de estructuras                                                */
/*---------------------------------------------------------------------------*/

/**BITMAPFILEHEADER***********************************************************

  Resumen       BMP file header.

  Descripción   It has the cuantitative variables of the structure

******************************************************************************/

typedef struct tagBITMAPFILEHEADER {
    char type[2];         // Should be "BM"
    int  bfSize;          // 4 bytes with the size
    char bfReserved[4];   // 4 bytes reserved
    int  bfOffBits;       // offset where the pixel array can be found
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

/**BITMAPINFOHEADER***********************************************************

  Resumen       BMP info header

  Descripción   It has the essential variables of the structure, 
                which contain the information of the image.

******************************************************************************/

typedef struct tagBITMAPINFOHEADER {
    int   biSize;         // size of the DIB header from this point
    int   biWidth;        // width in pixels
    int   biHeight;       //height in pixels
    short biPlanes;       // number of planes
    short biBitCount;     // number of bits per pixel
    int   biCompression;  // if compression BI_RGB = 0 ó BI_BITFIELDS = 3
    int   biSizeImage;    //size of the array data including padding
    int   biXPelsPerMeter;// hor resolution of the image
    int   biYPelsPerMeter;// ver resoilution of the image
    int   biClrUsed;      // number of colours in the palette, 0 if it uses all
    int   biClrImportant; // number of important colours, 0 if all are imp
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

/**PIXELS*********************************************************************

  Resumen       File pixels structure

  Descripción   Packed structure in order to read the pixels at 
                once (instead of reading/writing them one by one).

******************************************************************************/

typedef struct __attribute__((__packed__)){
    unsigned char b;
    unsigned char g;
    unsigned char r;
} PIXELS;

/**BMPFILE*******************************************************************

  Resumen       Image structure

  Descripción   Structure thah has as arguments all the before defined struct variables.

******************************************************************************/

typedef struct image{
    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;
    char *alignment;      // characters between header and pixel map
    unsigned int padding; // row padding within the pixel map. This is the difference between the header end and the start of the pixels
    PIXELS *im;           // Here you can use either an array or a multidimensional array
    // If you want to use a multidimensional array you should
    // define PIXELS **im; and perform 2 mallocs instead of one
} BMPFILE;


/*---------------------------------------------------------------------------*/
/* Protitpos de las funciones                                                */
/*---------------------------------------------------------------------------*/

/**checkifBMP********************************************************************

  Resumen      This function is used to check if a file is BMP.

  Descripción  The program must check the first 2 bytes so that they are the 
               characters 'B' and 'M'.The program must also check the following 
               4 bytes (file size) and whether it really corresponds to the size 
               returned by lstat(). If it isn't so, it isn't a valid BMP.

  Parámetros  -char * filename: In order to read a file by its name.
              -int* error: used to manage it in case we have to do it. 

  Efec. Colat. We have to return an int type variable.


******************************************************************************/

int checkifBMP(char *filename, int *error);


/**load_BMP********************************************************************

  Resumen      This function is used to load a BMP file (load its properties).

  Descripción  The program open the file in binary read, and reads all its
               headers and structures, in order to have them in a node of the list.

  Parámetros  -char *filename: In order to access a file by its name.
              -int* error: used to manage it in case we have to do it. 

  Efec. Colat. We have to return a BMPFILE type variable.


******************************************************************************/

BMPFILE *load_BMP(char *filename, int *error);


/**save_BMP********************************************************************

  Resumen      This function is used to save all the changes in the BMP file.

  Descripción  The program opens the file in writing mode, and overwrites all its
               headers and structures, in order to have them in a node of the list.

  Parámetros  -char * filename: In order to access a file by its name.
              -int* error: used to manage it in case we have to do it. 

  Efec. Colat. We have to return an int type variable.


******************************************************************************/

int save_BMP(BMPFILE *bmp_pic, char *filename, int *error);


/**freeBMP********************************************************************

  Resumen      This function is used to do "free" of the alignment and the 
               two headers of the BMPFILE.

  Descripción  It does three frees, one for the alignment, another one for the ih
               and another one for the fh.

  Parámetros  -BMPFILE *bmp_pic: Used to have access to the alignment, ih and fh.

  Efec. Colat. Void type function.


******************************************************************************/

void freeBMP(BMPFILE *bmp_pic);


#endif
