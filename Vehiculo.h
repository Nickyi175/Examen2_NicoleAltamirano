#ifndef VEHICULO_H
#define VEHICULO_H
#include <QString>

class Vehiculo
{
public:
    Vehiculo(const QString& modelo, int año, double kilometraje)
        : modelo(modelo), año(año), kilometraje(kilometraje) {}
    virtual ~Vehiculo() = default;

    virtual double calcularMantenimiento() const = 0;

    QString getModelo() const { return modelo; }
    int getAño() const { return año; }
    double getKilometraje() const { return kilometraje; }

    QString modelo;
    int año;
    float kilometraje;
};

#endif // VEHICULO_H
