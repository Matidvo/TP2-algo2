#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "tp2.h"

#define LOGGEAR "login\n"
#define LOGOUT "logout\n"
#define PUBLICAR "publicar\n"
#define LIKE "likear_post\n"
#define MOSTRAR_LIKES "mostrar_likes\n"
#define VER_SIGUIENTE "ver_siguiente_feed\n"

void pedir_comando(red_social_t* algogram, char* linea){
	if(!linea){
    		fprintf(stdout,"no se pudo leer la entrada estandar\n");
    		return;
  	}
	if(strcmp(linea, LOGGEAR) == 0){
		loggear(algogram);

	}else if(strcmp(linea, LOGOUT) == 0){
		logout(algogram);

	}else if(strcmp(linea, PUBLICAR) == 0){
		publicar(algogram);

	}else if(strcmp(linea, LIKE) == 0){
		likear_post(algogram);

	}else if(strcmp(linea, MOSTRAR_LIKES) == 0){
		mostrar_likes(algogram);

	}else if(strcmp(linea, VER_SIGUIENTE) == 0){
		ver_siguiente_feed(algogram);
	}

}

int main(int argc, char* argv[]){
	if(argc < 2){
    		printf("%d\n",argc);
    		fprintf(stderr,"%s\n","parametros invalidos");
  	}

	//Inicializar valores de usuarios y la red
  	char *nombre_archivo = argv[1];
  	FILE* archivo_usuarios = fopen(nombre_archivo,"r");
  	if(!archivo_usuarios){
    		fprintf(stdout,"%s\n","no se pudo abrir el archivo");
    		return -1;
  	}
  	red_social_t* algogram = red_inicializar(archivo_usuarios);
  	fclose(archivo_usuarios);

	//Programa principal
  	size_t capacidad = 0;
  	char *linea = NULL;
  	ssize_t leidos = getline(&linea,&capacidad,stdin);
  	while(leidos != EOF){
		pedir_comando(algogram, linea);
		leidos = getline(&linea,&capacidad,stdin);
  	}

  	free(linea);
  	red_destruir(algogram);
	return 0;
}
