#ifndef _LINKED_BLOCKS_STRINGPACK_H
#define _LINKED_BLOCKS_STRINGPACK_H

#include <cpplib/cpplib#gsock>

#include <cstdio>
#include <string>
using namespace std;

string makekey(string tag,string value)
{
	string ans;
	ans=tag;
	ans.push_back(':');
	ans.push_back('"');
	for(size_t i=0;i<value.size();i++)
	{
		if(value.at(i)=='\\')
		{
			ans.push_back('\\');
			ans.push_back('\\');
		}
		else if(value.at(i)=='"')
		{
			ans.push_back('\\');
			ans.push_back('"');
		}
		else
		{
			ans.push_back(value.at(i));
		}
	}
	ans.push_back('"');
	ans.push_back(';');

	return ans;
}

string extracttag(string ins)
{
	if(ins.find(":")!=string::npos)
	{
		return ins.substr(0,ins.find(":"));
	}
	else
	{
		return "";
	}
}

string extractvalue(string ins)
{
	string s=ins.substr(ins.find(":")+2);
	if(s.at(s.size()-1)==';') s.pop_back();//;
	if(s.at(s.size()-1)=='"') s.pop_back();//"
	string ans;
	for(size_t i=0;i<s.size();i++)
	{
		if(s.at(i)=='\\')
		{
			if(s.at(i+1)=='\\')
			{
				ans.push_back('\\');
			}
			else if(s.at(i+1)=='"')
			{
				ans.push_back('"');
			}
			i++;
		}
		else
		{
			ans.push_back(s.at(i));
		}
	}
	return ans;
}

class KeyBox
{
public:
    KeyBox()
    {
        p=0;
    }
    int addKey(string key)
    {
        vec.push_back(key);
        return 0;
    }
    string getKey(int id)
    {
        return vec.at(id);
    }
    int size()
    {
        return vec.size();
    }
    int clear()
    {
        vec.clear();
        return 0;
    }
    int nextKey(string& inc)
    {
        if(p>=size())
        {
            return 0;
        }
        inc=getKey(p);
        p++;
        return 1;
    }
    int reset()
    {
        p=0;
        return 0;
    }
    string toString()
    {
        thread_local char buff[64];
        memset(buff,0,64);
        sprintf(buff,"%d",(int)size());
        string s=makekey("KeyBox-Size",buff);
        for(auto& k:vec)
        {
            s+=k;
        }
        return s;
    }
private:
    vector<string> vec;
    int p;
};

KeyBox ParseFromString(string str)
{
    int offset=0;
    size_t ret;
    KeyBox kb;
    while((ret=str.find(";",offset))!=string::npos)
    {
        //printf("Offset %d, ret %d\n",offset,ret);
        string subs=str.substr(offset,ret-offset+1);
        //printf("subs= %s\n",subs.c_str());
        kb.addKey(subs);
        offset=ret+1;
    }
    return kb;
}

template<>
int sock::send(KeyBox& incBox)
{
    string s=incBox.toString();
    int local_sz=s.size();
    int sz=htonl(local_sz);
    int ret=sock::send((const char*)&sz,sizeof(sz));
    if(ret<0) return ret;
    return sock::send(s.c_str(),s.size());
}

template<>
int sock::recv(KeyBox& incBox)
{
    thread_local char buffer[2048];
    /// Commit Or Rollback
    KeyBox tempBox;
    int remote_sz;
    int ret;
    ret=sock::recv((char*)&remote_sz,sizeof(remote_sz));
    if(ret<0) return ret;
    int sz=ntohl(remote_sz);
    int received=0;
    string ans;
    while(received<sz)
    {
        memset(buffer,0,2048);
        ret=sock::recv(buffer,(sz-received>1024?1024:sz-received));
        if(ret<=0) return ret;
        else
        {
            ans+=buffer;
        }
        received+=ret;
    }
    incBox=ParseFromString(ans);
    return sz;
}

#endif /// _LINKED_BLOCKS_STRINGPACK_H
