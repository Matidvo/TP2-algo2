#include "usuario.h"
#include "heap.h"
#include <string.h>

struct usuario{
	char* nombre;
  	size_t id;
  	heap_t* feed;
};

void strincpy(const char* original, char** copia) {
    size_t tam = strlen(original);
    *copia = malloc(tam + 1);

    int i;
    for(i = 0; original[i] != '\0'; i++){
      if(original[i] == '\n'){
        (*copia)[i] = '\0';
      }else{
        (*copia)[i] = original[i];
      }
    }
    (*copia)[i] = '\0';
}

usuario_t* usuario_crear(char* nombre, size_t id, cmp_t cmp){
	usuario_t* usuario = malloc(sizeof(usuario_t));
	if(!usuario) return NULL;
	usuario->feed = heap_crear(cmp);
	if(!usuario->feed){
		free(usuario);
		return NULL;
	}
	usuario->id = id;
	strincpy(nombre, &usuario->nombre);
	return usuario;
}

void usuario_destruir(usuario_t* usuario){
	heap_destruir(usuario->feed, free);
	free(usuario->nombre);
	free(usuario);
}

size_t usuario_obtener_id(usuario_t* usuario){
	return usuario->id;
}

char* usuario_obtener_nombre(usuario_t* usuario){
	return usuario->nombre;
}

bool usuario_guardar_en_feed(usuario_t* usuario, void* dato){
	if(!heap_encolar(usuario->feed, dato)){
		return false;
	}
	return true;
}

void* usuario_obtener_post_feed(usuario_t* usuario){
	return heap_desencolar(usuario->feed);
}
