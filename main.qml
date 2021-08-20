import QtQuick 2.15
import QtQuick.Window 2.15
import QtCharts 2.3
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

Window {
    width: 640
    height: 640
    minimumHeight: 630
    visible: true
    title: qsTr("Word chart")

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
                max: drawer.maxCount * 1.1;
                titleText: "Quantity"
            }

            HorizontalBarSeries {
                id: hBarSeries
                axisX: xAxis
                axisY: BarCategoryAxis { categories: drawer.words }
                BarSet { values: drawer.charts }
            }
        }

        ProgressBar {
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.margins: 20
            id: progressBar
            from: 0
            to: 100
            value: drawer.progress ? 0 : drawer.progress
        }
        RowLayout {
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
            Layout.margins: 20

            Button {
                id: openButton
                text: qsTr("Open")
                onClicked: fileDialog.open()
            }
            Button {
                id: stopButton
                text: qsTr("Stop")
                onClicked: drawer.stopClicked();

            }
        }

    }
}
