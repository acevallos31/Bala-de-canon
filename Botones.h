#include "graphito.h"
#include <iostream>
using namespace std;
using namespace graphito;

enum {EBT_PLANO, EBT_REDONDEADO, EBT_3D};


class Boton{
private:
    int x, y;       //coordenadas del boton, sup-izq
    int ancho,alto; //dimensiones del boton
    int color;      //Color del boton
    string etiqueta;//el texto en el boton
    bool visible;
    int estilo;
    bool presionado;
    void actualizar();
public:
    Boton(int _x=100, int _y=100, int _ancho=100, int _alto=50,int _estilo=EBT_PLANO ,string _etiqueta="---");
    void mostrar();
    void ocultar();
    void recolor(int _color);
    void mover(int _x,int _y);
    void redim(int _ancho, int _alto);
    void reetiquetar(string _eqtiqueta);
    bool ratonSobre();
    bool click();

};
