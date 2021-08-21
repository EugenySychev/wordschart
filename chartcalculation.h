#ifndef CHARTCALCULATION_H
#define CHARTCALCULATION_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QString>
#include <QTextCodec>
#include <QList>

#define MAX_COUNT 15
#define CHUNK_SIZE 4096

class ChartCalculation : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief ChartCalculation is a calculation class
     * @param filename is a name of the file which analyzing
     * @param detailing for build top every chunk of file. If false - result calculated only on finish file reading
     * @param parent is parent object pointer
     */
    ChartCalculation(QString filename, bool detailing, QObject *parent = nullptr);

    /**
     * @brief calculate function for reading file and calculating words chart in file
     */
    void calculate();

public slots:
    /**
     * @brief stopChart is a function for stop reading and calculation
     */
    void stopChart();

signals:
    /**
     * @brief updateProgress signal of updating progress
     * @param progress is value of progress from 0 to 100 in percent. -1 is failed, 101 is finished.
     */
    void updateProgress(int progress);

    /**
     * @brief updateResult is emitting when some kind of results ready,
     * @param resultList list of top-15 words
     */
    void updateResult(QList<QPair<QString, int>> resultList);

    /**
     * @brief finished of calculation
     */
    void finished();

private:
    void updateTop();

    QString mFileName;
    QMap<QString, int> wordsChart;
    std::atomic_bool mIsRunning;
    int mMinimum;
    bool mEnabledDetailing;

};

#endif // CHARTCALCULATION_H
