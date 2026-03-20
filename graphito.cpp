
// VERSION: 2.0 (solo para windows)


//#if defined(_WIN32)

#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <math.h>
#include <process.h>
#include <windows.h>
#include <windowsx.h>

#define GRAPHITO_SOURCE
#include "graphito.h"

//-----------------------------------------------------------------------

std::string szClassName = "graphito";

//------------------- Variables generales del entorno -------------------

HWND            hWnd;                               // handler ventana principal
HBITMAP         hBitmap;                            // bitmap para pintar off-screen
HDC             hDCMem = NULL;                      // Device Context en memoria
int             VWidth  = 800;                      // ancho de la ventana
int             VHeight = 500;                      // alto de la ventana
std::string     Tventana = szClassName;             // titulo de la ventana
std::queue<int> _teclas;                            // cola de teclas
bool            _raton_dentro;                      // el raton está dentro del 'client area'
int             _xraton, _yraton;                   // posicion del raton
bool            _bot_izq, _bot_der;                 // botones izquierdo y derecho
COLORREF        _colorC = RGB(255, 255, 255);       // Color del contorno
COLORREF        _colorR = RGB(255, 255, 255);       // Color de rellenos
COLORREF        _colorT = RGB(255, 255, 255);       // Color de textos
int             TPen = graphito::EB_CONTINUO;       // estilo de trazo
int             GPen = 1;                           // grosor del trazo
int             RBrush = graphito::ER_SOLIDO;       // tipo de Relleno;
int             FTamano = 20;                       // Tamaño letra
int             FInclinacion = 0;                   // inclinación texto
bool            FNegrita = false;                   // texto en negrita
bool            FCursiva = false;                   // texto en cursiva
bool            FSubrayada = false;                 // texto subrayado
bool            FTachada = false;                   // texto Tachado
std::string     FNombre = "Times New Roman";        // nombre de la fuente
UINT            FJustificado = (TA_LEFT|TA_BOTTOM); // justificado Horizontal
//--------------------------------------------------------------------------

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);


VOID Thread(PVOID pvoid) {
   Sleep(50);
   _main_();
}

void call_main() {
   static bool started = false;
   if (!started) {
      _beginthread(Thread, 0, NULL); // Llama a 'main' (realmente  '_main_')
      started = true;
   }
}


void newMemDC(int w, int h) {
   if (hDCMem != NULL) {
      DeleteObject(hBitmap);
      DeleteDC(hDCMem);
   }
   HDC hDC = GetDC(hWnd);
   hDCMem  = CreateCompatibleDC(hDC);
   hBitmap = CreateCompatibleBitmap (hDC, w, h);
   SelectObject(hDCMem, hBitmap);
   SetBkMode(hDCMem, TRANSPARENT);
}

