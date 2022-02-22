import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

TextField {
    id: text_field
    anchors.margins: 50

    selectByMouse: true

    implicitHeight: 32

    font.pixelSize: 14
    readOnly: true

    background: Rectangle {
        id: text_field_style
        anchors.fill: parent
        border.width: 1
        radius: 4
        border.color: qsTr("#DCDCDC")
    }
}

