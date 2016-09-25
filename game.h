#include <cpplib/cpplib#gsock>
#include <cpplib/cpplib#time>
#include <cpplib/cpplib#gcolor>
#include <string>
#include "cppimport.h"
#include "keybox.h"
#include "mapbox.h"
#include "base64.h"

#ifdef __C4DROID__
#define _HARDWARE_DISK_ADDRESS_HC_SYS "/sdcard/HC_SYS"
#define _HARDWARE_DISK_ADDRESS_SDCARD "/sdcard"
#define _HARDWARE_DISK_ADDRESS_DATA_APP "/data/data/com.hctech.smallworld"
#define _CROSSPLAT_DISK_ADDRESS_SEPERATOR "/"
#else
#define _HARDWARE_DISK_ADDRESS_HC_SYS "D:\\HC_SYS"
#define _HARDWARE_DISK_ADDRESS_SDCARD "D:\\HC_SYS\\sdcard"
#define _HARDWARE_DISK_ADDRESS_DATA_APP "D:\\HC_SYS\\appdata\\smallworld"
#define _CROSSPLAT_DISK_ADDRESS_SEPERATOR "\\"
#endif // __C4DROID__


#define GAMEHARBOR_NET_SERVER_DOMAIN "gameharbor.cn"
#define GAMEHARBOR_NET_SERVER_IP "0.0.0.0"
#define GAMEHARBOR_NET_SERVER_PORT 48003
/// Set to TRUE because we don't have domain and IP now.
#define _CONFIG_GAMEHARBOR_NET_DOMAIN_NEED_DNS true


using namespace std;

namespace _cpplib_buildin_namespace
{
    mutex _cpplib_net_gethostbyname_mutex;
}

hostent* gethostbyname_cross_platform(const char* strName)/// Thread-Safe (Locked By Mutex)
{
    _cpplib_buildin_namespace::_cpplib_net_gethostbyname_mutex.lock();
    hostent* ent=gethostbyname(strName);
    _cpplib_buildin_namespace::_cpplib_net_gethostbyname_mutex.unlock();
    return ent;
}

string DNS_Resolve(string HostName)
{
    hostent* ent=gethostbyname_cross_platform(HostName.c_str());
    if(ent==NULL)return "";
    string ip(inet_ntoa(*(struct in_addr*)ent->h_addr));
    return ip;
}


namespace _smallworld_namespace
{
    mutex logmutex;
    FILE* _log_fp;
    s_time logtime;
    class _disk_class
    {
    public:
        string gamedir(){return _gamedir;}
        string sdcard(){return _basicdir;}
        string appdata(){return _internaldir;}
        string sep(){return _sep;}
        void configure()
        {
            _gamedir=_HARDWARE_DISK_ADDRESS_HC_SYS;
            _basicdir=_HARDWARE_DISK_ADDRESS_SDCARD;
            _internaldir=_HARDWARE_DISK_ADDRESS_DATA_APP;
            _sep=_CROSSPLAT_DISK_ADDRESS_SEPERATOR;
        }
    private:/// Public For Setting Functions.
        string _gamedir,_basicdir,_internaldir,_sep;
    }disk;
    struct _user_auth_pack
    {
        char* key;
        int keybuffsz;
    }user;

    bool _status_is_user_login;

    void* puser=(void*)&puser;
}
_smallworld_namespace::_disk_class disk;

