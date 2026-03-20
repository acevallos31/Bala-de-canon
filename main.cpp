#include "graphito.h"
using namespace graphito;

int main() {
    VDefine(900, 600, "Bala de Canon");

    int tecla = TC_NINGUNA;

    while (tecla != TC_ESCAPE) {
        VLimpia();

        FormatoRelleno(ER_SOLIDO, CL_GRIS);
        FormatoBorde(EB_CONTINUO, 1, CL_BLANCO);
        Rectangulo(50, 50, 850, 550);

        TFormato("Arial", 24, 0, 0, CL_BLANCO);
        TMostrar(100, 100, 400, 40, "Proyecto Bala de Canon");

        TFormato("Arial", 18, 0, 0, CL_AMARILLO);
        TMostrar(100, 140, 400, 40, "Presione ESC para salir");

        tecla = Tecla();
        Espera(200);
    }

    VCierra();
    return 0;
}
