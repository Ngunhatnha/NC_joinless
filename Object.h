
#ifndef OBJECT_H
#define OBJECT_H

// With coordinate
struct ObjWithCoord
{
	char feature;
	int instance;
	float x;
	float y;
	
	

	// Compare two objects
	bool operator==(const ObjWithCoord& obj) const;
	bool operator<(const ObjWithCoord& obj) const;

};

struct Interval {
	ObjWithCoord obj;
	float dis;
};

static bool compareIntervally(Interval i1, Interval i2)
{
	return (i1.dis < i2.dis);
}



// No coordinate
struct ObjWithoutCoord
{
	char feature;
	int instance;
	
	

	bool operator==(const ObjWithoutCoord & obj) const;
	bool operator<(const ObjWithoutCoord & obj) const;

};


class FindPoints
{
	ObjWithCoord PPoint;

public:
	FindPoints(const ObjWithCoord& point) : PPoint(point) {}

	bool operator()(const ObjWithCoord& point) const
	{
		return (point.x == PPoint.x && point.y == PPoint.y);
	}
};




#endif