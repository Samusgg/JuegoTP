#include "Colisiones.h"
#include <stdlib.h>
#include <stdio.h>
#include "Escenario.h"

/**
    \brief Comprueba si un punto est� dentro de un rect�ngulo.
    \param x Coordenada horizontal de la esquina superior izquierda del rect�ngulo.
    \param y1 Coordenada vertical de la esquina superior izquierda del rect�ngulo.
    \param w Anchura del rectangulo.
    \param h Altura del rectangulo.
    \param px Coordenada x del punto.
    \param px Coordenada y del punto.
    \return 1 Si hay est� dentro, 0 en caso contrario.
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
    \param x1 Coordenada horizontal de la esquina superior izquierda del primer rect�ngulo.
    \param y1 Coordenada vertical de la esquina superior izquierda del primer rect�ngulo.
    \param w1 Anchura del primer rectangulo a dibujar.
    \param h1 Altura del primer rectangulo a dibujar.
    \param x2 Coordenada horizontal de la esquina superior izquierda del segundo rect�ngulo.
    \param y2 Coordenada vertical de la esquina superior izquierda del segundo rect�ngulo.
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


/**
    \brief Funci�n que comprueba la colisi�n de un lateral de un bloque
    con respecto a un tipo de bloque de un escenario.
    \param e Escenario.
    \param x Coordenada horizontal del primer Punto.
    \param y Coordenada vertical del primer Punto.
    \param x2 Coordenada horizontal del segundo Punto.
    \param y2 Coordenada vertical del segundo Punto.
    \param tipoB Tipo de bloque del escenario.
    \return 1 si ha habido colisi�n, 0 si no.
*/
int colH_lateral(Escenario e, int x, int y, int x2, int y2,int tipoB) {
     if(tipo_bloque(e,x,y)== tipoB || tipo_bloque(e,x2,y2)== tipoB)
        return 1;
    return 0;
}


/**
    \brief Funci�n que comprueba si un rect�ngulo choca en alguno de sus vertices con
    un bloque del tipo indicado.
    \param e Escenario.
    \param x Coordenada horizontal del rect�ngulo.
    \param y Coordenada vertical del rect�ngulo.
    \param wt Anchura del rect�ngulo.
    \param ht Altura del rect�ngulo.
    \param tipoB Tipo de bloque del escenario.
*/
int dentro_bloque(Escenario e, int x, int y, int wt, int ht, int tipoB) {
     if(colH_lateral(e,x,y,x+wt,y,tipoB) ||
        colH_lateral(e,x+wt,y,x+wt,y+wt,tipoB) ||
        colH_lateral(e,x+wt,y+wt,x,y+wt,tipoB) ||
        colH_lateral(e,x,y+wt,x,y,tipoB)
        ){
        return 1;
    }
    return 0;
}


