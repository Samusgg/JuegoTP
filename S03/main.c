#include "Pantalla.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

/**
Estructura de datos que modela un ente
de mi videojuego.
*/
typedef struct {
    int x;
    int y;
    int vidas;
    int wt; //width
    int ht; //height
    Imagen imagen;
} Ente;

/**
Metodo para dibujar entes.
*/
void dibujarEnte (Ente * ente) {
    dibuja_imagen(ente -> imagen,ente -> x,ente -> y,ente -> wt,ente -> ht);//Ente
    actualiza_pantalla();
}


/**
Comprueba si dos rectangulos colisionan entre si.
*/
int solapeRectangulos( int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2 ) {
    if(((x1<=x2 && x1+w1>=x2)||(x1<=x2+w2 && x1+w1 >= x2+w2))&&
            ((y1<=y2 && y1+h1>=y2)||(y1<=y2+h2 && y1+h1 >= y2+h2))) {
        return 1;
    } else {
        return 0;
    }
}

void dibujarFondo(Ente * fondo, Ente * tesoro, int puntos) {
    char texto[4];
    sprintf(texto,"%d",puntos);

    rellena_fondo(255,255,255,255);
    dibujarEnte(fondo);
    color_trazo(255,0,0,255);
    dibuja_texto(texto,399,0);

    dibujarEnte(tesoro);
}


int main(int argc, char *argv[]) {
    srand(time(NULL));//Establecemos semilla aleatoria.

    crea_pantalla("Ejemplo 1",800,480);
    Ente heroe = {.x = 0, .y = 0, .vidas = 3, .wt = 50, .ht = 50, .imagen =  lee_imagen("./imagenes/steve.bmp",0)};
    Ente fondo = {.x = 0, .y = 0, .vidas = -1, .wt = 800, .ht = 480, .imagen = lee_imagen("./imagenes/map.bmp",0) };
    Ente tesoro = {.x = 749, .y = 429, .vidas = -1, .wt = 50, .ht = 50, .imagen = lee_imagen("./imagenes/esmerald.bmp",1) };
    Ente enemigo = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = lee_imagen("./imagenes/criper.bmp",0)};

    int puntuacion = 0, fin = 0;
    while(pantalla_activa() && !fin) {

        if(tecla_pulsada(SDL_SCANCODE_UP) && heroe.y>0) {
            heroe.y = heroe.y-5;
        }
        if(tecla_pulsada(SDL_SCANCODE_DOWN) && heroe.y+heroe.ht<479) {
            heroe.y = heroe.y+5;
        }
        if(tecla_pulsada(SDL_SCANCODE_LEFT) && heroe.x>0) {
            heroe.x = heroe.x-5;
        }
        if(tecla_pulsada(SDL_SCANCODE_RIGHT) && heroe.x+heroe.wt<799) {
            heroe.x = heroe.x+5;
        }
        dibujarFondo(&fondo,&tesoro,puntuacion);
        dibujarEnte(&heroe);//Heroe

        //Se encarga de modificar el movimiento del enemigo.
        if(heroe.x/2>enemigo.x/2) {
            enemigo.x = enemigo.x + 2;
        } else {
            enemigo.x = enemigo.x - 2;
        };
        if(heroe.y/2>enemigo.y/2) {
            enemigo.y = enemigo.y + 2;
        } else {
            enemigo.y = enemigo.y - 2;
        };
        dibujarEnte(&enemigo);//Enemigo


        //Colision con tesoro.
        if(solapeRectangulos(heroe.x,heroe.y,heroe.wt,heroe.ht,tesoro.x,tesoro.y,tesoro.wt,tesoro.ht)) {
            puntuacion ++;
            tesoro.x = rand()%751;
            tesoro.y = rand()%431;
        }
        //Colision con enemigo.
        if(solapeRectangulos(heroe.x,heroe.y,heroe.wt,heroe.ht,enemigo.x,enemigo.y,enemigo.wt,enemigo.ht)) {
            heroe.vidas --;
            if(heroe.vidas <= 0) {
                fin = 1;
            } else {
                heroe.x = rand()%751;
                heroe.y = rand()%431;
            }
        }

        espera(40);
    }
    libera_imagen(heroe.imagen);
    libera_imagen(tesoro.imagen);
    libera_imagen(enemigo.imagen);
    libera_pantalla();

    return 0;
}
