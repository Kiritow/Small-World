/** KoKo Win Library
*   -----------------
*   | |/  _  |/  _  |
*   | |\ |_| |\ |_| |
*   -----------------
*   *KoKo Win Library* is a C/C++ Library
*   Designed and Written by Kiritow.
*   Created For Light Window System.
*   Created : 20160923
*/

/** KoKo Parser System
*   *KoKo Parser System* is a group of methods
*       which can parse SourceString into drawable objects
*       or parse SourceString and draw things on Screen.
*   Created : 20160923
*/

#ifndef _kokoparser_h
#define _kokoparser_h
#include "kokowin.h"
#define parse_basic_getcolor_ifis(Target1,Target2) if(strcmp(Target1,Target2)==0)
color parse_basic_getcolor(const char* str)
{
    parse_basic_getcolor_ifis(str,"black") return color::black;
    else parse_basic_getcolor_ifis(str,"blue") return color::blue;
    else parse_basic_getcolor_ifis(str,"deepgreen") return color::deepgreen;
    else parse_basic_getcolor_ifis(str,"green") return color::green;
    else parse_basic_getcolor_ifis(str,"lightblue") return color::lightblue;
    else parse_basic_getcolor_ifis(str,"purple") return color::purple;
    else parse_basic_getcolor_ifis(str,"red") return color::red;
    else parse_basic_getcolor_ifis(str,"white") return color::white;
    else parse_basic_getcolor_ifis(str,"yellow") return color::yellow;
    else
    {
        return color::white;/// Default Color Is Color
    }
}

void parse_basic(const char* str)
{
    int L=strlen(str);
    char* p=(char*)str;
    while(p<str+L)
    {
        char* q=strstr(p,"{");
        if(q==NULL)
        {
            printf("%s",p);
            break;
        }
        if(q!=p)
        {
            char* content=new char[q-p+32];
            memset(content,0,q-p+32);
            strncpy(content,p,q-p);
            printf("%s",content);
            delete[] content;
        }
        char* qend=strstr(q,"}");
        char* cl=strstr(q,"=");
        char* cmdstr=new char[cl-q];
        unique_ptr<char[]> upa(cmdstr);
        memset(cmdstr,0,cl-q);
        strncpy(cmdstr,q+1,cl-q-1);
        if(strcmp(cmdstr,"color")==0) /// Change Color
        {
            char fcolor[16];
            memset(fcolor,0,16);
            char bcolor[16];
            memset(bcolor,0,16);
            char* comma=strstr(cl,",");
            strncpy(fcolor,cl+1,comma-cl-1);
            strncpy(bcolor,comma+1,qend-comma-1);
            color fi=parse_basic_getcolor(fcolor);
            color bi=parse_basic_getcolor(bcolor);
            cprint(fi,bi);
            p=qend+1;
            continue;
        }
        else if(strcmp(cmdstr,"goto")==0) /// Move Cursor To X,Y
        {
            char fpos[16];
            memset(fpos,0,16);
            char bpos[16];
            memset(bpos,0,16);
            char* comma=strstr(cl,",");
            strncpy(fpos,cl+1,comma-cl-1);
            strncpy(bpos,comma+1,qend-comma-1);
            int fi,bi;
            sscanf(fpos,"%d",&fi);
            sscanf(bpos,"%d",&bi);
            gotoxy(fi,bi);
            p=qend+1;
            continue;
        }
        else if(strcmp(cmdstr,"clear")==0) /// Clear The Screen. (Not Sure
        {
            cprint(CCLEAR);
            p=qend+1;
            continue;
        }
        else /// Unknown Command
        {
            p=qend+1;
        }
    }/// End of loop
}

