#ifndef SHIP_H
#define SHIP_H

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

struct sPoint
{
    double x;
    double y;
    double z;
    sPoint():x(0),y(0),z(0){}
    sPoint(double x,double y,double z):x(x),y(y),z(z){}
    sPoint(const sPoint& p)
    {
        x=p.x;
        y=p.y;
        z=p.z;
    }

    bool operator==( const sPoint& sP ) const
    {
        return x == sP.x&&y == sP.y&&z == sP.z;
    }
};

struct Cmp_by_Xz
{
    bool operator()(const sPoint &a,const sPoint &b)const
    {
        return a.x<b.x||(a.x==b.x&&a.z<b.z);
    }
};
/*
struct Cmp_by_Y
{
    bool operator()(const sPoint &a,const sPoint &b)const
    {
        return a.y<b.y;
    }
};
*/
struct Cmp_by_Zy
{
    bool operator()(const sPoint &a,const sPoint &b)const
    {
        return a.z<b.z||(a.z==b.z&&a.y<b.y);
    }
};


//新添加绘制静水力曲线的点；
struct sZValue            //静水力曲线中的（z,value）
{
    double z;
    double value;
};

class importError{};


class ship
{
public:
    friend class MainWindow;
    ship();
    virtual ~ship();

    bool import(string fileName);

    vector<sPoint> drawXZ(const double &x);            //返回某站的点，按Z排序;
    vector<sPoint> drawYZ(const sPoint &s);            //返回某站的点，按X排序;
    vector<sPoint> drawZX(const double &z);            //返回某水线面的点，按x排序;
    vector<sPoint> drawYX(const sPoint &s);


    /*
        void drawZb();                            //绘制Zb;
        void drawAreaXy();                        //绘制水线面图;
        void drawTPC();                           //绘制TPC图;
        void drawXf();                            //绘制Xf图;
        void drawXb();                            //绘制Xb图;
        void drawVolume();                        //绘制型排水体积图;
        void drawDisp();                          //绘制型排水体积图;
        void drawCb();                            //绘制Cb图;
        void drawCp();                            //绘制Cp图;
        void drawCwp();                           //绘制Cwp图;
        void drawCm();                            //绘制型排水体积图;
*/
    /*        double getZb(double zzz);                 //浮心垂向坐标Zb**静水力曲线;
        double getBm(double zzz);                 //横稳心半径Bm**静水力曲线;
        double getTPC(double zzz);                //每厘米吃水吨数TPC**静水力曲线;
        double getMTC(double zzz);                //每厘米纵倾力矩MTC**静水力曲线;
        double getBml(double zzz);                //纵稳心半径Bml**静水力曲线;
        double getXb(double zzz);                 //浮心纵坐标Xb**静水力曲线;
        double getDisp(double zzz);               //总排水量Disp**静水力曲线;
        double getCb(double zzz);                 //方形系数Cb**静水力曲线;
        double getCwp(double zzz);                //水线面系数Cwp**静水力曲线;

        */
    double getVolume(double zzz);             //型排水体积Volume**静水力曲线;
    double getXf(double z);                 //漂心纵坐标Xf**静水力曲线;
    double getAw(double z);             //水线面面积Aw**静水力曲线;

    void calCp(double zzz);                 //棱形系数Cp**静水力曲线;
    void calCm(double zzz);                 //中横剖面系数Cm**静水力曲线;
    /*
        void calZb();                 //浮心垂向坐标Zb**静水力曲线;
        void calBm();                 //横稳心半径Bm**静水力曲线;
        void calAreaXy();             //水线面面积Aw**静水力曲线;
        void calTPC();                //每厘米吃水吨数TPC**静水力曲线;
        void calMTC();                //每厘米纵倾力矩MTC**静水力曲线;
        void calBml();                //纵稳心半径Bml**静水力曲线;
        void calXf();                 //漂心纵坐标Xf**静水力曲线;
        void calXb();                 //浮心纵坐标Xb**静水力曲线;
        void calVolume();             //型排水体积Volume**静水力曲线;
        void calDisp();               //总排水量Disp**静水力曲线;
        void calCb();                 //方形系数Cb**静水力曲线;
        void calCp();                 //棱形系数Cp**静水力曲线;
        void calCwp();                //水线面系数Cwp**静水力曲线;
        void calCm();                 //中横剖面系数Cm**静水力曲线;
*/

