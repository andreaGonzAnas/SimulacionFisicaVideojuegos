#pragma once
class Vector3D
{
private:
	float x, y, z;
public:
	//Constructoras
	Vector3D();
	Vector3D(float _x, float _y, float _z);

	//Get
	float getX();
	float getY();
	float getZ();

	void setX(float x);
	void setY(float y);
	void setZ(float z);

	//Operaciones
	float operator*(const Vector3D x); //producto escalar
	Vector3D operator*(float k); //producto por un escalar
	Vector3D operator+(const Vector3D x); //suma
	Vector3D operator-(const Vector3D x); //resta
	bool operator==(const Vector3D x); //igualdad
	float modulo(); //calcular modulo
	void normalized(); //normaliza el vector
};

