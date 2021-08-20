#include "chartcalculation.h"
#include <QDebug>
ChartCalculation::ChartCalculation(QString filename, QObject *parent)
    : QObject(parent)
    , mFileName{filename}
{
}

void ChartCalculation::calculate()
{
    if (mFileName.contains("file://"))
        mFileName.remove("file://");
    QFile file(mFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error opening " << mFileName << " error: "  << file.errorString();
        emit updateProgress(-1);
        return;
    }
    int filesize = file.size();
    int processedDataSize = 0;
    mIsRunning = true;
    qDebug() << "Begin to count";
    while(!file.atEnd() && mIsRunning)
    {
        try {
            QString strData = QString(file.readLine());
            auto listData = strData.split(QRegExp("\\W+"), Qt::SkipEmptyParts);
            foreach(const QString &word, listData)
            {
                wordsChart[word]++;
            }
            processedDataSize += strData.size();
            qDebug() << "Processed " << processedDataSize << " from " << filesize;

            emit updateProgress(100 * processedDataSize / filesize);
        }  catch (...) {
            emit updateProgress(-1);
            file.close();
            return;
        }
    }
    updateTop();

}

void ChartCalculation::updateTop()
{
    QList<QPair<QString, int>> pairList;
    auto keys = wordsChart.keys();
    for(const auto& word : keys)
    {
        pairList.append(qMakePair(word, wordsChart[word]));
    }

    std::sort(pairList.begin(), pairList.end(),
              [](const QPair<QString, int> &a, const QPair<QString, int> &b)
    {
        return a.second > b.second;
    });

    if (pairList.size() > 15)
    {
        pairList = pairList.mid(0, 15);
    }

    std::sort(pairList.begin(), pairList.end(),
              [](const QPair<QString, int> &a, const QPair<QString, int> &b)
    {
        return a.first.toLower() > b.first.toLower();
    });

    emit updateResult(pairList);
}

void ChartCalculation::stopChart()
{
    mIsRunning = false;
}


