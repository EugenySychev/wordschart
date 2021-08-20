#include "chartadapt.h"
#include <QDebug>
#include <QThreadPool>
#include <QDebug>

ChartAdapt::ChartAdapt(QObject* parent)
    : QObject(parent)
    , calculation{nullptr}
{
    for (int i = 0; i < MAX_COUNT; i++)
    {
        mWords.append("");
        mCharts.append(0);
    }
}

void ChartAdapt::openClicked(QString filename)
{
    QThread *thread = new QThread();
    calculation = new ChartCalculation(filename);
    calculation->moveToThread(thread);
    connect(thread, &QThread::started, calculation, &ChartCalculation::calculate);
    connect(calculation, &ChartCalculation::updateProgress, this, &ChartAdapt::updateProgress, Qt::DirectConnection);
    connect(calculation, &ChartCalculation::updateResult, this, &ChartAdapt::updateCharts, Qt::DirectConnection);
    connect(calculation, &ChartCalculation::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::finished, thread, &ChartCalculation::deleteLater);
    thread->start();
    qDebug() << "Started";

}

void ChartAdapt::updateProgress(int progress)
{
    if (progress == 100) {
        emit finished();
    } else if (progress >= 0 && progress < 100) {
        mProgress = progress;
        emit progressChanged();
    } else {
        qDebug() << "Failed";
        emit failed();
    }
}

void ChartAdapt::updateCharts(QList<QPair<QString, int>> list)
{
    int i = 0;
    for(const auto& pair : list)
    {
        mWords[i] = pair.first;
        mCharts[i] = pair.second;
        i++;
    }
    while (i < MAX_COUNT)
    {
        mWords[i] = "";
        mCharts[i] = 0;
    }

    qDebug() << "Words " << mWords;
    emit chartChanged();
}

int ChartAdapt::getProgress()
{
    return mProgress ;
}

QVariantList ChartAdapt::getWords()
{
    return mWords;
}

QVariantList ChartAdapt::getChart()
{
    return mCharts;
}

int ChartAdapt::getMaxChart()
{
    int max = 0;
    for (const QVariant& val : mCharts)
    {
        max = max < val.toInt() ? val.toInt() : max;
    }
    return max;
}

void ChartAdapt::stopClicked()
{
    if (calculation)
        calculation->stopChart();
}

