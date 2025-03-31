# Simulación de Memoria Caché con Ajuste Dinámico de Línea

Un simulador de memoria caché que implementa un algoritmo de ajuste dinámico del tamaño de línea, evaluando diferentes patrones de acceso y políticas de reemplazo.


## Características Principales

- 🧠 Simulación de caché con múltiples niveles de asociatividad
- ⚡ Algoritmo de ajuste dinámico de tamaño de línea (16B-128B)
- 📊 Generación de métricas de rendimiento
- 🎨 Visualización de resultados con gráficos interactivos
- 🧩 Soporte para las políticas de reemplazo:
  - LRU (Least Recently Used)
  - Aleatorio
   
## 🌀 Patrones de Acceso Implementados

### 1. **SECUENCIAL**
**Definición**: Acceso lineal cíclico a direcciones.  
**Comportamiento**:  
- Direcciones consecutivas: `0 → 1 → 2 → ... → max-1 → 0 → 1...`  
- Localidad espacial perfecta  
- Ejemplo: Procesamiento de arrays en orden  

### 2. **ALEATORIO**  
**Definición**: Distribución uniforme en todo el espacio de direcciones.  
**Comportamiento**:  
- Sin patrones predecibles (`dirección = rand() % max`)  
- Baja localidad espacial/temporal  
- Ejemplo: Acceso a datos dispersos  

### 3. **CIRCULAR**  
**Definición**: Repetición periódica de bloques de 1024 direcciones.  
**Comportamiento**:  
- Ciclo fijo: `0-1023 → 0-1023 → ...`  
- Localidad temporal periódica  
- Ejemplo: Buffers de tamaño fijo  

### 4. **ESTRIDED**  
**Definición**: Saltos regulares de 128 bytes + bloques de 16 elementos.  
**Comportamiento**:  
- Patrón: `base + 0*128, base + 1*128, ..., base + 15*128`  
- Cada 16 accesos: `base += 1024`  
- Ejemplo: Acceso a columnas en matrices 2D  

### 5. **CLUSTERIZADO**  
**Definición**: Grupos de 256 accesos en zonas aleatorias.  
**Comportamiento**:  
- Cluster actual: `dirección ∈ [cluster, cluster+255]`  
- Cambio de cluster cada 256 accesos  
- Ejemplo: Procesamiento por lotes en memoria no contigua  

### 6. **MIXTO**  
**Definición**: Combinación probabilística de 3 patrones.  
**Distribución**:  
- 33% Secuencial: `0,1,2...1023`  
- 33% Aleatorio: Direcciones no correlacionadas  
- 33% Strided: Saltos alineados al tamaño de línea  
- Ejemplo: Simulación de cargas de trabajo realistas  

---

> **Nota Técnica**:  
> Todos los patrones incluyen mecanismos de ajuste circular (`% maxDireccion`) para garantizar direcciones válidas dentro del rango configurado.
## Estructura del Proyecto


## 📚 Bibliotecas de Terceros

### Tabulate
Parte de la interfaz gráfica/formatos de tablas se basa en la biblioteca [Tabulate](https://github.com/p-ranav/tabulate) de **p-ranav**, 
utilizada bajo licencia MIT.

```cpp
// Ejemplo de uso en tu código
#include <tabulate/table.hpp>
```


https://github.com/p-ranav/tabulate
