#include "graphito.h"
#include "Canon.h"

using namespace graphito;

int main(){

    VDefine(900,600,"Bala de Canon");

    Canon c1(70,450,110);

    c1.mostrar();

    int tecla = TC_NINGUNA;
    int angulo = 35;

    while(tecla != TC_ESCAPE){

        tecla = Tecla();

        if(tecla == TC_ARRIBA){
            angulo += 5;
            c1.rotar(angulo);
        }

        if(tecla == TC_ABAJO){
            angulo -= 5;
            c1.rotar(angulo);
        }

        Espera(20);
    }

    VCierra();
    return 0;
}
