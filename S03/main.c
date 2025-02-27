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
    int puntos;
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
    int dir; //Direccion
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
    int vx; //Velocidad
    int vy; //Velocidad
} BalaRep;
typedef BalaRep * Bala;

typedef FILE * Archivo;
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
void dibuja_fondo(Fondo * fondo) {
    //Dibuja fondo
    dibuja_imagen(fondo -> imagen,fondo -> x,fondo -> y,fondo -> wt,fondo -> ht);
}

/**
Dibuja informacion sobre los puntos y las vidas.
*/
void dibuja_info(int puntos, int vidas){
    char textoV[32] = "Vidas: ";
    char textoP[32] = "Puntos: ";
    char aux[12];
    sprintf(aux,"%d",vidas); //Transforma el entero a un array de caracteres.
    strcat(textoV, aux);     //Concatena los arrays de caracteres
    sprintf(aux,"%d",puntos);
    strcat(textoP, aux);

    color_trazo(255,0,0,255);
    dibuja_texto(textoV,399,0);
    dibuja_texto(textoP,200,0);
}

void dibuja_tesoro(Tesoro * tesoro) {
    //Dibuja tesoro
    dibuja_imagen(tesoro -> imagen,tesoro -> x,tesoro -> y,tesoro -> wt,tesoro -> ht);
}

//*****************************************
//          Funciones HEROE
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

//*****************************************
//          Funciones BALAS.
//*****************************************


/**
Es una funcion dedicada a crear balas para el heroe.
*/
Bala crea_bala (int x, int y, int vx, int vy) {
    Bala miBala = malloc(sizeof(BalaRep));
    miBala->x = x;
    miBala->y = y;
    miBala->wt =50;
    miBala->ht = 50;
    miBala->imagen = lee_imagen("./imagenes/arrow.bmp",1);
    miBala->vx = vx;
    miBala->vy = vy;
    return miBala;
}

void libera_bala(Bala b) {
    free(b->imagen);
    free(b);
}

void mueve_bala(Bala b) {
    b->y = b->y - b->vy;
}

void dibuja_bala(Bala b) {
    //Dibuja Bala
    dibuja_imagen(b -> imagen,b -> x,b -> y,b -> wt,b -> ht);
}

int get_x_bala(Bala b) {
    return b->x;
}

int get_y_bala(Bala b) {
    return b->y;
}

int colision_bala (Bala bala, int x, int y, int w, int h) {
    if(solape_rectangulos(x,y,w,h,bala->x,bala->y,bala->wt,bala->ht)) {
        return 1;
    }
    return 0;
}

int colision_enemigos_bala (EnemigoRep * enemigo [], int n, Bala bala) {
    for(int i = 0; i<n; i++) {
        if(colision_bala(bala,enemigo[i]->x, enemigo[i]->y, enemigo[i]->wt, enemigo[i]->ht)) {
            enemigo[i]->activo = 0;
            enemigo[i]->x = rand()%751;
            enemigo[i]->y = rand()%431;
            return 1;
        }
    }
    return 0;
}



//*****************************************
//          Funciones ENEMIGOS
//*****************************************

/**
Esta funcion mueve a los enemigos activos respecto al heroe.

*/
void mover_enemigos(EnemigoRep * enemigo[], int n, Heroe * heroe ) {
    for(int i = 0; i<n; i++) {
        if(enemigo[i]->activo == 1) {
            switch(enemigo[i]->dir) {
            case 0: {
                if(heroe->x/2>enemigo[i]->x/2) {
                    enemigo[i]->x = enemigo[i]->x + enemigo[i]->v;
                } else {
                    enemigo[i]->x = enemigo[i]->x - enemigo[i]->v;
                };
                break;
            }

            case 1: {
                if(heroe->y/2>enemigo[i]->y/2) {
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
Funcion que sirve para ver la colision del enemigo con objeto.

*/
int colision_enemigos_objeto (EnemigoRep * enemigo [], int n, int x, int y, int w, int h) {
    for(int i = 0; i<n; i++) {
        if(enemigo[i]->activo==1 && solape_rectangulos(x,y,w,h,enemigo[i]->x,enemigo[i]->y,enemigo[i]->wt,enemigo[i]->ht)) {
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
    Heroe heroe = {.x = 0, .y = 0, .vidas = 3, .wt = 50, .ht = 50, .imagen =  lee_imagen("./imagenes/steve.bmp",0),.v = 5, .puntos = 0,.activo = 1};
    Bala bala = NULL;
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

        dibuja_fondo(&fondo);
        dibuja_info(heroe.puntos,heroe.vidas);
        //Dibuja Tesoro
        dibuja_tesoro(&tesoro);

        //PROGRAMAS DE LA BALA
        if(bala==NULL && tecla_pulsada(SDL_SCANCODE_SPACE)) {
            bala = crea_bala(heroe.x,heroe.y,5,5);
        }
        if(bala!=NULL) {
            mueve_bala(bala);
            dibuja_bala(bala);
            if(get_y_bala(bala)<-51) {
                libera_bala(bala);
                bala = NULL;
            }
        }
        mover_heroe(&heroe);
        mover_enemigos(enemigos, nEnemigos, &heroe);


        //Colision con tesoro.
        if(solape_rectangulos(heroe.x,heroe.y,heroe.wt,heroe.ht,tesoro.x,tesoro.y,tesoro.wt,tesoro.ht)) {
            heroe.puntos ++;
            tesoro.x = rand()%751;
            tesoro.y = rand()%431;
        }

        //Colision enemigo bala
        if(bala!=NULL && colision_enemigos_bala(enemigos, nEnemigos,bala)) {
            libera_bala(bala);
            heroe.puntos++;
            bala = NULL;
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
    if(record < heroe.puntos){
        rewind(archi);
        fprintf(archi, "%d", heroe.puntos);
    }
    libera_imagen(fondo.imagen);
    fondo.imagen =  lee_imagen("./imagenes/floor.bmp",0);
    dibuja_fondo(&fondo);
    actualiza_pantalla();
    while(pantalla_activa()) {}


    //Final del programa.
    fclose(archi);
    libera_imagen(heroe.imagen);
    libera_imagen(tesoro.imagen);
    libera_imagen(imagenEnemigo);
    libera_imagen(fondo.imagen);
    libera_pantalla();

    return 0;
}//--
