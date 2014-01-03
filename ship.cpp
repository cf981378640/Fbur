#include <iostream>
#include <cmath>
#include <algorithm>
#include <cctype>
#include "ship.h"
using namespace std;

//后期：将calculate与get分离开来，构造函数即调用calculate;

ship::ship()
{
    const int numOfZ=10;
    const int numOfZhan=25;
    const int numOfPoint=500;

    vZhan.reserve(numOfZhan);
    vZ.reserve(numOfZ);
    vPoints.reserve(numOfPoint);
    vZb.reserve(numOfZ);          //浮心垂向坐标Zb**静水力曲线;
    vBm.reserve(numOfZ);        //横稳心半径Bm**静水力曲线;
    vAw.reserve(numOfZ);          //水线面面积Aw**静水力曲线;
    vTPC.reserve(numOfZ);         //每厘米吃水吨数TPC**静水力曲线;
    vMTC.reserve(numOfZ);         //每厘米纵倾力矩MTC**静水力曲线;
    vBml.reserve(numOfZ);         //纵稳心半径Bml**静水力曲线;
    vXf.reserve(numOfZ);          //漂心纵坐标Xf**静水力曲线;
    vXb.reserve(numOfZ);          //浮心纵坐标Xb**静水力曲线;
    vVolume.reserve(numOfZ);      //型排水体积Volume**静水力曲线;
    vDisp.reserve(numOfZ);        //总排水量Disp**静水力曲线;
    vCb.reserve(numOfZ);          //方形系数Cb**静水力曲线;
    vCp.reserve(numOfZ);          //棱形系数Cp**静水力曲线;
    vCwp.reserve(numOfZ);         //水线面系数Cwp**静水力曲线;
    vCm.reserve(numOfZ);          //中横剖面系数Cm**静水力曲线;

    vIt.reserve(numOfZ);
    vIl.reserve(numOfZ);
    vAs.reserve(numOfZhan);
    vMoyy.reserve(numOfZhan);

    Init();
}

ship::~ship()
{
    //dtor
}

void ship::Init()
{
    vZ.clear();
    vPoints.clear();
    vZb.clear();          //浮心垂向坐标Zb**静水力曲线;
    vBm.clear();        //横稳心半径Bm**静水力曲线;
    vAw.clear();          //水线面面积Aw**静水力曲线;
    vTPC.clear();         //每厘米吃水吨数TPC**静水力曲线;
    vMTC.clear();         //每厘米纵倾力矩MTC**静水力曲线;
    vBml.clear();         //纵稳心半径Bml**静水力曲线;
    vXf.clear();          //漂心纵坐标Xf**静水力曲线;
    vXb.clear();          //浮心纵坐标Xb**静水力曲线;
    vVolume.clear();      //型排水体积Volume**静水力曲线;
    vDisp.clear();        //总排水量Disp**静水力曲线;
    vCb.clear();          //方形系数Cb**静水力曲线;
    vCp.clear();          //棱形系数Cp**静水力曲线;
    vCwp.clear();         //水线面系数Cwp**静水力曲线;
    vCm.clear();          //中横剖面系数Cm**静水力曲线;


    vIt.clear();                 //水线面的横向惯性矩--用于计算横稳心;
    vIl.clear();                 //水线面的纵向惯性矩（对于中站横轴）--用于计算Ilf;

    vAs.clear();
    vMoyy.clear();

    mZ=0;
    Lpp=0,L0=0,d0=0,B=0,deltaL=0;
    nX=0;

    cx=m=n=ma=cs=cw=mw=nw=0;
    Xm=Lpp=Loa=B=Bw=deltaL=omega=f=0.0;
}

