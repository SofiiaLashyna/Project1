import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

Rectangle {
    id: root

    signal objectClicked(int vertexId)

    width: 1200
    height: 800
    color: "#0a0a0f"

    property vector3d defaultCameraPos: Qt.vector3d(0, 800, 1200)
    property vector3d defaultCameraTarget: Qt.vector3d(0, 0, 0)
    property bool isTracking: false

    function updateTargetPosition(x, y, z) {
        if (isTracking) {
            var newPos = Qt.vector3d(x, y, z);

            var dx = newPos.x - cameraTargetNode.position.x;
            var dy = newPos.y - cameraTargetNode.position.y;
            var dz = newPos.z - cameraTargetNode.position.z;

            solarSystemContainer.position = newPos;
            cameraTargetNode.position = newPos;

            mainCamera.position = Qt.vector3d(
                mainCamera.position.x + dx,
                mainCamera.position.y + dy,
                mainCamera.position.z + dz
            );
        }
    }

    function cameraZoomTo(x, y, z) {
        solarSystemContainer.position = Qt.vector3d(x, y, z);
        cameraTargetNode.position = Qt.vector3d(x, y, z);

        isTracking = true;

        starsContainer.opacity = 0.0;

        solarSystemContainer.visible = true;

        cameraController.enabled = false;

        zoomAnimationPos.to = Qt.vector3d(x, y + 300, z + 450);
        zoomAnimationTarget.to = Qt.vector3d(x, y, z);
        zoomAnimation.start();
    }

    function cameraReset() {
        isTracking = false;

        starsContainer.opacity = 1.0;

        solarSystemContainer.visible = false;

        resetAnimation.start();

        cameraController.enabled = true;
    }
    property color currentStarColor: "#ffaa00"

    function setStarColor(r, g, b) {
        currentStarColor = Qt.rgba(r, g, b, 1);
    }

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
            position: root.defaultCameraPos
            eulerRotation: Qt.vector3d(-45, 0, 0)
            clipNear: 0.1
            clipFar: 100000
        }

        Node {
            id: cameraTargetNode
            position: Qt.vector3d(0, 0, 0)
        }

        OrbitCameraController {
            id: cameraController
            camera: mainCamera
            origin: cameraTargetNode
            panEnabled: true
            xSpeed: 0.5
            ySpeed: 0.5
        }

        Node {
            id: galaxyRoot

            scale: Qt.vector3d(0.3, 0.3, 0.3)

            Node {
                id: starsContainer
                opacity: 1.0
                Behavior on opacity { NumberAnimation { id: galaxyOpacityAnim; duration: 1000 } }

                DirectionalLight { color: "white"; brightness: 1.5; eulerRotation: Qt.vector3d(-30, 45, 0); castsShadow: false }
                DirectionalLight { color: "#4060ff"; brightness: 0.3; eulerRotation: Qt.vector3d(30, -45, 0) }
                DirectionalLight { color: "#ffffff"; brightness: 0.2; eulerRotation: Qt.vector3d(0, 0, 0) }

                Repeater3D {
                    id: objectsRepeater
                    model: celestialModel

                    delegate: Node {
                        id: objectNode
                        position: Qt.vector3d(Number(model.xPos||0), Number(model.yPos||0), Number(model.zPos||0))
                        property int objectIndex: index
                        property bool isHovered: false

                        Model {
                            id: sphereModel
                            source: "#Sphere"
                            pickable: true
                            property real baseScale: model.sizeFactor * 0.5
                            scale: Qt.vector3d(baseScale * (objectNode.isHovered ? 1.2 : 1.0), baseScale * (objectNode.isHovered ? 1.2 : 1.0), baseScale * (objectNode.isHovered ? 1.2 : 1.0))
                            Behavior on scale { Vector3dAnimation { duration: 200; easing.type: Easing.OutCubic } }
                            materials: [ PrincipledMaterial { baseColor: model.objectColor || "white"; metalness: 0.3; roughness: 0.5; lighting: PrincipledMaterial.NoLighting } ]
                        }

                        Model {
                            source: "#Sphere"
                            pickable: false
                            property real glowScale: model.sizeFactor * 0.5
                            scale: Qt.vector3d(glowScale, glowScale, glowScale)
                            opacity: 0.2
                            materials: [ PrincipledMaterial { baseColor: model.objectColor || "white"; opacity: 0.2; alphaMode: PrincipledMaterial.Blend; lighting: PrincipledMaterial.NoLighting } ]
                            SequentialAnimation on scale {
                                running: true; loops: Animation.Infinite
                                Vector3dAnimation { to: Qt.vector3d(model.sizeFactor * 1.8, model.sizeFactor * 1.8, model.sizeFactor * 1.8); duration: 2000; easing.type: Easing.InOutSine }
                                Vector3dAnimation { to: Qt.vector3d(model.sizeFactor * 1.5, model.sizeFactor * 1.5, model.sizeFactor * 1.5); duration: 2000; easing.type: Easing.InOutSine }
                            }
                        }
                    }
                }
            }
        }

        Node {
            id: solarSystemContainer
            visible: planetModel.rowCount > 0

            Model {
                source: "#Sphere"
                position: Qt.vector3d(0, 0, 0)
                scale: Qt.vector3d(2.5, 2.5, 2.5)
                castsShadows: false
                materials: [
                    DefaultMaterial {
                        diffuseColor: root.currentStarColor
                        lighting: DefaultMaterial.NoLighting
                    }
                ]
            }

            Model {
                source: "#Sphere"
                scale: Qt.vector3d(4.0, 4.0, 4.0)
                opacity: 0.15
                materials: [ DefaultMaterial {
                    diffuseColor: root.currentStarColor
                    lighting: DefaultMaterial.NoLighting
                } ]
            }

            PointLight {
                color: root.currentStarColor
                brightness: 3.0
                castsShadow: true
                shadowMapQuality: Light.ShadowMapQualityHigh
            }

            Repeater3D {
                model: planetModel

                Node {
                    NumberAnimation on eulerRotation.y {
                        from: 0; to: 360;
                        duration: model.rotationSpeed;
                        loops: Animation.Infinite;
                        running: true
                    }

                    Node {
                        position: Qt.vector3d(model.orbitRadius, 0, 0)

                        Model {
                            source: "#Sphere"
                            scale: Qt.vector3d(model.planetSize, model.planetSize, model.planetSize)
                            materials: PrincipledMaterial {
                                baseColorMap: Texture {
                                    source: model.texturePath
                                    tilingModeHorizontal: Texture.Repeat
                                    tilingModeVertical: Texture.ClampToEdge
                                }

                                metalness: 0.0
                                roughness: 0.8
                                baseColor: "white"
                            }

                            NumberAnimation on eulerRotation.y {
                                from: 0; to: 360;
                                duration: 10000 + Math.random() * 10000
                                loops: Animation.Infinite
                            }
                        }
                    }
                }
            }
        }
    }
    ParallelAnimation {
        id: zoomAnimation
        Vector3dAnimation { id: zoomAnimationPos; target: mainCamera; property: "position"; duration: 1500; easing.type: Easing.InOutCubic }
        Vector3dAnimation { id: zoomAnimationTarget; target: cameraTargetNode; property: "position"; duration: 1500; easing.type: Easing.InOutCubic }
    }

    ParallelAnimation {
        id: resetAnimation
        Vector3dAnimation { target: mainCamera; property: "position"; to: root.defaultCameraPos; duration: 1500; easing.type: Easing.InOutCubic }
        Vector3dAnimation { target: cameraTargetNode; property: "position"; to: root.defaultCameraTarget; duration: 1500; easing.type: Easing.InOutCubic }
        Vector3dAnimation { target: mainCamera; property: "eulerRotation"; to: Qt.vector3d(-45, 0, 0); duration: 1500; easing.type: Easing.InOutCubic }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: view3D
        property var hoveredObject: null
        onPositionChanged: function(mouse){
            var result = view3D.pick(mouse.x, mouse.y)
            if (hoveredObject && hoveredObject !== result.objectHit) { if (hoveredObject.parent) hoveredObject.parent.isHovered = false }
            if (result.objectHit && result.objectHit.pickable) {
                hoveredObject = result.objectHit
                if (result.objectHit.parent) { result.objectHit.parent.isHovered = true; mouseArea.cursorShape = Qt.PointingHandCursor }
            } else { hoveredObject = null; mouseArea.cursorShape = Qt.ArrowCursor }
        }
        onClicked: function(mouse){
            var result = view3D.pick(mouse.x, mouse.y)
            if (result.objectHit && result.objectHit.pickable) {
                var objNode = result.objectHit.parent
                if (objNode && objNode.objectIndex !== undefined) {
                    root.objectClicked(objNode.objectIndex)
                }
            }
        }
        onExited: { if (hoveredObject && hoveredObject.parent) hoveredObject.parent.isHovered = false; hoveredObject = null; cursorShape = Qt.ArrowCursor }
    }

    Column { anchors.right: parent.right; anchors.top: parent.top; anchors.margins: 20; spacing: 10;
        Rectangle { width: 200; height: 150; color: "#1a1a2e"; opacity: 0.9; radius: 10; border.color: "#4a5568"; border.width: 1;
            Column { anchors.fill: parent; anchors.margins: 15; spacing: 8;
                Text { text: "Керування"; color: "#e0e0e0"; font.bold: true; font.pixelSize: 14 }
                Text { text: "ЛКМ + рух: Обертання"; color: "#b0b0b0"; font.pixelSize: 11; wrapMode: Text.WordWrap; width: parent.width }
                Text { text: "Коліщатко: Zoom"; color: "#b0b0b0"; font.pixelSize: 11 }
                Text { text: "ПКМ + рух: Панорама"; color: "#b0b0b0"; font.pixelSize: 11; wrapMode: Text.WordWrap; width: parent.width }
                Text { text: "Клік на об'єкті: Вибір"; color: "#b0b0b0"; font.pixelSize: 11; wrapMode: Text.WordWrap; width: parent.width }
            }
        }
        Rectangle { width: 200; height: 60; color: "#1a1a2e"; opacity: 0.9; radius: 10; border.color: "#4a5568"; border.width: 1;
            Column { anchors.centerIn: parent; spacing: 5;
                Text { text: "Об'єктів у галактиці:"; color: "#b0b0b0"; font.pixelSize: 11; anchors.horizontalCenter: parent.horizontalCenter }
                Text { text: celestialModel ? celestialModel.rowCount() : "0"; color: "#4fd1c5"; font.bold: true; font.pixelSize: 20; anchors.horizontalCenter: parent.horizontalCenter }
            }
        }
    }
}