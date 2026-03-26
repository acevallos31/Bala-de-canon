#include "Blanco.h"

#include <stdlib.h>
#include <time.h>

using namespace graphito;

Blanco::Blanco(int _x, int _y, int _radio) {
    x = _x;
    y = _y;
    radio = _radio;
    visible = false;

    static bool inicializado = false;
    if(!inicializado){
        srand((unsigned)time(NULL));
        inicializado = true;
    }
}

void Blanco::mostrar() {
    if(!visible){
        // Diana simple: anillo rojo, centro blanco, punto rojo
        FormatoBorde(EB_CONTINUO, 2, CL_ROJO);
        FormatoRelleno(ER_SOLIDO, CL_ROJO);
        Circulo(x, y, radio);

        FormatoBorde(EB_CONTINUO, 1, CL_BLANCO);
        FormatoRelleno(ER_SOLIDO, CL_BLANCO);
        Circulo(x, y, radio - 9);

        FormatoBorde(EB_CONTINUO, 1, CL_ROJO);
        FormatoRelleno(ER_SOLIDO, CL_ROJO);
        Circulo(x, y, radio - 18);

        visible = true;
    }
}

void Blanco::ocultar() {
    if(visible){
        FormatoBorde(EB_CONTINUO, 2, CL_NEGRO);
        FormatoRelleno(ER_SOLIDO, CL_NEGRO);
        Circulo(x, y, radio);
        Circulo(x, y, radio - 9);
        Circulo(x, y, radio - 18);
        visible = false;
    }
}

void Blanco::mover(int _x, int _y) {
    bool v = visible;

    if(v)
        ocultar();

    x = _x;
    y = _y;

    if(v)
        mostrar();
}

void Blanco::reubicarAleatorio() {
    int ancho = VAncho();
    int alto  = VAlto();

    int minX = (ancho > 0) ? ancho / 2 : 450;
    int maxX = (ancho > 0) ? ancho - radio - 20 : 860;
    int minY = radio + 20;
    int maxY = (alto > 0) ? alto - radio - 140 : 420;

    if(maxX <= minX) maxX = minX + 1;
    if(maxY <= minY) maxY = minY + 1;

    int nx = minX + rand() % (maxX - minX + 1);
    int ny = minY + rand() % (maxY - minY + 1);

    mover(nx, ny);
}

bool Blanco::impacto(int px, int py, int margen) {
    int dx = px - x;
    int dy = py - y;
    int r  = radio + margen;
    return (dx * dx + dy * dy) <= (r * r);
}

int Blanco::getX() {
    return x;
}

int Blanco::getY() {
    return y;
}

int Blanco::getRadio() {
    return radio;
}
