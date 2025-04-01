#include "cache.h"

//------------------------------------------------------| Implementación de Cache |-----------------------------------------------------//

Cache::Cache(int tamTotal, int tamLinea, int asociat, const string& politica)
    : tamanoTotal(tamTotal), tamanoLinea(tamLinea), asociatividad(asociat), 
      politica(politica), contadorAccesos(0), contadorAciertos(0),
      contadorFallos(0), ciclosTotales(0){
    
    calcularBitsDireccion();
    validarConfiguracion();
    
    //Inicializar conjuntos
    conjuntos.resize(numConjuntos);
    for(auto& conjunto : conjuntos){
        conjunto.reserve(asociatividad);
    }
}

void Cache::calcularBitsDireccion(){
    numConjuntos = tamanoTotal / (tamanoLinea * asociatividad);
    bitsOffset = static_cast<int>(log2(tamanoLinea));
    bitsIndice = static_cast<int>(log2(numConjuntos));
    bitsEtiqueta = 32 - bitsOffset - bitsIndice;
}

void Cache::validarConfiguracion(){
    if((1 << bitsOffset) != tamanoLinea || (1 << bitsIndice) != numConjuntos){
        throw invalid_argument("Error: Los tamaños deben ser potencias de dos");
    }
}

int Cache::obtenerIndiceConjunto(int direccion) const{
    return (direccion >> bitsOffset) & ((1 << bitsIndice) - 1);
}

int Cache::obtenerEtiquetaDireccion(int direccion) const{
    return (direccion >> (bitsOffset + bitsIndice)) & ((1 << bitsEtiqueta) - 1);
}

void Cache::reiniciar(){
    for(auto& conjunto : conjuntos){
        for(auto& linea : conjunto){
            linea = LineaCache();
        }
    }
    contadorAccesos = 0;
    contadorAciertos = 0;
    contadorFallos = 0;
    ciclosTotales = 0;
}

void Cache::acceder(int direccion){
    contadorAccesos++;
    int indice = obtenerIndiceConjunto(direccion);
    int etiqueta = obtenerEtiquetaDireccion(direccion);
    
    auto& conjunto = conjuntos[indice];
    bool acierto = false;

    //Buscar en el conjunto
    for(auto& linea : conjunto){
        if(linea.estaValida() && linea.obtenerEtiqueta() == etiqueta){
            acierto = true;
            linea.actualizarUso(contadorAccesos);
            break;
        }
    }

    if(acierto){
        contadorAciertos++;
        ciclosTotales += tiempoAcierto;
    }else {
        contadorFallos++;
        ciclosTotales += tiempoAcierto + penalizacionFallo;
        
        LineaCache nuevaLinea;
        nuevaLinea.configurarLinea(etiqueta, contadorAccesos);
        
        if(conjunto.size() < asociatividad){
            conjunto.push_back(nuevaLinea);
        }else {
            //Aplicar política de reemplazo
            if(politica == "LRU"){
                auto lru = min_element(conjunto.begin(), conjunto.end(),
                    [](const LineaCache& a, const LineaCache& b){
                        return a.obtenerUltimoUso() < b.obtenerUltimoUso();
                    });
                *lru = nuevaLinea;
            }else {
                //Reemplazo aleatorio
                int indiceAleatorio = rand() % conjunto.size();
                conjunto[indiceAleatorio] = nuevaLinea;
            }
        }
    }
}

//Métodos de acceso a estadísticas
int Cache::obtenerAciertos() const{ return contadorAciertos; }
int Cache::obtenerFallos() const{ return contadorFallos; }
int Cache::obtenerCiclosTotales() const{ return ciclosTotales; }
int Cache::obtenerTamanoLinea() const{ return tamanoLinea; }

double Cache::calcularTasaAciertos() const{ 
    int total = contadorAciertos + contadorFallos;
    return total > 0 ? static_cast<double>(contadorAciertos) / total : 0.0;
}

double Cache::calcularTiempoPromedio() const{
    int total = contadorAciertos + contadorFallos;
    return total > 0 ? static_cast<double>(ciclosTotales) / total : 0.0;
}

void Cache::cambiarTamanoLinea(int nuevoTamanoLinea){
    tamanoLinea = nuevoTamanoLinea;
    calcularBitsDireccion();
    validarConfiguracion();
    //Resetear contadores y datos

    conjuntos.resize(numConjuntos);

    //Inicializar cada conjunto (vaciar líneas existentes)
    for(auto& conjunto : conjuntos){
        conjunto.clear();
        conjunto.reserve(asociatividad);
    }
}
