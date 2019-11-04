#include <allegro++/allegro.h>
#include <iostream>

using namespace AllegroPP;
using namespace std;

double pi = ALLEGRO_PI;

struct Vector {
   float x = 0.0f;
   float y = 0.0f;
   float z = 0.0f;
};

struct Camera {
   Vector position;
   Vector xaxis; /* This represent the direction looking to the right. */
   Vector yaxis; /* This is the up direction. */
   Vector zaxis; /* This is the direction towards the viewer ('backwards'). */
   double vertical_field_of_view; /* In radians. */
};

struct Example {
   Camera camera;

   /* controls sensitivity */
   double mouse_look_speed;
   double movement_speed;

   /* keyboard and mouse state */
   int button[10];
   int key[ALLEGRO_KEY_MAX];
   int keystate[ALLEGRO_KEY_MAX];
   int mouse_dx, mouse_dy;

   /* control scheme selection */
   int controls;
   std::string controls_names[3];

   /* used to draw some info text */
   ALLEGRO_FONT *font;
   
   /* the vertex data */
   vector<ALLEGRO_VERTEX> v;
};

/* Calculate the dot product between two vectors. This corresponds to the
 * angle between them times their lengths.
 */
static double vector_dot_product(Vector a, Vector b)
{
   return a.x * b.x + a.y * b.y + a.z * b.z;
}

/* Return a vector multiplied by a scalar. */
static Vector vector_mul(Vector a, float s)
{
   Vector v = {a.x * s, a.y * s, a.z * s};
   return v;
}

/* Return the vector norm (length). */
static double vector_norm(Vector a)
{
   return sqrt(vector_dot_product(a, a));
}

/* Return a normalized version of the given vector. */
static Vector vector_normalize(Vector a)
{
   double s = vector_norm(a);
   if (s == 0)
      return a;
   return vector_mul(a, 1 / s);
}

/* In-place add another vector to a vector. */
static void vector_iadd(Vector &a, Vector b)
{
   a.x += b.x;
   a.y += b.y;
   a.z += b.z;
}

/* Rotate the camera around the given axis. */
static void camera_rotate_around_axis(Camera &c, Vector axis, double radians)
{
   ALLEGRO_TRANSFORM t;
   al_identity_transform(&t);
   al_rotate_transform_3d(&t, axis.x, axis.y, axis.z, radians);
   al_transform_coordinates_3d(&t, &c.xaxis.x, &c.xaxis.y, &c.xaxis.z);
   al_transform_coordinates_3d(&t, &c.yaxis.x, &c.yaxis.y, &c.yaxis.z);
   al_transform_coordinates_3d(&t, &c.zaxis.x, &c.zaxis.y, &c.zaxis.z);
}

/* Move the camera along its x axis and z axis (which corresponds to
 * right and backwards directions).
 */
static void camera_move_along_direction(Camera &camera, double right,
   double forward)
{
   vector_iadd(camera.position, vector_mul(camera.xaxis, right));
   vector_iadd(camera.position, vector_mul(camera.zaxis, -forward));
}

/* Get a vector with y = 0 looking in the opposite direction as the camera z
 * axis. If looking straight up or down returns a 0 vector instead.
 */
static Vector get_ground_forward_vector(Camera &camera)
{
   Vector move = vector_mul(camera.zaxis, -1);
   move.y = 0;
   return vector_normalize(move);
}

/* Get a vector with y = 0 looking in the same direction as the camera x axis.
 * If looking straight up or down returns a 0 vector instead.
 */
static Vector get_ground_right_vector(Camera &camera)
{
   Vector move = camera.xaxis;
   move.y = 0;
   return vector_normalize(move);
}

/* Like camera_move_along_direction but moves the camera along the ground plane
 * only.
 */
static void camera_move_along_ground(Camera &camera, double right,
   double forward)
{
   Vector f = get_ground_forward_vector(camera);
   Vector r = get_ground_right_vector(camera);
   camera.position.x += f.x * forward + r.x * right;
   camera.position.z += f.z * forward + r.z * right;
}

/* Calculate the pitch of the camera. This is the angle between the z axis
 * vector and our direction vector on the y = 0 plane.
 */
static double get_pitch(Camera &c)
{
   Vector f = get_ground_forward_vector(c);
   return asin(vector_dot_product(f, c.yaxis));
}

