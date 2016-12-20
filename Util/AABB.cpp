#include "AABB.h"

AABB RotateAABB(const AABB& aabb, const mat3& rotation)
{
	AABB::PointArray points = aabb.GetPoints();
	for (AABB::PointArray::size_type i = 0; i < points.size(); ++i)
	{
		points[i] = rotation * points[i];
	}
	vec3 nearPoint = points[0];
	vec3 farPoint = points[0];
	for (AABB::PointArray::size_type i = 0; i < points.size(); ++i)
	{
		vec3 curPoint = points[i];
		for (unsigned int j = 0; j < 3; ++j)
		{
			nearPoint[j] = min(curPoint[j], nearPoint[j]);
			farPoint[j] = max(curPoint[j], farPoint[j]);
		}
	}
	return AABB(nearPoint, farPoint);
}