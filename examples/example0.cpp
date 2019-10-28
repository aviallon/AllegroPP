#include <allegro++/allegro.h>
#include <functional>
#include <cmath>

using namespace std;
using namespace AllegroPP;

double i = 0.0;
double i2 = Math::PI/2;
void fen1redr(Allegro* allegro, float FPS){
   allegro->clearScreen();
   
   i += Math::PI/256;
   allegro->draw_text(640/2, 480/2, "Hello, aviallon!", allegro->rgb(cos(i)*127+128, 0, 0)); // Draw "Hello, world!" in the middle of the screen (in black).
}

void fen2redr(Allegro* allegro, float FPS){
   allegro->clearScreen();
   
   i2 += Math::PI/256;
   allegro->draw_text(200/2, 200/2, "Hello, world!", allegro->rgb(cos(i2)*127+128, 0, 0)); // Draw "Hello, world!" in the middle of the screen (in black).
}

void testBtn(Allegro* allegro, Button* btn){
   std::cout << btn->name << std::endl;
}

void onMouseMove(Allegro* allegro, void* context, uint16_t event, int x, int y){
   cout << "(" << x << ";" << y << ")" << endl;
}

void onKeyUp(Allegro* allegro, void* context, uint16_t event, uint8_t keycode){
   cout << int(keycode) << endl;
}

void animateFen1(Allegro* allegro, float fps){
   //cout << "Sleeping Fen 1 for a full second... but it shall not affect fen 2 !" << endl;
   std::this_thread::sleep_for(std::chrono::milliseconds(33));
   //cout << "Slept well :D" << endl;
}

int main(){
   Allegro::init(); // Required before doing anything else
   
   Allegro fen1 = Allegro();
   Allegro fen2 = Allegro();
   
   fen1.createWindow(30, 640, 480); // Create a window with a width of 640 and a height of 480, refreshing at 30 fps.
   fen2.createWindow(30, 200, 200);
   
   cout << 1.0f/30 << endl;

   fen1.setRedrawFunction(&fen1redr);
   fen2.setRedrawFunction(&fen2redr);
   
   fen1.setAnimateFunction(&animateFen1);
   
   //fen1.bindMouseMove(&onMouseMove);
   fen1.bindKeyUp(&onKeyUp);
   
   fen1.getGUI()->displayMessage("test", 1000);
   
   fen1.getGUI()->newBtn("Test", 0, 0, 60, 150, &testBtn);
   fen2.getGUI()->newBtn("Pomme", 100, 0, 60, 150, &testBtn);

   Allegro::startLoop(); // starts the loop

   return 0;
}