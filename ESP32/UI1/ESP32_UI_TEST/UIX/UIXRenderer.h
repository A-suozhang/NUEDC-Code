#ifndef __UIXRENDERER_H__
#define __UIXRENDERER_H__

#include "FontAssets.h"

class UIXRenderer{
    public:

    //r g b 0 to 32
    static unsigned short makecolor(unsigned char r,unsigned g,unsigned b){
        g*=2;
        unsigned char highbyte,lowbyte;
        highbyte=((unsigned char)r<<3)+((unsigned char)g>>3);
        lowbyte=((unsigned char)g<<5)+(unsigned char)b;
        return ((unsigned short)lowbyte<<8)+highbyte;
    }
    //page y range is pagestarty<=pagey<pageendy
    static bool updaterefresharea(int* refreshsp,int* refreshep,int pagestarty,int pageendy,int objecttopy,int objectbottomy){
        bool inpage=false;
        if(!(objecttopy>pageendy || objectbottomy<pagestarty)){
            inpage=true;
            int objecttopry=objecttopy-pagestarty,objectbottomry=objectbottomy-pagestarty;
            if(objecttopry<0)objecttopry=0;
            if(objectbottomry>=pageendy-pagestarty) objectbottomry=(pageendy-pagestarty)-1;
            if(*refreshsp>objecttopry) *refreshsp=objecttopry;
            if(*refreshep<objectbottomry) *refreshep=objectbottomry;
        }
        return inpage;
    }

    static void clear(unsigned short* dispmem,int pagestarty,int pageendy,unsigned short col,int refreshsp=0,int refreshep=319){
        int pagelen=pageendy-pagestarty-1;
        for(int ry=refreshsp;ry<=pagelen && ry<=refreshep;ry++){
            for(int x=0;x<=239;x++){
                dispmem[240*ry+x]=col;
            }
        }
    }

    static void drawrectangle(unsigned short* dispmem,int pagestarty, int pageendy, int l,int r,int t,int b,unsigned short col,int refreshsp=0,int refreshep=319){
        int xsp=l,xep=r,ysp=t-pagestarty,yep=b-pagestarty;
        if(ysp<0) ysp=0;
        if(yep>=pageendy-pagestarty) yep=pageendy-pagestarty-1;
        if(xsp<0) xsp=0;
        if(xep>=240) xep=239;
        if(ysp<refreshsp) ysp=refreshsp;
        for(int ry=ysp; ry<=yep && ry<=refreshep; ry++){
            for(int x=xsp;x<=xep;x++){
                dispmem[240*ry+x]=col;
            }
        }
    }

    static void drawrectangleframe(unsigned short* dispmem,int pagestarty, int pageendy, int l,int r,int t,int b,unsigned short col,int refreshsp=0,int refreshep=319){
        int xsp=l,xep=r,ysp=t-pagestarty,yep=b-pagestarty;
        if(ysp<0) ysp=0;
        if(yep>=pageendy-pagestarty) yep=pageendy-pagestarty-1;
        if(xsp<0) xsp=0;
        if(xep>=240) xep=239;
        if(ysp<refreshsp) ysp=refreshsp;
        for(int ry=ysp; ry<=yep && ry<=refreshep; ry++){
            for(int x=xsp;x<=xep;x++){
                //TODO IMPROVE PERFORMANCE
                if(ry+pagestarty==t || ry+pagestarty==b || x==l || x==r)
                    dispmem[240*ry+x]=col;
            }
        }
    }

    static void drawcharacter(unsigned short* dispmem,int pagestarty,int pageendy,unsigned char chr,int x,int y,unsigned short col,int refreshsp=0,int refreshep=319,unsigned char scale=1,unsigned char font=1){
        if(chr>=32&&chr<=127){
            //possible performance boost here: test y
            unsigned char fontwidth,fontheight;
            switch(font){
                case 0://fontdata_small6x8
                fontwidth=8;fontheight=8;
                break;
                case 1://fontdata_big16x32
                fontwidth=16;fontheight=32;
                break;
                default:
                return;
            }
            for(int gpy=0;gpy<fontheight;gpy++){
                unsigned short chrrow;
                if(font==0)
                    chrrow=fontdata_small6x8[chr-32][gpy];
                else
                    chrrow=fontdata_big16x32[chr-32][gpy];
                for(int gpx=0;gpx<fontheight;gpx++){
                    int gppxy=y+gpy*scale, gppxx=x+gpx*scale;
                    int gppxry=gppxy-pagestarty;
                    //test pixel
                    if(chrrow>>(fontwidth-1-gpx)&0x01){
                        for(int i=0;i<scale;i++){
                            int ry=gppxry+i;
                            if(ry>=0 && ry>=refreshsp && ry<=refreshep && ry<pageendy-pagestarty){
                                for(int j=0;j<scale;j++){
                                    int rx=gppxx+j;
                                    if(rx>=0&&rx<240){
                                        dispmem[240*ry+rx]=col;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    static void drawstring(unsigned short* dispmem,int pagestarty,int pageendy,unsigned char* str,unsigned char len,int x,int y,unsigned short col,int refreshsp=0,int refreshep=319,unsigned char scale=1,unsigned char font=1){
        unsigned char fontspacing;
        switch(font){
            case 0:
            fontspacing=6;
            break;
            case 1:
            fontspacing=16;
            break;
            default:
            return;
        }
        for(int i=0;i<len;i++){
            drawcharacter(dispmem,pagestarty,pageendy,str[i],x+i*scale*fontspacing,y,col,refreshsp,refreshep,scale,font);
        }
    }
};

#endif
