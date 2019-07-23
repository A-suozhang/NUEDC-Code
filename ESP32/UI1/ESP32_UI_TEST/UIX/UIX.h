#ifndef __UIX_H__
#define __UIX_H__

#include "ILI9341TFTDriver.h"
#include "UIXObjects.h"
#include "UIXRenderer.h"
#include "esp_log.h"

#define UIXPANELNUM 5

struct UIXInitConfig{
    unsigned char spihost=3;  //2 HSPI 3 VSPI
    unsigned long spispeed=20000000;
    unsigned char dispdcpin=17;
    unsigned char disprstpin=16;
    unsigned char dispmemorypages=4;
    bool limitframerate=true;
    unsigned int frameinterval=15;
};
struct UIXPanel{
    char* label="Panel";
    DoublyLinkedList<UIXObject> uixobjects;
};

static int uixcurrentpanel=0;
static int uixlastpanel=0;
static int uixuserpanelnum=1;
static struct UIXPanel uixpanels[UIXPANELNUM];
static bool uixpanelchanged=false;
static UIXButton* uixpanelbuttons[UIXPANELNUM];

//input pad members
char uixinputpadtext[100];
char* uixinputpadtextaddr;
UIXDynamicText uixipdisp(uixinputpadtext,COLWHITE,20,25,1,1);
UIXButton* ipbtns[15];
char* ipbtncaps[15]={"7","8","9","4","5","6","1","2","3",".","0","-",",","C","OK"};

void panelbuttonclick(int tag,UIXButton* obj){
    if(uixcurrentpanel<uixuserpanelnum){
        if(uixcurrentpanel!=tag){
            uixpanelchanged=true;
            uixlastpanel=uixcurrentpanel;
            uixcurrentpanel=tag;
        }
        for(int i=0;i<uixuserpanelnum;i++){
            if(uixpanelbuttons[i]->selected)
                uixpanelbuttons[i]->animatei=1;
            uixpanelbuttons[i]->selected=false;
            uixpanelbuttons[i]->redraw=true;
            uixpanelbuttons[i]->col=COLGRAY;
        }
        uixpanelbuttons[tag]->selected=true;
        uixpanelbuttons[tag]->col=COLVSBLUE;
    }
}

//input pad click
void inputpadbtnclick(int tag,UIXButton* btn){
    int inspos=strlen(uixinputpadtext);
    bool insflag=true;
    if(inspos<=12){
        switch(tag){
            case 0: uixinputpadtext[inspos]='7';break;
            case 1: uixinputpadtext[inspos]='8';break;
            case 2: uixinputpadtext[inspos]='9';break;
            case 3: uixinputpadtext[inspos]='4';break;
            case 4: uixinputpadtext[inspos]='5';break;
            case 5: uixinputpadtext[inspos]='6';break;
            case 6: uixinputpadtext[inspos]='1';break;
            case 7: uixinputpadtext[inspos]='2';break;
            case 8: uixinputpadtext[inspos]='3';break;
            case 9: uixinputpadtext[inspos]='.';break;
            case 10: uixinputpadtext[inspos]='0';break;
            case 11: uixinputpadtext[inspos]='-';break;
            case 12: uixinputpadtext[inspos]=',';break;
        }
    }
    switch(tag){
        case 13:
        for(int i=0;i<inspos;i++){
            uixinputpadtext[i]='\0';
        }
        insflag=false;
        break;
        case 14:
        //enter
        strcpy(uixinputpadtextaddr,uixinputpadtext);
        uixcurrentpanel=uixlastpanel;
        uixpanelchanged=true;
        insflag=false;
        break;
    }
    if(insflag) uixinputpadtext[inspos+1]='\0';
    uixipdisp.invalidate();
}
void initializeinputpadbuttons(UIXButton* btns[]){
    for(int row=0;row<5;row++){
        for(int col=0;col<3;col++){
            btns[row*3+col]=new UIXButton(20+col*(53+20),20+53+col*(53+20),82+40*row,82+30+40*row,COLVSBLUE,COLCYAN,ipbtncaps[row*3+col],inputpadbtnclick,row*3+col);
        }
    }
    btns[13]->col=COLVSORANGE;
    btns[13]->colhighlight=COLORANGE;
    btns[14]->col=COLVSORANGE;
    btns[14]->colhighlight=COLORANGE;
    btns[14]->animated=false;
}
void inputpadbegin(){
    initializeinputpadbuttons(ipbtns);
    for(int i=0;i<15;i++){
        uixpanels[UIXPANELNUM-1].uixobjects+=*(ipbtns[i]);
    }
    uixpanels[UIXPANELNUM-1].uixobjects+=uixipdisp;
}

