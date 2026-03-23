#include "graphito.h"

using namespace graphito;

class Canon {
private:
    int x, y;
    int largo;
    int angulo;
    bool visible;

public:
    Canon(int _x=100, int _y=400, int _largo=80);

    void mostrar();
    void ocultar();
    void mover(int _x, int _y);
    void rotar(int _angulo);

    int getX();
    int getY();
    int getAngulo();
};
