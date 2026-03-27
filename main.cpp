#include "graphito.h"
#include "Canon.h"
#include "Bala.h"
#include "Blanco.h"
#include "Botones.h"

using namespace graphito;

const int JUEGO_IZQ = 20;
const int JUEGO_ARR = 20;
const int JUEGO_DER = 1180;
const int JUEGO_ABA = 605;

const int PANEL_IZQ = 0;
const int PANEL_ARR = 615;
const int PANEL_DER = 1199;
const int PANEL_ABA = 799;

const int CONTROL_X = 280;
const int CONTROL_Y = 640;

void dibujarEscenarioBase(){
    FormatoBorde(EB_CONTINUO, 1, CL_PLATA);
    FormatoRelleno(ER_SOLIDO, CL_GRIS);
    Rectangulo(0, 0, 1199, 799);

    FormatoBorde(EB_CONTINUO, 2, CL_GRIS);
    FormatoRelleno(ER_SOLIDO, CL_NEGRO);
    Rectangulo(JUEGO_IZQ, JUEGO_ARR, JUEGO_DER, JUEGO_ABA);
}

void dibujarCajaValor(int x, int y, int ancho, int alto, const std::string& texto){
    FormatoBorde(EB_CONTINUO, 2, CL_GRIS);
    FormatoRelleno(ER_SOLIDO, CL_BLANCO);
    Rectangulo(x, y, x + ancho, y + alto);

    // Limpiar interior para evitar residuos visuales al actualizar el valor
    FormatoBorde(EB_CONTINUO, 0, CL_BLANCO);
    FormatoRelleno(ER_SOLIDO, CL_BLANCO);
    Rectangulo(x + 2, y + 2, x + ancho - 2, y + alto - 2);

    TFormato("Consolas", 24, 0, FT_NEGRITA, CL_NEGRO);
    TJustificar(JT_IZQUIERDA);
    TMostrar(x + 10, y + 9, ancho - 16, alto - 12, texto);
}

void dibujarCampoControl(const std::string& etiqueta, const std::string& valor,
                        int xEtiqueta, int yEtiqueta,
                        int xCaja, int yCaja, int anchoCaja, int altoCaja){
    TFormato("Arial", 18, 0, FT_NEGRITA, CL_NEGRO);
    TJustificar(JT_IZQUIERDA);
    TMostrar(xEtiqueta, yEtiqueta, 180, 22, etiqueta);
    dibujarCajaValor(xCaja, yCaja, anchoCaja, altoCaja, valor);
}

void dibujarPanelControles(){
    FormatoBorde(EB_CONTINUO, 2, CL_GRIS);
    FormatoRelleno(ER_SOLIDO, CL_PLATA);
    Rectangulo(PANEL_IZQ, PANEL_ARR, PANEL_DER, PANEL_ABA);

    // Texto estatico: solo se dibuja una vez
    TFormato("Arial", 12, 0, FT_NEGRITA, CL_NEGRO);
    TJustificar(JT_IZQUIERDA);
    TMostrar(CONTROL_X + 12, CONTROL_Y + 87, 160, 16, "Flechas arriba/abajo");
}

void actualizarPanelControles(int angulo, int velocidad){
    // Limpiar SOLO las zonas de texto/casillas, evitando columnas de botones
    FormatoBorde(EB_CONTINUO, 0, CL_PLATA);
    FormatoRelleno(ER_SOLIDO, CL_PLATA);
    Rectangulo(CONTROL_X, CONTROL_Y - 12, CONTROL_X + 145, CONTROL_Y + 73);
    Rectangulo(CONTROL_X + 182, CONTROL_Y - 12, CONTROL_X + 345, CONTROL_Y + 73);

    // Dibujar cada campo como unidad etiqueta+casilla+valor
    dibujarCampoControl("ANGULO:", std::to_string(angulo),
                        CONTROL_X + 10, CONTROL_Y - 8,
                        CONTROL_X + 10, CONTROL_Y + 20, 130, 50);
    dibujarCampoControl("VELOCIDAD:", std::to_string(velocidad) + " px/s",
                        CONTROL_X + 180, CONTROL_Y - 8,
                        CONTROL_X + 180, CONTROL_Y + 20, 160, 50);
}

void actualizarMarcador(int disparos, int aciertos, int puntos){
    // Dibujar marcador en el panel de controles, a la izquierda del angulo
    FormatoBorde(EB_CONTINUO, 0, CL_PLATA);
    FormatoRelleno(ER_SOLIDO, CL_PLATA);
    Rectangulo(15, PANEL_ARR + 8, 260, PANEL_ABA - 8);

    TFormato("Consolas", 22, 0, FT_NEGRITA, CL_NEGRO);
    TJustificar(JT_IZQUIERDA);
    TMostrar(20, PANEL_ARR + 16, 235, 26, "Disparos: " + std::to_string(disparos));
    TMostrar(20, PANEL_ARR + 55, 235, 26, "Blancos:  " + std::to_string(aciertos));
    TMostrar(20, PANEL_ARR + 94, 235, 26, "Puntos:   " + std::to_string(puntos));
}

