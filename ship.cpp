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
    vZ.clear();
    vPoints.clear();
    vPoints2.clear();
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

   Lpp=0,L0=0,d0=0,B=0,deltaL=0;
   nX=0,nZ=0;

   cx=m=n=ma=cs=cw=mw=nw=0;
   Xm=Lpp=Loa=B=Bw=deltaL=omega=f=0.0;

}

ship::~ship()
{
    //dtor
}

void ship::Init()
{
    vZ.clear();
    vPoints.clear();
    vPoints2.clear();
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

   Lpp=0,L0=0,d0=0,B=0,deltaL=0;
   nX=0,nZ=0;

   cx=m=n=ma=cs=cw=mw=nw=0;
   Xm=Lpp=Loa=B=Bw=deltaL=omega=f=0.0;
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
    vPoints2=vPoints;
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

    sort(vPoints.begin(),vPoints.end(),Cmp_by_X());
    vPoints.erase( unique( vPoints.begin(), vPoints.end() ), vPoints.end() );

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
    sort(v.begin(),v.end(),Cmp_by_Y());
    v.erase( unique( v.begin(), v.end() ), v.end() );

    return v;
}

vector<sPoint> ship::drawYX(const sPoint &s)
{
    //
    vector<sPoint> v;
    for (sPoint p : vPoints2)
        if(p.y==s.y)v.push_back(p);
//    sort(v.begin(),v.end(),Cmp_by_Z());
    return v;
}

/*
vector<sPoint> ship::drawYX(const sPoint &s)
{
    vector<sPoint> v;
    vector<sPoint> u;
    for (sPoint p : vPoints2)
        if(p.y==s.y)
        {
            if(p.x<ma)v.push_back(p);
            else u.push_back(p);
        }
    sort(v.begin(),v.end(),Cmp_by_X());
    sort(u.begin(),u.end(),Cmp_by_X());
    v.insert(v.end(), u.begin(), u.end());
    return v;
}
*/

