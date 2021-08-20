#ifndef CHARTCALCULATION_H
#define CHARTCALCULATION_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QString>
#include <QTextCodec>
#include <QList>

#define MAX_COUNT 15

class ChartCalculation : public QObject
{
    Q_OBJECT
public:
    ChartCalculation(QString filename, QObject *parent = nullptr);

    void calculate();

public slots:
    void stopChart();

signals:
    /**
     * @brief updateProgress signal of updating progress
     * @param progress is value of progress from 0 to 100 in percent. -1 is failed, 101 is finished.
     */
    void updateProgress(int progress);

    void updateResult(QList<QPair<QString, int>> resultList);
    void finished();

private:
    QString mFileName;
    QMap<QString, int> wordsChart;
    std::atomic_bool mIsRunning;

    void updateTop();
};

#endif // CHARTCALCULATION_H
