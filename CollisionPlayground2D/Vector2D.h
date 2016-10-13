#ifndef _VECTOR2D_H
#define _VECTOR2D_H

template <typename T>

class Vector2D {
public:
	T x, y;

	Vector2D() : x(0), y(0) {}
	Vector2D(T x, T y) : x(x), y(y) {}
	Vector2D(const Vector2D& o) : x(o.x), y(o.y) {}

	static Vector2D<T> zero() { return Vector2D<T>(static_cast<T>(0), static_cast<T>(0)); }
	static Vector2D<T> one()  { return Vector2D<T>(static_cast<T>(1), static_cast<T>(1)); }

	Vector2D operator+(const Vector2D& o) const { return Vector2D( x + o.x, y + o.y); }
	Vector2D operator-(const Vector2D& o) const { return Vector2D( x - o.x, y - o.y); }
	Vector2D operator*(T t) const { return Vector2D( x * t, y * t); }
	Vector2D operator/(T t) const { return Vector2D( x / t, y / t); }

	Vector2D& operator+=(const Vector2D& o) { x += o.x; y += o.y; return *this; }
	Vector2D& operator-=(const Vector2D& o) { x -= o.x; y -= o.y; return *this; }
	Vector2D& operator*=(T t) {	x *= t; y *= t;	return *this; }
	Vector2D& operator/=(T t) {	x /= t; y /= t;	return *this; }
	
	bool operator==(const Vector2D& o) const { return x == o.x && y == o.y; }
	bool isZero() const { return x == static_cast<T>(0) && y == static_cast<T>(0); }

	T cross(const Vector2D& o) const { return x*o.y - y*o.x; }
	T dot(const Vector2D& o)   const { return x*o.x + y*o.y; }

	T magnitude2() const { return x*x + y*y; }
	T magnitude()  const { return std::sqrt(magnitude2()); }

	Vector2D normalize() const {
		T mag = magnitude();
		return mag == 0 ? Vector2D() : Vector2D(x/mag, y/mag);
	}

	// See if the vector is "inside" vectors a and b.
	// Considers vectors parallel with a or b to be inside.
	bool isInside(const Vector2D& a, const Vector2D& b) const {
		return (a.cross(*this) * a.cross(b) >= 0) && (b.cross(*this) * b.cross(a) >= 0);
	}

	template <typename U>
	Vector2D<U> convert(U (*conversionFunction)(T)) const {
		return Vector2D<U> ( conversionFunction(x), conversionFunction(y));
	}
};

template <typename T> inline Vector2D<T> operator*(T t, const Vector2D<T>& v) { return v * t; }

#endif // _VECTOR2D_H