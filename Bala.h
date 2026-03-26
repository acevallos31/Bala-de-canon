#pragma once
#include "graphito.h"

using namespace graphito;

class Bala {
private:
    float x, y;     // posicion actual
    float vx, vy;   // velocidad en x e y
    bool activa;    // si la bala esta en vuelo

public:
    Bala();

    // Dispara desde la punta del canon con angulo en grados y velocidad dada
    void disparar(float origenX, float origenY, int angulo, float velocidad);

    // Actualiza posicion con fisica (llamar en cada frame)
    void mover();

    // Dibuja la bala en pantalla
    void mostrar();

    // Borra la bala de pantalla
    void ocultar();

    // Retorna si la bala esta en vuelo
    bool estaActiva();

    // Permite consultar posicion para colisiones
    float getX();
    float getY();
    int getRadio();

    // Detiene la bala (por ejemplo, cuando impacta)
    void apagar();
};
