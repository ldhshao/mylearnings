import QtQuick 2.0
import QtQuick.Controls 1.4
import FruitModel 1.0

Item{
    width: 200
    height:200

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

        //model: fruitModel
        model: mymodel
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

   // ListModel {
   //   id: fruitModel

   //   ListElement {
   //       name: "Apple"
   //       cost: 2.45
   //   }
   //   ListElement {
   //       name: "Orange"
   //       cost: 3.25
   //   }
   //  ListElement {
   //      name: "Banana"
   //      cost: 1.95
   //  }
   // }
   Timer{
       property int idx: 0
       interval: 500; running: true; repeat: true
       //onTriggered: time.text = Date().toString()
       onTriggered: function(){
           //mymodel.rowReset(0);
           mytbl.model = null;
           mytbl.model = mymodel;
       }
   }
    Text{
        anchors.top:mytbl.bottom
        id:time
    }
}

