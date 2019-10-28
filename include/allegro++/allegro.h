#pragma once

#define __ENABLE_GUI__

#include <cmath>
#include <cctype>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <memory>

#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_memfile.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/file.h>
//#include <allegro5/primitives.h>
#if __cplusplus >= 201703L
   #include <filesystem>
#else
   #include <experimental/filesystem>
   namespace std {
      namespace filesystem = std::experimental::filesystem::v1;
   }
#endif

#include <allegro++/math.h>
#include <allegro++/mouse.h>
#include <allegro++/color.h>
#include <allegro++/primitives.h>
#include <allegro++/sprites.h>
#include <allegro++/gui.h>

#define MAX(a, b) (((a > b))?(a):(b))
#define MIN(a, b) (((a < b))?(a):(b))