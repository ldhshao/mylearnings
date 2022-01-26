import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    TableView {
        property int lastSelRow: -1

        id: mytbl
        width: 200
        height: 100
        TableViewColumn{
            width: 100
            title:"fruit"
            role:"name"
        }
        TableViewColumn{
            width: 60
            title:"price"
            role:"cost"
        }

        model: fruitModel
        onClicked: {
            console.log("current row " + this.currentRow);
            console.log("selection mode " + this.selectionMode);
            if (this.selectionMode == SelectionMode.ContiguousSelection){
                lastSelRow = (lastSelRow < this.currentRow)? lastSelRow: this.currentRow;
                this.selection.clear();
                this.selection.select(lastSelRow, this.currentRow);
            }
            else{
                lastSelRow = this.currentRow;
            }
        }

        Keys.onPressed: {
            if (event.key === Qt.Key_Control) {
              this.selectionMode = SelectionMode.ExtendedSelection;
              event.accepted = true;
            }
            else if (event.key === Qt.Key_Shift) {
              this.selectionMode = SelectionMode.ContiguousSelection;
              event.accepted = true;
            }
        }
        Keys.onReleased: {
            if (event.key === Qt.Key_Control) {
              this.selectionMode = SelectionMode.SingleSelection;
              event.accepted = true;
            }
            else if (event.key === Qt.Key_Shift) {
              this.selectionMode = SelectionMode.SingleSelection;
              event.accepted = true;
            }

        }
    }

    ListModel {
      id: fruitModel

      ListElement {
          name: "Apple"
          cost: 2.45
      }
      ListElement {
          name: "Orange"
          cost: 3.25
      }
     ListElement {
         name: "Banana"
         cost: 1.95
     }
    }
    ListView {
        id: mylist1
        width: 200
        height: 100
        anchors {top:mytbl.bottom; left:mytbl.left}
        focus: true

        model: mymodel
        delegate: Text{
            text: "fruit: "+ name + " " + cost
        }
        Component.onCompleted: {
            console.log(mymodel);
        }
    }
    Text {
        id: notifybar
        anchors {top: mylist1.bottom; left:mylist1.left}

        Timer {
          interval: 2000; running: true; repeat: true
          onTriggered: {
              notifybar.text = "timer: " + mymodel.index(0,0,mymodel).data;
          }
        }
    }
}
