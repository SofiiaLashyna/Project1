import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

Rectangle {
    id: root

    signal objectClicked(int vertexId)

    width: 1200
    height: 800
    color: "#0a0a0f"

    View3D {
        id: view3D
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "#0a0a0f"
            backgroundMode: SceneEnvironment.Color
            antialiasingMode: SceneEnvironment.MSAA
            antialiasingQuality: SceneEnvironment.High
        }

        PerspectiveCamera {
            id: mainCamera
            position: Qt.vector3d(0, 10000, 15000)
            eulerRotation: Qt.vector3d(-35, 0, 0)
            clipNear: 10
            clipFar: 1000000
        }

        OrbitCameraController {
            id: cameraController
            camera: mainCamera
            origin: galaxyRoot
            panEnabled: true
            xSpeed: 0.5
            ySpeed: 0.5
        }

        Node {
            id: galaxyRoot


            Node {
                id: galaxyScale
                scale: Qt.vector3d(0.5, 0.5, 0.5)

                DirectionalLight {
                    color: "white"
                    brightness: 1.5
                    eulerRotation: Qt.vector3d(-30, 45, 0)
                    castsShadow: false
                }

                DirectionalLight {
                    color: "#4060ff"
                    brightness: 0.3
                    eulerRotation: Qt.vector3d(30, -45, 0)
                }

                DirectionalLight {
                    color: "#ffffff"
                    brightness: 0.2
                    eulerRotation: Qt.vector3d(0, 0, 0)
                }

                Repeater3D {
                    id: objectsRepeater
                    model: celestialModel

                    delegate: Node {
                        id: objectNode
                        position: Qt.vector3d(
                            Number(model.xPos || 0),
                            Number(model.yPos || 0),
                            Number(model.zPos || 0)
                        )

                        property int objectIndex: index
                        property bool isHovered: false

                        Model {
                            id: sphereModel
                            source: "#Sphere"
                            pickable: true

                            property real baseScale: model.sizeFactor * 0.1
                            scale: Qt.vector3d(
                                baseScale * (objectNode.isHovered ? 1.2 : 1.0),
                                baseScale * (objectNode.isHovered ? 1.2 : 1.0),
                                baseScale * (objectNode.isHovered ? 1.2 : 1.0)
                            )

                            Behavior on scale {
                                Vector3dAnimation {
                                    duration: 200
                                    easing.type: Easing.OutCubic
                                }
                            }

                            materials: [
                                PrincipledMaterial {
                                    baseColor: model.objectColor || "white"
                                    metalness: 0.3
                                    roughness: 0.5
                                    lighting: PrincipledMaterial.NoLighting
                                }
                            ]
                        }

                        Model {
                            source: "#Sphere"
                            pickable: false

                            property real glowScale: model.sizeFactor * 2.0
                            scale: Qt.vector3d(glowScale, glowScale, glowScale)
                            opacity: 0.2

                            materials: [
                                PrincipledMaterial {
                                    baseColor: model.objectColor || "white"
                                    opacity: 0.2
                                    alphaMode: PrincipledMaterial.Blend
                                    lighting: PrincipledMaterial.NoLighting
                                }
                            ]

                            SequentialAnimation on scale {
                                running: true
                                loops: Animation.Infinite
                                Vector3dAnimation {
                                    to: Qt.vector3d(
                                        model.sizeFactor * 1.8,
                                        model.sizeFactor * 1.8,
                                        model.sizeFactor * 1.8
                                    )
                                    duration: 2000
                                    easing.type: Easing.InOutSine
                                }
                                Vector3dAnimation {
                                    to: Qt.vector3d(
                                        model.sizeFactor * 1.5,
                                        model.sizeFactor * 1.5,
                                        model.sizeFactor * 1.5
                                    )
                                    duration: 2000
                                    easing.type: Easing.InOutSine
                                }
                            }
                        }
                    }
                }

                Model {
                    source: "#Rectangle"
                    scale: Qt.vector3d(500, 500, 1)
                    position: Qt.vector3d(0, -100, 0)
                    eulerRotation: Qt.vector3d(-90, 0, 0)
                    pickable: false
                    materials: [
                        PrincipledMaterial {
                            baseColor: "#1a1a2e"
                            opacity: 0.1
                            alphaMode: PrincipledMaterial.Blend
                        }
                    ]
                }
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: view3D

        property var hoveredObject: null

        onPositionChanged: function(mouse){
            var result = view3D.pick(mouse.x, mouse.y)

            if (hoveredObject && hoveredObject !== result.objectHit) {
                if (hoveredObject.parent) {
                    hoveredObject.parent.isHovered = false
                }
            }

            if (result.objectHit && result.objectHit.pickable) {
                hoveredObject = result.objectHit
                if (result.objectHit.parent) {
                    result.objectHit.parent.isHovered = true
                    mouseArea.cursorShape = Qt.PointingHandCursor
                }
            } else {
                hoveredObject = null
                mouseArea.cursorShape = Qt.ArrowCursor
            }
        }

        onClicked: function(mouse){
            var result = view3D.pick(mouse.x, mouse.y)
            if (result.objectHit && result.objectHit.pickable) {
                var objNode = result.objectHit.parent
                if (objNode && objNode.objectIndex !== undefined) {
                    root.objectClicked(objNode.objectIndex)
                    console.log("Clicked object", objNode.objectIndex)
                }
            }
        }

        onExited: {
            if (hoveredObject && hoveredObject.parent) {
                hoveredObject.parent.isHovered = false
            }
            hoveredObject = null
            cursorShape = Qt.ArrowCursor
        }
    }

    Column {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 20
        spacing: 10

        Rectangle {
            width: 200
            height: 150
            color: "#1a1a2e"
            opacity: 0.9
            radius: 10
            border.color: "#4a5568"
            border.width: 1

            Column {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 8

                Text {
                    text: "Керування"
                    color: "#e0e0e0"
                    font.bold: true
                    font.pixelSize: 14
                }

                Text {
                    text: "ЛКМ + рух: Обертання"
                    color: "#b0b0b0"
                    font.pixelSize: 11
                    wrapMode: Text.WordWrap
                    width: parent.width
                }

                Text {
                    text: "Коліщатко: Zoom"
                    color: "#b0b0b0"
                    font.pixelSize: 11
                }

                Text {
                    text: "ПКМ + рух: Панорама"
                    color: "#b0b0b0"
                    font.pixelSize: 11
                    wrapMode: Text.WordWrap
                    width: parent.width
                }

                Text {
                    text: "Клік на об'єкті: Вибір"
                    color: "#b0b0b0"
                    font.pixelSize: 11
                    wrapMode: Text.WordWrap
                    width: parent.width
                }
            }
        }

        Rectangle {
            width: 200
            height: 60
            color: "#1a1a2e"
            opacity: 0.9
            radius: 10
            border.color: "#4a5568"
            border.width: 1

            Column {
                anchors.centerIn: parent
                spacing: 5

                Text {
                    text: "Об'єктів у галактиці:"
                    color: "#b0b0b0"
                    font.pixelSize: 11
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    text: celestialModel ? celestialModel.rowCount() : "0"
                    color: "#4fd1c5"
                    font.bold: true
                    font.pixelSize: 20
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 20
        width: 120
        height: 40
        color: "#2d3748"
        opacity: 0.9
        radius: 8
        border.color: "#4a5568"
        border.width: 1

        Text {
            anchors.centerIn: parent
            text: "Reset View"
            color: "#e0e0e0"
            font.pixelSize: 12
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                mainCamera.position = Qt.vector3d(0, 1000, 2500)
                mainCamera.eulerRotation = Qt.vector3d(-10, 0, 0)
            }
        }
    }


}