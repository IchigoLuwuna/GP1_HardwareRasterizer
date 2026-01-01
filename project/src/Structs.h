#ifndef STRUCTS_H
#define STRUCTS_H
#include "ColorRGB.h"

namespace dae
{
struct Vector2;
struct Vector3;
struct Vector4;
struct Vertex;

struct Vector2 final
{
	float x{};
	float y{};

	Vector2() = default;
	Vector2( float _x, float _y );
	Vector2( const Vector2& from, const Vector2& to );

	float Magnitude() const;
	float SqrMagnitude() const;
	float Normalize();
	Vector2 Normalized() const;

	static float Dot( const Vector2& v1, const Vector2& v2 );
	static float Cross( const Vector2& v1, const Vector2& v2 );

	// Member Operators
	Vector2 operator*( float scale ) const;
	Vector2 operator/( float scale ) const;
	Vector2 operator+( const Vector2& v ) const;
	Vector2 operator-( const Vector2& v ) const;
	Vector2 operator-() const;
	Vector2& operator+=( const Vector2& v );
	Vector2& operator-=( const Vector2& v );
	Vector2& operator/=( float scale );
	Vector2& operator*=( float scale );
	float& operator[]( int index );
	float operator[]( int index ) const;

	static const Vector2 UnitX;
	static const Vector2 UnitY;
	static const Vector2 Zero;
};

struct Vector3 final
{
	float x{};
	float y{};
	float z{};

	Vector3() = default;
	Vector3( float _x, float _y, float _z );
	Vector3( const Vector3& from, const Vector3& to );
	Vector3( const Vector4& v );

	float Magnitude() const;
	float SqrMagnitude() const;
	float Normalize();
	Vector3 Normalized() const;

	static float Dot( const Vector3& v1, const Vector3& v2 );
	static Vector3 Cross( const Vector3& v1, const Vector3& v2 );
	static Vector3 Project( const Vector3& v1, const Vector3& v2 );
	static Vector3 Reject( const Vector3& v1, const Vector3& v2 );
	static Vector3 Reflect( const Vector3& v1, const Vector3& v2 );

	Vector4 ToPoint4() const;
	Vector4 ToVector4() const;
	Vector2 GetXY() const;

	// Member Operators
	Vector3 operator*( float scale ) const;
	Vector3 operator/( float scale ) const;
	Vector3 operator+( const Vector3& v ) const;
	Vector3 operator-( const Vector3& v ) const;
	Vector3 operator-() const;
	Vector3& operator+=( const Vector3& v );
	Vector3& operator-=( const Vector3& v );
	Vector3& operator/=( float scale );
	Vector3& operator*=( float scale );
	float& operator[]( int index );
	float operator[]( int index ) const;
	bool operator==( const Vector3& v ) const;

	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
	static const Vector3 Zero;
};

struct Vector4 final
{
	float x;
	float y;
	float z;
	float w;

	Vector4() = default;
	Vector4( float _x, float _y, float _z, float _w );
	Vector4( const Vector3& v, float _w );

	float Magnitude() const;
	float SqrMagnitude() const;
	float Normalize();
	Vector4 Normalized() const;

	Vector2 GetXY() const;
	Vector3 GetXYZ() const;

	static float Dot( const Vector4& v1, const Vector4& v2 );

	// operator overloading
	Vector4 operator*( float scale ) const;
	Vector4 operator+( const Vector4& v ) const;
	Vector4 operator-( const Vector4& v ) const;
	Vector4& operator+=( const Vector4& v );
	float& operator[]( int index );
	float operator[]( int index ) const;
	bool operator==( const Vector4& v ) const;
};

struct Vertex final
{
	Vector3 position{};
	ColorRGB color{};
	Vector2 UV{};
};

// Global Operators
inline Vector2 operator*( float scale, const Vector2& v )
{
	return { v.x * scale, v.y * scale };
}
inline Vector3 operator*( float scale, const Vector3& v )
{
	return { v.x * scale, v.y * scale, v.z * scale };
}
} // namespace dae
#endif
