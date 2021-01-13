#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "simul_ext_func.c"

#define LONGITUD_COMANDO 100

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

//*algo que devuelva el inodo a ocupar

int main() {
	
//	String pato = string(stdin);
	
	
	char comando[LONGITUD_COMANDO];
	char datosComando[DATOSCOMANDO][LONGITUD_COMANDO];
	
//	char orden[LONGITUD_COMANDO];
//	char argumento1[LONGITUD_COMANDO];
//	char argumento2[LONGITUD_COMANDO];

	
	//printf("��%i��",ComprobarComando(zaborra = string(stdin),datosComando));
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
	Renombrar(directorio, &ext_blq_inodos, "CHISTE.txt", "NOTUVOGRASIA.TXT");
	Directorio(directorio, &ext_blq_inodos);
	
	
	
	
	
	
	
	
	
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
    char completo[strlen(strcomando)+1];
    
	//Se vuelca strcomando en completo. Se hace con memcpy para copiar tambien el fin de cadena.
	memcpy(completo, strcomando, strlen(strcomando)+1);
	
	//Se separa por partes:
	partes = strtok(completo, " ");
	
	//Se guarda el comando por partes.
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
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){ //COMANDO info
    printf("\nNumeros de Inodos: %i", psup -> s_inodes_count);
    printf("\nNumeros de Bloques: %i", psup -> s_blocks_count);
    printf("\nNumeros de Bloques Libres: %i", psup -> s_free_blocks_count);
    printf("\nNumeros de Inodos Libres: %i", psup -> s_free_inodes_count);
    printf("\nPrimer Bloque de Data: %i", psup -> s_first_data_block);
    printf("\nTamaino de Bloque: %i", psup -> s_block_size);
    //printf("Tamaino de Relleno: %i", psup -> s_relleno);
}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){ //COMANDO bytemaps
    int i;
    /*Bit map de inodos (como en los ejercicios en clase)*/
    
    //Hacemos un bucle donde pasamos por todos los elemnetos que se guarda en el bmap_inodos
    printf("\nInodos :");
    for(i=0;i<MAX_INODOS;i++) {
        printf("%i ",ext_bytemaps->bmap_inodos[i]);
	}
	
    //Lo mismo de antes pero con Bitmaps de bloque
    printf("\nBloques [0-25] :");  
	for(i=0;i<25;i++) {
        printf("%i ",ext_bytemaps->bmap_bloques[i]);
	}

    //Lo dejo comentado por razones... porque no se en teoria todo lo relacionado con relleno no deberia de existir...
    //Lo mismo de antes pero con Bitmaps de relleno (no se que es pero venga esto tiene un bitmap asique imprimir)
    /*printf("\nBitmap De Relleno: ");
    i -= SIZE_BLOQUE-(MAX_BLOQUES_PARTICION+MAX_INODOS)*sizeof(char);
    while(++i) {
        printf("%i ",ext_bytemaps->bmap_relleno[-i]);
	}
*/
    
}


// A partir de blq_inodos
/*typedef struct {
  unsigned int size_fichero;
  unsigned short int i_nbloque[MAX_NUMS_BLOQUE_INODO];
} EXT_SIMPLE_INODE;
*/

//Estructura de *inodos
/*typedef struct {
  EXT_SIMPLE_INODE blq_inodos[MAX_INODOS];
  unsigned char blq_relleno[SIZE_BLOQUE-MAX_INODOS*sizeof(EXT_SIMPLE_INODE)];
} EXT_BLQ_INODOS;
*/

//Estructura de *directorio
/*typedef struct {
  char dir_nfich[LEN_NFICH];
  unsigned short int dir_inodo;
} EXT_ENTRADA_DIR;
*/
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos){ //COMANDO dir
	//Se iterara sobre los directorios usados o borrados hasta llegar a uno vacio (o al maximo de ellos en su defecto).
	//Esta expresion aparecera repetidas veces durante el codigo.
	// SOP	     BORRADO , i<MAX
	//		     00  01  11  10
	// NULL   0  0   1   1   0
	//		  1  0   0   1   0
	
	int i = 0;
	while( (directorio->dir_inodo != NULL_INODO || strcmp(directorio->dir_nfich, "" ) == 0) && i++ < MAX_INODOS ) {
		//Si el nombre de 
		if( strcmp(directorio->dir_nfich, "") == 0 || strcmp(directorio->dir_nfich, ".") == 0 )
			++directorio;
		else {
			//Info de directorio.
			printf("[%s]\t| tamaino: %i\t| inodo: %i  bloques: ", directorio->dir_nfich, inodos->blq_inodos[directorio->dir_inodo].size_fichero, directorio->dir_inodo);
			
			//Bloques que ocupa el directorio.
			{
				int j=0;
				while((*(inodos->blq_inodos[directorio->dir_inodo].i_nbloque+j) != NULL_BLOQUE) && j < MAX_NUMS_BLOQUE_INODO)
					printf("%i ", *(inodos->blq_inodos[directorio->dir_inodo].i_nbloque+j++));
					++directorio;
				printf("\n");
			}
		}
	}
}

//Un poco como indica el nombre, se busca el fichero deseado y devuelve la posicion del fichero (es lo que se me ocurrio ya que devuelve un int)
int BuscaFich(EXT_ENTRADA_DIR *directorio, char *nombre) {
    //bucle para buscar todos los directorios
    int i;
    //como no hay un numero maximo de ficheros, pues se imporvisa
    for (i = 0; directorio ->dir_inodo != 0xFFFF; i++)
    {
        //Se compara el nombre del fichero con todos los existentes y si coinciden con el correcto, se devulve i, que es la de verces que se tuvo que recorrer este bucle para encontrar en nombre
        if(strcmp(nombre,(directorio++)->dir_nfich)==0){
            return i;
        }
    }
    
        //comprobar si es el mismo nombre de fichero
    return 0;
}

//Estructura de *inodos
/*typedef struct {
  EXT_SIMPLE_INODE blq_inodos[MAX_INODOS];
  unsigned char blq_relleno[SIZE_BLOQUE-MAX_INODOS*sizeof(EXT_SIMPLE_INODE)];
} EXT_BLQ_INODOS;
*/

//Estructura de *directorio
/*typedef struct {
  char dir_nfich[LEN_NFICH];
  unsigned short int dir_inodo;
} EXT_ENTRADA_DIR;
*/
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo){
int position;

    //Con esto utilizamos buscaFich que se creo para ver si coincide con el nombre nuevo, en caso de ser hasi se sale de la funcion
    if (BuscaFich(directorio,nombrenuevo) != 0){
        printf("\n Este nombre ya esta cogido \n");
        return 1;
    }
    //Si por el contrario, no es el if de arriba, se busca y se mira en caso de existir el nombreantiguo quepuso, y en caso correcto, 
    //Se ve que fichero es, (su posicion) y luego se sobre escribe el nombres
    if(BuscaFich(directorio,nombreantiguo)){
        position=BuscaFich(directorio,nombreantiguo);
        strcpy(directorio[position].dir_nfich,nombrenuevo);
        return 1;
    }

}

int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre){
	//coso->bloque->info
	
	//buscaDir
	//bu
}