#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "Pantalla.h"
#include "Colisiones.h"
#include "Bala.h"
#include "Rafaga.h"
#include "Ejercito.h"

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
    Ejercito enemigos = crea_ejercito();

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
        mueve_ejercito(enemigos,heroe->x,heroe->y);
        dibuja_ejercito(enemigos);

        //Colision con tesoro.
        if(solape_rectangulos(heroe->x,heroe->y,heroe->wt,heroe->ht,tesoro->x,tesoro->y,tesoro->wt,tesoro->ht)) {
            heroe->puntos ++;
            tesoro->x = rand()%751;
            tesoro->y = rand()%431;
        }


        if(colision_ejercito_rafaga(enemigos,listaBalas)) {
            heroe->puntos++;
        }

        //Se encarga de controlar la vida del heroe en funcion de las colisiones.
        if(colision_ejercito_objeto(enemigos,heroe->x,heroe->y,heroe->wt,heroe->ht)) {
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
            inserta_enemigo(enemigos);
            mod_aleatoria(enemigos);
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
    libera_rafaga(listaBalas);
    fclose(archi);
    libera_imagen(heroe->imagen);
    libera_imagen(tesoro->imagen);
    libera_imagen(fondo->imagen);
    libera_ejercito(enemigos);
    libera_pantalla();

    return 0;
}//--
