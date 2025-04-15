#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "Pantalla.h"
#include "Colisiones.h"
#include "Bala.h"
#include "Rafaga.h"
#include "Ejercito.h"
#include "Escenario.h"

/**
AMPLIACIONES REALIZADAS:
1. #Alcance
**/

#define TBloque 32 //Tamaño de bloques de los escenarios.

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
void mover_heroe(Heroe heroe, Escenario e) {
    int x = heroe->x ;
    int y = heroe->y;
    int xW = (heroe->x+heroe->wt);
    int yH = (heroe->y+heroe->ht);

    if(tecla_pulsada(SDL_SCANCODE_UP) &&
            !colH_lateral(e,x,y - heroe->v,xW,y - heroe->v,1)&&
            !colH_lateral(e,x,y - heroe->v,xW,y - heroe->v,2)) {
        heroe->y = heroe->y - heroe->v;
    }

    if(tecla_pulsada(SDL_SCANCODE_DOWN) &&
            !colH_lateral(e,x,yH + heroe->v,xW,yH + heroe->v,1)&&
            !colH_lateral(e,x,yH + heroe->v,xW,yH + heroe->v,2) ) {
        heroe->y = heroe->y + heroe->v;
    }
    if(tecla_pulsada(SDL_SCANCODE_LEFT) &&
            !colH_lateral(e,x - heroe->v,y,x - heroe->v,yH,1)&&
            !colH_lateral(e,x - heroe->v,y,x - heroe->v,yH,2)) {
        heroe->x = heroe->x - heroe->v;
    }

    if(tecla_pulsada(SDL_SCANCODE_RIGHT) &&
            !colH_lateral(e,xW + heroe->v,y,xW + heroe->v,yH,1)&&
            !colH_lateral(e,xW + heroe->v,y,xW + heroe->v,yH,2)) {
        heroe->x = heroe->x + heroe->v;
    }
    dibuja_imagen(heroe -> imagen,heroe -> x,heroe -> y,heroe -> wt,heroe -> ht);

}

//*****************************************
//          Funciones ESCENARIO
//*****************************************

