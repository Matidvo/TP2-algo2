#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tp2.h"
#include "post.h"
#include "hash.h"
#include "usuario.h"

//CONSTANTES
#define CAPACIDAD_INICIAL 30
#define NO_ENCONTRADO -1

struct red_social{
  	hash_t* usuarios;
  	usuario_t* usuario_loggeado;
  	post_t** posts;
  	size_t cantidad_posts;
  	size_t cantidad_usuarios;
  	size_t capacidad_post;
};

/*
**														**					
**					FUNCIONES AUXILIARES					**
**														*/

//GUARDAR LINEA DE STDIN
char* cargar_linea(){
	char* linea_aux = NULL;
	size_t tope = 0;
	if(getline(&linea_aux, &tope, stdin) < 0){
		free(linea_aux);
		return NULL;
	}

	return linea_aux;
}

int _heap_cmp(const post_feed_t* usuario1, const post_feed_t* usuario2){
	if(post_feed_obtener_afinidad(usuario1) > post_feed_obtener_afinidad(usuario2)){return -1;}
	if(post_feed_obtener_afinidad(usuario1)< post_feed_obtener_afinidad(usuario2)){return 1;}
	if(post_feed_obtener_afinidad(usuario1) == post_feed_obtener_afinidad(usuario2)){
		if(post_feed_obtener_id(usuario1) > post_feed_obtener_id(usuario2)){return -1;}
		if(post_feed_obtener_id(usuario1) < post_feed_obtener_id(usuario2)){return 1;}
	}
	return 0;
}

int heap_cmp(const void* usuario1, const void* usuario2){
  return _heap_cmp((post_feed_t*)usuario1,(post_feed_t*)usuario2);
}

void destruir_usuario_wrapper(void* usuario){
	usuario_destruir(usuario);
}


void cargar_usuarios(FILE* archivo, hash_t* usuarios, size_t* cantidad_de_usuarios){
	char* linea = NULL;
	size_t tope = 0;
	while(getline(&linea, &tope, archivo) > 0){
		usuario_t* usuario = usuario_crear(linea, *cantidad_de_usuarios, heap_cmp);
		hash_guardar(usuarios, usuario_obtener_nombre(usuario), usuario);
		(*cantidad_de_usuarios)++;
	}

	free(linea);
}

int encontrar_post_id(post_t** posts, size_t cantidad, size_t id){
	for(int i = 0; i<cantidad; i++){
		if(post_id(posts[i]) == id){
			return i;
		}
	}
	return NO_ENCONTRADO;
}


bool redimensionar_post(red_social_t* red, size_t nueva_capacidad){
	post_t** post_nuevo = realloc(red->posts, nueva_capacidad * sizeof(post_t*)); 
    	if(post_nuevo == NULL){
        	return false;
   	}
    	red->posts = post_nuevo;
    	red->capacidad_post = nueva_capacidad;
    	return true;
}

void actualiza_feed_usuarios(hash_t* usuarios, post_t* post, char* autor, size_t id){
	hash_iter_t* iter = hash_iter_crear(usuarios);
	usuario_t* usuario_actual;
	while(!hash_iter_al_final(iter)){
		const char* clave = hash_iter_ver_actual(iter);
		usuario_actual = hash_obtener(usuarios, clave);
		if(strcmp(usuario_obtener_nombre(usuario_actual), autor) != 0){
      		size_t afinidad = (size_t)abs((int)usuario_obtener_id(usuario_actual) - (int)id);
			post_feed_t* post_feed = post_feed_crear(post, afinidad);
			usuario_guardar_en_feed(usuario_actual, post_feed);
		}
		hash_iter_avanzar(iter);
	}

	hash_iter_destruir(iter);
}


/*
**														**					
**					PRIMITIVAS DEL PROGRAMA					**
**														*/


red_social_t* red_inicializar(FILE* usuarios){
	red_social_t* red = malloc(sizeof(red_social_t));
	if(!red) return NULL;
	red->posts = malloc(CAPACIDAD_INICIAL*sizeof(post_t*));
	if(!red->posts){
		free(red);
		return NULL;
	}
	red->cantidad_posts = 0;
	red->capacidad_post = CAPACIDAD_INICIAL;
	red->cantidad_usuarios = 0;
	red->usuarios = hash_crear(destruir_usuario_wrapper);
	if(!red->usuarios){
		free(red->posts);
		free(red);
		return NULL;
	}
	cargar_usuarios(usuarios, red->usuarios, &red->cantidad_usuarios);
	red->usuario_loggeado = NULL;
	
	return red;
}


