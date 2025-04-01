
#include "resultadoSimulacion.h"

//-----------------------------------| Implementación de ResultadoSimulacion |-----------------------------------------------------//

ResultadoSimulacion::ResultadoSimulacion(int tamLinea, int ciclos, int aciertos, int fallos)
    : tamanoLinea(tamLinea), tiempoTotal(ciclos){
    int totalAccesos = aciertos + fallos;
    tasaAciertos = totalAccesos > 0 ? static_cast<double>(aciertos) / totalAccesos : 0.0;
    tiempoPromedio = totalAccesos > 0 ? static_cast<double>(ciclos) / totalAccesos : 0.0;
}

int ResultadoSimulacion::obtenerTamanoLinea() const{ return tamanoLinea; }
double ResultadoSimulacion::obtenerTasaAciertos() const{ return tasaAciertos; }
double ResultadoSimulacion::obtenerTiempoPromedio() const{ return tiempoPromedio; }
int ResultadoSimulacion::obtenerTiempoTotal() const{ return tiempoTotal; }