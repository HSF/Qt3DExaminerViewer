import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import QtQuick.Scene3D 2.15

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Button {
        id: button1
        text: "button1"
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 10
        }
    }

    Scene3D {
        focus: true
        anchors {
            top: button1.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 10
        }
        aspects: ["input", "logic"]
        Win3D {}
    }

    /*
    TextField {
        id: textF
        text: "Edit me"
        anchors {
            top: parent.top
            left: parent.left
        }
        font {
            family: "Helvetica"
            pointSize: 12
            italic: true
        }
    }
    Label {
        text: calculateText()
        anchors {
            top: textF.bottom
            topMargin: 20
            left: parent.left
        }
        color: colorByTime()
    }
    function calculateText(){
        var x = textF.text;
        return "(" + x + ")";
    }
    function colorByTime(){
        var date = new Date();
        var seconds = date.getSeconds();
        if(seconds < 15) return "red";
        if(seconds < 30) return "yellow";
        if(seconds < 45) return "green";
        return "purple";
    }*/
}
