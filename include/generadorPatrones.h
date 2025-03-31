#ifndef GENERADOR_PATRONES_H
#define GENERADOR_PATRONES_H


#include <vector>
#include <string>
#include <random>
#include <iomanip>

using namespace std;

// Clase generadora de patrones de acceso
class GeneradorPatrones {
public:
    enum TipoPatron {
        SECUENCIAL,
        ALEATORIO,
        CIRCULAR,
        ESTRIDED,
        CLUSTERIZADO,
        MIXTO
    };

    static vector<int> generarAccesos(TipoPatron patron, size_t numAccesos, int maxDireccion = 1048576, int tamLinea=128);
     //método para obtener nombres
    static string obtenerNombrePatron(TipoPatron patron);
    
    //método para obtener lista de nombres
    static vector<string> obtenerNombresPatrones();
};







#endif