int WINAPI WinMain (HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszArgument,int nFunsterStil){
    static WNDCLASSEX wincl;
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName.c_str();
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if (!RegisterClassEx (&wincl))
       return 0;


    hWnd = CreateWindowEx (
      0,                   /* Extended possibilites for variation */
      szClassName.c_str(), /* Classname */
      Tventana.c_str(),    /* Titulo de la ventana */
      WS_SYSMENU,          /* Solo el botón cerrar */
      CW_USEDEFAULT,       /* Windows decide la posicion */
      CW_USEDEFAULT,       /* Windows decide la posicion */
      VWidth,              /* Ancho de la ventana */
      VHeight,             /* Alto de la ventana */
      HWND_DESKTOP,        /* dentro del escritorio */
      NULL,                /* Sin menu */
      hThisInstance,       /* Manejador del programa */
      NULL                 /* No Window Creation data */
    );

    hBitmap = NULL;

    ShowWindow (hWnd, nFunsterStil );
    ShowWindow (GetConsoleWindow(), SW_HIDE);

    MSG messages;
    while (GetMessage (&messages, NULL, 0, 0)) {
       TranslateMessage(&messages);
       DispatchMessage(&messages);
    }

    ShowWindow (GetConsoleWindow(), SW_SHOW);

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
   switch (message) {
   case WM_SIZE: {
      RECT R;
      GetClientRect(hWnd, &R);
      int w = R.right - R.left;
      int h = R.bottom - R.top;
      if (w == 0 && h == 0) break; // Al minimizar envia WM_SIZE(0,0)

      if (hDCMem == NULL || w != VWidth || h != VHeight) {
         newMemDC(w, h);
         call_main();
      }
      break;
   }

   case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);
      SelectObject(hDCMem, hBitmap);
      if (hBitmap != NULL) {
         BitBlt(hdc, 0, 0, VWidth, VHeight, hDCMem, 0, 0, SRCCOPY);
      }
      EndPaint(hWnd, &ps);
      break;
   }
   case WM_MOUSEMOVE: {
      _raton_dentro = true;
      _xraton = GET_X_LPARAM(lParam);
      _yraton = GET_Y_LPARAM(lParam);
      _bot_izq = wParam & MK_LBUTTON;
      _bot_der = wParam & MK_RBUTTON;
      break;
   }
   case WM_MOUSELEAVE: {
      _raton_dentro = false;
      break;
   }
   case WM_LBUTTONDOWN: {
      _bot_izq = true;
      break;
   }
   case WM_LBUTTONUP: {
      _bot_izq = false;
      break;
   }
   case WM_RBUTTONDOWN: {
      _bot_der = true;
      break;
   }
   case WM_RBUTTONUP: {
      _bot_der = false;
      break;
   }
   case WM_KEYDOWN: {
     bool push_it = false;



     push_it |= (wParam == VK_ESCAPE);  // Escape

     // Flechas
     push_it |= (wParam == VK_LEFT ||
                 wParam == VK_RIGHT ||
                 wParam == VK_UP ||
                 wParam == VK_DOWN);


     push_it |= (wParam == VK_SPACE);   // Barra espaciadora

     push_it |= (wParam == VK_BACK);    // Backspace

     push_it |= (wParam == VK_RETURN);  // Enter

     push_it |= (wParam == VK_TAB);     // Tabulador

     // Números 0-9
     push_it |= (wParam >= 48 && wParam <= 57);

     // Letras A-Z
     push_it |= (wParam >= 65 && wParam <= 90);

     // Letra Ñ
     push_it |= (wParam == VK_OEM_3);   //para incluir la Ñ

     // Teclas de función
     for (unsigned int i = 0; i < 12; i++) {
       push_it |= (wParam == (VK_F1 + i));  //hasta 12 teclas de funcion f1-f12
     }

     if (push_it) _teclas.push(wParam);

     break;
   }
   case WM_DESTROY: {

      DeleteObject (hBitmap);
      DeleteDC (hDCMem);
      PostQuitMessage(0);
        return 0;

        break;
   }
   default:
      return DefWindowProc (hWnd, message, wParam, lParam);
   }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
//   Funciones del API
//
////////////////////////////////////////////////////////////////////////////////



