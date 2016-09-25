#KoKo - Usage
>
**KoKo Win Library** is a C/C++ Library.  
Designed and Written by Kiritow.  
Created For Light Window System.  

## Use Parser (Recommended)
### Direct Parser
This Parser will change the console screen while parsing.  
**Method: _void_ parse_basic(_const char*_ source)**  
**Source String Grammar**  
{goto=X,Y} Move Cursor to (X,Y)  
>
In Windows, Console Cursor is (0,0) by default. In C4droid, Console Cursor is (1,1) by default.

{color=yellow,red} Change Color to Yellow(Frontground) and Red(Background)  
>
Colors are defined in CPPLIB-GCOLOR.  
Avaliable colors can be found in method **parse_basic_getcolor**  

{clear=} Clear The Screen
>
Clear The Screen. Cursor may be set to default or not. It depends on how **cprint(CCLEAR)** is implemented.  

**Example**
```
const char* source="{clear=}{goto=0,0}{color=yellow,red}Hello World!\n{color=blue,black}This is another line.\n";
int main()
{
  parse_basic(source);
  return 0;
}
```

### Advanced Parser
This Parser will create a pointer to **class** *Screen*.  
**Method: _Screen*_ parse_str_basic(_const char*_ source)**  
**Source String Grammar**  
{goto=X,Y}  
{color=yellow,red}  
{clear=}  
  
{choice=fcolor,bcolor,xfcolor,xbcolor,same,str}  
  or {choice=fcolor,bcolor,xfcolor,xbcolor,diff,str,xstr}  
>
Generate a *Choice* object.  
fcolor,bcolor : Inactive Color.  
xfcolor,xbcolor : Active Color.  
same/diff : If Strings are same in active and inactive mode.
str : inactive shown string.  
xstr : active shown string.  
**Important**: shown string should be packed with [# and #], like "[#Hello World#]".  

**Example**  
```
const char* source=
"{goto=0,0}{color=white,black}Choose Your Team Now !\n"
"{color=black,blue}Union of Citys{choice=white,black,black,blue,same,[# Make the world peaceful.#]}\n"
"{color=black,red}Party of Independence{choice=white,black,black,red,same,[# Fighting for new independent world.#]}\n"
"{color=black,yellow}Neutrality{choice=white,black,black,yellow,same,[# No political thoughts.#]}\n";

int main()
{
  Screen* p=parse_str_basic(source);
  p->setboundary(0,0,80,15);
  p->display();
  return 0;
}
```

## Use Objects

**class** DrawableObject 
>
**Important**: This is a virtual class and cannot be instantiated. All Drawable Objects are derived from here.  
**virtual** *void* draw(*DrawPen&* pen)
**virtual** *void* drawat(*DrawPen&* pen,*int* x,*int* y)

**class** Text : **public** *DrawableObject*  
>
Text with color information.  

**class** Choice : **public** *DrawableObject*  
>
A Choice. Can be activate and inactivate. Use Text inside.  
**virtual** *void* activate()  
**virtual** *void* inactivate()  


**class** CommandObject : **public** *DrawableObject*  
>
**Important**: This is a virtual class and cannot be instantiated. All Commands(Executable Objects) are derived from here.  
**virtual** *void* execute(*DrawPen&* pen)  
  Derived class should rewrite this method. This method will be executed when **execute** or **draw** or **drawat** called.  

**class** MoveCursorCMD : **public** *CommandObject*  
>
Move Cursor. Call **DrawPen::gotoxy_public** inside.  

**class** CleanScreenCMD : **public** *CommandObject*  
>
Clean the screen. Call **DrawPen::clear** inside.  


**class** DrawPen  
>
DrawPen is a pen which will directly operate on the screen and change the console. **DrawPen::print_real** will call **printf**.  

**class** Screen  
>
A Screen stores many Drawable Objects. You can set display mode in Screen. It use an internal DrawPen to show objects.  

**class** ScreenStack [Unfinished]
>
ScreenStack is a stack that stores screens. Screens are sorted by Screen-Weight. The lower screen-weight, the lower level of the screen. Screen with low level will be displayed firstly and may be covered by later screens.  
ScreenStack will be part of **class** WindowManager in later version.  