bool ship::importPrinDim()
{
    if(!inFile.eof())
    {
        iOutput<<"读入船舶主尺度及相关参数...\n";
        //此处应对数据做检查
        if(!(inFile>>cx>>m>>n>>ma>>cs>>cw>>mw>>nw)||!(inFile>>Xm>>Lpp>>Loa>>B>>Bw>>deltaL>>omega>>f>>O))
            throw importError();
    }
    return true;
}
bool ship::importOffsets()
{
    sPoint pPoint;
    string biaoshi;
    if(!(inFile>>biaoshi))throw importError();

    char ch;
    while (inFile>>ch)
    {
        inFile.unget();
        if(!isdigit(ch)&&ch!='-')break;

        inFile>>pPoint.x;
        if(pPoint.x==-1)
            for(int j=0;inFile.peek()!='\n';j++)
            {
                //去空格及'\t'
                inFile.get(ch);
                while(ch==' '||ch=='\t')
                {
                    inFile.get(ch);
                }
                if(ch=='\n')break;
                else inFile.putback(ch);
                //去空格完成

                double temp=-1;
                inFile>>temp;
                vZ.push_back(temp);
            }
        else
        {
            vZhan.push_back(pPoint.x);
            for(int j=0;inFile.peek()!='\n';j++)
            {
                //去空格及'\t'
                inFile.get(ch);
                while(ch==' '||ch=='\t')
                {
                    inFile.get(ch);
                }
                if(ch=='\n')break;
                else inFile.putback(ch);


                double temp=-1;
                inFile>>temp;
                if(temp>=0)
                {
                    pPoint.y=temp;
                    pPoint.z=vZ[j];
                    vPoints.push_back(pPoint);
                }
            }
        }
    }
    return true;
}

bool ship::importAddPTrans()
{
    sPoint pPoint;
    char ch;
    string sch;
    if(!(inFile>>sch))throw importError();

    while (inFile>>ch)
    {
        inFile.putback(ch);

        if(!isdigit(ch)&&ch!='-')break;

        inFile>>pPoint.x;

        for(int j=0;inFile.peek()!='\n';j++)
        {
            //去空格及'\t'
            inFile.get(ch);
            while(ch==' '||ch=='\t')
            {
                inFile.get(ch);
            }
            if(ch=='\n')break;
            else inFile.putback(ch);
            //去空格及'\t'完成

            double temp=-1;
            double temp1=-1;

            inFile>>temp>>temp1;
            if(temp>=0)
            {
                pPoint.z=temp;
                pPoint.y=temp1;
                vPoints.push_back(pPoint);
            }
        }
    }
    return true;
}

bool ship::importAddPWplane()
{
    sPoint pPoint;

    char ch;
    string sch;
    double temp=-1;
    double temp1=-1;
    if(!(inFile>>sch))throw importError();
    while (inFile>>ch)
    {
        inFile.putback(ch);

        if(!isdigit(ch)&&ch!='-')break;

        inFile>>pPoint.z;

        for(int j=0;inFile.peek()!='\n';j++)
        {
            //去空格
            inFile.get(ch);
            while(ch==' '||ch=='\t')
            {
                inFile.get(ch);
            }
            if(ch=='\n')break;
            else inFile.putback(ch);
            //去空格

            inFile>>temp>>temp1;
            if(temp!=-1||temp1!=-1)
            {
                pPoint.x=temp/deltaL+Xm;
                pPoint.y=temp1;
                vPoints.push_back(pPoint);
            }
        }
    }
    return true;
}

bool ship::import(string fileName)
{
    inFile.open(fileName.c_str());
    if(!inFile)throw importError();
    Init();
    if(!(importPrinDim()&&importOffsets()&&importAddPTrans()&&importAddPWplane()))
        throw importError();
    inFile.close();

    sort(vPoints.begin(),vPoints.end(),Cmp_by_Xz());
    vPoints.erase( unique( vPoints.begin(), vPoints.end() ), vPoints.end() );

    vector<sPoint>(vPoints).swap(vPoints);
    mZ=*max_element(vZ.begin(),vZ.end());
    nX=2*Xm;
    //    cerr<<"站数："<<nX;
    return true;
}

