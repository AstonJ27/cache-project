#include "generadorPatrones.h"

//------------------------------------------/ Implementación de GeneradorPatrones |--------------------------------------//

vector<int> GeneradorPatrones::generarAccesos(TipoPatron patron, size_t numAccesos, int maxDireccion,int tamLinea){
    vector<int> accesos;
    random_device rd;
    mt19937 gen(rd());
    
    switch(patron){
        case SECUENCIAL: {
            //Acceso secuencial cíclico  
            
            for(size_t i = 0; i < numAccesos; ++i){
                accesos.push_back(i % maxDireccion);
            }
            break;
        }
        case ALEATORIO: {
            //Acceso completamente aleatorio

            uniform_int_distribution<> distrib(0, maxDireccion-1);
            for(size_t i = 0; i < numAccesos; ++i){
                accesos.push_back(distrib(gen));
            }
            break;
        }
        case CIRCULAR: {
            //Acceso circular en bloques de 1024 direcciones
            const int ciclo = 1024;
            for(size_t i = 0; i < numAccesos; ++i){
                accesos.push_back(i % ciclo);
            }
            break;
        }
        case ESTRIDED: {
            //Patrón con stride de 128 bytes y bloques de 16 accesos
            int base = 0;
            for(size_t i = 0; i < numAccesos; ++i){
                accesos.push_back(base + (i % 16) * 128);
                if((i + 1) % 16 == 0){
                    base += 1024; // Incrementar base cada 16 accesos
                }
            }
            break;
        }
        case CLUSTERIZADO:{
            //Distribución para posiciones base de clusters
            //Restar 256 para que el cluster completo quepa en el rango [0, maxDireccion)
            std::uniform_int_distribution<> distrib(0, maxDireccion - 256); 

            //Inicializar primer cluster en posición aleatoria válida
            int clusterActual = distrib(gen); 

            for(size_t i = 0; i < numAccesos; ++i){
                //Cambiar de cluster cada 256 accesos
                if(i % 256 == 0 && i != 0){ 
                    clusterActual = distrib(gen); //Nuevo cluster aleatorio
                }

                //Calcular dirección dentro del cluster actual
                int desplazamiento = i % 256; //Offset relativo al cluster [0-255]
                int direccion = clusterActual + desplazamiento;

                //Asegurar que la dirección no exceda maxDireccion
                if(direccion >= maxDireccion){
                    direccion = direccion % maxDireccion; //Ajuste circular
                }

                //Registrar acceso
                accesos.push_back(direccion);
            }

            break;
        }
        case MIXTO:{
            // Combinación de diferentes patrones
            uniform_int_distribution<> selector(0, 2);
            int baseSecuencial = 0;
            for(size_t i = 0; i < numAccesos; ++i){
                switch(selector(gen)){
                    case 0:{
                        //Secuencial
                        accesos.push_back(baseSecuencial++ % 1024);
                        break;
                    }
                    case 1:{
                        // Aleatorio
                        accesos.push_back(gen() % maxDireccion);
                        break;
                    }
                    case 2:{ 
                        // Strided
                        int stride = tamLinea;
                        int maxStride = maxDireccion / 16; // Evitar desbordamiento
                        accesos.push_back((i / 16) * (stride * 4) % maxStride + (i % 16) * stride);
                        break;
                    }
                }
            }
            break;
        }
        
        default: throw invalid_argument("Patrón no reconocido");
    }
    
    return accesos;
}

string GeneradorPatrones::obtenerNombrePatron(TipoPatron patron){
    switch(patron){
        case SECUENCIAL:    return "Secuencial";
        case ALEATORIO:     return "Aleatorio";
        case CIRCULAR:      return "Circular";
        case ESTRIDED:      return "Strided";
        case CLUSTERIZADO:  return "Clusterizado";
        case MIXTO:         return "Mixto";
        default:            return "Desconocido";
    }
}

vector<string> GeneradorPatrones::obtenerNombresPatrones(){
    return {
        "Secuencial",
        "Aleatorio",
        "Circular",
        "Strided",
        "Clusterizado",
        "Mixto"
    };
}