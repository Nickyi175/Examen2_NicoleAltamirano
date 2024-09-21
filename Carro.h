#ifndef CARRO_H
#define CARRO_H
#include "Vehiculo.h"

class Carro : public Vehiculo
{
public:
    Carro(const QString& modelo, int anio, double kilometraje)
        : Vehiculo(modelo, anio, kilometraje) {}

    double calcularMantenimiento() const override {
        return (kilometraje / 5000) * 1000;
    }
};

#endif // CARRO_H
