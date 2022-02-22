import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


Item {
    id: files_view
    clip: true

    RowLayout {
        id: layout

        anchors.fill: parent

        Component {
             id: highlight
             Rectangle {
                 width: table.width;
                 height: 400
                 color: "lightsteelblue"; radius: 5
                 y: list.currentItem.y
                 Behavior on y {
                     SpringAnimation {
                         spring: 3
                         damping: 0.2
                     }
                 }
             }
         }

        ListView {
            id: table

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 10
            spacing: 5
            clip: true

            headerPositioning: ListView.OverlayHeader

            function size() {
                return table.count
            }

            model: filesviewmodel
            delegate: FileItemDelegate {}

            highlight: highlight
            highlightFollowsCurrentItem: false
            focus: true
        }
    }
}
