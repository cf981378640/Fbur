#include <iostream>
#include <cmath>
#include <algorithm>
#include "ship.h"
using namespace std;

//后期：将calculate与get分离开来，构造函数即调用calculate;

string dbl2str(double dbl){
std::ostringstream strs;
strs << dbl;
std::string str = strs.str();
return str;
}

ship::ship()
{
   Bm=0,MTC=0,Bml=0;

   Moy=0,Mxoz=0;
   Lpp=0,L0=0,d0=0,B=0,deltaL=0;
   nX=0,nZ=0;

   cx=m=n=ma=cs=cw=mw=nw=0;
   Xm=Lpp=Loa=B=Bw=deltaL=omega=f=0.0;

}

ship::~ship()
{
    //dtor
}
void ship::importPrinDim()
{
    if(!inFile.eof())
    {
        iOutput<<"读入船舶主尺度及相关参数...\n";
        //此处应对数据做检查
        inFile>>cx>>m>>n>>ma>>cs>>cw>>mw>>nw;
//        iOutput<<cx<<"\t"<<m<<"\t"<<n<<"\t"<<ma<<"\t"<<cs<<"\t"<<cw<<"\t"<<mw<<"\t"<<nw<<endl;
        inFile>>Xm>>Lpp>>Loa>>B>>Bw>>deltaL>>omega>>f>>O;
//        iOutput<<xm<<"\t"<<lpp<<"\t"<<loa<<"\t"<<b<<"\t"<<bw<<"\t"<<deltaL<<"\t"<<omega<<"\t"<<h<<"\t"<<o<<endl;    
    }
}
void ship::importOffsets()
{
    iOutput<<"读入型值表...\n";
    double temp=-1;
    char ch;
    while (inFile>>ch)
        {
        inFile.putback(ch);
//        iOutput<<"ch:"<<ch<<endl;
        if(ch=='C')break;
        if(temp==0)break;
        inFile>>pPoint.x;

        if(pPoint.x<0)
            for(int j=0;inFile.peek()!='\n';j++)
            {
                inFile>>temp;
                vZ.push_back(temp);
                nZ++;
            }
        else
        {
            for(int j=0;inFile.peek()!='\n';j++)
            {
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
}

void ship::importAddPTrans()
{
    char ch;
    string sch;
    double temp=-1;
    double temp1=-1;
    inFile>>sch;
    if(sch=="CD")iOutput<<"读入横剖面插值点...\n";
while (inFile>>ch)
        {
        inFile.putback(ch);
        if(ch=='C')break;
        if(temp==0)break;
        inFile>>pPoint.x;

            for(int j=0;inFile.peek()!='\n';j++)
            {
                inFile.get(ch);
                while(ch==' '||ch=='\t')
                    {
                        inFile.get(ch);
                    }
                if(ch=='\n')break;
                else inFile.putback(ch);
                inFile>>temp>>temp1;
                if(temp>=0)
                {
                    pPoint.z=temp;
                    pPoint.y=temp1;
                    vPoints.push_back(pPoint);
            }
        }
    }
}

void ship::importAddPWplane()
{
    iOutput<<"读入水线面插值点...\n";
    char ch;
    string sch;
    double temp=-1;
    double temp1=-1;
    inFile>>sch;
while (inFile>>ch)
        {
        inFile.putback(ch);
        if(ch=='C')break;
        if(temp==0)break;
        inFile>>pPoint.z;

            for(int j=0;inFile.peek()!='\n';j++)
            {
                inFile.get(ch);
                while(ch==' '||ch=='\t')
                    {
                        inFile.get(ch);
                    }
                if(ch=='\n')break;
                else inFile.putback(ch);
                inFile>>temp>>temp1;
                if(temp!=-1||temp1!=-1)
                {
                    pPoint.x=temp/deltaL+Xm;
                    pPoint.y=temp1;
                    vPoints.push_back(pPoint);
            }
        }
    }
}

bool ship::import(string fileName)
{
    inFile.open(fileName.c_str());
    importPrinDim();
    string biaoshi;
    if((inFile>>biaoshi))
    {
        importOffsets();
    }
    importAddPTrans();
    importAddPWplane();
    inFile.close();
    nX=2*Xm;
//    cerr<<"站数："<<nX;
    return true;
}


vector<sPoint> ship::drawXZ(double &x)
{
    //
    vector<sPoint> v;
    for (sPoint p : vPoints)
        if(p.x==x)v.push_back(p);
    sort(v.begin(),v.end(),Cmp_by_Y());
    return v;
}

vector<sPoint> ship::drawYZ(sPoint &s)
{
    vector<sPoint> v;
    vector<sPoint> u;
    for (sPoint p : vPoints)
        if(p.y==s.y)
        {
            if(p.x<ma)v.push_back(p);
            else u.push_back(p);
        }
    sort(v.begin(),v.end(),Cmp_by_Z());
    sort(u.begin(),u.end(),Cmp_by_Z());
    v.insert(v.end(), u.begin(), u.end());
    return v;
}

vector<sPoint> ship::drawZX(double &z)
{
    vector<sPoint> v;
    for (sPoint p : vPoints)
        if(p.z==z)v.push_back(p);
    sort(v.begin(),v.end(),Cmp_by_X());
    return v;
}

/*
void ship::drawZb()
{
    cout<<"开始模拟绘制浮心纵向坐标Zb图\n";
    for(double z:vZ)
        dOutput<<dbl2str(z)<<'\t'<<dbl2str(getZb(z))<<'\n';
}

void ship::drawAreaXy()
{
    dOutput<<"开始模拟绘制水线面面积图\n";
    for(double z:vZ)
        dOutput<<z<<'\t'<<getAreaXy(z)<<endl;
}

void ship::drawTPC()
{
    dOutput<<"开始模拟绘制每厘米吃水吨数图\n";
    for(double z:vZ)
        dOutput<<z<<'\t'<<getTPC(z)<<endl;
}

void ship::drawXf()
{
    dOutput<<"开始模拟绘制漂心纵坐标图\n";
    for(double z:vZ)
        dOutput<<z<<'\t'<<getXf(z)<<endl;
}

void ship::drawXb()
{
    dOutput<<"开始模拟绘制浮心纵坐标图\n";
    for(double z:vZ)
        dOutput<<z<<'\t'<<getXb(z)<<endl;
}

void ship::drawVolume()
{
    dOutput<<"开始模拟绘制总排水体积图\n";
    for(double z:vZ)
        dOutput<<z<<'\t'<<getVolume(z)<<endl;
}

void ship::drawDisp()
{
    dOutput<<"开始模拟绘制总排水量图\n";
    for(double z:vZ)
        dOutput<<z<<'\t'<<getDisp(z)<<endl;
}

void ship::drawCb()
{
    dOutput<<"开始模拟绘制方形系数图\n";
    for(double z:vZ)
        dOutput<<z<<'\t'<<getCb(z)<<endl;
}

void ship::drawCp()
{
    dOutput<<"开始模拟绘制棱形系数图\n";
    for(double z:vZ){
        dOutput<<z<<'\t'<<getCp(z)<<endl;
 //       getCp(z);
    }
}

void ship::drawCwp()
{
    dOutput<<"开始模拟绘制棱形系数图\n";
    for(double z:vZ){
        dOutput<<z<<'\t'<<getCwp(z)<<endl;
    }
}


void ship::drawCm()
{
    for(double z:vZ){
        cout<<z<<'\t'<<getCm(z)<<endl;
}

}*/
void ship::calculateAreaXy(double zzz)
{
    double sumY=0,sumMoy=0;
    int i=0;
    sPoint pBegin,pEnd;
    for (sPoint p : vPoints)
        {
            if (p.z-zzz>-0.00001&&p.z-zzz<0.00001&&i<=nX)  //0.000001改用 const 常量
            {
                if(i==0)pBegin=p;
                pEnd=p;
                switch(i%2)
                {
                     case 0:sumY+=(2*p.y);sumMoy+=(i-Xm)*2*p.y;break;
                     case 1:sumY+=(4*p.y);sumMoy+=(i-Xm)*4*p.y;break;
                }
                i++;
            }
        }
    sumY-=(pBegin.y+pEnd.y);
//    cerr<<"sumY"<<sumY<<endl;

    double deltaX=(pEnd.x-pBegin.x)/(i-1)*deltaL;     //i可能为0，之后验证
    double Aw=2.0*sumY*deltaX/3.0;
    cerr<<"Aw"<<Aw<<endl;

    sumMoy-=-nX*pBegin.y/2.0+nX*pEnd.y/2.0;
    Moy=2*sumMoy*deltaX*deltaX/3;

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
}

void ship::calculateAreaYz(double xxx,double zzz)
{
    double sumY=0,sumMoyy=0,deltaZ=0;    //sumMoyy用于画邦戎曲线
    int i=0;
    sPoint pBegin,pEnd;
    for (sPoint p : vPoints)
    {
        if(p.x-xxx>-0.0001&&p.x-xxx<0.0001)
        {
            if(zzz!=-1&&p.z>zzz)break;
            if(i==0)pBegin=p;
            pEnd=p;
            sumY+=p.y;
            sumMoyy+=i*p.y;
            ++i;
        }
    }
    deltaZ=(pEnd.z-pBegin.z)/(i-1);
    sumY-=(pBegin.y+pEnd.y)/2;
    sumMoyy-=i*pEnd.y/2;
    As=2*deltaZ*deltaZ*sumY;
    Moyy=2*deltaZ*deltaZ*sumMoyy;
    cout<<deltaZ<<endl;
}
void ship::calculateVolume(double zzz)
{
    int i=0;
    double Xb=0,Zb=0;
    double Volume;
    double sumArea=0,sumMyoz=0,sumMxoy=0;
    double areaBegin,areaEnd,xfBegin=0,xfEnd=0;

    for (double zz : vZ)
        {
            if(zzz!=-1&&zz-0.0001>zzz)break;
 //           cout<<zz<<"\t";
            if(i==0)
            {

                areaBegin=getAreaXy(zz);
                xfBegin=getXf(zz);
            }
            areaEnd=getAreaXy(zz);
            xfEnd=getXf(zz);

            sumArea+=getAreaXy(zz);
            sumMyoz+=getAreaXy(zz)*getXf(zz);
            sumMxoy+=i*getAreaXy(zz);      //此处getAreaXy(zz)可在FOR循环开始时统一计算；
            i++;
        }
    if(i>1)
    {
    sumArea-=(areaBegin+areaEnd)/2;
    sumMyoz-=(areaEnd*xfEnd+areaBegin*xfBegin)/2;
    sumMxoy-=i*areaEnd/2;

    Volume=(vZ[1]-vZ[0])*sumArea;
    Myoz=(vZ[1]-vZ[0])*sumMyoz;
    Mxoz=(vZ[1]-vZ[0])*(vZ[1]-vZ[0])*sumMxoy;

    //if(Volume==0);
    Xb=Myoz/Volume;
    Zb=Mxoz/Volume;
    }
    else
    {
        Volume=0;
        Xb=0;
    }

    double Cb=Volume/(Lpp*B*zzz);
    double Disp=omega*Volume;

    sZValue p;
    p.z=zzz;
    p.value=Volume;
    vVolume.push_back(p);
    p.value=Zb;
    vZb.push_back(p);
    p.value=Xb;
    vXb.push_back(p);
    p.value=Cb;
    vCb.push_back(p);
    p.value=Disp;
    vDisp.push_back(p);
}
/*
void ship::calculate(double xxx=-1,double zzz=-1)
{
    calculateAreaXy(zzz);
    calculateAreaYz(xxx,zzz);
    calculateVolume(zzz);
}
*/

double ship::getAreaXy(double z)           //用常量引用
{
    for(sZValue p : vAw)
        if(p.z==z)return p.value;
/*
    calculateAreaXy(zzz);
    return Aw;
之后考虑调用的问题

*/
}

/*
double ship::getTPC(double zzz)
{
    TPC=omega*getAreaXy(zzz);
    if(TPC>0)return TPC;
    else throw Wwrong();
}

double ship::getMTC(double zzz)
{
    //每厘米纵倾力矩
}

double ship::getBml(double zzz)
{
    //纵稳心半径
}
*/
double ship::getAreaYz(double x,double z)
{
    calculateAreaYz(x,z);
    return As;
}
double ship::getAreaYz(double xxx)
{
    return getAreaYz(xxx,-1);
}

double ship::getXf(double z)
{
    for(sZValue p : vXf)
        if(p.z==z)return p.value;
    /*
    calculateAreaXy(zzz);
    return Xf;
    */
}


double ship::getVolume(double z)
{
    for(sZValue p : vVolume)
        if(p.z==z)return p.value;
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
    calculateVolume(z);
    calculateAreaXy(z);
    calCm(z);
    calCp(z);
    }
    for(sZValue p : vAw)
        cerr<<"vAw:"<<p.z<<","<<p.value<<endl;
}
/*
double ship::getVolume(double zzz)
{
    calculateVolume(zzz);
    return Volume;
}

double ship::getDisp(double zzz)
{
    return omega*getVolume(zzz);
}

double ship::getCb(double zzz)
{
    //方形系数
//    if(zzz==0)zzz=0.000001;
    Cb=getVolume(zzz)/(Lpp*B*zzz);
    return Cb;
}
*/
void ship::calCp(double zzz)
{
    //getAreaYz(ma,zzz)在最高的水面为nan
    //棱形系数
//    cout<<"Volume"<<getVolume(zzz)<<endl;
//    cout<<"As"<<getAreaYz(zzz)<<endl;
    double Cp=getVolume(zzz)/(Lpp*getAreaYz(ma,zzz));
//    cout<<"getAreaYz(zzz)"<<getAreaYz(ma,zzz)<<endl;
    sZValue p;
    p.z=zzz;
    p.value=Cp;
    vCb.push_back(p);
}
/*
double ship::getCwp(double zzz)
{
    Cwp=getAreaXy(zzz)/(Lpp*B);
    return Cwp;
}
*/
void ship::calCm(double zzz)
{
    double Cm=getAreaYz(ma,zzz)/(B*zzz);
    sZValue p;
    p.z=zzz;
    p.value=Cm;
    vCm.push_back(p);
}
/*
double ship::getXb(double zzz)
{
    getVolume(zzz);
    return Xb;
}
*/
/*
double ship::getZb(double zzz)
{
    getVolume(zzz);
    return Zb;
}


double ship::getBm(double zzz)
{
    //
}
*/
