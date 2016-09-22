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

#ifndef _kokowin_h
#define _kokowin_h

#include <cpplib/cpplib#gcolor>
#include <string>
#include <vector>
using namespace std;

class Screen;

/** Why Use Marco ?
*   Because we cannot process things like "..." in any way now.
*   But Marco can.
*   So, though ugly, we still have to use it.
*/
#define penprint(Pen,FormatString,args...) Pen.clearbuff();sprintf(Pen.getbuff(),FormatString,##args);Pen.print_deal()

class DrawPen
{
public:
    void cprint(color fcolor,color bcolor)
    {
        /// Right Now, let this pen directly operate it.
        ::cprint(fcolor,bcolor);
    }
    void gotoxy_public(int x,int y) /// This Function Will Change ch and cw
    {
        DrawPen::gotoxy(x,y);
        ch=y;
        cw=x;
    }
    void clear()/// Clear Whole Area and reset ch and cw to 0, reset color to default color
    {
        DrawPen::gotoxy(0,0);
        int w=RightDown.x-LeftUp.x;
        int h=RightDown.y-LeftUp.y;
        memset(buff,' ',w);
        buff[w]=0;
        ::cprint();
        for(int i=0;i<h;i++)
        {
            DrawPen::gotoxy(0,i);
            printf(buff);
        }
        restart();
        DrawPen::gotoxy(0,0);
    }
    int print_deal() /// Deal With Buffer. WARNING: '\t' is not recognized
    {
        /** NOTICE
        In ANSI/GBK, A Chinese character occupy 2 * 8-Bit in Memory and 2 Print-Bit on screen.
                So we can use strlen and Pointer sum/sub to print.
        In UTF-8/UNICODE, A Chinese Character may occupy 3 or more than 3 * 8-Bit in Memory. But it still
                    occupy 2 Print-Bit on screen.
                So we can't just strlen or Pointer sum/sub.
                We may use some other Methods later. Just now, this problem happens only on Linux/Android(C4D).
        */
        int L=strlen(buff);
        int w=RightDown.x-LeftUp.x;
        int h=RightDown.y-LeftUp.y;
        char* p=buff;
        while(p<buff+L&&ch<h)
        {
            char* q=strstr(p,"\n");

            if(q==NULL)
            {
                q=buff+L;
            }
            else if(q==p)
            {
                ch++;
                DrawPen::gotoxy(0,ch);
                cw=0;
                p=q+1;
                continue;
            }

            if(q-p>w-cw)
            {
                char* k=p+w-cw;
                char temp=k[0];
                k[0]=0;
                print_real(p);
                ch++;
                k[0]=temp;
                p=k;
                DrawPen::gotoxy(0,ch);
                cw=0;
                continue;
            }
            else
            {
                char temp=q[0];
                q[0]=0;
                int ret=print_real(p);
                q[0]=temp;
                p=q;
                cw+=ret;
                continue;
            }
        }

        return ch;/// Return How Many Lines This Object Use
    }
    int print_real(const char* RawString) /// Call printf() Directly
    {
        return printf("%s",RawString);
    }
    char* getbuff()
    {
        return buff;
    }
    void setbuffsize(int sz)
    {
        if(buff!=NULL) delete[] buff;
        buff=NULL;
        buffsize=sz;
        buff=new char[buffsize];
        clearbuff();
    }
    void clearbuff()
    {
        memset(buff,0,buffsize);
    }
    void restart()
    {
        ch=0;
        cw=0;
    }

    DrawPen(int LeftUpX,int LeftUpY,int RightDownX,int RightDownY,int BuffSize)
    {
        buff=NULL;
        restart();
        setbuffsize(BuffSize);
        setboundary(LeftUpX,LeftUpY,RightDownX,RightDownY);
    }
protected:
    friend class Screen;
    void setboundary(int LeftUpX,int LeftUpY,int RightDownX,int RightDownY)
    {
        LeftUp.x=LeftUpX;
        LeftUp.y=LeftUpY;
        RightDown.x=RightDownX;
        RightDown.y=RightDownY;
    }


private:
    void gotoxy(int x,int y)
    {
        ::gotoxy(LeftUp.x+x,LeftUp.y+y);
    }


    struct PenPos
    {
        int x,y;
    }LeftUp,RightDown;
    char* buff;
    int buffsize;

