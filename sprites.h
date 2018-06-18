#ifndef SPRITES_H
#define SPRITES_H

#include <string>
#include <memory>

using std::string;
using std::shared_ptr;
using std::cout;

class Sprite{
	std::shared_ptr<ALLEGRO_BITMAP> sprite;
	bool isDefined = false;
public:
	Sprite(){
		
	}

	Sprite(shared_ptr<ALLEGRO_BITMAP> sprt){
		sprite = sprt;
		isDefined = true;
	}
	
	void drawSprite(int x, int y, unsigned w = 0, unsigned h=0, bool alignCenter = false){
		if(w + h == 0){
			al_draw_bitmap(sprite.get(), x, y, 0);
		} else {
			float sw = al_get_bitmap_width(sprite.get());
			float sh = al_get_bitmap_height(sprite.get());
			if(alignCenter){
				x = x-w/2;
				y = y-h/2;
			}
			al_draw_scaled_bitmap(sprite.get(), 0, 0, sw, sh, x, y, w, h, 0);
		}
	}
	
	float getWidth(){
		return al_get_bitmap_width(sprite.get());
	}
	
	float getHeight(){
		return al_get_bitmap_height(sprite.get());
	}
	
	operator bool(){
		return isDefined;
	}
};

class SpriteMap{
	string rel_path;
	std::shared_ptr<ALLEGRO_BITMAP> spritemap;

public:
	
	SpriteMap(){
		
	}

	SpriteMap(string rel_path){
		this->rel_path = rel_path;
		ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
		al_remove_path_component(path, -1);
		al_set_path_filename(path, rel_path.c_str());
		spritemap = shared_ptr<ALLEGRO_BITMAP>(al_load_bitmap(al_path_cstr(path, '/')), al_destroy_bitmap);
	}
	
	Sprite getSprite(int x, int y, int w, int h){
		shared_ptr<ALLEGRO_BITMAP> sprt(al_create_sub_bitmap(spritemap.get(), x, y, w, h), al_destroy_bitmap);
		return Sprite(sprt);
	}
	
	Sprite getWholeSprite(){
		shared_ptr<ALLEGRO_BITMAP> sprt(spritemap.get(), al_destroy_bitmap);
		return Sprite(sprt);
	}

};

#endif