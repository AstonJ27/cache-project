
#ifndef AJUSTE_DINAMICO_H
#define AJUSTE_DINAMICO_H


#include "cache.h"
#include "resultadoSimulacion.h"
#include "generadorPatrones.h"

#include <vector>


using namespace std;

// Clase para simulación con ajuste dinámico
class AjusteDinamico {
private:
    struct PuntoAjuste {
        int acceso;
        int nuevoTamano;
    };

    vector<ResultadoSimulacion> historial;
    vector<PuntoAjuste> ajustes;

public:
    static pair<vector<ResultadoSimulacion>, vector<PuntoAjuste>> simular(
        Cache& cache, 
        GeneradorPatrones::TipoPatron patron,
        size_t numAccesos,
        int intervalo,
        double umbral,
        int minTam,
        int maxTam
    );
};

#endif


