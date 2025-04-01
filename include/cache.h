#ifndef CACHE_H
#define CACHE_H

#include "lineaCache.h"

#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <iomanip>

using namespace std;

// Clase principal que implementa la memoria caché
class Cache {
private:
    int tamanoTotal;    // Tamaño total en bytes
    int tamanoLinea;    // Tamaño de cada línea en bytes
    size_t asociatividad;  // Número de vías por conjunto
    string politica;    // Política de reemplazo
    int numConjuntos;   // Cantidad de conjuntos
    
    // Componentes de dirección
    int bitsOffset;
    int bitsIndice;
    int bitsEtiqueta;
    
    // Configuración de tiempos
    const int tiempoAcierto = 1;
    const int penalizacionFallo = 100;
    
    // Contadores de rendimiento
    int contadorAccesos;
    int contadorAciertos;
    int contadorFallos;
    int ciclosTotales;

    vector<vector<LineaCache>> conjuntos;

    void calcularBitsDireccion();
    void validarConfiguracion();
    int obtenerIndiceConjunto(int direccion) const;
    int obtenerEtiquetaDireccion(int direccion) const;

public:
    Cache(int tamTotal, int tamLinea, int asociat, const string& politica);
    
    void reiniciar();
    void acceder(int direccion);
    
    // Métodos de acceso a estadísticas
    int obtenerAciertos() const;
    int obtenerFallos() const;
    int obtenerCiclosTotales() const;
    int obtenerTamanoLinea() const;
    double calcularTasaAciertos() const;
    double calcularTiempoPromedio() const;
    void cambiarTamanoLinea(int nuevoTamanoLinea);


    int obtenerTamanoTotal() const { return tamanoTotal; }
    int obtenerAsociatividad() const { return asociatividad; }
    string obtenerPolitica() const { return politica; }

};

#endif // CACHE_H