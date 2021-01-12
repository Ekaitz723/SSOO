#include "cabeceras.h"

char* string(FILE *f) {

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
}

void nofufa(char* mensaje) {
	printf("\n");
	printf(mensaje);
	exit(-1);
}
