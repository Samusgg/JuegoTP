#ifndef __Bala_H__
#define __Bala_H__
#include "Colisiones.h"
#include "Pantalla.h"

typedef struct BalaRep * Bala;

/**
    \brief Es una funcion dedicada a crear balas para el heroe.
    \param x Coordenada horizontal de la esquina superior izquierda de la bala.
    \param y Coordenada vertical de la esquina superior izquierda de la bala.
    \param vx Velocidad de la bala en el eje x.
    \param vy Velocidad de la bala en el eje y.
    \return Puntero de la nueva bala.
*/
Bala crea_bala (int x, int y, int vx, int vy);

/**
    \brief Libera de memoria la bala.
    \param b Puntero a BalaRep

*/
void libera_bala(Bala b);

/**
    \brief Mueve la bala modificando su posici�n.
    \param b Puntero a BalaRep

*/
void mueve_bala(Bala b);

/**
    \brief Dibuja la bala.
    \param b Puntero a BalaRep
    \param angulo Angulo de inclinaci�n bala en sentido horario.

*/
void dibuja_bala(Bala b);

/**
    \brief Obtiene posicion en x de la bala.
    \param b Puntero a BalaRep.
    \return N�mero entero de la posici�n en x de la bala.
*/
int get_x_bala(Bala b);

/**
    \brief Devuelve la anchura de la bala.
    \param b Puntero a BalaRep.
    \return Anchura de la bala.
*/
int get_wt_bala(Bala b);

/**
    \brief Devuelve la altura de la bala.
    \param b Puntero a BalaRep.
    \return Altura de la bala.
*/
int get_ht_bala(Bala b);

/**
    \brief Cambia la direcci�n de la bala.
    \param b Puntero a BalaRep.
*/
void set_dir_bala(Bala b, int i);

/**
    \brief Devuelve la direcci�n de la bala.
    \param b Puntero a BalaRep.
    \return Direcci�n de la bala.
*/
int get_dir_bala(Bala b);

/**
    \brief Obtiene posicion en y de la bala.
    \param b Puntero a BalaRep.
    \return N�mero entero de la posici�n en y de la bala.
*/
int get_y_bala(Bala b);


/**
    \brief Obtiene posicion en x de la bala.
    \param  bala Puntero a estructura BalaRep.
    \param x Coordenada horizontal de la esquina superior izquierda de la bala.
    \param y Coordenada vertical de la esquina superior izquierda de la bala.
    \param w Anchura de la bala.
    \param h Altura de la bala.
    \return 1 si hay colisi�n, 0 en caso contrario.
*/
int colision_bala (Bala bala, int x, int y, int w, int h);
#endif
