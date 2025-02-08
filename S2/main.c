#include <stdio.h>
#include <math.h>
#include <time.h>
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
    if(((x1<=x2 && y1<=y2)&&(x1+w1>=x2 && y1+h1>=y2))||
       ((x1<=x2+w2 && y1<=y2)&&(x1+w1>=x2+w2 && y1+h1>=y2))||
       ((x1<=x2 && y1<=y2+h2)&&(x1+w1>=x2 && y1+h1>=y2+h2))||
       ((x1<=x2+w2 && y1<=y2+h2)&&(x1+w1>=x2+w2 && y1+h1>=y2+h2))){
        return 1;
    }else{
    return 0;
    }
}

double calcPi (){
    double x,y;
    int nTotal,nExitos;

    //Inicializa la semilla.
     srand(time(NULL));

     while(nExitos!=1000000){
       x = ((double) rand() / RAND_MAX);
       y = ((double) rand() / RAND_MAX);
       if(pow(x,2)+pow(y,2)<=1){
        nExitos++;
       }
        nTotal++;
     }

    double pi = 4*((double)nExitos/nTotal);
    return pi;

}

//********************
/* Funcion Principal*/
//********************
int main (){
  /*
    printf("Si %d\n",solapeRectangulos(0,0,4,2,3,1,4,3));
    printf("Si %d\n",solapeRectangulos(3,1,4,3,0,0,4,2));
    printf("No %d\n",solapeRectangulos(0,0,4,2,0,3,4,2));
*/
    printf("%f",calcPi());
    return 0;
}


