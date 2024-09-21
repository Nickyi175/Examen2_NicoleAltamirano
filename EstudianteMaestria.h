#ifndef ESTUDIANTEMAESTRIA_H
#define ESTUDIANTEMAESTRIA_H
#include "estudiante.h"

class EstudianteMaestria : public Estudiante
{
public:
    EstudianteMaestria(QString nombre, int edad) : Estudiante(nombre, edad) {}

    double promedio() override {
        double sumaNotas = 0;
        int sumaUV = 0;
        for (int i = 0; i < calificaciones.size(); ++i) {
            if (calificaciones[i] >= 59.5) {
                sumaNotas += calificaciones[i] * unidadesValorativas[i];
            }
            sumaUV += unidadesValorativas[i];
        }
        return (sumaUV > 0) ? sumaNotas / sumaUV : 0;
    }

};

#endif // ESTUDIANTEMAESTRIA_H
