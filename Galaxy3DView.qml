import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

Rectangle {
    id: root
    width: 1200
    height: 800
    color: "#0a0a0f"

    signal objectClicked(int vertexId)
    signal objectDoubleClicked(int vertexId)
    signal backgroundClicked()

    property int currentSystemType: 0
    property string centralTexturePath: ""

    property vector3d defaultCameraPos: Qt.vector3d(0, 800, 1200)
    property vector3d defaultCameraTarget: Qt.vector3d(0, 0, 0)

    property bool isTracking: false
    property bool isZooming: false

    property vector3d startZoomOffset: Qt.vector3d(0,0,0)
    property vector3d endZoomOffset: Qt.vector3d(0, 300, 450)
    property vector3d startZoomRotation: Qt.vector3d(0,0,0)
    property real zoomProgress: 0.0

    function setCentralTexture(path) { centralTexturePath = path; }
    function setCurrentSystemType(type) { currentSystemType = type; }

    function updateTargetPosition(x, y, z) {
        if (isTracking) {
            var newPos = Qt.vector3d(x, y, z);

            var dx = newPos.x - cameraTargetNode.position.x;
            var dy = newPos.y - cameraTargetNode.position.y;
            var dz = newPos.z - cameraTargetNode.position.z;

            solarSystemContainer.position = newPos;
            cameraTargetNode.position = newPos;

            if (!isZooming) {
                mainCamera.position = Qt.vector3d(
                    mainCamera.position.x + dx,
                    mainCamera.position.y + dy,
                    mainCamera.position.z + dz
                );
            }
        }
    }

    function cameraZoomTo(x, y, z) {
        console.log("Zoom started to: " + x + ", " + z);

        var targetPos = Qt.vector3d(x, y, z);
        solarSystemContainer.position = targetPos;
        cameraTargetNode.position = targetPos;

        startZoomOffset = mainCamera.position.minus(targetPos);
        startZoomRotation = mainCamera.eulerRotation;

        starsContainer.opacity = 0.0;
        solarSystemContainer.visible = true;

        cameraController.enabled = false;

        isTracking = true;
        isZooming = true;

        smoothZoomAnim.start();
    }
    function cameraReset() {
        console.log("Camera Reset");
        smoothZoomAnim.stop();
        isZooming = false;
        isTracking = false;

        currentSystemType = 0;
        starsContainer.opacity = 1.0;
        solarSystemContainer.visible = false;

        resetAnimation.start();
        cameraController.enabled = true;
    }

    function setStarColor(r, g, b) {
        currentStarColor = Qt.rgba(r, g, b, 1);
    }

    property color currentStarColor: "#ffaa00"

    function drawPath(pathSegments) {
        pathContainer.children.forEach(c => c.destroy());

        for (var i = 0; i < pathSegments.length; i++) {
            var segment = pathSegments[i];

            var vStart = Qt.vector3d(segment.x1, segment.y1, segment.z1);
            var vEnd = Qt.vector3d(segment.x2, segment.y2, segment.z2);

            lineComponent.createObject(pathContainer, {
                startPos: vStart,
                endPos: vEnd
            });
        }
    }


    function clearPath() {
        pathContainer.children.forEach(c => c.destroy());
    }

    function getDistance(v1, v2) {
        return Math.sqrt(Math.pow(v1.x-v2.x, 2) + Math.pow(v1.y-v2.y, 2) + Math.pow(v1.z-v2.z, 2));
    }

    ListModel {
        id: pathModel
    }

    View3D {
        id: view3D
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "#0a0a0f"
            backgroundMode: SceneEnvironment.SkyBox
            antialiasingMode: SceneEnvironment.MSAA
            antialiasingQuality: SceneEnvironment.High
            lightProbe: Texture { source: "qrc:/3DView/textures/skybox.hdr" }
            probeExposure: 0.7
        }

        PerspectiveCamera {
            id: mainCamera
            position: root.defaultCameraPos
            eulerRotation: Qt.vector3d(-45, 0, 0)
            clipNear: 0.1
            clipFar: 1000000
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
                Behavior on opacity {
                    NumberAnimation {
                        duration: 1000
                    }
                }

                DirectionalLight {
                    color: "white"; brightness: 1.5; eulerRotation: Qt.vector3d(-30, 45, 0); castsShadow: false
                }

                Repeater3D {
                    id: objectsRepeater
                    model: celestialModel
                    delegate: Node {
                        id: objectNode
                        position: Qt.vector3d(Number(model.xPos || 0), Number(model.yPos || 0), Number(model.zPos || 0))
                        property int objectIndex: index
                        property int type: model.objectType
                        property bool isHovered: false

                        Node {
                            visible: objectNode.type === 1
                            Model {
                                id: sphereModel
                                source: "#Sphere"
                                pickable: true
                                property real baseScale: model.sizeFactor * 0.5
                                scale: Qt.vector3d(baseScale * (objectNode.isHovered ? 1.2 : 1.0), baseScale * (objectNode.isHovered ? 1.2 : 1.0), baseScale * (objectNode.isHovered ? 1.2 : 1.0))
                                Behavior on scale {
                                    Vector3dAnimation {
                                        duration: 200; easing.type: Easing.OutCubic
                                    }
                                }
                                materials: [PrincipledMaterial {
                                    baseColor: model.objectColor || "white"; metalness: 0.3; roughness: 0.5; lighting: PrincipledMaterial.NoLighting
                                }]
                            }

                            Model {
                                source: "#Sphere"
                                pickable: false
                                property real glowScale: model.sizeFactor * 0.5
                                scale: Qt.vector3d(glowScale, glowScale, glowScale)
                                opacity: 0.2
                                materials: [PrincipledMaterial {
                                    baseColor: model.objectColor || "white"; opacity: 0.2; alphaMode: PrincipledMaterial.Blend; lighting: PrincipledMaterial.NoLighting
                                }]
                                SequentialAnimation on scale {
                                    running: true; loops: Animation.Infinite
                                    Vector3dAnimation {
                                        to: Qt.vector3d(model.sizeFactor * 1.8, model.sizeFactor * 1.8, model.sizeFactor * 1.8); duration: 2000; easing.type: Easing.InOutSine
                                    }
                                    Vector3dAnimation {
                                        to: Qt.vector3d(model.sizeFactor * 1.5, model.sizeFactor * 1.5, model.sizeFactor * 1.5); duration: 2000; easing.type: Easing.InOutSine
                                    }
                                }
                            }
                        }
                        Node {
                            visible: objectNode.type === 2
                            scale: Qt.vector3d(2.0, 2.0, 2.0)
                            Model {
                                source: "#Sphere"
                                pickable: true
                                scale: Qt.vector3d(model.sizeFactor, model.sizeFactor * 0.8, model.sizeFactor)
                                eulerRotation: Qt.vector3d(Math.random() * 360, Math.random() * 360, 0)
                                materials: [
                                    DefaultMaterial {
                                        diffuseMap: Texture {
                                            source: "qrc:/3DView/textures/nebula.png"
                                        }
                                        diffuseColor: model.objectColor
                                        opacity: 0.4
                                        blendMode: DefaultMaterial.Screen
                                        lighting: DefaultMaterial.NoLighting
                                        cullMode: Material.NoCulling
                                    }
                                ]
                                NumberAnimation on eulerRotation.y {
                                    from: 0;
                                    to: 360; duration: 50000; loops: Animation.Infinite
                                }
                            }
                            Model {
                                source: "#Sphere"
                                scale: Qt.vector3d(model.sizeFactor * 0.7, model.sizeFactor * 0.7, model.sizeFactor * 0.7)
                                materials: [DefaultMaterial {
                                    diffuseMap: Texture {
                                        source: "qrc:/3DView/textures/nebula.png"
                                    }
                                    diffuseColor: "white"
                                    opacity: 0.2
                                    blendMode: DefaultMaterial.Screen
                                    lighting: DefaultMaterial.NoLighting
                                }]
                                NumberAnimation on eulerRotation.x {
                                    from: 0;
                                    to: 360; duration: 40000; loops: Animation.Infinite
                                }
                            }
                        }
                    }
                }
            }

            Node {
                id: pathContainer

                Component {
                    id: lineComponent
                    Node {
                        id: lineNode
                        property vector3d startPos
                        property vector3d endPos

                        function updateOrientation() {
                            var dx = endPos.x - startPos.x;
                            var dy = endPos.y - startPos.y;
                            var dz = endPos.z - startPos.z;

                            var dist = Math.sqrt(dx*dx + dy*dy + dz*dz);
                            cylinderModel.scale = Qt.vector3d(0.05, dist / 100, 0.05);
                            cylinderModel.y = dist / 2;

                            var yaw = Math.atan2(dx, dz) * 180 / Math.PI;

                            var horizontalDist = Math.sqrt(dx*dx + dz*dz);
                            var pitch = Math.atan2(horizontalDist, dy) * 180 / Math.PI;

                            lineNode.eulerRotation = Qt.vector3d(pitch, yaw, 0);
                        }

                        position: startPos

                        Component.onCompleted: updateOrientation()
                        onStartPosChanged: updateOrientation()
                        onEndPosChanged: updateOrientation()

                        Model {
                            id: cylinderModel
                            source: "#Cylinder"
                            materials: DefaultMaterial {
                                diffuseColor: "#00ff00"
                                lighting: DefaultMaterial.NoLighting
                                opacity: 0.8
                                blendMode: DefaultMaterial.Screen
                                cullMode: Material.NoCulling
                            }
                        }
                    }
                }
            }
        }
        Node {
            id: solarSystemContainer
            visible: root.isTracking

            Node {
                id: starSystemNode
                visible: opacity > 0.01
                opacity: root.currentSystemType === 1 ? 1.0 : 0.0
                Behavior on opacity { NumberAnimation { duration: 2000; easing.type: Easing.InOutQuad } }
                Texture {
                    id: dynamicStarTexture
                    source: root.centralTexturePath
                }

                Model {
                    source: "#Sphere"
                    position: Qt.vector3d(0, 0, 0)
                    scale: Qt.vector3d(2.5, 2.5, 2.5)
                    castsShadows: false
                    materials: [
                        DefaultMaterial {
                            diffuseColor: root.currentStarColor
                            lighting: DefaultMaterial.NoLighting
                            diffuseMap: root.centralTexturePath !== "" ? dynamicStarTexture : null
                        }
                    ]
                }
                Model {
                    source: "#Sphere"
                    scale: Qt.vector3d(4.0, 4.0, 4.0)
                    opacity: 0.15
                    materials: [DefaultMaterial {
                        diffuseColor: root.currentStarColor
                        lighting: DefaultMaterial.NoLighting
                    }]
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
                            from: 0; to: 360; duration: model.rotationSpeed; loops: Animation.Infinite; running: true
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
                                    from: 0; to: 360; duration: 10000 + Math.random() * 10000; loops: Animation.Infinite
                                }
                            }
                        }
                    }
                }
            }
            Node {
                id: nebulaNode
                visible: opacity > 0.01
                opacity: root.currentSystemType === 2 ? 1.0 : 0.0
                Behavior on opacity { NumberAnimation { duration: 2000; easing.type: Easing.InOutQuad } }
                Model {
                    source: "#Sphere"
                    scale: Qt.vector3d(15.0, 12.0, 15.0)
                    materials: [
                        DefaultMaterial {
                            diffuseMap: Texture { source: root.centralTexturePath }
                            diffuseColor: root.currentStarColor
                            opacity: 0.6
                            blendMode: DefaultMaterial.Screen
                            lighting: DefaultMaterial.NoLighting
                            cullMode: Material.NoCulling
                        }
                    ]
                    NumberAnimation on eulerRotation.y { from: 0; to: 360; duration: 60000; loops: Animation.Infinite; running: true }
                    NumberAnimation on eulerRotation.z { from: 0; to: 360; duration: 120000; loops: Animation.Infinite; running: true }
                }
                Model {
                    source: "#Sphere"
                    scale: Qt.vector3d(12.0, 12.0, 12.0)
                    materials: [
                        DefaultMaterial {
                            diffuseMap: Texture { source: root.centralTexturePath }
                            diffuseColor: "white"
                            opacity: 0.3
                            blendMode: DefaultMaterial.Screen
                            lighting: DefaultMaterial.NoLighting
                            cullMode: Material.NoCulling
                        }
                    ]
                    NumberAnimation on eulerRotation.x { from: 0; to: -360; duration: 50000; loops: Animation.Infinite; running: true }
                }
            }
        }
    }

    NumberAnimation {
        id: smoothZoomAnim
        target: root
        property: "zoomProgress"
        from: 0.0
        to: 1.0
        duration: 1500
        easing.type: Easing.InOutCubic

        onStarted: root.zoomProgress = 0.0

        onFinished: {
            root.isZooming = false;
            cameraController.enabled = true;
            mainCamera.position = cameraTargetNode.position.plus(root.endZoomOffset);
        }
    }

    onZoomProgressChanged: {
        if (isZooming) {
            var t = zoomProgress;
            var currOffX = startZoomOffset.x * (1-t) + endZoomOffset.x * t;
            var currOffY = startZoomOffset.y * (1-t) + endZoomOffset.y * t;
            var currOffZ = startZoomOffset.z * (1-t) + endZoomOffset.z * t;

            mainCamera.position = cameraTargetNode.position.plus(Qt.vector3d(currOffX, currOffY, currOffZ));

            var rotX = startZoomRotation.x * (1-t) + (-35) * t;
            var rotY = startZoomRotation.y * (1-t) + 0 * t;
            var rotZ = startZoomRotation.z * (1-t) + 0 * t;
            mainCamera.eulerRotation = Qt.vector3d(rotX, rotY, rotZ);
        }
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

        Timer {
            id: clickTimer
            interval: 250
            repeat: false
            property int pendingVertexId: -1

            onTriggered: {
                if (pendingVertexId !== -1) {
                    root.objectClicked(pendingVertexId);
                    pendingVertexId = -1;
                }
            }
        }

        onPositionChanged: function (mouse) {
            var result = view3D.pick(mouse.x, mouse.y)
            if (hoveredObject && hoveredObject !== result.objectHit) {
                if (hoveredObject.parent) hoveredObject.parent.isHovered = false
            }
            if (result.objectHit && result.objectHit.pickable) {
                hoveredObject = result.objectHit
                if (result.objectHit.parent) {
                    result.objectHit.parent.isHovered = true;
                    mouseArea.cursorShape = Qt.PointingHandCursor
                }
            } else {
                hoveredObject = null;
                mouseArea.cursorShape = Qt.ArrowCursor
            }
        }

        onClicked: function (mouse) {
            var result = view3D.pick(mouse.x, mouse.y);
            if (result.objectHit && result.objectHit.pickable) {
                var candidate = result.objectHit.parent;
                while (candidate) {
                    if (candidate.objectIndex !== undefined) {
                        clickTimer.pendingVertexId = candidate.objectIndex;
                        clickTimer.restart();
                        return;
                    }
                    candidate = candidate.parent;
                }
            } else {
                root.backgroundClicked();
            }
        }

        onDoubleClicked: function (mouse) {
            if (clickTimer.running) {
                clickTimer.stop();
                clickTimer.pendingVertexId = -1;
            }

            var result = view3D.pick(mouse.x, mouse.y);
            if (result.objectHit && result.objectHit.pickable) {
                var candidate = result.objectHit.parent;
                while (candidate) {
                    if (candidate.objectIndex !== undefined) {
                        root.objectDoubleClicked(candidate.objectIndex);
                        return;
                    }
                    candidate = candidate.parent;
                }
            }
        }

        onExited: {
            if (hoveredObject && hoveredObject.parent) hoveredObject.parent.isHovered = false;
            hoveredObject = null;
            cursorShape = Qt.ArrowCursor
        }
    }

    Column {
        anchors.right: parent.right; anchors.top: parent.top; anchors.margins: 20; spacing: 10;
        Rectangle {
            width: 200; height: 150; color: "#1a1a2e"; opacity: 0.9; radius: 10; border.color: "#4a5568"; border.width: 1;
            Column {
                anchors.fill: parent; anchors.margins: 15; spacing: 8;
                Text { text: "Керування"; color: "#e0e0e0"; font.bold: true; font.pixelSize: 14 }
                Text { text: "ЛКМ + рух: Обертання"; color: "#b0b0b0"; font.pixelSize: 11; wrapMode: Text.WordWrap; width: parent.width }
                Text { text: "Коліщатко: Zoom"; color: "#b0b0b0"; font.pixelSize: 11 }
                Text { text: "ПКМ + рух: Панорама"; color: "#b0b0b0"; font.pixelSize: 11; wrapMode: Text.WordWrap; width: parent.width }
                Text { text: "Клік на об'єкті: Вибір"; color: "#b0b0b0"; font.pixelSize: 11; wrapMode: Text.WordWrap; width: parent.width }
            }
        }
        Rectangle {
            width: 200; height: 60; color: "#1a1a2e"; opacity: 0.9; radius: 10; border.color: "#4a5568"; border.width: 1;
            Column {
                anchors.centerIn: parent; spacing: 5;
                Text { text: "Об'єктів у галактиці:"; color: "#b0b0b0"; font.pixelSize: 11; anchors.horizontalCenter: parent.horizontalCenter }
                Text { text: celestialModel ? celestialModel.rowCount() : "0"; color: "#4fd1c5"; font.bold: true; font.pixelSize: 20; anchors.horizontalCenter: parent.horizontalCenter }
            }
        }
    }
}