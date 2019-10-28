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
