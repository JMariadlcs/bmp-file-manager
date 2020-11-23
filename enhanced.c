
/**enhanced.c*****************************************************************

  Fichero     enhanced.c

  Resumen     All method related with enhanced version

  Descripción This class contains all method releated with the enhanced
			  version. The managing of complex orders and commands are
			  done in this methods

  Autor       Francisco Atalaya, Jose María de La Cruz
			  Patricia Paredes, Iosif Mondoc

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
#include "enhanced.h"
#include "error.h"
/*---------------------------------------------------------------------------*/
/* Definición de las funciones                                               */
/*---------------------------------------------------------------------------*/

void printParamMenu(){
	printf("Program for handling and editing BMP files.\n\n");
	printf("Usage:\n");
	printf("./saucem_enhanced [--help] [--sort size|name|time|default] [-l logfile] [-i file] [-o file] [--clean] [--no-save]\n\n");
	printf("--help                          shows this text and quits\n");
	printf("--sort size|name|time|default   sets the internal order of the collection:\n");
	printf("                                   size    By file size, ascendant\n");
	printf("                                   name    By file name\n");
	printf("                                   time    By modification time\n");
	printf("                                   default By node identifier\n");
	printf("-l logfile                      sets the path of the log file\n");
	printf("-i file                         indicates the file from where the initial\n");
	printf("                                collection is read\n");
	printf("-o file                         indicates the file where the collection is\n");
	printf("                                stored at the end of the program\n");
	printf("--clean                         The program begins with an empty collection\n");
	printf("--no-save                       The collection is not stored at the end\n");
}
void printParam(struct options * opt){
	if(opt->help==1){				//if a --help has been introduced ir prints and close the program
		printParamMenu();
	}else{							//if nt help: it prints the parameters introduced
		printf("Parameters:\n");
		if(opt->l==1){printf("Log file: %s\n",opt->logfile);}
		if(opt->i==1){printf("Initial file: %s\n",opt->initfile);}
		if(opt->o==1){printf("Persistence file: %s\n",opt->savefile);}
		
		printf("Internal order: ");
		if(opt->sort_type==0){printf("by ID\n");}
		if(opt->sort_type==1){printf("by size\n");}
		if(opt->sort_type==2){printf("by name\n");}
		if(opt->sort_type==3){printf("by time\n");}
	}
}
struct options* fillStructOpt(int argc,char *argv[], struct options* opt,int *error){
	
	//initalating the struct
	opt->help=0;		//boolean variable
	opt->sort_type=0; 	//0-Default 1-size 2-name 3-time. Initialized in default(id)
	opt->l=0;			//boolean variable
	opt->logfile=NULL;
	opt->i=0;			//boolean variable
	opt->initfile=NULL;
	opt->o=0;			//boolean variable
	opt->savefile=NULL;
	opt->clean=0;		//boolean variable
	opt->no_save=0;		//boolean variable
	
	int i;
	for(i=1;i<argc;i++){		//traveling the argv array
		
		//--help
		if(strcmp(argv[i],"--help")==0){
			opt->help=1;
		
		
		//--sort type
		}else if(strcmp(argv[i],"--sort")==0){
			
			if(i==(argc-1)){						//limit reach:we cant compare i+1 because it is a segmentation fault
													//it is argc-1 because the array index starts at 0
				*error=ERR_SORT_ARG;
			}else if(strcmp(argv[i+1],"size")==0){
				opt->sort_type=1;
				i++; 								//jump the array index where the type is
			}else if(strcmp(argv[i+1],"name")==0){
				opt->sort_type=2;
				i++; 
			}else if(strcmp(argv[i+1],"time")==0){
				opt->sort_type=3;
				i++; 
			}else if(strcmp(argv[i+1],"default")==0){
				opt->sort_type=0;
				i++; 
			}else{									//sort type non introduced
				*error=ERR_SORT_ARG;
				
			}
		
		
		//-l logfile
		}else if(strcmp(argv[i],"-l")==0){
			
			if(i==(argc-1)){					//limit reach:we cant compare i+1 because it is a segmentation fault
												//it is argc-1 because the array index starts at 0
				*error=ERR_LOGFILE;
			
			}else if(argv[i+1][0]=='-'){		// error:logfile non introduced, then, the next index is another
				*error=ERR_LOGFILE;				// parameter or the end of the array is reched(case above);
			}else{
				opt->l=1;						//no error-> set to one the boolean int
				opt->logfile=strdup(argv[i+1]);	//save the filename
				i++;							//jump the array index where the filename is
			}
		
	
		//-i file
		}else if(strcmp(argv[i],"-i")==0){
			
			if(i==(argc-1)){						//limit reach:we cant compare i+1 because it is a segmentation fault
													//it is argc-1 because the array index starts at 0
				*error=ERR_INIT_FILE;
			
			}else if(argv[i+1][0]=='-'){ 			//initial file non introduced
				*error=ERR_INIT_FILE;
			}else{
				opt->i=1;							//no error-> set to one the boolean int
				opt->initfile=strdup(argv[i+1]);	//save the filename
				i++;								//jump the array index where the filename is
			}
			
		
		
		//-o file
		}else if(strcmp(argv[i],"-o")==0){
			if(i==(argc-1)){				//limit reach:we cant compare i+1 because it is a segmentation fault
											//it is argc-1 because the array index starts at 0
				*error=ERR_SAVE_FILE;
			
			}else if(argv[i+1][0]=='-'){ 	//save file non introduced
				*error=ERR_SAVE_FILE;
			}else{
				opt->o=1;							//no error-> set to one the boolean int
				opt->savefile=strdup(argv[i+1]);	//save the filename
				i++;								//jump the array index where the filename is
			}
		
		//--clean
		}else if(strcmp(argv[i],"--clean")==0){
			//empty collection init
			opt->clean=1;	//set to one the boolean int
			
		
		
		}else if(strcmp(argv[i],"--no-save")==0){
			opt->no_save=1;	//set to one the boolean int
			
		}else{
			*error=ERR_INVALID_COMMAND;
		}
	}
	//contradictions manage->if two parameters are contradictories it is an error
	if( (opt->clean==1&&opt->i==1) || (opt->o==1&&opt->no_save==1)){
		*error= ERR_CONTR_COMMAND;
	}
	return opt;
}
void free_opt(struct options * opt){
	
	if(opt->l==1){			//if logfile used
		free(opt->logfile);	//free the strdup
	}
	if(opt->i==1){			//if initfile used
		free(opt->initfile);//free the strdup
	}
	if(opt->o==1){			//if savefile used
		free(opt->savefile);//free the strdup
	}
	free(opt);
}



