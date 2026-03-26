#include "graphito.h"
#include "Canon.h"
#include "Bala.h"
#include "Blanco.h"
#include "Botones.h"

using namespace graphito;

const int JUEGO_IZQ = 15;
const int JUEGO_ARR = 15;
const int JUEGO_DER = 885;
const int JUEGO_ABA = 455;

const int PANEL_IZQ = 0;
const int PANEL_ARR = 460;
const int PANEL_DER = 899;
const int PANEL_ABA = 599;

const int CONTROL_X = 210;
const int CONTROL_Y = 485;

void dibujarEscenarioBase(){
    FormatoBorde(EB_CONTINUO, 1, CL_PLATA);
    FormatoRelleno(ER_SOLIDO, CL_GRIS);
    Rectangulo(0, 0, 899, 599);

    FormatoBorde(EB_CONTINUO, 2, CL_GRIS);
    FormatoRelleno(ER_SOLIDO, CL_NEGRO);
    Rectangulo(JUEGO_IZQ, JUEGO_ARR, JUEGO_DER, JUEGO_ABA);
}

void dibujarCajaValor(int x, int y, int ancho, int alto, const std::string& texto){
    FormatoBorde(EB_CONTINUO, 2, CL_GRIS);
    FormatoRelleno(ER_SOLIDO, CL_BLANCO);
    Rectangulo(x, y, x + ancho, y + alto);

    TFormato("Arial", 18, 0, FT_NEGRITA, CL_NEGRO);
    TJustificar(JT_CENTRO);
    TMostrar(x, y + 7, ancho, alto - 6, texto);
}

void dibujarPanelControles(){
    FormatoBorde(EB_CONTINUO, 2, CL_GRIS);
    FormatoRelleno(ER_SOLIDO, CL_PLATA);
    Rectangulo(PANEL_IZQ, PANEL_ARR, PANEL_DER, PANEL_ABA);

    TFormato("Arial", 16, 0, FT_NEGRITA, CL_NEGRO);
    TJustificar(JT_IZQUIERDA);
    TMostrar(CONTROL_X + 15, CONTROL_Y - 2, 120, 20, "ANGULO:");
    TMostrar(CONTROL_X + 175, CONTROL_Y - 2, 150, 20, "VELOCIDAD:");
}

void actualizarPanelControles(int angulo, int velocidad){
    dibujarCajaValor(CONTROL_X + 10, CONTROL_Y + 26, 105, 40, std::to_string(angulo));
    dibujarCajaValor(CONTROL_X + 180, CONTROL_Y + 26, 125, 40, std::to_string(velocidad) + " px/s");

    FormatoBorde(EB_CONTINUO, 1, CL_PLATA);
    FormatoRelleno(ER_SOLIDO, CL_PLATA);
    Rectangulo(CONTROL_X + 8, CONTROL_Y + 70, CONTROL_X + 140, CONTROL_Y + 90);
    TFormato("Arial", 12, 0, FT_NEGRITA, CL_NEGRO);
    TJustificar(JT_IZQUIERDA);
    TMostrar(CONTROL_X + 10, CONTROL_Y + 72, 120, 14, "Flechas arriba/abajo");
}

void actualizarMarcador(int disparos, int aciertos, int puntos){
    FormatoBorde(EB_CONTINUO, 1, CL_NEGRO);
    FormatoRelleno(ER_SOLIDO, CL_NEGRO);
    Rectangulo(640, 80, 860, 170);

    TFormato("Consolas", 18, 0, FT_NEGRITA, CL_BLANCO);
    TJustificar(JT_IZQUIERDA);
    TMostrar(650, 92, 190, 22, "Disparos: " + std::to_string(disparos));
    TMostrar(650, 118, 190, 22, "Blancos:  " + std::to_string(aciertos));
    TMostrar(650, 144, 190, 22, "Puntos:   " + std::to_string(puntos));
}

