#include "Vector3D.h"
#include <cmath>

Vector3D::Vector3D(): x(0.0), y(0.0), z(0.0)
{
}

Vector3D::Vector3D(float _x, float _y, float _z): x(_x), y(_y), z(_z)
{
}

float Vector3D::getX()
{
	return x;
}

float Vector3D::getY()
{
	return y;
}

float Vector3D::getZ()
{
	return z;
}

void Vector3D::setX(float _x)
{
	x = _x;
}

void Vector3D::setY(float _y)
{
	y = _y;
}

void Vector3D::setZ(float _z)
{
	z = _z;
}

float Vector3D::operator*(const Vector3D vector) //producto escalar
{
	return ((x * vector.x) + (y * vector.y) + (z * vector.z));
}

Vector3D Vector3D::operator*(float k) //producto por un escalar
{
	return Vector3D(k * x, k * y, k * z);
}

Vector3D Vector3D::operator+(const Vector3D vector)
{
	return Vector3D(x + vector.x, y + vector.y, z + vector.z);
}

Vector3D Vector3D::operator-(const Vector3D vector)
{
	return Vector3D(x - vector.x, y - vector.y, z - vector.z);
}

bool Vector3D::operator==(const Vector3D vector)
{
	return  x == vector.x && y == vector.y && z == vector.z;
}

float Vector3D::modulo()
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

void Vector3D::normalized()
{
	//calcular modulo
	float modOriginal = modulo();
	x /= modOriginal;
	y /= modOriginal;
	z /= modOriginal;
}
