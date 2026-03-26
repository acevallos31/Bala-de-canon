#include "Bala.h"
#include <math.h>

using namespace graphito;

const float GRAVEDAD = 0.4f;
const int   RADIO    = 8;

Bala::Bala() {
    x      = 0;
    y      = 0;
    vx     = 0;
    vy     = 0;
    activa = false;
}

void Bala::disparar(float origenX, float origenY, int angulo, float velocidad) {
    if (activa) return;   // no disparar si ya hay una bala en vuelo

    double rad = angulo * M_PI / 180.0;

    x  = origenX;
    y  = origenY;
    vx = (float)(cos(rad) * velocidad);
    vy = (float)(sin(rad) * velocidad);

    activa = true;
}

void Bala::mover() {
    if (!activa) return;

    ocultar();   // borra en posicion vieja

    x  += vx;
    y  -= vy;    // y crece hacia abajo en pantalla
    vy -= GRAVEDAD;

    // Detener si sale de la pantalla
    if (x < 15 || x > 885 || y > 455 || y < 15) {
    activa = false;
    return;
}

    mostrar();   // dibuja en nueva posicion
}

void Bala::mostrar() {
    FormatoBorde(EB_CONTINUO, 1, CL_NARANJA);
    FormatoRelleno(ER_SOLIDO, CL_NARANJA);
    Circulo((int)x, (int)y, RADIO);
}

void Bala::ocultar() {
    FormatoBorde(EB_CONTINUO, 1, CL_NEGRO);
    FormatoRelleno(ER_SOLIDO, CL_NEGRO);
    Circulo((int)x, (int)y, RADIO);
}

bool Bala::estaActiva() {
    return activa;
}

float Bala::getX() {
    return x;
}

float Bala::getY() {
    return y;
}

int Bala::getRadio() {
    return RADIO;
}

void Bala::apagar() {
    if(activa){
        ocultar();
        activa = false;
    }
}
