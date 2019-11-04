#pragma once

#include <allegro++/allegro.h>

namespace AllegroPP {
   
   namespace Math {
      const double PI = 3.141592653589793238462643383279502884197169399375105820974;
      
      int clamp(double x, int min, int max);

      float clamp(double x, float min, float max);
   }
   
   class Vec2D {
   private:
      arma::vec2 vec;
   public:
   
      Vec2D(double x = 0.0, double y = 0.0);
      Vec2D(arma::vec2 v);
      
      Vec2D operator+(const Vec2D &v2);
      Vec2D operator-(const Vec2D &v2);
      Vec2D operator/(double k);
      
      friend Vec2D operator*(const Vec2D &v, double k);
      friend Vec2D operator*(double k, const Vec2D &v);
      
      /**
       * @brief Computes the dot product between two vecs
       * @param Vec2D
       */
      double operator*(const Vec2D &v2);
      
      friend std::ostream& operator<<(std::ostream& os, const Vec2D &v);
   };
   
   class Vec3D {
   private:
      arma::vec3 vec;
   public:
   
      Vec3D(double x = 0.0, double y = 0.0, double z = 0.0);
      Vec3D(arma::vec3 v);
      
      Vec3D operator+(const Vec3D &v2);
      Vec3D operator-(const Vec3D &v2);
      Vec3D operator/(double k);
      
      double getX();
      double getY();
      double getZ();
      
      void setX(double x);
      void setY(double y);
      void setZ(double z);
      
      friend Vec3D operator*(const Vec3D &v, double k);
      friend Vec3D operator*(double k, const Vec3D &v);
      
      /**
       * @brief Computes the dot product between two vecs
       * @param Vec2D
       */
      double operator*(const Vec3D &v2);
      
      /**
       * @brief Returns vectorial product of the two vectors
       * @param v2
       */
      Vec3D operator^(const Vec3D &v2);
      
      friend std::ostream& operator<<(std::ostream& os, const Vec3D &v);
   };

}