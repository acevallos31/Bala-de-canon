#include "graphito.h"
#include "Canon.h"
#include "Botones.h"

using namespace graphito;

int main(){

    VDefine(900,600,"Bala de Canon");


    Canon c1(70,450,110);

    c1.mostrar();

     Boton br(280,500,100,50,EBT_3D,"Fuego");
     Boton ba(420,500,100,50,EBT_3D,"Fin");
     Boton bv(550,500,100,50,EBT_3D,"+");
     Boton bs(700,500,100,50,EBT_3D,"-");
     Espera(100);
     br.mostrar();
     ba.mostrar();
     bv.mostrar();
     bs.mostrar();

        if(br.click()){
            Mensaje("hola");
        }


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
