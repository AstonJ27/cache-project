#ifndef RESULTADO_SIMULACION_H
#define RESULTADO_SIMULACION_H


// Clase para resultados de simulación
class ResultadoSimulacion {
private:
    int tamanoLinea;
    double tasaAciertos;
    double tiempoPromedio;
    int tiempoTotal;

public:
    ResultadoSimulacion(int tamLinea, int ciclos, int aciertos, int fallos);
    
    // Getters
    int obtenerTamanoLinea() const;
    double obtenerTasaAciertos() const;
    double obtenerTiempoPromedio() const;
    int obtenerTiempoTotal() const;
};


#endif