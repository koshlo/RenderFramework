#pragma once

#include <array>
#include "../Math/Vector.h"

class AABB
{
public:
	AABB(const float* minCoord, const float* maxCoord) :
		_left(minCoord[0]),
		_bottom(minCoord[1]),
		_front(minCoord[2])
	{
		SetRight(maxCoord[0]);
		SetTop(maxCoord[1]);
		SetBack(maxCoord[2]);
	}

	float GetLeft() const { return _left; }
	float GetBottom() const { return _bottom; }
	float GetFront() const { return _front; }
	float GetRight() const { return _left + _width; }
	float GetTop() const { return _bottom + _height; }
	float GetBack() const { return _front + _depth; }

	void SetRight(float right) { _width = right - _left; }
	void SetTop(float top) { _height = top - _bottom; }
	void SetBack(float back) { _depth = back - _front; }

	typedef std::array<vec3, 8> PointArray;
	PointArray GetPoints() const
	{
		PointArray points = {
			vec3(_left, _bottom, _front),
			vec3(_left, _bottom, GetBack()),
			vec3(_left, GetTop(), _front),
			vec3(_left, GetTop(), GetBack()),
			vec3(GetRight(), _bottom, _front),
			vec3(GetRight(), _bottom, GetBack()),
			vec3(GetRight(), GetTop(), _front),
			vec3(GetRight(), GetTop(), GetBack())
		};
		return points;
	}
private:
	float _left, _bottom, _front;
	float _width, _height, _depth;
};

AABB RotateAABB(const AABB& aabb, const mat3& rotation);