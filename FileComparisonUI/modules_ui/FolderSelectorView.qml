import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../customs" as Customs

import Compare.Types 1.0

Rectangle {
    id: folder_selector

   implicitHeight: parent.height

   color: "transparent"

    property string text_path: qsTr("")
    property int count_files
    property int sideType

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            id: id_row

            Layout.fillWidth: true
            Layout.fillHeight: true

            Customs.CToolButton {
                id: tool_btn
                icon.source: "qrc:/img/folder-48.png"
                Layout.fillWidth: false
                Layout.leftMargin: 10
                Layout.rightMargin: 5
                Layout.topMargin: 10
                Layout.bottomMargin: 10
                Layout.alignment: Qt.AlignCenter

                clicked: () => {
                             if (folder_selector.sideType === CompareTypes.Left) {
                                 controller.openDir(CompareTypes.Left)
                             } else if (folder_selector.sideType === CompareTypes.Right) {
                                 controller.openDir(CompareTypes.Right)
                             }
                         }
            }

            Customs.CTextField {
                id: text_field

                Layout.fillWidth: true
                Layout.leftMargin: 0
                Layout.rightMargin: 10
                Layout.topMargin: 10
                Layout.bottomMargin: 10
                Layout.alignment: Qt.AlignCenter

                placeholderText: qsTr("Выберите директорию")
                text: folder_selector.text_path
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignVCenter
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.bottomMargin: 10

            color: "transparent"

            Text {
                text: count_files != 0 ? qsTr("Количество файлов: ")
                                         + count_files : qsTr("Количество файлов: ")
            }

        }
    }
}


