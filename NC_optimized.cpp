#include <algorithm>
#include <execution>
//#include <tbb/tbb.h>
#include "NaturalNeighborhood.h"
#include <iostream>


NaturalNeighborhood::NaturalNeighborhood()
{

}
NaturalNeighborhood::~NaturalNeighborhood()
{

}

void NaturalNeighborhood::clear()
{
    m_vecData.clear();
}

bool NaturalNeighborhood::ReadData(std::vector<std::vector<std::string>> dataList)
{

    for (std::vector<std::string> vec : dataList)
    {
        ObjWithCoord instance = { vec[0][0], std::stoi(vec[1]), std::stof(vec[2]), std::stof(vec[3]) };
        m_vecData.push_back(instance);
    }
    return 0;
}

/*
bool NaturalNeighborhood::get_data(const std::string& strFile)
{
    FILE* fp = NULL;
    fopen_s(&fp, strFile.c_str(), "rt");
    if (!fp)
        return false;
    char pBuff[255];
    unsigned short nLine = 0;
    unsigned char vecIndex[4];
    unsigned short nChar = 0, i = 0, index;
    PointData mPointData;
    while (fgets(pBuff, 255, fp) != NULL)
    {
        if (nLine > 0)
        {
            nChar = (unsigned short)strlen(pBuff);
            index = 0;
            for (i = 0; i < nChar; i++)
            {
                if (pBuff[i] == ',')
                {
                    pBuff[i] = 0;
                    vecIndex[index++] = i + 1;
                    if (index == 4)
                        break;
                }
            }
            if (index == 4)
            {
                mPointData.Feature = pBuff[0];
                mPointData.Instance = atoi(pBuff + vecIndex[0]);
                mPointData.LocXY[0] = (float)atof(pBuff + vecIndex[1]);
                mPointData.LocXY[1] = (float)atof(pBuff + vecIndex[2]);
                mPointData.Checkin = atoi(pBuff + vecIndex[3]);
                m_vecData.emplace_back(mPointData);
            }
        }
        nLine++;
    }
    fclose(fp);
    return true;
}*/
unsigned short NaturalNeighborhood::get_Density(const ObjWithCoord& corePoint, ObjWithCoord* aPoint, const std::vector< ObjWithCoord*>& vecTi) const
{
    double dPowDiameter = pow((aPoint->x - corePoint.x), 2) + pow((aPoint->y - corePoint.y), 2);
    double dPowDiameterTmp;
    double xyCenter[2];
    xyCenter[0] = 0.5 * (aPoint->x + corePoint.x);
    xyCenter[1] = 0.5 * (aPoint->y + corePoint.y);
    double dPowRadius = 0.25 * dPowDiameter;
    unsigned short nRet = 0;
    for (size_t i = 0; i < vecTi.size(); i++)
    {
        if (vecTi[i] == aPoint)
            continue;
        // check in circle first
        dPowDiameterTmp = pow((vecTi[i]->x - corePoint.x), 2) + pow((vecTi[i]->y - corePoint.y), 2);
        if (dPowDiameterTmp < dPowDiameter)
        {
            // check in circle second
            dPowDiameterTmp = pow((vecTi[i]->x - xyCenter[0]), 2) + pow((vecTi[i]->y - xyCenter[1]), 2);
            if (dPowDiameterTmp <= dPowRadius)
                nRet++;
        }
        else
            break;
    }
    return nRet;
}
bool NaturalNeighborhood::find_Point(ObjWithCoord* aPoint, const std::vector< ObjWithCoord*>& vecTi) const
{
    for (size_t k = 0; k < vecTi.size(); k++)
    {
        if (vecTi[k] == aPoint)
            return true;
    }
    return false;
}
bool NaturalNeighborhood::Intersec(const std::vector< ObjWithCoord*>& vecTi, const std::vector< ObjWithCoord*>& vecTi2) const
{
    for (size_t k = 0; k < vecTi.size(); k++)
    {
        for (size_t m = 0; m < vecTi2.size(); m++)
            if (vecTi[k] == vecTi2[m])
            {
                return true;
            }
    }
    return false;
}
int NaturalNeighborhood::get_point_loc(ObjWithCoord* aPoint) const
{
    return aPoint - m_vecData.data();
    /*
    for (size_t k = 0; k < m_vecData.size(); k++) {
        if (aPoint == m_vecData.data() + k)
            return k;
    }
    return -1;
    */
}
std::vector < std::vector<ObjWithCoord>> NaturalNeighborhood::direct_connectivity()
{
    size_t numPoint = m_vecData.size();
    std::vector< ObjWithCoord*> vecT(numPoint);
    for (size_t k = 0; k < numPoint; k++)
        vecT[k] = m_vecData.data() + k;
    size_t i, j;
    ObjWithCoord corePoint;

    std::unordered_map<size_t, std::vector< ObjWithCoord*>> mapT(numPoint - 1);
    std::unordered_map<size_t, std::vector< ObjWithCoord*>> mapCC;
    std::unordered_map<size_t, std::vector< ObjWithCoord*>> mapBC;
    std::unordered_map<size_t, std::vector< ObjWithCoord*>> mapPC;
    std::unordered_map<size_t, std::vector< ObjWithCoord*>> mapCS;

    std::vector<size_t> vecIndirect(numPoint);
    std::vector<size_t> vecBreak(numPoint);
    std::vector<unsigned char> vecFlag(numPoint);

    unsigned short iDensity;
    int deltaDensity;
    size_t sizeof_sortObjWithCoord = sizeof(ObjWithCoord*) * (numPoint - 1);
    // step 1
    for (i = 0; i < numPoint; i++)
    {
        corePoint = m_vecData[i];
        std::sort(std::execution::par_unseq, vecT.begin(), vecT.end(), [corePoint](ObjWithCoord* a, ObjWithCoord* b)->bool
            {
                return ((pow((a->x - corePoint.x), 2) + pow((a->y - corePoint.y), 2)) <
                    (pow((b->x - corePoint.x), 2) + pow((b->y - corePoint.y), 2)));
            });
        j = 0;
        std::vector< ObjWithCoord*> vecCC;
        std::vector< ObjWithCoord*> mapTmp12(numPoint);
        ///mapT[i] = vecT;
        //mapT[i].assign(vecT.begin() + 1, vecT.end());
        mapT[i].resize(numPoint - 1);
        std::memcpy(mapT[i].data(), vecT.data() + 1, sizeof_sortObjWithCoord);
        do {
            //iDensity = get_Density(corePoint, vecT[j], vecT);
            iDensity = get_Density(corePoint, mapT[i][j], mapT[i]);
            if (iDensity == 0)
                vecCC.push_back(mapT[i][j]);
            j++;
        } while (iDensity == 0 && j <= mapT[i].size());
        mapCC[i] = vecCC;
        vecIndirect[i] = j - 1;
    }
    // step 2
    for (i = 0; i < numPoint; i++)
    {
        mapBC[i] = mapCC[i];
        corePoint = m_vecData[i];
        for (j = vecIndirect[i]; j < numPoint - 1; j++)
        {
            iDensity = get_Density(corePoint, mapT[i][j], mapT[i]);
            deltaDensity = iDensity - get_Density(corePoint, mapT[i][j - 1], mapT[i]);
            if (deltaDensity >= 0)
                mapBC[i].push_back(mapT[i][j]);
            else
                break;
        }
        //vecBreak[i] = j - 1;
        vecBreak[i] = j;
    }
    // step 3
    for (i = 0; i < numPoint; i++)
    {
        corePoint = m_vecData[i];
        mapPC[i] = mapBC[i];
        for (j = vecBreak[i]; j < numPoint - 1; j++)
        {
            iDensity = get_Density(corePoint, mapT[i][j], mapT[i]);
            deltaDensity = iDensity - get_Density(corePoint, mapT[i][j - 1], mapT[i]);
            if (deltaDensity >= 0)
                mapPC[i].push_back(mapT[i][j]);
            else if (Intersec(mapBC[get_point_loc(mapT[i][j])], mapBC[i]))//else if (find_Point(mapT[i][j], mapBC[i])) 
                mapPC[i].push_back(mapT[i][j]);
            else
                break;
        };
        mapCS[i] = mapPC[i];
    }
    // step 4
    do {
        /*
        for (i = 0; i < numPoint; i++)
        {
            j = 0;
            vecFlag[i] = 0;
            do
            {
                if (j >= mapCS[i].size()) {
                    j++;
                    continue;
                }
                int idx = get_point_loc(mapCS[i][j]);
                if (find_Point(&m_vecData[i], mapCS[idx]));
                    //continue;
                else if (find_Point(mapCS[i][j], mapCC[i])) {
                    if (!Intersec(mapCC[i], mapCS[idx])) {
                        mapCS[i].erase(mapCS[i].begin() + j, mapCS[i].end());
                        vecFlag[i] = 1;
                    }
                }
                else {
                    std::vector< PointData*> mapTmp(mapCS[i].begin(),mapCS[i].begin() + j);
                    if (!Intersec(mapTmp, mapCS[idx])){
                        mapCS[i].erase(mapCS[i].begin() + j, mapCS[i].end());
                        vecFlag[i] = 1;
                    }
                }
                j++;
            } while ((vecFlag[i] == 0) && (j < mapCS[i].size()));

        }*/

        int idx;
        std::vector< ObjWithCoord*> mapTmp(numPoint);
        for (i = 0; i < numPoint; i++)
        {
            vecFlag[i] = 0;
            for (j = 0; j < mapCS[i].size(); j++)
            {
                idx = get_point_loc(mapCS[i][j]);
                if (find_Point(&m_vecData[i], mapCS[idx])) continue;
                if (find_Point(mapCS[i][j], mapCC[i]))
                {
                    if (!Intersec(mapCC[i], mapCS[idx]))
                    {
                        mapCS[i].erase(mapCS[i].begin() + j, mapCS[i].end());
                        vecFlag[i] = 1;
                        break;
                    }
                }
                else
                {
                    //mapTmp.assign(mapCS[i].begin(),mapCS[i].begin() + j);
                    std::memcpy(mapTmp.data(), mapCS[i].data(), j * sizeof(ObjWithCoord*));
                    if (!Intersec(mapTmp, mapCS[idx]))
                    {
                        mapCS[i].erase(mapCS[i].begin() + j, mapCS[i].end());
                        vecFlag[i] = 1;
                        break;
                    }
                }
            }
        }

    } while (std::accumulate(vecFlag.begin(), vecFlag.end(), 0) != 0);
    std::cout << "done";


    std::vector < std::vector<ObjWithCoord>> result;
    std::vector<ObjWithCoord> tmp;
    for (i = 0; i < numPoint; i++)
    {
        tmp.clear();
        for (j = 0; j < mapCS[i].size(); j++)
        {
            tmp.push_back(*mapCS[i][j]);
        }
        result.push_back(tmp);
    }
    return result;
}
/*
bool NaturalNeighborhood::Create(const std::string& strFile)
{
    if (!get_data(strFile))
        return false;
    direct_connectivity();
    return true;
}*/
