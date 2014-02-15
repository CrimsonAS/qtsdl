import Sdl 2.0

Window {
    Image {
        source: "abmp.bmp"

        Rectangle {
            x:  Math.floor((Math.random()*500)+1);
            y:  Math.floor((Math.random()*500)+1);
        }
    }
}
