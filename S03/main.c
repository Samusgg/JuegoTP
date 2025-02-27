#include "Pantalla.h"
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

/**
    BALA
*/
typedef struct {
    int x;  //Eje x
    int y;  //Eje Y
    int wt; //Anchura
    int ht; //Altura
    Imagen imagen;
    int vx; //Velocidad
    int vy; //Velocidad
    int dir; //Dirección
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
    \brief Comprueba si dos rectangulos colisionan entre si.
    \param x1 Coordenada horizontal de la esquina superior izquierda del primer rectángulo.
    \param y1 Coordenada vertical de la esquina superior izquierda del primer rectángulo.
    \param w1 Anchura del primer rectangulo a dibujar.
    \param h1 Altura del primer rectangulo a dibujar.
    \param x2 Coordenada horizontal de la esquina superior izquierda del segundo rectángulo.
    \param y2 Coordenada vertical de la esquina superior izquierda del segundo rectángulo.
    \param w2 Anchura del segundo rectangulo a dibujar.
    \param h2 Altura del segundo rectangulo a dibujar.
    \return 1 Si hay solape, 0 en caso contrario.
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
void dibuja_info(int puntos, int vidas) {
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
//          Funciones BALAS.
//*****************************************


/**
    \brief Es una funcion dedicada a crear balas para el heroe.
    \param x Coordenada horizontal de la esquina superior izquierda de la bala.
    \param y Coordenada vertical de la esquina superior izquierda de la bala.
    \param vx Velocidad de la bala en el eje x.
    \param vy Velocidad de la bala en el eje y.
    \return Puntero de la nueva bala.
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

/**
    \brief Libera de memoria la bala.
    \param b Puntero a BalaRep

*/
void libera_bala(Bala b) {
    free(b->imagen);
    free(b);
}

/**
    \brief Mueve la bala modificando su posición.
    \param b Puntero a BalaRep

*/
void mueve_bala(Bala b) {
    switch(b->dir) {
    case 0: {
        b->y = b->y - b->vy;
        break;
    }
    case 1: {
        b->x = b->x - b->vx;
        break;
    }
    case 2: {
        b->x = b->x + b->vx;
        break;
    }
    case 3: {
        b->y = b->y + b->vy;
        break;
    }
    }

}

/**
    \brief Dibuja la bala.
    \param b Puntero a BalaRep
    \param angulo Angulo de inclinación bala en sentido horario.

*/
void dibuja_bala(Bala b, int dir) {
    //Dibuja Bala
    switch(dir) {
    case 0: {
        dibuja_imagen_transformada(b->imagen,b->x,b->y,b->wt,b->ht,-45.0,SDL_FLIP_NONE);
        break;
    }
    case 1: {
        dibuja_imagen_transformada(b->imagen,b->x,b->y,b->wt,b->ht,-135.0,SDL_FLIP_NONE);
        break;
    }
    case 2: {
        dibuja_imagen_transformada(b->imagen,b->x,b->y,b->wt,b->ht,-315.0,SDL_FLIP_NONE);
        break;
    }
    case 3: {
        dibuja_imagen_transformada(b->imagen,b->x,b->y,b->wt,b->ht,-225.0,SDL_FLIP_NONE);
        break;
    }
    }
}

/**
    \brief Obtiene posicion en x de la bala.
    \param b Puntero a BalaRep.
    \return Número entero de la posición en x de la bala.
*/
int get_x_bala(Bala b) {
    return b->x;
}

/**
    \brief Obtiene posicion en y de la bala.
    \param b Puntero a BalaRep.
    \return Número entero de la posición en y de la bala.
*/
int get_y_bala(Bala b) {
    return b->y;
}

/**
    \brief Obtiene posicion en x de la bala.
    \param  bala Puntero a estructura BalaRep.
    \param x Coordenada horizontal de la esquina superior izquierda de la bala.
    \param y Coordenada vertical de la esquina superior izquierda de la bala.
    \param w Anchura de la bala.
    \param h Altura de la bala.
    \return 1 si hay colisión, 0 en caso contrario.
*/
int colision_bala (Bala bala, int x, int y, int w, int h) {
    if(solape_rectangulos(x,y,w,h,bala->x,bala->y,bala->wt,bala->ht)) {
        return 1;
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
    struct FondoStr fondoStr = {.x = 0, .y = 0, .wt = 800, .ht = 480, .imagen = lee_imagen("./imagenes/map.bmp",0)};
    struct TesoroStr tesoroStr = {.x = 749, .y = 429, .wt = 50, .ht = 50, .imagen = lee_imagen("./imagenes/esmerald.bmp",1)};
    struct HeroeStr heroeStr = {.x = 0, .y = 0, .vidas = 3, .wt = 50, .ht = 50, .imagen =  lee_imagen("./imagenes/steve.bmp",0),.v = 5, .puntos = 0,.activo = 1};
    Fondo fondo = &fondoStr;
    Tesoro tesoro = &tesoroStr;
    Heroe heroe = &heroeStr;

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

        dibuja_fondo(fondo);
        dibuja_info(heroe->puntos,heroe->vidas);
        //Dibuja Tesoro
        dibuja_tesoro(tesoro);

        //PROGRAMAS DE LA BALA
        if(bala==NULL) {
            if(tecla_pulsada(SDL_SCANCODE_W)) {
                bala = crea_bala(heroe->x,heroe->y,7,7);
                bala->dir = 0;
            }
            if(tecla_pulsada(SDL_SCANCODE_A)) {
                bala = crea_bala(heroe->x,heroe->y,7,7);
                bala->dir = 1;
            }
            if(tecla_pulsada(SDL_SCANCODE_D)) {
                bala = crea_bala(heroe->x,heroe->y,7,7);
                bala->dir = 2;
            }
            if(tecla_pulsada(SDL_SCANCODE_S)) {
                bala = crea_bala(heroe->x,heroe->y,7,7);
                bala->dir = 3;
            }
        }
        if(bala!=NULL) {
            mueve_bala(bala);
            dibuja_bala(bala,bala->dir);

            if(bala->y < -51 || bala->y > 531 ||
                    bala->x < -51 || bala->x > 851) {
                libera_bala(bala);
                bala = NULL;
            }
        }
        mover_heroe(heroe);
        mover_enemigos(enemigos, nEnemigos, heroe);


        //Colision con tesoro.
        if(solape_rectangulos(heroe->x,heroe->y,heroe->wt,heroe->ht,tesoro->x,tesoro->y,tesoro->wt,tesoro->ht)) {
            heroe->puntos ++;
            tesoro->x = rand()%751;
            tesoro->y = rand()%431;
        }

        //Colision enemigo bala
        if(bala!=NULL && colision_enemigos_bala(enemigos, nEnemigos,bala)) {
            libera_bala(bala);
            heroe->puntos++;
            bala = NULL;
        }


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
    fclose(archi);
    libera_imagen(heroe->imagen);
    libera_imagen(tesoro->imagen);
    libera_imagen(imagenEnemigo);
    libera_imagen(fondo->imagen);
    libera_pantalla();

    return 0;
}//--
