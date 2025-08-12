#ifndef RECTANGLE_H
#define RECTANGLE_H

template<typename T>
struct Rectangle {
    T x;
    T y;
    T width;
    T height;

    Rectangle() = default;

    Rectangle(T x_, T y_, T width_, T height_)
            : x(x_), y(y_), width(width_), height(height_) {
    }

    T area() const {
        return width * height;
    }

    bool contains(T px, T py) const {
        return px >= x && px <= (x + width) &&
               py >= y && py <= (y + height);
    }

    Rectangle<T> intersectRect(Rectangle<T> other) const {
        T x1 = std::max(x, other.x);
        T y1 = std::max(y, other.y);
        T x2 = std::min(x + width, other.x + other.width);
        T y2 = std::min(y + height, other.y + other.height);

        T w = x2 - x1;
        T h = y2 - y1;

        if (w <= 0 || h <= 0) {
            return Rectangle<T>(T(0), T(0), T(0), T(0)); // No intersection
        }

        return Rectangle<T>(x1, y1, w, h);
    }

    T intersectArea(Rectangle<T> other) const {
        return this->intersectRect(other).area();
    }
};

#endif //RECTANGLE_H
