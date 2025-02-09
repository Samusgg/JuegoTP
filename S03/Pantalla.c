#include "Pantalla.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <math.h>


// TIPOS PÚBLICOS

// La estructura Imagen es la implementación interna del TDA Imagen ofrecido de forma pública por la biblioteca.
struct ImagenRep
{
    SDL_Surface *surface;
    SDL_Texture * texture;
};


// TIPOS PRIVADOS

// El tipo Pantalla (que es privado al módulo de la biblioteca) representa todos los datos que mantienen el estado de los distintos componentes que permiten dibujar en la pantalla gráfica. Desde la ventana hasta los colores usados para dibujar las líneas o rellenar las figuras, pasando por las texturas y mecanismos de renderizado utilizados.
typedef struct Pantalla
{
    int width;
    int height;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* surface;
	SDL_Color strokeColor, fillColor;
	int active;
    int fullscreen;
    int fsWidth;
    int fsHeight;
    void (* update)();
    void (* dibuja_triangulo)(int x1, int y1, int x2, int y2, int x3, int y3);
} Pantalla;

void dibuja_triangulo_fast(int x1, int y1, int x2, int y2, int x3, int y3);
void dibuja_triangulo_slow(int x1, int y1, int x2, int y2, int x3, int y3);


// VARIABLES PRIVADAS

// La variable pantalla (que es privada al módulo de la biblioteca) mantiene el estado de la ventana donde se mostrarán todas las primitivas gráficas que se lleven a cabo.
static Pantalla pantalla;


// FUNCIONES PRIVADAS

// Función privada para gestión de errores SDL.
void error_pantalla()
{
    printf("%s\n", SDL_GetError());
    exit(EXIT_FAILURE);
}

// Esta función privada se encarga de capturar el evento de cierre de la ventana
static int manejador(void *userdata, const SDL_Event *event)
{
    if ( (event->type == SDL_APP_TERMINATING || event->type == SDL_QUIT) && pantalla.active )
    {
		pantalla.active = 0;
        return(0);
    }
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_CLOSE)
	{
		pantalla.active = 0;
		return(0);
	}
    return(1);
}

void software_update() {
    if (SDL_UpdateWindowSurface(pantalla.window)==-1) error_pantalla();

}

void hardware_update() {
    SDL_RenderPresent( pantalla.renderer );
}

// FUNCIONES PÚBLICAS

