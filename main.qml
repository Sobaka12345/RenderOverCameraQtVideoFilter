import QtQuick 2.4
import QtMultimedia 5.12
import videofilter.cus 1.0
import QtQuick.Window 2.13

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("FRAMEBUFFER_TEST")

    Rectangle {
        id: cameraContainer
        height: parent.height
        width: parent.width

        Camera {
            id: camera
            onCameraStateChanged: {
            }
        }

        VideoOutput {
            id:video
            anchors.fill: parent
            filters: [ filter ]
            source: camera
            autoOrientation: true
        }

        CustomVideoFilter {
            id: filter
        }
    }
}