namespace graphito {

void VDefine(int ample, int alt,std::string vtitulo){
    VWidth  = ample;                                    // ancho de la ventana
    VHeight = alt;                                      // alto de la ventana
    SetWindowPos(hWnd, NULL, 0, 0, VWidth, VHeight, SWP_NOMOVE);
    if(vtitulo.length()>0)
       SetWindowTextA(hWnd,vtitulo.c_str());
    newMemDC(VWidth, VHeight);
}

int VAncho() {
   return VWidth;
}

int VAlto() {
   return VHeight;
}

void VCierra() {
  PostMessage(hWnd, WM_CLOSE, 0, 0);
}

void VRefresca() {
   Espera(10);
   InvalidateRect(hWnd, NULL, FALSE);
}

void VLimpia() {
   RECT R;
   SetRect(&R, 0, 0, VWidth, VHeight);
   HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
   FillRect(hDCMem, &R, hBrush);
   DeleteObject(hBrush);
   VRefresca();
}

//----------------- Trazos ------------------------------------
static COLORREF _colores[] = {
   RGB(0, 0, 0),       // CL_NEGRO
   RGB(255, 0, 0),     // CL_ROJO
   RGB(0, 255, 0),     // CL_VERDE
   RGB(0, 0, 255),     // CL_AZUL
   RGB(255, 255, 0),   // CL_AMARILLO
   RGB(255, 0, 255),   // CL_MAGENTA
   RGB(0, 255, 255),   // CL_CYAN
   RGB(255, 255, 255), // CL_BLANCO
   RGB(128, 128, 128), // CL_GRIS
   RGB(119, 67, 66),   // CL_CAFE
   RGB(255, 127, 39),  // CL_NARANJA
   RGB(250, 128, 114), // CL_SALMON
   RGB(255, 192, 203), // CL_ROSADO
   RGB(255, 105, 180), // CL_ROSADOFUERTE
   RGB(255, 215, 0),   // CL_ORO
   RGB(230, 230, 250), // CL_LAVANDA
   RGB(238, 130, 238), // CL_VIOLETA
   RGB(147, 112, 219), // CL_MORADO
   RGB(173, 255, 47),  // CL_LIMON
   RGB(128, 128, 0),   // CL_OLIVO
   RGB(64, 224, 208),  // CL_TURQUESA
   RGB(192, 192, 192), // CL_PLATA

};

void FormatoBorde(int EBorde, int GLinea, int color){
    _colorC = _colores[color];
    TPen = ((EBorde>0)&&(EBorde<5))?EBorde:0;
    GPen = (TPen == EB_CONTINUO)?GLinea:1;
}

void FormatoRelleno(int ERelleno, int color){
    _colorR = _colores[color];
    RBrush = ((ERelleno>=0)&&(ERelleno<=7))?ERelleno:6;

}

void Punto(int x, int y) {
    BeginPath(hDCMem);
    MoveToEx(hDCMem, x, y, NULL);
    LineTo(hDCMem, x+1, y);
    EndPath(hDCMem);
    HPEN hPen = CreatePen(TPen, GPen, _colorC);
    SelectObject(hDCMem, hPen);
    StrokePath(hDCMem);
    DeleteObject(hPen);
    VRefresca();
}

void Linea(int x_ini, int y_ini, int x_fin, int y_fin) {
   BeginPath(hDCMem);
   MoveToEx(hDCMem, int(x_ini), int(y_ini), NULL);
   LineTo(hDCMem, int(x_fin), int(y_fin));
   EndPath(hDCMem);
   HPEN hPen = CreatePen(TPen, GPen, _colorC);
   SelectObject(hDCMem, hPen);
   StrokePath(hDCMem);
   DeleteObject(hPen);
   VRefresca();
}
void LineaPolar(int x, int y, int lon, int ang){
    float angrad= ang*M_PI/180;
    Linea(x,y,x+cos(angrad)*lon,y-sin(angrad)*lon);
}

inline void _rect(int izq, int arr, int der, int aba) {
   BeginPath(hDCMem);
   Rectangle(hDCMem,izq,arr,der,aba);
   EndPath(hDCMem);
}

void Rectangulo(int izq, int arr, int der, int aba) {
    HGDIOBJ orig;

    if(RBrush!=ER_NORELLENO){
        HBRUSH hBrush = CreateHatchBrush(RBrush,_colorR);
        orig = SelectObject(hDCMem, hBrush);
        _rect(izq, arr, der, aba);
        FillPath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hBrush);
    }
    if(GPen>0){
        HPEN hPen = CreatePen(TPen, GPen, _colorC);
        orig = SelectObject(hDCMem, hPen);
        _rect(izq, arr, der, aba);
        StrokePath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hPen);
    }

    VRefresca();
}


inline void _rectred(int izq, int arr, int der, int aba, int Resquina) {
   BeginPath(hDCMem);
   RoundRect(hDCMem, izq, arr, der, aba, Resquina, Resquina);
   EndPath(hDCMem);
}

void RectanguloRedondeado(int izq, int arr, int der, int aba, int Resquina) {
    HGDIOBJ orig;
    if(RBrush!=ER_NORELLENO){
        HBRUSH hBrush = CreateHatchBrush(RBrush,_colorR);
        orig = SelectObject(hDCMem, hBrush);
        _rectred(izq, arr, der, aba, Resquina);
        FillPath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hBrush);
    }
    if(GPen>0){
        HPEN hPen = CreatePen(TPen, GPen, _colorC);
        orig = SelectObject(hDCMem, hPen);
        _rectred(izq, arr, der, aba, Resquina);
        StrokePath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hPen);
    }

    VRefresca();
}

