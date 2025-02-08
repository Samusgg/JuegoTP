#include <stdio.h>
#include <math.h>
#include "Pantalla.h"
#include <stdlib.h>

int dentroRectangulo(int x, int y, int h, int w, int px, int py){
   return (x<=px && px<x+w) && (y<=py && py<y+h);
}

double dPuntoPunto (double x2, double x1, double y2, double y1){
    double x = x2-x1;
    double y = y2-y1;
    double argumentoRaiz = pow(x,2)+pow(y,2);

    return sqrt (argumentoRaiz);
}

int colisionCirculos(int x2, int x1, int y1, int y2, int r1, int r2){
    int d = dPuntoPunto(x2,x1,y2,y1);
    int sumaRa = r1 + r2;
    if(d < sumaRa){
        return 1;
    }else{
        return 0;
    }
}

int solapeRectangulos( int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2 ){

}

//********************
/* Funcion Principal*/
//********************
int main (){
    printf("%f",dPuntoPunto(50,25,50,25));
    return 0;
}
