import Sdl 2.0

Window {
    Image {
        source: "abmp.bmp"

        Rectangle {
            x:  10
            y:  10

            onPressed: {
                x = x + 10
                y = y + 10
            }
        }
    }
}
