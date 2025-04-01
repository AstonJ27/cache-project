#ifndef GRAFIC_H
#define GRAFIC_H

#include "resultadoSimulacion.h"    //objeto que guarda los resultados obtenidos
#include <vector>
#include <string>
#include <map>

void generarGraficoConsolidado(const std::map<std::string, std::vector<ResultadoSimulacion>>& resultadosPorPatron);

#endif  //GRAFIC_H