#ifndef __Escenario_h__
#define __Escenario_h__
#include "Pantalla.h"

typedef struct EscenarioRep * Escenario;

/**
    \brief Crea un escenario vacío y le asocia una imagen de fondo para que la use al dibujarlo.
    PreRequisitos: Para que funcione correctamente es necesario que el ancho y la altura de la imagen
    sean múltiplos del tamaño de bloque.
    \param fondo Imagen del fondo.
    \param anchoImagen Anchura de la imagen en pixeles.
    \param alturaImagen Altura de la imagen en pixeles.
    \param tBloque Tamaño de los bloques que construyen la imagen en pixeles.
*/
Escenario crea_escenario( Imagen fondo, int anchoImgen, int alturaImagen, int tBloque);

/**
    \brief Libera la memoria del escenario.
    \param e Escenario del cual liberar memoria.
*/
void libera_escenario( Escenario e );

/**
    \brief Añade un obstáculo definido mediante el rectángulo x,y,w,h.
    PreRequisitos: Para que funcione correctamente es necesario que las coordenadas y tamaños del obstáculo
    den múltiplos del tamaño de bloque.
    Nota: Los obstáculos se pueden ir superponer hasta terminar de modelar las colisiones
    en el mapa como se quiera. Tiene mayor orden de importancia siempre el último obstáculo
    añadido.
    \param e Escenario al cual se le va a añadir un obstáculo.
    \param tipoObstaculo
    "0" Si no es un obstáculo.
    "1" Si no se puede traspasar por nadie.
    "2" Si solo una parte de los entes puede traspasarlo.
    \param x Coordenada horizontal superior izquierda.
    \param y Coordenada vertical superior izquierda.
    \param w Anchura en pixeles del obstáculo.
    \param h Altura en pixeles del obstáculo.

*/
void inserta_obstaculo( Escenario e, int tipoObstaculo, int x, int y, int w, int h);

/**
    \brief  Función que te dice el tipo de bloque en el que se encuentra unas
    coordenadas. El tipo de bloque nos indica si se puede traspasar el bloque
    o si por el contrario "es un muro".
    \param x coordenada horizontal
    \param y coordenada vertical
    \return "0" Si se puede traspasar por todos
    "1" Si no se puede traspasar por nadie.
    "2" Si solo una parte de los entes puede traspasarlo.
*/
int tipo_bloque(Escenario e, int x, int y);


/**
    \brief Dibuja la imagen de fondo que representa el escenario.
    \param e Escenario a dibujar.
*/
void dibuja_escenario( Escenario e );

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
int colH_lateral(Escenario e, int x, int y, int x2, int y2,int tipoB);

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
int dentro_bloque(Escenario e, int x, int y, int wt, int ht, int tipoB);

#endif
