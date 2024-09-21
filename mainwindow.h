#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "estudiantelicenciatura.h"
#include "estudiantemaestria.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "Carro.h"
#include "Camion.h"
#include "EmpleadoFijo.h"
#include "EmpleadoContratado.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_notas_clicked();

    void on_btn_guardar_clicked();

    void on_btn_cargar_clicked();

    void on_btn_vehiculo_clicked();

    void on_btn_guardarVehi_clicked();

    void on_btn_cargarVehi_clicked();

    void on_btn_Eliminar_clicked();

    void on_comboBox_empleado_currentIndexChanged(int index);

    void on_btn_empleado_clicked();

private:
    Ui::MainWindow *ui;
    QVector<Estudiante*> estudiantes;
    QVector<int> calificacionesTemp;
    QVector<int> unidadesValorativasTemp;
    QVector<QString> materiasTemp;
    QVector<Vehiculo*> vehiculos;
    void actualizarArchivoTexto();
    bool ingresarNotasYUV(int numMaterias);
    void actualizarTabla();
    void actualizarTabla1();
    void actualizarArchivoJSON();
    void cargarVehiculos();
    void guardarEstudiantesEnArchivo(const QString& archivo);
    void cargarEstudiantesDesdeArchivo(const QString& archivo);
    QVector<Empleado*> empleados;
    void actualizarTabla();
    void guardarEmpleados();
    void cargarEmpleados();
    void actualizarTabla3();
};
#endif // MAINWINDOW_H
