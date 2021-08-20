#include "chartcalculation.h"
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
    mMinimum = 0;

    QByteArray chunk;
    chunk = file.read(CHUNK_SIZE);

    while (!chunk.isEmpty() && mIsRunning)
    {
        try {
            QTextCodec *codec = QTextCodec::codecForName("UTF-8");
            QString strData{codec->toUnicode(chunk)};
            auto listData = strData.split(QRegExp("\\W+"), Qt::SkipEmptyParts);
            foreach(const QString &word, listData)
            {
                wordsChart[word]++;
            }
            processedDataSize += chunk.size();

            emit updateProgress(100 * processedDataSize / filesize);

            if (mEnabledDetailing)
            {
                updateTop();
            }
        }  catch (...) {
            emit updateProgress(-1);
            file.close();
            return;
        }
        chunk = file.read(CHUNK_SIZE);

    }
    if (!mEnabledDetailing)
    {
        updateTop();
    }
    emit updateProgress(100);

}

void ChartCalculation::updateTop()
{
    QList<QPair<QString, int>> pairList;
    auto keys = wordsChart.keys();
    for(const auto& word : keys)
    {
        if (wordsChart[word] >= mMinimum)
            pairList.append(qMakePair(word, wordsChart[word]));
    }

    std::sort(pairList.begin(), pairList.end(),
              [](const QPair<QString, int> &a, const QPair<QString, int> &b)
    {
        return a.second > b.second;
    });


    if (pairList.size() > MAX_COUNT)
    {
        pairList = pairList.mid(0, MAX_COUNT);
    }
    mMinimum = pairList.back().second;
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


