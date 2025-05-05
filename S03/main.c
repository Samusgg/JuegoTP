#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "Pantalla.h"
#include "Colisiones.h"
#include "Bala.h"
#include "Rafaga.h"
#include "Ejercito.h"
#include "Escenario.h"


/** \mainpage main
    \brief Este es un proyecto para la asignatura de <i>Tecnologías de la
    Programación</i> de la <strong>Universidad de Murcia</strong>.

    Se trata de un videojuego ambientado en "Minecraft". El objetivo del mismo
    es el de ir consiguiendo puntos a base de eliminar enemigos o recoger esmeraldas.
    <p>
    <strong> AMPLIACIONES REALIZADAS:</strong>
        <ol>
            <li>\#Alcance</li>
            <li>\#MasEnemigos</li>
            <li>\#EjercitoDinámico</li>
            <li>\#Escenario</li>
        </ol>
    </p>
    \author Samuel Espín Santos
**/

/**
   \file  main.c
   \brief Archivo principal de ejecución del proyecto.
*/

#define TBloque 32 //Tamaño de bloques de los escenarios.

/**
    \struct TesoroStr
    \brief  Almacena los datos referente a las esmeraldas.
*/
struct TesoroStr {
    int x;  /*!< Coordenada horizontal esquina superior izquierda. */
    int y;  /*!< Coordenada vertical esquina superior izquierda. */
    int wt; /*!< Anchura de la imagen */
    int ht; /*!< Altura de la imagen*/
    Imagen imagen; /*!< Imagen */
};
typedef struct TesoroStr * Tesoro;


