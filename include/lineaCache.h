#ifndef LINE_CACHE_H
#define LINE_CACHE_H

// Clase que representa una línea de la memoria caché
class LineaCache {
private:
    int etiqueta;     // Parte de la dirección almacenada
    bool valida;      // Indica si la línea contiene datos válidos
    int ultimoUso;    // Contador para políticas de reemplazo

public:
    LineaCache();
    void actualizarUso(int cicloActual);
    bool estaValida() const;
    int obtenerEtiqueta() const;
    void configurarLinea(int nuevaEtiqueta, int cicloActual);
    int obtenerUltimoUso() const;
};


#endif