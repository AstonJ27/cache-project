# Simulación de Memoria Caché con Ajuste Dinámico de Línea

[![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![BSD-3-Clause License](https://img.shields.io/badge/Licencia-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Tabulate License](https://img.shields.io/badge/Tabulate-MIT-green)](https://github.com/p-ranav/tabulate/blob/master/LICENSE)

Un simulador de memoria caché que implementa un algoritmo de ajuste dinámico del tamaño de línea, evaluando diferentes patrones de acceso y políticas de reemplazo, y calcula la tasa de aciertos
en base al tamaño de linea

## Características Principales

- 🧠 Simulación de caché con múltiples niveles de asociatividad
- ⚡ Algoritmo de ajuste dinámico de tamaño de línea (16B-128B)
- 📊 Generación de métricas de rendimiento
- 🎨 Visualización de resultados con gráficos de barras
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

## 📚 Bibliotecas de Terceros

### Tabulate
- **Función**: Generación de tablas en consola (tambien usado para crear graficas de barra)
- **Repositorio**: [p-ranav/tabulate](https://github.com/p-ranav/tabulate)
- **Licencia**: [MIT](THIRD-PARTY-LICENSES#tabulate)
- **Uso en código**:

  ```cpp
  //grafic.h
  #ifndef GRAFIC_H
  #define GRAFIC_H

  #include "resultadoSimulacion.h"    //objeto que guarda los resultados obtenidos
  #include <vector>
  #include <string>
  #include <map>

  void generarGraficoConsolidado(const std::map<std::string, std::vector<ResultadoSimulacion>>& resultadosPorPatron);

  #endif  //GRAFIC_H  
  ```
  **Para crear la leyenda en base al tamaño de linea:**
  ```cpp
  //Leyenda del grafico en relacion al tamaño de linea
  Table leyenda;
  leyenda.add_row({"Tamaño", "Color Representativo"});
  for (const auto& [tam, color] : COLORES_TAMANOS) {
    leyenda.add_row({std::to_string(tam) + "B", "    "})
      .format()
      .hide_border()
      .font_color(Color::white);

    leyenda[leyenda.size()-1][1].format().background_color(color);
  }
  std::cout << "\n\033[1;33mLEYENDA:\033[0m\n" << leyenda << "\n";
  ```

  **Para hacer las grâficas:**

  ```cpp
    //Función para crear gráficos base
    auto crearGraficoBase = [&](bool esTasa){
        Table chart;
        chart.format()
            .font_color(Color::white)
            .padding_left(0)
            .padding_right(0)
            .hide_border()
            .multi_byte_characters(true);

        const double max_global = esTasa ? 100.0 : max_tiempo;
        const auto niveles = crearNiveles(esTasa, max_global);

        for(double nivel : niveles){
            Table::Row_t fila;
            std::string etiqueta;
            etiqueta = std::to_string(static_cast<int>(nivel));

            if(esTasa){
              etiqueta = etiqueta + "%";
            }
            
            if(etiqueta.length() < 4){
                etiqueta = " " + etiqueta;
            }
            fila.push_back(etiqueta);

            // 4 espacios por barra + 3 de separación
            for (const auto& [patron, resultados] : resultadosPorPatron) {
                for (size_t i = 0; i < resultados.size(); ++i) {
                    fila.push_back("    ");
                }
                fila.push_back("   "); // Aumentar espacio entre grupos
            }
            chart.add_row(fila);
        }
        return chart;
    };

    //Construir gráficos base
    Table chart_tasa = crearGraficoBase(true);
    Table chart_tiempo = crearGraficoBase(false);

    //Función para aplicar estilos
    auto aplicarEstilos = [&](Table& chart, bool esTasa) {
        const double max_global = esTasa ? 100.0 : max_tiempo;
        const auto niveles = crearNiveles(esTasa, max_global);
        size_t col_actual = 1;

        for(const auto& [patron, resultados] : resultadosPorPatron){
            for(const auto& res : resultados){
                const Color color = COLORES_TAMANOS.at(res.obtenerTamanoLinea());
                const double valor = esTasa ? 
                    res.obtenerTasaAciertos() * 100 : 
                    res.obtenerTiempoPromedio();

                for(size_t i = 0; i < niveles.size(); ++i){
                    const double nivel = niveles[i];
                    bool pinta = valor >= nivel;

                    if(pinta && i < chart.size()){
                        chart[i][col_actual]
                            .format()
                            .background_color(color)
                            .width(7); // Ancho consistente
                    }
                }
                col_actual++;
            }
            col_actual += 1; // Saltar espacio entre grupos
        }
    };

    //Aplicar estilos
    aplicarEstilos(chart_tasa, true);
    aplicarEstilos(chart_tiempo, false);
  ```
  
