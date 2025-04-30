#ifndef __Rafaga_h__
#define __Rafaga_h__
#include "Bala.h"
#include "Colisiones.h"
#include "Escenario.h"

/**
    \file Rafaga.h
    \brief TDA que modela una lista de balas.
*/

typedef struct TDARafaga * Rafaga;

/**
    \brief Devuelve una nueva r�faga vac�a, es decir,
    sin ninguna bala, pero lista para almacenar cualquier n�mero de balas.
    \param alcance Alcance total en pixeles del las balas de la r�faga.
    \return Rafaga Lista de balas vac�a.
*/
Rafaga crea_rafaga(int alcance);

/**
    \brief Libera toda la memoria asociada a la r�faga r
    y las balas que contenga.
    \param r TDA Rafaga de balas
*/
void libera_rafaga(Rafaga r);

/**
    \brief A�ade a la r�faga r la bala b.
    \param r Lista de balas.
    \param b Bala a insertar en r.
*/
void inserta_rafaga(Rafaga r, Bala b);

/**
    \brief Mueve todas las balas contenidas en la r�faga r
    y elimina todas las balas que, tras moverse, quedan situadas fuera de la pantalla
    o chocan con un obst�culo
    \param r Lista de balas.
    \param e Escenario por el que se van a mover las balas.
*/
void mueve_rafaga(Rafaga r, Escenario e);

/**
    \brief Muestra todas las balas contenidas en la r�faga r.
    \param r Lista de balas.
*/
void dibuja_rafaga(Rafaga r);

/**
    \brief Devuelve el n�mero de balas que incluye la r�faga r.
    \param r Lista de balas.
    \return int Longitud rafaga.
*/
int longitud_rafaga(Rafaga r);

/**
    \brief Busca y elimina la primera bala incluida en la r�faga r que se solape
     con el rect�ngulo con esquina superior izquierda en (x,y), anchura w y altura h. La
     funci�n devolver� 1 si elimin� alguna bala y 0 en caso contrario.
    \param r Lista de balas.
    \param x Coordenada x esquina superior izquierda del rect�ngulo.
    \param y Coordenada y esquina superior izquierda del rect�ngulo.
    \param w Anchura del rect�ngulo.
    \param h Altura del rect�ngulo.
    \return 1 Si ha habido colisi�n, 0 si no.
*/
int colision_rafaga(Rafaga r, int x, int y, int w, int h);


#endif // __Rafaga_h__
