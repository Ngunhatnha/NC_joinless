#include <vector> 
#include <string>
#include "Object.h"
#include <unordered_map>
struct PointData
{
	unsigned char Feature;
	unsigned short Instance;
	float LocXY[2];
	unsigned short Checkin;
};

class NaturalNeighborhood
{
public:
	NaturalNeighborhood();
	~NaturalNeighborhood();
	//bool Create(const std::string& strFile);
	bool ReadData(const std::vector<std::vector<std::string>> dataList);
	std::vector < std::vector<ObjWithCoord>> direct_connectivity();
	std::vector<  ObjWithCoord> m_vecData;
	void clear();
private:
	bool find_Point(ObjWithCoord* aPoint, const std::vector<  ObjWithCoord*>& vecTi) const;
	bool Intersec(const std::vector<  ObjWithCoord*>& vecTi, const std::vector<  ObjWithCoord*>& vecTi2) const;
	int get_point_loc(ObjWithCoord* aPoint) const;
	//bool get_data(const std::string& strFile);
	
	unsigned short get_Density(const  ObjWithCoord& corePoint, ObjWithCoord* aPoint, const std::vector<  ObjWithCoord*>& vecTi) const;
	
};

#pragma once
