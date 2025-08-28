import QtQuick 2.4

QtObject {
    id: stateFlow

    property var screenFlowStack: []
    property string screenFlow: screenFlowStack.length > 0 ? screenFlowStack[screenFlowStack.length - 1] : ""

    function setScreenFlow(flow) {
        var index = screenFlowStack.indexOf(flow);

        // If already in stack, remove old occurrence
        if (index !== -1) {
            screenFlowStack.splice(index, 1);
        } else if (screenFlowStack.length > 25) {
            // keep max 25 elements
            screenFlowStack.shift();
        }

        // Push new (or moved) flow to top
        screenFlowStack.push(flow);
        screenFlow = flow;
    }

    function backScreen() {
        if (screenFlowStack.length === 0)
            return;
        screenFlowStack.pop();
        screenFlow = screenFlowStack.length > 0 ? screenFlowStack[screenFlowStack.length - 1] : "";
    }

    function clearState() {
        screenFlowStack = [];
        screenFlow = "";
    }

    function isScreenFlow(flow) {
        if (screenFlowStack.length === 0)
            return false;
        return screenFlowStack[screenFlowStack.length - 1] === flow;
    }

    function isEmptyScreenFlow() {
        return screenFlowStack.length === 0;
    }
}