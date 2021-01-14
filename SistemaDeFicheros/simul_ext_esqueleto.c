#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "simul_ext_func.c"



//Pongo las X a las funcione que estan hechas ya escritas (no dejadas en comentario) y si alguien esta empezandolas que ponga su nombre. Tambien XX significa "escrito y ejecutado(probado)"
void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps); // XX

int ComprobarComando(char *strcomando, char datosComando[][LONGITUD_COMANDO]); // XX

void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup); // XX

int BuscaFich(EXT_ENTRADA_DIR *directorio, char *nombre); // XX

void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos); // XX

int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombreantiguo, char *nombrenuevo); //XX

int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
             EXT_DATOS *memdatos, char *nombre);// Danila

int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *nombre,  FILE *fich);

int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);

void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);

void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);

void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);

void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);

//int InsufEspacio(EXT_BLQ_INODOS ext_blq_inodos0);

//int ContarBloques()

//int ComprInodos()

//*algo que devuelva el inodo a ocupar

int main() {
	
//	String pato = string(stdin);
	
	
	char comando[LONGITUD_COMANDO];
	char datosComando[DATOSCOMANDO][LONGITUD_COMANDO];
	
//	char orden[LONGITUD_COMANDO];
//	char argumento1[LONGITUD_COMANDO];
//	char argumento2[LONGITUD_COMANDO];

	
	//printf("ï¿½ï¿½%iï¿½ï¿½",ComprobarComando(zaborra = string(stdin),datosComando));
	//printf("\n\n%s %s %s\n",datosComando[0], datosComando[1], datosComando[2]);
	//free(zaborra);
	
	//return 0;
	
	int i,j;
	unsigned long int m;
    EXT_SIMPLE_SUPERBLOCK ext_superblock;
    EXT_BYTE_MAPS ext_bytemaps;
    EXT_BLQ_INODOS ext_blq_inodos;
    EXT_ENTRADA_DIR directorio[MAX_FICHEROS];
    EXT_DATOS memdatos[MAX_BLOQUES_DATOS];
    EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
    int entradadir;
    int grabardatos;
    FILE *fent;
    
     
     
    // Lectura del fichero completo de una sola vez
     
    fent = fopen("particion.bin","r+b");
    fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);    
     
     
    memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
    memcpy(&directorio,(EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
    memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
    memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
    memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);
     
	//Printbytemaps(&ext_bytemaps);
	//LeeSuperBloque(&ext_superblock);
	//BuscaFich(directorio[3],"BelloGal.txt");
	//Renombrar(directorio, &ext_blq_inodos, "NOTUVOGRASIA.TXT", "CHISTE.txt");

	//Imprimir(directorio,&ext_blq_inodos,memdatos,"BelloGal.txt");
	//Borrar(directorio,&ext_blq_inodos ,&ext_bytemaps, &ext_superblock,"HOLA.txt", fent);
	Directorio(directorio, &ext_blq_inodos);
//	InsufEspacio(&ext_bytemaps);
//	printf("%i",ComprInodos(&ext_bytemaps));
	
	return 0;
	
	
	
	/* 
    // Buce de tratamiento de comandos
    for (;;){
		do {
		printf (">> ");
		fflush(stdin);
		fgets(comando, LONGITUD_COMANDO, stdin);
		} while (ComprobarComando(comando,datosComando));
		//Comando tiene todo
		//datosComando
		//(Hay 7 comandos + (salir))
	    if (!strcmp(orden,"dir")) {
            Directorio(&directorio,&ext_blq_inodos);
            continue;
    		}
    	if (!strcmp(orden,"bytemaps")) {
            LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
            continue;
    		}
    	if (!strcmp(orden,"rename")) {
            Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo);
            continue;
    		}
    	if (!strcmp(orden,"imprimir")) {
           Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre);
            continue;
    		}
    	if (!strcmp(orden,"bytemaps")) {
            Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
            continue;
    		}
    	if (!strcmp(orden,"remove")) {
            Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,char *nombre,  FILE *fich);
            continue;
    		}
    	if (!strcmp(orden,"copy")) {
           int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
            continue;
    		}
    
        // Escritura de metadatos en comandos rename, remove, copy     
        Grabarinodosydirectorio(&directorio,&ext_blq_inodos,fent);
        GrabarByteMaps(&ext_bytemaps,fent);
        GrabarSuperBloque(&ext_superblock,fent);
        if (grabardatos)
        	GrabarDatos(&memdatos,fent);
        grabardatos = 0;
        //Si el comando es salir se habrÃ¡n escrito todos los metadatos
        //faltan los datos y cerrar
        if (strcmp(orden,"salir")==0){
        	GrabarDatos(&memdatos,fent);
            fclose(fent);
            return 0;
        }
    }*/
}