#define log(Format,Args...) \
do{\
lock_guard<mutex> guard(_smallworld_namespace::logmutex);\
_smallworld_namespace::logtime=whattime();\
fprintf(_smallworld_namespace::_log_fp,"[%d:%d:%d]",\
        _smallworld_namespace::logtime.hour,\
        _smallworld_namespace::logtime.min,\
        _smallworld_namespace::logtime.sec);\
fprintf(_smallworld_namespace::_log_fp,Format,##Args);\
fprintf(_smallworld_namespace::_log_fp,"\n");\
fflush(_smallworld_namespace::_log_fp);}while(0)

void _OnExit_RealExit()
{
    /// Use CPrint
    cprint(CEXIT);
    /// For Safe
    exit(0);
}

void OnExit()
{
    cprint(color::red,color::black);
    printf("Request To Shutdown This Application.\n");
    /// Shutdown Log System.
    if(_smallworld_namespace::_log_fp!=NULL)
    {
        log("Shutting Down Log System.");
        fclose(_smallworld_namespace::_log_fp);
    }

    /// All Done.
    printf("Shutdown Now.\n");
    cprint();
    _OnExit_RealExit();
}

int LogSystemSetup()
{
    FILE* fp=fopen("smallworld_log.txt","w");
    if(fp==NULL)
    {
        printf("Unable to start log System.\n");
        return -1;
    }
    else
    {
        log("Log System Start.");
        return 0;
    }
}

void StartUpdate()
{

}


/// Reserved
void SelfCheck()
{
    printf("GameHarbor Loader (For SmallWorld)\n");
    printf("Loader Version: v0.1\n");
    printf("Configuring Log System...\n");
    int ret=LogSystemSetup();
    if(ret<0)
    {
        printf("Fatal Error.\n");
        OnExit();
    }
    printf("Checking Updates...\n");
    do
    {
        log("Local Search Has not been implemented. Only Check Install Pack From DiskBasicDIR.");
        log("Start Configuring Disk System.");
        disk.configure();
        log("Detecting Update Package...");
        string tfile=disk.sdcard()+disk.sep()+"smallworld_updatepack.vz";
        FILE* fp=fopen(tfile.c_str(),"r");
        if(fp!=NULL)
        {
            fclose(fp);
            log("Update Pack Found.");
            StartUpdate();
            printf("Please Restart This Game to install Patch.\n");
            OnExit();
        }
        log("Update Not Found.");
    }while(0);
    printf("Done.\n");

    log("Check Done.");
    printf("Loader Check Done.\n");
    _smallworld_namespace::_status_is_user_login=false;
}

void BeforeExit()
{
    log("Start Termination Function.");
    printf("Bye-bye~\n");
    OnExit();
}

char username[1024];
char rawpass[1024];
int Login()
{
    log("Login() Called");
    if(_smallworld_namespace::_status_is_user_login)
    {
        log("Has Logged. Free Memory");
        free(_smallworld_namespace::user.key);
    }
    _smallworld_namespace::user.keybuffsz=1024;
    log("Allocate New Memory");
    _smallworld_namespace::user.key=(char*)malloc(_smallworld_namespace::user.keybuffsz);
    memset(_smallworld_namespace::user.key,0,1024);

    printf("Welcome To GameHarbor!\n");
    printf("Please Complete Your Passport Information\n");
    printf("USER NAME:");
    cprint(CREFRESH);
    gets(username);
    printf("PASS-WORD:");
    cprint(CREFRESH);
    gets(rawpass);

    log("UserName and Pass Got.");
    printf("Connecting to GameHarbor...\n");

    MapBox box;
    box.add("uname",username);
    box.add("pass",rawpass);

    string server_ip;
    if(_CONFIG_GAMEHARBOR_NET_DOMAIN_NEED_DNS)
    {
        log("Only Domain, Need DNS Resolve.");
        server_ip=DNS_Resolve(GAMEHARBOR_NET_SERVER_DOMAIN);
        if(server_ip.size()<3)
        {
            log("Failed to resolve DNS");
            printf("Failed when connecting to GameHarbor.net.\n");
            return -1;
        }
    }
    else
    {
        server_ip=GAMEHARBOR_NET_SERVER_IP;
    }
    log("Server IP is %s",server_ip.c_str());
    sock s;
    int ret=s.connect(server_ip.c_str(),GAMEHARBOR_NET_SERVER_PORT);
    if(ret<0)
    {
        log("Connect Return %d, Failed to connect to %s",ret,server_ip.c_str());
        printf("Failed when connecting to GameHarbor.\n");
        return -2;
    }
    ret=s.send(box);
    log("Authentication sent.");
    if(ret<0)
    {
        log("Error Occur While Sending AUTH. ret=%d",ret);
        printf("Failed when connecting to GameHarbor.\n");
        return -3;
    }
    box.clear();
    ret=s.recv(box);
    if(ret<0)
    {
        log("Error Occur While Receving AUTH. ret=%d",ret);
        printf("Failed when connecting to GameHarbor.\n");
        return -4;
    }

    if(!box.haskey("STATUS"))
    {
        log("Bad Response. No Status Received.");
        printf("Failed when connecting to GameHarbor.\n");
        return -5;
    }

    string uuid_cookie;
    if(box["STATUS"]=="OK")
    {
        if(!box.haskey("UUID"))
        {
            log("Bad Response. No UUID Found.\n");
            printf("Failed when connecting to GameHarbor.\n");
            return -6;
        }
        uuid_cookie=box["UUID"];
    }

    return 0;/// END
}
