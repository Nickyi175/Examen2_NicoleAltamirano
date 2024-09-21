#ifndef EMPLEADOFIJO_H
#define EMPLEADOFIJO_H
#include "Empleado.h"

class EmpleadoFijo : public Empleado
{
public:
    EmpleadoFijo(QString nombre, int edad, float salario)
        : Empleado(nombre, edad, salario) {}

    float calcularSalario() override {
        return salario;
    }
};

#endif // EMPLEADOFIJO_H
