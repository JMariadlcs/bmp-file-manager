/***************       bmp.c      ***************************************************
 * 
 *   Fichero     bmp.c
 * 
 *  Resumen     All method related with the manage of a BMP file.
 * 
 *  Descripción This class has the methods releated with the node.
 *			  Each time it is needed to modify the collection data, it 
 *			  will be needed to use a method of this class.
 *			  
 *   Autor       Francisco Atalaya and Jose María de La Cruz, Iosif Mondoc, Patricia Paredes Martín
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

/*------------INCLUDES-------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/wait.h>

/*------------OTHER CLASSES INCLUDES-----------------------------------------*/

#include "bmp.h"
#include "error.h"
#include "list.h"

/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/

int checkifBMP( char* filename, int *error){
    
    FILE *file;                     //We declare a file type 
    int check = 0;                  //Used to check fread errors.
    //We open the file in binary-read mode checkup its possible fopen error.
    
    if((file=fopen(filename, "rb")) == NULL){ 
        *error=errno;
        return -1;
    }
                                    //To check if its BMP:
    char BMP[2];                    //Check first two chars of the file
    check = fread(BMP, 1, 2, file); //We read its first 2 characters
    if (check != 2){
        *error = FREAD_ERROR;
        fclose(file);
        return -1;
    }
    
    if(BMP[0]!='B' || BMP[1]!='M'){ //Condition must fulfill to be a BMP
        *error = NOT_BMP_ERROR; 
        return -1;
    }
    
    //We have already the first two header chars are 'B' and 'M'
    //We now have to check the file size
    struct stat estado;
    stat(filename,&estado);
    int leersize;
    
    
    check = fread(&leersize, 4, 1, file);   //We check if its size corresponds to a BMP file
    if (check != 1){                        //fread error check.
        *error = FREAD_ERROR;
        fclose(file);
        return -1;        
    }
    
    if(leersize!=estado.st_size){           //If size doesn't fulfill the BMP condition,
        *error = NOT_BMP_ERROR;             //Not a BMP file
        fclose(file);
        return -1;
    }
    
    fclose(file);                           //We close the file before leaving
    return 0;
}




