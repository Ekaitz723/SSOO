#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cabeceras.h"

//Devuelve true si es que no es posible crear otro fichero (copiar el fichero).
int InsufEspacio(EXT_BYTE_MAPS *ext_bytemaps,unsigned short int *bloques , char *nombre) {
	int i;
	
	//SE USARA EN COPIAR
	//pepe = inodos->blq_inodos[BuscaFich(directorio,nombre)].i_nbloque;
	
	int cantBloquesNecesarios = ContarBloques(bloques,0);
//	int pos = posicion=BuscaFich(directorio,nombre);
	
	/*Recorremos los bytemaps hasta encontrar un 0. Entoces devolvemos false. En el caso del bytemap de bloques, se necesitaran la misma cantidad de 0 que de bloques que ocupa el fichero pasado por parametro.*/
	
	if(ComprInodos())
		return 1;

	for(i=0;i<CANTDATOSBITMAP;i++) {
        printf("%i ",ext_bytemaps->bmap_bloques[i]);
	}
	
	return 0;
	 
}

//Devuelve false si hay un inodo libre.
int ComprInodos(EXT_BYTE_MAPS *ext_bytemaps) {
	int i;
	for(i=0;i<MAX_INODOS;i++) {
        if(!ext_bytemaps->bmap_inodos[i])
        	return 0;
	}
	printf("No quedan inodos libres. Borra algun fichero.\n");
	return 1;
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
	for(i=0;i<CANTDATOSBITMAP;i++) {
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

void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos){ //COMANDO dir
	//Se iterara sobre los directorios usados o borrados hasta llegar a uno vacio (o al maximo de ellos en su defecto).
	//Esta expresion aparecera repetidas veces durante el codigo.
	// SOP	     BORRADO , i<MAX
	//		     00  01  11  10
	// NULL   0  0   1   1   0
	//		  1  0   0   1   0
	
	int i = 0;
	while( (directorio->dir_inodo != NULL_INODO || strcmp(directorio->dir_nfich, "" ) == 0)  && i++ < MAX_INODOS ) {
		//Si el nombre de 
		if( strcmp(directorio->dir_nfich, "") == 0 || strcmp(directorio->dir_nfich, ".") == 0 )
			++directorio;
		else {
			//Info de directorio.
			printf("[%s]\t| tamaino: %i\t| inodo: %i  bloques: ", directorio->dir_nfich, inodos->blq_inodos[directorio->dir_inodo].size_fichero, directorio->dir_inodo);

			//Bloques que ocupa el directorio.
            printf(" [%i]\n",ContarBloques(inodos->blq_inodos[(directorio++)->dir_inodo].i_nbloque,inodos,1));
		}
	}
}

//Devuelve la cantidad de bloques que ocupa un fichero
int ContarBloques(unsigned short int *bloque, int verbose) {
	int j;
	for(j=0;(*(bloque+j) != NULL_BLOQUE) && j < MAX_NUMS_BLOQUE_INODO;j++) {
		if(verbose)
			printf("%i ", *(bloque+j));
	}	
	return j;
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
//OBJETIVO: Muestra el contenido del fichero especificado como un texto. DeberÃ¡ comprobar que el
//fichero existe. Si el fichero ocupa mÃ¡s de un bloque, debe aparecer en el orden
//correcto.

//Codigo:  
int i,j,posicion; 
//Se busca el directorio que se desea imprimir en un bucle que pase por todos los directorios
    if(BuscaFich(directorio,nombre))
    {
    //consigue la cantidad de bloques
    posicion=BuscaFich(directorio,nombre);
        //Bucle donde va imprimiendo bloque a bloque EN ORDEN
        int numBloques
        for ( i = 0; i < ; i++)
        {
        	//inodos->blq_inodos[directorio[posicion].dir_inodo] coincide mucho y te sincroniza el nodo del directorio con los bloques que le corresponden 
        	//el -4 y +4 coinciden con el hecho de usa dos factores, el azar y leerse la practica donde dice que las tres primeras posiciones estan ocupadas 
			//y ya en la 4ï¿½ para arriba estan ocupadas por los bloques de datos (se ve en la linea 87 aprox)
			//EOF es 0.
           memdatos = memdatos + inodos->blq_inodos[directorio[posicion].dir_inodo].i_nbloque[i]-4; 
           for (j=0; j<= SIZE_BLOQUE && memdatos->dato[j] != 0; j++){
				printf("%c", memdatos->dato[j]);
			}
			//le restauramos la posicion de los datos para ir a l siguiente linea (metadatos - .... y +4 al final) pero esto es solo una teria hasta un punto y me ha funcionado con esto
			memdatos = memdatos - inodos->blq_inodos[directorio[posicion].dir_inodo].i_nbloque[i]+4;
            
        }
        printf("\n");
        return 0;

        //Else dara un error de este fichero no existe, le vale verga vamos
        if (strcmp(nombre, "\n") == 0){
		printf("No me seas lelo, ponme un fichero que leer\n");
		return 1;
        }
       
        printf("Emm no se de que me hablas \n");
        

    }
    
    
}


//6 parametros, de structs que llevan a otras variables y que estas llevan a otras variables..... Ameno...
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,char *nombre,  FILE *fich){
	int i = 0, j,posicion;
    //Vamos a busacar al suertudo
	if (BuscaFich(directorio, nombre) != 0){	
		posicion= BuscaFich(directorio, nombre);
        //vamos a empezar a liberar cosas (empezando por el superbloque)
		while( (directorio->dir_inodo != NULL_INODO || strcmp(directorio->dir_nfich, "" ) == 0) && i++ < MAX_INODOS);
		//Primero el bloques libres
        ext_superblock->s_free_blocks_count += (i - 1);								
		//Luego inodos
        ext_superblock->s_free_inodes_count += 1;									
		//Ahora el nombre ya no existe
        strcpy(directorio->dir_nfich , "");											
		//le cambiamos los bit´maps tambien
        //los de inodo
        ext_bytemaps->bmap_inodos[posicion] = 0;						
		inodos->blq_inodos[posicion].size_fichero = 0;	
        printf("A");				
		for(i = 0; inodos->blq_inodos[posicion].i_nbloque[i] != NULL_BLOQUE; i++){
            //los de bloque
			ext_bytemaps->bmap_bloques[inodos->blq_inodos[posicion].i_nbloque[i]] = 0;	
		}
        printf("B");
		for(i = 0; i <=MAX_NUMS_BLOQUE_INODO; i++){
			if(ext_superblock->s_first_data_block == inodos->blq_inodos[posicion].i_nbloque[i]){
				printf("C");
                for(j = 4; j<=MAX_BLOQUES_DATOS; j++){
					if(ext_bytemaps->bmap_bloques[j] == 1){
						ext_superblock->s_first_data_block = j;			
						break;
					}
				}
			}
            //Ponienod la direccion 0xFFFF
			inodos->blq_inodos[posicion].i_nbloque[i] = NULL_BLOQUE;	
		}
        printf("D");
        //Lo mismo de antes peor con inodo
		directorio->dir_inodo = NULL_INODO;										
		return 0;
	}
	if (strcmp(nombre, "\0") == 0){
		printf("El fichero al cual quiere usted acudir esta escrito de forma erronea o no existe'\n");
		return 1;
	}
	else{
		printf("No hay ficheros, te los cargaste todos\n");
		return 1;
	}
	
}

