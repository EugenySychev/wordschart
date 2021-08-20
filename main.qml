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
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            drawer.openClicked(fileDialog.fileUrl)
        }
        onRejected: {
            console.log("Canceled")
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
                max: drawer.maxCount;
                titleText: "Частота"
                labelsFont: Qt.font({pointSize: 7})
            }

            HorizontalBarSeries {
                id: hBarSeries
                axisX: xAxis
                axisY: BarCategoryAxis {
                    categories: drawer.words
                    labelsFont: Qt.font({pointSize: 7})
                }
                BarSet {
                    values: drawer.charts
                }
            }
            visible: drawer.progress ? true: false
        }

        CheckBox {
            id: detailChecker
            checked: true
            Layout.alignment: Qt.AlignHCenter
            text: "Менять гистограмму в процессе"
            onCheckStateChanged: {
                drawer.changeDetailing(detailChecker.checked)
            }
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "Прогресс " + String(drawer.progress) + "%"
            visible: drawer.progress ? true: false
        }

        ProgressBar {
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.margins: 20
            id: progressBar
            from: 0
            to: 100
            value: drawer.progress ? drawer.progress : 0
            visible: drawer.progress ? true: false
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
                onClicked: drawer.stopClicked();

            }
        }

    }
}
