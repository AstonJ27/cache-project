#include "grafic.h"
#include <tabulate/table.hpp>
#include <algorithm>
#include <cmath>

using namespace tabulate;

void generarGraficoConsolidado(const std::map<std::string, std::vector<ResultadoSimulacion>>& resultadosPorPatron) {
    const std::map<int, Color> COLORES_TAMANOS = {
        {16, Color::green},
        {32, Color::yellow},
        {64, Color::blue},
        {128, Color::red}
    };
    //Leyenda

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

    // 1. Calcular máximo para tiempo
    double max_tiempo = 0.0;
    for (const auto& [patron, resultados] : resultadosPorPatron) {
        for (const auto& res : resultados) {
            max_tiempo = std::max(max_tiempo, res.obtenerTiempoPromedio());
        }
    }
    if (max_tiempo == 0.0){
        max_tiempo = 1.0;
    }
    // 2. Función para crear niveles del eje Y 
    auto crearNiveles = [](bool esTasa, double max_global) {
        std::vector<double> niveles;
        if (esTasa) {
            for (int i = 100; i >= 5; i -= 5){
                niveles.push_back(i);
            }
            niveles.push_back(1.0);
        } else {
            for (int i = 100; i >= 5; i -= 5) {
                niveles.push_back(max_global * (i / 100.0));

            }
            niveles.push_back(1.0);
        }
        return niveles;
    };

    // 3. Función para crear gráficos base (modificada)
    auto crearGraficoBase = [&](bool esTasa) {
        Table chart;
        chart.format()
            .font_color(Color::white)
            .padding_left(0)
            .padding_right(0)
            .hide_border()
            .multi_byte_characters(true);

        const double max_global = esTasa ? 100.0 : max_tiempo;
        const auto niveles = crearNiveles(esTasa, max_global);

        for (double nivel : niveles) {
            Table::Row_t fila;
            std::string etiqueta;
            
            if (esTasa) {
                if (nivel == 1.0) etiqueta = " 1%";
                else etiqueta = std::to_string(static_cast<int>(nivel)) + "%";
            } else {
                etiqueta = std::to_string(static_cast<int>(nivel));
            }
            
            if (etiqueta.length() < 4) etiqueta = " " + etiqueta;
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

    // 4. Construir gráficos base
    Table chart_tasa = crearGraficoBase(true);
    Table chart_tiempo = crearGraficoBase(false);

    // 5. Función para aplicar estilos (corregida para 1%)
    auto aplicarEstilos = [&](Table& chart, bool esTasa) {
        const double max_global = esTasa ? 100.0 : max_tiempo;
        const auto niveles = crearNiveles(esTasa, max_global);
        size_t col_actual = 1;

        for (const auto& [patron, resultados] : resultadosPorPatron) {
            for (const auto& res : resultados) {
                const Color color = COLORES_TAMANOS.at(res.obtenerTamanoLinea());
                const double valor = esTasa ? 
                    res.obtenerTasaAciertos() * 100 : 
                    res.obtenerTiempoPromedio();

                for (size_t i = 0; i < niveles.size(); ++i) {
                    const double nivel = niveles[i];
                    bool pinta = valor >= nivel;

                    if (pinta && i < chart.size()) {
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

    // 6. Aplicar estilos
    aplicarEstilos(chart_tasa, true);
    aplicarEstilos(chart_tiempo, false);

    // 7. Crear tabla de nombres de patrones separada
    auto crearTablaNombres = [&]() {
        Table nombres;
        nombres.format()
            .hide_border()
            .padding_left(0)
            .padding_right(0);

        Table::Row_t fila;
        for (const auto& [patron, resultados] : resultadosPorPatron) {
            fila.push_back("            " + patron + "          ");
        }
        nombres.add_row(fila);
        
        // Añadir separación entre nombres
        for (auto& cell : nombres[0]) {
            cell.format()
                .border_left(" ")
                .border_right(" ")
                .font_align(FontAlign::center);
        }
        return nombres;
    };

    // 8. Imprimir gráficos con nombres separados
    std::cout <<"                           ";
    std::cout << "\033[1;4:2;33m" << "GRAFICO CONSOLIDADO" <<"\e[0m\n";
    
    std::cout << "\n\033[1;34mTASAS DE ACIERTO (1% - 100%)\033[0m\n\n"
              << chart_tasa << "\n"
              << crearTablaNombres() << "\n";
    //pausar la ejecucion
    std::cout << "\nPresione Enter para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpiar buffer
    std::cout << std::endl;
    
    std::cout << "\n\033[1;34mTIEMPOS PROMEDIO\033[0m\n\n" 
              << chart_tiempo << "\n"
              << crearTablaNombres() << "\n";


}