/**
    \struct HeroeStr
    \brief  Almacena la información referente al jugador.
*/
struct HeroeStr {
    int x;          /*!< Coordenada horizontal esquina superior izquierda. */
    int y;          /*!< Coordenada vertical superior izquierda. */
    int wt;         /*!< Anchura de la imagen del jugador. */
    int ht;         /*!< Altura de la imagen del jugador. */
    int vidas;      /*!< Vidas del jugador. */
    Imagen imagen;  /*!< Imagen del Jugador */
    int v;          /*!< Velocidad del héroe. */
    int puntos;     /*!< Puntos del jugador. */
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
    \param nBalas Número de balas en activo.
*/
void dibuja_info(int puntos, int vidas, int nBalas) {
    char textoV[36];
    char textoP[40];
    char textoB[36];
    snprintf(textoV, 36, "Vidas: %d", vidas); //Transforma el entero a character y concatena.
    snprintf(textoP, 40, "Puntos: %d", puntos);
    snprintf(textoB, 36, "Balas: %d", nBalas);


    color_trazo(255,255,255,255);
    dibuja_texto(textoV,0,0);   //Vidas
    dibuja_texto(textoB,100,0); //Balas
    dibuja_texto(textoP,200,0); //Puntos
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
    \brief Dibuja al Héroe.
    \param heroe Puntero a estructura HeroeStr.
*/
void dibuja_heroe(Heroe heroe) {
    //Dibuja tesoro
    dibuja_imagen(heroe -> imagen,heroe -> x,heroe -> y,heroe -> wt,heroe -> ht);
}

/**
    \brief Mueve al heroe.
    \param heroe Puntero a estructura HeroeStr.
    \param e Escenario sobre el que se va a mover el héroe.
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
}

//*****************************************
//          Funciones ESCENARIO
//*****************************************

/**
    \brief Función que inserta todos los obstáculos dentro de un escenario.
    \param fondo Escenario sobre el cual se va a poner obstáculos.
*/
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

/**
    \brief Función que muestra por pantalla un record conseguido por parte
    del jugador.
    \param record Record que se va a mostrar por pantalla.
*/
void pantalla_record(int record) {
    Imagen imaRecord1 =  lee_imagen("./imagenes/fondoRecord1.bmp",0);
    Imagen imaRecord2 =  lee_imagen("./imagenes/fondoRecord2.bmp",0);
    color_trazo(255,255,255,255);
    char puntos[5];
    snprintf(puntos, 4, "%d", record);

    int fin = 0, contador = 0, animacion = 0;
    while(pantalla_activa() && !fin) {
        //Mini animación del texto
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
    \brief Partida principal del jugador.
    \return Puntos obtenidos por el héroe.
*/
int partida () {
    Escenario fondo = crea_escenario(lee_imagen("./imagenes/map.bmp",0),800,480,TBloque);
    iniciar_obstaculos(fondo);
    struct TesoroStr tesoroStr = {.x = 23*TBloque, .y = 13*TBloque, .wt = TBloque, .ht = TBloque, .imagen = lee_imagen("./imagenes/esmerald.bmp",1)};
    struct HeroeStr heroeStr = {.x = 2*TBloque, .y = 2*TBloque, .vidas = 3, .wt = TBloque, .ht = TBloque, .imagen =  lee_imagen("./imagenes/steve.bmp",0),.v = 4, .puntos = 0};
    Tesoro tesoro = &tesoroStr;
    Heroe heroe = &heroeStr;
    Rafaga listaBalas = crea_rafaga(150);
    Bala bAux = NULL;

    //ENEMIGOS
    Ejercito enemigos = crea_ejercito(fondo,10);

    //**

    int fin = 0, iteracion = 0, contFlecha = 0;
    while(pantalla_activa() && !fin) {


        //PROGRAMAS LISTA DE BALAS.
        //Arriba
        if(tecla_pulsada(SDL_SCANCODE_W) && contFlecha >= 4) {
            contFlecha = 0;
            bAux = crea_bala(heroe->x,heroe->y,10,10);
            set_dir_bala(bAux,0);
            inserta_rafaga(listaBalas,bAux);
        }
        //Izquierda
        if(tecla_pulsada(SDL_SCANCODE_A) && contFlecha >= 4) {
            contFlecha = 0;
            bAux = crea_bala(heroe->x,heroe->y,10,10);
            set_dir_bala(bAux,1);
            inserta_rafaga(listaBalas,bAux);
        }
        //Derecha
        if(tecla_pulsada(SDL_SCANCODE_D) && contFlecha >= 4) {
            contFlecha = 0;
            bAux = crea_bala(heroe->x,heroe->y,10,10);
            set_dir_bala(bAux,2);
            inserta_rafaga(listaBalas,bAux);
        }
        //Abajo
        if(tecla_pulsada(SDL_SCANCODE_S) && contFlecha >= 4) {
            contFlecha = 0;
            bAux = crea_bala(heroe->x,heroe->y,10,10);
            set_dir_bala(bAux,3);
            inserta_rafaga(listaBalas,bAux);
        }

        mueve_rafaga(listaBalas,fondo);


        mover_heroe(heroe, fondo);

        mueve_ejercito(enemigos,fondo);


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

                    //Para que nazca en un sitio que se pueda traspasar.
                } while(dentro_bloque(fondo,heroe->x,heroe->y,heroe->wt,heroe->ht,1));

            } else {
                fin = 1;
            }
        }

        //Dibujos
        dibuja_escenario(fondo);
        dibuja_info(heroe->puntos,heroe->vidas,longitud_rafaga(listaBalas));
        dibuja_tesoro(tesoro);
        dibuja_rafaga(listaBalas);
        dibuja_heroe(heroe);
        dibuja_ejercito(enemigos);

        actualiza_pantalla();
        espera(40);

        if(contFlecha<4) {
            contFlecha++;
        }
        if(iteracion == 25) {
            mod_aleatoria(enemigos);
            mod_aleatoria(enemigos);
            mod_aleatoria(enemigos);
        }
        //Add nuevos enemigos.
        if(iteracion >= 40) {
            inserta_enemigo(enemigos,fondo);
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
//MENU
//******

/**
    \brief Sirve para modelar el comportamiento de un boton.
    \param boton Imagen del boton.
    \param x Coordenada horizontal del boton.
    \param y Coordenada vertical del boton.
    \param h Altura del boton.
    \param w Anchura del boton.
*/
int evento_boton(Imagen boton, int x, int y, int h, int w) {
    int dBoton = dentroRectangulo(x,y,h,w,x_raton(),y_raton());
    if(dBoton) {
        dibuja_imagen(boton,x-2,y-1,w+4,h+2);
        if(boton_raton_pulsado(SDL_BUTTON_LEFT)) {
            return 1;
        }
    }
    dibuja_imagen(boton,x,y,w,h);
    return 0;
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

    int opcion = 0;
    while(pantalla_activa() && opcion==0) {
        dibuja_imagen(menuImagen,0,0,800,480);

        //Boton 1
        if(evento_boton(boton1,300,220,50,200)) {
            opcion = 1;
        }
        //Boton 2
        if(evento_boton(boton2,300,290,50,200)) {
            opcion = 2;
        }
        //Boton 3
        if(evento_boton(boton3,300,360,50,200)) {
            opcion = 3;
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

//************************************
//************************************
//************************************
//MAIN
//*********
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
        //         ZONA DE RECORD
        //*************************************

        fscanf(archi, "%d", &record);
        if(record < puntos) {
            rewind(archi); //Mueve el puntero de estritura al principio del archivo.
            fprintf(archi, "%d", puntos);
            pantalla_record(puntos);
            record = puntos;
        }
    }

    fclose(archi);
    libera_pantalla();

    return 0;
}//--

