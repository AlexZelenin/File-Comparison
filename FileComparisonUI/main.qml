import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15
import QtQuick.Dialogs 1.3

import "modules_ui" as Modules
import "customs" as Customs

ApplicationWindow {
    id: root
    width: 800
    height: 600
    visible: true
    title: qsTr("File Comparison")

    header: ToolBar {
        id: tool_bar

        height: 48

        background: Rectangle {
            id: tool_style
            color: qsTr("#00A550")

            DropShadow {
                id: btn_shadow
                anchors.fill: tool_style

                cached: true
                horizontalOffset: 0
                verticalOffset: 1
                radius:6
                samples: 10
                color: qsTr("#2C5545")
                smooth: true
                source: tool_style
            }
        }

        RowLayout {
            id: row_tool
            anchors.fill: parent
            ToolButton {
                id: btn_menu

                Layout.leftMargin: 5
                Layout.rightMargin: 10

                icon.source: "qrc:/img/squared-menu-50.png"
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Меню")
                onClicked:  menu.open()
            }

            Customs.CToolButton {
                id: btn_run
                icon.source: "qrc:/img/play-button-64.png"
                clicked: () => {
                             controller.executeCompare()
                         }
            }

            Customs.CToolButton {
                id: btn_clr
                signal clear

                icon.source: "qrc:/img/empty-trash-48.png"
                clicked: () => {
                             clear()
                             controller.clear()
                         }
            }

            Label {
                text: ""
                font.pixelSize: 14
                color: "white"
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
        }
    }

    Menu {
        id: menu
        y: 5
        x: 8

        MenuItem {
            text: qsTr("Выход")
            onClicked: root.close()
        }
    }

    Modules.MainView {
        anchors.fill: parent
    }

    MessageDialog {
         id: messageDialog
         icon: StandardIcon.Question
         title: "Закрытие приложения"
         text: "Выйти из программы?"
         standardButtons: StandardButton.Yes | StandardButton.No

         onYes: {
             controller.clear()
             controller.closeThread()
         }
     }

    onClosing: {
        close.accepted = false
        onTriggered: messageDialog.open()
    }
}
