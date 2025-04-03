#include "Pantalla.h"
#include "Colisiones.h"
#include "Bala.h"
#include "Rafaga.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


/**
    FONDO
*/
struct FondoStr {
    int x;  //Eje x
    int y;  //Eje Y
    int wt; //Anchura
    int ht; //Altura
    Imagen imagen;
};
typedef struct FondoStr * Fondo;

/**
    TESORO
*/
struct TesoroStr {
    int x;  //Eje x
    int y;  //Eje Y
    int wt; //Anchura
    int ht; //Altura
    Imagen imagen;
};
typedef struct TesoroStr * Tesoro;


/**
    HEROE
*/
struct HeroeStr {
    int x;  //Eje x
    int y;  //Eje Y
    int wt; //Anchura
    int ht; //Altura
    int vidas;
    Imagen imagen;
    int v; //Velocidad
    int puntos;
    int activo;
};
typedef struct HeroeStr * Heroe;

/**
    ENEMIGO
*/
typedef struct {
    int x;  //Eje x
    int y;  //Eje Y
    int wt; //Anchura
    int ht; //Altura
    int vidas;
    Imagen imagen;
    int v; //Velocidad
    int activo; //0 si "no hay enemigo", 1 si lo hay.
    int dir; //Dirección
} EnemigoRep;


typedef FILE * Archivo;
/*
******************************************************************
******************************************************************
                        BANCO DE FUNCIONES
******************************************************************
******************************************************************
*/


/**
    \brief Dibuja el fondo.
    \param fondo Puntero a la estructura FondoStr.
*/
void dibuja_fondo(Fondo fondo) {
    //Dibuja fondo
    dibuja_imagen(fondo -> imagen,fondo -> x,fondo -> y,fondo -> wt,fondo -> ht);
}

/**
    \brief Dibuja informacion sobre los puntos y las vidas.
    \param puntos Puntuacion.
    \param vidas Vidas del heroe restantes.
*/
void dibuja_info(int puntos, int vidas, int nBalas) {
    char textoV[36];
    char textoP[40];
    char textoB[36];
    snprintf(textoV, 36, "Vidas: %d", vidas); //Transforma el entero a character y concatena.
    snprintf(textoP, 40, "Puntos: %d", puntos);
    snprintf(textoB, 36, "Balas: %d", nBalas);


    color_trazo(255,255,255,255);
    dibuja_texto(textoV,0,0);
    dibuja_texto(textoP,100,0);
    dibuja_texto(textoB,200,0);
}

/**
    \brief Dibuja el tesoro.
    \param tesoro Puntero a estructura TesoroStr.
*/
void dibuja_tesoro(Tesoro tesoro) {
    //Dibuja tesoro
    dibuja_imagen(tesoro -> imagen,tesoro -> x,tesoro -> y,tesoro -> wt,tesoro -> ht);
}


//*****************************************
//          Funciones HEROE
//*****************************************


