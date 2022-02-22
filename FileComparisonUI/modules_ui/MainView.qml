import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import "../customs" as Customs

import Compare.Types 1.0

Item {
    id: main_view

    Rectangle {
        id: header

        property string text: qsTr("")

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        implicitHeight: 32

        Text {
            anchors.fill: parent
            text: header.text

            font.pixelSize: 14

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        gradient: Gradient {
            GradientStop { position: 0.1; color: "#DCDCDC" }
            GradientStop { position: 1.0; color: "#F5F5F5" }
        }

        Connections {
            target: btn_clr

            function onClear() {
                header.text = qsTr("")
                progress.to = 0
                progress.from = 0
                folder_selector_left.text_path = qsTr("")
                folder_selector_left.count_files = 0
                folder_selector_right.count_files = 0
                folder_selector_right.text_path = qsTr("")
            }
        }

        Connections {
            target: controller

            function onCountInLeft(count: int) {
                progress.to += count
            }

            function onCountInRight(count: int) {
                progress.to += count
            }
        }

        Connections {
            target: filesviewmodel

            function onProgressChanged(value: int) {
                header.text = qsTr("Выполняется чтение директорий...")
                progress.value = value
            }

            function onCountFinded(count: int) {
                header.text = qsTr("Найдено совпадений: ") + count
            }
        }

        ProgressBar {
            id: progress

            to: 0
            from: 0
            value: 0

            background: Rectangle {
                id: progress_style
                implicitWidth: header.width
                implicitHeight: header.height

                color: "transparent"
            }

            contentItem: Item {
                implicitWidth: header.width
                implicitHeight: header.height

                Rectangle {
                    width: progress.visualPosition * header.width
                    height: header.height
                    radius: 4
                    color: "#00A550"
                    opacity: 0.3
                }
            }
        }
    }

    RowLayout {
        id: folder_selector_row
        anchors {
            top: header.bottom
            left: main_view.left
            right: main_view.right
        }

        implicitHeight: 100

        Connections {
            target: controller

            // Left
            function onPathLeftSelected(path: string) {
                folder_selector_left.text_path = path
            }

            function onCountInLeft(count: int) {
                folder_selector_left.count_files = count
            }


            // Right
            function onPathRightSelected(path: string) {
                folder_selector_right.text_path = path
            }

            function onCountInRight(count: int) {
                folder_selector_right.count_files = count
            }
        }

        Rectangle {
            id: left_item

            implicitHeight: 80

            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignTop

            FolderSelectorView {
                id: folder_selector_left
                anchors.top: left_item.top
                width: left_item.width
                sideType: CompareTypes.Left

            }
        }

        Rectangle {
            id: right_item

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop

            FolderSelectorView {
                id: folder_selector_right
                anchors.top: right_item.top
                width: right_item.width
                sideType: CompareTypes.Right
            }
        }
    }

    Rectangle {
        id: h_line
        anchors {
            left: parent.left
            right: parent.right
            top: folder_selector_row.bottom
        }

        implicitHeight: 2

        color: "#DCDCDC"
    }

    FilesView {
        anchors {
            top: h_line.bottom
            left: main_view.left
            right: main_view.right
            bottom: main_view.bottom
        }
    }
}
