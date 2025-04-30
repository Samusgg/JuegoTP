#include "Rafaga.h"
#include <stdlib.h>
#include <stdio.h>

/**
   NODO LISTA DE BALAS
*/
struct Nodo {
    Bala miBala;
    struct Nodo * sig;
};
typedef struct Nodo NodoBala;


/**
   Constuctor del tipo del TDA
*/
struct TDARafaga {
    NodoBala * listaBalas;
    int nBalas;
    int alcance;
};

//*****************************************
//        Funciones BALAS
//*****************************************


/**
    \brief Crea una lista de balas.
    \return Puntero cabecera lista de balas.
*/
NodoBala * crea_lista_balas() {
    NodoBala * cabecera = malloc(sizeof(NodoBala));
    cabecera->sig = NULL;
    cabecera->miBala = NULL;
    return cabecera;

}

/**
    \brief Inserta una bala en una lista de balas.
    \param cabecera Puntero cabecera de una lista de balas.
    \param b Struct de la bala a insertar.

*/
void inserta_lista_balas ( NodoBala * cabecera, Bala b ) {

    NodoBala * nodoBuscado = cabecera;

    while(nodoBuscado->sig != NULL) {
        nodoBuscado = nodoBuscado->sig;
    }//Busca el nodo al final de la lista.


    NodoBala * nuevoNodo = malloc(sizeof(NodoBala));
    nuevoNodo->sig = NULL;
    nuevoNodo->miBala = b;
    nodoBuscado->sig = nuevoNodo;
}

/**
    \brief Libera de memoria una lista de balas.
    \param cabecera Puntero cabecera de una lista de balas.
*/
void libera_lista_balas ( NodoBala * cabecera ) {
    NodoBala * nodoLibre;
    while(cabecera->sig != NULL) {
        nodoLibre = cabecera->sig;
        cabecera->sig = nodoLibre->sig;
        libera_bala(nodoLibre->miBala);
        free(nodoLibre);
    }
    free(cabecera);
}

/**
    \brief Funci�n que indica si una bala est� fuera de la pantalla.
    \param miBala Bala a comprobar.
    \return 1 si est� fuera de la pantalla. 0 si no.
*/
int fueraPantalla(Bala miBala) {
    int x = get_x_bala(miBala);
    int y = get_y_bala(miBala);
    if(x < -50 || x > 850 || y < -50 || y >530) {
        return 1;
    } else {
        return 0;
    }
}

/**
    \brief  Funci�n que indica si una bala est� fuera del alcance indicado.
    \param  miBala Bala a comprobar.
    \param  xRef Coordenada X en la que se lanz� por primera vez la bala.
    \param  yRef Coordenada Y en la que se lanz� por primera vez la bala.
    \return 1 si est� fuera del alcance estimado, 0 si no.
*/
int fueraAlcance(Bala miBala, int alcance){
    int x = get_x_bala(miBala);
    int y = get_y_bala(miBala);
    if(x < get_xRef_bala(miBala)-alcance || x > get_xRef_bala(miBala)+alcance || y < get_yRef_bala(miBala)-alcance || y > get_yRef_bala(miBala)+alcance) {
        return 1;
    } else {
        return 0;
    }
}
/**
    \brief Mueve todas las balas de una estructura enlazada (lista).
    \param nodoActivo Puntero cabecera de una lista de balas.
    \param e Escenario sobre el cual se va an a mover las balas.
    \param alcance Alcance m�ximo en p�xeles que pueden moverse las balas.
    \return int N�mero de balas menos.
*/
int mueve_lista_balas ( NodoBala * nodoActivo, Escenario e, int alcance) {
    int nMenos = 0;

    NodoBala * aux = NULL;
    Bala bala = NULL;
    while(nodoActivo->sig != NULL) {
        bala = nodoActivo->sig->miBala;
        mueve_bala(bala);
        if(fueraPantalla(bala) ||
           fueraAlcance(bala,alcance)||
           dentro_bloque(e,get_x_bala(bala),get_y_bala(bala),get_wt_bala(bala),get_ht_bala(bala),1)) {
            aux =  nodoActivo->sig;
            nodoActivo->sig = nodoActivo->sig->sig;
            libera_bala(aux->miBala);
            free(aux);
            nMenos++;
        } else {
            nodoActivo = nodoActivo->sig;
        }
    }
    return nMenos;
}

