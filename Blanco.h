#pragma once

#include "graphito.h"

using namespace graphito;

class Blanco {
private:
    int x, y;
    int radio;
    bool visible;

public:
    Blanco(int _x=700, int _y=260, int _radio=28);

    void mostrar();
    void ocultar();
    void mover(int _x, int _y);
    void reubicarAleatorio();

    bool impacto(int px, int py, int margen=0);

    int getX();
    int getY();
    int getRadio();
};