    double getAs(double x);
    double getAs(double x, double z);


    void calculate();

    ifstream inFile;
    ostringstream iOutput;
    ostringstream cOutput;
    ostringstream dOutput;

    bool exLinesPlan(const string &fileName);
    bool exHyCurve(const string &fileName);
    bool ExBC(const string &fileName);
protected:
    bool importOffsets();
    bool importPrinDim();
    bool importAddPTrans();
    bool importAddPWplane();

    void calculate(double xxx,double zzz);
    void calculateAw(double zzz);
    void calculateAs(double xx, double zz);
    void calculateVolume(double zzz);

    void Init();

    int cx,m,n,ma,cs,cw,mw,nw;
    //依次为 cx-船舶类型 m-计算横剖面数 n-计算水线数 ma-最大横剖面所在序号
    //cs-横剖面插入点数 cw-水线面插入点数
    //mw-有涡尾横剖面站数（包括尾端面,无涡尾取0）
    //nw-有涡尾水线根数 (包括基线,无涡尾取0)

    double Xm,Lpp,Loa,B,Bw,deltaL,omega,f,O;
    //Xm: 船舯站号( 5 | 10 )
    //Lpp:垂线间长(m)
    //Loa:最大长(m)
    //B:  型宽(m)
    //Bw: 设计水线宽(m)
    //deltaL ΔL:标准站距(m)
    //ω omega: 水的重度(t/m3)
    //h:  甲板梁拱(m)
    //o:  设计纵倾(m)

    double mZ;
    double L0,d0;  //L-垂线间长,L0-0站以后部分长度，d0-0站以后部分形心到0站距离
    int nX;             //nX-站数

    vector<double> vZ;
    vector<double> vZhan;
    vector<sPoint> vPoints;

    vector<sZValue> vZb;                 //浮心垂向坐标Zb**静水力曲线;
    vector<sZValue> vBm;     //**        //横稳心半径Bm**静水力曲线;
    vector<sZValue> vAw;                 //水线面面积Aw**静水力曲线;
    vector<sZValue> vTPC;                //每厘米吃水吨数TPC**静水力曲线;
    vector<sZValue> vMTC;                //每厘米纵倾力矩MTC**静水力曲线;
    vector<sZValue> vBml;                //纵稳心半径Bml**静水力曲线;
    vector<sZValue> vXf;                 //漂心纵坐标Xf**静水力曲线;
    vector<sZValue> vXb;                 //浮心纵坐标Xb**静水力曲线;
    vector<sZValue> vVolume;             //型排水体积Volume**静水力曲线;
    vector<sZValue> vDisp;               //总排水量Disp**静水力曲线;
    vector<sZValue> vCb;                 //方形系数Cb**静水力曲线;
    vector<sZValue> vCp;                 //棱形系数Cp**静水力曲线;
    vector<sZValue> vCwp;                //水线面系数Cwp**静水力曲线;
    vector<sZValue> vCm;                 //中横剖面系数Cm**静水力曲线;

    vector<sZValue> vIt;                 //水线面的横向惯性矩--用于计算横稳心;
    vector<sZValue> vIlf;                //水线面的纵向惯性矩（对于漂心横轴）--用于计算纵稳心;
    vector<sZValue> vIl;                 //水线面的纵向惯性矩（对于中站横轴）--用于计算Ilf;

    vector<sPoint> vAs;
    vector<sPoint> vMoyy;
};

#endif // SHIP_H
