#include "Bala.h"
#include <stdlib.h>
#include <stdio.h>

/**
    BALA
*/
struct BalaRep{
    int x;  //Eje x
    int y;  //Eje Y
    int xRef; //Coordenada x inicial (Cuando se creo la bala)
    int yRef; //Coordenada y inicial (Cuando se creo la bala)
    int wt; //Anchura
    int ht; //Altura
    Imagen imagen;
    int vx; //Velocidad
    int vy; //Velocidad
    int dir; //Dirección
};


//*****************************************
//          Funciones BALA.
//*****************************************


/**
    \brief Es una funcion dedicada a crear balas para el heroe.
    \param x Coordenada horizontal de la esquina superior izquierda de la bala.
    \param y Coordenada vertical de la esquina superior izquierda de la bala.
    \param vx Velocidad de la bala en el eje x.
    \param vy Velocidad de la bala en el eje y.
    \return Puntero de la nueva bala.
*/
Bala crea_bala (int x, int y, int vx, int vy) {
    Bala miBala = malloc(sizeof(struct BalaRep));
    miBala->x = x;
    miBala->y = y;
    miBala->xRef = x;
    miBala->yRef = y;
    miBala->wt = 32;
    miBala->ht = 32;
    miBala->imagen = lee_imagen("./imagenes/arrow.bmp",1);
    miBala->vx = vx;
    miBala->vy = vy;
    return miBala;
}

/**
    \brief Libera de memoria la bala.
    \param b Puntero a BalaRep

*/
void libera_bala(Bala b) {
    free(b->imagen);
    free(b);
}

/**
    \brief Mueve la bala modificando su posición.
    \param b Puntero a BalaRep

*/
void mueve_bala(Bala b) {
    switch(b->dir) {
    case 0: {
        b->y = b->y - b->vy;
        break;
    }
    case 1: {
        b->x = b->x - b->vx;
        break;
    }
    case 2: {
        b->x = b->x + b->vx;
        break;
    }
    case 3: {
        b->y = b->y + b->vy;
        break;
    }
    }

}

/**
    \brief Dibuja la bala.
    \param b Puntero a BalaRep

*/
void dibuja_bala(Bala b) {
    //Dibuja Bala
    switch(b->dir) {
    case 0: {
        dibuja_imagen_transformada(b->imagen,b->x,b->y,b->wt,b->ht,-45.0,SDL_FLIP_NONE);
        break;
    }
    case 1: {
        dibuja_imagen_transformada(b->imagen,b->x,b->y,b->wt,b->ht,-135.0,SDL_FLIP_NONE);
        break;
    }
    case 2: {
        dibuja_imagen_transformada(b->imagen,b->x,b->y,b->wt,b->ht,-315.0,SDL_FLIP_NONE);
        break;
    }
    case 3: {
        dibuja_imagen_transformada(b->imagen,b->x,b->y,b->wt,b->ht,-225.0,SDL_FLIP_NONE);
        break;
    }
    }
}

/**
    \brief Obtiene posicion en x de la bala.
    \param b Puntero a BalaRep.
    \return Número entero de la posición en x de la bala.
*/
int get_x_bala(Bala b) {
    return b->x;
}

/**
    \brief Obtiene posicion "x" inicial de la bala..
    \param b Puntero a BalaRep.
    \return Número entero de la posición "x" inicial la bala.
*/
int get_xRef_bala(Bala b) {
    return b->xRef;
}

/**
    \brief Obtiene posicion en y de la bala.
    \param b Puntero a BalaRep.
    \return Número entero de la posición en y de la bala.
*/
int get_y_bala(Bala b) {
    return b->y;
}

/**
    \brief Obtiene posicion "y" inicial de la bala.
    \param b Puntero a BalaRep.
    \return Número entero de la posición "y" inicial la bala.
*/
int get_yRef_bala(Bala b) {
    return b->yRef;
}

/**
    \brief Devuelve la anchura de la bala.
    \param b Puntero a BalaRep.
    \return Anchura de la bala.
*/
int get_wt_bala(Bala b){
return b->wt;
}


/**
    \brief Devuelve la altura de la bala.
    \param b Puntero a BalaRep.
    \return Altura de la bala.
*/
int get_ht_bala(Bala b){
return b->ht;
}


/**
    \brief Devuelve la dirección de la bala.
    \param b Puntero a BalaRep.
    \return Dirección de la bala.
*/
int get_dir_bala(Bala b){
    return b->dir;
}


/**
    \brief Cambia la dirección de la bala.
    \param b Puntero a BalaRep.
    \param i Nueva dirección de la bala.
*/
void set_dir_bala(Bala b, int i){
    b->dir = i;
}

/**
    \brief Obtiene posicion en x de la bala.
    \param  bala Puntero a estructura BalaRep.
    \param x Coordenada horizontal de la esquina superior izquierda de la bala.
    \param y Coordenada vertical de la esquina superior izquierda de la bala.
    \param w Anchura de la bala.
    \param h Altura de la bala.
    \return 1 si hay colisión, 0 en caso contrario.
*/
int colision_bala (Bala bala, int x, int y, int w, int h) {
    if(solape_rectangulos(x,y,w,h,bala->x,bala->y,bala->wt,bala->ht)) {
        return 1;
    }
    return 0;
}
