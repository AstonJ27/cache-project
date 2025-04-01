#include "ajusteDinamico.h"

//------------------------------------| Implementación de AjusteDinamico |-----------------------------------//

pair<vector<ResultadoSimulacion>, vector<AjusteDinamico::PuntoAjuste>> 
AjusteDinamico::simular(Cache& cache, GeneradorPatrones::TipoPatron patron, size_t numAccesos, int intervalo, 
                        double umbral, int minTam, int maxTam) {
    
    vector<ResultadoSimulacion> historial;
    vector<PuntoAjuste> ajustes;
    
    //Generar accesos usando el tamaño de línea inicial
    int tamActual = cache.obtenerTamanoLinea();
    auto accesos = GeneradorPatrones::generarAccesos(patron, numAccesos, 1048576, tamActual);
    
    //Variables para rastrear contadores locales
    int aciertosAcumulados = 0;
    int fallosAcumulados = 0;
    int ciclosAcumulados = 0;
    
    for(size_t i = 0; i < numAccesos; i += intervalo) {
        int finBloque = min((int)i + intervalo, (int)numAccesos);
        
        //Guardar estado inicial de la caché
        int aciertosAntes = cache.obtenerAciertos();
        int fallosAntes = cache.obtenerFallos();
        int ciclosAntes = cache.obtenerCiclosTotales();
        
        // Procesar bloque de accesos
        for(int j = i; j < finBloque; ++j){
            cache.acceder(accesos[j]);
        }
        
        //Calcular métricas usando diferencia de contadores globales
        int aciertosBloque = cache.obtenerAciertos() - aciertosAntes;
        int fallosBloque = cache.obtenerFallos() - fallosAntes;
        int ciclosBloque = cache.obtenerCiclosTotales() - ciclosAntes;
        
        // Acumular métricas para evitar reinicios
        aciertosAcumulados += aciertosBloque;
        fallosAcumulados += fallosBloque;
        ciclosAcumulados += ciclosBloque;
        
        //Calcular tasa usando el bloque actual
        int totalBloque = aciertosBloque + fallosBloque;
        double tasa = totalBloque > 0 ? 
            static_cast<double>(aciertosBloque) / totalBloque : 0.0;
        
        //Ajustar tamaño
        int nuevoTam = tamActual;
        if(tasa < umbral){
            nuevoTam = min(tamActual * 2, maxTam);
        }else{
            nuevoTam = max(tamActual / 2, minTam);
        }
        
        //Ajustar si hay cambio

        if(nuevoTam != tamActual){
            cache.cambiarTamanoLinea(nuevoTam);
            ajustes.push_back({finBloque, nuevoTam});
            tamActual = nuevoTam;

            // --- INICIO DE MODIFICACIÓN CRÍTICA ---
            size_t inicioRegenerar = finBloque;

            //Calcular accesos restantes de forma segura
            int cantidadRegenerar = numAccesos - inicioRegenerar;

            //Solo regenerar si hay accesos pendientes
            if(cantidadRegenerar > 0 && inicioRegenerar < numAccesos) {

                //Generar nuevos accesos con el tamaño actual
                auto nuevosAccesos = GeneradorPatrones::generarAccesos(
                    patron,
                    cantidadRegenerar,
                    1048576,
                    tamActual
                );

                //Reemplazar desde inicioRegenerar en adelante
                if(inicioRegenerar < accesos.size()) {
                    //Eliminar elementos antiguos
                    accesos.erase(accesos.begin() + inicioRegenerar, accesos.end());
                }

                //Insertar nuevos accesos
                accesos.insert(accesos.end(), nuevosAccesos.begin(), nuevosAccesos.end());

                //Asegurar tamaño exacto (por si hay discrepancias)
                accesos.resize(numAccesos);
            }
            // --- FIN DE MODIFICACIÓN ---
        }
        
        //Registrar métricas usando acumulados
        historial.emplace_back(
            nuevoTam,
            ciclosAcumulados,
            aciertosAcumulados,
            fallosAcumulados
        );
    }
    
    return {historial, ajustes};
}