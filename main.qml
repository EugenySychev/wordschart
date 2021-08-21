import QtQuick 2.15
import QtQuick.Window 2.15
import QtCharts 2.3
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

Window {
    width: 640
    height: 700
    minimumHeight: 700
    visible: true
    title: "Словочарт"

    FileDialog {
        id: fileDialog
        title: "Выберите файл"
        folder: shortcuts.home
        onAccepted: {
            chartAdapt.openClicked(fileDialog.fileUrl)
        }
    }

    ColumnLayout {
        anchors.fill: parent

        ChartView {
            id: hBar
            legend.visible: false
            Layout.fillHeight: true
            Layout.fillWidth: true

            ValueAxis {
                id: xAxis
                min: 0
                max: chartAdapt ? chartAdapt.maxCount : 0
                titleText: "Частота"
                labelsFont: Qt.font({pointSize: 7})
            }

            HorizontalBarSeries {
                id: hBarSeries
                axisX: xAxis
                axisY: BarCategoryAxis {
                    categories: chartAdapt ? chartAdapt.words : [""]
                    labelsFont: Qt.font({pointSize: 7})
                }
                BarSet {
                    values: chartAdapt ? chartAdapt.charts : [0]
                }
            }
            visible: chartAdapt ? (chartAdapt.progress ? true: false ) : false
        }

        CheckBox {
            id: detailChecker
            checked: true
            Layout.alignment: Qt.AlignHCenter
            text: "Менять гистограмму в процессе"
            onCheckStateChanged: {
                chartAdapt.changeDetailing(detailChecker.checked)
            }
        }

        Label {
            id: progressLabel
            Layout.alignment: Qt.AlignHCenter
            visible: chartAdapt ? (chartAdapt.progress ? true: false) : false
        }

        ProgressBar {
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.margins: 20
            id: progressBar
            from: 0
            to: 100
            value: chartAdapt ? chartAdapt.progress : 0
            visible: chartAdapt ? true : false
        }

        RowLayout {
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
            Layout.margins: 20

            Button {
                id: openButton
                text: "Открыть файл"
                onClicked: fileDialog.open()
            }

            Button {
                id: stopButton
                text: "Остановить"
                onClicked: chartAdapt.stopClicked();
            }
        }
    }

    Connections {
        target: chartAdapt
        function onFailed() {
            progressLabel.text = "Ошибка чтения файла.";
        }
        function onProgressChanged() {
            if (chartAdapt.progress !== 100)
            {
                progressLabel.text = "Прогресс " + String(chartAdapt.progress) + "%";
            } else {
                progressLabel.text = "Завершено.";
            }
        }
    }
}
