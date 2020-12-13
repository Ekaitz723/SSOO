#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Lineas en cache.
 * Bytes por linea.
 * Tamaino texto. Debe ser igual al maximo de accesos.
 * Tamaino RAM.
 * Tiempo de acceso a RAM.
 * Valor inicial de la etiqueta.
 * Valor inicial de los datos.
 */
#define LINEAS 4
#define BYTESLINEAS 8
#define MAXTEXTO 100
#define TAMRAM 1024
#define ACCRAM 10
#define ETQINI 0xFF
#define DATINI 0

//RECOLECTOR DE BASURA
void* zaborra;

typedef struct {
	short int ETQ;
	//A ver, Datos es unsigned char * y no short int [BYTESLINEAS] ya que queremos guardar chars.
	unsigned char Datos[BYTESLINEAS];
} T_LINEA_CACHE;

typedef struct {
	int etq;
	char lin;
	short int pal;
} DATOS_ACCESO;

/* Inicializa a 0xFF las etiquetas y a 0 los datos de un T_LINEA_CACHE.
 * Recoge T_LINEA_CACHE**.
 * No devuelve nada (void).
 */
void inicializarCache(T_LINEA_CACHE *);

/* Consigue una linea de un FILE*. Hara falta gestinar el acceso al puntero para hacer despues free(char*).
 * Recoge el fichero FILE*.
 * Devuelve una linea de FILE* (char*).
 */
char* fconstr(FILE *);

/*Convierte numero hexadecimal en char* a int de cada linea de un FILE*.
 *Recoge el fichero FILE*.
 *Devuelve el numero hexadecimal de la linea a la que este apuntando FILE* (int).
 */
int fstrtx(FILE *);

/* Separa un acceso (int) en tres partes: etiqueta, linea y palabra.
 * Recoge un acceso (int).
 * Devuelve una estructura DATOS_ACCESO.
 */
DATOS_ACCESO conDatosAcceso(int);

/* Imprime los datos de la cache.
 * Recoje un puntero a la cache (T_LINEA_CAHE*).
 * No devuelve nada.
 */
 void imprimirCache(T_LINEA_CACHE*);
 
/*Muestra mensaje de error y mata el proceso.
 *Recoge el mensaje (char*).
 *Obviamente no devuelve nada (void).
 */
void nofufa(char *);


