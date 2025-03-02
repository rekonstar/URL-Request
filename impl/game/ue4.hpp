#include <Windows.h>
#include <iostream>
#include <d3d9.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "../globals.hpp"

class fvector2d {
public:
	double x;
	double y;

	// Standardkonstruktor
	fvector2d() : x(0), y(0) {}

	// Konstruktor, der zwei double-Werte akzeptiert
	fvector2d(double xValue, double yValue) : x(xValue), y(yValue) {}

	inline bool is_Zero() {
		return (x == 0) && (y == 0);
	}

	inline bool in_screen() {
		return (x >= 0 && x <= globals::screen_width && y >= 0 && y <= globals::screen_height);
	}
};

class fvector
{
public:
	fvector() : x(0.f), y(0.f), z(0.f)
	{

	}

	fvector(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
	{

	}
	~fvector()
	{

	}

	double x;
	double y;
	double z;

	inline double Dot(fvector v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline double Distance(fvector v)
	{
		return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	inline double Length()
	{
		return sqrt(x * x + y * y + z * z);
	}

	fvector operator+(fvector v)
	{
		return fvector(x + v.x, y + v.y, z + v.z);
	}

	fvector operator-(fvector v)
	{
		return fvector(x - v.x, y - v.y, z - v.z);
	}

	fvector operator*(double number) const
	{
		return fvector(x * number, y * number, z * number);
	}
};

struct fplane : fvector
{
	double W = 0;


	fvector ToVector3()
	{
		fvector value;
		value.x = this->x;
		value.y = this->y;
		value.z = this->y;

		return value;
	}
};

struct fquat
{
	double x;
	double y;
	double z;
	double w;
};

struct fmatrix
{
	fplane XPlane;
	fplane YPlane;
	fplane ZPlane;
	fplane WPlane;
};

struct ftransform
{
	fplane  rot;
	fvector translation;
	char    pad[8];
	fvector scale;

	inline D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		double x2 = rot.x + rot.x;
		double y2 = rot.y + rot.y;
		double z2 = rot.z + rot.z;

		double xx2 = rot.x * x2;
		double yy2 = rot.y * y2;
		double zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		double yz2 = rot.y * z2;
		double wx2 = rot.W * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		double xy2 = rot.x * y2;
		double wz2 = rot.W * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		double xz2 = rot.x * z2;
		double wy2 = rot.W * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

static D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}

static D3DMATRIX Matrix(fvector rot, fvector origin = fvector(0, 0, 0))
{
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}

class frotator
{
public:
	frotator() : Pitch(0.f), Yaw(0.f), Roll(0.f)
	{

	}

	frotator(double _Pitch, double _Yaw, double _Roll) : Pitch(_Pitch), Yaw(_Yaw), Roll(_Roll)
	{

	}
	~frotator()
	{

	}

	double Pitch;
	double Yaw;
	double Roll;
	inline frotator get() {
		return frotator(Pitch, Yaw, Roll);
	}
	inline void set(double _Pitch, double _Yaw, double _Roll) {
		Pitch = _Pitch;
		Yaw = _Yaw;
		Roll = _Roll;
	}

	inline frotator Clamp() {
		frotator result = get();
		if (result.Pitch > 180)
			result.Pitch -= 360;
		else if (result.Pitch < -180)
			result.Pitch += 360;
		if (result.Yaw > 180)
			result.Yaw -= 360;
		else if (result.Yaw < -180)
			result.Yaw += 360;
		if (result.Pitch < -89)
			result.Pitch = -89;
		if (result.Pitch > 89)
			result.Pitch = 89;
		while (result.Yaw < -180.0f)
			result.Yaw += 360.0f;
		while (result.Yaw > 180.0f)
			result.Yaw -= 360.0f;

		result.Roll = 0;
		return result;

	}
	double Length() {
		return sqrt(Pitch * Pitch + Yaw * Yaw + Roll * Roll);
	}

	frotator operator+(frotator angB) { return frotator(Pitch + angB.Pitch, Yaw + angB.Yaw, Roll + angB.Roll); }
	frotator operator-(frotator angB) { return frotator(Pitch - angB.Pitch, Yaw - angB.Yaw, Roll - angB.Roll); }
	frotator operator/(float flNum) { return frotator(Pitch / flNum, Yaw / flNum, Roll / flNum); }
	frotator operator*(float flNum) { return frotator(Pitch * flNum, Yaw * flNum, Roll * flNum); }
	bool operator==(frotator angB) { return Pitch == angB.Pitch && Yaw == angB.Yaw && Yaw == angB.Yaw; }
	bool operator!=(frotator angB) { return Pitch != angB.Pitch || Yaw != angB.Yaw || Yaw != angB.Yaw; }

};

static float powf_( float _X, float _Y ) {
	return (_mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(_X), _mm_set_ss(_Y))));
}

static float sqrtf_( float _X ) {
	return (_mm_cvtss_f32(_mm_sqrt_ps(_mm_set_ss(_X))));
}

static double get_cross_distance( double x1, double y1, double x2, double y2 ) {
	return sqrtf(powf((x2 - x1), 2) + powf_((y2 - y1), 2));
}

enum bone : int
{
	base = 0,
	pelvis = 2,
	lthigh1 = 71,
	lthigh2 = 77,
	lthigh3 = 72,
	lcalf = 74,
	lfoot2 = 73,
	lfoot = 86,
	ltoe = 76,
	rthigh1 = 78,
	rthigh2 = 84,
	rthigh3 = 79,
	rcalf = 81,
	rfoot2 = 82,
	rfoot = 87,
	rtoe = 83,
	spine1 = 7,
	spine2 = 5,
	spine3 = 2,
	lcollarbone = 9,
	lupperarm = 35,
	lforearm1 = 36,
	lforearm23 = 10,
	lforearm2 = 34,
	lforearm3 = 33,
	lpalm = 32,
	lhand = 11,
	rcollarbone = 98,
	rupperarm = 64,
	rforearm1 = 65,
	rforearm23 = 39,
	rforearm2 = 63,
	rforearm3 = 62,
	rhand = 40,
	rpalm = 58,
	neck = 67,
	head = 110,
	chest = 37
};

enum EFortBuildingState : int 
{
	Placement = 0,
	EditMode = 1,
	None = 2,
	EFortBuildingState_MAX = 3,
};