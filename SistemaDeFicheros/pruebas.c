#include <stdio.h>
#include <string.h>


int main(){

int i;

for(i=0;i<3 && i< 10;i++) {
printf("º%iº",i);
}
if(i==3) {
	printf("es el mismo");
}

return 0;
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
            printf(" [%i]\n",ContarBloques(inodos->blq_inodos[(directorio++)->dir_inodo].i_nbloque,1,NULL));
		}
	}
}