inline void _arc(int x_cen, int y_cen, int radio, int ang_i, int ang_f) {
    BeginPath(hDCMem);
    MoveToEx(hDCMem, x_cen+cos(ang_i*M_PI/180)*radio, y_cen-sin(ang_i*M_PI/180)*radio, NULL); //
    AngleArc(hDCMem, x_cen , y_cen , radio, ang_i, ang_f-ang_i);
    EndPath(hDCMem);
}

void Arco(int x_cen, int y_cen, int radio, int ang_i, int ang_f) {
    HGDIOBJ orig;

    if(GPen>0){
        HPEN hPen = CreatePen(TPen, GPen, _colorC);
        orig = SelectObject(hDCMem, hPen);
        _arc(x_cen, y_cen, radio, ang_i,ang_f);
        StrokePath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hPen);
    }

    VRefresca();
}

inline void _sect(int x_cen, int y_cen, int radio, int ang_i, int ang_f) {
    BeginPath(hDCMem);
    MoveToEx(hDCMem,x_cen,y_cen,NULL);
    LineTo(hDCMem, x_cen+cos(ang_i*M_PI/180)*radio, y_cen-sin(ang_i*M_PI/180)*radio); // Inicio del Arco
    AngleArc(hDCMem, x_cen , y_cen , radio, ang_i, ang_f-ang_i);
    LineTo(hDCMem,x_cen,y_cen); //cerrar el sector
    EndPath(hDCMem);
}

void Sector(int x_cen, int y_cen, int radio, int ang_i, int ang_f) {
    HGDIOBJ orig;

    if(RBrush!=ER_NORELLENO){
        HBRUSH hBrush = CreateHatchBrush(RBrush,_colorR);
        orig = SelectObject(hDCMem, hBrush);
        _sect(x_cen, y_cen, radio, ang_i,ang_f);
        FillPath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hBrush);
    }
    if(GPen>0){
        HPEN hPen = CreatePen(TPen, GPen, _colorC);
        orig = SelectObject(hDCMem, hPen);
        _sect(x_cen, y_cen, radio, ang_i,ang_f);
        StrokePath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hPen);
    }
    VRefresca();
}

inline void _circ(int x_cen, int y_cen, int radio) {
    BeginPath(hDCMem);
    Ellipse(hDCMem,(x_cen-radio),(y_cen-radio),(x_cen+radio),(y_cen+radio));
    EndPath(hDCMem);
}

void Circulo(int x_cen, int y_cen, int radio) {
    HGDIOBJ orig;

    if(RBrush!=ER_NORELLENO){
        HBRUSH hBrush = CreateHatchBrush(RBrush,_colorR);
        orig = SelectObject(hDCMem, hBrush);
        _circ(x_cen, y_cen, radio);
        FillPath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hBrush);
    }
    if(GPen>0){
        HPEN hPen = CreatePen(TPen, GPen, _colorC);
        orig = SelectObject(hDCMem, hPen);
        _circ(x_cen, y_cen, radio);
        StrokePath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hPen);
    }

    VRefresca();
}

inline void _elip(int izq, int arr, int der, int aba) {
    BeginPath(hDCMem);
    Ellipse(hDCMem,izq,arr,der,aba);
    EndPath(hDCMem);
}

void Elipse(int izq, int arr, int der, int aba) {
    HGDIOBJ orig;

    if(RBrush!=ER_NORELLENO){
        HBRUSH hBrush = CreateHatchBrush(RBrush,_colorR);
        orig = SelectObject(hDCMem, hBrush);
        _elip(izq, arr, der, aba);
        FillPath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hBrush);
    }
    if(GPen>0){
        HPEN hPen = CreatePen(TPen, GPen, _colorC);
        orig = SelectObject(hDCMem, hPen);
        _elip(izq, arr, der, aba);
        StrokePath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hPen);
    }

    VRefresca();
}

int _PuntoBezier(std::vector<int>puntos, float t){

    if(puntos.size()>2){
        std::vector<int>npuntos;

        for(size_t i=0;i<(puntos.size()-1);i++)
            npuntos.push_back((1-t)*puntos[i]+t*puntos[i+1]);

        return _PuntoBezier(npuntos,t);

    }else{
        return ((1-t)*puntos[0]+t*puntos[1]);
    }


}

