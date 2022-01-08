#include "AQuadTree.h"
vector<BoundingBox> AQuadTree::HitQueue = {};

AQuadTree::AQuadTree(Ray ray, Vector3 Center, Vector3 Extent)
{
	m_Box = BoundingBox(Center, Extent);
	m_Ray = ray;
}

AQuadTree::~AQuadTree()
{

}

BoundingBox AQuadTree::SubDevide(Vector3 Start, Vector3 End)
{
#if 1
	float dist = 0.0f;
	BoundingBox nullBox = BoundingBox(Vector3(0.f), Vector3(0.f));
	if (m_Box.Extents.x <= 0.5f || m_Box.Extents.z <= 0.5f)
	{
		HitQueue.push_back(m_Box);
		return m_Box;
	}

	if (m_Ray.Intersects(m_Box, dist))
	{
		Vector3 halfExtent;
		halfExtent.x = m_Box.Extents.x * 0.5f;
		halfExtent.y = m_Box.Extents.y;
		halfExtent.z = m_Box.Extents.z * 0.5f;

		m_NW.reset(new AQuadTree(m_Ray, m_Box.Center + Vector3(-halfExtent.x, 0.0f, halfExtent.z), halfExtent));
		m_NE.reset(new AQuadTree(m_Ray, m_Box.Center + Vector3(halfExtent.x, 0.0f, halfExtent.z), halfExtent));
		m_SE.reset(new AQuadTree(m_Ray, m_Box.Center + Vector3(halfExtent.x, 0.0f, -halfExtent.z), halfExtent));
		m_SW.reset(new AQuadTree(m_Ray, m_Box.Center + Vector3(-halfExtent.x, 0.0f, -halfExtent.z), halfExtent));

		BoundingBox bNW = m_NW->SubDevide(Start, End);
		BoundingBox bNE = m_NE->SubDevide(Start, End);
		BoundingBox bSE = m_SE->SubDevide(Start, End);
		BoundingBox bSW = m_SW->SubDevide(Start, End);
										 
		if (Vector3(bNW.Extents) != nullBox.Extents && Vector3(bNW.Center) != nullBox.Center) { return bNW; }
		if (Vector3(bNE.Extents) != nullBox.Extents && Vector3(bNE.Center) != nullBox.Center) { return bNE; }
		if (Vector3(bSE.Extents) != nullBox.Extents && Vector3(bSE.Center) != nullBox.Center) { return bSE; }
		if (Vector3(bSW.Extents) != nullBox.Extents && Vector3(bSW.Center) != nullBox.Center) { return bSW; }

	}

#else
	float dist = 0.0f;
	BoundingBox nullBox = BoundingBox(Vector3(0.f), Vector3(0.f));
	if (m_Box.Extents.x <= 0.5f || m_Box.Extents.z <= 0.5f)
	{
		vb.emplace_back(m_Box);
		return m_Box;
	}

	if (Line_AABB( Start, End, m_Box, hpEnter, hpExit))
	{ 
		Vector3 halfExtent;
		halfExtent.x = m_Box.Extents.x * 0.5f;
		halfExtent.y = m_Box.Extents.y;
		halfExtent.z = m_Box.Extents.z * 0.5f;

		m_NW.reset(new AQuadTree(m_Ray, m_Box.Center + Vector3(-halfExtent.x, 0.0f, halfExtent.z), halfExtent));
		m_NE.reset(new AQuadTree(m_Ray, m_Box.Center + Vector3(halfExtent.x, 0.0f, halfExtent.z), halfExtent));
		m_SE.reset(new AQuadTree(m_Ray, m_Box.Center + Vector3(halfExtent.x, 0.0f, -halfExtent.z), halfExtent));
		m_SW.reset(new AQuadTree(m_Ray, m_Box.Center + Vector3(-halfExtent.x, 0.0f, -halfExtent.z), halfExtent));

		BoundingBox bNW = m_NW->SubDevide(hpEnter, hpExit);
		BoundingBox bNE = m_NE->SubDevide(hpEnter, hpExit);
		BoundingBox bSE = m_SE->SubDevide(hpEnter, hpExit);
		BoundingBox bSW = m_SW->SubDevide(hpEnter, hpExit);

		if (Vector3(bNW.Extents) != nullBox.Extents && Vector3(bNW.Center) != nullBox.Center) { return bNW; }
		if (Vector3(bNE.Extents) != nullBox.Extents && Vector3(bNE.Center) != nullBox.Center) { return bNE; }
		if (Vector3(bSE.Extents) != nullBox.Extents && Vector3(bSE.Center) != nullBox.Center) { return bSE; }
		if (Vector3(bSW.Extents) != nullBox.Extents && Vector3(bSW.Center) != nullBox.Center) { return bSW; }
	}
#endif
	return nullBox;
}

//Helper function for Line/AABB test.  Tests collision on a single dimension
//Param:    Start of line, Direction/length of line,
//          Min value of AABB on plane, Max value of AABB on plane
//          Enter and Exit "timestamps" of intersection (OUT)
//Return:   True if there is overlap between Line and AABB, False otherwise
//Note:     Enter and Exit are used for calculations and are only updated in case of intersection
bool AQuadTree::Line_AABB_1d(float start, float dir, float min, float max, float& enter, float& exit)
{
	//If the line segment is more of a point, just check if it's within the segment
	if (fabs(dir) < 1.0E-8)
		return (start >= min && start <= max);

	//Find if the lines overlap
	float   ooDir = 1.0f / dir;
	float   t0 = (min - start) * ooDir;
	float   t1 = (max - start) * ooDir;

	//Make sure t0 is the "first" of the intersections
	if (t0 > t1)
		std::swap(t0, t1);

	//Check if intervals are disjoint
	if (t0 > exit || t1 < enter)
		return false;

	//Reduce interval based on intersection
	if (t0 > enter)
		enter = t0;
	if (t1 < exit)
		exit = t1;

	return true;
}

//Check collision between a line segment and an AABB
//Param:    Start point of line segement, End point of line segment,
//          One corner of AABB, opposite corner of AABB,
//          Location where line hits the AABB (OUT)
//Return:   True if a collision occurs, False otherwise
//Note:     If no collision occurs, OUT param is not reassigned and is not considered useable
bool AQuadTree::Line_AABB(const Vector3& s, const Vector3& e,const BoundingBox& box, Vector3& hitPointEnter, Vector3& hitPointExit)
{
	const Vector3& min = box.Center - box.Extents;
	const Vector3& max = box.Center + box.Extents;

	float       enter = 0.0f;
	float       exit = 1.0f;
	Vector3	    dir = e - s;

	//Check each dimension of Line/AABB for intersection
	if (!Line_AABB_1d(s.x, dir.x, min.x, max.x, enter, exit))
		return false;
	if (!Line_AABB_1d(s.y, dir.y, min.y, max.y, enter, exit))
		return false;
	if (!Line_AABB_1d(s.z, dir.z, min.z, max.z, enter, exit))
		return false;

	//If there is intersection on all dimensions, report that point
	hitPointEnter = s + dir * enter;
	hitPointExit = s + dir * exit;

	return true;
}