void crea_pantalla(char titulo[], int w, int h)
{
    if ( pantalla.active ) return;
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) error_pantalla();

	// Crea la ventana donde se dibujará
	pantalla.window = SDL_CreateWindow(titulo, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    if (pantalla.window==NULL) error_pantalla();

    // Intentamos obtner un renderer hardware
	pantalla.renderer = SDL_CreateRenderer( pantalla.window, -1, SDL_RENDERER_ACCELERATED );
    pantalla.update = hardware_update;
    if (pantalla.renderer==NULL) {
        // Si el renderer de hardware falla probamos con render software
        SDL_DestroyWindow(pantalla.window);
        pantalla.window = SDL_CreateWindow(titulo, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
        if (pantalla.window==NULL) error_pantalla();
        // Obtenemos la superficie asociada a la ventana
        pantalla.surface = SDL_GetWindowSurface(pantalla.window);
        if (pantalla.surface==NULL) error_pantalla();
        // Crea el renderizador que utilizaremos para dibujar en la superficie
        pantalla.renderer = SDL_CreateSoftwareRenderer(pantalla.surface);
        pantalla.update = software_update;
        if (pantalla.renderer==NULL) {
    		error_pantalla();
        }
	}
    if ( SDL_VERSION_ATLEAST(2,0,18)) {
        pantalla.dibuja_triangulo = dibuja_triangulo_fast;
    } else {
        pantalla.dibuja_triangulo = dibuja_triangulo_slow;
    }
	// Activamos el modo de dibujo con transparencias en el renderizador
    if (SDL_SetRenderDrawBlendMode(pantalla.renderer, SDL_BLENDMODE_BLEND)==-1) error_pantalla();

	pantalla.strokeColor.r = 0;
	pantalla.strokeColor.g = 0;
	pantalla.strokeColor.b = 0;
	pantalla.strokeColor.a = 255;

	pantalla.fillColor.r = 255;
	pantalla.fillColor.g = 255;
	pantalla.fillColor.b = 255;
	pantalla.fillColor.a = 255;
    pantalla.width = w;
    pantalla.height = h;

	pantalla.active = 1;
    pantalla.fullscreen = 0;
	SDL_SetEventFilter((SDL_EventFilter)manejador, NULL);
    SDL_RenderSetLogicalSize(pantalla.renderer, w, h);
}

void activa_pantalla_completa() {
    if ( !pantalla.active ) return;
    SDL_SetWindowFullscreen(pantalla.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_DisplayMode dm;
    SDL_GetWindowDisplayMode(pantalla.window, &dm);
    pantalla.fullscreen = 1;
    pantalla.fsWidth = dm.w;
    pantalla.fsHeight = dm.h;
}

void desactiva_pantalla_completa() {
    if ( !pantalla.active ) return;
    SDL_SetWindowFullscreen(pantalla.window, 0);
    pantalla.fullscreen = 0;
}

void libera_pantalla()
{
	if ( !pantalla.active ) return;
    pantalla.active = 0;
    SDL_DestroyRenderer(pantalla.renderer);
    SDL_DestroyWindow(pantalla.window);
	SDL_Quit();
}

int anchura_pantalla()
{
    return pantalla.width;
}

int altura_pantalla()
{
    return pantalla.height;
}

int pantalla_activa()
{
    SDL_PumpEvents();
	return pantalla.active;
}

void espera(int ms)
{
	 SDL_Delay(ms);
}

void actualiza_pantalla()
{
    pantalla.update();
}

int tecla_pulsada(SDL_Scancode scancode) //SDL_Keycode keycode)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	return state[scancode];
}

int boton_raton_pulsado(int boton)
{
    int estado = SDL_GetMouseState(NULL, NULL);
    return estado & SDL_BUTTON(boton);
}

int x_raton()
{
    int xi,yi;
    SDL_GetMouseState(&xi, &yi);
    if ( pantalla.fullscreen ) {
        return xi*pantalla.width / pantalla.fsWidth;
    } else {
        return xi;
    }
}

int y_raton()
{
    int xi,yi;
    SDL_GetMouseState(&xi, &yi);
    if ( pantalla.fullscreen ) {
        return yi*pantalla.height/ pantalla.fsHeight;
    } else {
        return yi;
    }
}

void color_trazo(int r, int g, int b, int a)
{
	pantalla.strokeColor.r = r;
	pantalla.strokeColor.g = g;
	pantalla.strokeColor.b = b;
	pantalla.strokeColor.a = a;
}

void color_relleno(int r, int g, int b, int a)
{
	pantalla.fillColor.r = r;
	pantalla.fillColor.g = g;
	pantalla.fillColor.b = b;
	pantalla.fillColor.a = a;
}

void rellena_fondo(int r, int g, int b, int a)
{
	if (SDL_SetRenderDrawColor(pantalla.renderer, r,g,b, a)==-1) error_pantalla();
	if (SDL_RenderClear(pantalla.renderer)==-1) error_pantalla();
}

void dibuja_punto(int x, int y)
{
	if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.strokeColor.r, pantalla.strokeColor.g, pantalla.strokeColor.b, pantalla.strokeColor.a)==-1) error_pantalla();
	if (SDL_RenderDrawPoint(pantalla.renderer, x,y)==-1) error_pantalla();
}

void dibuja_linea(int x1, int y1, int x2, int y2)
{
	if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.strokeColor.r, pantalla.strokeColor.g, pantalla.strokeColor.b, pantalla.strokeColor.a)==-1) error_pantalla();
	if (SDL_RenderDrawLine(pantalla.renderer, x1,y1, x2,y2)==-1) error_pantalla();
}

void dibuja_triangulo(int x1, int y1, int x2, int y2, int x3, int y3) {
    pantalla.dibuja_triangulo(x1,y1,x2,y2,x3,y3);
}

