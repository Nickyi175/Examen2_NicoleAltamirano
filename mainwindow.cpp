#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cargarEstudiantesDesdeArchivo("base_de_datos.txt");
    actualizarTabla();
    cargarVehiculos();
    cargarEmpleados();
    ui->label_horas->setVisible(false);
    ui->spinBox_horas->setVisible(false);
}

MainWindow::~MainWindow()
{
    qDeleteAll(vehiculos);
    qDeleteAll(empleados);
    delete ui;
    qDeleteAll(estudiantes);
}

void MainWindow::on_btn_notas_clicked()
{
    QString nombre = ui->lE_nombreEstudiante->text();
    int edad = ui->spinBox_edad->value();
    int numMaterias = ui->spinBox_clases->value();
    QString tipoEstudiante = ui->comboBox_gradoAcad->currentText();

    if (nombre.isEmpty() || numMaterias <= 0) {
        QMessageBox::warning(this, "Advertencia", "Debe ingresar todos los datos correctamente.");
        return;
    }


    if (!ingresarNotasYUV(numMaterias)) {
        return;
    }

    Estudiante* nuevoEstudiante = nullptr;

    if (tipoEstudiante == "Licenciatura") {
        nuevoEstudiante = new EstudianteLicenciatura(nombre, edad);
    } else if (tipoEstudiante == "Maestría") {
        nuevoEstudiante = new EstudianteMaestria(nombre, edad);
    }

    if (nuevoEstudiante) {
        for (int i = 0; i < numMaterias; ++i) {
            nuevoEstudiante->agregarMateria(materiasTemp[i], calificacionesTemp[i], unidadesValorativasTemp[i]);
        }
        estudiantes.append(nuevoEstudiante);
    }

    actualizarTabla();
}

bool MainWindow::ingresarNotasYUV(int numMaterias) {
    for (int i = 0; i < numMaterias; ++i) {
        bool ok;

        QString materia = QInputDialog::getText(this, "Ingrese Materia", "Nombre de la materia:", QLineEdit::Normal, "", &ok);
        if (!ok || materia.isEmpty()) {
            return false;
        }

        QString notaTexto = QInputDialog::getText(this, "Ingrese Nota", "Nota de la clase:", QLineEdit::Normal, "", &ok);
        if (!ok) {
            return false;
        }


        QString uvTexto = QInputDialog::getText(this, "Ingrese UV", "Unidades Valorativas de la clase:", QLineEdit::Normal, "", &ok);
        if (!ok) {
            return false;
        }

        double nota = notaTexto.toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error", "Ingrese un número válido para la nota.");
            return false;
        }

        int uv = uvTexto.toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Error", "Ingrese un número válido para las UV.");
            return false;
        }

        materiasTemp.append(materia);
        calificacionesTemp.append(nota);
        unidadesValorativasTemp.append(uv);
    }

    return true;
}


void MainWindow::on_btn_guardar_clicked()
{
    guardarEstudiantesEnArchivo("base_de_datos.txt");


}


void MainWindow::on_btn_cargar_clicked()
{
    estudiantes.clear();
    cargarEstudiantesDesdeArchivo("base_de_datos.txt");
    actualizarTabla();
}

void MainWindow::actualizarTabla() {
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Nombre" << "Edad" << "Materia" << "Nota" << "UV" << "Promedio");

    for (Estudiante* estudiante : estudiantes) {
        double promedio = estudiante->promedio();

        for (int i = 0; i < estudiante->materias.size(); ++i) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(estudiante->getNombre()));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(estudiante->getEdad())));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(estudiante->materias[i]));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(estudiante->calificaciones[i])));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(estudiante->unidadesValorativas[i])));

            if (i == 0) {
                ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(promedio)));
            } else {
                ui->tableWidget->setItem(row, 5, new QTableWidgetItem(""));
            }
        }
    }
}

void MainWindow::guardarEstudiantesEnArchivo(const QString& archivo) {
    QFile file(archivo);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (Estudiante* est : estudiantes) {
            out << est->getNombre() << "," << est->getEdad();

            for (int i = 0; i < est->calificaciones.size(); ++i) {
                out << "," << est->materias[i] << "," << est->calificaciones[i] << "," << est->unidadesValorativas[i];
            }
            out << "\n";
        }
        file.close();
    }
}

void MainWindow::cargarEstudiantesDesdeArchivo(const QString& archivo) {
    QFile file(archivo);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList datos = line.split(",");

            if (datos.size() < 3) continue;

            QString nombre = datos[0];
            int edad = datos[1].toInt();


            Estudiante* nuevoEstudiante;

            if (edad < 30) {
                nuevoEstudiante = new EstudianteLicenciatura(nombre, edad);
            } else {
                nuevoEstudiante = new EstudianteMaestria(nombre, edad);
            }

            for (int i = 2; i < datos.size(); i += 3) {
                QString materia = datos[i];
                int calificacion = datos[i + 1].toInt();
                int uv = datos[i + 2].toInt();

                nuevoEstudiante->agregarMateria(materia, calificacion, uv);
            }

            estudiantes.append(nuevoEstudiante);
        }
        file.close();
    }
}


