#ifndef CANVAS_ITY_UTILSO
#define CANVAS_ITY_UTILSO

#include "canvas_ity.h"

namespace canvas_ity
{

    // 2D vector math operations
    const float pi = 3.14159265f;
    xy::xy() : x(0.0f), y(0.0f) {}
    xy::xy(float new_x, float new_y) : x(new_x), y(new_y) {}
    static xy &operator+=(xy &left, xy right)
    {
        left.x += right.x;
        left.y += right.y;
        return left;
    }
    static xy &operator-=(xy &left, xy right)
    {
        left.x -= right.x;
        left.y -= right.y;
        return left;
    }
    static xy &operator*=(xy &left, float right)
    {
        left.x *= right;
        left.y *= right;
        return left;
    }
    static xy const operator+(xy left, xy right)
    {
        return left += right;
    }
    static xy const operator-(xy left, xy right)
    {
        return left -= right;
    }
    static xy const operator*(float left, xy right)
    {
        return right *= left;
    }
    static xy const operator*(affine_matrix const &left, xy right)
    {
        return xy(left.a * right.x + left.c * right.y + left.e,
                  left.b * right.x + left.d * right.y + left.f);
    }
    static float dot(xy left, xy right)
    {
        return left.x * right.x + left.y * right.y;
    }
    static float length(xy that)
    {
        return sqrtf(dot(that, that));
    }
    static float direction(xy that)
    {
        return atan2f(that.y, that.x);
    }
    static xy const normalized(xy that)
    {
        return 1.0f / std::max(1.0e-6f, length(that)) * that;
    }
    static xy const perpendicular(xy that)
    {
        return xy(-that.y, that.x);
    }
    static xy const lerp(xy from, xy to, float ratio)
    {
        return from + ratio * (to - from);
    }
    // ensure 0 <= angle < 360
    float normalize_angle(float angle)
    {
        return fmodf(angle, 360) + (angle < 0 ? 360 : 0);
    }

    // RGBA color operations
    rgba::rgba() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
    rgba::rgba(float new_r, float new_g, float new_b, float new_a)
        : r(new_r), g(new_g), b(new_b), a(new_a) {}
    static rgba &operator+=(rgba &left, rgba right)
    {
        left.r += right.r;
        left.g += right.g;
        left.b += right.b;
        left.a += right.a;
        return left;
    }
    // static rgba &operator-=( rgba &left, rgba right ) {
    //     left.r -= right.r; left.g -= right.g; left.b -= right.b;
    //     left.a -= right.a; return left; }
    static rgba &operator*=(rgba &left, float right)
    {
        left.r *= right;
        left.g *= right;
        left.b *= right;
        left.a *= right;
        return left;
    }
    static rgba const operator+(rgba left, rgba right)
    {
        return left += right;
    }
    // static rgba const operator-( rgba left, rgba right ) {
    //     return left -= right; }
    static rgba const operator*(float left, rgba right)
    {
        return right *= left;
    }
#if (CANVAS_ITY_IMPLEMENTATION + 0) & LINEARIZE_RGB
    static float linearized(float value)
    {
        return value < 0.04045f ? value / 12.92f : powf((value + 0.055f) / 1.055f, 2.4f);
    }
    static float delinearized(float value)
    {
        return value < 0.0031308f ? 12.92f * value : 1.055f * powf(value, 1.0f / 2.4f) - 0.055f;
    }
#else
    static float linearized(float value) { return value; }
    static float delinearized(float value) { return value; }
#endif
    static rgba const linearized(rgba that)
    {
        return rgba(linearized(that.r), linearized(that.g),
                    linearized(that.b), that.a);
    }
    static rgba const premultiplied(rgba that)
    {
        return rgba(that.r * that.a, that.g * that.a,
                    that.b * that.a, that.a);
    }
    static rgba const delinearized(rgba that)
    {
        return rgba(delinearized(that.r), delinearized(that.g),
                    delinearized(that.b), that.a);
    }
    static rgba const unpremultiplied(rgba that)
    {
        static float const threshold = 1.0f / 8160.0f;
        return that.a < threshold ? rgba(0.0f, 0.0f, 0.0f, 0.0f) : rgba(1.0f / that.a * that.r, 1.0f / that.a * that.g, 1.0f / that.a * that.b, that.a);
    }
    static rgba const clamped(rgba that)
    {
        return rgba(std::min(std::max(that.r, 0.0f), 1.0f),
                    std::min(std::max(that.g, 0.0f), 1.0f),
                    std::min(std::max(that.b, 0.0f), 1.0f),
                    std::min(std::max(that.a, 0.0f), 1.0f));
    }

    // Helpers for TTF file parsing
    static int unsigned_8(std::vector<unsigned char> &data, int index)
    {
        return data[static_cast<size_t>(index)];
    }
    static int signed_8(std::vector<unsigned char> &data, int index)
    {
        size_t place = static_cast<size_t>(index);
        return static_cast<signed char>(data[place]);
    }
    static int unsigned_16(std::vector<unsigned char> &data, int index)
    {
        size_t place = static_cast<size_t>(index);
        return data[place] << 8 | data[place + 1];
    }
    static int signed_16(std::vector<unsigned char> &data, int index)
    {
        size_t place = static_cast<size_t>(index);
        return static_cast<short>(data[place] << 8 | data[place + 1]);
    }
    static int signed_32(std::vector<unsigned char> &data, int index)
    {
        size_t place = static_cast<size_t>(index);
        return (data[place + 0] << 24 | data[place + 1] << 16 |
                data[place + 2] << 8 | data[place + 3] << 0);
    }

}

#endif