void dibuja_triangulo_slow(int x1, int y1, int x2, int y2, int x3, int y3) {
    if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.fillColor.r, pantalla.fillColor.g, pantalla.fillColor.b, pantalla.fillColor.a)==-1) error_pantalla();
    // Reordenar puntos para que el 1 sea el más alto y poder dibujar líneas horizontales desde 1 hacia 2 y 3
    if (y1 > y2)
    {
        double t = x1;
        x1 = x2;
        x2 = t;
        t = y1;
        y1 = y2;
        y2 = t;
    }
    if (y2 > y3)
    {
        double t = x2;
        x2 = x3;
        x3 = t;
        t = y2;
        y2 = y3;
        y3 = t;
        if (y1 > y2)
        {
            double t = x1;
            x1 = x2;
            x2 = t;
            t = y1;
            y1 = y2;
            y2 = t;
        }
    }
    double xl = x1; // x izquierda
    double xr = x1;
    double dxl = (x2-x1)/(y2-y1); // Incremento iquierdo
    double dxr = (x3-x1)/(y3-y1);
    int y = y1; // altura inicial
    // Primera mitad del triángulo
    y2 = floor(y2); // Para no pasarnos de la mitad exacta en pixels
    while ( y < y2 )
    {
        if (SDL_RenderDrawLine(pantalla.renderer,xl, y, xr, y)==-1) error_pantalla();
        xl += dxl;
        xr += dxr;
        y = y + 1;
    }
    xl = x2;
    // Segunda mitad del triángulo
    dxl = (x3-xl)/(y3-y);
    dxr = (x3-xr)/(y3-y);
    while ( y < y3 )
    {
        if (SDL_RenderDrawLine(pantalla.renderer,xl, y, xr, y)==-1) error_pantalla();
        xl += dxl;
        xr += dxr;
        y = y + 1;
    }
    if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.strokeColor.r, pantalla.strokeColor.g, pantalla.strokeColor.b, pantalla.strokeColor.a)==-1) error_pantalla();
    if (SDL_RenderDrawLine(pantalla.renderer, x1,y1, x2,y2)==-1) error_pantalla();
    if (SDL_RenderDrawLine(pantalla.renderer, x2,y2, x3,y3)==-1) error_pantalla();
    if (SDL_RenderDrawLine(pantalla.renderer, x3,y3, x1,y1)==-1) error_pantalla();
}

void dibuja_triangulo_fast(int x1, int y1, int x2, int y2, int x3, int y3)
{
    SDL_Vertex vertex_1 = {{x1,y1}, {pantalla.fillColor.r, pantalla.fillColor.g, pantalla.fillColor.b, pantalla.fillColor.a}, {1, 1}};
    SDL_Vertex vertex_2 = {{x2,y2}, {pantalla.fillColor.r, pantalla.fillColor.g, pantalla.fillColor.b, pantalla.fillColor.a}, {1, 1}};
    SDL_Vertex vertex_3 = {{x3,y3}, {pantalla.fillColor.r, pantalla.fillColor.g, pantalla.fillColor.b, pantalla.fillColor.a}, {1, 1}};

    SDL_Vertex vertices[] = {
        vertex_1,
        vertex_2,
        vertex_3
    };
    SDL_RenderGeometry( pantalla.renderer, NULL, vertices, 3, NULL, 0);

    if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.strokeColor.r, pantalla.strokeColor.g, pantalla.strokeColor.b, pantalla.strokeColor.a)==-1) error_pantalla();
    if (SDL_RenderDrawLine(pantalla.renderer, x1,y1, x2,y2)==-1) error_pantalla();
    if (SDL_RenderDrawLine(pantalla.renderer, x2,y2, x3,y3)==-1) error_pantalla();
    if (SDL_RenderDrawLine(pantalla.renderer, x3,y3, x1,y1)==-1) error_pantalla();
}