//implementation
extern void inputboxclick(char* inputboxtext){
    uixlastpanel=uixcurrentpanel;
    uixcurrentpanel=UIXPANELNUM-1;    //FIX THIS!!!
    uixpanelchanged=true;
    uixinputpadtextaddr=inputboxtext;
    uixinputpadtext[0]='\0';
    uixipdisp.invalidate();
}

class UIX{
    private:
    DMASPI* spi;
    ILI9341* ilidisp;
    UIXInitConfig* initconfig;
    unsigned long lasttime=0;
    public:

    //temp
    unsigned long testcounter=0;
    bool backgroundupdated=false;

    UIXTouchContext touchcontext;
    UIXDrawContext drawcontext;

    UIXPanel panel[UIXPANELNUM+1];
    DoublyLinkedList<UIXObject> uixobjects;

    UIX(UIXInitConfig* conf){
        initconfig=conf;
    }

    //initialize with default config
    UIX(){
        UIXInitConfig* defaultconf=new UIXInitConfig(); //memory leak but whatever
        initconfig=defaultconf;
    }

    ~UIX(){

    }

    void begin(){
        if(initconfig->spihost==2)
            spi=new DMASPI(2);
        else
            spi=new DMASPI(3);
        ilidisp=new ILI9341(spi,initconfig->dispdcpin,initconfig->disprstpin,initconfig->dispmemorypages);

        spi->begin(0,initconfig->spispeed);
        ilidisp->begin();

        UIXBackground* bg;
        bg=new UIXBackground(COLBLACK);
        uixobjects+=*bg;
        //panel buttons
        int panelbuttonx=0;
        int panelbuttonwidth;
        for(int i=0;i<uixuserpanelnum;i++){
            panelbuttonwidth=strlen(uixpanels[i].label)*8+10;
            uixpanelbuttons[i]=new UIXButton(panelbuttonx,panelbuttonx+panelbuttonwidth-1,290,319,COLGRAY,COLCYAN,uixpanels[i].label,&panelbuttonclick,i,true,false);
            uixobjects+=*(uixpanelbuttons[i]);
            panelbuttonx+=panelbuttonwidth+2;
        }
        uixpanelbuttons[uixcurrentpanel]->selected=true;
        uixpanelbuttons[uixcurrentpanel]->col=COLVSBLUE;
        inputpadbegin();
    }

