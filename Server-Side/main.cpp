#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cpplib/cpplib#gsock>
#include <vector>
#include <mutex>

#include "../SmallWorld20160916/MapBox.h"
using namespace std;
#define MAXWIDTH 120
#define MAXHEIGHT 40

int getUnitBasicDamage(int UnitType)
{

}
int getUnitMaxHP(int UnitType)
{

}
string getUnitDefaultName(int UnitType)
{

}

struct userinfo
{
    int id;
    int party;
    string name;
};

struct unitinfo
{
    int id;
    int party;
    int uid;
    int type;
    int hp;
    int adv_damage;
    string name;
};

struct cityinfo
{
    int id;
    string name;
    int party;
    int uid;
    int hp;
    int hpmax;
    int level;
    int exp;/// City Level Grow From 1 -> 2 need 1000 exp
};

unitinfo getUintByID(int id)
{

}


cityinfo getCityByID(int id)
{

}

struct blockinfo
{
    int id;
    int party;/// 0:Empty 1:City-Union 2:Independent-Party 3:Non-Government
    int uid;/// UID of owner. 0: Natural
    int unitid;/// 0: Non-Unit Else: Unit ID
    int cityid;/// 0: Non-City Else: City ID
};

mutex lock_worldmap;
blockinfo worldmap[MAXWIDTH][MAXHEIGHT];

/// Positions are sure to be in range.
vector<vector<blockinfo>> getMapInfo_Vec(int uid,int LeftUpBlockX,int LeftUpBlockY,int RightDownBlockX,int RightDownBlockY)
{
    lock_worldmap.lock();
    int w=RightDownBlockX-LeftUpBlockX;
    int h=RightDownBlockY-LeftUpBlockY;
    vector<vector<blockinfo>> vec;
    for(int i=0;i<h;i++)
    {
        vector<blockinfo> tvec;
        for(int j=0;j<w;j++)
        {
            tvec.push_back(worldmap[LeftUpBlockX+j][LeftUpBlockY+i]);
        }
        vec.push_back(tvec);
    }
    lock_worldmap.unlock();

    return vec;
}

class MapInfoClass
{
public:
    MapBox toMapBox()
    {

    }
public:
    vector<vector<blockinfo>> vec;
};

MapInfoClass getMapInfo(int uid,int LX,int LY,int RX,int RY)
{
    int LeftUpX=min(LX,RX);
    int LeftUpY=min(LY,RY);
    int RightDownX=max(LX,RX);
    int RightDownY=max(LY,RY);

    LeftUpX=max(LeftUpX,0);
    LeftUpY=max(LeftUpY,0);
    RightDownX=min(RightDownX,MAXWIDTH-1);
    RightDownY=min(RightDownY,MAXHEIGHT-1);

    MapInfoClass cc;
    cc.vec=getMapInfo_Vec(uid,LeftUpX,LeftUpY,RightDownX,RightDownY);

    return cc;
}

int main()
{
    return 0;
}
