#include <allegro++/allegro.h>
#include <functional>
#include <cmath>
#include <string>
#include <vector>

using namespace std;
using namespace AllegroPP;

class Context {
public:
   bool winClosed = false;
   unsigned animate_counter = 0;
   
   queue<Allegro> windows;
   queue<std::string> strings;
   
   unsigned owner_number = 0;
};

double i = 0.0;
double i2 = Math::PI/2;
void fen1redr(Allegro* allegro, float FPS){
   allegro->clearScreen();
   
   Context *ctx = (Context*)allegro->getContext();
   
   std::cout << "Test..." << std::endl;
   
   if(ctx->winClosed){
      Color green(0x00FF00);
      allegro->draw_text(640/2, 480/2+40, "A window has been closed !", green.toAllegro());
   }
   
   {
      std::stringstream ss;
      ss << "Animate counter : " << ctx->animate_counter;
      
      allegro->draw_text(640/2, 480/2+80, ss.str().c_str(), Color(0x0000FF).toAllegro());
   }
   
   {
      std::stringstream ss;
      ss << "Frametime : " << allegro->getFrametime() << "ms";
      
      allegro->draw_text(allegro->getDisplayWidth(), 0, ss.str().c_str(), Color(0).toAllegro(), ALLEGRO_ALIGN_RIGHT);
   }
   
   i += Math::PI/256;
   allegro->draw_text(640/2, 480/2, "Hello, aviallon!", allegro->rgb(cos(i)*127+128, 0, 0)); // Draw "Hello, world!" in the middle of the screen.
}

void fen2redr(Allegro* allegro, float FPS){
   allegro->clearScreen();
   
   Context *ctx = (Context*)allegro->getContext();
   
   if(ctx->winClosed){
      allegro->draw_text(200/2, 200/2+40, "A window has been closed !", Color(0x00FF00).toAllegro());
   }
   
   i2 += Math::PI/256;
   allegro->draw_text(200/2, 200/2, "Hello, world!", allegro->rgb(cos(i2)*127+128, 0, 0)); // Draw "Hello, world!" in the middle of the screen.
}

void popRedraw(Allegro* allegro, float fps){
   allegro->clearScreen();
   
   //std::string str = *(std::string*)allegro->getContext();
   
   std::cout << "Small window : " << "apples" << std::endl;
   
   allegro->draw_text(allegro->getDisplayWidth()/2, allegro->getDisplayHeight(), "str");
}

void testBtn(Allegro* allegro, Button* btn){
   std::cout << btn->name << std::endl;
   //allegro->getGUI()->displayMessage(btn->name, 500);
   
   Context *ctx = (Context*)allegro->getContext();
   
   ctx->windows.push(Allegro());
   std::cout << "Thread ID : " << ctx->windows.back().getThreadId() << std::endl;
   ctx->windows.back().createWindow(24, 100, 100);
   //ctx->strings.push_back(std::string("WTF"));
   //ctx->windows[ctx->windows.size() - 1].setContext(&ctx->strings[ctx->strings.size() - 1]);
   ctx->windows.back().setRedrawFunction(&popRedraw);
}

void onMouseMove(Allegro* allegro, void* context, uint16_t event, int x, int y){
   cout << "(" << x << ";" << y << ")" << endl;
}

void onKeyUp(Allegro* allegro, void* context, uint16_t event, uint8_t keycode){
   cout << int(keycode) << endl;
}

void animateFen1(Allegro* allegro, float fps){
   Context *ctx = (Context*)allegro->getContext();
   ctx->animate_counter++;
   
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void onWindowClosed(Allegro* allegro, void* context){
   Context *ctx = (Context*)context;
   ctx->winClosed = true;
   ctx->owner_number--;
   
   if(ctx->owner_number == 0){
      while(ctx->windows.size() > 0){
         ctx->windows.front().quit();
      }
   }
   
   cout << "You closed a window !" << endl;
}

void onWindowCreated(Allegro* allegro, void* context){
   Context *ctx = (Context*)context;
   
   ctx->owner_number++;
}

int main(){
   Allegro::init(); // Required before doing anything else
   
   Context ctx;
      
   Allegro fen1 = Allegro();
   Allegro fen2 = Allegro();
   
   fen1.setContext((void*)&ctx);
   fen2.setContext((void*)&ctx);
   
   fen1.bindWindowCreated(&onWindowCreated);
   fen2.bindWindowCreated(&onWindowCreated);
   
   fen1.createWindow(10, 640, 480); // Create a window with a width of 640 and a height of 480, refreshing at 30 fps.
   fen2.createWindow(10, 200, 200);
   
   cout << 1.0f/30 << endl;
   
   Vec2D v1(3, 1);
   Vec2D v2(-1, 0);
   
   // Yup, you can define colors like that x)
   Color red = 0xFF0000;
   Color blue = 0x0000FF;
   Color magenta = red + blue;
   
   cout << "v1 = " << v1 << ", " << "v2 = " << v2 << endl;
   cout << "v1·v2 = " << v1*v2 << "; v1*2 = " << 2*v1 << endl;
   cout << "red + blue = " << red << " + " << blue << " = " << magenta << endl;

   fen1.setRedrawFunction(&fen1redr);
   fen2.setRedrawFunction(&fen2redr);
   
   fen1.setAnimateFunction(&animateFen1);
   
   //fen1.bindMouseMove(&onMouseMove);
   fen1.bindKeyUp(&onKeyUp);
   
   fen1.bindWindowClosed(&onWindowClosed);
   fen2.bindWindowClosed(&onWindowClosed);
   
   fen1.getGUI()->newBtn("Test", 0, 0, 60, 150, &testBtn);
   fen2.getGUI()->newBtn("Pomme", 100, 0, 60, 150, &testBtn);

   Allegro::startLoop(); // starts the loop

   return 0;
}