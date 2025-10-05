#ifndef CHARTS_H
#define CHARTS_H

// ==================== GRÁFICO =====================
// Usa nombres calificados para evitar depender de `using namespace`
QWidget* crearGraficoMeteorologico(QWidget* parent = nullptr) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // Qt6 no usa el namespace QtCharts
    auto* maxTempSeries = new QLineSeries();
    auto* minTempSeries = new QLineSeries();
    auto* rainfallSet = new QBarSet("Rainfall");
    auto* barSeries = new QBarSeries();
    auto* axisX = new QBarCategoryAxis();
    auto* axisYTemp = new QValueAxis();
    auto* axisYRain = new QValueAxis();
    auto* chart = new QChart();
    auto* chartView = new QChartView(chart, parent);
#else
    // Qt5 usa el namespace QtCharts
    auto* maxTempSeries = new QtCharts::QLineSeries();
    auto* minTempSeries = new QtCharts::QLineSeries();
    auto* rainfallSet = new QtCharts::QBarSet("Rainfall");
    auto* barSeries = new QtCharts::QBarSeries();
    auto* axisX = new QtCharts::QBarCategoryAxis();
    auto* axisYTemp = new QtCharts::QValueAxis();
    auto* axisYRain = new QtCharts::QValueAxis();
    auto* chart = new QtCharts::QChart();
    auto* chartView = new QtCharts::QChartView(chart, parent);
#endif

    *rainfallSet << 5 << 8 << 4;

    maxTempSeries->append(0, 25);
    maxTempSeries->append(1, 28);
    maxTempSeries->append(2, 27);

    minTempSeries->append(0, 15);
    minTempSeries->append(1, 18);
    minTempSeries->append(2, 17);

    barSeries->append(rainfallSet);
    axisX->append({"Day 1", "Day 2", "Day 3"});
    axisYTemp->setTitleText("Temperature (°C)");
    axisYRain->setTitleText("Rainfall (mm)");

    chart->addSeries(maxTempSeries);
    chart->addSeries(minTempSeries);
    chart->addSeries(barSeries);
    chart->setTitle("Weather Overview");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisYTemp, Qt::AlignLeft);
    chart->addAxis(axisYRain, Qt::AlignRight);

    maxTempSeries->attachAxis(axisX);
    maxTempSeries->attachAxis(axisYTemp);
    minTempSeries->attachAxis(axisX);
    minTempSeries->attachAxis(axisYTemp);
    barSeries->attachAxis(axisX);
    barSeries->attachAxis(axisYRain);

    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

#endif