#include <stdlib.h>
#include <stdio.h>
#include "Pantalla.h"
#include "Escenario.h"

struct EscenarioRep {
    Imagen imagen;    //Imagen del escenario
    int anchoImagen;  //Ancho de imagen
    int alturaImagen; //Altura de imagen
    int tBloque;      //Tamaño ancho de un bloque (Suponemos bloques cuadrados).
    int * mapaColision; //Es un mapa bidimensional de colisiones.
    int nFilas;       //Numero de filas mapa colisiones.
    int nColumnas;       //Numero de columnas mapa colisiones.
};

/**
    \brief Crea un escenario vacío y le asocia una imagen de fondo para que la use al dibujarlo.
    \param fondo Imagen del fondo.
    \param anchoImagen Anchura de la imagen en pixeles.
    \param alturaImagen Altura de la imagen en pixeles.
    \param tBloque Tamaño de los bloques que construyen la imagen en pixeles.
*/
Escenario crea_escenario( Imagen fondo, int anchoImagen, int alturaImagen, int tBloque){
    Escenario e = malloc(sizeof(struct EscenarioRep));
    e->imagen = fondo;
    e->anchoImagen = anchoImagen;
    e->alturaImagen = alturaImagen;
    e->tBloque = tBloque;

    int anchoBloq = anchoImagen/tBloque;
    int altoBloq = alturaImagen/tBloque;
    e->nFilas = altoBloq;
    e->nColumnas = anchoBloq;

    e->mapaColision = calloc(anchoBloq*altoBloq,4*anchoBloq*altoBloq); //Como malloc, pero inicializa a 0 todos los bits.
    return e;
};

/**
    \brief Libera la memoria del escenario.
    \param e Escenario del cual liberar memoria.
*/
void libera_escenario( Escenario e ){
    free(e->mapaColision);
    free(e->imagen);
    free(e);
};

/**
    \brief Añade un obstáculo definido mediante el rectángulo x,y,w,h.
    PreRequisitos: Para que funcione correctamente es necesario que las coordenadas y tamaños del obstáculo
    den múltiplos del tamaño de bloque.
    \param e Escenario al cual se le va a añadir un obstáculo.
    \param tipoObstaculo
    "0" si no es un obtáculo.
    "1" Si se puede traspasar por todos.
    "2" Si solo una parte de los entes puede traspasarlo.
    \param x Coordenada horizontal superior izquierda.
    \param y Coordenada vertical superior izquierda.
    \param w Anchura en pixeles del obstáculo.
    \param h Altura en pixeles del obstáculo.

*/
void inserta_obstaculo( Escenario e, int tipoObstaculo, int x, int y, int w, int h){
    int indX = x/e->tBloque;
    int indY = y/e->tBloque;
    int indX_fin =  (x+w)/e->tBloque;
    int indY_fin =  (y+h)/e->tBloque;

    //Recorrido Vertical
    for(int iY = indY; iY<indY_fin; iY++){
        //Recorrido horizontal.
        for(int iX = indX; iX<indX_fin; iX++){
            e->mapaColision[iY*e->nColumnas + iX] = tipoObstaculo;
        }
    }
};

/**
    \brief  Función que te dice el tipo de bloque en el que se encuentra unas
    coordenadas. El tipo de bloque nos indica si se puede traspasar el bloque
    o si por el contrario "es un muro".
    \param x coordenada horizontal
    \param y coordenada vertical
    \return "0" Si no se puede traspasar por nadie.
    "1" Si se puede traspasar por todos.
    "2" Si solo una parte de los entes puede traspasarlo.
    "-1" Si las coordenadas no existen.
*/
int tipo_bloque(Escenario e, int x, int y){
    int columna = x/e->tBloque; //Indice Columna
    int fila = y/e->tBloque; //Indice Fila

    if(fila < e->nFilas && columna < e->nColumnas){
       return  e->mapaColision[fila*e->nColumnas+columna];
    }
    return -1;
};


/**
    \brief Dibuja la imagen de fondo que representa el escenario.
    \param e Escenario a dibujar.
*/
void dibuja_escenario( Escenario e ){
    dibuja_imagen(e -> imagen,0,0,e->anchoImagen,e -> alturaImagen);
};


/**
    \brief Función que comprueba la colisión de un lateral de un bloque
    con respecto a un tipo de bloque de un escenario.
    \param e Escenario.
    \param x Coordenada horizontal del primer Punto.
    \param y Coordenada vertical del primer Punto.
    \param x2 Coordenada horizontal del segundo Punto.
    \param y2 Coordenada vertical del segundo Punto.
    \param tipoB Tipo de bloque del escenario.
    \return 1 si ha habido colisión, 0 si no.
*/
int colH_lateral(Escenario e, int x, int y, int x2, int y2,int tipoB) {
     if(tipo_bloque(e,x,y)== tipoB || tipo_bloque(e,x2,y2)== tipoB)
        return 1;
    return 0;
}


/**
    \brief Función que comprueba si un rectángulo choca en alguno de sus vertices con
    un bloque del tipo indicado.
    \param e Escenario.
    \param x Coordenada horizontal del rectángulo.
    \param y Coordenada vertical del rectángulo.
    \param wt Anchura del rectángulo.
    \param ht Altura del rectángulo.
    \param tipoB Tipo de bloque del escenario.
*/
int dentro_bloque(Escenario e, int x, int y, int wt, int ht, int tipoB) {
     if(colH_lateral(e,x,y,x+wt,y,tipoB) ||
        colH_lateral(e,x+wt,y,x+wt,y+wt,tipoB) ||
        colH_lateral(e,x+wt,y+wt,x,y+wt,tipoB) ||
        colH_lateral(e,x,y+wt,x,y,tipoB)
        ){
        return 1;
    }
    return 0;
}

