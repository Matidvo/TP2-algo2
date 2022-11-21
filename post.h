#ifndef _POST_H
#define _POST_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct post post_t;
typedef struct post_feed post_feed_t;

void stringcpy(const char* original, char** copia);

//POST 

/*
Post: Crea un struct post vacio
*/
post_t* post_crear(char* texto, char* autor, size_t id);

/*
Pre: El struct post fue creado
Post:Aumenta la cantidad de likes del post y guarda el usuario
	que dio el like
*/
void post_guardar_like_de_usuario(post_t* post, char* nombre, void* dato);

/*
Pre: El struct post fue creado
Post:Devuelve la cantidad de likes
*/
size_t post_cantidad_likes(post_t* post);

/*
Pre: El struct post fue creado
Post:Muestra los usuarios almacenados en los likes del post
*/
void post_mostrar_usuarios_likes(post_t* post);

/*
Pre: El struct post fue creado
Post:Devuelve el id del post
*/
size_t post_id(post_t* post);

/*
Pre: El struct post fue creado
Post:Destruye el struct post_t
*/
void post_destruir(post_t* post);


//POST FEED

/*
Pre: El struct post fue creado
Post: Devuelve un struct de post_feed_t vacio
*/
post_feed_t* post_feed_crear(post_t* post, size_t afinidad);

/*
Pre: El struct post_feed fue creado
Post: Devuelve la afinidad del post
*/
size_t post_feed_obtener_afinidad(const post_feed_t* post_feed);

/*
Pre: El struct post_feed fue creado
Post:Devuelve el id del post_feed
*/
size_t post_feed_obtener_id(const post_feed_t* post_feed);

/*
Pre: El struct post_feed fue creado
Post:Imprime el estado del post_feed
*/
void post_feed_mostrar(post_feed_t* post_feed);

/*
Pre: El struct post_feed fue creado
Post:Destruye el post_feed
*/
void post_feed_destruir(post_feed_t* post_feed);

#endif  // _POST_H
