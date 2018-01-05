// Point3D.h: interface for the CPoint3D class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "stdafx.hpp"

struct CPoint3D  
{
public:
	double x, y, z;
	CPoint3D();
	CPoint3D(double x, double y, double z);
	inline CPoint3D& operator +=(const CPoint3D& pt);
	inline CPoint3D& operator -=(const CPoint3D& pt);
	inline CPoint3D& operator *=(double times);
	inline CPoint3D& operator /=(double times);
	inline CPoint3D operator /(double times) const;
	inline double Len() const;
	inline void Normalize();
	bool operator==(const CPoint3D& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}
};

CPoint3D& CPoint3D::operator +=(const CPoint3D& pt)
{
	x += pt.x;
	y += pt.y;
	z += pt.z;
	return *this;
}

CPoint3D& CPoint3D::operator -=(const CPoint3D& pt)
{
	x -= pt.x;
	y -= pt.y;
	z -= pt.z;
	return *this;
}

CPoint3D& CPoint3D::operator *=(double times)
{
	x *= times;
	y *= times;
	z *= times;
	return *this;
}

CPoint3D& CPoint3D::operator /=(double times)
{
	x /= times;
	y /= times;
	z /= times;
	return *this;
}

CPoint3D CPoint3D::operator /(double times) const
{
	return CPoint3D(x / times, y / times, z / times);
}

double CPoint3D::Len() const
{
	return sqrt(x * x + y * y + z * z);
}

void CPoint3D::Normalize()
{
	double len = Len();
	x /= len;
	y /= len;
	z /= len;
}

CPoint3D operator +(const CPoint3D& pt1, const CPoint3D& pt2);
CPoint3D operator -(const CPoint3D& pt1, const CPoint3D& pt2);
CPoint3D operator *(const CPoint3D& pt, double times);
CPoint3D operator *(double times, const CPoint3D& pt);
CPoint3D operator *(const CPoint3D& pt1, const CPoint3D& pt2);
CPoint3D VectorCross(const CPoint3D& pt1, const CPoint3D& pt2, const CPoint3D& pt3);
void VectorCross(const float* u, const float* v, float * n);

double operator ^(const CPoint3D& pt1, const CPoint3D& pt2);
double GetTriangleArea(const CPoint3D& pt1, const CPoint3D& pt2, const CPoint3D& pt3);
double AngleBetween(const CPoint3D& pt1, const CPoint3D& pt2);
double AngleBetween(const CPoint3D& pt1, const CPoint3D& pt2, const CPoint3D& pt3);
float VectorDot(const float* u, const float* v);
float AngleBetween(const float* u, const float* v);



// Point3D.cpp: implementation of the CPoint3D class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPoint3D::CPoint3D()
{
	x = y = z = 0;
}

CPoint3D::CPoint3D(double x, double y, double z) : x(x), y(y), z(z)
{
}

CPoint3D operator +(const CPoint3D& pt1, const CPoint3D& pt2)
{
	return CPoint3D(pt1.x + pt2.x, pt1.y + pt2.y, pt1.z + pt2.z);
}

CPoint3D operator -(const CPoint3D& pt1, const CPoint3D& pt2)
{
	return CPoint3D(pt1.x - pt2.x, pt1.y - pt2.y, pt1.z - pt2.z);
}

CPoint3D operator *(const CPoint3D& pt, double times)
{
	return CPoint3D(pt.x * times, pt.y * times, pt.z * times);
}

CPoint3D operator *(double times, const CPoint3D& pt)
{
	return CPoint3D(pt.x * times, pt.y * times, pt.z * times);
}

CPoint3D operator*(const CPoint3D& pt1, const CPoint3D& pt2)
{
	return CPoint3D(pt1.y * pt2.z - pt1.z * pt2.y,
		pt1.z * pt2.x - pt1.x * pt2.z,
		pt1.x * pt2.y - pt1.y * pt2.x);
}

CPoint3D VectorCross(const CPoint3D& pt1, const CPoint3D& pt2, const CPoint3D& pt3)
{
	return (pt2 - pt1) * (pt3 - pt2);
}

void VectorCross(const float* u, const float* v, float* n)
{
	n[0] = u[1] * v[2] - u[2] * v[1];
	n[1] = u[2] * v[0] - u[0] * v[2];
	n[2] = u[0] * v[1] - u[1] * v[0];
}

double operator ^(const CPoint3D& pt1, const CPoint3D& pt2)
{
	return pt1.x * pt2.x + pt1.y * pt2.y + pt1.z * pt2.z;
}

double GetTriangleArea(const CPoint3D& pt1, const CPoint3D& pt2, const CPoint3D& pt3)
{
	CPoint3D crossProduct = (pt2 - pt1) * (pt3 - pt2);
	return 0.5 * crossProduct.Len();
}

double AngleBetween(const CPoint3D& pt1, const CPoint3D& pt2)
{
	double cosAngle = (pt1 ^ pt2) / pt1.Len() / pt2.Len();
	if (cosAngle >= 1)
	{
		cosAngle = 1;
	}
	else if (cosAngle <= -1)
	{
		cosAngle = -1;
	}
	return acos(cosAngle);
}

double AngleBetween(const CPoint3D& pt1, const CPoint3D& pt2, const CPoint3D& pt3)
{
	CPoint3D u = pt2 - pt1;
	CPoint3D v = pt3 - pt2;
	double cosAngle = (u ^ v) / u.Len() / v.Len();
	if (cosAngle >= 1)
	{
		cosAngle = 1;
	}
	else if (cosAngle <= -1)
	{
		cosAngle = -1;
	}
	return acos(cosAngle);
}

float AngleBetween(const float* u, const float* v)
{
	float lenU = sqrt(u[0] * u[0] + u[1] * u[1] + u[2] * u[2]);
	float lenV = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	float dot = VectorDot(u, v) / lenU / lenV;
	if (dot < -1)
		dot = -1.0;
	if (dot > 1)
		dot = 1.0;
	return acos(dot);
}

float VectorDot(const float* u, const float* v)
{
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}
