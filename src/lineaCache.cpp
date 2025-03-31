#include "lineaCache.h"

//-----------------------------------| Implementación de LineaCache |-----------------------------------------------------//

LineaCache::LineaCache() : etiqueta(-1), valida(false), ultimoUso(0) {}

void LineaCache::actualizarUso(int cicloActual) {
    ultimoUso = cicloActual;
}

bool LineaCache::estaValida() const { 
    return valida; 
}

int LineaCache::obtenerEtiqueta() const { 
    return etiqueta; 
}

void LineaCache::configurarLinea(int nuevaEtiqueta, int cicloActual) {
    etiqueta = nuevaEtiqueta;
    valida = true;
    ultimoUso = cicloActual;
}

int LineaCache::obtenerUltimoUso() const { 
    return ultimoUso; 
}