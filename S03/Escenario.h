#ifndef __Escenario_h__
#define __Escenario_h__
#include "Pantalla.h"

typedef struct EscenarioRep * Escenario;

/**
    \brief Crea un escenario vac�o y le asocia una imagen de fondo para que la use al dibujarlo.
    PreRequisitos: Para que funcione correctamente es necesario que el ancho y la altura de la imagen
    sean m�ltiplos del tama�o de bloque.
    \param fondo Imagen del fondo.
    \param anchoImagen Anchura de la imagen en pixeles.
    \param alturaImagen Altura de la imagen en pixeles.
    \param tBloque Tama�o de los bloques que construyen la imagen en pixeles.
*/
Escenario crea_escenario( Imagen fondo, int anchoImgen, int alturaImagen, int tBloque);

/**
    \brief Libera la memoria del escenario.
    \param e Escenario del cual liberar memoria.
*/
void libera_escenario( Escenario e );

/**
    \brief A�ade un obst�culo definido mediante el rect�ngulo x,y,w,h.
    PreRequisitos: Para que funcione correctamente es necesario que las coordenadas y tama�os del obst�culo
    den m�ltiplos del tama�o de bloque.
    Nota: Los obst�culos se pueden ir superponer hasta terminar de modelar las colisiones
    en el mapa como se quiera. Tiene mayor orden de importancia siempre el �ltimo obst�culo
    a�adido.
    \param e Escenario al cual se le va a a�adir un obst�culo.
    \param tipoObstaculo
    "1" Si no se puede traspasar por nadie.
    "2" Si solo una parte de los entes puede traspasarlo.
    \param x Coordenada horizontal superior izquierda.
    \param y Coordenada vertical superior izquierda.
    \param w Anchura en pixeles del obst�culo.
    \param h Altura en pixeles del obst�culo.

*/
void inserta_obstaculo( Escenario e, int tipoObstaculo, int x, int y, int w, int h);

/**
    \brief  Funci�n que te dice el tipo de bloque en el que se encuentra unas
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

#endif
