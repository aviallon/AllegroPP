#include <allegro++/allegro.h>

using namespace AllegroPP;

class Context {
public:
   int x = 0;
   int y = 0;
};

void redraw(Allegro* allegro, float fps){
   Context* ctx = (Context*)allegro->getContext();
   allegro->draw_rectangle(0, 0, 640, 480, Color(255, 255, 255), 1, true); // we erase the screen
   allegro->draw_ellipse_r(ctx->x, ctx->y, 8, 8, Color(0, 0, 0), 2, false); //... and draw an circle at the mouse coordinates.
}

void mouseMove(Allegro* allegro, void* context, uint16_t event, int x, int y){
	if(event & Allegro::MOUSE_MOVED && !(event & Allegro::MOUSE_WHEELED)) {
		Context* ctx = (Context*)allegro->getContext();
      ctx->x = x;
      ctx->y = y;
	}

	// The context pointer is used to share any kind of information between different functions bound to events.
}

int main(){
   Allegro::init();
	Allegro allegro = Allegro();
	allegro.createWindow(60, 640, 480);

   Context ctx;
   
   allegro.setContext((void*)&ctx);

	allegro.bindMouseMove(&mouseMove); // bind mouse moved events to mouseMove function
   
   allegro.setRedrawFunction(&redraw);
   
	Allegro::startLoop();

	return 0;
}
