#include <iostream>
#include <map>
#include <vector>
#include "printFunctions.h"
#include "Object.h"
#include <set>


using namespace std;

/**
* @brief: This function print the instance number of each feature.
* @param: totalInstEachFeat: instance number of each feature
* @retval: Nope
*/
void printInstanceNumber(std::map<char, int> totalInstEachFeat)
{
	std::cout << "The instance number of each feature is: " << std::endl;
	for (auto const& inst : totalInstEachFeat)
	{
		std::cout << inst.first << ":" << inst.second << ",";
	}
	std::cout << endl;
}


/**
* @brief: This function print the star neighborhoods.
* @param: SN: the star neighborhoods of each instances.
* @retval: Nope
*/
void printGrid(std::map<std::pair<int, int>, std::vector<ObjWithCoord>> grid)
{
	for (auto const& cell : grid)
	{
		std::cout << "The cell is: " << "(" << cell.first.first << cell.first.second << "): ";
		std::cout << "Instances: ";
		for (auto const& obj : cell.second)
		{
			std::cout << obj.feature << "." << obj.instance << ", ";
		}
		std::cout << endl;
	}

}



/**
* @brief: This function print the star neighborhoods.
* @param: SN: the star neighborhoods of each instances.
* @retval: Nope
*/
void printSN(std::map<ObjWithoutCoord, std::set<ObjWithoutCoord>> SN)
{
	std::cout << "The star neighbor: " << endl;
	for (auto const& inst : SN)
	{
		std::cout << inst.first.feature << "." << inst.first.instance << " : ";
		for (auto const& nei : inst.second)
		{
			std::cout << nei.feature << "." << nei.instance << ", ";
		}
		std::cout << endl;
	}
	std::cout << endl;
}

/**
* @brief: This function print the group star neighborhoods.
* @param: SN: the star neighborhoods of each instances.
* @retval: Nope
*/
void printStarNeighborhood(std::map<char, std::map<ObjWithoutCoord, std::set<ObjWithoutCoord>>> SN)
{
	std::cout << "The star neighborhoods are: " << endl;
	for (auto const& feat : SN)
	{
		std::cout << "Feature: " << feat.first << endl;
		for (auto const& inst : feat.second)
		{
			std::cout << "Instance: " << inst.first.feature << "." << inst.first.instance << ": ";
			for (auto const& neigh : inst.second)
			{
				std::cout << neigh.feature << "." << neigh.instance << ", ";
			}
			std::cout << endl;
		}
		std::cout << endl;
	}
}



/**
* @brief: This function print all candidate patterns.
* @param: PK: a hash map of prevalent patterns
* @retval: Nope
*/
void printCandidates(std::vector<std::vector<char>> Ck)
{
	std::cout << "The candidates are: " << endl;
	for (auto const& pattern : Ck)
	{
		for (auto const& feat : pattern)
		{
			std::cout << feat;
		}
		std::cout << ", ";
	}
	std::cout << endl;
}


/**
* @brief: This function print candidate patterns and their table instances
* @param: SIk: a hash map of candidate pateterns and their table instasnces
* @retval: Nope
*/
void printStarInstance(std::map<std::vector<char>, std::vector<std::vector<std::vector<ObjWithoutCoord>>>>& SIk)
{
	std::cout << "The star instances of candidate patterns: " << endl;
	for (auto const& item : SIk)
	{
		std::cout << "{";
		for (auto const& patt : item.first)
		{
			std::cout << patt;
		}
		std::cout << "}"<< endl;

		
		for (auto const& tbist : item.second)
		{
			for (auto const& oneFeat : tbist)
			{
				for (auto const& inst : oneFeat)
				{
					std::cout << inst.feature <<"."<<inst.instance<< ",";
				}
				std::cout << endl;
			}
			std::cout << endl;
		}
		std::cout << endl;
	}
	std::cout << endl;
}


/**
* @brief: This function print all prevalent patterns.
* @param: PK: a hash map of prevalent patterns
* @retval: Nope
*/
void printPrevalentPattern(std::map<vector<char>, float> Pk)
{
	std::cout << "The prevalent patterns are: " << endl;
	for (auto const& pattern : Pk)
	{
		std::cout << "( ";
		for (auto const& feature : pattern.first)
		{
			std::cout << feature;
		}
		std::cout << " ): ";
		std::cout << pattern.second << endl;
	}
}




/**
* @brief: This function prints patterns by sizes
* @param: sizePats
* @retval: Nope
*/
void printPattbySize(std::map<int, int>& sizePats)
{
	std::cout << "Size	# patterns" << endl;
	for (auto const& item : sizePats)
	{
		std::cout << item.first << " : " << item.second << endl;
	}
	std::cout << endl;
}
