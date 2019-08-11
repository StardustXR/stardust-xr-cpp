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

        RenderTargetSelector {
            target: RenderTarget {
                attachments: [RenderTargetOutput {
                    attachmentPoint: RenderTargetOutput.Color0
                    layer: 0
                    mipLevel: 1
                    texture: Texture2D {
                        id:leftEyeTexture
                        width: OpenXRGraphics.eyeDimensions.width
                        height: OpenXRGraphics.eyeDimensions.height
                        format: Texture.R8_SNorm;
                        generateMipMaps: false
                        magnificationFilter: Texture.Linear
                        minificationFilter: Texture.LinearMipMapLinear
                        wrapMode {
                            x: WrapMode.ClampToEdge
                            y: WrapMode.ClampToEdge
                        }
                        Component.onCompleted: {
                            OpenXRGraphics.leftEyeTexture = this;
                        }
                    }
                }]
            }

            Viewport {
                normalizedRect: Qt.rect(0,0,0.5,0.5)
                CameraSelector {
                    camera: leftEye
                }
            }
        }

        RenderTargetSelector {
            target: RenderTarget {
                attachments: [RenderTargetOutput {
                    attachmentPoint: RenderTargetOutput.Color0
                    layer: 0
                    mipLevel: 1
                    texture: Texture2D {
                        id:rightEyeTexture
                        width: OpenXRGraphics.eyeDimensions.width
                        height: OpenXRGraphics.eyeDimensions.height
                        format: Texture.R8_SNorm;
                        generateMipMaps: false
                        magnificationFilter: Texture.Linear
                        minificationFilter: Texture.LinearMipMapLinear
                        wrapMode {
                            x: WrapMode.ClampToEdge
                            y: WrapMode.ClampToEdge
                        }
                        Component.onCompleted: {
                            OpenXRGraphics.rightEyeTexture = this;
                        }
                    }
                }]
            }

            Viewport {
                normalizedRect: Qt.rect(0.5,0.5,1,1)
                CameraSelector {
                    camera: rightEye
                }
            }
        }
    }
    pickingSettings.pickMethod: PickingSettings.TrianglePicking
    pickingSettings.faceOrientationPickingMode: PickingSettings.FrontAndBackFace

}
