﻿#pragma warning(disable:4996)
// ECC.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "ECC.h"
#include <string>
void printBigPrime();
void findECC();
void findG(LL a, LL b, LL p, LL minN);

int main()
{
    /*ECC ecc(2, 1, 6, 11, Point(7, 9));
    Point p = ecc.mul(6, Point(7, 9));

    std::cout << p.x << "   " << p.y << std::endl;*/

 //   std::string str = "椭圆曲线算法";
 //   printf("%s", str.data());
 //   str.data();
	//printf("%d", -5 % 3);
    //std::cout << "Hello World!\n";

    //printBigPrime();
    findECC();
}
