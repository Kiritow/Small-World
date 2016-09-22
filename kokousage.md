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
"{color=black,yellow}Neutrality{clear=}{choice=white,black,black,yellow,same,[# No political thoughts.#]}\n";

int main()
{
  Screen* p=parse_str_basic(source);
  p->setboundary(0,0,80,15);
  p->display();
  return 0;
}
```
