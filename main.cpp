/** Small World Project
*   GameHarbor : Small World Project
*   HC Game Center
*   This time we gonna to make real game!
*/

#include "game.h"
using namespace std;

import("SmallWorldMain.dll",_main_DLL);
getfuncin(_main_DLL,int,GameStart,void*);
int main()
{
    SelfCheck();
    /// main Check
    if(GameStart==NULL)
    {
        log("Unable to load DLL. Abort.");
        printf("Sorry, We Cannot Load Main Game.\n"
               "Please Contact Us For More Information.\n");
        OnExit();
    }
    while(Login()<0);
    int ret;
    while(1)
    {
        ret=GameStart(_smallworld_namespace::puser);
        if(ret==4) while(Login()<0);
        else break;
    }
    BeforeExit();
    return 0;
}
