#include "graphito.h";
#include <iostream>
#include "botones.h"

using namespace std;
using namespace graphito;


Boton::Boton(int _x, int _y, int _ancho, int _alto, string _etiqueta){
    x = _x;
    y = _y;
    ancho = _ancho;
    alto = _alto;
    etiqueta = _etiqueta;
    color = CL_PLATA;
    estilo = EBT_REDONDEADO;
    visible = false;
}

void Boton::mostrar(){
    if(!visible){
        FormatoRelleno(ER_SOLIDO,color);
        switch(estilo){
            case EBT_PLANO:
                FormatoBorde(EB_CONTINUO,2,CL_NEGRO);
                Rectangulo(x,y,x+ancho,y+alto);
                break;
            case EBT_REDONDEADO:
                FormatoBorde(EB_CONTINUO,2,CL_NEGRO);
                RectanguloRedondeado(x,y,x+ancho,y+alto,30);
                break;
            //case EBT_3D:
        }



        TFormato("Arial",30,0,FT_NEGRITA,CL_NEGRO);
        TJustificar(JT_CENTRO);
        TMostrar(x+ancho/2,y+alto/2,ancho,alto,etiqueta);

        visible = true;
    }
}

void Boton::ocultar(){
    if(visible){
        FormatoBorde(EB_CONTINUO,2,CL_PLATA);
        FormatoRelleno(ER_SOLIDO,CL_PLATA);
        Rectangulo(x,y,x+ancho,y+alto);
        visible = false;
    }
}

void Boton::actualizar(){
    if(visible){
        visible = false;
        mostrar();
    }
}
void Boton::recolor(int _color){
    color = _color;
    actualizar();
}

void Boton::mover(int _x,int _y){
    bool esvisible = visible;

    if(esvisible)
        ocultar();

    x = _x;
    y = _y;

    if(esvisible)
        mostrar();
}

void Boton::redim(int _ancho,int _alto){
    bool esvisible = visible;

    if(esvisible)
        ocultar();

    ancho = _ancho;
    alto = _alto;

    if(esvisible)
        mostrar();
}

void Boton::reetiquetar(string _etiqueta){
    bool esvisible = visible;

    if(esvisible)
        ocultar();

    etiqueta = _etiqueta;

    if(esvisible)
        mostrar();
}
