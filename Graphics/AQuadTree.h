#pragma once

#include "ACommonIncludes.h"


class AQuadTree
{
public:
	AQuadTree(Ray ray, Vector3 Center, Vector3 Extent);
	~AQuadTree();

	BoundingBox SubDevide(Vector3 Start, Vector3 End);
	bool Line_AABB_1d(float start, float dir, float min, float max, float& enter, float& exit);
	bool Line_AABB(const Vector3& s, const Vector3& e, const BoundingBox& box, Vector3& hitPointEnter, Vector3& hitPointExit);
	static vector<BoundingBox> HitQueue;
private:
	BoundingBox		m_Box;
	Ray				m_Ray;
	unique_ptr<AQuadTree>		m_NW;
	unique_ptr<AQuadTree>		m_NE;
	unique_ptr<AQuadTree>		m_SE;
	unique_ptr<AQuadTree>		m_SW;
	Vector3 hpEnter;
	Vector3 hpExit;
};

