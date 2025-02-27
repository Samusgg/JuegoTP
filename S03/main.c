#include "Pantalla.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>




/**
    FONDO
*/
typedef struct {
    int x;
    int y;
    int wt; //width
    int ht; //height
    Imagen imagen;
} Fondo;

/**
    TESORO
*/
typedef struct {
    int x;
    int y;
    int wt; //width
    int ht; //height
    Imagen imagen;
} Tesoro;


/**
    HEROE
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
} Heroe;

/**
    ENEMIGO
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
} EnemigoRep;

/**
    BALA
*/
typedef struct {
    int x;
    int y;
    int wt; //width
    int ht; //height
    Imagen imagen;
    int v; //Velocidad
    int activo;
} BalaRep;
typedef BalaRep * Bala;

/*
******************************************************************
******************************************************************
                        BANCO DE FUNCIONES
******************************************************************
******************************************************************
*/

/**
Comprueba si dos rectangulos colisionan entre si.
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
Dibuja el fondo

*/
void dibuja_fondo(Fondo * fondo, Tesoro * tesoro, int puntos, int vidas) {
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


//*****************************************
//          Funciones heroe.
//*****************************************


/**
Mueve al heroe.
*/
void mover_heroe(Heroe * heroe) {
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

Bala crea_bala (int x, int y, int vx, int vy){
     return malloc(sizeof(BalaRep));
}

void libera_bala(Bala b){
   free(b->imagen);
   free(b);
}

void mueve_bala(Bala b){

}

void dibuja_bala(Bala b){
}

int get_x_bala(Bala b){

}

int get_y_bala(Bala b){
}


//*****************************************
//          Funciones enemigos.
//*****************************************

/**
Esta funcion mueve a los enemigos activos respecto al heroe.

*/
void mover_enemigos(EnemigoRep * enemigo[], int n, Heroe * heroe ) {
    for(int i = 0; i<n; i++) {
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
Funcion que sirve para ver la colision del enemigo con objeto.

*/
int colision_enemigos_objeto (EnemigoRep * enemigo [], int n, int x, int y, int w, int h) {
    for(int i = 0; i<n; i++) {
        if(solape_rectangulos(x,y,w,h,enemigo[i]->x,enemigo[i]->y,enemigo[i]->wt,enemigo[i]->ht)) {
            return 1;
        }
    }
    return 0;
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
    Bala = NULL;

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

        dibuja_fondo(&fondo,&tesoro,0,heroe.vidas);
        mover_heroe(&heroe);
        mover_enemigos(enemigos, 10, &heroe);


        //Colision con tesoro.
        if(solape_rectangulos(heroe.x,heroe.y,heroe.wt,heroe.ht,tesoro.x,tesoro.y,tesoro.wt,tesoro.ht)) {
            puntuacion ++;
            tesoro.x = rand()%751;
            tesoro.y = rand()%431;
        }


        //Se encarga de controlar la vida del heroe en funcion de las colisiones.
        if(colision_enemigos_objeto(enemigos,10,heroe.x,heroe.y,heroe.wt,heroe.ht)) {
            heroe.vidas --;
            if(heroe.vidas>0) {
                heroe.x = rand()%751;
                heroe.y = rand()%431;
            } else {
                fin = 1;
            }
        }
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
