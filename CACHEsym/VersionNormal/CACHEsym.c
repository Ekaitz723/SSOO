#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Lineas en cache:
#define LINEAS 4

//Bytes por linea:
#define BYTESLINEAS 8

#define HDIRECCION (int)ceil((float)BYTESLINEAS/2.0)

//RECOLECTOR DE BASURA
void* zaborra;

typedef struct {
	short int ETQ;
	short int Datos[BYTESLINEAS];
} T_LINEA_CACHE;

/*Inicializa a 0xFF las etiquetas y a 0 los datos de un T_LINEA_CACHE.
 *Recoge T_LINEA_CACHE* e int con la cantidad de ellas.
 *No devuelve nada (void).
 */
void inicializarCache(T_LINEA_CACHE *,int);

/*Consigue una linea de un FILE*. Hara falta gestinar el acceso al puntero para hacer despues free(FILE*).
 *Recoge el fichero FILE*.
 *Devuelve una linea de FILE* (char*).
 */
char* fconstr(FILE *);

/*Convierte numero hexadecimal en char* a int de cada linea de un FILE*.
 *Recoge el fichero FILE*.
 *Devuelve el numero hexadecimal de la linea a la que este apuntando FILE* (int).
 */
int fstrtx(FILE *);

/*Muestra mensaje de error y mata el proceso.
 *Recoge el mensaje (char*).
 *Obviamente no devuelve nada (void).
 */
void nofufa(char *);


int main(int argc, char *argv[]) {
	printf("º%iº",sizeof(char));
	return 1;
//!0	Texto.
	//Guarda el tamaÃo de la cadena texto.
	int tamTexto=1;

	//En cuanto sumamos el realloc en texto.
	int aumentoTexto=15;
	
	//Char* para guardar el texto.
	char* texto;
	texto = (char*)malloc(sizeof(char)*aumentoTexto);
	if (!texto)
		nofufa("\nError al hacer malloc.\n");
//!0	

//!1	Preparaciones para el bucle principal e inicializaciones.

	//Variables inicializadas.
	int tiempoglobal = 0;
	int numfallos = 0;
	
	//Lineas de la cache. Crear array e inicializar cada elemento.
	T_LINEA_CACHE lineas[LINEAS];
	inicializarCache(lineas,LINEAS);
	
	//Puntero a RAM para ir leyendolo en vez de almacenarlo por completo.
	FILE *pRAM = fopen("RAM.bin","rb");
	if(!pRAM)
		nofufa("No se encuentra 'RAM.bin'.\nRevisa si se encuentra en el mismo directorio.\n");
	
	//Puntero a accesos_memoria.txt.
	FILE *accesos = fopen("accesos_memoria.txt","r");
	if(!accesos)
		nofufa("No se encuentra 'accesos_memoria.txt'.\nRevisa si se encuentra en el mismo directorio.\n");
	
	//Variables para guardar el dato, la etiqueta, la linea y la palabra. Optimizados para este ejercicio.
	//En caso de modificar la info sobre la cache, por ejemplo, cambiando la cantidad de lineas,
	//cambiarlo a int, o optimizarlo para ese caso.
	int dato;
	int etq;
	char lin;
	short int pal;
//!1

//!2	Bucle principal.
	while(!feof(accesos)) {
		
		//Se lee una direccion del fichero.
		dato = fstrtx(accesos);
		
		//A FUNCION
		//Se obtiene etq y linea con mascaras de bit y desplazamientos a nivel de bit.
		pal = dato		& 0b111;
		lin = (dato>>3)	& 0b11;
		etq = (dato>>5)	& 0b11111;
		//En ese ultimo realmente no hacia falta ya que en los datos todo apartir de ahi es 0, pero si cambian los datos esto cogeria bien la etiqueta aun el cambio.
		
		//Si no esta cargado en CACHE.
		if(lineas[lin].ETQ!=etq) {
			//Se imprime:
			printf("\nT: %d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X bloque %02X", tiempoglobal, numfallos++,dato,etq,lin,pal,lin);
			
			tiempoglobal+=10;
			
			printf("\nSe esta cargando el bloque %02X en la linea %02X",lin,lin);

			//Mueve el puntero pRAM y lee los datos.
			fseek(pRAM,dato>>3,SEEK_SET);
			fread(lineas[lin].Datos, 4, 8, pRAM);
			lineas[lin].ETQ=etq;
		}
		
		//Se imprime:
		printf("\nT: %d, Acierto de CACHE, ADDR %04X ETQ %X linea %02X palabra %02X DATO %02X", tiempoglobal,dato,etq,lin,pal,lineas[lin].Datos[pal]);
		//TODO imprimir lo que falta.
		
		//Se ainade al texto el caracter DATO.
		texto[tamTexto-1] = lineas[lin].Datos[pal];
		tamTexto++;
		if( !(tamTexto%aumentoTexto) ) {
			texto = (char*)realloc(texto,tamTexto+aumentoTexto);
			
			//Si hubo algun problema con el realloc, saldra.
			if(!texto) {
				nofufa("Error con el malloc.\n");
			}
		}
		
//		sleep(2);
	}
//!2

//!3	Fin del programa. Se imprime lo siguiente:
	
	//TODO fclose();
	
	//Se imprime el numero total de accesos, numero de fallos y tiempo medio de acceso.
	printf("\nAccesos: %i, numero de fallos: %i, tiempo medio de acceso: %f\n",tamTexto-1,numfallos,0.0);
	
	//Se completa la cadena con un final de cadena.
	texto[tamTexto-1]='\0';
	
	//Se imprime el mensaje con el texto leido caracter a caracter desde la cache.
	printf(texto);
//!3
	
	free(texto);
	free(zaborra);
	
	return 0;
}

void inicializarCache(T_LINEA_CACHE *lineas, int cantLineas) {
	if(cantLineas--) {
		return;
	}
	
	int i=0;
	lineas[cantLineas].ETQ = 0xFF;
	while(i++<BYTESLINEAS)
		lineas[cantLineas].Datos[i]=0;
	
	inicializarCache(lineas,cantLineas);
}

char* fconstr(FILE *f) {

	//En cuanto sumamos el realloc
	int aumento=6;

	//Guarda la cadena que se devolvera.
	char* str;
	str = (char*)malloc(sizeof(char)*aumento);	
	if (!str) {
		nofufa("\nError al leer");
	}
	
	//Guarda el tamaino de la cadena.
	int tam=1;

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
}

int fstrtx(FILE *f) {
	(char*)zaborra = fconstr(f);
	return (int)strtol(zaborra, NULL, 16);
}

void nofufa(char* mensaje) {
	printf("\n");
	printf(mensaje);
	exit(-1);
}