void MainWindow::on_btn_vehiculo_clicked()
{
    QString modelo = ui->le_modelo->text();
    int año = ui->spinBox_year->value();
    double kilometraje = ui->doubleSpinBox_km->value();

    QString tipoVehiculo = ui->comboBox_vehiculo->currentText();
    Vehiculo* nuevoVehiculo = nullptr;

    if (tipoVehiculo == "Carro") {
        nuevoVehiculo = new Carro(modelo, año, kilometraje);
    } else if (tipoVehiculo == "Camión") {
        bool ok;
        double capacidadCarga = QInputDialog::getDouble(this, "Capacidad de Carga", "Ingrese la capacidad de carga:", 0, 0, 10000, 1, &ok);
        if (!ok) return;

        int viajesLargos = QInputDialog::getInt(this, "Viajes Largos", "Ingrese el número de viajes largos:", 0, 0, 100, 1, &ok);
        if (!ok) return;

        nuevoVehiculo = new Camion(modelo, año, kilometraje, capacidadCarga, viajesLargos);
    }

    if (nuevoVehiculo) {
        vehiculos.append(nuevoVehiculo);
        actualizarArchivoTexto();
        actualizarArchivoJSON();
        actualizarTabla1();
        QMessageBox::information(this, "Éxito", "Vehículo agregado.");
    }
}


void MainWindow::cargarVehiculos() {
    QFile jsonFile("vehiculos.json");
    if (!jsonFile.open(QIODevice::ReadOnly)) return;

    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
    QJsonArray jsonArray = jsonDoc.array();

    for (const auto& item : jsonArray) {
        QJsonObject jsonObj = item.toObject();

        QString modelo = jsonObj["modelo"].toString();
        int año = jsonObj["año"].toInt();
        double kilometraje = jsonObj["kilometraje"].toDouble();
        QString tipoVehiculo = jsonObj["tipoVehiculo"].toString();

        if (tipoVehiculo == "Carro") {
            vehiculos.append(new Carro(modelo, año, kilometraje));
        } else if (tipoVehiculo == "Camión") {
            double capacidadCarga = jsonObj["capacidadCarga"].toDouble();
            int viajesLargos = jsonObj["viajesLargos"].toInt();

            vehiculos.append(new Camion(modelo, año, kilometraje, capacidadCarga, viajesLargos));
        }
    }

    actualizarTabla1();
}

void MainWindow::actualizarTabla1() {
    ui->tableWidget_2->clear();
    ui->tableWidget_2->setRowCount(vehiculos.size());
    ui->tableWidget_2->setColumnCount(4);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList() << "Modelo" << "Año" << "Kilometraje" << "Costo Mantenimiento");

    for (int i = 0; i < vehiculos.size(); ++i) {
        ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(vehiculos[i]->getModelo()));
        ui->tableWidget_2->setItem(i, 1, new QTableWidgetItem(QString::number(vehiculos[i]->getAño())));
        ui->tableWidget_2->setItem(i, 2, new QTableWidgetItem(QString::number(vehiculos[i]->getKilometraje())));
        ui->tableWidget_2->setItem(i, 3, new QTableWidgetItem(QString::number(vehiculos[i]->calcularMantenimiento())));
    }
}

void MainWindow::on_btn_guardarVehi_clicked()
{
    QFile file("vehiculos.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonArray jsonArray;

        for (const Vehiculo* vehiculo : vehiculos) {
            QJsonObject jsonObject;
            jsonObject["modelo"] = vehiculo->getModelo();
            jsonObject["año"] = vehiculo->getAño();
            jsonObject["kilometraje"] = vehiculo->getKilometraje();
            jsonObject["tipoVehiculo"] = (dynamic_cast<const Carro*>(vehiculo) ? "Carro" : "Camión");
            jsonObject["mantenimiento"] = vehiculo->calcularMantenimiento();
            jsonArray.append(jsonObject);
        }

        QJsonDocument jsonDoc(jsonArray);
        file.write(jsonDoc.toJson());
        file.close();
        QMessageBox::information(this, "Éxito", "Vehículos guardados correctamente.");
    } else {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo.");
    }
}


void MainWindow::on_btn_cargarVehi_clicked()
{
    QFile file("vehiculos.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
        if (jsonDoc.isArray()) {
            qDeleteAll(vehiculos);
            vehiculos.clear();

            for (const QJsonValue &value : jsonDoc.array()) {
                QJsonObject jsonObject = value.toObject();
                QString modelo = jsonObject["modelo"].toString();
                int año = jsonObject["año"].toInt();
                double kilometraje = jsonObject["kilometraje"].toDouble();

                if (jsonObject["tipoVehiculo"].toString() == "Carro") {
                    vehiculos.append(new Carro(modelo, año, kilometraje));
                } else {
                    double capacidadCarga = jsonObject["capacidadCarga"].toDouble();
                    int viajesLargos = jsonObject["viajesLargos"].toInt();
                    vehiculos.append(new Camion(modelo, año, kilometraje, capacidadCarga, viajesLargos));
                }
            }
            actualizarTabla1();
            QMessageBox::information(this, "Éxito", "Vehículos cargados correctamente.");
        }
    } else {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo.");
    }
}


