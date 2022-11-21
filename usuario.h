#ifndef _USUARIO_H
#define _USUARIO_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct usuario usuario_t;
typedef int (*cmp_t)(const void *a, const void *b);

/*
Pre:El cmp sera la prioridad usada para mostrar el feed del usuario
Post:Fue creado el usuario
*/
usuario_t* usuario_crear(char* nombre, size_t id, cmp_t cmp);

/*
Pre: El struct usuario fue creado
Post:Devuelve el id del usuario
*/
size_t usuario_obtener_id(usuario_t* usuario);

/*
Pre: El struct usuario fue creado
Post:Guarda el dato en el feed del usuario
*/
bool usuario_guardar_en_feed(usuario_t* usuario, void* dato);

/*
Pre:El struct usuario fue creado
Post:Devuelve el dato ingresado en el feed
*/
void* usuario_obtener_post_feed(usuario_t* usuario);

/*
Pre:El struct usuario fue creado
Post:Devuelve el nombre del usuario
*/
char* usuario_obtener_nombre(usuario_t* usuario);

/*
Pre:El struct usuario fue creado
Post:Destruye el struct
*/
void usuario_destruir(usuario_t* usuario);

#endif  // _USUARIO_H














#endif  // _USUARIO_H