vector<sPoint> ship::drawXZ(const double &x)
{
    //
    vector<sPoint> v;
    for (sPoint p : vPoints)
        if(p.x==x)v.push_back(p);
    sort(v.begin(),v.end(),Cmp_by_Zy());
    v.erase( unique( v.begin(), v.end() ), v.end());

    return v;
}

vector<sPoint> ship::drawYZ(const sPoint &s)
{
    vector<sPoint> v;
    vector<sPoint> u;
    for (sPoint p : vPoints)
        if(p.y==s.y&&p.z<mZ)
        {
            if(p.x<ma)v.push_back(p);
            else u.push_back(p);
        }
    sort(v.begin(),v.end(),Cmp_by_Xz());
    sort(u.begin(),u.end(),Cmp_by_Xz());
    v.insert(v.end(), u.begin(), u.end());
    return v;
}

vector<sPoint> ship::drawZX(const double &z)
{
    vector<sPoint> v;
    for (sPoint p : vPoints)
        if(p.z==z)v.push_back(p);
    sort(v.begin(),v.end(),Cmp_by_Xz());
    v.erase( unique( v.begin(), v.end() ), v.end() );
    return v;
}

void ship::calculateAw(double zzz)            //改用梯形法（有些站没有交点的问题；2013/12/20）
{
    double Aw=0,Moy=0,It=0,Il=0;
    double xx=-1,yy=-1;

    for(sPoint p: drawZX(zzz))
    {
        p.x=(p.x-Xm)*deltaL;
        if(xx!=-1)
        {
            Aw+=(p.y+yy)*(p.x-xx);
            Moy+=(p.x*p.y+xx*yy)*(p.x-xx);
            It+=(p.y*p.y*p.y+yy*yy*yy)*(p.x-xx);
            Il+=(p.x*p.x*p.y+xx*xx*yy)*(p.x-xx);
        }
        xx=p.x;
        yy=p.y;
    }
    It/=3.0;       //水线面的横向惯性矩

    double Xf=Moy/Aw;
    double Cwp=Aw/Lpp/B;
    double TPC=omega*Aw/100;

    sZValue p;
    p.z=zzz;

    p.value=Aw;
    vAw.push_back(p);

    p.value=Xf;
    vXf.push_back(p);

    p.value=Cwp;
    vCwp.push_back(p);

    p.value=TPC;
    vTPC.push_back(p);

    p.value=It;
    vIt.push_back(p);

    p.value=Il;
    vIl.push_back(p);
}

void ship::calculateAs(double xxx,double zzz)
{
    double As=0;
    double Moyy=0;    //sumMoyy用于画邦戎曲线
    double y=-1;
    double zz=-1;
    for (sPoint p : drawXZ(xxx))
    {
        if(zzz!=-1&&p.z>zzz)break;
        if(zz!=-1)
        {
            As+=(p.y+y)*(p.z-zz);
            Moyy+=(p.z*p.y+zz*y)*(p.z-zz);
        }
        zz=p.z;
        y=p.y;
    }
    sPoint as;
    as.x=xxx;
    as.z=zzz;
    as.y=As;
    vAs.push_back(as);
    sort(vAs.begin(),vAs.end(),Cmp_by_Xz());

    sPoint moyy=as;
    moyy.y=Moyy;
    vMoyy.push_back(moyy);
    sort(vMoyy.begin(),vMoyy.end(),Cmp_by_Xz());

    if(xxx==ma){
        double Cm=As/(y*zzz*2);
        sZValue p;
        p.z=zzz;
        p.value=Cm;
        //    cerr<<"Cm"<<Cm<<","<<endl;
        vCm.push_back(p);
    }
}

