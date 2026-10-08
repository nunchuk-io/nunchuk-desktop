import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Item {
    id: calloutRoot
    width: 300  // Parent width
    height: calloutShape.height // Parent height
    z: 1000  // Ensure the callout is on top of other elements
    // Define triangle height and width as properties
    property int triangleHeight: 15 // Triangle height
    property int triangleWidth: 25   // Triangle width
    property int shapeRadius: 12
    property int shapeMargins: 12
    property string contentText: ""

    readonly property int calloutBOTTOM: 0
    readonly property int calloutTOP: 1
    property int calloutPosition: calloutTOP

    function setCalloutPosition(mouseX, mouseY, mouseGlobalY) {
        if(mouseGlobalY > calloutRoot.height) {
            if(calloutPosition !== calloutBOTTOM){
                calloutPosition = calloutBOTTOM
                calloutShape.requestPaint()
            }
            x = mouseX - width / 2
            y = mouseY - height
        }
        else {
            if(calloutPosition !== calloutTOP){
                calloutPosition = calloutTOP
                calloutShape.requestPaint()
            }
            x = mouseX - width / 2
            y = mouseY
        }
    }
    function convertToClickableLinks(text) {
        // Regular expression to match URLs (simplified)
        var urlRegex = /(https?:\/\/[^\s]+)/g;
        // Replace URLs in the text with clickable <a> tags
        return text.replace(urlRegex, "<a href='$1'>$1</a>");
    }

    // Drop shadow effect for the shape
    DropShadow {
        anchors.fill: parent
        radius: 10
        samples: 16
        color: "#444444"  // Dark gray with transparency for a subtle shadow
        source: calloutShape
        horizontalOffset: 3
        verticalOffset: 3
    }

    // Main callout shape with a triangle pointer
    Canvas {
        id: calloutShape
        width: parent.width  // Set width to follow parent size
        height: textContainer.height + triangleHeight + 20 // Adjust height based on text height

        onPaint: {
            var ctx = getContext("2d");

            // Clear the canvas
            ctx.clearRect(0, 0, width, height);

            var rectWidth = width;  // Use canvas width
            var rectHeight = height - triangleHeight;  // Adjust for triangle pointer
            var radius = shapeRadius;

            // Draw the rounded rectangle with triangle pointer
            ctx.fillStyle = "white";
            ctx.strokeStyle = "gray";
            ctx.lineWidth = 1;

            ctx.beginPath();

            // Check triangle position
            if (calloutPosition === calloutBOTTOM) {
                // Draw the shape with the triangle at the bottom
                ctx.moveTo(radius, 0);
                ctx.lineTo(rectWidth - radius, 0);
                ctx.quadraticCurveTo(rectWidth, 0, rectWidth, radius);
                ctx.lineTo(rectWidth, rectHeight - radius);
                ctx.quadraticCurveTo(rectWidth, rectHeight, rectWidth - radius, rectHeight);
                ctx.lineTo((rectWidth / 2) + (triangleWidth / 2), rectHeight); // Right base of the triangle
                ctx.lineTo(rectWidth / 2, rectHeight + triangleHeight); // Bottom point of the triangle
                ctx.lineTo((rectWidth / 2) - (triangleWidth / 2), rectHeight); // Left base of the triangle
                ctx.lineTo(radius, rectHeight);  // Continue with the rounded rectangle path
                ctx.quadraticCurveTo(0, rectHeight, 0, rectHeight - radius);
                ctx.lineTo(0, radius);
                ctx.quadraticCurveTo(0, 0, radius, 0);
            } else {
                // Draw the shape with the triangle at the top
                ctx.moveTo(radius, triangleHeight);
                ctx.lineTo((rectWidth / 2) - (triangleWidth / 2), triangleHeight); // Left base of the triangle
                ctx.lineTo(rectWidth / 2, 0); // Top point of the triangle
                ctx.lineTo((rectWidth / 2) + (triangleWidth / 2), triangleHeight); // Right base of the triangle
                ctx.lineTo(rectWidth - radius, triangleHeight);
                ctx.quadraticCurveTo(rectWidth, triangleHeight, rectWidth, triangleHeight + radius);
                ctx.lineTo(rectWidth, rectHeight + triangleHeight - radius);
                ctx.quadraticCurveTo(rectWidth, rectHeight + triangleHeight, rectWidth - radius, rectHeight + triangleHeight);
                ctx.lineTo(radius, rectHeight + triangleHeight);
                ctx.quadraticCurveTo(0, rectHeight + triangleHeight, 0, rectHeight + triangleHeight - radius);
                ctx.lineTo(0, triangleHeight + radius);
                ctx.quadraticCurveTo(0, triangleHeight, radius, triangleHeight);
            }

            ctx.closePath();
            ctx.fill();
            ctx.stroke();
        }

        // Center the text in the rectangle area
        Rectangle {
            id: textContainer
            width: calloutShape.width - shapeMargins // Adjust for padding
            anchors.centerIn: parent
            anchors.verticalCenterOffset: calloutPosition === calloutBOTTOM ? -(triangleHeight / 2) : (triangleHeight / 2)
            height: text.height
            color: "#FFFFFF"
            z: 1001  // Ensure the callout is on top of other elements
            Text {
                id: text
                width: parent.width
                text: convertToClickableLinks(contentText)
                color: "#000000"
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.pixelSize: 12 // Optional: Adjust font size if needed
            }
        }
    }
}