Screen* parse_str_basic(const char* str)
{
    Screen* pScreen=new Screen;
    int L=strlen(str);
    char* p=(char*)str;
    color cf=color::white;
    color cb=color::black;
    while(p<str+L)
    {
        char* q=strstr(p,"{");
        if(q==NULL)
        {
            Text* ptext=new Text(p,cf,cb);
            pScreen->add(ptext);
            break;
        }
        if(q!=p)
        {
            char* content=new char[q-p+32];
            memset(content,0,q-p+32);
            strncpy(content,p,q-p);
            Text* ptext=new Text(content,cf,cb);
            pScreen->add(ptext);
            delete[] content;
        }
        char* qend=strstr(q,"}");
        char* cl=strstr(q,"=");
        char* cmdstr=new char[cl-q];
        unique_ptr<char[]> upa(cmdstr);
        memset(cmdstr,0,cl-q);
        strncpy(cmdstr,q+1,cl-q-1);
        if(strcmp(cmdstr,"color")==0) /// Change Color
        {
            char fcolor[16];
            memset(fcolor,0,16);
            char bcolor[16];
            memset(bcolor,0,16);
            char* comma=strstr(cl,",");
            strncpy(fcolor,cl+1,comma-cl-1);
            strncpy(bcolor,comma+1,qend-comma-1);
            color fi=parse_basic_getcolor(fcolor);
            color bi=parse_basic_getcolor(bcolor);
            cf=fi;
            cb=bi;
            p=qend+1;
            continue;
        }
        else if(strcmp(cmdstr,"goto")==0) /// Move Cursor To X,Y
        {
            char fpos[16];
            memset(fpos,0,16);
            char bpos[16];
            memset(bpos,0,16);
            char* comma=strstr(cl,",");
            strncpy(fpos,cl+1,comma-cl-1);
            strncpy(bpos,comma+1,qend-comma-1);
            int fi,bi;
            sscanf(fpos,"%d",&fi);
            sscanf(bpos,"%d",&bi);
            MoveCursorCMD* pmove=new MoveCursorCMD(fi,bi);
            pScreen->add(pmove);
            p=qend+1;
            continue;
        }
        else if(strcmp(cmdstr,"clear")==0) /// Clear The Screen.
        {
            CleanScreenCMD* pclean=new CleanScreenCMD;
            pScreen->add(pclean);
            p=qend+1;
            continue;
        }
        else if(strcmp(cmdstr,"choice")==0) /// A Choice
        {
            /// NOTICE: x... is the activated status
            char fcolor[16];
            memset(fcolor,0,16);
            char bcolor[16];
            memset(bcolor,0,16);

            char xfcolor[16];
            memset(xfcolor,0,16);
            char xbcolor[16];
            memset(xbcolor,0,16);

            char tsame[16];
            memset(tsame,0,16);

            char* comma=strstr(cl,",");/// fcolor,bcolor
            char* comma2=strstr(comma+1,",");/// bcolor,xfcolor
            char* comma3=strstr(comma2+1,",");/// xfcolor,xbcolor

            char* comma4=strstr(comma3+1,",");/// xbcolor,same
            char* comma5=strstr(comma4+1,",");/// same,[#text

            char* text_end=strstr(comma5+1,"#]");/// text#]

            strncpy(fcolor,cl+1,comma-cl-1);
            strncpy(bcolor,comma+1,comma2-comma-1);
            strncpy(xfcolor,comma2+1,comma3-comma2-1);
            strncpy(xbcolor,comma3+1,comma4-comma3-1);
            strncpy(tsame,comma4+1,comma5-comma4-1);

            color fi=parse_basic_getcolor(fcolor);
            color bi=parse_basic_getcolor(bcolor);
            color xfi=parse_basic_getcolor(xfcolor);
            color xbi=parse_basic_getcolor(xbcolor);

            char* k_text=new char[text_end-comma5+32];
            unique_ptr<char[]> upt(k_text);
            memset(k_text,0,text_end-comma5+32);
            strncpy(k_text,comma5+3,text_end-comma5-3);
            Text t_inactive(k_text,fi,bi);

            Text t_active;
            if(strcmp(tsame,"same")!=0)/// Not Same
            {
                char* xtext_begin=strstr(text_end+2,"[#");
                char* xtext_end=strstr(xtext_begin+2,"#]");
                char* k_xtext=new char[xtext_end-xtext_begin+32];
                unique_ptr<char[]> uptx(k_xtext);
                memset(k_xtext,0,xtext_end-xtext_begin+32);
                strncpy(k_xtext,xtext_begin+2,xtext_end-xtext_begin-2);
                t_active.setstr(k_xtext);
                t_active.setcolor(xfi,xbi);
            }
            else
            {
                t_active=t_active;
                t_active.setcolor(xfi,xbi);
            }

            Choice* pchoice=new Choice;
            pchoice->setActiveText(t_active);
            pchoice->setInactiveText(t_inactive);

            pScreen->add(pchoice);
            p=qend+1;
            continue;
        }
        else /// Unknown Command
        {
            p=qend+1;
        }
    }/// End of loop

    return pScreen;
}
#endif /// End of _kokoparser_h
