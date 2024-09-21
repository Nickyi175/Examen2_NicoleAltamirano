#ifndef ESTUDIANTE_H
#define ESTUDIANTE_H
#include <QString>
#include <QVector>

class Estudiante
{
public:

    Estudiante(QString nombre, int edad) : nombre(nombre), edad(edad) {}

    virtual ~Estudiante() {}

    QString getNombre() const { return nombre; }
    int getEdad() const { return edad; }

    virtual double promedio() = 0;

    void agregarMateria(const QString& materia, int calificacion, int uv) {
        materias.append(materia);
        calificaciones.append(calificacion);
        unidadesValorativas.append(uv);
    }

    QString nombre;
    int edad;
    QVector<QString> materias;
    QVector<int> calificaciones;
    QVector<int> unidadesValorativas;

//protected:


};

#endif // ESTUDIANTE_H
