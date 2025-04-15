#ifndef _Ejercito_h_
#define _Ejercito_h_
#include "Rafaga.h"

typedef struct EjercitoTDA * Ejercito;

/**
    \brief Devuelve un nuevo ej�rcito vac�o, es decir, sin ning�n enemigo, pero
    con capacidad para almacenar hasta N enemigos. Decide y fija t� el valor de N.
    \return Ejercito de enemigos.
    **/
Ejercito crea_ejercito(Escenario fondo);

/**
    \brief Libera toda la memoria asociada al ej�rcito e.
    \param e Ejercito de enemigos **/
void libera_ejercito( Ejercito e );

/**
    \brief Crea un nuevo enemigo en las coordenadas en (x,y), anchura w y altura
    h, y lo a�ade al grupo de enemigos que forma el ej�rcito e, siempre que a�n quede
    espacio libre.
    \param e Ejercito de enemigos
    **/
void inserta_enemigo (Ejercito e, Escenario fondo);

/** \brief Mueve todos los enemigos contenidos en el ej�rcito e.
    \param e Ejercito de enemigos.
    \param xR Eje X de referencia.
    \param yR Eje y de referencia.
**/
void mueve_ejercito(Ejercito e,int xR, int yR, Escenario fondo);

/**
    \brief Muestra todos los enemigos contenidos en el ej�rcito e.
    \param e Ejercito de enemigos.
**/
void dibuja_ejercito( Ejercito e );

/**
    \brief Devuelve 1 si el rect�ngulo con esquina superior izquierda en (x,y),
    anchura w y altura h se solapa con el de alguno de los enemigos que contenga el
    ej�rcito e.
    \param e Ejercito de enemigos.
    \param x Coordenada vertical, esquina superior izquierda.
    \param y Coordenada horizontal, esquina superior izquierda.
    \param w Anchura del rectangulo que representa al objeto.
    \param h Altura del rectangulo que representa al objeto.
    \return 1 Si se solapan, 0 si no.
**/
int colision_ejercito_objeto( Ejercito e, int x, int y, int w, int h );

/**
    \brief Elimina todos los enemigos incluidos en el ejecito e que
    colisionen con alguna de las balas de la r�faga r y devolver el n�mero de enemigos
    eliminados.
    \param e Ejercito de enemigos.
    \param r Rafaga de balas.
**/
int colision_ejercito_rafaga( Ejercito e, Rafaga r );

/**
    \brief Libera toda la memoria asociada al ejercito.
    \param e Ejercito de enemigos.
*/
void libera_ejercito( Ejercito e);

/**
    \brief Cambia de direcci�n un enemigo de forma aleatoria.
    \param e Ejercito de enemigos.
*/
void mod_aleatoria(Ejercito e);
#endif // _Ejercito_h_