int main(int argc, char *argv[]) {

//!0	Texto.

	//Char [100] para guardar el texto.
	char texto[MAXTEXTO];
	
	//En caso de querer inicializar todos sus caracteres a '\0'
//	memset(texto,'\0',sizeof(texto));
	
	//Variable que guarda tamaino actual de texto.
	int tamTexto=0;
//!0	

//!1	Preparaciones para el bucle principal e inicializaciones.

	//Variables inicializadas.
	int tiempoglobal = 0;
	int numfallos = 0;
	
	//Lineas de la cache. Crear array e inicializar cada elemento.
	T_LINEA_CACHE lineas[LINEAS];
	inicializarCache(lineas);
	
	//Puntero a RAM para volcarlo en RAM. Al finalizar el volcado se cierra el FILE*.
	unsigned char RAM[TAMRAM];
	FILE *pRAM = fopen("RAM.bin","rb");
	if(!pRAM)
		nofufa("No se encuentra 'RAM.bin'.\nRevisa si se encuentra en el mismo directorio.\n");
	fgets(RAM,TAMRAM,pRAM);
	fclose(pRAM);
	
	//Puntero a accesos_memoria.txt.
	FILE *accesos = fopen("accesos_memoria.txt","r");
	if(!accesos)
		nofufa("No se encuentra 'accesos_memoria.txt'.\nRevisa si se encuentra en el mismo directorio.\n");
	
	//Variables para guardar el dato, la etiqueta, la linea y la palabra. Optimizados para este ejercicio.
	DATOS_ACCESO dato_acceso;
	int dato;
//!1

//!2	Bucle principal.

	while(!feof(accesos)) {
		
		//Se lee una direccion del fichero.
		dato = fstrtx(accesos);
		
		//Se obtiene etq y linea con mascaras de bit y desplazamientos a nivel de bit.
		dato_acceso = conDatosAcceso(dato);
		
		tiempoglobal++;
		//Se comprueba la existencia del dato en cache.
		printf("\n");
		if(lineas[dato_acceso.lin].ETQ!=dato_acceso.etq) {
			//Se imprime:
			printf("\nT: %d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X bloque %02X", tiempoglobal, ++numfallos,dato,dato_acceso.etq,dato_acceso.lin,dato_acceso.pal,dato/BYTESLINEAS);
			
			tiempoglobal+=ACCRAM;
			
			printf("\nSe esta cargando el bloque %02X en la linea %02X",dato/BYTESLINEAS,dato_acceso.lin);

			//Copia los datos en la linea desde la RAM.
			memcpy(lineas[dato_acceso.lin].Datos, RAM+(dato&0b1111111000),BYTESLINEAS);
			lineas[dato_acceso.lin].ETQ=dato_acceso.etq;
		}
		
		//Se imprimen los datos del acceso y el contenido de la cache.
		printf("\nT: %d, Acierto de CACHE, ADDR %04X ETQ %X linea %02X palabra %02X DATO %02X", tiempoglobal,dato,dato_acceso.etq,dato_acceso.lin,dato_acceso.pal,lineas[dato_acceso.lin].Datos[dato_acceso.pal]);
		imprimirCache(lineas);
		
		//Se ainade al texto el caracter DATO.
		texto[tamTexto++] = lineas[dato_acceso.lin].Datos[dato_acceso.pal];
//		sleep(2);
	}
//!2

//!3	Fin del programa. Se imprimen datos finales y se cierra y libera todo.
	
	//Se completa la cadena con un final de cadena.
	texto[tamTexto]='\0';
	
	//Se cierran los FILE*. Si se llego hasta aca significa que accesos apunta a un fichero.
	fclose(accesos);
	
	//Se imprime el numero total de accesos, numero de fallos y tiempo medio de acceso.
	//TODO esto es el tiempo medio efectivo
	//Tiempo total/num accesos
	printf("\n\nAccesos: %i, numero de fallos: %i, tiempo medio de acceso: %.2f\nEl texto: ",tamTexto,numfallos,(float)tiempoglobal/tamTexto);
	
	//Se imprime el mensaje con el texto leido caracter a caracter desde la cache.
	printf(texto);
	
	free(zaborra);
	
	return 0;
//!3
}

void inicializarCache(T_LINEA_CACHE *lineas) {
	unsigned char i=LINEAS, a;
	
	while(i--) {
//		*(lineas+i) = (T_LINEA_CACHE){0xFF, a};
//		printf("ºpatoº%iºº",(lineas+i)->Datos[i]);
		(lineas+i)->ETQ = ETQINI;
		a=BYTESLINEAS;
		while(a--) {
			(lineas+i)->Datos[a]=DATINI;
		}
	}
}

char* fconstr(FILE *f) {

	//En cuanto sumamos el realloc
	unsigned char aumento=6;

	//Guarda la cadena que se devolvera.
	char* str;
	str = (char*)malloc(sizeof(char)*aumento);	
	if (!str) {
		nofufa("\nError al leer");
	}
	
	//Guarda el tamaino de la cadena.
	unsigned char tam=1;

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
	if(zaborra) {
		free(zaborra);
	}
	zaborra = fconstr(f);
	return (int)strtol(zaborra, NULL, 16);
	
	//Con fscanf en vez de fconstr:
//	char* linea;
//	fscanf(f, "%s", linea);
//	return (int)strtol(linea, NULL, 16);
}

DATOS_ACCESO conDatosAcceso(int acceso) {
	return (DATOS_ACCESO){(acceso>>5) & 0b11111, (acceso>>3) & 0b11, acceso & 0b111};;
	//En ese ultimo realmente no hacia falta el &b111, ya que en los datos todo apartir de ahi es 0, pero si cambian los datos esto cogeria bien la etiqueta aun el cambio.
}

 void imprimirCache(T_LINEA_CACHE *lineas) {
	//Iteradores de bucle.
	unsigned char a, i;
	
	for(a=0;LINEAS-a;a++) {
		printf("\nETQ:%02X  Datos",lineas[a].ETQ);
		i=BYTESLINEAS;
		while(i--)
			printf(" %02X", lineas[a].Datos[i]);
	}
 }
	
void nofufa(char* mensaje) {
	printf("\n");
	printf(mensaje);
	exit(-1);
}

