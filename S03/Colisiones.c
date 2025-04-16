#include "Colisiones.h"
#include <stdlib.h>
#include <stdio.h>

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
int dentroRectangulo(int x, int y, int h, int w, int px, int py){
   return (x<=px && px<x+w) && (y<=py && py<y+h);
}

/**
    \brief Calcula la distancia entre dos puntos.
    \param x2 Coordenada horizontal de la esquina superior izquierda.
    \param x1 Coordenada horizontal de la esquina superior izquierda.
    \param y2 Coordenada vertical de la esquina superior izquierda.
    \param y1 Coordenada vertical de la esquina superior izquierda.
    \return Distancia entre dos puntos.
*/
double dPuntoPunto (double x2, double x1, double y2, double y1){
    double x = x2-x1;
    double y = y2-y1;
    double argumentoRaiz = pow(x,2)+pow(y,2);

    return sqrt (argumentoRaiz);
}


/**
    \brief Comprueba si dos circulos colisionan entre si.
    \param x2 Coordenada horizontal de la esquina superior izquierda.
    \param x1 Coordenada horizontal de la esquina superior izquierda.
    \param y1 Coordenada vertical de la esquina superior izquierda.
    \param y2 Coordenada vertical de la esquina superior izquierda.
    \return 1 Si hay solape, 0 en caso contrario.
*/
int colisionCirculos(int x2, int x1, int y1, int y2, int r1, int r2){
    int d = dPuntoPunto(x2,x1,y2,y1);
    int sumaRa = r1 + r2;
    if(d < sumaRa){
        return 1;
    }else{
        return 0;
    }
}


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
int solape_rectangulos( int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2 ) {
    if(((x1<=x2 && x1+w1>=x2)||(x1<=x2+w2 && x1+w1 >= x2+w2))&&
            ((y1<=y2 && y1+h1>=y2)||(y1<=y2+h2 && y1+h1 >= y2+h2))) {
        return 1;
    } else {
        return 0;
    }
}

