#ifndef EMPLEADOCONTRATADO_H
#define EMPLEADOCONTRATADO_H
#include "Empleado.h"

class EmpleadoContratado : public Empleado
{
private:
    int horasTrabajadas;
    float tarifaPorHora;

public:
    EmpleadoContratado(QString nombre, int edad, float tarifaPorHora)
        : Empleado(nombre, edad, 0), tarifaPorHora(tarifaPorHora), horasTrabajadas(0) {}

    void setHorasTrabajadas(int horas) { horasTrabajadas = horas; }
    float calcularSalario() override {
        return tarifaPorHora * horasTrabajadas;
    }
};

#endif // EMPLEADOCONTRATADO_H