void CurvaBezier(std::vector<std::array <int,2>> puntos){
    HPEN hPen = CreatePen(TPen, GPen, _colorC);
    HGDIOBJ orig = SelectObject(hDCMem, hPen);

    std::vector<int> psx;
    std::vector<int> psy;



    for(size_t i = 0;i<puntos.size();i++){
        psx.push_back(puntos[i][0]);
        psy.push_back(puntos[i][1]);
    }

    BeginPath(hDCMem);
    MoveToEx(hDCMem,puntos[0][0],puntos[0][1],NULL);

    for(float t=0;t<=1;t+=0.05)
        LineTo(hDCMem,_PuntoBezier(psx,t),_PuntoBezier(psy,t));

    LineTo(hDCMem,puntos.back()[0],puntos.back()[1]);

    EndPath(hDCMem);
    StrokePath(hDCMem);
    SelectObject(hDCMem, orig);
    DeleteObject(hPen);

    VRefresca();
}

inline void _polyR(int x_cen, int y_cen,int radio, int NLados) {
    float AngI;
    float PasoAngular = 2*M_PI/NLados;
    BeginPath(hDCMem);
    if(NLados%2){
        AngI=M_PI/2;
        MoveToEx(hDCMem,x_cen,y_cen-radio,NULL);
    }else{
        AngI=0;
        MoveToEx(hDCMem,x_cen+radio,y_cen,NULL);
    }

    for(int L=1;L<=NLados;L++){
        LineTo(hDCMem,x_cen+radio*cos(AngI+PasoAngular*L),y_cen-radio*sin(AngI-PasoAngular*L));
    }
    EndPath(hDCMem);
}

void PoligonoRegular(int x_cen, int y_cen,int radio, int NLados){
    HGDIOBJ orig;

    if(RBrush!=ER_NORELLENO){
        HBRUSH hBrush = CreateHatchBrush(RBrush,_colorR);
        orig = SelectObject(hDCMem, hBrush);
        _polyR(x_cen, y_cen,radio, NLados);
        FillPath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hBrush);
    }
    if(GPen>0){
        HPEN hPen = CreatePen(TPen, GPen, _colorC);
        orig = SelectObject(hDCMem, hPen);
        _polyR(x_cen, y_cen,radio, NLados);
        StrokePath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hPen);
    }

    VRefresca();

}

inline void _polyIR(std::vector<std::array <int,2>> puntos) {

    BeginPath(hDCMem);
    MoveToEx(hDCMem,puntos[0][0],puntos[0][1],NULL);
    for(int L=1;L<int(puntos.size());L++)
        LineTo(hDCMem,puntos[L][0],puntos[L][1]);

    LineTo(hDCMem,puntos[0][0],puntos[0][1]);
    EndPath(hDCMem);
}

void PoligonoIrregular(std::vector<std::array <int,2>> puntos){
    HGDIOBJ orig;

    if(RBrush!=ER_NORELLENO){
        HBRUSH hBrush = CreateHatchBrush(RBrush,_colorR);
        orig = SelectObject(hDCMem, hBrush);
        _polyIR(puntos);
        FillPath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hBrush);
    }
    if(GPen>0){
        HPEN hPen = CreatePen(TPen, GPen, _colorC);
        orig = SelectObject(hDCMem, hPen);
        _polyIR(puntos);
        StrokePath(hDCMem);
        SelectObject(hDCMem, orig);
        DeleteObject(hPen);
    }

    VRefresca();

}


//----------------------------- Texto ----------------------------------------------
void TJustificar(int _estilo){
    switch(_estilo){
    case JT_CENTRO: FJustificado = (TA_CENTER|TA_BOTTOM);break;
    case JT_DERECHA: FJustificado =(TA_RIGHT|TA_BOTTOM);break;
    default : FJustificado = (TA_LEFT|TA_BOTTOM);
    }
}

void TFormato(std::string nombrefuente, int tamanofuente, int inclinacion, int NSCT, int color){
    FNombre = nombrefuente;
    FTamano = tamanofuente;
    FInclinacion = inclinacion;
    _colorT = _colores[color];
    FNegrita = false;
    FCursiva = false;
    FSubrayada = false;
    FTachada = false;

    if(NSCT&1)  FNegrita = true;

    if(NSCT&2)  FSubrayada = true;

    if(NSCT&4)  FCursiva = true;

    if(NSCT&8)  FTachada = true;
}

