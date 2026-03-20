// VERSION: 2.0


#ifndef _GRAPHITO_H_
#define _GRAPHITO_H_

#include <iostream>
#include <vector>
#include <array>

#ifndef GRAPHITO_SOURCE
#define main _main_
#endif

int _main_();

//------------------- Funciones del API -------------------

namespace graphito {
//------------------- Constantes -------------------------
enum {
  TC_ESCAPE,
  TC_IZQUIERDA, TC_DERECHA, TC_ARRIBA, TC_ABAJO,
  TC_F1, TC_F2, TC_F3, TC_F4, TC_F5, TC_F6, TC_F7, TC_F8, TC_F9, TC_F10, TC_F11, TC_F12,
  TC_ESPACIO,
  TC_ENTER,
  TC_RETROCESO,
  TC_TABULADOR,
  TC_NINGUNA
};


enum {          //---------- PALETA DE COLORES ----------
  CL_NEGRO, CL_ROJO, CL_VERDE, CL_AZUL, CL_AMARILLO, CL_MAGENTA, CL_CYAN, CL_BLANCO,
  CL_GRIS, CL_CAFE, CL_NARANJA, CL_SALMON, CL_ROSADO, CL_ROSADOFUERTE, CL_ORO,
  CL_LAVANDA, CL_VIOLETA, CL_MORADO, CL_LIMON, CL_OLIVO, CL_TURQUESA, CL_PLATA
};

enum {          //---------- ESTILOS DE BORDES -----------
    EB_CONTINUO, EB_RAYAS, EB_PUNTOS, EB_RAYAPUNTO, EB_RAYAPUNTOPUNTO,
};

enum{           //---------- ESTILOS DE RELLENO ----------
    ER_HORIZONTAL, ER_VERTICAL, ER_DIAGONALDERECHA, ER_DIAGONALIZQUIERDA,
    ER_CUADRICULADO, ER_DIAGONALCRUZADO, ER_SOLIDO, ER_NORELLENO
};

enum {          //---------- ESTILO DE FIGURA ---------
    EF_SOLOBORDE=1, EF_SOLORELLENO, EF_COMPLETO
};

enum{           //---------- ESTILOS DE JUSTIFICADO DE TEXTO ----------
    JT_IZQUIERDA, JT_CENTRO, JT_DERECHA
};

enum{           //---------- ESTILOS FORMATOS DE TEXTO ----------
    FT_NORMAL, FT_NEGRITA, FT_SUBRAYADO, FT_CURSIVA=4, FT_TACHADO=8

};

//------------------ Funciones de Ventana -----------------
void VDefine(int ample=800, int alt=500,std::string vtitulo="");
int  VAncho();
int  VAlto();
void VLimpia();
void VRefresca();
void VCierra();

//------------------ Funciones de Trazos ------------------
void FormatoBorde(int EBorde, int GLinea=1,int color=CL_BLANCO);
void FormatoRelleno(int ERelleno,int color=CL_BLANCO);

void Punto(int x, int y);

void Linea(int x_ini, int y_ini, int x_fin, int y_fin);
void LineaPolar(int x, int y, int lon,int ang);

void Rectangulo(int izq, int arr, int der, int aba);
void RectanguloRedondeado(int izq, int arr, int der, int aba, int Resquina);
void Elipse(int izq, int arr, int der, int aba);

void Circulo(int x_cen, int y_cen, int radio);
void Arco(int x_cen, int y_cen, int radio, int ang_i, int ang_f);
void Sector(int x_cen, int y_cen, int radio, int ang_i, int ang_f);

void PoligonoRegular(int x_cen, int y_cen,int radio, int NLados);
void PoligonoIrregular(std::vector<std::array <int,2>> puntos);

void CurvaBezier(std::vector<std::array <int,2>>puntos);


//------------------ Funciones de Texto ---------------------
void TJustificar(int _estilo);
void TFormato(std::string nombrefuente, int tamanofuente, int inclinacion=0, int NSCT=FT_NORMAL, int color=CL_BLANCO);
void TMostrar(int x, int y, int ancho, int alto, const std::string& texto);
int TAltura();

//--------------- Funciones de Entrada ----------------------
int Tecla();
void LimpiaMemoriaTecla();

bool Raton(int& x, int& y);
bool RatonDentro();
int RatonX();
int RatonY();
void RatonBotones(bool& izq, bool& der);
bool RatonBotonIzq();
bool RatonBotonDer();

//---------------------- Otros ---------------------------
void Mensaje(std::string msj);
bool Pregunta(std::string msj);
void Espera(int miliseg);

} // namespace graphito

#endif


