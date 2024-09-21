#ifndef ESTUDIANTELICENCIATURA_H
#define ESTUDIANTELICENCIATURA_H
#include "estudiante.h"

class EstudianteLicenciatura : public Estudiante
{
public:
    EstudianteLicenciatura(QString nombre, int edad) : Estudiante(nombre, edad) {}

    double promedio() override {
        double sumaNotas = 0;
        int sumaUV = 0;
        for (int i = 0; i < calificaciones.size(); ++i) {
            sumaNotas += calificaciones[i] * unidadesValorativas[i];
            sumaUV += unidadesValorativas[i];
        }
        return (sumaUV > 0) ? sumaNotas / sumaUV : 0;
    }

};

#endif // ESTUDIANTELICENCIATURA_H