vector<sPoint> ship::drawZX(const double &z)
{
    vector<sPoint> v;
    for (sPoint p : vPoints)
        if(p.z==z)v.push_back(p);
    sort(v.begin(),v.end(),Cmp_by_X());
    v.erase( unique( v.begin(), v.end() ), v.end() );
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
void ship::calculateAw(double zzz)            //改用梯形法（有些站没有交点的问题；2013/12/20）
{
    double sumAw=0,sumMoy=0,sumIt=0,sumIl=0;
    double xx=-1,yy=-1;
    for (sPoint p : vPoints2)
        {
            if (p.z-zzz>-0.00001&&p.z-zzz<0.00001)  //0.000001改用 const 常量
            {

                if(xx!=-1)
                {
                    sumAw+=(p.y+yy)*(p.x-xx)/2.0;
                    sumMoy+=(p.x*p.y+xx*yy)*(p.x-xx)/2.0;
                    sumIt+=(p.y*p.y*p.y+yy*yy*yy)*(p.x-xx)/2.0;
                    sumIl+=(p.x*p.x*p.y+xx*xx*yy)*(p.x-xx)/2.0;
                }
                xx=p.x;
                yy=p.y;
            }
        }

    double Aw=2.0*sumAw;            //水线面面积Aw
    double Moy=2.0*sumMoy;         //水线面对OY轴静矩Moy
    double It=sumIt*2.0/3.0;       //水线面的横向惯性矩
    double Il=2.0*sumIl;           //水线面的横向惯性矩

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
    double sumY=0,sumMoyy=0,deltaZ=0;    //sumMoyy用于画邦戎曲线
    int i=0;
    sPoint pBegin,pEnd;
    for (sPoint p : vPoints2)
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

/*
void ship::calculateAs(double xxx,double zzz)             //问题！！！
{
    double sumAs=0,sumMoyy=0;    //sumMoyy用于画邦戎曲线
    double zz=-1,yy=-1;
    for (sPoint p : vPoints2)
        {
        if(zzz!=-1&&p.z>(zzz+0.001))break;
        if(p.x==xxx)  //0.0001改用 const 常量
            {
                if(zz!=-1)
                {
                    sumAs+=(p.y+yy)*(p.z-zz)/2.0;
                    sumMoyy+=(p.z*p.y+zz*yy)*(p.z-zz)/2.0;
                }
                zz=p.z;
                yy=p.y;
            }
        }

    As=2*sumAs;
    cerr<<"As"<<zzz<<","<<As<<endl;

    Moyy=2*sumMoyy;

/*
    sZValue p;
    p.z=zzz;

    p.value=As;
    vAs.push_back(p);

    p.value=Za;
    vZa.push_back(p);
*/
//}
/*
void ship::calculateVolume(double zzz)            使用纵向计算法
{
    double xx=-1;
    double Xb=0,Zb=0;
    double oldAs=0,oldMoyy=0;
    double Volume=0,Myoz=0,Mxoy=0;

    for (sPoint p : vPoints2)
    {
            if(xx!=-1&&xx!=p.x)
            {
                calculateAreaYz(p.x,zzz);
                Volume+=(As+oldAs)*(p.x-xx)/2.0;
                Myoz+=(p.x*As+xx*oldAs)*(p.x-xx)/2;
                Mxoy+=(p.x*Moyy+xx*oldMoyy)*(p.x-xx)/2;
                xx=p.x;
                oldAs=As;
                oldMoyy=Moyy;
            }
    }
    Xb=Myoz/Volume;
    Zb=Mxoy/Volume;

    double Cb=Volume/(Lpp*B*zzz);
    double Disp=omega*Volume;

    for(sZValue pp : vIt)           //获取对应的It,用的多的话改成函数；
        if(pp.z=zzz)
            double Bm=pp.value/Volume;

    for(sZValue pp : vIl)           //获取对应的It,用的多的话改成函数；
        if(pp.z=zzz)
            double Bml=(pp.value-getAreaXy(pp.z)*getXf(pp.z)*getXf(pp.z))/Volume;
    double MTC=Volume*Bml/(100*Lpp);

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

    p.value=Bm;
    vBm.push_back(p);

    p.value=Bml;
    vBml.push_back(p);

    p.value=MTC;
    vMTC.push_back(p);
}
*/

void ship::calculateVolume(double zzz)
{
    int i=0;
    double Xb=0,Zb=0;
    double Volume=0;
    double sumArea=0,sumMyoz=0,sumMxoy=0;
    double areaBegin,areaEnd,xfBegin=0,xfEnd=0;           //之后做优化

    double Cp=0;
    double volume2=0;
    for (double zz : vZ)
        {
            if(zzz!=-1&&zz-0.0001>zzz)break;
 //           cout<<zz<<"\t";
            if(i==0)
            {
                areaBegin=getAw(zz);
                xfBegin=getXf(zz);
            }
            areaEnd=getAw(zz);
            xfEnd=getXf(zz);

            volume2=areaEnd*zz;

            sumArea+=getAw(zz);
            sumMyoz+=getAw(zz)*getXf(zz);
            sumMxoy+=i*getAw(zz);      //此处getAreaXy(zz)可在FOR循环开始时统一计算；
            i++;
        }

    if(i>1)
    {
    sumArea-=(areaBegin+areaEnd)/2;
    sumMyoz-=(areaEnd*xfEnd+areaBegin*xfBegin)/2;
    sumMxoy-=i*areaEnd/2;

    Volume=(vZ[1]-vZ[0])*sumArea;
    double Myoz=(vZ[1]-vZ[0])*sumMyoz;
    double Mxoz=(vZ[1]-vZ[0])*(vZ[1]-vZ[0])*sumMxoy;

    Cp=Volume/volume2;
//    cerr<<"Cp"<<Cp<<endl;

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

/*
void ship::calculate(double xxx=-1,double zzz=-1)
{
    calculateAreaXy(zzz);
    calculateAreaYz(xxx,zzz);
    calculateVolume(zzz);
}
*/

double ship::getAw(double z)           //用常量引用
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
    calculateAs(x,z);
    cerr<<"As"<<As<<endl;
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
        return -1;

    /*
    calculateAreaXy(zzz);
    return Xf;
    */
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
    calCm(z);
    }
/*    for(sZValue p : vAw)
        cerr<<"vAw:"<<p.z<<","<<p.value<<endl;
*/
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
/*void ship::calCp(double zzz)
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
*/

/*
double ship::getCwp(double zzz)
{
    Cwp=getAreaXy(zzz)/(Lpp*B);
    return Cwp;
}
*/


void ship::calCm(double zzz)
{
    calculateAs(ma,zzz);
    double Cm=As/(B*zzz);
    sZValue p;
    p.z=zzz;
    p.value=Cm;
//    cerr<<"Cm"<<Cm<<","<<endl;
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
bool ship::exLinesPlan(string fileName)
{
    ofstream outFile(fileName.c_str(),ios_base::out);
    if (outFile.is_open())
    {
        //复制的
        for (sPoint p : vPoints)
        {
          double x=-1;
          if(p.x==x)continue;
            x=p.x;
            vector<sPoint>sP=drawXZ(p.x);

            outFile<<"spline\n";
            for (sPoint pp : sP)
            {
                if(pp.x<ma)pp.y=-pp.y;
                outFile<<pp.y<<","<<pp.z<<"\n";
            }
            outFile<<"\n\n\n";

/*
            glBegin(GL_LINE_STRIP);
            for (sPoint p : sP)
            {
                glVertex3f(GLfloat(p.x*deltaL- deltaL*ma)/Lpp*5,GLfloat(p.z)/Lpp*5,-GLfloat(p.y)/Lpp*5);
            }
            glEnd();

            x=p.x;
        }
            if(p.z!=z){
            vector<sPoint>sP=drawZX(p.z);
            glBegin(GL_LINE_STRIP);
            for (sPoint p : sP)
            {
                glVertex3f(GLfloat(p.x*deltaL- deltaL*ma)/Lpp*5,GLfloat(p.z)/Lpp*5,GLfloat(p.y)/Lpp*5);
            }
            glEnd();
            glBegin(GL_LINE_STRIP);
            for (sPoint p : sP)
            {
                glVertex3f(GLfloat(p.x*deltaL- deltaL*ma)/Lpp*5,GLfloat(p.z)/Lpp*5,-GLfloat(p.y)/Lpp*5);
    //            cerr<<GLfloat(p.x*deltaL- deltaL*ma)/Lpp*5<<","<<GLfloat(p.z)/Lpp*5<<","<<-GLfloat(p.y)/Lpp*5;
            }
            glEnd();

            z=p.z;
        */
        }
    }

    outFile.close();
    return true;
}