/**
    \brief Dibuja todas las balas de una lista.
    \param cabecera Puntero cabecera de una lista de balas.
*/
void dibuja_lista_balas ( NodoBala * cabecera ) {
    NodoBala * nodoActivo = cabecera;
    while(nodoActivo->sig != NULL) {
        nodoActivo = nodoActivo->sig;
        dibuja_bala(nodoActivo->miBala);
    }
    //Porque puede ser que no haya balas
    //Y solo tengamos la cabecera.
    if(nodoActivo->miBala!=NULL) {
        dibuja_bala(nodoActivo->miBala);
    }
}


/**
    \brief Funcion que sirve para ver si hay colision entre los enemigos con una bala.
    \param n Longitud del array de enemigos.
    \param xe Coordenada horizontal de la esquina superior izquierda del enemigo.
    \param ye Coordenada vertical de la esquina superior izquierda del enemigo.
    \param we Anchura del enemigo.
    \param he Altura del enemigo.
    \return 1 si existe colision, 0 si no.
*/
int colision_lista_balas(NodoBala * cabecera, int xe, int ye, int we, int he) {

    NodoBala * nodoActivo = cabecera;
    NodoBala * aux = NULL;
    while(nodoActivo->sig != NULL) {
        if(solape_rectangulos(get_x_bala(nodoActivo->sig->miBala),get_y_bala(nodoActivo->sig->miBala),get_wt_bala(nodoActivo->sig->miBala),get_ht_bala(nodoActivo->sig->miBala),xe,ye,we,he)) {
            libera_bala(nodoActivo->sig->miBala);
            aux = nodoActivo->sig;
            nodoActivo->sig = aux->sig;
            free(aux);
            return 1;
        } else {
            nodoActivo = nodoActivo->sig;
        }
    }
    return 0;
}



//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************
//                    FUNCIONES DE EL TDA RAFAGA
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************


/**
    \brief Esta funci�n devuelve una nueva r�faga vac�a, es decir,
    sin ninguna bala, pero lista para almacenar cualquier n�mero de balas.
    \return Rafaga Lista de balas vac�a.
*/
Rafaga crea_rafaga(int alcance) {
    Rafaga listaRafaga = malloc(sizeof(struct TDARafaga));
    listaRafaga -> listaBalas = crea_lista_balas();
    listaRafaga -> nBalas = 0;
    listaRafaga->alcance = alcance;
    return listaRafaga;
};

/**
    \brief Esta funci�n libera toda la memoria asociada a la r�faga r
    y las balas que contenga.
    \param r TDA Rafaga de balas
*/
void libera_rafaga(Rafaga r) {
    libera_lista_balas(r->listaBalas);
    r->nBalas = 0;
};

/**
    \brief Esta funci�n a�ade a la r�faga r la bala b.
    \param r TDA Rafaga de balas
    \param b Bala a insertar en r
*/
void inserta_rafaga(Rafaga r, Bala b) {
    r->nBalas++;
    inserta_lista_balas(r->listaBalas,b);
};


/**
    \brief Mueve todas las balas contenidas en la r�faga r
    y elimina todas las balas que, tras moverse, quedan situadas fuera de la pantalla
    o chocan con un obst�culo
    \param r TDA Rafaga de balas
    \param e Escenario sobre el cual se va an a mover las balas.
*/
void mueve_rafaga(Rafaga r, Escenario e) {
    int nBalasMenos = mueve_lista_balas(r->listaBalas, e, r->alcance);
    r->nBalas = r->nBalas - nBalasMenos;
};

/**
    \brief Esta funci�n muestra todas las balas contenidas en la r�faga r.
    \param r TDA Rafaga de balas
*/
void dibuja_rafaga(Rafaga r) {
    dibuja_lista_balas(r->listaBalas);
};

/**
    \brief Esta funci�n devuelve el n�mero de balas que incluye la r�faga r.
    \param r TDA Rafaga de balas
    \return int Longitud rafaga
*/
int longitud_rafaga(Rafaga r) {
    return r->nBalas;
};


/**
    \brief Esta funci�n busca y elimina la primera bala incluida en la r�faga r que se solape
     con el rect�ngulo con esquina superior izquierda en (x,y), anchura w y altura h. La
     funci�n devolver� 1 si elimin� alguna bala y 0 en caso contrario.
    \param r TDA Rafaga de balas
    \param x Coordenada x esquina superior izquierda del rect�ngulo.
    \param y Coordenada y esquina superior izquierda del rect�ngulo.
    \param w Anchura del rect�ngulo.
    \param h Altura del rect�ngulo.
    \return 1 Si ha habido colisi�n, 0 si no.
*/
int colision_rafaga(Rafaga r, int x, int y, int w, int h) {
    if(colision_lista_balas(r->listaBalas,x,y,w,h)) {
        r->nBalas--;
        return 1;
    } else {
        return 0;
    }
};
