#ifndef __Colisiones_H__
#define __Colisiones_H__
#include "Escenario.h"

/**
    \brief Comprueba si un punto está dentro de un rectángulo.
    \param x Coordenada horizontal de la esquina superior izquierda del rectángulo.
    \param y1 Coordenada vertical de la esquina superior izquierda del rectángulo.
    \param w Anchura del rectangulo.
    \param h Altura del rectangulo.
    \param px Coordenada x del punto.
    \param px Coordenada y del punto.
    \return 1 Si hay está dentro, 0 en caso contrario.
*/
int dentroRectangulo(int x, int y, int h, int w, int px, int py);

/**
    \brief Calcula la distancia entre dos puntos.
    \param x2 Coordenada horizontal de la esquina superior izquierda.
    \param x1 Coordenada horizontal de la esquina superior izquierda.
    \param y2 Coordenada vertical de la esquina superior izquierda.
    \param y1 Coordenada vertical de la esquina superior izquierda.
    \return Distancia entre dos puntos.
*/
double dPuntoPunto (double x2, double x1, double y2, double y1);

/**
    \brief Comprueba si dos circulos colisionan entre si.
    \param x2 Coordenada horizontal de la esquina superior izquierda.
    \param x1 Coordenada horizontal de la esquina superior izquierda.
    \param y1 Coordenada vertical de la esquina superior izquierda.
    \param y2 Coordenada vertical de la esquina superior izquierda.
    \return 1 Si hay solape, 0 en caso contrario.
*/
int colisionCirculos(int x2, int x1, int y1, int y2, int r1, int r2);

/**
    \brief Comprueba si dos rectangulos colisionan entre si.
    \param x1 Coordenada horizontal de la esquina superior izquierda del primer rectángulo.
    \param y1 Coordenada vertical de la esquina superior izquierda del primer rectángulo.
    \param w1 Anchura del primer rectangulo a dibujar.
    \param h1 Altura del primer rectangulo a dibujar.
    \param x2 Coordenada horizontal de la esquina superior izquierda del segundo rectángulo.
    \param y2 Coordenada vertical de la esquina superior izquierda del segundo rectángulo.
    \param w2 Anchura del segundo rectangulo a dibujar.
    \param h2 Altura del segundo rectangulo a dibujar.
    \return 1 Si hay solape, 0 en caso contrario.
*/
int solape_rectangulos( int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2 );

/**
    \brief Función que comprueba la colisión de un lateral de un bloque
    con respecto a un tipo de bloque de un escenario.
    \param e Escenario.
    \param x Coordenada horizontal del primer Punto.
    \param y Coordenada vertical del primer Punto.
    \param x2 Coordenada horizontal del segundo Punto.
    \param y2 Coordenada vertical del segundo Punto.
    \param tipoB Tipo de bloque del escenario.
    \return 1 si ha habido colisión, 0 si no.
*/
int colH_lateral(Escenario e, int x, int y, int x2, int y2,int tipoB);

/**
    \brief Función que comprueba si un rectángulo choca en alguno de sus vertices con
    un bloque del tipo indicado.
    \param e Escenario.
    \param x Coordenada horizontal del rectángulo.
    \param y Coordenada vertical del rectángulo.
    \param wt Anchura del rectángulo.
    \param ht Altura del rectángulo.
    \param tipoB Tipo de bloque del escenario.
*/
int dentro_bloque(Escenario e, int x, int y, int wt, int ht, int tipoB);
#endif
