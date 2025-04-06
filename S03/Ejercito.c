#include "Rafaga.h"
#include "Ejercito.h"
#include "Pantalla.h"
#include "Colisiones.h"
#define MAX 10 //Numero total de enemigos.

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
    EjercitoTDA
*/
struct EjercitoTDA {
    EnemigoRep * enemigos;
};


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
    \param x Eje xR de referencia al que acercarse.
    \param x Eje yR de referencia al que acercarse.
*/
void mover_enemigos(EnemigoRep enemigo[], int n, int xR, int yR ) {
    for(int i = 0; i<n; i++) {
        if(enemigo[i].activo == 1) {
            switch(enemigo[i].dir) {
            case 0: {
                if(xR>enemigo[i].x) {
                    enemigo[i].x = enemigo[i].x + enemigo[i].v;
                } else {
                    enemigo[i].x = enemigo[i].x - enemigo[i].v;
                };
                break;
            }

            case 1: {
                if(yR>enemigo[i].y) {
                    enemigo[i].y = enemigo[i].y + enemigo[i].v;
                } else {
                    enemigo[i].y = enemigo[i].y - enemigo[i].v;
                };
                break;
            }
            }
        }

    }

}

/**
    \brief Dibuja a todos lo enemigos
    \param enemigo Array de punteros a enemigos.
    \param n Longitud del array de enemigos.

*/
void dibuja_enemigos(EnemigoRep enemigo[], int n) {
    for(int i = 0; i<n; i++) {
        if(enemigo[i].activo == 1) {
            dibuja_imagen(enemigo[i].imagen,enemigo[i].x,enemigo[i].y,enemigo[i].wt,enemigo[i].ht);
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
int colision_enemigos_objeto (EnemigoRep enemigo [], int n, int x, int y, int w, int h) {
    for(int i = 0; i<n; i++) {
        if(enemigo[i].activo==1 && solape_rectangulos(x,y,w,h,enemigo[i].x,enemigo[i].y,enemigo[i].wt,enemigo[i].ht)) {
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
    \return Numero enemigos eliminados.

*/
int colision_enemigos_lista_balas (EnemigoRep enemigo [], int n, Rafaga listaBalas) {
    int contador = 0;
    for(int i = 0; i<n; i++) {
        if(enemigo[i].activo == 1 && colision_rafaga(listaBalas,enemigo[i].x, enemigo[i].y, enemigo[i].wt, enemigo[i].ht)) {
            enemigo[i].activo = 0;
            enemigo[i].x = rand()%751;
            enemigo[i].y = rand()%431;
            contador++;
        }
    }
    return contador;
}

//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************
//                   FUNCIONES DE EL TDA EJERCITO
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************

/**
    \brief Devuelve un nuevo ejército vacío, es decir, sin ningún enemigo, pero
    con capacidad para almacenar hasta N enemigos. Decide y fija tú el valor de N.
    \return Ejercito de enemigos.
    **/
Ejercito crea_ejercito() {
    EnemigoRep * enemigos = malloc(sizeof(EnemigoRep)*MAX);
    Imagen imaCriper = lee_imagen("./imagenes/criper.bmp",0);
    enemigos[0] = (EnemigoRep) {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imaCriper, .v = 1, .activo = 1,.dir = rand()%2};
    enemigos[1] = (EnemigoRep) {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imaCriper, .v = 1, .activo = 1,.dir = rand()%2};
    enemigos[2] = (EnemigoRep) {.x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imaCriper, .v = 1, .activo = 1,.dir = rand()%2};
    for(int i = 3; i<MAX; i++) {
        enemigos[i] = (EnemigoRep) {
            .x = rand()%751, .y = rand()%431, .vidas = 1, .wt = 50, .ht = 50, .imagen = imaCriper, .v = 1, .activo = 0,.dir = rand()%2
        };
    }

    Ejercito e = malloc(sizeof(struct EjercitoTDA));
    e->enemigos = enemigos;
    return e;
};


/**
    \brief "Inserta" un nuevo enemigo poniendolo como activo.
    \param e Ejercito de enemigos
    **/
void inserta_enemigo (Ejercito e) {
    int i = 0;
    while(i<MAX-1 && e->enemigos[i].activo != 0) {
        i++;
    }
    e->enemigos[i].activo = 1;
};

/** \brief Mueve todos los enemigos contenidos en el ejército e.
    \param e Ejercito de enemigos.
    \param xR Eje X de referencia.
    \param yR Eje y de referencia.
**/
void mueve_ejercito(Ejercito e,int xR, int yR) {
    mover_enemigos(e->enemigos,MAX,xR,yR);
};

/**
    \brief Muestra todos los enemigos contenidos en el ejército e.
    \param e Ejercito de enemigos.
**/
void dibuja_ejercito( Ejercito e) {
    dibuja_enemigos(e->enemigos,MAX);
};

/**
    \brief Devuelve 1 si el rectángulo con esquina superior izquierda en (x,y),
    anchura w y altura h se solapa con el de alguno de los enemigos que contenga el
    ejército e.
    \param e Ejercito de enemigos.
    \param x Coordenada vertical, esquina superior izquierda.
    \param y Coordenada horizontal, esquina superior izquierda.
    \param w Anchura del rectangulo que representa al objeto.
    \param h Altura del rectangulo que representa al objeto.
    \return 1 Si se solapan, 0 si no.
**/
int colision_ejercito_objeto( Ejercito e, int x, int y, int w, int h ) {
    return colision_enemigos_objeto(e->enemigos,MAX,x,y,w,h);
};

/**
    \brief Elimina todos los enemigos incluidos en el ejecito e que
    colisionen con alguna de las balas de la ráfaga r y devolver el número de enemigos
    eliminados.
    \param e Ejercito de enemigos.
    \param r Rafaga de balas.
    \return numero enemigos eliminados.
**/
int colision_ejercito_rafaga( Ejercito e, Rafaga r ) {
    return colision_enemigos_lista_balas(e->enemigos,MAX,r);
};


/**
    \brief Libera toda la memoria asociada al ejercito.
    \param e Ejercito de enemigos.
*/
void mod_aleatoria(Ejercito e) {
    //Modifica direccion aleatoriamente.
    int toggle = rand()%10;
    switch(e->enemigos[toggle].dir) {
    case 0:
        e->enemigos[toggle].dir = 1;
        break;
    case 1:
        e->enemigos[toggle].dir = 0;
        break;
    }
};

/**
    \brief Libera toda la memoria asociada al ejercito
    y se nulifica los enemigos del ejercito.
    \param e Ejercito de enemigos.
*/
void libera_ejercito( Ejercito e ) {
    free(e->enemigos[0].imagen);
    free(e->enemigos);
    free(e);
}
