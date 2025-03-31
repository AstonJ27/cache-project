#ifndef GRAFIC_H
#define GRAFIC_H

//width 128
//height 44
#include "resultadoSimulacion.h"
#include <vector>
#include <string>
#include <map>

void generarGraficoConsolidado(const std::map<std::string, std::vector<ResultadoSimulacion>>& resultadosPorPatron);

#endif