#ifndef CHARTADAPT_H
#define CHARTADAPT_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QVariantList>
#include <QTimer>
#include <QPointer>
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
     * @brief ChartAdapt constructor
     * @param parent pointer to parent object
     */
    ChartAdapt(QObject* parent = nullptr);

    // functions for qml interaction
    int getProgress();
    int getMaxChart();
    QVariantList getWords();
    QVariantList getChart();

signals:
    /**
     * @brief progressChanged signal of changing progress
     */
    void progressChanged();

    /**
     * @brief chartChanged emitting for redraw chart
     */
    void chartChanged();

    /**
     * @brief failed of calculation
     */
    void failed();

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

    /**
     * @brief changeDetailing enabling or disabling detailing
     */
    void changeDetailing(bool);

    /**
     * @brief finished calculation
     */
    void finished();

private:
    QPointer<ChartCalculation> calculation;
    QVariantList mWords;
    QVariantList mCharts;
    int mProgress;
    QTimer mTimer;
    bool mEnabledDetailing;
    QPointer<QThread> thread;
};

#endif // CHARTADAPT_H