void iniciar_obstaculos(Escenario fondo) {
    //Obstaculos del marco
    inserta_obstaculo(fondo,1,0,0,TBloque,480);
    inserta_obstaculo(fondo,1,0,0,800,TBloque);
    inserta_obstaculo(fondo,1,24*TBloque,0,TBloque,480);
    inserta_obstaculo(fondo,1,0,14*TBloque,800,TBloque);

    //Obstaculos del medio
    inserta_obstaculo(fondo,1,5*TBloque,5*TBloque,4*TBloque,TBloque);
    inserta_obstaculo(fondo,1,5*TBloque,9*TBloque,4*TBloque,TBloque);

    inserta_obstaculo(fondo,1,16*TBloque,5*TBloque,4*TBloque,TBloque);
    inserta_obstaculo(fondo,1,16*TBloque,9*TBloque,4*TBloque,TBloque);

    //Lava
    inserta_obstaculo(fondo,2,12*TBloque,5*TBloque,TBloque,5*TBloque);
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

void pantalla_record(int record) {
    Imagen imaRecord1 =  lee_imagen("./imagenes/fondoRecord1.bmp",0);
    Imagen imaRecord2 =  lee_imagen("./imagenes/fondoRecord2.bmp",0);
    color_trazo(255,255,255,255);
    char puntos[5];
    snprintf(puntos, 4, "%d", record);

    int fin = 0, contador = 0, animacion = 0;
    while(pantalla_activa() && !fin) {
        if(animacion) {
            dibuja_imagen(imaRecord2,0,0,800,480);
        } else {
            dibuja_imagen(imaRecord1,0,0,800,480);
        }
        dibuja_texto("PUNTOS:",360,280);
        dibuja_texto(puntos,430,280);

        //Boton 1
        if(tecla_pulsada(SDL_SCANCODE_SPACE)) {
            fin = 1;
        }
        espera(40);
        actualiza_pantalla();

        if(contador >= 15) {
            animacion = !animacion;
            contador = 0;
        }
        contador++;
    }
    libera_imagen(imaRecord1);
    libera_imagen(imaRecord2);
}

/**
    \brief Partida principal del juegador.
*/
int partida () {
    Escenario fondo = crea_escenario(lee_imagen("./imagenes/map.bmp",0),800,480,TBloque);
    iniciar_obstaculos(fondo);
    struct TesoroStr tesoroStr = {.x = 749, .y = 429, .wt = 32, .ht = 32, .imagen = lee_imagen("./imagenes/esmerald.bmp",1)};
    struct HeroeStr heroeStr = {.x = 32*2, .y = 32*2, .vidas = 3, .wt = 32, .ht = 32, .imagen =  lee_imagen("./imagenes/steve.bmp",0),.v = 4, .puntos = 0,.activo = 1};
    Tesoro tesoro = &tesoroStr;
    Heroe heroe = &heroeStr;

    Rafaga listaBalas = crea_rafaga();
    Bala bAux = NULL;

    //ENEMIGOS
    Ejercito enemigos = crea_ejercito(fondo);

    //**

    int fin = 0, iteracion = 0;
    while(pantalla_activa() && !fin) {

        dibuja_escenario(fondo);
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

        mover_heroe(heroe, fondo);
        mueve_ejercito(enemigos,fondo);
        dibuja_ejercito(enemigos);

        //Colision con tesoro.
        if(solape_rectangulos(heroe->x,heroe->y,heroe->wt,heroe->ht,tesoro->x,tesoro->y,tesoro->wt,tesoro->ht)) {
            heroe->puntos = heroe->puntos + 5;
            do {
                tesoro->x = rand()%737;
                tesoro->y = rand()%337;
            } while(dentro_bloque(fondo,tesoro->x,tesoro->y,tesoro->wt,tesoro->ht,1)||
                    dentro_bloque(fondo,tesoro->x,tesoro->y,tesoro->wt,tesoro->ht,2));
        }

        heroe->puntos = heroe->puntos + colision_ejercito_rafaga(enemigos,listaBalas);

        //Se encarga de controlar la vida del heroe en funcion de las colisiones.
        if(colision_ejercito_objeto(enemigos,heroe->x,heroe->y,heroe->wt,heroe->ht)) {
            heroe->vidas --;
            if(heroe->vidas>0) {
                do {
                    heroe->x = rand()%737;
                    heroe->y = rand()%337;
                } while(dentro_bloque(fondo,heroe->x,heroe->y,heroe->wt,heroe->ht,1));

            } else {
                fin = 1;
            }
        }


        actualiza_pantalla();
        espera(40);

        //Add nuevos enemigos.
        if(iteracion >= 25) {
            inserta_enemigo(enemigos,fondo);
            mod_aleatoria(enemigos);
            mod_aleatoria(enemigos);
            mod_aleatoria(enemigos);
            iteracion = 0;
        } else {
            iteracion++;
        }

    }//Termina While = 1


    //Final Partida.
    libera_rafaga(listaBalas);
    libera_imagen(heroe->imagen);
    libera_escenario(fondo);
    libera_imagen(tesoro->imagen);
    libera_ejercito(enemigos);
    return heroe->puntos;
}

/**
    \brief Muestra una imagen con la explicación del juego.
*/
void ayuda() {

    Imagen ayudaImagen =  lee_imagen("./imagenes/controles.bmp",0);


    int fin = 0;
    while(pantalla_activa() && !fin) {
        dibuja_imagen(ayudaImagen,0,0,800,480);

        if(tecla_pulsada(SDL_SCANCODE_SPACE)) {
            fin = 1;
        }
        espera(40);
        actualiza_pantalla();
    }
    libera_imagen(ayudaImagen);
}

//*****************
//*****************
/**
    \brief Sirve para controlar el dibujado del boton.
    \param boton Imagen del boton.
    \param x Coordenada horizontal del boton.
    \param y Coordenada vertical del boton.
    \param h Altura del boton.
    \param w Anchura del boton.
*/
void dibujar_boton(Imagen boton,int dentro,int x, int y, int h, int w) {
    if(dentro) {
        dibuja_imagen(boton,x-2,y-1,w+4,h+2);
    } else {
        dibuja_imagen(boton,x,y,w,h);
    }
}


/**
    \brief Muestra un menú del juego con las diferentes opciones.
    \return Opción elegida.
*/
int menu() {

    Imagen menuImagen =  lee_imagen("./imagenes/menu.bmp",0);
    Imagen boton1 =  lee_imagen("./imagenes/botonJugar.bmp",0);
    Imagen boton2 =  lee_imagen("./imagenes/botonAyuda.bmp",0);
    Imagen boton3 =  lee_imagen("./imagenes/botonSalir.bmp",0);

    int fin = 0, opcion = 0, dBoton1 = 0, dBoton2 = 0, dBoton3 = 0;
    while(pantalla_activa() && !fin) {
        dibuja_imagen(menuImagen,0,0,800,480);

        //Boton 1
        dBoton1 = dentroRectangulo(300,220,50,200,x_raton(),y_raton());
        dibujar_boton(boton1,dBoton1,300,220,50,200);
        if(dBoton1 && boton_raton_pulsado(SDL_BUTTON_LEFT)) {
            opcion = 1;
            fin = 1;
        }

        //Boton 2
        dBoton2 = dentroRectangulo(300,290,50,200,x_raton(),y_raton());
        dibujar_boton(boton2,dBoton2,300,290,50,200);
        if(dBoton2 && boton_raton_pulsado(SDL_BUTTON_LEFT)) {
            opcion = 2;
            fin = 1;
        }

        //Boton 3
        dBoton3 = dentroRectangulo(300,360,50,200,x_raton(),y_raton());
        dibujar_boton(boton3,dBoton3,300,360,50,200);
        if(dBoton3 && boton_raton_pulsado(SDL_BUTTON_LEFT)) {
            opcion = 3;
            fin = 1;
        }

        actualiza_pantalla();
        espera(40);
    }
    libera_imagen(boton1);
    libera_imagen(boton2);
    libera_imagen(boton3);
    libera_imagen(menuImagen);
    return opcion;
};



//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// TODO: INFORMAR AL USUARIO DE LOS RECORDS
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int main(int argc, char *argv[]) {
    srand(time(NULL));//Establecemos semilla aleatoria.

    crea_pantalla("Ejemplo 1",800,480);
    Archivo archi = fopen("./puntos.txt", "r+");

    int puntos = 0, record = 0, fin = 0;
    while(pantalla_activa() && !fin) {
        int opcion = menu();
        switch(opcion) {
        case 1:
            puntos = partida();
            break;

        case 2:
            ayuda();
            break;
        case 3:
            fin = 1;
            break;
        }

        //*************************************
        //*************************************
        // ZONA DE RECORD
        //*************************************

        fscanf(archi, "%d", &record);
        if(record < puntos) {
            rewind(archi);
            fprintf(archi, "%d", puntos);
            pantalla_record(puntos);
        }
    }

    fclose(archi);
    libera_pantalla();

    return 0;
}//--