void dibuja_rectangulo(int x, int y, int w, int h)
{
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;

    if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.fillColor.r, pantalla.fillColor.g, pantalla.fillColor.b, pantalla.fillColor.a)==-1) error_pantalla();
    if (SDL_RenderFillRect(pantalla.renderer, &r)==-1) error_pantalla();
	if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.strokeColor.r, pantalla.strokeColor.g, pantalla.strokeColor.b, pantalla.strokeColor.a)==-1) error_pantalla();
	if (SDL_RenderDrawRect(pantalla.renderer, &r)==-1) error_pantalla();
}

void dibuja_circulo(int x, int y, int r)
{
    int dy;
    r=round(r);
	// Dibujar círculo
    if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.fillColor.r, pantalla.fillColor.g, pantalla.fillColor.b, pantalla.fillColor.a)==-1) error_pantalla();
    for (dy = 0; dy <= r; dy++)
    {
        double dx = floor(sqrt((2.0 *r * dy) - (double)(dy * dy)));
        for (int px = x - dx; px <= x + dx; px++)
        {
            if (SDL_RenderDrawPoint(pantalla.renderer, px,y + r - dy)==-1) error_pantalla();
            if (SDL_RenderDrawPoint(pantalla.renderer, px,y - r + dy-1)==-1) error_pantalla();
         }
    }
	// Dibujar circunferencia
    if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.strokeColor.r, pantalla.strokeColor.g, pantalla.strokeColor.b, pantalla.strokeColor.a)==-1) error_pantalla();
    double error = -r;
    double px = r - 0.5;
    double py = 0.5;
    double cx = x - 0.5;
    double cy = y - 0.5;
#define ERROR 0.00001
    while (px >= py)
    {
        if (SDL_RenderDrawPoint(pantalla.renderer, cx + px, cy + py)==-1) error_pantalla();
        if (SDL_RenderDrawPoint(pantalla.renderer, cx + py, cy + px)==-1) error_pantalla();

        if ( fabs(px) > ERROR )
        {
            if (SDL_RenderDrawPoint(pantalla.renderer, cx - px, cy + py)==-1) error_pantalla();
            if (SDL_RenderDrawPoint(pantalla.renderer, cx + py, cy - px)==-1) error_pantalla();
        }

        if ( fabs(py) > ERROR )
        {
            if (SDL_RenderDrawPoint(pantalla.renderer, cx + px, cy - py)==-1) error_pantalla();
            if (SDL_RenderDrawPoint(pantalla.renderer, cx - py, cy + px)==-1) error_pantalla();
        }

        if (fabs(px) > ERROR && fabs(py) > ERROR )
        {
            if (SDL_RenderDrawPoint(pantalla.renderer, cx - px, cy - py)==-1) error_pantalla();
            if (SDL_RenderDrawPoint(pantalla.renderer, cx - py, cy - px)==-1) error_pantalla();
        }

        error += py;
        ++py;
        error += py;

        if (error >= 0)
        {
            --px;
            error -= px;
            error -= px;
        }
    }
}


