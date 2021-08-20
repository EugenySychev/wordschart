#ifndef CHARTADAPT_H
#define CHARTADAPT_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QVariantList>
#include "chartcalculation.h"


class ChartAdapt : public QObject
{
    Q_OBJECT

public:

    Q_PROPERTY(int progress READ getProgress NOTIFY progressChanged)
    Q_PROPERTY(int maxCount READ getMaxChart NOTIFY chartChanged)
    Q_PROPERTY(QVariantList words READ getWords NOTIFY chartChanged)
    Q_PROPERTY(QVariantList charts READ getChart NOTIFY chartChanged)
    /**
     * @brief ChartDrawer constructor
     * @param parent pointer to parent object
     */
    ChartAdapt(QObject* parent = nullptr);

    // functions for qml interaction
    int getProgress();
    QVariantList getWords();
    QVariantList getChart();
    int getMaxChart();

signals:
    void progressChanged();
    void chartChanged();
    void failed();
    void finished();

public slots:
    /**
     * @brief openClicked processing open file dialog completed
     */
    void openClicked(QString);

    /**
     * @brief updateProgress processing progress changing
     */
    void updateProgress(int);

    /**
     * @brief updateCharts processing update of chart
     */
    void updateCharts(QList<QPair<QString, int> >);

    /**
     * @brief stopClicked processing stop button click
     */
    void stopClicked();

private:
    ChartCalculation* calculation;
    QVariantList mWords;
    QVariantList mCharts;
    int mProgress;
};

#endif // CHARTADAPT_H
