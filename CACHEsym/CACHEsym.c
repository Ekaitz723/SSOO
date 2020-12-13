#include "actividadEvaluableCadenas.h"


int main(int argc, char *argv[]) {

//Preconfiguración antes del bucle:

	//Guardar opcion del bucle. Tambien usado como iterador.
	char sel=3;
	
	
	//Lectura de argumentos:
	
	//Se introdujo como argumento '?'.
	if (strchr(argv[1],63)) {
		printf("\nDebes especificar numeros enteros no nulos como el alto del carton (primer elemento) y el ancho (segundo elemento).\n");
		return 0;
	}
	
	//No se introdujo suficientes argumentos.
	if (argc < 3) {
		printf("\nFaltan argumentos.\nPara mas info prueba con '?'\n");
		return 1;
	}
	
	//No se introdujo un numero natural (excluyendo 0) en alguno de los argumentos.
	while(sel-->1) 
		if (atoi(argv[sel])<1) {
			printf("\nIntroduciste un valor incorrecto en el %i argumento\nPara mas info prueba con '?'\n",sel);
			return 2;
		}


	//Se crea puntero de usuario_t y la variable tamUsuarios que guarda el tamaino de *usuarios.
	usuario_t  *usuarios = NULL;
	unsigned int tamUsuarios = 0;
	usuarios = (usuario_t*)malloc(sizeof(usuario_t)*tamUsuarios);


//Bucle:
	while (usuarios) {
		
		//Imprime las opciones y lee opcion.
		printf("\n\n[1] Introducir jugador\n[2] Mostrar Cartones\n[3] Salir\n\n|: ");
		
		//Lee opcion del 1 al 3 ambos incluidos.
		//Solo se queda con el primer caracter valido. Esto es, si se introduce 4q\n03, lo leera como un 3 e ignorara el resto.
		do {
			sel = getc(stdin);
		} while (sel<49 || sel>51);
		
		//Limpia buffer.
		fflush(stdin);
		
		//Opciones.
		if ((sel-=49)==introducir) {
			
			//Realloc a usuarios con un elemento extra.
			usuarios = (usuario_t*)realloc(usuarios,sizeof(usuario_t)*(++tamUsuarios));
			
			//Creacion de un elemento carton en usuario.
			(usuarios+tamUsuarios-1)->carton_usuario = (carton_t*)malloc(sizeof(carton_t)*1);
			
			//Ancho y alto.
			((usuarios+tamUsuarios-1)->carton_usuario)->alto = atoi(argv[1]);
			((usuarios+tamUsuarios-1)->carton_usuario)->ancho = atoi(argv[2]);
			
			//Rellena datos usuario.
			rellenarUsuario((usuarios+tamUsuarios-1));
		}
		else if (sel==mostrar) {
			sel = tamUsuarios;
			while (sel--) 
				leerUsuario((usuarios+sel));
		}
		else if (sel==salir) {
			
	//Free memoria dinamica.
			
			while(tamUsuarios--) {
				
		//Free usuario.
			//Free nombre usuario.
				free(usuarios->nombre_usuario);
				
			//Free carton usuario.
				//Free numeros carton usuario.
				sel = atoi(argv[1]);
				while(sel--) {
					free(usuarios->carton_usuario->numeros[sel]);
				}
				free(usuarios->carton_usuario->numeros);
				
				free(usuarios->carton_usuario);
			}
			free(usuarios);
			
			usuarios = NULL;			
		}
	}
	
	return 0;
}

/*1607865202*/
