#include <iostream>
#include "ECC.h"
#define P 9967
using namespace std;

void printBigPrime()
{
    long long int min;
    long long int max;
    cout << "请输入搜索最小值:";
    cin >> min;
    cout << "请输入搜索最大值:";
    cin >> max;
    cout << "素数列表：";
    for (long long int a = min; a <= max; a++)
    {
        long long int key = 0;
        for (long long int i = 2; i <= a / 2; i++)
        {
            if (a % i == 0)
            {
                key = 1;
                break;
            }
        }
        if (!key && a % 4 == 3)
        {
            cout << a << " ";
        }
    }
    cout << endl;
}

void findECC()
{
    int aMax = P, bMax = P;
    //cout << "素域范围必须为" << P << "，输入a的最大值:" << endl;
    //cin >> aMax;
    //cout << "输入b的最大值:" << endl;
    //cin >> bMax;
    //cout << "输入最少的整数解数量:" << endl;
    //cin >> nMin;
    //cout << "可行的曲线:" << endl;
    //ECC ecc;
    cout << "P = " << P << endl;
    int MAXN = 0;
    int mA = 0, mB = 0;
    /*for (int a = 1; a < aMax; a++)
        for (int b = 1; b < bMax; b++)
        {*/
            int n = 0,a= 188,b= 9220;
            //ecc.setEllipticParameter(0, a, b, P, Point(0,0));
            for (int x = 1; x < P; x++)
            {
                int m = x * x * x + a * x + b;
				int tmp = round(sqrt(m));
                if (tmp*tmp == m)
                {
                    n++;
                }
            }
			cout << n << endl;
            /*if (n > MAXN)
            {
                MAXN = n;
                mA = a;
                mB = b;
                cout << "找到a = " << a << " , b = " << b << " , n = " << n << endl;
            }*/
        //}
    cout << "解的数量最多时：" << endl;
    cout << "a:" << mA << ",b:" << mB << ",解的最大数量:" << MAXN << endl;
}

void findG(LL a, LL b, LL p, LL minN) // minN是G的最小阶数
{
    ECC ecc;
    ecc.setEllipticParameter(0, a, b, p, Point(0, 0));
    for (LL x = 1; x < p; ++x)
    {
        int m = x * x * x + a * x + b;
        if (sqrt(m) - int(sqrt(m)) <= 0)
        {
            Point p0 = Point(x,LL(sqrt(m)));
            int n = 1;
            Point tP = p0;
            while (!tP.O())
            {
                ++n;
                tP = ecc.add(tP, p0);
            }
            if (n >= minN)
            {
                cout << "x = " << p0.x << " , y = " << p0.y << " , n = " << n << endl;
            }
        }
    }
}