int main(){

    VDefine(1200,800,"Bala de Canon");

    Canon c1(90, 580, 140);
    Bala  bala;
    Blanco blanco;
    float velocidad = 15.0f;

    Boton bv(CONTROL_X + 348, CONTROL_Y + 20, 32, 24, EBT_3D, "+");
    Boton bs(CONTROL_X + 348, CONTROL_Y + 46, 32, 24, EBT_3D, "-");
    Boton br(CONTROL_X + 500, CONTROL_Y + 20, 120, 50, EBT_3D, "Fuego");
    Boton ba(CONTROL_X + 640, CONTROL_Y + 20, 110, 50, EBT_3D, "Fin");
    // Botones + y - para el angulo (a la derecha de la caja de angulo)
    Boton bAngUp (CONTROL_X + 148, CONTROL_Y + 20, 32, 24, EBT_3D, "+");
    Boton bAngDn (CONTROL_X + 148, CONTROL_Y + 46, 32, 24, EBT_3D, "-");
    // Botones de velocidad rapida debajo de la caja de velocidad
    Boton bVel5 (CONTROL_X + 180, CONTROL_Y + 78, 48, 30, EBT_3D, "5");
    Boton bVel15(CONTROL_X + 233, CONTROL_Y + 78, 48, 30, EBT_3D, "15");
    Boton bVel30(CONTROL_X + 286, CONTROL_Y + 78, 48, 30, EBT_3D, "30");
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
    bAngUp.mostrar();
    bAngDn.mostrar();
    bVel5.mostrar();
    bVel15.mostrar();
    bVel30.mostrar();

    int tecla  = TC_NINGUNA;
    int angulo = 35;
    int disparos = 0;
    int puntos = 0;
    int aciertos = 0;
    int fallosConsecutivos = 0;
    bool impactoEnDisparoActual = false;
    int alturasCanon[3] = {580, 380, 180};
    int indiceAlturaCanon = 0;
    // Contadores para auto-repeat de botones + y -
    // 300ms de retardo inicial (30 frames), luego repite cada 80ms (8 frames)
    const int HOLD_RETARDO  = 30;
    const int HOLD_INTERVALO = 8;
    int holdV = 0;
    int holdS = 0;

    while(tecla != TC_ESCAPE){
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

        // Botones + y - del angulo
        if(bAngUp.click()){
            angulo += 5;
            c1.rotar(angulo);
            angulo = c1.getAngulo();
            actualizarPanel = true;
        }
        if(bAngDn.click()){
            angulo -= 5;
            c1.rotar(angulo);
            angulo = c1.getAngulo();
            actualizarPanel = true;
        }

        // Ajustar velocidad con botones + y - (click inicial)
        if(bv.click()){
            velocidad += 1.0f;
            if(velocidad > 30.0f) velocidad = 30.0f;
            actualizarPanel = true;
        }

        if(bs.click()){
            velocidad -= 1.0f;
            if(velocidad < 1.0f) velocidad = 1.0f;
            actualizarPanel = true;
        }

        // Auto-repeat: si se mantiene presionado + o -
        if(bv.ratonSobre() && RatonBotonIzq()){
            holdV++;
            if(holdV > HOLD_RETARDO && (holdV - HOLD_RETARDO) % HOLD_INTERVALO == 0){
                velocidad += 1.0f;
                if(velocidad > 30.0f) velocidad = 30.0f;
                actualizarPanel = true;
            }
        } else {
            holdV = 0;
        }

        if(bs.ratonSobre() && RatonBotonIzq()){
            holdS++;
            if(holdS > HOLD_RETARDO && (holdS - HOLD_RETARDO) % HOLD_INTERVALO == 0){
                velocidad -= 1.0f;
                if(velocidad < 1.0f) velocidad = 1.0f;
                actualizarPanel = true;
            }
        } else {
            holdS = 0;
        }

        // Botones de velocidad preestablecida
        if(bVel5.click()){
            velocidad = 5.0f;
            actualizarPanel = true;
        }
        if(bVel15.click()){
            velocidad = 15.0f;
            actualizarPanel = true;
        }
        if(bVel30.click()){
            velocidad = 30.0f;
            actualizarPanel = true;
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

        if(actualizarPanel){
            actualizarPanelControles(angulo, (int)velocidad);
        }

        if(actualizarInfo){
            actualizarMarcador(disparos, aciertos, puntos);
        }

        Espera(10);
    }

    VCierra();
    return 0;
}
