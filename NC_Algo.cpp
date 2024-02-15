#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "Object.h"

#define INF 99999

class NC {
public:
	std::vector<std::vector<Interval>> Distance;
	std::vector<ObjWithCoord> AllPoint;
	std::vector<std::vector<ObjWithCoord>> CoreNeigh;
	std::vector<std::vector<ObjWithCoord>> DensityNeigh;
	//std::vector<std::vector<ObjWithCoord>> ExtendNeigh;
	std::vector<std::vector<ObjWithCoord>> FinalNeigh;
	std::vector<int> Indirect;
	std::vector<int> Break;
	//std::vector<int> PosibleOutlier;
	//std::vector<std::vector<float>> Adjmaxtix;
	//std::vector<int> IsOutlier;

	void Step1() {
		CalDistanceOrder();
		for (int i = 0; i < AllPoint.size(); i++) {
			std::vector<ObjWithCoord> tmp;
			int j = 1;
			do {

				int Dens = calDensity(AllPoint[i], Distance[i][j].obj);
				if (Dens == 0) {
					tmp.push_back(Distance[i][j].obj);
				}
				else {
					Indirect.push_back(j);
					break;
				}

				j++;
			} while (j < Distance[i].size());
			CoreNeigh.push_back(tmp);
		}
		std::cout << "Step1 done";
		Step2();
	}

	void Step2() {
		DensityNeigh = CoreNeigh;
		for (int i = 0; i < AllPoint.size(); i++) {

			int j = Indirect[i];
			int PrevDens = calDensity(AllPoint[i], Distance[i][j - 1].obj);
			do {
				int Dens = calDensity(AllPoint[i], Distance[i][j].obj);
				if (Dens >= PrevDens) {
					DensityNeigh[i].push_back(Distance[i][j].obj);
				}
				else {
					break;
				}

				PrevDens = Dens;
				j++;
			} while (j < Distance[i].size());
			Break.push_back(j);
		}
		std::cout << "Step2 done";
		Indirect.clear();
		Step3();
	}


	void Step3() {
		FinalNeigh = DensityNeigh;

		for (int i = 0; i < AllPoint.size(); i++) {

			int j = Break[i];
			int PrevDens = calDensity(AllPoint[i], Distance[i][j - 1].obj);
			while (j < Distance[i].size())
			{
				int Dens = calDensity(AllPoint[i], Distance[i][j].obj);
				if (Dens >= PrevDens) {
					FinalNeigh[i].push_back(Distance[i][j].obj);
				}
				else if (Intersec(DensityNeigh[i], DensityNeigh[std::find_if(AllPoint.begin(), AllPoint.end(), FindPoints(Distance[i][j].obj)) - AllPoint.begin()])) {
					FinalNeigh[i].push_back(Distance[i][j].obj);
				}
				else {
					break;
				}

				PrevDens = Dens;
				j++;
			}

		}
		//FinalNeigh = ExtendNeigh;
		Distance.clear();
		DensityNeigh.clear();
		Break.clear();
		std::cout << "Step3 done";
		Step4();
	}

	void Step4() {
		int flag = 0;
		int smallFlag ;
		int count = 0;
		do {
			//std::fill(flagI, flagI + 50000, 0);
			flag = 0;
			for (int i = 0; i < AllPoint.size(); i++) {
				int j = 0;
				smallFlag = 0;
				/*
				if (i == 913) {
					std::cout << "vnjkv";
				}*/
				while (smallFlag == 0 && j < FinalNeigh[i].size())
				{
					int idx = std::find_if(AllPoint.begin(), AllPoint.end(), FindPoints(FinalNeigh[i][j])) - AllPoint.begin();

					//auto idx2 = std::find_if(FinalNeigh[idx].begin(), FinalNeigh[idx].end(), FindPoints(AllPoint[i]) );
					//std::cout << idx2->x;
					//std::cout << idx2->y;

					if (std::find_if(FinalNeigh[idx].begin(), FinalNeigh[idx].end(), FindPoints(AllPoint[i])) != FinalNeigh[idx].end()) {
						//continue;
					}
					else if (std::find_if(CoreNeigh[i].begin(), CoreNeigh[i].end(), FindPoints(FinalNeigh[i][j])) != CoreNeigh[i].end()) {
						if (!Intersec(CoreNeigh[i], FinalNeigh[idx])) {
							FinalNeigh[i].erase(FinalNeigh[i].begin() + j, FinalNeigh[i].end());
							smallFlag += 1;
						}
					}
					else {
						std::vector<ObjWithCoord>::const_iterator first = FinalNeigh[i].begin();
						std::vector<ObjWithCoord>::const_iterator last = FinalNeigh[i].begin() + j;
						if (j != 0) last -= 1;

						std::vector<ObjWithCoord> tmp(first, last);
						if (!Intersec(tmp, FinalNeigh[idx])) {
							FinalNeigh[i].erase(FinalNeigh[i].begin() + j, FinalNeigh[i].end());
							smallFlag += 1;
						}
					}

					j++;
				}
				flag += smallFlag;
			}
			count += 1;
			std::cout << count << std::endl;
		} while (flag != 0 );
	}