void ship::calculateVolume(double zzz)
{
    double Xb=0,Zb=0;
    double Volume=0,Myoz=0,Mxoy=0;
    double oldAw=-1,oldXf=-1,oldZ=-1;           //之后做优化
    //    double oldZhan=-1;
    double Cp=0;
    double Zvolume=0;

    /*
    for(double zhan : vZhan)
    {
        if(oldZhan!=-1)
        {
            Volume+=(oldAs+getAs(zhan,zzz))*(zhan-oldZhan);
        }
    oldZhan=zhan;
    oldAs=getAs(zhan,zzz);
    }
    Volume*=deltaL/2;
    */


    for (double zz : vZ)
    {
        if(zzz!=-1&&zz>zzz)break;
        if(oldZ!=-1)
        {
            Volume+=(oldAw+getAw(zz))*(zz-oldZ);
            Myoz+=(oldXf*oldAw+getXf(zz)*getAw(zz))*(zz-oldZ);
            Mxoy+=(oldZ*oldAw+zz*getAw(zz))*(zz-oldZ);
        }
        oldZ=zz;
        oldAw=getAw(zz);
        oldXf=getXf(zz);
    }

    Volume/=2;
    Myoz/=2;
    Mxoy/=2;

    Zvolume=getAw(zzz)*zzz;

    Cp=Volume/Zvolume;
    Xb=Myoz/Volume;
    Zb=Mxoy/Volume;

    double Cb=Volume/(Lpp*B*zzz);

    double Disp=omega*Volume;

    double Bm=0;
    for(sZValue pp : vIt)           //获取对应的It,用的多的话改成函数；
        if(pp.z==zzz)
            Bm=pp.value/Volume;

    double Bml=0;
    for(sZValue pp : vIl)           //获取对应的It,用的多的话改成函数；
        if(pp.z==zzz)
            Bml=(pp.value-getAw(pp.z)*getXf(pp.z)*getXf(pp.z))/Volume;

    double MTC=Volume*Bml/(100*Lpp);
    sZValue p;
    p.z=zzz;

    p.value=Volume;
    vVolume.push_back(p);

    p.value=Zb;
    vZb.push_back(p);

    p.value=Xb;
    vXb.push_back(p);

    p.value=Disp;
    vDisp.push_back(p);

    p.value=Bm;
    vBm.push_back(p);

    p.value=Bml;
    vBml.push_back(p);

    p.value=MTC;
    vMTC.push_back(p);

    p.value=Cb;
    vCb.push_back(p);

    p.value=Cp;
    vCp.push_back(p);
}


double ship::getAw(double z)           //用常量引用
{
    for(sZValue p : vAw)
        if(p.z==z)return p.value;
    return -1;
}

double ship::getXf(double z)
{
    for(sZValue p : vXf)
        if(p.z==z)return p.value;
    return -1;
}


double ship::getVolume(double z)
{
    for(sZValue p : vVolume)
        if(p.z==z)return p.value;
    return -1;

    /*
    calculateVolume(-1);
    return Volume;
*/
}

void ship::calculate()
{
    ////
    for(double z : vZ)
    {
        calculateAw(z);
        calculateVolume(z);
    }
    for(sPoint p : vPoints)
        calculateAs(p.x,p.z);
}