    int ch,cw;
};



class DrawableObject
{
public:
    virtual void draw(DrawPen& pen)=0;
    virtual void drawat(DrawPen& pen,int x,int y)
    {
        pen.gotoxy_public(x,y);
        draw(pen);
    }
};

class Screen
{
public:
    Screen() : pen(0,0,0,0,1024)
    {
        pen_has_boundary_flag=false;
    }
    Screen(int LeftUpX,int LeftUpY,int RightDownX,int RightDownY) :
        pen(LeftUpX,LeftUpY,RightDownX,RightDownY,1024)
    {
        pen_has_boundary_flag=true;
    }
    Screen(int LeftUpX,int LeftUpY,int RightDownX,int RightDownY,int BufferSize) :
        pen(LeftUpX,LeftUpY,RightDownX,RightDownY,BufferSize)
    {
        pen_has_boundary_flag=true;
    }

    void display()
    {
        gotoxy(LeftUp.x,LeftUp.y);
        /// Cannot print if Pen is not Ready.
        if(!isPenReady()) return;
        pen.restart();
        for(auto& obj:vec)
        {
            obj->draw(pen);
        }
    }

    bool isPenReady()
    {
        return pen_has_boundary_flag;
    }
    void add(DrawableObject* pObject)
    {
        vec.push_back(pObject);
    }

    void setboundary(int LeftUpX,int LeftUpY,int RightDownX,int RightDownY)
    {
        LeftUp.x=LeftUpX;
        LeftUp.y=LeftUpY;
        RightDown.x=RightDownX;
        RightDown.y=RightDownY;
        adjustPen();
    }

    void adjustPen()
    {
        pen.setboundary(LeftUp.x,LeftUp.y,RightDown.x,RightDown.y);
        pen_has_boundary_flag=true;
    }
private:
    vector<DrawableObject*> vec;
    struct Pos
    {
        int x,y;
    }LeftUp,RightDown;/// Rectangle of drawable area (|`` _|)
    DrawPen pen;
    bool pen_has_boundary_flag;
};

class ScreenStack
{
public:
    int add(Screen* scr,int Important=0)
    {
        return 0;
    }
    void display()
    {
        for(auto& ss:vec)
        {
            ss->display();
        }
    }
private:
    vector<Screen*> vec;
};



class CommandObject : public DrawableObject
{
public:
    virtual void execute(DrawPen& pen)=0;
    virtual void draw(DrawPen& pen) override
    {
        execute(pen);
    }
    virtual void drawat(DrawPen& pen,int x,int y) override
    {
        execute(pen);
    }
};
/**************************************Drawable Objects**********************************************/

class Text : public DrawableObject
{
public:
    Text()
    {
        str="";
        f=color::white;
        b=color::black;
    }
    Text(string in_str,color in_front,color in_back)
    {
        str=in_str;
        f=in_front;
        b=in_back;
    }
    virtual void draw(DrawPen& pen) override
    {
        pen.cprint(f,b);
        penprint(pen,"%s",str.c_str());
    }
    void setcolor(color front,color back)
    {
        f=front;
        b=back;
    }
    void setstr(string inc)
    {
        str=inc;
    }
private:
    color f,b;
    string str;
};
class Choice : public DrawableObject
{
public:
    Choice()
    {
        active_flag=false;/// Inactive By default
    }
    virtual void draw(DrawPen& pen) override
    {
        if(active_flag) t_active.draw(pen);
        else t_inactive.draw(pen);
    }
    void setActiveText(Text t)
    {
        t_active=t;
    }
    void setInactiveText(Text t)
    {
        t_inactive=t;
    }
    void activate()
    {
        active_flag=true;
    }
    void inactivate()
    {
        active_flag=false;
    }
private:
    Text t_active,t_inactive;
    bool active_flag;
};

/**************************************Command Objects**********************************************/
class MoveCursorCMD : public CommandObject
{
public:
    MoveCursorCMD(int movetox,int movetoy)
    {
        x=movetox;
        y=movetoy;
    }
    virtual void execute(DrawPen& pen) override
    {
        pen.gotoxy_public(x,y);
    }
private:
    int x,y;
};
class CleanScreenCMD : public CommandObject
{
public:
    virtual void execute(DrawPen& pen) override
    {
        pen.clear();
    }
};

#endif /// End of _kokowin_h
