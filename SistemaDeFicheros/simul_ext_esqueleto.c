#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "simul_ext_func.c"

#define LONGITUD_COMANDO 100

//Pongo las X a las funcione que estan hechas ya escritas (no dejadas en comentario) y si alguien esta empezandolas que ponga su nombre
void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps); // X

int ComprobarComando(char *strcomando, char datosComando[][LONGITUD_COMANDO]); // X

void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup); // X

int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombre);

void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);

int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombreantiguo, char *nombrenuevo);

int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
             EXT_DATOS *memdatos, char *nombre);

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

//*algo que devuelva el inodo a ocupar

int main() {
	
//	String pato = string(stdin);
	
	
	char comando[LONGITUD_COMANDO];
	char datosComando[DATOSCOMANDO][LONGITUD_COMANDO];
	
//	char orden[LONGITUD_COMANDO];
//	char argumento1[LONGITUD_COMANDO];
//	char argumento2[LONGITUD_COMANDO];

	
	printf("��%i��",ComprobarComando(zaborra = string(stdin),datosComando));
	printf("\n\n�%s %s %s�",datosComando[0], datosComando[1], datosComando[2]);
	free(zaborra);
	
	
	return 0;
	
	/* 
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
    ...
     
    fent = fopen("particion.bin","r+b");
    fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);    
     
     
    memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
    memcpy(&directorio,(EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
    memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
    memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
    memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);
     
    // Buce de tratamiento de comandos
    for (;;){
		do {
		printf (">> ");
		fflush(stdin);
		fgets(comando, LONGITUD_COMANDO, stdin);
		} while (ComprobarComando(comando,datosComando));
	    if (!strcmp(orden,"dir")) {
            Directorio(&directorio,&ext_blq_inodos);
            continue;
    		}
        ...
        // Escritura de metadatos en comandos rename, remove, copy     
        Grabarinodosydirectorio(&directorio,&ext_blq_inodos,fent);
        GrabarByteMaps(&ext_bytemaps,fent);
        GrabarSuperBloque(&ext_superblock,fent);
        if (grabardatos)
        	GrabarDatos(&memdatos,fent);
        grabardatos = 0;
        //Si el comando es salir se habrán escrito todos los metadatos
        //faltan los datos y cerrar
        if (strcmp(orden,"salir")==0){
        	GrabarDatos(&memdatos,fent);
            fclose(fent);
            return 0;
        }
    }*/
}



int ComprobarComando(char *strcomando, char datosComando[][LONGITUD_COMANDO]) {
	if (!strcmp(strcomando, "\n")){
		return 1;
	}
	
	int i = 0;
	String partes;
	
    
   
    
	//Se crea un array de char para guardar una copia de strcomando.
    //Da error poruqe el tamaño del array hay que mantener constante... a no ser....
    char completo[strlen(strcomando)+1];
    
	
	//Se vuelca strcomando en completo. Se hace con memcpy para copiar tambien el fin de cadena.
	memcpy(completo, strcomando, strlen(strcomando)+1);
	
	//Se separa por partes:
	partes = strtok(completo, " ");
	
	
	do {
		strcpy(datosComando[i++], partes);
	} while(partes = strtok(NULL, " "));
	
	return 0;
}
/*
//Aqui me lo pongo para no olvidarme de ninguno
unsigned int s_inodes_count;  X        
  unsigned int s_blocks_count; X         
 unsigned int s_free_blocks_count;X   
unsigned int s_free_inodes_count;  X  
  unsigned int s_first_data_block;    X  
  unsigned int s_block_size;   X     
  unsigned char s_relleno[SIZE_BLOQUE-6*sizeof(unsigned int)]; 
*/
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){
    printf("Numero de Inodos: %i", psup -> s_inodes_count);
    printf("Numero de Bloques: %i", psup -> s_blocks_count);
    printf("Numero de Bloques Libres: %i", psup -> s_free_blocks_count);
    printf("Numero de Inodos Libres: %i", psup -> s_free_inodes_count);
    printf("Primer Bloque de Data: %i", psup -> s_first_data_block);
    printf("Tamaño de bloque: %i", psup -> s_block_size);
    printf("Tamaño de Relleno: %i", psup -> s_relleno);
}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
    
    //Bit map de inodos (como en los ejercicios en clase)
    printf("\nBitmap De Inodos: ");
    //Hacemos un bucle donde pasamos por todos los elemnetos que se guarda en el bmap_inodos
    for(int i =0;i<MAX_INODOS;i++){
        printf("%i ",ext_bytemaps->bmap_inodos[i]);
    }
    //Lo mismo de antes pero con Bitmaps de bloque
    printf("\nBitmap De Bloques: ");
    for(int j =0;j<MAX_BLOQUES_PARTICION;j++){
        printf("%i ",ext_bytemaps->bmap_bloques[j]);
    }
    //Lo mismo de antes pero con Bitmaps de relleno (no se que es pero venga esto tiene un bitmap asique imprimir)
    printf("\nBitmap De Relleno: ");
    for(int k =0;k<SIZE_BLOQUE-(MAX_BLOQUES_PARTICION+MAX_INODOS)*sizeof(char);k++){
        printf("%i ",ext_bytemaps->bmap_relleno[k]);
    }
}