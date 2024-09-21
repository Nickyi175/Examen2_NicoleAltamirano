#ifndef CAMION_H
#define CAMION_H
#include "Vehiculo.h"

class Camion : public Vehiculo
{
private:
    double capacidadCarga;
    int viajesLargos;

public:
    Camion(const QString& modelo, int anio, double kilometraje, double capacidadCarga, int viajesLargos)
        : Vehiculo(modelo, anio, kilometraje), capacidadCarga(capacidadCarga), viajesLargos(viajesLargos) {}

    double calcularMantenimiento() const override {
        double costo = 0;
        if (getKilometraje() > 100000) {
            costo += (getKilometraje() / 100000) * 5000;
        }
        return costo;
    }
};

#endif // CAMION_H