/* Calculate the yaw of the camera. This is basically the compass direction.
 */
static double get_yaw(Camera &c)
{
   return atan2(c.zaxis.x, c.zaxis.z);
}

/* Calculate the roll of the camera. This is the angle between the x axis
 * vector and its project on the y = 0 plane.
 */
static double get_roll(Camera &c)
{
   Vector r = get_ground_right_vector(c);
   return asin(vector_dot_product(r, c.yaxis));
}

static Example ex;

static void setup_3d_projection()
{
   
   ALLEGRO_TRANSFORM projection;
   ALLEGRO_DISPLAY *display = al_get_current_display();
   double dw = al_get_display_width(display);
   double dh = al_get_display_height(display);
   al_identity_transform(&projection);
   al_translate_transform_3d(&projection, 0, 0, -1);
   double f = tan(ex.camera.vertical_field_of_view / 2);
   al_perspective_transform(&projection, -1 * dw / dh * f, f,
      1,
      f * dw / dh, -f, 1000);
   al_use_projection_transform(&projection);
}

/* Adds a new vertex to our scene. */
static void add_vertex(double x, double y, double z, ALLEGRO_COLOR color)
{
  ALLEGRO_VERTEX v;
   v.x = x;
   v.y = y;
   v.z = z;
   v.color = color;
   ex.v.push_back(v);
}

/* Adds two triangles (6 vertices) to the scene. */
static void add_quad(double x, double y, double z,
   double ux, double uy, double uz,
   double vx, double vy, double vz, ALLEGRO_COLOR c1, ALLEGRO_COLOR c2)
{
   add_vertex(x, y, z, c1);
   add_vertex(x + ux, y + uy, z + uz, c1);
   add_vertex(x + vx, y + vy, z + vz, c2);
   add_vertex(x + vx, y + vy, z + vz, c2);
   add_vertex(x + ux, y + uy, z + uz, c1);
   add_vertex(x + ux + vx, y + uy + vy, z + uz + vz, c2);
}

/* Create a checkerboard made from colored quads. */
static void add_checkerboard()
{
   int x, y;
   ALLEGRO_COLOR c1 = al_color_name("yellow");
   ALLEGRO_COLOR c2 = al_color_name("green");

   for (y = 0; y < 20; y++) {
      for (x = 0; x < 20; x++) {
         double px = x - 20 * 0.5;
         double py = 0.2;
         double pz = y - 20 * 0.5;
         ALLEGRO_COLOR c = c1;
         if ((x + y) & 1) {
            c = c2;
            py -= 0.1;
         }
         add_quad(px, py, pz, 1, 0, 0, 0, 0, 1, c, c);
      }
   }
}

/* Create a skybox. This is simply 5 quads with a fixed distance to the
 * camera.
 */
static void add_skybox()
{
   Vector p = ex.camera.position;
   ALLEGRO_COLOR c1 = al_color_name("black");
   ALLEGRO_COLOR c2 = al_color_name("blue");
   ALLEGRO_COLOR c3 = al_color_name("white");

   /* Back skybox wall. */
   add_quad(p.x - 50, 0, p.z - 50, 100, 0, 0, 0, 50, 0, c1, c2);
   /* Front skybox wall. */
   add_quad(p.x - 50, 0, p.z + 50, 100, 0, 0, 0, 50, 0, c1, c2);
   /* Left skybox wall. */
   add_quad(p.x - 50, 0, p.z - 50, 0, 0, 100, 0, 50, 0, c1, c2);
   /* Right skybox wall. */
   add_quad(p.x + 50, 0, p.z - 50, 0, 0, 100, 0, 50, 0, c1, c2);

   /* Top of skybox. */
   add_vertex(p.x - 50, 50, p.z - 50, c2);
   add_vertex(p.x + 50, 50, p.z - 50, c2);
   add_vertex(p.x, 50, p.z, c3);

   add_vertex(p.x + 50, 50, p.z - 50, c2);
   add_vertex(p.x + 50, 50, p.z + 50, c2);
   add_vertex(p.x, 50, p.z, c3);

   add_vertex(p.x + 50, 50, p.z + 50, c2);
   add_vertex(p.x - 50, 50, p.z + 50, c2);
   add_vertex(p.x, 50, p.z, c3);

   add_vertex(p.x - 50, 50, p.z + 50, c2);
   add_vertex(p.x - 50, 50, p.z - 50, c2);
   add_vertex(p.x, 50, p.z, c3);
}


