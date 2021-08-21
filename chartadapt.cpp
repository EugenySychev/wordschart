#include "chartadapt.h"
#include <QDebug>
#include <QThreadPool>

ChartAdapt::ChartAdapt(QObject* parent)
    : QObject(parent)
    , calculation{nullptr}
    , mProgress{0}
    , mEnabledDetailing{true}
{
    for (int i = 0; i < MAX_COUNT; i++)
    {
        mWords.append("");
        mCharts.append(0);
    }
    mTimer.setInterval(100);
    connect(&mTimer, &QTimer::timeout, this, &ChartAdapt::chartChanged);
    connect(&mTimer, &QTimer::timeout, this, &ChartAdapt::progressChanged);
}

void ChartAdapt::openClicked(QString filename)
{
    QThread *thread = new QThread();
    calculation = new ChartCalculation(filename, mEnabledDetailing);
    calculation->moveToThread(thread);

    connect(thread, &QThread::started, calculation, &ChartCalculation::calculate);
    connect(thread, &QThread::finished, thread, &ChartCalculation::deleteLater);

    connect(calculation, &ChartCalculation::updateProgress, this, &ChartAdapt::updateProgress, Qt::DirectConnection);
    connect(calculation, &ChartCalculation::updateResult, this, &ChartAdapt::updateCharts, Qt::DirectConnection);
    connect(calculation, &ChartCalculation::finished, &mTimer, &QTimer::stop);
    connect(calculation, &ChartCalculation::finished, this, &ChartAdapt::finished);

    thread->start();
    mTimer.start();
}

void ChartAdapt::updateProgress(int progress)
{
    if (progress >= 0 && progress <= 100) {
        mProgress = progress;
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
        mWords[i] = pair.first + QString(" (%1)").arg(pair.second);
        mCharts[i] = pair.second;
        i++;
    }
    mMutex.lock();
    while (i < MAX_COUNT)
    {
        mWords[i] = "";
        mCharts[i] = 0;
        i++;
    }
    mMutex.unlock();
}

int ChartAdapt::getProgress()
{
    return mProgress ;
}

QVariantList ChartAdapt::getWords()
{

    mMutex.lock();
    QVariantList list{mWords};
    mMutex.unlock();
    return list;
}

QVariantList ChartAdapt::getChart()
{
    mMutex.lock();
    QVariantList list{mCharts};
    mMutex.unlock();
    return list;
}

int ChartAdapt::getMaxChart()
{
    int max = 0;
    for (const QVariant& val : mCharts)
    {
        max = max < val.toInt() ? val.toInt() : max;
    }
    QString strMax = QString::number(max);
    int lmax = strMax.length();

    if (lmax > 2)
    {
        max = (QString::number(QString::number(max).left(2).toInt() + 1).append(QString().fill('0', lmax - 2))).toInt();
    }
    return max;
}

void ChartAdapt::stopClicked()
{
    if (calculation)
    {
        calculation->stopChart();
    }
}

void ChartAdapt::changeDetailing(bool val)
{
    mEnabledDetailing = val;
}

void ChartAdapt::finished()
{
    mProgress = 100;
    emit progressChanged();
    emit chartChanged();
}

