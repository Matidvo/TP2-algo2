#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "hash.c"
#include "heap.h"
#include "usuario.h"

struct red_social{
  	hash_t* usuarios;
  	usuario_loggeado_t* usuario_loggeado;
  	post_t** posts;
  	size_t cantidad_posts;
  	size_t cantidad_usuarios;
  	size_t capacidad_post;
};

void actualiza_feed_usuarios(hash_t* usuarios, post_t* post){
	hash_iter_t* iter = iter_usuario_crear(usuarios);
	usuario_loggeado_t* usuario_actual;
	while(!hash_iter_al_final(iter)){
		const char* clave = hash_iter_ver_actual(iter);
		usuario_actual = obtener_usuario()
		if(strcmp(usuario_actual->nombre, post->autor) != 0){
			post_feed_t* post_feed = crear_post_feed(post, usuario_actual->id);
			heap_encolar(usuario_actual->feed, post_feed);
		}
		hash_iter_avanzar(iter);
	}

	hash_iter_destruir(iter);
}

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
	red->usuarios = crear_usuarios();
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
	destruir_usuarios(red->usuarios);
	for(int i = 0; i<red->cantidad_posts; i++){
		destruir_post(red->posts[red->cantidad_posts - 1]);
	}
	free(red->posts);
	free(red);
}

void loggear(red_social_t* red){
	if(red->usuario_loggeado){
		fprintf(stderr, "Error: Ya habia un usuario loggeado.\n");
		return;
	}

	char* linea = cargar_linea();
	if(!linea){return;}
	char* clave = NULL; 
	strncpy(&clave, linea, strlen(linea));

	if(!usuario_pertenece(red->usuarios, clave)){
		fprintf(stdout, "%s", "Error: usuario no existente\n");
		return;
	}

	usuario_loggeado_t* usuario_aux = obtener_usuario(red->usuarios, clave);
	red->usuario_loggeado = usuario_aux;
	fprintf(stdout,"Hola %s\n", red->usuario_loggeado->nombre);

	free(clave);
	free(linea);

}

void logout(red_social_t* red){
	if(!red->usuario_loggeado){
		fprintf(stderr, "Error: no habia usuario loggeado\n");
		return;
	}

	fprintf(stdout,"Adios\n");
	red->usuario_loggeado = NULL;

}

void publicar(red_social_t* red){
	if(!red->usuario_loggeado){
		fprintf(stderr, "Error: no habia usuario loggeado\n");
	}

	char* linea = cargar_linea();
	if(!linea) return;
	if(red->cantidad_posts ==  red->capacidad_post){
		if(!redimensionar_post(red, red->capacidad_post * 2)){
			free(linea);
			return;
		}
	}
	post_t* post = crear_post(linea, red->usuario_loggeado->nombre, red->cantidad_posts);
	if(!post){
		free(linea);
		return;
	}
	red->posts[red->cantidad_posts] = post;
	red->cantidad_posts++;
	//fprintf(stdout, "%s", "Post publicado\n");
	actualiza_feed_usuarios(red->usuarios, post);
	fprintf(stdout, "%s\n", "Post publicado");
}

void ver_siguiente_feed(red_social_t* red){
 	if(!red->usuario_loggeado){
    		fprintf(stdout,"%s\n","Usuario no loggeado o no hay mas posts para ver");
    		return;
  	}
  	post_feed_t* post_prox = heap_desencolar(red->usuario_loggeado->feed);
	if(!post_prox){
		fprintf(stdout,"%s\n","Usuario no loggeado o no hay mas posts para ver");
		return;
	}

	fprintf(stdout, "Post ID %ld\n", post_prox->post->id);
	fprintf(stdout, "%s dijo: %s\n", post_prox->post->autor, post_prox->post->texto);
	fprintf(stdout, "Likes: %ld\n", post_prox->post->likes);
	free(post_prox);
}


void likear_post(red_social_t* red){
	char* linea =cargar_linea();
	if(!linea)return;
	red->usuario_loggeado->id = (size_t)atoi(linea);

	int indice = encontrar_post_id(red->posts, red->cantidad_posts, red->usuario_loggeado->id);
	if(indice == NO_ENCONTRADO || !red->usuario_loggeado){
		fprintf(stderr, "Error: Post inexistente o sin likes.\n");
		free(linea);
		return;
	}
	if(!abb_pertenece(red->posts[indice]->usuarios_likes, red->usuario_loggeado->nombre)){
		abb_guardar(red->posts[indice]->usuarios_likes, red->usuario_loggeado->nombre, red->usuario_loggeado);
		red->posts[indice]->likes++;
	}
	fprintf(stderr, "Post likeado\n");

}


void mostrar_likes(red_social_t* red){
	char* linea = cargar_linea();
	red->usuario_loggeado->id = (size_t)atoi(linea);

	int indice = encontrar_post_id(red->posts, red->cantidad_posts, red->usuario_loggeado->id);
	if(indice == NO_ENCONTRADO){
		printf("Error: Post inexistente o sin likes.\n");
		free(linea);
		return;
	}
	if(red->posts[indice]->likes == 0){
		printf("Error: Post inexistente o sin likes.\n");
		free(linea);
		return;
	}

	printf("El post tiene %ld likes:", red->posts[indice]->likes);
	abb_iter_t* iter = abb_iter_in_crear(red->posts[indice]->usuarios_likes);
	while(abb_iter_in_al_final(iter)){
		printf("	%s", abb_iter_in_ver_actual(iter));
		abb_iter_in_avanzar(iter);
	}
	abb_iter_in_destruir(iter);
}