/**
    \brief Mueve al heroe.
    \param heroe Puntero a estructura HeroeStr.
*/
void mover_heroe(Heroe heroe) {
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


//*****************************************
//          Funciones ENEMIGOS
//*****************************************

/**
    \brief Esta funcion mueve a los enemigos activos respecto al heroe.
           El enemigo se mueve siempre siguiendo al héroe.
           Su atributo "dir", gestiona si se mueve en el eje x (En caso de ser 0)
           O en el eje y (En caso de ser 1).
    \param enemigo Array de punteros a enemigos.
    \param n Longitud del array de enemigos.
    \param heroe Puntero a estructura de HeroeStr.
*/
void mover_enemigos(EnemigoRep * enemigo[], int n, Heroe heroe ) {
    for(int i = 0; i<n; i++) {
        if(enemigo[i]->activo == 1) {
            switch(enemigo[i]->dir) {
            case 0: {
                if(heroe->x>enemigo[i]->x) {
                    enemigo[i]->x = enemigo[i]->x + enemigo[i]->v;
                } else {
                    enemigo[i]->x = enemigo[i]->x - enemigo[i]->v;
                };
                break;
            }

            case 1: {
                if(heroe->y>enemigo[i]->y) {
                    enemigo[i]->y = enemigo[i]->y + enemigo[i]->v;
                } else {
                    enemigo[i]->y = enemigo[i]->y - enemigo[i]->v;
                };
                break;
            }
            }
            dibuja_imagen(enemigo[i] -> imagen,enemigo[i] -> x,enemigo[i] -> y,enemigo[i] -> wt,enemigo[i] -> ht);
        }

    }

}


/**
    \brief Funcion que sirve para ver si hay colision entre los enemigos con un objeto.
    \param n Longitud del array de enemigos.
    \param x Coordenada horizontal de la esquina superior izquierda del objeto.
    \param y Coordenada vertical de la esquina superior izquierda del objeto.
    \param w Anchura del objeto.
    \param h Altura del objeto.
    \return 1 si existe colision, 0 si no.
*/
int colision_enemigos_objeto (EnemigoRep * enemigo [], int n, int x, int y, int w, int h) {
    for(int i = 0; i<n; i++) {
        if(enemigo[i]->activo==1 && solape_rectangulos(x,y,w,h,enemigo[i]->x,enemigo[i]->y,enemigo[i]->wt,enemigo[i]->ht)) {
            return 1;
        }
    }
    return 0;
}


/**
    \brief Detecta la colisión entre los enemigos y la bala.
    \param enemigo Array de punteros a enemigos.
    \param n Longitud del array de enemigos.
    \param bala Puntero a estructura de BalaRep.
    \return 1 si existe colision, 0 en caso contrario.

*/
int colision_enemigos_lista_balas (EnemigoRep * enemigo [], int n, Rafaga listaBalas) {
    for(int i = 0; i<n; i++) {
        if(enemigo[i]->activo == 1 && colision_rafaga(listaBalas,enemigo[i]->x, enemigo[i]->y, enemigo[i]->wt, enemigo[i]->ht)) {
            enemigo[i]->activo = 0;
            enemigo[i]->x = rand()%751;
            enemigo[i]->y = rand()%431;
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


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// TODO: INFORMAR AL USUARIO DE LOS RECORDS
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int main(int argc, char *argv[]) {
    srand(time(NULL));//Establecemos semilla aleatoria.

    crea_pantalla("Ejemplo 1",800,480);
    struct FondoStr fondoStr = {.x = 0, .y = 0, .wt = 800, .ht = 480, .imagen = lee_imagen("./imagenes/map.bmp",0)};
    struct TesoroStr tesoroStr = {.x = 749, .y = 429, .wt = 50, .ht = 50, .imagen = lee_imagen("./imagenes/esmerald.bmp",1)};
    struct HeroeStr heroeStr = {.x = 0, .y = 0, .vidas = 3, .wt = 50, .ht = 50, .imagen =  lee_imagen("./imagenes/steve.bmp",0),.v = 5, .puntos = 0,.activo = 1};
    Fondo fondo = &fondoStr;
    Tesoro tesoro = &tesoroStr;
    Heroe heroe = &heroeStr;

    Rafaga listaBalas = crea_rafaga();
    Bala bAux = NULL;
    Archivo archi = fopen("./puntos.txt", "r+");

    //ENEMIGOS
    Imagen imagenEnemigo = lee_imagen("./imagenes/criper.bmp",0);
    EnemigoRep ene1 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 1,.dir = rand()%2};
    EnemigoRep ene2 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 1,.dir = rand()%2};
    EnemigoRep ene3 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 1,.dir = rand()%2};
    EnemigoRep ene4 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 0,.dir = rand()%2};
    EnemigoRep ene5 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 0,.dir = rand()%2};
    EnemigoRep ene6 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 0,.dir = rand()%2};
    EnemigoRep ene7 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 0,.dir = rand()%2};
    EnemigoRep ene8 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 0,.dir = rand()%2};
    EnemigoRep ene9 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 0,.dir = rand()%2};
    EnemigoRep ene10 = {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imagenEnemigo, .v = 1, .activo = 0,.dir = rand()%2};
    int nEnemigos = 10;
    EnemigoRep * enemigos [10] = {&ene1,&ene2,&ene3,&ene4,&ene5,&ene6,&ene7,&ene8,&ene9,&ene10};
    //**

    int fin = 0, iteracion = 0;
    while(pantalla_activa() && !fin) {

        dibuja_fondo(fondo);
        dibuja_info(heroe->puntos,heroe->vidas,longitud_rafaga(listaBalas));
        //Dibuja Tesoro
        dibuja_tesoro(tesoro);


        //PROGRAMAS LISTA DE BALAS.
        if(tecla_pulsada(SDL_SCANCODE_W)) {
            bAux = crea_bala(heroe->x,heroe->y,7,7);
            set_dir_bala(bAux,0);
            inserta_rafaga(listaBalas,bAux);
        }
        if(tecla_pulsada(SDL_SCANCODE_A)) {
            bAux = crea_bala(heroe->x,heroe->y,7,7);
            set_dir_bala(bAux,1);
            inserta_rafaga(listaBalas,bAux);
        }
        if(tecla_pulsada(SDL_SCANCODE_D)) {
            bAux = crea_bala(heroe->x,heroe->y,7,7);
            set_dir_bala(bAux,2);
            inserta_rafaga(listaBalas,bAux);
        }
        if(tecla_pulsada(SDL_SCANCODE_S)) {
            bAux = crea_bala(heroe->x,heroe->y,7,7);
            set_dir_bala(bAux,3);
            inserta_rafaga(listaBalas,bAux);
        }

        mueve_rafaga(listaBalas);
        dibuja_rafaga(listaBalas);


        mover_heroe(heroe);
        mover_enemigos(enemigos, nEnemigos, heroe);


        //Colision con tesoro.
        if(solape_rectangulos(heroe->x,heroe->y,heroe->wt,heroe->ht,tesoro->x,tesoro->y,tesoro->wt,tesoro->ht)) {
            heroe->puntos ++;
            tesoro->x = rand()%751;
            tesoro->y = rand()%431;
        }


        if(colision_enemigos_lista_balas(enemigos, nEnemigos,listaBalas)) {
            heroe->puntos++;
        }

        /*
        //Colision enemigo bala
        if(bala!=NULL && colision_enemigos_bala(enemigos, nEnemigos,bala)) {
            libera_bala(bala);
            heroe->puntos++;
            bala = NULL;
        }
        */


        //Se encarga de controlar la vida del heroe en funcion de las colisiones.
        if(colision_enemigos_objeto(enemigos,10,heroe->x,heroe->y,heroe->wt,heroe->ht)) {
            heroe->vidas --;
            if(heroe->vidas>0) {
                heroe->x = rand()%751;
                heroe->y = rand()%431;
            } else {
                fin = 1;
            }
        }


        actualiza_pantalla();
        espera(40);


        //Add nuevos enemigos.
        if(iteracion >= 25) {
            int i = 0;
            while(i<nEnemigos-1 && enemigos[i]->activo != 0) {
                i++;
            }
            enemigos[i]->activo = 1;

            //Modifica direccion aleatoriamente.
            int toggle = rand()%10;
            switch(enemigos[toggle]->dir) {
            case 0:
                enemigos[toggle]->dir = 1;
                break;
            case 1:
                enemigos[toggle]->dir = 0;
                break;
            }


            iteracion = 0;
        } else {
            iteracion++;
        }

    }//Termina While = 1


    //*************************************
    //*************************************
    // ZONA DE RECORD
    //*************************************


    //WHILE PARA DIBUJAR EL FONDO DEL RECORD.
    int record = 0;
    fscanf(archi, "%d", &record);
    if(record < heroe->puntos) {
        rewind(archi);
        fprintf(archi, "%d", heroe->puntos);
    }
    libera_imagen(fondo->imagen);
    fondo->imagen =  lee_imagen("./imagenes/floor.bmp",0);
    dibuja_fondo(fondo);
    actualiza_pantalla();
    while(pantalla_activa()) {}


    //Final del programa.
    libera_lista_balas(listaBalas);
    fclose(archi);
    libera_imagen(heroe->imagen);
    libera_imagen(tesoro->imagen);
    libera_imagen(imagenEnemigo);
    libera_imagen(fondo->imagen);
    libera_pantalla();

    return 0;
}//--