void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich){
	rewind(fich);
	fwrite(ext_superblock, SIZE_BLOQUE, 1, fich);
}

void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich){
    fwrite(ext_bytemaps, SIZE_BLOQUE, 1, fich);
}

void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich){
	fwrite(inodos, SIZE_BLOQUE, 1, fich);
    fwrite(directorio, SIZE_BLOQUE, 1, fich);
}

void GrabarDatos(EXT_DATOS *memdatos, FILE *fich){
    fwrite(memdatos, SIZE_BLOQUE, MAX_BLOQUES_DATOS, fich);
}

/*char* string(FILE *f) {

	//En cuanto sumamos el realloc
	char aumento=AUMCONSTR;

	//Guarda la cadena que se devolvera.
	char* str;
	str = (char*)malloc(sizeof(char)*aumento);	//Aqui hay un error
	if (!str) {
		nofufa("\nError al leer");
	}
	
	//Guarda el tamaino de la cadena.
	char tam=1;

	//Guarda temporalmente cada caracter.
	char tmp = fgetc(f);
	
	//Guarda cada char en str mientras no encuentre un salto de linea o EOF.
	while( (tmp!='\n') && !feof(f)) {
		str[tam-1]=tmp;
		tam++;
		if( !(tam%aumento) ) {
			str = (char*)realloc(str,tam+aumento);
			
			//Si hubo algun problema con el realloc, saldra.
			if(!str) {
				nofufa("\nError en el realloc");
			}
		}
		tmp=getc(f);
	}
	str[tam-1]='\0';
	
	return str;
}*/

/*void nofufa(char* mensaje) {
	printf("\n");
	printf(mensaje);
	exit(-1);
}*/
