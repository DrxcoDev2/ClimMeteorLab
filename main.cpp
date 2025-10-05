#include <iostream>
#include <cmath>
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QStyleFactory>
#include <QFile>
#include <QHBoxLayout>
#include <QFrame>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTextStream>
#include <QRegularExpression>
#include <vector>

// --- Qt Charts ---
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

#include "include/calc.h"
#include "include/header.h"
#include "include/axios.h"
#include "include/charts.h"

#include <ogrsf_frmts.h> // GDAL/OGR

// ==================== MAPA =====================
class MapWidget : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    MapWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {
        loadPolygonsFromFile("./resources/polygons.txt");
    }

protected:
    std::vector<std::vector<std::pair<double,double>>> polygons;

    void initializeGL() override {
        initializeOpenGLFunctions();
        glClearColor(0.78f, 0.78f, 0.78f, 1.0f); // gris retro
    }

    void resizeGL(int w, int h) override {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-20, 20, 30, 50, -1, 1);
        glMatrixMode(GL_MODELVIEW);
    }

    void paintGL() override {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.0f, 0.39f, 0.0f);

        for (auto &poly : polygons) {
            glBegin(GL_LINE_LOOP);
            for (auto &p : poly)
                glVertex2f(p.first, p.second);
            glEnd();
        }
    }

    void loadPolygonsFromFile(const QString &filename) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream in(&file);
        std::vector<std::pair<double,double>> currentPoly;
        QRegularExpression pointRegex("\\{\\s*([\\-\\d\\.]+)\\s*,\\s*([\\-\\d\\.]+)\\s*\\}");

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();

            if (line.startsWith("polygons.push_back({"))
                currentPoly.clear();
            else if (line.startsWith("});")) {
                if (!currentPoly.empty())
                    polygons.push_back(currentPoly);
            } else {
                QRegularExpressionMatch match = pointRegex.match(line);
                if (match.hasMatch()) {
                    double lon = match.captured(1).toDouble();
                    double lat = match.captured(2).toDouble();
                    currentPoly.emplace_back(lon, lat);
                }
            }
        }
        if (!currentPoly.empty())
            polygons.push_back(currentPoly);
    }
};



// ==================== DEPURE =====================
QString depure(QString ciudad, int dia) {
    ciudad = ciudad.toLower();
    double media = 0;

    if (ciudad == "madrid") media = 15.0;
    else if (ciudad == "barcelona") media = 16.0;
    else if (ciudad == "valencia") media = 19.0;
    else if (ciudad == "sevilla") media = 18.0;
    else if (ciudad == "zaragoza") media = 15.0;
    else return "Ciudad no reconocida.";

    if (dia < 0 || dia > 365) return "Día inválido (0-365).";

    double temp = temperature(media, 5.0, 365, 0, dia);
    return "Día " + QString::number(dia) + " - " + ciudad + ": " + QString::number(temp, 'f', 2) + " °C";
}

// ==================== MAIN =====================
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QFile file("resources/styles.scss");
    if (file.open(QFile::ReadOnly))
        app.setStyleSheet(QLatin1String(file.readAll()));

    QWidget window;
    window.resize(800, 600);

    QLabel *inputLabelCiudad = new QLabel("Ciudad:");
    QLineEdit *inputCiudad = new QLineEdit();
    QLabel *inputLabelDia = new QLabel("Día (0-365):");
    QLineEdit *inputDia = new QLineEdit();
    QLabel *inputLabelCoords = new QLabel("Coordenadas:");
    QLineEdit *inputCoords = new QLineEdit();
    QPushButton *btn = new QPushButton("Mostrar resultados");
    QLabel *label = new QLabel("Esperando...");
    label->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setHorizontalSpacing(2);
    gridLayout->setVerticalSpacing(2);

    gridLayout->addWidget(inputLabelCiudad, 0, 0);
    gridLayout->addWidget(inputCiudad, 0, 1);
    gridLayout->addWidget(inputLabelCoords, 0, 2);
    gridLayout->addWidget(inputCoords, 0, 3);
    gridLayout->addWidget(inputLabelDia, 1, 0);
    gridLayout->addWidget(inputDia, 1, 1);
    gridLayout->addWidget(btn, 2, 0);
    gridLayout->addWidget(label, 2, 1);

    inputLabelDia->setFixedWidth(80);
    inputLabelCiudad->setFixedWidth(80);
    inputLabelCoords->setFixedWidth(80);
    inputCoords->setFixedWidth(180);
    label->setFixedWidth(180);

    QWidget *header = crearHeader("ClimMeteorLab 2025 - 1.0.0", &window);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(2);
    mainLayout->setAlignment(Qt::AlignTop);

    mainLayout->addWidget(header, 0, Qt::AlignTop);
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(crearGraficoMeteorologico());
    mainLayout->addStretch(0);

    window.setLayout(mainLayout);
    window.show();

    QObject::connect(btn, &QPushButton::clicked, [&]() {
        QString ciudad = inputCiudad->text();
        int dia = inputDia->text().toInt();
        QString tempStr = depure(ciudad, dia);

        QRegularExpression re("(-?\\d+(\\.\\d+)?)(?!.*\\d)");
        QRegularExpressionMatch match = re.match(tempStr);

        double tempC = 0;
        if (match.hasMatch()) tempC = match.captured(1).toDouble();
        else {
            label->setText("Error: no se pudo leer la temperatura");
            return;
        }

        double tempK = to_kelvin(tempC);
        int Td = 25;

        label->setText(
            QString("Temperatura en %1 el día %2:\nCelsius: %3 °C\nKelvin: %4 K|\nPresión: %5 hPa|\nHR: %6 %\nViento: %7 m/s")
                .arg(ciudad)
                .arg(dia)
                .arg(tempC)
                .arg(tempK)
                .arg(pressure_ISA(100, tempC) / 100.0)
                .arg(relative_humidity(vapor_pressure_from_dewpoint(Td), tempC))
                .arg(getWindSpeed(59, 60))
        );
    });

    return app.exec();
}
