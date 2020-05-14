#pragma warning(disable:4996)
// ECC.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "ECC.h"
#include "MD5.h"

int main()
{
    ECC ecc(2, 1, 6, 11, Point(7, 9));
    Point p = ecc.mul(6, Point(7, 9));
    std::cout << p.x << "   " << p.y << std::endl;


    /*std::string str = MD5("abcde").toString();
    str += "12345";
    std::cout << str << std::endl;*/


    /*LL ll = 15;
    std::string str = "number is ";
    char  s[100];
    sprintf(s, "%x", ll);
    str += s;
    std::cout << str;*/
}
