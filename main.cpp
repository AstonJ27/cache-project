#include "cache.h"
#include "resultadoSimulacion.h"
#include "generadorPatrones.h"
#include "ajusteDinamico.h"

#include "grafic.h"

#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>

#define ASOC_CONJUNTOS 4
#define CORR_DIR 1
#define TOTAL_ASOC 16384/16

using namespace std;

//Función para pausar y limpiar pantalla
void pausarYLimpiar(){
    cout << "\nPresione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar buffer
    //system("cls || clear"); // Limpiar terminal
    cout << endl;
}


void imprimirTabla(const string& nombrePatron, const vector<ResultadoSimulacion>& resultados){
    //Encabezado de la tabla
    cout << "\n";
    cout <<"     ═══════════════════════════════════════════════════════════════\n";
    cout <<"                   Análisis del patrón: " << nombrePatron << "      \n";
    cout <<"     ═══════════════════════════════════════════════════════════════\n";
    
    //Encabezados de columnas
    cout << "┌───────────────────┬────────────┬────────────┬────────────┬────────────┐\n";
    cout << "│ Métrica           │   16B      │   32B      │   64B      │   128B     │\n";
    cout << "├───────────────────┼────────────┼────────────┼────────────┼────────────┤\n";
    
    //Fila de tasa de aciertos
    cout << "│ Tasa de Aciertos  │ ";
    for(const auto& res : resultados){
        cout << setw(9) << fixed << setprecision(2) << res.obtenerTasaAciertos() * 100 << "% │ ";
    }
    cout << "\n";
    
    //Fila de tiempo promedio
    cout << "│ Tiempo en ciclos  │ ";
    for(const auto& res : resultados){
        cout << setw(10) << fixed << setprecision(2) << res.obtenerTiempoPromedio() << " │ ";
    }
    cout << "\n";
    
    //Pie de tabla
    cout << "└───────────────────┴────────────┴────────────┴────────────┴────────────┘\n";

}


int main(){
    std::map<std::string, std::vector<ResultadoSimulacion>> resultadosConsolidados;
    
    srand(time(nullptr));
    cout << endl;
    //Configuración común
    constexpr int tamanoTotal = 16384;                      //16KB
    constexpr int asociatividad = 4;                        //Asociativa por conjuntos
    const string politica = "LRU";                          //Politica LRU
    constexpr int numAccesos = 100000;                      //Cantidad de accesos a caché
    const vector<int> tamanosLinea = {16, 32, 64, 128};     //Tamaños disponibles

    //Configurar patrones
    const vector<pair<GeneradorPatrones::TipoPatron, string>> patrones = {
        {GeneradorPatrones::SECUENCIAL, "Secuencial"},
        {GeneradorPatrones::ALEATORIO, "Aleatorio"},
        {GeneradorPatrones::CIRCULAR, "Circular"},
        {GeneradorPatrones::CLUSTERIZADO, "Clusterizado"},
        {GeneradorPatrones::ESTRIDED, "Estrided"},
        {GeneradorPatrones::MIXTO, "Mixto"}



    };

    //Parte 1: Simulación con tamaños fijos
    cout <<"                  ";
    cout << "\033[1;4:2;33m" << "SIMULACIÓN CON TAMAÑOS DE LÍNEA FIJOS" <<"\e[0m"<< endl ;

    // loop de patrones
    for(const auto& [tipoPatron, nombrePatron] : patrones){
        vector<ResultadoSimulacion> resultados;
        
        for(int tamLinea : tamanosLinea){
            Cache cache(tamanoTotal, tamLinea, asociatividad, politica);
            auto accesos = GeneradorPatrones::generarAccesos(tipoPatron, numAccesos);
            
            for(int dir : accesos){
                cache.acceder(dir);
            }
            
            resultados.emplace_back(
                tamLinea,
                cache.obtenerCiclosTotales(),
                cache.obtenerAciertos(),
                cache.obtenerFallos()
            );
        }
        
        resultadosConsolidados[nombrePatron] = resultados;
        // Mostrar resultados
        imprimirTabla(nombrePatron, resultados);
   
    }

    pausarYLimpiar(); //Esperar entrada y limpiar

    generarGraficoConsolidado(resultadosConsolidados);

    pausarYLimpiar(); //Esperar entrada y limpiar

    //Parte 2: Simulación de ajuste dinámico
    cout <<"                  ";
    cout << "\033[1;4:2;33m" << "SIMULACIÓN CON AJUSTE DINAMICO" <<"\e[0m"<< endl ;
    GeneradorPatrones::TipoPatron patron = GeneradorPatrones::MIXTO;
    
    int tamLinea = 16;
    Cache cacheInicial(tamanoTotal, tamLinea, CORR_DIR, "LRU"); // 16KB, 16 de linea,1-vía (directa), politica LRU 
    auto [historial, ajustes] = AjusteDinamico::simular(
        cacheInicial,
        patron,
        numAccesos,     // Accesos totales 100000
        2500,           // Intervalo más grande (para permitir "calentamiento")
        0.5,            // Umbral alto para forzar reducciones
        16,
        128
    );

    // Mostrar puntos de ajuste con formato mejorado
    cout << "\n\033[1;36mPuntos de ajuste dinámico:\033[0m\n";
    for(const auto& ajuste : ajustes){
        cout << "• En acceso: \033[1;33m" << setw(6) << ajuste.acceso 
             << "\033[0m → Nuevo tamaño: \033[1;32m" << ajuste.nuevoTamano << "B\033[0m\n";
    }

    // Mostrar evolución con formato de tabla
    cout << "\n\033[1;36mEvolución del rendimiento:\033[0m\n";
    cout << "┌──────────┬─────────┬───────────┬──────────────┐\n";
    cout << "│  Bloque  │ Tamaño  │ Aciertos  │ Tiempo avg   │\n";
    cout << "├──────────┼─────────┼───────────┼──────────────┤\n";
    
    const int muestras = 5;
    const int paso = max(1, (int)historial.size() / muestras);
    for(size_t i = 0; i < historial.size(); i += paso){
        const auto& punto = historial[i];
        cout << "│ " << setw(8) << i * paso 
             << " │ " << setw(6) << punto.obtenerTamanoLinea() << "B"
             << " │ " << setw(8) << fixed << setprecision(1) << punto.obtenerTasaAciertos() * 100 << "%"
             << " │ " << setw(5) << fixed << setprecision(2) << punto.obtenerTiempoPromedio() << " ciclos │\n";
    }
    cout << "└──────────┴─────────┴───────────┴──────────────┘\n";

    pausarYLimpiar();
    return 0;
}