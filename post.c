#include "post.h"
#include "abb.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define NO_ENCONTRADO -1
#define CAPACIDAD_INICIAL 30

struct post{
  	char* texto;
  	char* autor;
  	size_t likes;
  	abb_t* usuarios_likes;
  	size_t id;
};

struct post_feed{
  	size_t afinidad;
  	post_t* post;
};


void stringcpy(const char* original, char** copia) {
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

//POST 
post_t* post_crear(char* linea, char* autor, size_t id){
	post_t* post = malloc(sizeof(post_t));
	stringcpy(autor, &post->autor);
	stringcpy(linea, &post->texto);
	post->id = id;
	post->likes = 0;
	post->usuarios_likes = abb_crear(strcmp, free);
	if(!post->usuarios_likes) return NULL;
	return post;
}

void post_destruir(post_t* post){
	abb_destruir(post->usuarios_likes);
	free(post->autor);
	free(post->texto);
	free(post);
}

size_t post_cantidad_likes(post_t* post){
	return post->likes;
}

void post_guardar_like_de_usuario(post_t* post, char* nombre, void* dato){
	if(abb_pertenece(post->usuarios_likes, nombre)) return;

	post->likes++;	
	abb_guardar(post->usuarios_likes, nombre, dato);
}

void post_mostrar_usuarios_likes(post_t* post){
	abb_iter_t* iter = abb_iter_in_crear(post->usuarios_likes);
	while(!abb_iter_in_al_final(iter)){
		fprintf(stdout,"	%s\n", abb_iter_in_ver_actual(iter));
		abb_iter_in_avanzar(iter);
	}
	abb_iter_in_destruir(iter);
}

size_t post_id(post_t* post){
	return post->id;
}


//POST FEED
post_feed_t* post_feed_crear(post_t* post, size_t afinidad){
	post_feed_t* post_feed = malloc(sizeof(post_feed_t));
	if(!post_feed)return NULL;
	post_feed->post = post;
	post_feed->afinidad = afinidad;
	return post_feed;
}

size_t post_feed_obtener_afinidad(const post_feed_t* post_feed){
	return post_feed->afinidad;
}

size_t post_feed_obtener_id(const post_feed_t* post_feed){
	return post_feed->post->id;
}
void post_feed_mostrar(post_feed_t* post_feed){
	fprintf(stdout, "Post ID %ld\n", post_feed->post->id);
	fprintf(stdout, "%s dijo: %s\n", post_feed->post->autor, post_feed->post->texto);
	fprintf(stdout, "Likes: %ld\n", post_feed->post->likes);
}

void post_feed_destruir(post_feed_t* post){
	free(post);
}