/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Lineas en cache.
 * Bytes por linea.
 * Tamaino texto. Debe ser igual al maximo de accesos.
 * Tamaino RAM.
 * Tiempo de acceso a RAM.
 * Valor inicial de la etiqueta.
 * Valor inicial de los datos.
 */
#define LINEAS 4
#define BYTESLINEAS 8
#define MAXTEXTO 100
#define TAMRAM 1024
#define ACCRAM 10
#define ETQINI 0xFF
#define DATINI 0


//#define HDIRECCION (int)ceil((float)BYTESLINEAS/2.0)


//RECOLECTOR DE BASURA
void* zaborra;

/* COSAS
 * Comentarios generales de funciones
 * Probar con otros RAM.bin y accesos.txt
*/

typedef struct {
	short int ETQ;
	//A ver, Datos es unsigned char * y no short int [BYTESLINEAS] ya que queremos guardar chars.
	unsigned char Datos[BYTESLINEAS];
} T_LINEA_CACHE;

typedef struct {
	int etq;
	char lin;
	short int pal;
} DATOS_ACCESO;

/* Inicializa a 0xFF las etiquetas y a 0 los datos de un T_LINEA_CACHE.
 * Recoge T_LINEA_CACHE**.
 * No devuelve nada (void).
 */
void inicializarCache(T_LINEA_CACHE *);

/* Consigue una linea de un FILE*. Hara falta gestinar el acceso al puntero para hacer despues free(char*).
 * Recoge el fichero FILE*.
 * Devuelve una linea de FILE* (char*).
 */
char* fconstr(FILE *);

/*Convierte numero hexadecimal en char* a int de cada linea de un FILE*.
 *Recoge el fichero FILE*.
 *Devuelve el numero hexadecimal de la linea a la que este apuntando FILE* (int).
 */
int fstrtx(FILE *);

/* 
 * 
 * 
 */
DATOS_ACCESO conDatosAcceso(int);

/* 
 * 
 * 
 */
 void imprimirCache(T_LINEA_CACHE*);
 
/*Muestra mensaje de error y mata el proceso.
 *Recoge el mensaje (char*).
 *Obviamente no devuelve nada (void).
 */
void nofufa(char *);


