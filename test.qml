import Sdl 2.0

Window {
    id: win
    title: "This is a test"
    property int clicks

    Image {
        source: "abmp.bmp"
        width: parent.width
        height: parent.height

        Rectangle {
            x:  10
            y:  10
            width: 100
            height: 200

            onPressed: {
                x = x + 10
                y = y + 10

                win.title = "You have clicked the rectangle " + ++win.clicks + " times"
            }
        }
    }
}
