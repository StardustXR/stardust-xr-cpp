import QtQuick 2.0

import Qt3D.Core 2.13
import Qt3D.Extras 2.13
import Qt3D.Render 2.13

import QtQuick.Window 2.13

import OpenXRGraphics 1.0


RenderSettings {
    id:renderSettings

    activeFrameGraph: Viewport {
        normalizedRect: Qt.rect(0,0,1,1)

        ClearBuffers {
            buffers: ClearBuffers.ColorBuffer
            clearColor: Qt.rgba(0,0,0,0)
        }

        Viewport {
            normalizedRect: Qt.rect(0,0,0.5,0.5)
            CameraSelector {
                camera: leftEye
            }
        }

        Viewport {
            normalizedRect: Qt.rect(0.5,0.5,1,1)
            CameraSelector {
                camera: rightEye
            }
        }
    }
    pickingSettings.pickMethod: PickingSettings.TrianglePicking
    pickingSettings.faceOrientationPickingMode: PickingSettings.FrontAndBackFace
}