bool ship::exLinesPlan(const string &fileName)
{
    ofstream outFile(fileName.c_str(),ios_base::out);
    if (outFile.is_open())
    {
        //复制的
        for (int i=0;i<2*Xm+1;i++)
        {
            vector<sPoint>sP=drawXZ(i);
            outFile<<"spline\n";
            for (sPoint pp : sP)
            {
                if(pp.x<ma)pp.y=-pp.y;
                outFile<<pp.y*1000<<","<<(pp.z+4*mZ)*1000<<"\n";
            }
            outFile<<" \n \n \n ";

        }

        for (double z: vZ)
        {
            if(z==0)continue;
            vector<sPoint>sP=drawZX(z);
            outFile<<"spline\n";
            for (sPoint pp : sP)
            {
                outFile<<(pp.x-Xm)*deltaL*1000<<","<<pp.y*1000<<"\n";
            }
            outFile<<"  \n  \n  \n";
        }

        /*
        double y=0;
        for (sPoint p: vPoints)
        {
            if(p.y!=y)
            {
            vector<sPoint>sP=drawYZ(p);
            outFile<<"spline\n";
            for (sPoint pp : sP)
            {
                outFile<<(pp.x-Xm)*deltaL*1000<<","<<(pp.z+2*mZ)*1000<<"\n";
            }
            outFile<<"  \n  \n  \n";
            }
            y=p.y;
        }
*/

        for (int i=0;i<2*Xm+1;i++)
        {
            outFile<<"line\n";
            outFile<<(i-Xm)*deltaL*1000<<","<<2*mZ*1000<<"\n";
            outFile<<(i-Xm)*deltaL*1000<<","<<3.5*mZ*1000<<"\n";
            outFile<<" \n \n \n ";
            outFile<<"line\n";
            outFile<<(i-Xm)*deltaL*1000<<","<<0<<"\n";
            outFile<<(i-Xm)*deltaL*1000<<","<<B*500<<"\n";
            outFile<<" \n \n \n ";
        }

        for (double z: vZ)
        {
            outFile<<"line\n";
            outFile<<-B*500<<","<<(z+4*mZ)*1000<<"\n";
            outFile<<B*500<<","<<(z+4*mZ)*1000<<"\n";
            outFile<<" \n \n \n ";
            outFile<<"line\n";
            outFile<<-Xm*deltaL*1000<<","<<(z+2*mZ)*1000<<"\n";
            outFile<<Xm*deltaL*1000<<","<<(z+2*mZ)*1000<<"\n";
            outFile<<" \n \n \n ";
        }
        outFile<<"line\n";
        outFile<<-B*500<<","<<4*mZ*1000<<"\n";
        outFile<<-B*500<<","<<5*mZ*1000<<"\n";
        outFile<<" \n \n \n ";
        outFile<<"line\n";
        outFile<<0<<","<<4*mZ*1000<<"\n";
        outFile<<0<<","<<5*mZ*1000<<"\n";
        outFile<<" \n \n \n ";
        outFile<<"line\n";
        outFile<<B*500<<","<<4*mZ*1000<<"\n";
        outFile<<B*500<<","<<5*mZ*1000<<"\n";
        outFile<<" \n \n \n ";

        outFile<<"line\n";
        outFile<<-Xm*deltaL*1000<<","<<0<<"\n";
        outFile<<Xm*deltaL*1000<<","<<0<<"\n";
        outFile<<" \n \n \n ";

        outFile<<"line\n";
        outFile<<-Xm*deltaL*1000<<","<<B*500<<"\n";
        outFile<<Xm*deltaL*1000<<","<<B*500<<"\n";
        outFile<<" \n \n \n ";
    }

    outFile<<" \nz \n"<<"a \n ";
    outFile.close();
    return true;
}