void TMostrar(int x, int y, int ancho, int alto, const std::string& texto) {
    HFONT HFuente;
    RECT areaR;


    areaR.top = y-alto/2;
    areaR.bottom = y+alto/2;

    if(FJustificado==(TA_CENTER|TA_BOTTOM)){
        areaR.left = x-ancho/2;
        areaR.right = x+ancho/2;
   }

   if(FJustificado==(TA_RIGHT|TA_BOTTOM)){
        areaR.left = x-ancho;
        areaR.right = x;
   }
   if(FJustificado==(TA_LEFT|TA_BOTTOM)){
        areaR.left = x;
        areaR.right = x+ancho;
   }

   SetTextColor(hDCMem, _colorT);
   HFuente = CreateFont(FTamano,0 ,FInclinacion*10, FInclinacion*10,
            (FNegrita)?700:400, FCursiva, FSubrayada, FTachada,
            DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
            PROOF_QUALITY, DEFAULT_PITCH | FF_ROMAN, FNombre.c_str());
   SelectObject(hDCMem, HFuente);
   SetTextAlign(hDCMem, FJustificado);
   ExtTextOutA(hDCMem, int(x), int(y+TAltura()/2),ETO_CLIPPED,&areaR,texto.c_str(), int(texto.size()),NULL);
   VRefresca();
}

int TAltura(){
    return FTamano;
}


//--------------------------- Otros -----------------------------------
void Mensaje(std::string msj) {
   MessageBox(hWnd, msj.c_str(), "Mensaje...", MB_OK);
}

bool Pregunta(std::string msj) {
   return MessageBox(hWnd, msj.c_str(), "Pregunta...", MB_YESNO) == IDYES;
}

void Espera(int miliseg) {
   Sleep(miliseg);
}

int Tecla() {
    if (_teclas.empty()) return TC_NINGUNA;

    int ret = TC_NINGUNA;
    switch(_teclas.front()) {
    case VK_LEFT:   ret = TC_IZQUIERDA; break;
    case VK_RIGHT:  ret = TC_DERECHA; break;
    case VK_UP:     ret = TC_ARRIBA; break;
    case VK_DOWN:   ret = TC_ABAJO; break;
    case VK_ESCAPE: ret = TC_ESCAPE; break;
    case VK_SPACE:  ret = TC_ESPACIO; break;
    case VK_RETURN: ret = TC_ENTER; break;
    case VK_F1:     ret = TC_F1; break;
    case VK_F2:     ret = TC_F2; break;
    case VK_F3:     ret = TC_F3; break;
    case VK_F4:     ret = TC_F4; break;
    case VK_F5:     ret = TC_F5; break;
    case VK_F6:     ret = TC_F6; break;
    case VK_F7:     ret = TC_F7; break;
    case VK_F8:     ret = TC_F8; break;
    case VK_F9:     ret = TC_F9; break;
    case VK_F10:    ret = TC_F10; break;
    case VK_F11:    ret = TC_F11; break;
    case VK_F12:    ret = TC_F12; break;
    case VK_BACK:   ret = TC_RETROCESO; break;
    case VK_TAB:    ret = TC_TABULADOR; break;
    case VK_OEM_3:  ret = 'Ñ';break;
    default: ret = _teclas.front();
    }
    _teclas.pop();
    return ret;
}

void LimpiaMemoriaTecla(){
    while(!_teclas.empty())
        _teclas.pop();
}

bool Raton(int& x, int& y) {
   if (!_raton_dentro) {
      return false;
   }
   x = _xraton;
   y = _yraton;
   return true;
}

bool RatonDentro() {
   return _raton_dentro;
}

int RatonX() {
   return _xraton;
}

int RatonY() {
   return _yraton;
}

void RatonBotones(bool& izq, bool& der) {
   izq = _bot_izq;
   der = _bot_der;
}

bool RatonBotonIzq() {
   return _bot_izq;
}

bool RatonBotonDer() {
   return _bot_der;
}


} // namespace graphito

///////////////////////////////////////////////////////////////////////////////////////


//#endif