	/*
	void Outlier() {
		for (int i = 0; i < AllPoint.size(); i++) {

			for (int j = 0; j < CoreNeigh[i].size(); j++) {
				if (std::find_if(CoreNeigh[j].begin(), CoreNeigh[j].end(), FindPoints(AllPoint[i])) != CoreNeigh[j].end()) {
					PosibleOutlier.push_back(i);
					break;
				}
			}

		}
		std::vector<Points> tmp;
		for (int i : PosibleOutlier) {
			tmp.clear();
			Adjmaxtix.clear();
			std::vector<Points>::const_iterator first = ExtendNeigh[i].begin();
			std::vector<Points>::const_iterator last = ExtendNeigh[i].end();
			tmp.assign(first, last);
			tmp.push_back(AllPoint[i]);
			AdjacentMatrix(tmp);
			double max = MST();
			if (max == Distance[i][1].dis) IsOutlier[i] = 1;

			checkFinalNeighbour();
			std::cout << i << "Possible Outlier done!" << std::endl;
		}

	}

	void checkFinalNeighbour() {
		for (int i = 0; i < FinalNeigh.size(); i++) {
			if (FinalNeigh[i].size() == 0)
				IsOutlier[i] = 1;
		}
	}

	void AdjacentMatrix(std::vector<ObjWithCoord> vec) {

		for (int i = 0; i < vec.size(); i++) {
			std::vector<float> tmp2;
			for (int j = 0; j < vec.size(); j++) {
				tmp2.push_back(calDistance(vec[i], vec[j]));
			}
			Adjmaxtix.push_back(tmp2);
		}
	}
	*/
	bool EqualZero(int* arr, int range) {
		for (int i = 0; i < range; i++) {
			if (arr[i] != 0)
				return false;
		}
		return true;
	}

	bool Intersec(std::vector<ObjWithCoord> vector1, std::vector<ObjWithCoord> vector2) {
		for (std::vector<ObjWithCoord>::iterator i = vector1.begin(); i != vector1.end(); ++i)
		{
			if (std::find_if(vector2.begin(), vector2.end(), FindPoints(*i)) != vector2.end())
			{
				return true;
			}
		}
		return false;
	}

	int calDensity(ObjWithCoord point1, ObjWithCoord points2) {
		int Density = 0;
		ObjWithCoord center;
		center.x = (point1.x + points2.x) / 2;
		center.y = (point1.y + points2.y) / 2;

		float radius = calDistance(center, point1);
		for (ObjWithCoord point : AllPoint) {
			if (calDistance(center, point) <= radius)
				Density += 1;
		}
		return Density - 2;
	}


	void CalDistanceOrder() {

		for (ObjWithCoord point1 : AllPoint) {
			std::vector<Interval> tmp;

			for (ObjWithCoord point2 : AllPoint) {
				struct Interval tmpInter;
				tmpInter.obj = point2;
				tmpInter.dis = calDistance(point1, point2);
				tmp.push_back(tmpInter);
			}
			sort(tmp.begin(), tmp.end(), compareIntervally);
			Distance.push_back(tmp);
		}
	}


	inline float calDistance(ObjWithCoord X, ObjWithCoord Y) {
		return sqrt(pow(X.x - Y.x, 2) + pow(X.y - Y.y, 2));
	}

	std::vector<std::vector<ObjWithCoord>> getResult() {
		return FinalNeigh;
	}

	void clear() {
		Distance.clear();
		AllPoint.clear();
		CoreNeigh.clear();
		DensityNeigh.clear();
		//ExtendNeigh.clear();
		FinalNeigh.clear();
		Indirect.clear();
		Break.clear();
	}

	/*
	double MST() {
		int num_edge;            // number of edge
		std::vector < Interval > tmp;
		// mst_vertex - array to track vertices selected for spanning tree
		int mst_vertex[5000];
		int V = Adjmaxtix[0].size();
		// set selected false initially
		memset(mst_vertex, false, sizeof(mst_vertex));

		// set number of edge to 0
		num_edge = 0;

		//let 0th vertex be the first to be selected
		mst_vertex[0] = true;
		int x;            //  row 
		int y;            //  col 
		double max = -100000;
		// print details of MST
		//cout << "The Minimum Spanning Tree as per Prim's Algorithm:" << endl;
		//cout << "Edge" << " : " << "Weight";
		//cout << endl;
		while (num_edge < V - 1) {

			//Prim's algorithm code
			int min = INF;
			x = 0;
			y = 0;

			for (int i = 0; i < V; i++) {
				if (mst_vertex[i]) {
					for (int j = 0; j < V; j++) {
						if (!mst_vertex[j] && Adjmaxtix[i][j]) { // not in mst_vertex and there is an edge
							if (min > Adjmaxtix[i][j]) {
								min = Adjmaxtix[i][j];
								x = i;
								y = j;
							}
						}
					}
				}
			}
			//cout << x << " - " << y << " :  " << G[x][y];
			//cout << endl;
			if (max < Adjmaxtix[x][y])
				max = Adjmaxtix[x][y];
			mst_vertex[y] = true;
			num_edge++;
		}

		return max;
	}
	*/
};
