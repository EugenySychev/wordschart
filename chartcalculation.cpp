#include "chartcalculation.h"
#include <QApplication>
#include <QDebug>

ChartCalculation::ChartCalculation(QString filename, bool detailing, QObject *parent)
    : QObject(parent)
    , mFileName{filename}
    , mEnabledDetailing{detailing}
{
}

void ChartCalculation::calculate()
{
    if (mFileName.contains("file://"))
    {
        mFileName.remove("file://");
    }

    QFile file(mFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error opening " << mFileName << " error: "  << file.errorString();
        emit updateProgress(-1);
        return;
    }

    int filesize = file.size();

    mIsRunning = true;
    mMinimum = 0;
    int processingPerc = 0;
    int prevPerc = 0;
    while(!file.atEnd() && mIsRunning)
    {
        try {
            QString strData = QString(file.readLine());
            auto listData = strData.split(QRegExp("\\W+"), Qt::SkipEmptyParts);
            foreach(const QString &word, listData)
            {
                wordsChart[word]++;
            }
            processingPerc = 100 * file.pos() / filesize;

            emit updateProgress(processingPerc);
            QApplication::processEvents();
            if (mEnabledDetailing && processingPerc > prevPerc)
            {
                prevPerc = processingPerc;
                updateTop();
            }
        }  catch (...) {
            emit updateProgress(-1);
            file.close();
            return;
        }
    }
    if (!mEnabledDetailing)
    {
        updateTop();
    }
    emit updateProgress(100);
    emit finished();
}

void ChartCalculation::updateTop()
{
    QList<QPair<QString, int>> pairList;
    auto keys = wordsChart.keys();

    // go through words map
    for(const auto& word : keys)
    {
        // Select only more then previous minimum of top-15 words
        if (wordsChart[word] >= mMinimum)
            pairList.append(qMakePair(word, wordsChart[word]));
    }

    // sorting by quantity
    std::sort(pairList.begin(), pairList.end(),
              [](const QPair<QString, int> &a, const QPair<QString, int> &b)
    {
        return a.second > b.second;
    });

    // crop top-15 words
    if (pairList.size() > MAX_COUNT)
    {
        pairList = pairList.mid(0, MAX_COUNT);
    }

    // get minimum of top-15 for next 'counts'
    mMinimum = pairList.back().second;

    // sorting by alphabet
    std::sort(pairList.begin(), pairList.end(),
              [](const QPair<QString, int> &a, const QPair<QString, int> &b)
    {
        return a.first.toLower() > b.first.toLower();
    });

    emit updateResult(pairList);
    QApplication::processEvents();
}

void ChartCalculation::stopChart()
{
    mIsRunning = false;
}