int main(){

    VDefine(900,600,"Bala de Canon");

    Canon c1(70, 450, 110);
    Bala  bala;
    Blanco blanco;
    float velocidad = 15.0f;

    Boton bv(CONTROL_X + 320, CONTROL_Y + 18, 38, 30, EBT_3D, "+");
    Boton bs(CONTROL_X + 320, CONTROL_Y + 56, 38, 30, EBT_3D, "-");
    Boton br(CONTROL_X + 385, CONTROL_Y + 14, 95, 72, EBT_3D, "Fuego");
    Boton ba(CONTROL_X + 490, CONTROL_Y + 14, 85, 72, EBT_3D, "Fin");
    Espera(100);
    dibujarEscenarioBase();
    dibujarPanelControles();
    actualizarPanelControles(35, (int)velocidad);
    actualizarMarcador(0, 0, 0);
    blanco.reubicarAleatorio();
    blanco.mostrar();
    c1.mostrar();
    br.mostrar();
    ba.mostrar();
    bv.mostrar();
    bs.mostrar();

    int tecla  = TC_NINGUNA;
    int angulo = 35;
    int disparos = 0;
    int puntos = 0;
    int aciertos = 0;
    int fallosConsecutivos = 0;
    bool impactoEnDisparoActual = false;
    int alturasCanon[3] = {450, 410, 370};
    int indiceAlturaCanon = 0;

    //while(tecla != TC_ESCAPE){
        bool actualizarPanel = false;
        bool actualizarInfo = false;

        tecla = Tecla();

        if(tecla == TC_ARRIBA){
            angulo += 5;
            c1.rotar(angulo);
            angulo = c1.getAngulo();
            actualizarPanel = true;
        }

        if(tecla == TC_ABAJO){
            angulo -= 5;
            c1.rotar(angulo);
            angulo = c1.getAngulo();
            actualizarPanel = true;
        }

        // Ajustar velocidad con botones + y -
        if(bv.click()){
            velocidad += 1.0f;

            if(velocidad > 30.0f)
                velocidad = 30.0f;

            actualizarPanel = true;
            actualizarInfo = true;
        }

        if(bs.click()){
            velocidad -= 1.0f;

            if(velocidad < 1.0f)
                velocidad = 1.0f;

            actualizarPanel = true;
            actualizarInfo = true;
        }

        // Disparo con ESPACIO o boton Fuego
        if((tecla == TC_ESPACIO || br.click()) && !bala.estaActiva() && disparos < 20){
            bala.disparar(c1.getPuntaX(), c1.getPuntaY(), c1.getAngulo(), velocidad);
            impactoEnDisparoActual = false;
            disparos++;
            actualizarInfo = true;

            // Regla: cada 2 disparos cambia la altura del canon
            if(disparos % 2 == 0){
                indiceAlturaCanon = (indiceAlturaCanon + 1) % 3;
                c1.mover(c1.getX(), alturasCanon[indiceAlturaCanon]);
                actualizarInfo = true;
            }
        }

        // Fin con boton
        if(ba.click()){
            tecla = TC_ESCAPE;
        }

        bool estabaActiva = bala.estaActiva();

        // Detectar impacto antes de mover
        if(bala.estaActiva() && blanco.impacto((int)bala.getX(), (int)bala.getY(), bala.getRadio())){
            impactoEnDisparoActual = true;
            bala.apagar();
            blanco.reubicarAleatorio();
            puntos += 10;
            aciertos++;
            fallosConsecutivos = 0;
            actualizarInfo = true;
        }

        // Mover bala cada frame
        bala.mover();

        // Detectar impacto despues de mover
        if(bala.estaActiva() && blanco.impacto((int)bala.getX(), (int)bala.getY(), bala.getRadio())){
            impactoEnDisparoActual = true;
            bala.apagar();
            blanco.reubicarAleatorio();
            puntos += 10;
            aciertos++;
            fallosConsecutivos = 0;
            actualizarInfo = true;
        }

        // Si la bala termino su vuelo sin impacto, cuenta como fallo
        if(estabaActiva && !bala.estaActiva() && !impactoEnDisparoActual){
            puntos -= 2;
            fallosConsecutivos++;
            actualizarInfo = true;

            if(fallosConsecutivos >= 3){
                blanco.reubicarAleatorio();
                fallosConsecutivos = 0;
                actualizarInfo = true;
            }
        }

        // Fin automatico al completar 20 disparos
        if(disparos >= 20 && !bala.estaActiva()){
            Mensaje("Fin de partida. Puntos: " + std::to_string(puntos) + " | Aciertos: " + std::to_string(aciertos));
            tecla = TC_ESCAPE;
        }

        if(actualizarPanel)
            actualizarPanelControles(angulo, (int)velocidad);

        if(actualizarInfo){
            actualizarMarcador(disparos, aciertos, puntos);
            blanco.ocultar();
            blanco.mostrar();
        }

        Espera(5000);
    }

  //  VCierra();
   // return 0;
//}