void red_destruir(red_social_t* red){
	hash_destruir(red->usuarios);
	if(red->posts){
		for(size_t i=0;i < red->cantidad_posts;i++){
			post_destruir(red->posts[i]);
		}
		free(red->posts);
	}
	free(red);
}


void loggear(red_social_t* red){
	if(red->usuario_loggeado){
		fprintf(stdout, "Error: Ya habia un usuario loggeado\n");
		return;
	}

	char* linea = cargar_linea();
	if(!linea){return;}
	char* clave = NULL; 
	stringcpy(linea, &clave);

	if(!hash_pertenece(red->usuarios, clave)){
		free(clave);
		free(linea);
		fprintf(stdout, "Error: usuario no existente\n");
		return;
	}

	usuario_t* usuario_aux = hash_obtener(red->usuarios, clave);
	red->usuario_loggeado = usuario_aux;
	fprintf(stdout,"Hola %s\n", usuario_obtener_nombre(red->usuario_loggeado));
	free(clave);
	free(linea);

}

void logout(red_social_t* red){
	if(!red->usuario_loggeado){
		fprintf(stdout, "Error: no habia usuario loggeado\n");
		return;
	}

	fprintf(stdout,"Adios\n");
	red->usuario_loggeado = NULL;

}

void publicar(red_social_t* red){
	if(!red->usuario_loggeado){
		fprintf(stdout,"%s\n","Error: no habia usuario loggeado");
		return;
	}

	char* linea = cargar_linea();
	if(red->cantidad_posts ==  red->capacidad_post){
		if(!redimensionar_post(red, (red->capacidad_post * 2))){
			free(linea);
			return;
		}
	}

	char* autor = usuario_obtener_nombre(red->usuario_loggeado);
	post_t* post = post_crear(linea, autor, red->cantidad_posts);
	if(!post){
		free(linea);
		return;
	}
	red->posts[red->cantidad_posts] = post;
	red->cantidad_posts++;
	actualiza_feed_usuarios(red->usuarios, post, autor, usuario_obtener_id(red->usuario_loggeado));
	fprintf(stdout, "Post publicado\n");
	free(linea);
}


void ver_siguiente_feed(red_social_t* red){
 	if(!red->usuario_loggeado){
    		fprintf(stdout, "Usuario no loggeado o no hay mas posts para ver\n");
    		return;
  	}
  	post_feed_t* post_feed = usuario_obtener_post_feed(red->usuario_loggeado);
	if(!post_feed){
		fprintf(stdout, "Usuario no loggeado o no hay mas posts para ver\n");
		return;
	}

	post_feed_mostrar(post_feed);
	free(post_feed);
}

void likear_post(red_social_t* red){
	char* linea =cargar_linea();
	if(!linea)return;
	if(!red->usuario_loggeado){
		fprintf(stdout, "Error: Usuario no loggeado o Post inexistente\n");
		free(linea);
		return;
	}

	size_t id = (size_t)atoi(linea);
	int indice = encontrar_post_id(red->posts, red->cantidad_posts, id);
	if(indice == NO_ENCONTRADO){
		fprintf(stdout, "Error: Usuario no loggeado o Post inexistente\n");
		free(linea);
		return;
	}
	post_guardar_like_de_usuario(red->posts[indice], usuario_obtener_nombre(red->usuario_loggeado), NULL);
	fprintf(stdout, "Post likeado\n");
	free(linea);

}


void mostrar_likes(red_social_t* red){
	char* linea = cargar_linea();
	size_t id = (size_t)atoi(linea);
	int indice = encontrar_post_id(red->posts, red->cantidad_posts, id);
	if(indice == NO_ENCONTRADO ){
		fprintf(stdout, "Error: Post inexistente o sin likes\n");
		free(linea);
		return;
	}

	if(post_cantidad_likes(red->posts[indice]) == 0){
		fprintf(stdout, "Error: Post inexistente o sin likes\n");
		free(linea);
		return;
	}
	fprintf(stdout, "El post tiene %ld likes:\n", post_cantidad_likes(red->posts[indice]));
	post_mostrar_usuarios_likes(red->posts[indice]);
	free(linea);
}
