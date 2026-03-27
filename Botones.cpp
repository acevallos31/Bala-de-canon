#include "graphito.h";
#include <iostream>
#include "botones.h"

using namespace std;
using namespace graphito;


Boton::Boton(int _x, int _y, int _ancho, int _alto,int _estilo, string _etiqueta){
    x = _x;
    y = _y;
    ancho = _ancho;
    alto = _alto;
    estilo = _estilo;
    etiqueta = _etiqueta;
    color = CL_PLATA;
    visible = false;
    presionado = false;
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
                case EBT_3D:
                    FormatoBorde(EB_CONTINUO,0);
                    Rectangulo(x,y,x+ancho,y+alto);
                    FormatoBorde(EB_CONTINUO,2,(presionado)?CL_NEGRO:CL_BLANCO);
                    Linea(x,y,x,y+alto);
                    Linea(x,y,x+ancho,y);
                    FormatoBorde(EB_CONTINUO,2,(presionado)?CL_BLANCO:CL_NEGRO);
                    Linea(x,y+alto,x+ancho,y+alto);
                    Linea(x+ancho,y,x+ancho,y+alto);
        }

        int tamFuente = alto - 8;
        if(tamFuente < 12) tamFuente = 12;
        if(tamFuente > 30) tamFuente = 30;
        TFormato("Arial",tamFuente,0,FT_NEGRITA,CL_NEGRO);
        TJustificar(JT_CENTRO);
        TMostrar(x + ancho/2, y + alto/2, ancho, alto, etiqueta);

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
bool Boton::ratonSobre(){
    if(visible){
        int rx, ry;
        Raton(rx,ry);
        return((rx>=x)&&(rx<=x+ancho)&&(ry>=y)&&(ry<=y+alto));
    }else
        return false;

}

bool Boton::click(){
    if(!visible)
        return false;

    // Al soltar el mouse, el boton siempre vuelve a estado normal
    if(!RatonBotonIzq() && presionado){
        presionado = false;
        actualizar();
    }

    if(ratonSobre()){
        if(RatonBotonIzq() && !presionado){
            presionado = true;
            actualizar();
            return true;
        }
    }

    return false;
}