void MainWindow::on_btn_Eliminar_clicked()
{
    QStringList vehiculosList;
    for (const auto& vehiculo : vehiculos) {
        vehiculosList << QString("%1 (Año: %2), (%3 Km)").arg(vehiculo->getModelo()).arg(vehiculo->getAño()).arg(vehiculo->getKilometraje());
    }

    bool ok;
    QString selectedVehiculo = QInputDialog::getItem(this, "Eliminar Vehículo", "Seleccione un vehículo:", vehiculosList, 0, false, &ok);
    if (ok && !selectedVehiculo.isEmpty()) {
        for (int i = 0; i < vehiculos.size(); ++i) {
            if (selectedVehiculo.startsWith(vehiculos[i]->getModelo()) && selectedVehiculo.contains(QString::number(vehiculos[i]->getAño()))  && selectedVehiculo.contains(QString::number(vehiculos[i]->getKilometraje()))) {
                delete vehiculos[i];
                vehiculos.removeAt(i);
                break;
            }
        }
        actualizarArchivoTexto();
        actualizarArchivoJSON();
        actualizarTabla1();
        QMessageBox::information(this, "Éxito", "Vehículo eliminado.");
    }
}

void MainWindow::actualizarArchivoTexto() {
    QFile file("vehiculos.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const auto& vehiculo : vehiculos) {
            out << vehiculo->getModelo() << ","
                << vehiculo->getAño() << ","
                << vehiculo->getKilometraje() << ","
                << vehiculo->calcularMantenimiento() << "\n";
        }
        file.close();
    } else {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo de texto.");
    }
}

void MainWindow::actualizarArchivoJSON() {
    QJsonArray jsonArray;

    for (const auto& vehiculo : vehiculos) {
        QJsonObject jsonObject;
        jsonObject["modelo"] = vehiculo->getModelo();
        jsonObject["año"] = vehiculo->getAño();
        jsonObject["kilometraje"] = vehiculo->getKilometraje();
        jsonObject["tipoVehiculo"] = (dynamic_cast<const Carro*>(vehiculo) ? "Carro" : "Camión");
        jsonObject["costoMantenimiento"] = vehiculo->calcularMantenimiento();
        jsonArray.append(jsonObject);
    }

    QJsonDocument jsonDoc(jsonArray);
    QFile file("vehiculos.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(jsonDoc.toJson());
        file.close();
    } else {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo JSON.");
    }
}

void MainWindow::on_comboBox_empleado_currentIndexChanged(int index)
{
    if (index == 1) {
        ui->label_horas->setVisible(true);
        ui->spinBox_horas->setVisible(true);
    } else {
        ui->label_horas->setVisible(false);
        ui->spinBox_horas->setVisible(false);
    }
}


void MainWindow::guardarEmpleados() {
    QFile file("empleados.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const auto& empleado : empleados) {
            out << empleado->getNombre() << "," << empleado->getEdad() << ","
                << empleado->calcularSalario() << ","
                << (dynamic_cast<EmpleadoContratado*>(empleado) ? "Contratado" : "Fijo") << "\n";
        }
        file.close();
    }
}

void MainWindow::cargarEmpleados() {
    QFile file("empleados.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        empleados.clear();
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            QString nombre = fields[0];
            int edad = fields[1].toInt();
            float salario = fields[2].toFloat();
            QString tipo = fields[3];

            if (tipo == "Contratado") {
                EmpleadoContratado* emp = new EmpleadoContratado(nombre, edad, salario);
                empleados.append(emp);
            } else {
                EmpleadoFijo* emp = new EmpleadoFijo(nombre, edad, salario);
                empleados.append(emp);
            }
        }
        file.close();
    }
}

void MainWindow::on_btn_empleado_clicked()
{
    QString nombre = ui->le_nombreEmpleado->text();
    int edad = ui->spinBox_edadEmpleado->value();
    QString tipo = ui->comboBox_empleado->currentText();
    float salario;

    if (tipo == "Fijo") {
        salario = ui->doubleSpinBox_salario->value();
        empleados.append(new EmpleadoFijo(nombre, edad, salario));
    } else {
        salario = ui->doubleSpinBox_tarifa->value();
        int horas = ui->spinBox_horas->value();
        EmpleadoContratado* empleado = new EmpleadoContratado(nombre, edad, salario);
        empleado->setHorasTrabajadas(horas);
        empleados.append(empleado);
    }
    actualizarTabla();
    QMessageBox::information(this, "Éxito", "Empleado agregado.");
}