bool ship::exHyCurve(const string &fileName)
{
    ofstream outFile(fileName.c_str(),ios_base::out);
    if (outFile.is_open())
    {

        outFile<<"\n-------------------------\n"
                 "型排水体积V\n"
                 "-------------------------\n";
        outFile<<"z,体积\n";
        for(sZValue p: vVolume)
        {
            outFile<<p.z<<","<<p.value<<"\n";
        }

        outFile<<"\n-------------------------\n"
                 "总排水量\n"
                 "-------------------------\n";
        outFile<<"z,排水量\n";
        for(sZValue p: vDisp)
        {
            outFile<<p.z<<","<<p.value<<"\n";
        }

        outFile<<"\n-------------------------\n"
                 "浮心纵向坐标Xb\n"
                 "-------------------------\n";
        outFile<<"z,Xb\n";
        for(sZValue p: vXb)
        {
            outFile<<p.z<<","<<p.value<<"\n";
        }

        outFile<<"\n-------------------------\n"
                 "浮心垂向坐标Zb\n"
                 "-------------------------\n";
        outFile<<"z,Zb\n";
        for(sZValue p: vZb)
        {
            outFile<<p.z<<","<<p.value<<"\n";
        }

        outFile<<"\n-------------------------\n"
                 "水线面面积Aw\n"
                 "-------------------------\n";
        outFile<<"z,Aw\n";
        for(sZValue p: vAw)
        {
            outFile<<p.z<<","<<p.value<<"\n";
        }
        outFile<<"\n-------------------------\n"
                 "漂心Xf\n"
                 "-------------------------\n";
        outFile<<"z,Xf\n";
        for(sZValue p: vXf)
        {
            outFile<<p.z<<","<<p.value<<"\n";
        }
        outFile<<"\n-------------------------\n"
                 "每厘米吃水吨数TPC\n"
                 "-------------------------\n";
        outFile<<"z,TPC\n";
        for(sZValue p: vTPC)
        {
            outFile<<p.z<<","<<p.value<<"\n";
        }
        outFile<<"\n-------------------------\n"
                 "横稳心半径BM\n"
                 "-------------------------\n";
        outFile<<"z,BM\n";
        for(sZValue p: vBm)
        {
            outFile<<p.z<<","<<p.value<<"\n";
        }
        outFile<<"\n-------------------------\n"
                 "纵稳心半径BML\n"
                 "-------------------------\n";
        outFile<<"z,BML\n";
        for(sZValue p: vBml)
        {
            outFile<<p.z<<","<<p.value<<"\n";
        }

        outFile<<"\n-------------------------\n"
                 "每厘米纵倾力矩MTC\n"
                 "-------------------------\n";
        outFile<<"z,MTC\n";
        for(sZValue p: vMTC)
        {
            outFile<<p.z<<","<<p.value<<"\n";
        }

        outFile<<"\n-------------------------\n"
                 "水线面系数Cwp\n"
                 "-------------------------\n";
        outFile<<"z,Cwp\n";
        for(sZValue p: vCwp)
        {
            outFile<<p.z<<","<<p.value<<"\n";
        }

        outFile<<"\n-------------------------\n"
                 "中横剖面系数Cm\n"
                 "-------------------------\n";
        outFile<<"z,Cm\n";
        for(sZValue p: vCm)
        {
            outFile<<p.z<<","<<p.value<<"\n";
        }

        outFile<<"\n-------------------------\n"
                 "方形系数Cb\n"
                 "-------------------------\n";
        outFile<<"z,Cb\n";
        for(sZValue p: vCb)
        {
            outFile<<p.z<<","<<p.value<<"\n";
        }


        outFile<<"\n-------------------------\n"
                 "棱形系数Cp\n"
                 "-------------------------\n";
        outFile<<"z,Cp\n";
        for(sZValue p: vCp)
        {
            outFile<<p.z<<","<<p.value<<"\n";
        }
    }
    return true;
}

bool ship::ExBC(const string &fileName)
{
    ofstream outFile(fileName.c_str(),ios_base::out);
    if (outFile.is_open())
    {
        double x=-1;
        for(sPoint As : vAs)
        {
            if(x!=As.x)
                outFile<<" \nspline\n";
            outFile<<(As.y/4+As.x*8)*100<<","<<As.z*800<<"\n";
            x=As.x;
        }
        for(sPoint moyy : vMoyy)
        {
            if(x!=moyy.x)
                outFile<<" \n \nspline\n";
            outFile<<(moyy.y/4+moyy.x*8)*100<<","<<moyy.z*800<<"\n";
            x=moyy.x;
        }
        outFile<<"zoom\na \n\n";


/*
        for(sPoint As : vAs)
        {
            double x=-1;
            if(x==-1||As.x==x)
            {
                calculateAs(x,p.z);
            }
            x=p.x;
        }
*/

    }
    return true;
}

