# Allegro++

## How to build

```bash
$ git clone https://github.com/aviallon/AllegroPP.git
$ cd AllegroPP
$ mkdir build
$ cd build
$ cmake ..
$ make -j$(nproc)
$ sudo make install
```

## How to use

You first need C++14 at least, because the filesystem TS is needed. Of course, C++17 is even better.

Simply include `<allegro++/allegro.h>`, and everything will automatically get included

You also need to link against these libraries :
```
allegro++
allegro
allegro_image
allegro_primitives
allegro_memfile
allegro_ttf
allegro_dialog
allegro_color
allegro_font
armadillo
stdc++fs
```
The last one is needed if you are using GCC 8 and below. For Clang, it's `c++fs`.

It works a little bit like Tkinter, for Python users.

```c++
#include <allegro++/allegro.h>

using namespace AllegroPP;

void draw(Allegro* allegro, float fps){
   allegro->clearScreen(); // screen is not cleared by default
   allegro->draw_text(640/2, 480/2, "Hello, world!", allegro->rgb(0, 0, 0)); // Draw "Hello, world!" in the middle of the screen (in black).
}

int main(){
   Allegro::init(); // Required before doing anything else
   Allegro window = Allegro();

   window.createWindow(30, 640, 480, ALLEGRO_WINDOWED); // Create a window with a width of 640 and a height of 480, refreshing at 30 fps.

   window.setRedrawFunction(&draw);

   Allegro::startLoop();

   return 0;
}
```

I'll give more detailed examples in a dedicated folder (see example0 for a complex multi-window example, with heavy animation)
