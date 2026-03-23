#include "Canon.h"
#include <math.h>

using namespace graphito;

Canon::Canon(int _x, int _y, int _largo){
    x = _x;
    y = _y;
    largo = _largo;
    angulo = 35;
    visible = false;
}

void Canon::mostrar(){
    if(!visible){

        double rad = angulo * M_PI / 180.0;

        // punto de salida del tubo un poco mas arriba del centro de la base
        int bx = x;
        int by = y - 18;

        // tubo grueso
        int x1 = bx + 10 * cos(rad + M_PI/2);
        int y1 = by - 10 * sin(rad + M_PI/2);

        int x2 = bx + 10 * cos(rad - M_PI/2);
        int y2 = by - 10 * sin(rad - M_PI/2);

        int xf = bx + largo * cos(rad);
        int yf = by - largo * sin(rad);

        int x3 = xf + 10 * cos(rad - M_PI/2);
        int y3 = yf - 10 * sin(rad - M_PI/2);

        int x4 = xf + 10 * cos(rad + M_PI/2);
        int y4 = yf - 10 * sin(rad + M_PI/2);

        // 1. dibujar primero el tubo
        FormatoBorde(EB_CONTINUO, 2, CL_LIMON);
        FormatoRelleno(ER_SOLIDO, CL_LIMON);
        PoligonoIrregular({
            {x1,y1},
            {x2,y2},
            {x3,y3},
            {x4,y4}
        });

        // 2. dibujar despues la base para tapar la parte interna del tubo
        FormatoBorde(EB_CONTINUO, 2, CL_AMARILLO);
        FormatoRelleno(ER_SOLIDO, CL_AMARILLO);
        Sector(x, y, 55, 0, 180);

        visible = true;
    }
}

void Canon::ocultar(){
    if(visible){

        double rad = angulo * M_PI / 180.0;

        int bx = x;
        int by = y - 18;

        int x1 = bx + 10 * cos(rad + M_PI/2);
        int y1 = by - 10 * sin(rad + M_PI/2);

        int x2 = bx + 10 * cos(rad - M_PI/2);
        int y2 = by - 10 * sin(rad - M_PI/2);

        int xf = bx + largo * cos(rad);
        int yf = by - largo * sin(rad);

        int x3 = xf + 10 * cos(rad - M_PI/2);
        int y3 = yf - 10 * sin(rad - M_PI/2);

        int x4 = xf + 10 * cos(rad + M_PI/2);
        int y4 = yf - 10 * sin(rad + M_PI/2);

        // borrar primero el tubo
        FormatoBorde(EB_CONTINUO, 2, CL_NEGRO);
        FormatoRelleno(ER_SOLIDO, CL_NEGRO);
        PoligonoIrregular({
            {x1,y1},
            {x2,y2},
            {x3,y3},
            {x4,y4}
        });

        // borrar despues la base
        Sector(x, y, 55, 0, 180);

        visible = false;
    }
}

void Canon::mover(int _x, int _y){
    bool v = visible;

    if(v)
        ocultar();

    x = _x;
    y = _y;

    if(v)
        mostrar();
}

void Canon::rotar(int _angulo){
    bool v = visible;

    if(v)
        ocultar();

    angulo = _angulo;

    if(angulo < 0)
        angulo = 0;

    if(angulo > 90)
        angulo = 90;

    if(v)
        mostrar();
}

int Canon::getX(){
    return x;
}

int Canon::getY(){
    return y;
}

int Canon::getAngulo(){
    return angulo;
}