void redraw(Allegro* allegro, float fps){
   allegro->clearScreen();
   //allegro->draw_rectangle(0, 0, allegro->getDisplayWidth()/2, allegro->getDisplayHeight()/2, Color(0xFF00FF), 1, true);
   
   
   ALLEGRO_TRANSFORM projection = *al_get_current_projection_transform();

   setup_3d_projection();

   ALLEGRO_COLOR back = al_color_name("black");
   ALLEGRO_COLOR front = al_color_name("white");
   al_clear_to_color(back);

   /* We use a depth buffer. */
   //ALLEGRO_RENDER_STATE s;
   al_set_render_state(ALLEGRO_RENDER_STATE::ALLEGRO_DEPTH_TEST, 1);
   al_clear_depth_buffer(1);
   
   /* Recreate the entire scene geometry - this is only a very small example
    * so this is fine.
    */
   ex.v = vector<ALLEGRO_VERTEX>();
   add_checkerboard();
   add_skybox();

   /* Construct a transform corresponding to our camera. This is an inverse
    * translation by the camera position, followed by an inverse rotation
    * from the camera orientation.
    */
   ALLEGRO_TRANSFORM t;
   al_build_camera_transform(&t,
      ex.camera.position.x, ex.camera.position.y, ex.camera.position.z,
      ex.camera.position.x - ex.camera.zaxis.x,
      ex.camera.position.y - ex.camera.zaxis.y,
      ex.camera.position.z - ex.camera.zaxis.z,
      ex.camera.yaxis.x, ex.camera.yaxis.y, ex.camera.yaxis.z);
   al_use_transform(&t);
   al_draw_prim(&ex.v[0], nullptr, nullptr, 0, (int)(ex.v.size()),
     ALLEGRO_PRIM_TYPE::ALLEGRO_PRIM_TRIANGLE_LIST);

   /* Restore projection. */
   al_identity_transform(&t);
   al_use_transform(&t);
   al_use_projection_transform(&projection);
   
   /* Draw some text. */
//   int th = al_get_font_line_height(ex.font);
//   al_draw_textf(ex.font, front, 0, th * 0, 0,
//      "look: %+3.1f/%+3.1f/%+3.1f (change with left mouse button and drag)",
//         -c.zaxis.x, -c.zaxis.y, -c.zaxis.z);
//   double pitch = get_pitch(c) * 180 / pi;
//   double yaw = get_yaw(c) * 180 / pi;
//   double roll = get_roll(c) * 180 / pi;
//   al_draw_textf(ex.font, front, 0, th * 1, 0,
//      "pitch: %+4.0f yaw: %+4.0f roll: %+4.0f", pitch, yaw, roll);
//   al_draw_textf(ex.font, front, 0, th * 2, 0,
//      "vertical field of view: %3.1f (change with Z/X)",
//         c.vertical_field_of_view * 180 / pi);
//   al_draw_textf(ex.font, front, 0, th * 3, 0, "move with WASD or cursor");
//   al_draw_textf(ex.font, front, 0, th * 4, 0, "control style: %s (space to change)",
//      ex.controls_names[ex.controls].ptr);
}

static void setup_scene()
{
   ex.camera.xaxis.x = 1;
   ex.camera.yaxis.y = 1;
   ex.camera.zaxis.z = 1;
   ex.camera.position.y = 2;
   ex.camera.vertical_field_of_view = 60 * pi / 180;

   ex.mouse_look_speed = 0.03;
   ex.movement_speed = 0.05;

   ex.controls_names[0] = "FPS";
   ex.controls_names[1] = "airplane";
   ex.controls_names[2] = "spaceship";

   ex.font = al_create_builtin_font();
}

int main(int argc, char** argv){
   Allegro::init();
   
   
   al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
   al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
   al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 16, ALLEGRO_SUGGEST);
   
   Allegro window = Allegro();
   
   window.createWindow(60, 640, 480, ALLEGRO_RESIZABLE);
   
   setup_scene();
   
   window.setRedrawFunction(&redraw);
   
   Allegro::startLoop();
   
   return 0;
}