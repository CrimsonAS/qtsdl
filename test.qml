import Sdl 2.0

Window {
    id: win

    Image {
        source: "abmp.bmp"
        width: win.width
        height: win.height

        Rectangle {
            x:  10
            y:  10
            width: 100
            height: 200

            onPressed: {
                x = x + 10
                y = y + 10
            }
        }
    }
}
