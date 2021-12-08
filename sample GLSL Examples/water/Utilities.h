// Copyright Jay Conrod 2010.
// http://jayconrod.com
// jayconrod@gmail.com
//
// You are free to modify and distribute this code as long as this
// copyright notice is maintained.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 

#ifndef Utilities_h
#define Utilities_h

#include <string>

struct Pair {
    Pair() {}
    Pair(float u, float v) : u(u), v(v) {}

    float u, v;
};

struct Color {
    Color(float red, float green, float blue, float alpha) :
    red(red), green(green), blue(blue), alpha(alpha) {}

    float* array() { return (float*) this; }

    float red, green, blue, alpha;
};  

float clamp(float min, float value, float max);
std::string format(const std::string &fmt, ...);
double getTime();
bool isPowerOfTwo(int n);
int padToPowerOfTwo(int n);
std::string readTextFromFile(const std::string &filename);
float uniformRandomInRange(float min, float max);

#endif
