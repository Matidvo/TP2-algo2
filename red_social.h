#ifndef RED_SOCIAL_H
#define RED_SOCIAL_H


typedef struct red_social red_social_t;

/*Crea una red social y carga los usuarios del archivo
  PRE: el archivo es valido*/
red_social_t* red_inicializar(FILE* usuarios);


/*Destruye la red social y libera la memoria pedida.
  PRE: la red fue inicializada*/
void red_destruir(red_social_t* red);

/*Permite al usuario loggearse para usar las demas funciones. Imprime error si ya habia un usuario
  loggeado o el usuario ingresado no existe.
  PRE: la red fue inicializada*/
void loggear(red_social_t* red);

/*Permite al usuario cerrar la sesion
  PRE: la red fue inicializada*/
void logout(red_social_t* red);

/*Publica un post en el feed del usuario y actualiza el de los demas. Si no habia usuario loggeado falla
  PRE: la red fue inicializada*/
void publicar(red_social_t* red);

/*Muestra al usuario el siguiente post en su feed, que sera el de mas afinidad.
  Si no habia usuario loggeado o no hay mas posts para ver falla.
  PRE: la red fue inicializada*/
void ver_siguiente_feed(red_social_t* red);

/*Likea un post del feed segun el id ingresado.
  Si el post es inexistente o no habi usuario loggeado falla
  PRE: la red fue inicializada*/
void likear_post(red_social_t* red);

/*Muestra la cantidad de likes de un post del feed, segun su id.
  Si el post es inexistente falla.
  PRE: la red fue inicializada*/
void mostrar_likes(red_social_t* red);

void actualiza_feed_usuarios(hash_t* usuarios, post_t* post);






#endif
