import QtQuick 2.15
import QtQuick.Window 2.15
import QtCharts 2.3
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Word chart")


    Button {
        anchors.centerIn: parent
        text: qsTr("Open")
        anchors.verticalCenterOffset: (parent.height - height - 20) / 2
        anchors.horizontalCenterOffset: 0
        onClicked: fileDialog.open()
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            drawer.openClicked(fileDialog.fileUrl)
            //            Qt.quit()
        }
        onRejected: {
            console.log("Canceled")
            //            Qt.quit()
        }
    }

    ProgressBar {
        id: progressBar

        x: 225
        y: 418
        from: 0
        to: 100
        value: drawer.progress ? 0 : drawer.progress

    }

    ChartView {
        id: hBar
        x: 151
        y: 31
        width: parent.width / 2
        height: parent.width / 2
        legend.visible: false
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
}