BMPFILE *load_BMP(char *filename, int *error) {
    
    FILE *file_ptr;         //FILE variable declaration
    
    size_t check = 0;       //Variable used to check fread errors.
    
    if(access(filename, R_OK) != 0){    //Before everything, we check if we have reading permission.
        *error = PERMISSIONS_ERROR;
        return NULL;
    }
    
    file_ptr = fopen(filename, "rb");   //open reading in binary the file
    
    if (!file_ptr) {                    //fopen error check     
        *error = errno;                 //We check if there is really an image or not
        return NULL;        
        
    }
    
    //We allocate memory for the entire BMP file (which has the 2 headers).
    BMPFILE *bmp_pic = (struct image*)malloc(sizeof(BMPFILE));
    
    if(bmp_pic == NULL){                //Malloc error checkup.
        *error = MALLOC_FAIL;
        fclose(file_ptr);
        return NULL;
    }
    
 
    /*          BITMAP FILE HEADER                                               
     * For each fread of the attributes of the header, 
     * we also check their possible errors.
     */
    check = fread(bmp_pic->fh.type, sizeof(char), 2, file_ptr);
    
    if (check != 2){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    check = fread(&(bmp_pic->fh.bfSize), sizeof(int), 1, file_ptr);
    
    if (check != 1){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    check = fread(bmp_pic->fh.bfReserved, sizeof(char), 4, file_ptr);
    
    if (check != 4){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    check = fread(&(bmp_pic->fh.bfOffBits), sizeof(int), 1, file_ptr);
    
    if (check != 1){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    /*          BITMAP INFO HEADER                                               
     * For each fread of the attributes of the header, 
     * we also check their possible errors.
     */
    check = fread(&(bmp_pic->ih.biSize), sizeof(int), 1, file_ptr);
    
    if (check != 1){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    check = fread(&(bmp_pic->ih.biWidth), sizeof(int), 1, file_ptr);
    
    if (check != 1){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    check = fread(&(bmp_pic->ih.biHeight), sizeof(int), 1, file_ptr);
    
    if (check != 1){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    check = fread(&(bmp_pic->ih.biPlanes), sizeof(short), 1, file_ptr);
    
    if (check != 1){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    check = fread(&(bmp_pic->ih.biBitCount), sizeof(short), 1, file_ptr);
    
    if (check != 1){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    check = fread(&(bmp_pic->ih.biCompression), sizeof(int), 1, file_ptr);
    
    if (check != 1){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    check = fread(&(bmp_pic->ih.biSizeImage), sizeof(int), 1, file_ptr);
    
    if (check != 1){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    check = fread(&(bmp_pic->ih.biXPelsPerMeter), sizeof(int), 1, file_ptr);
    
    if (check != 1){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    check = fread(&(bmp_pic->ih.biYPelsPerMeter), sizeof(int), 1, file_ptr);
    
    if (check != 1){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    check = fread(&(bmp_pic->ih.biClrUsed), sizeof(int), 1, file_ptr);
    
    if (check != 1){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    check = fread(&(bmp_pic->ih.biClrImportant), sizeof(int), 1, file_ptr);
    
    if (check != 1){
        
        *error = FREAD_ERROR;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
        
    }
    
    //Since we have to check if there is padding, we initialize it:
    
    bmp_pic->padding = 0;
    /*
     * Then we check if there is padding between the header end and 
     * the pixels.Also we store it in the header variable.
     */
    if ((bmp_pic->ih.biWidth * 3) % 4 != 0) {
        
        bmp_pic->padding = 4 - (bmp_pic->ih.biWidth * 3) % 4;
        
    }
    
    long header_end = ftell(file_ptr);      //ftell tell us the header header_end
    
    if(header_end == -1){   //ftell error checkup
        *error = errno;
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
    }
    
    long difference = bmp_pic->fh.bfOffBits - header_end;   //In order to check if there is alignment.
    
    if(difference != 0){                                    //It means, alignment does exist. 
        
        bmp_pic->alignment = (char*)malloc(difference*sizeof(char)); //Memory allocation for alignment.
        
        if(bmp_pic->alignment == NULL){                   //MALLOC error checkup
            *error = MALLOC_FAIL;
            free(bmp_pic);
            fclose(file_ptr);
            return NULL;
        }
        
        check = fread(bmp_pic->alignment, sizeof(char), difference, file_ptr);  //We read the alignment.
        
        if (check != difference){                       //Fread error checkup
            
            *error = FREAD_ERROR;
            free(bmp_pic -> alignment);
            free(bmp_pic);
            fclose(file_ptr);
            return NULL;
            
        }
        
    } else {                                            //If there's no alignment, we skip it
        
        bmp_pic->alignment = NULL;
        
    }
    
    //We allocate memory for the the array of pixels of the file (width*high).
    int total_pixels = bmp_pic->ih.biWidth * bmp_pic->ih.biHeight;
    
    bmp_pic->im = (PIXELS *)malloc(total_pixels * sizeof(PIXELS));
    
    if(bmp_pic->im == NULL){                            //MALLOC error checkup
        *error = MALLOC_FAIL;
        free(bmp_pic->alignment);
        free(bmp_pic);
        fclose(file_ptr);
        return NULL;
    }
    
    int i;
    
    for(i = 0; i < (bmp_pic->ih.biHeight); i++){        //We go through all the pixels 
                                                        //and read them all at once.
        //for(j = 0; j < bmp_pic->ih.biWidth; j++){
        
        check = fread( &(bmp_pic->im[bmp_pic->ih.biWidth * i]), sizeof(PIXELS), bmp_pic->ih.biWidth, file_ptr );
        
        if (check != bmp_pic->ih.biWidth){              //fread error checkup.
            
            *error = FREAD_ERROR;
            free(bmp_pic->im);
            free(bmp_pic -> alignment);
            free(bmp_pic);
            fclose(file_ptr);
            return NULL;
            
        }
        
        //}
        
        int check_fseek = fseek(file_ptr, bmp_pic->padding, SEEK_CUR);      //We skip the padding using fseek.
        
        if(check_fseek == -1){                          //fseek error checkup.

            *error = errno;
            free(bmp_pic->im);
            free(bmp_pic -> alignment);
            free(bmp_pic);
            fclose(file_ptr);
            return NULL;
            
        } 
        
    }
    
    
    //After we have done all the mallocs we want to close the file and return it.
    fclose(file_ptr);
    
    return bmp_pic;
    
}


int save_BMP(BMPFILE *bmp_pic, char *filename, int *error){ //-1->error, 0->no error
    #ifdef DEBUG
		printf("DEBUG: Function used to save the BMP file");
	#endif
    size_t fwriteerror = 0;     //variable used to check the fwrite possible errors.
    
    /*
     *Before everything, we check the real user's permissions for the file:
     *Tests whether the file exists, and if it can be accessed for writing.
     */
    if((access(filename, F_OK) == 0) && (access(filename, W_OK) != 0)){
        *error = PERMISSIONS_ERROR;
        freeBMP(bmp_pic);
        return -1;
    }
    
    FILE *file_ptr = fopen(filename, "w");      //We open the file in write mode.
    
    if(!file_ptr){                            //fopen error checkup.
        *error = errno;
        freeBMP(bmp_pic);
        return -1;
    }
    
    /*          BITMAP FILE HEADER                                               
     * For each fwrite of the attributes of the header, 
     * we also check their possible errors.
     */    
    fwriteerror = fwrite(bmp_pic->fh.type, sizeof(char), 2, file_ptr);
    
    if(fwriteerror != 2){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    fwriteerror = fwrite(&(bmp_pic->fh.bfSize), sizeof(int), 1, file_ptr);
    
    if(fwriteerror != 1){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    fwriteerror = fwrite(bmp_pic->fh.bfReserved, sizeof(char), 4, file_ptr);
    
    if(fwriteerror != 4){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    fwriteerror = fwrite(&(bmp_pic->fh.bfOffBits), sizeof(int), 1, file_ptr);
    
    if(fwriteerror != 1){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    /*          BITMAP INFO HEADER                                               
     * For each fwrite of the attributes of the header, 
     * we also check their possible errors.
     */
    fwriteerror = fwrite(&(bmp_pic->ih.biSize), sizeof(int), 1, file_ptr);
    
    if(fwriteerror != 1){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    fwriteerror = fwrite(&(bmp_pic->ih.biWidth), sizeof(int), 1, file_ptr);
    
    if(fwriteerror != 1){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    fwriteerror = fwrite(&(bmp_pic->ih.biHeight), sizeof(int), 1, file_ptr);
    
    if(fwriteerror != 1){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    fwriteerror = fwrite(&(bmp_pic->ih.biPlanes), sizeof(short), 1, file_ptr);
    
    if(fwriteerror != 1){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    fwriteerror = fwrite(&(bmp_pic->ih.biBitCount), sizeof(short), 1, file_ptr);
    
    if(fwriteerror != 1){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    fwriteerror = fwrite(&(bmp_pic->ih.biCompression), sizeof(int), 1, file_ptr);
    
    if(fwriteerror != 1){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    fwriteerror = fwrite(&(bmp_pic->ih.biSizeImage), sizeof(int), 1, file_ptr);
    
    if(fwriteerror != 1){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    fwriteerror = fwrite(&(bmp_pic->ih.biXPelsPerMeter), sizeof(int), 1, file_ptr);
    
    if(fwriteerror != 1){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    fwriteerror = fwrite(&(bmp_pic->ih.biYPelsPerMeter), sizeof(int), 1, file_ptr);
    
    if(fwriteerror != 1){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    fwriteerror = fwrite(&(bmp_pic->ih.biClrUsed), sizeof(int), 1, file_ptr);
    
    if(fwriteerror != 1){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    fwriteerror = fwrite(&(bmp_pic->ih. biClrImportant), sizeof(int), 1, file_ptr);
    
    if(fwriteerror != 1){
        *error=FWRITE_ERROR;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    
    long header_end = ftell(file_ptr);      //ftell tell us the header header_end.
    
    if(header_end == -1){                   //ftell error checkup.                 
        *error = errno;		
        freeBMP(bmp_pic);
        fclose(file_ptr);
        return -1;
    }
    
    long difference = bmp_pic->fh.bfOffBits - header_end;   //as in load_BMP we check the alignment.
    
    if(difference != 0){
        
        fwriteerror = fwrite(bmp_pic->alignment, sizeof(char), difference, file_ptr);
        
        if (fwriteerror != difference){                     //fwrite error checkup.
            
            *error = FREAD_ERROR;		
            freeBMP(bmp_pic);
            fclose(file_ptr);
            return -1;
        }
        
    } 
    
    int i;
    
    for(i = 0; i < bmp_pic->ih.biHeight; i++){              //We go through all the pixels 
                                                            //and read them all at once.
        //for(j = 0; j < bmp_pic->ih.biWidth; j++){	
        
        fwriteerror = fwrite(&(bmp_pic->im[bmp_pic->ih.biWidth*i]), sizeof(PIXELS), bmp_pic->ih.biWidth, file_ptr); //A pixel is written
        
        if(fwriteerror!=bmp_pic->ih.biWidth){               //fwrite error checkup.
            
            *error=FWRITE_ERROR;		
            freeBMP(bmp_pic);
            fclose(file_ptr);
            return -1;
            
        }
                
        if(bmp_pic->padding!=0){    //each time we finish writting a line, we write padding
            
            char a [3] = {0,0,0};   //we create a buffer with the maximum size of a possible padding.
            
            fwriteerror = fwrite(a, sizeof(char), bmp_pic->padding, file_ptr);
            
            if(fwriteerror!=bmp_pic->padding){  //fwrite error checkup.
                
                *error=FWRITE_ERROR;		
                freeBMP(bmp_pic);
                fclose(file_ptr);
                return -1;
                
            }
            
        }
        
    }
    
    freeBMP(bmp_pic);              //after writting all the data we free the temporal file and wew close it.
    fclose(file_ptr);
    return 0;                      //We return 0, which means success!
    
}

void freeBMP(BMPFILE *bmp_pic){     //Function used to free a BMPFILE structure.
    
    free(bmp_pic->alignment);
    free(bmp_pic->im);
    free(bmp_pic);
    
}