static unsigned short int letters[95][13] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},// space :32
    {0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},// ! :33
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36},
    {0x00, 0x00, 0x00, 0x66, 0x66, 0xff, 0x66, 0x66, 0xff, 0x66, 0x66, 0x00, 0x00},
    {0x00, 0x00, 0x18, 0x7e, 0xff, 0x1b, 0x1f, 0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18},
    {0x00, 0x00, 0x0e, 0x1b, 0xdb, 0x6e, 0x30, 0x18, 0x0c, 0x76, 0xdb, 0xd8, 0x70},
    {0x00, 0x00, 0x7f, 0xc6, 0xcf, 0xd8, 0x70, 0x70, 0xd8, 0xcc, 0xcc, 0x6c, 0x38},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x0c, 0x0e},
    {0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c},
    {0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30},
    {0x00, 0x00, 0x00, 0x00, 0x99, 0x5a, 0x3c, 0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x00, 0x00},
    {0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03},
    {0x00, 0x00, 0x3c, 0x66, 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c},
    {0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18},
    {0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0xe7, 0x7e},
    {0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e},
    {0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xff, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c},
    {0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
    {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
    {0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03, 0x03, 0xff},
    {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
    {0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x03, 0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
    {0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06},
    {0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60},
    {0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0c, 0x06, 0x03, 0xc3, 0xc3, 0x7e},
    {0x00, 0x00, 0x3f, 0x60, 0xcf, 0xdb, 0xd3, 0xdd, 0xc3, 0x7e, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
    {0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
    {0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
    {0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc},
    {0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
    {0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff},
    {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
    {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},
    {0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06},
    {0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3},
    {0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
    {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3},
    {0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3},
    {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e},
    {0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
    {0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c},
    {0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
    {0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e},
    {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff},
    {0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3,0xc3, 0xc3, 0xc3, 0xc3},
    {0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
    {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
    {0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff},
    {0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c},
    {0x00, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60},
    {0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18},
    {0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x30, 0x70},
    {0x00, 0x00, 0x7f, 0xc3, 0xc3, 0x7f, 0x03, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
    {0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03, 0x03, 0x03},
    {0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x33, 0x1e},
    {0x7e, 0xc3, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0},
    {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00},
    {0x38, 0x6c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x00},
    {0x00, 0x00, 0xc6, 0xcc, 0xf8, 0xf0, 0xd8, 0xcc, 0xc6, 0xc0, 0xc0, 0xc0, 0xc0},
    {0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78},
    {0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xfe, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00},
    {0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00},
    {0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x00},
    {0x00, 0x00, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xc3, 0xe7, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00},
    {0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x0f, 0x18, 0x18, 0x18, 0x38, 0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    {0x00, 0x00, 0xf0, 0x18, 0x18, 0x18, 0x1c, 0x0f, 0x1c, 0x18, 0x18, 0x18, 0xf0},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00}  // :126
};

void dibuja_texto(char *st, int x, int y)
{
    int i = 0;
    if (SDL_SetRenderDrawColor(pantalla.renderer, pantalla.strokeColor.r, pantalla.strokeColor.g, pantalla.strokeColor.b, pantalla.strokeColor.a)==-1) error_pantalla();
    while (st[i])
    {
        int letra = st[i];
        if ( letra < ' ' ) letra = ' ';
        if ( letra > 126 ) letra = ' ';
        letra = letra - ' ';
        for ( int fila = 0 ; fila < 13 ; fila++ )
        {
            int mascara = 128;
            for ( int columna = 0 ; columna < 8 ; columna++ )
            {
                if (mascara & letters[letra][fila])
                {
                    if (SDL_RenderDrawPoint(pantalla.renderer, x+i*9+columna,y+13-fila)==-1) error_pantalla();
                }
                mascara /= 2;
            }
        }
        i++;
    }
}

#define MAX(a,b) (a>b?a:b)

char *lee_texto(char *titulo) {

    int x = pantalla.width/4;
    int y = pantalla.height/2 - 10;
    int max = 1024;
	SDL_Event event;
	char *text = malloc(sizeof(char)*max);
	char *msg = malloc(sizeof(char)*(max+1));
    int length_box = pantalla.width/2;
    int chars_box = length_box / 9;
	int length = 0;
	int quit = 0;
	int cursor = 0;
    int msg_start = 0;
	int tick = 0;
    SDL_PumpEvents();


	while ( SDL_PollEvent( &event ) )
		;
    SDL_StartTextInput();
	while (!quit) {
	 	/* Comprobamos todos los eventos producidos hasta el momento */
		while( SDL_PollEvent( &event ) ) {

		    switch( event.type ) {
		    	// Evento de tipo pulsación de tecla
		    	case SDL_KEYDOWN:
		    		// Tecla de borrado
			    	if (event.key.keysym.sym == SDLK_BACKSPACE && length > 0  && cursor > 0) {
			    		for ( int i = cursor ; i < length ; i++ ) {
			    			text[i-1] = text[i];
			    		}
			    		length--;
			    		text[length] = '\0';
			    		cursor--;
			    	}
			    	// Tecla return hace finalizar la entrada de texto y devolver lo que haya hasta el momento
			    	if (event.key.keysym.sym == SDLK_RETURN) {
			    		quit = 1;
			    	}
			    	// Tecla escape hace cancelar la entrada de texto
			    	if (event.key.keysym.sym == SDLK_ESCAPE) {
			    		quit = 1;
			    		length = 0;
			    	}
			    	// La flecha izquierda mueve el cursor a la izquierda
			    	if (event.key.keysym.sym == SDLK_LEFT && cursor > 0) {
			    		cursor--;
			    	}
			    	// La flecha derecha mueve el cursor a la derecha
			    	if (event.key.keysym.sym == SDLK_RIGHT && cursor < length) {
			    		cursor++;
			    	}
			    	break;
			    // Evento de tipo inserción de texto
                case SDL_TEXTINPUT:
                	{
	                	// Contar letras del textinput
	                	int eventlength = 0;
	                	char *textptr = event.text.text;
	                	for ( char *iptr = event.text.text ; *iptr ; iptr++ ) eventlength++;
	                	if ( eventlength > 1 ) textptr++;
	                    // Insertar texto en la posición del cursor sólo si cabe
	                    if ( *textptr > 0 && length < max ) {
	                    	/* Si el cursor no está al final primero desplazamos las letras situadas a
	                    	continuación del cursor un espacio a la derecha */
		                    if (cursor < length) {
		                    	for ( int i = length ; i >= cursor ; i--) {
		                    		text[i+1] = text[i];
		                    	}
		                    }
		                    // Después añadimos el texto a incluir filtrando los caracteres especiales
		                    for ( char *iptr = textptr ; *iptr ; iptr++) {
		                    	text[cursor] = *iptr;
						        if (*iptr == -62 || *iptr == -61) {
						        	iptr++;
						        	cursor--;
						        }
		                    	length++;
		                    	cursor++;
		                    }
	                    }
                	}
                    break;
		        case SDL_QUIT:
		            quit = 1;
		            length = 0;
		            pantalla.active = 0;
		            break;

		        default:
 		            break;
		    }
		}
        if ( cursor - msg_start > chars_box ) {
            msg_start++;
        }
        if ( cursor - msg_start < 0 ) {
            msg_start--;
        }
		for ( int i = msg_start ; i < length && i-msg_start < chars_box ; i++ ) {
			msg[i-msg_start] = text[i];
		}
        msg[length - msg_start] = '\0';

        rellena_fondo(128,128,128,255);
		color_relleno(192,192,192, 255);
		color_trazo(64,64,64,255);
		dibuja_rectangulo( x,y, length_box+1, 40 );

		color_relleno(255,255,255, 255);
		color_trazo(128,128,128, 255);
		dibuja_rectangulo(x+1,y+20, length_box-1, 19);
		color_trazo(0,0,0, 255);
		dibuja_texto(titulo, x+4,y+2);
		color_trazo(0,0,0, 255);

		dibuja_texto(msg, x+4,y+22);

		tick++;
		if (tick < 20) {
			dibuja_linea(x + 4 + (cursor-msg_start)*9 , y + 33 + 1, x + 4 + (cursor-msg_start)*9 , y + 23 );
		}
		if (tick == 25) {
			tick = 0;
		}
		actualiza_pantalla();
		espera(40);
	}
    SDL_StopTextInput();
    char *output = NULL;
    if (length > 0) {
	    output = malloc(sizeof(char)*(length+1));
		for ( int i = 0 ; i < length ; i++ ) {
			output[i] = text[i];
		}
	    output[length] = '\0';
    }
    free(msg);
    free(text);
	return output;
}

int anchura_imagen(Imagen imagen)
{
    return imagen->surface->w;
}

int altura_imagen(Imagen imagen)
{
    return imagen->surface->h;
}

Imagen lee_imagen(char *fichero, int transparencia)
{
    SDL_Surface *surface = SDL_LoadBMP(fichero);
    if (surface == NULL) {
    	printf( "Error leyendo el fichero '%s'.\n1) Comprueba que dicho fichero realmente exista.\n2) Si el fichero existe probablemente no se corresponda con el formato BMP sin comprimir.\n\n", fichero );
	    exit(EXIT_FAILURE);
    }

    if (transparencia)
    {
        if (surface->format->palette)
        {
            if (SDL_SetColorKey(surface, SDL_TRUE, *(Uint8 *) surface->pixels)==-1) error_pantalla();
        }
        else
        {
            switch (surface->format->BitsPerPixel)
            {
                case 15: if (SDL_SetColorKey(surface, SDL_TRUE, (*(Uint16 *) surface->pixels) & 0x00007FFF)==-1) error_pantalla();
                         break;
                case 16: if (SDL_SetColorKey(surface, SDL_TRUE, *(Uint16 *) surface->pixels)==-1) error_pantalla();
                         break;
                case 24: if (SDL_SetColorKey(surface, SDL_TRUE, (*(Uint32 *) surface->pixels) & 0x00FFFFFF)==-1) error_pantalla();
                         break;
                case 32: if (SDL_SetColorKey(surface, SDL_TRUE, *(Uint32 *) surface->pixels)==-1) error_pantalla();
                         break;
            }
        }
    }

    Imagen imagen = calloc(1, sizeof(struct ImagenRep));

    // Guardamos la superficie pero en convirtiÃ©ndola primero al formato que hemos usado para crear la ventana
    SDL_PixelFormat *pixelformat = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
    if (pixelformat==NULL) error_pantalla();

    imagen->surface = SDL_ConvertSurface(surface, pixelformat, 0);
    if (imagen->surface==NULL) error_pantalla();

    // Hack para que funcione transparencia en Arch

    if (SDL_HasColorKey(surface) && !SDL_HasColorKey(imagen->surface)){
        if (imagen->surface->format->palette)
        {
            if (SDL_SetColorKey(imagen->surface, SDL_TRUE, *(Uint8 *) imagen->surface->pixels)==-1) error_pantalla();
        }
        else
        {
            switch (imagen->surface->format->BitsPerPixel)
            {
                case 15: if (SDL_SetColorKey(imagen->surface, SDL_TRUE, (*(Uint16 *) imagen->surface->pixels) & 0x00007FFF)==-1) error_pantalla();
                         break;
                case 16: if (SDL_SetColorKey(imagen->surface, SDL_TRUE, *(Uint16 *) imagen->surface->pixels)==-1) error_pantalla();
                         break;
                case 24: if (SDL_SetColorKey(imagen->surface, SDL_TRUE, (*(Uint32 *) imagen->surface->pixels) & 0x00FFFFFF)==-1) error_pantalla();
                         break;
                case 32: if (SDL_SetColorKey(imagen->surface, SDL_TRUE, *(Uint32 *) imagen->surface->pixels)==-1) error_pantalla();
                         break;
            }
        }
    }

    //SDL_SetColorKey(imagen->surface, SET_TRANSPARENT_PIXEL, SDLGetColorKey(surface));

    imagen->texture = SDL_CreateTextureFromSurface(pantalla.renderer,imagen->surface);
    SDL_FreeFormat(pixelformat);

    // Liberamos la superficie original
    SDL_FreeSurface(surface);
    return imagen;
}

void libera_imagen(Imagen imagen)
{
    SDL_FreeSurface(imagen->surface);
    SDL_DestroyTexture(imagen->texture);
    free(imagen);
}

void dibuja_imagen(Imagen imagen, int x, int y, int w, int h)
{

    SDL_Rect rSalida;
    rSalida.x = x;
    rSalida.y = y;
    rSalida.w = w;
    rSalida.h = h;

    SDL_Point Centro;
    Centro.x = w/2;
    Centro.y = h/2;
    SDL_RenderCopyEx(pantalla.renderer,imagen->texture,NULL,&rSalida,0,&Centro, SDL_FLIP_NONE);
}

void dibuja_imagen_transformada(Imagen imagen, int x, int y, int w, int h, double angulo, SDL_RendererFlip volteo)
{
    SDL_Rect rSalida;
    rSalida.x = x;
    rSalida.y = y;
    rSalida.w = w;
    rSalida.h = h;

    SDL_Point Centro;
    Centro.x = w/2;
    Centro.y = h/2;

    SDL_RenderCopyEx(pantalla.renderer,imagen->texture,NULL,&rSalida,angulo,&Centro,volteo);
}


