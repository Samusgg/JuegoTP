#include "Pantalla.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


/*
******************************************************************
******************************************************************
                        BANCO DE FUNCIONES
******************************************************************
******************************************************************
*/


/**
Modela un Ente Muerto
de mi videojuego.
*/
typedef struct {
    int x;
    int y;
    int wt; //width
    int ht; //height
    Imagen imagen;
} EnteMuerto;
typedef EnteMuerto Fondo;
typedef EnteMuerto Tesoro;

/**
Modela un Ente Vivo.

TODO: Separa los structs en varios.
Asi que sí, se más fiel a la hoja.
*/
typedef struct {
    int x;
    int y;
    int vidas;
    int wt; //width
    int ht; //height
    Imagen imagen;
    int v; //Velocidad
    int activo;
} EnteVivo;

typedef EnteVivo Heroe;
typedef EnteVivo EnemigoRep;
typedef EnteVivo Bala;


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

/**
Dibuja el fondo

*/
void dibujarFondo(EnteMuerto * fondo, EnteMuerto * tesoro, int puntos, int vidas) {
    char texto[4];
    sprintf(texto,"%d",vidas);

    rellena_fondo(255,255,255,255);
    //Dibuja fondo
    dibuja_imagen(fondo -> imagen,fondo -> x,fondo -> y,fondo -> wt,fondo -> ht);

    color_trazo(255,0,0,255);
    dibuja_texto(texto,399,0);

    //Dibuja Tesoro
    dibuja_imagen(tesoro -> imagen,tesoro -> x,tesoro -> y,tesoro -> wt,tesoro -> ht);
}

/**
Mueve al heroe.
*/
void moverHeroe(EnteVivo * heroe) {
    if(tecla_pulsada(SDL_SCANCODE_UP) && heroe->y>0) {
        heroe->y = heroe->y - heroe->v;
    }
    if(tecla_pulsada(SDL_SCANCODE_DOWN) && heroe->y+heroe->ht<479) {
        heroe->y = heroe->y + heroe->v;
    }
    if(tecla_pulsada(SDL_SCANCODE_LEFT) && heroe->x>0) {
        heroe->x = heroe->x - heroe->v;
    }
    if(tecla_pulsada(SDL_SCANCODE_RIGHT) && heroe->x+heroe->wt<799) {
        heroe->x = heroe->x + heroe->v;
    }
    dibuja_imagen(heroe -> imagen,heroe -> x,heroe -> y,heroe -> wt,heroe -> ht);

}

/**
Es una funcion dedicada a crear balas para el heroe.
*/
Bala crearBala (int x, int y, int vx, int vy){

}

void liberarBala(){

}

//*****************************************
//          Funciones enemigos.
//*****************************************
/**
Esta funcion mueve a los enemigos activos respecto al heroe.

*/
void moverEnemigos(EnteVivo * enemigo[], EnteVivo * heroe) {
    for(int i = 0; i<10; i++) {
        if(enemigo[i]->activo == 1) {
            if(heroe->x/2>enemigo[i]->x/2) {
                enemigo[i]->x = enemigo[i]->x + enemigo[i]->v;
            } else {
                enemigo[i]->x = enemigo[i]->x - enemigo[i]->v;
            };
            if(heroe->y/2>enemigo[i]->y/2) {
                enemigo[i]->y = enemigo[i]->y + enemigo[i]->v;
            } else {
                enemigo[i]->y = enemigo[i]->y - enemigo[i]->v;
            };
            dibuja_imagen(enemigo[i] -> imagen,enemigo[i] -> x,enemigo[i] -> y,enemigo[i] -> wt,enemigo[i] -> ht);
        }
    }
}

/**
Comprueba la colision de los enemigos activos.
*/
int colisionEnemigos(EnteVivo * enemigo[], EnteVivo * heroe) {
    for(int i = 0; i<10; i++) {
        //Colision con enemigo.
        if(enemigo[i]->activo == 1 && solapeRectangulos(heroe->x,heroe->y,heroe->wt,heroe->ht,enemigo[i]->x,enemigo[i]->y,enemigo[i]->wt,enemigo[i]->ht)) {
            heroe->vidas --;
            if(heroe->vidas <= 0) {
                return 1;
            } else {
                heroe->x = rand()%751;
                heroe->y = rand()%431;
                return 0;
            }
        }
    }
}
/*
******************************************************************
******************************************************************
******************************************************************
******************************************************************
******************************************************************
******************************************************************
******************************************************************
******************************************************************
******************************************************************
******************************************************************
******************************************************************
******************************************************************
******************************************************************
******************************************************************
******************************************************************
******************************************************************
*/
//  CODIGO PRINCIPAL
int main(int argc, char *argv[]) {
    srand(time(NULL));//Establecemos semilla aleatoria.

    crea_pantalla("Ejemplo 1",800,480);
    Fondo fondo = {.x = 0, .y = 0, .wt = 800, .ht = 480, .imagen = lee_imagen("./imagenes/map.bmp",0)};
    Tesoro tesoro = {.x = 749, .y = 429, .wt = 50, .ht = 50, .imagen = lee_imagen("./imagenes/esmerald.bmp",1)};
    Heroe heroe = {.x = 0, .y = 0, .vidas = 3, .wt = 50, .ht = 50, .imagen =  lee_imagen("./imagenes/steve.bmp",0),.v = 5, .activo = 1};

    //TODO: Refactorizar enemigos para inicializar en alguna funcion.
    Imagen imagenEnemigo = lee_imagen("./imagenes/criper.bmp",0);
    EnemigoRep ene1 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 1};
    EnemigoRep ene2 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 1};
    EnemigoRep ene3 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 1};
    EnemigoRep ene4 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 0};
    EnemigoRep ene5 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 0};
    EnemigoRep ene6 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 0};
    EnemigoRep ene7 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 0};
    EnemigoRep ene8 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 0};
    EnemigoRep ene9 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 0};
    EnemigoRep ene10 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 0};

    EnemigoRep * enemigos [10] = {&ene1,&ene2,&ene3,&ene4,&ene5,&ene6,&ene7,&ene8,&ene9,&ene10};
    //**

    int puntuacion = 0, fin = 0, iteracion = 0;
    while(pantalla_activa() && !fin) {

        dibujarFondo(&fondo,&tesoro,0,heroe.vidas);
        moverHeroe(&heroe);
        moverEnemigos(enemigos, &heroe);


        //Colision con tesoro.
        if(solapeRectangulos(heroe.x,heroe.y,heroe.wt,heroe.ht,tesoro.x,tesoro.y,tesoro.wt,tesoro.ht)) {
            puntuacion ++;
            tesoro.x = rand()%751;
            tesoro.y = rand()%431;
        }


        fin = colisionEnemigos(enemigos,&heroe);
        actualiza_pantalla();
        espera(40);


        //Add nuevos enemigos.
        if(iteracion >= 25) {
            int i = 0;
            while(i<9 && enemigos[i]->activo != 0) {
                i++;
            }
            enemigos[i]->activo = 1;
            iteracion = 0;
        } else {
            iteracion++;
        }

    }//Termina While = 1

    libera_imagen(heroe.imagen);
    libera_imagen(tesoro.imagen);
    libera_imagen(imagenEnemigo);
    libera_imagen(fondo.imagen);
    libera_pantalla();

    return 0;
}
