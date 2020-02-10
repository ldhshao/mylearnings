import QtQuick 2.0
import Message 1.0

Item{
    width: 200
    height:200

    Message{
       id:msg
       author: "me"
    }

    Text {
       id: text1
       anchors{ top:parent.top; left:parent.left}
       text: msg.author    // invokes Message::author() to get this value

       Component.onCompleted: {
            msg.author = "Jonah2";
            console.log(msg.author);
       }
    }
}