int main(int argc, char *argv[]) {

//!0	Texto.

	//Char [100] para guardar el texto.
	char texto[MAXTEXTO];
	
	//En caso de querer inicializar todos sus caracteres a '\0'
//	memset(texto,'\0',sizeof(texto));
	
	//Variable que guarda tamaino actual de texto.
	int tamTexto=0;
//!0	

//!1	Preparaciones para el bucle principal e inicializaciones.

	//Variables inicializadas.
	int tiempoglobal = 0;
	int numfallos = 0;
	
	//Lineas de la cache. Crear array e inicializar cada elemento.
	T_LINEA_CACHE lineas[LINEAS];
	inicializarCache(lineas);
	
	//Puntero a RAM para volcarlo en RAM. Al finalizar el volcado se cierra el FILE*.
	unsigned char RAM[TAMRAM];
	FILE *pRAM = fopen("RAM.bin","rb");
	if(!pRAM)
		nofufa("No se encuentra 'RAM.bin'.\nRevisa si se encuentra en el mismo directorio.\n");
	fgets(RAM,TAMRAM,pRAM);
	fclose(pRAM);
	
	//Puntero a accesos_memoria.txt.
	FILE *accesos = fopen("accesos_memoria.txt","r");
	if(!accesos)
		nofufa("No se encuentra 'accesos_memoria.txt'.\nRevisa si se encuentra en el mismo directorio.\n");
	
	//Variables para guardar el dato, la etiqueta, la linea y la palabra. Optimizados para este ejercicio.
	DATOS_ACCESO dato_acceso;
	int dato;
//!1

//!2	Bucle principal.

	while(!feof(accesos)) {
		
		//Se lee una direccion del fichero.
		dato = fstrtx(accesos);
		
		//Se obtiene etq y linea con mascaras de bit y desplazamientos a nivel de bit.
		dato_acceso = conDatosAcceso(dato);
		
		tiempoglobal++;
		//Se comprueba la existencia del dato en cache.
		printf("\n");
		if(lineas[dato_acceso.lin].ETQ!=dato_acceso.etq) {
			//Se imprime:
			printf("\nT: %d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X bloque %02X", tiempoglobal, ++numfallos,dato,dato_acceso.etq,dato_acceso.lin,dato_acceso.pal,dato/BYTESLINEAS);
			
			tiempoglobal+=ACCRAM;
			
			printf("\nSe esta cargando el bloque %02X en la linea %02X",dato/BYTESLINEAS,dato_acceso.lin);

			//Copia los datos en la linea desde la RAM.
			memcpy(lineas[dato_acceso.lin].Datos, RAM+(dato&0b1111111000),BYTESLINEAS);
			lineas[dato_acceso.lin].ETQ=dato_acceso.etq;
		}
		
		//Se imprimen los datos del acceso y el contenido de la cache.
		printf("\nT: %d, Acierto de CACHE, ADDR %04X ETQ %X linea %02X palabra %02X DATO %02X", tiempoglobal,dato,dato_acceso.etq,dato_acceso.lin,dato_acceso.pal,lineas[dato_acceso.lin].Datos[dato_acceso.pal]);
		imprimirCache(lineas);
		
		//Se ainade al texto el caracter DATO.
		texto[tamTexto++] = lineas[dato_acceso.lin].Datos[dato_acceso.pal];
//		sleep(2);
	}
//!2

//!3	Fin del programa. Se imprimen datos finales y se cierra y libera todo.
	
	//Se completa la cadena con un final de cadena.
	texto[tamTexto]='\0';
	
	//Se cierran los FILE*. Si se llego hasta aca significa que accesos apunta a un fichero.
	fclose(accesos);
	
	//Se imprime el numero total de accesos, numero de fallos y tiempo medio de acceso.
	//TODO esto es el tiempo medio efectivo
	//Tiempo total/num accesos
	printf("\n\nAccesos: %i, numero de fallos: %i, tiempo medio de acceso: %.2f\nEl texto: ",tamTexto,numfallos,(float)tiempoglobal/tamTexto);
	
	//Se imprime el mensaje con el texto leido caracter a caracter desde la cache.
	printf(texto);
	
	free(zaborra);
	
	return 0;
//!3
}

void inicializarCache(T_LINEA_CACHE *lineas) {
	unsigned char i=LINEAS, a;
	
	while(i--) {
//		*(lineas+i) = (T_LINEA_CACHE){0xFF, a};
//		printf("ºpatoº%iºº",(lineas+i)->Datos[i]);
		(lineas+i)->ETQ = ETQINI;
		a=BYTESLINEAS;
		while(a--) {
			(lineas+i)->Datos[a]=DATINI;
		}
	}
}

char* fconstr(FILE *f) {

	//En cuanto sumamos el realloc
	unsigned char aumento=6;

	//Guarda la cadena que se devolvera.
	char* str;
	str = (char*)malloc(sizeof(char)*aumento);	
	if (!str) {
		nofufa("\nError al leer");
	}
	
	//Guarda el tamaino de la cadena.
	unsigned char tam=1;

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
	if(zaborra) {
		free(zaborra);
	}
	zaborra = fconstr(f);
	return (int)strtol(zaborra, NULL, 16);
	
	//Con fscanf en vez de fconstr:
//	char* linea;
//	fscanf(f, "%s", linea);
//	return (int)strtol(linea, NULL, 16);
}

DATOS_ACCESO conDatosAcceso(int acceso) {
	return (DATOS_ACCESO){(acceso>>5) & 0b11111, (acceso>>3) & 0b11, acceso & 0b111};;
	//En ese ultimo realmente no hacia falta el &b111, ya que en los datos todo apartir de ahi es 0, pero si cambian los datos esto cogeria bien la etiqueta aun el cambio.
}

 void imprimirCache(T_LINEA_CACHE *lineas) {
	//Iteradores de bucle.
	unsigned char a, i;
	
	for(a=0;LINEAS-a;a++) {
		printf("\nETQ:%02X  Datos",lineas[a].ETQ);
		i=BYTESLINEAS;
		while(i--)
			printf(" %02X", lineas[a].Datos[i]);
	}
 }
	
void nofufa(char* mensaje) {
	printf("\n");
	printf(mensaje);
	exit(-1);
}

/*1607865202*/
