#include "Top5.h"
#include "graphito.h"
using namespace graphito;
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

void guardarTop5(int puntos){

    vector<int> puntajes;
    ifstream archivo("top5.txt");

    int p;
    while(archivo >> p){
        puntajes.push_back(p);
    }
    archivo.close();

    puntajes.push_back(puntos);

    sort(puntajes.begin(), puntajes.end(), greater<int>());

    if(puntajes.size() > 5){
        puntajes.resize(5);
    }

    ofstream salida("top5.txt");
    for(int i = 0; i < puntajes.size(); i++){
        salida << puntajes[i] << endl;
    }
    salida.close();
}

void mostrarTop5(){

    ifstream archivo("top5.txt");
    vector<int> puntajes;

    int p;
    while(archivo >> p){
        puntajes.push_back(p);
    }
    archivo.close();

    string top = "TOP 5:\n";

    for(int i = 0; i < puntajes.size(); i++){
        top += to_string(i+1) + ". " + to_string(puntajes[i]) + " pts\n";
    }

    Mensaje(top);
}
