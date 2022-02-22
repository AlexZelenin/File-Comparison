import QtQuick 2.15
import QtQuick.Controls 2.15


CButton {
    id: tool_button

    property alias icon: image

    btnColor: "#fff"
    btnColorEnterd: "#dcdcdc"
    btnDownColor: "#bcbcbc"

    width: 32
    height: 32

    btnText: qsTr("")

    Image {
        id: image
        width:tool_button.width
        height: tool_button.height
        fillMode: Image.PreserveAspectFit
        source: ""
    }

}
