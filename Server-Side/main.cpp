#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cpplib/cpplib#gsock>
#include <vector>
#include <mutex>

#include "../MapBox.h"
using namespace std;
#define MAXWIDTH 120
#define MAXHEIGHT 40

inline bool inround(int LeftBound,int& val,int RightBound)
{
	return (LeftBound<=val&&val<=RightBound);
}

#define MAX_UNIT_TYPE 128
int _unit_basic_dmg[MAX_UNIT_TYPE];
int _unit_max_hp[MAX_UNIT_TYPE];
string _unit_default_name[MAX_UNIT_TYPE];

int getUnitBasicDamage(int UnitType)
{
	return _unit_basic_dmg[UnitType];
}
int getUnitMaxHP(int UnitType)
{
	return _unit_max_hp[UnitType];
}
string getUnitDefaultName(int UnitType)
{
	return _unit_default_name[UnitType];
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
    int party;/// -1: Unknown 0:Empty 1:City-Union 2:Independent-Party 3:Non-Government
    int uid;/// -1: Unknown .UID of owner. 0: Natural(Empty)
    int unitid;/// -1: Unknown 0: Non-Unit Else: Unit ID
    int cityid;/// -1: Unknown 0: Non-City Else: City ID
};

mutex lock_worldmap;
blockinfo worldmap[MAXWIDTH][MAXHEIGHT];

/// Positions are sure to be in range.
vector<vector<blockinfo>> getMapInfo_Vec(int LeftUpBlockX,int LeftUpBlockY,int RightDownBlockX,int RightDownBlockY)
{
    lock_worldmap.lock();
    int w=RightDownBlockX-LeftUpBlockX+1;
    int h=RightDownBlockY-LeftUpBlockY+1;
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
    	MapBox box;
    	box.add("Content-Type","MapInfo");
    	box.add("Content-From","Server");
    	char buff[16];
    	memset(buff,0,16);
    	sprintf(buff,"%d",LX);
    	box.add("LX",buff);
    	
    	memset(buff,0,16);
    	sprintf(buff,"%d",LY);
    	box.add("LY",buff);
    	
    	memset(buff,0,16);
    	sprintf(buff,"%d",RX);
    	box.add("RX",buff);
    	
    	memset(buff,0,16);
    	sprintf(buff,"%d",RY);
    	box.add("RY",buff);
    	
    	
    }
public:
    vector<vector<blockinfo>> vec;
    int LX,LY,RX,RY;
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
    cc.vec=getMapInfo_Vec(LeftUpX,LeftUpY,RightDownX,RightDownY);
    cc.LX=LeftUpX;
    cc.LY=LeftUpY;
    cc.RX=RightDownX;
    cc.RY=RightDownY;
    int sz=(cc.RX-cc.LX+1)*(cc.RY-cc.LY+1);
    char* kk=new char[sz];
    memset(kk,0,sz);
    for(int i=0;i<cc.RY-cc.LY+1;i++)
    {
    	for(int j=0;j<cc.RX-cc.LX+1;j++)
    	{
    		if(cc.vec.at(i).at(j).uid==uid|| (getUserByID(uid).party==cc.vec.at(i).at(j).party && cc.vec.at(i).at(j).party!=3) )
    		kk[i*(cc.RX-cc.LX+1)+j]=1;
    		else
    		kk[i*(cc.RX-cc.LX+1)+j]=0;
    	}
    }
    for(int i=0;i<cc.RY-cc.LY+1;i++)
    {
    	for(int j=0;j<cc.RX-cc.LX+1;j++)
    	{
    		bool canbevis=kk[i*(cc.RX-cc.LX+1)+j];
    		//UP
    		if(!canbevis&&i>=1)
    		{
    			if(kk[(i-1)*(cc.RX-cc.LX+1)+j])
    			canbevis=true;
    		}
    		//Down
    		if(!canbevis&&i<=MAXHEIGHT-2)
    		{
    			if(kk[(i+1)*(cc.RX-cc.LX+1)+j])
    			canbevis=true;
    		}
    		//Left
    		if(!canbevis&&j>=1)
    		{
    			if(kk[i*(cc*RX-cc.LX+1)+(j-1)])
    			canbevis=true;
    		}
    		//Right
    		if(!canbevis&&j<=MAXWIDTH-2)
    		{
    			if(kk[i*(cc.RX-cc.LX+1)+(j+1)])
    			canbevis=true;
    		}
    		
    		if(!canbevis)
    		{
    			//cannot be vis
    			cc.vec.at(i).at(j).party=-1;
    			cc.vec.at(i).at(j).uid=-1;
    			cc.vec.at(i).at(j).unitid=-1;
    			cc.vec.at(i).at(j).cityid=-1;
    		}
    	}
    }
    
    
    

    return cc;
}