    bool tick(){
        if(initconfig->limitframerate){
            unsigned long timenow=millis();
            if(timenow>lasttime && timenow-lasttime<(initconfig->frameinterval)){
                return false;
            }
            lasttime=timenow;
        }
        testcounter++;
        ilidisp->flushlastpage();

        touchcontext.lasttouchx=touchcontext.touchx;
        touchcontext.lasttouchy=touchcontext.touchy;
        touchcontext.lasttouchz=touchcontext.touchz;
        touchcontext.lasttouchpressed=touchcontext.touchpressed;
        unsigned short ntouchx,ntouchy;
        ilidisp->readtouchscreen(&ntouchx,&ntouchy,&(touchcontext.touchz),&(touchcontext.touchpressed));
        if((!touchcontext.lasttouchpressed)&&touchcontext.touchpressed){
            touchcontext.touchx=ntouchx;
            touchcontext.touchy=ntouchy;
        }else{
            if(abs(touchcontext.lasttouchx-ntouchx)<10)
                touchcontext.touchx=touchcontext.lasttouchx/2+ntouchx/2;
            else
                touchcontext.touchx=ntouchx;
            if(abs(touchcontext.lasttouchy-ntouchy)<10)
                touchcontext.touchy=touchcontext.lasttouchy/2+ntouchy/2;
            else
                touchcontext.touchy=ntouchy;
        }

        unsigned short* dispmempixel=(unsigned short*)(ilidisp->dispmem);
        drawcontext.dispmem=dispmempixel;

        //check panel update
        if(uixpanelchanged){
            uixpanelchanged=false;
            DoublyLinkedListNode<UIXObject>* uoitpredraw=uixobjects.head;
            while(uoitpredraw!=NULL){
                UIXObject* uobj=uoitpredraw->obj;
                uobj->redraw=true;
                uoitpredraw=uoitpredraw->next;
            }
            uoitpredraw=uixpanels[uixcurrentpanel].uixobjects.head;
            while(uoitpredraw!=NULL){
                UIXObject* uobj=uoitpredraw->obj;
                uobj->redraw=true;
                uoitpredraw=uoitpredraw->next;
            }
        }
        //iterate touch objects
        DoublyLinkedListNode<UIXObject>* uoit=uixobjects.head;
        while(uoit!=NULL){
            UIXObject* uobj=uoit->obj;
            uobj->tick(&touchcontext);
            uoit=uoit->next;
        }
        uoit=uixpanels[uixcurrentpanel].uixobjects.head;
        while(uoit!=NULL){
            UIXObject* uobj=uoit->obj;
            uobj->tick(&touchcontext);
            uoit=uoit->next;
        }

        do{
            int pagenow=ilidisp->currentpage,pageheight=320/(ilidisp->memorypages);
            drawcontext.pagestarty=pagenow*pageheight;
            drawcontext.pageendy=(pagenow+1)*pageheight;
            drawcontext.refreshsp=79;drawcontext.refreshep=0;

            //get refresh area
            DoublyLinkedListNode<UIXObject>* uoitra=uixobjects.head;
            while(uoitra!=NULL){
                UIXObject* uobj=uoitra->obj;
                if(uobj->drawn && uobj->redraw){
                    uobj->updaterefresharea(&(drawcontext.refreshsp),&(drawcontext.refreshep),drawcontext.pagestarty,drawcontext.pageendy);
                }
                uoitra=uoitra->next;
            }
            uoitra=uixpanels[uixcurrentpanel].uixobjects.head;
            while(uoitra!=NULL){
                UIXObject* uobj=uoitra->obj;
                if(uobj->drawn && uobj->redraw){
                    uobj->updaterefresharea(&(drawcontext.refreshsp),&(drawcontext.refreshep),drawcontext.pagestarty,drawcontext.pageendy);
                }
                uoitra=uoitra->next;
            }

            //draw
            DoublyLinkedListNode<UIXObject>* uoitd=uixobjects.head;
            while(uoitd!=NULL){
                UIXObject* uobj=uoitd->obj;
                if(uobj->drawn){
                    uobj->draw(&drawcontext);
                }
                uoitd=uoitd->next;
            }
            uoitd=uixpanels[uixcurrentpanel].uixobjects.head;
            while(uoitd!=NULL){
                UIXObject* uobj=uoitd->obj;
                if(uobj->drawn){
                    uobj->draw(&drawcontext);
                }
                uoitd=uoitd->next;
            }

            if(drawcontext.refreshsp<=drawcontext.refreshep)
                ilidisp->sendpage(drawcontext.refreshsp,drawcontext.refreshep);
        }while(ilidisp->nextpage()); 

        DoublyLinkedListNode<UIXObject>* uoird=uixobjects.head;
        while(uoird!=NULL){
            UIXObject* uobj=uoird->obj;
            uobj->redraw=false;
            uoird=uoird->next;
        }
        uoird=uixpanels[uixcurrentpanel].uixobjects.head;
        while(uoird!=NULL){
            UIXObject* uobj=uoird->obj;
            uobj->redraw=false;
            uoird=uoird->next;
        }

        if(!backgroundupdated){
            backgroundupdated=true;
        }

        return true;
    }
};

#endif
