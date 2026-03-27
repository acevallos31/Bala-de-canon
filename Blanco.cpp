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

    // Mantener el blanco lejos del cañon y fuera de la zona baja dificil
    int minX = 420;
    int maxX = 1180 - radio - 20;

    int minY = 20 + radio;
    int maxY = 470 - radio;

    if(maxX <= minX) maxX = minX + 1;
    if(maxY <= minY) maxY = minY + 1;

    int nx = minX + rand() % (maxX - minX);
    int ny = minY + rand() % (maxY - minY);

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
