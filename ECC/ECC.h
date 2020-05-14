#pragma once
#include <string>
#define LL long long
#define INF 0x7FFFFFFFFFFFFFFF
#define BYTE_PER_POINT 1 //将多少个字节压缩成一个点

struct Point
{
	LL x, y;
	Point()
	{
		this->x = 0;
		this->y = 0;
	}
	Point(LL x, LL y)
	{
		this->x = x;
		this->y = y;
	}
	bool operator == (const Point& p) const
	{
		return this->x == p.x && this->y == p.y;
	}
	bool O()
	{
		return this->x == INF && this->y == INF;
	}
};

struct SignedMessage
{
	std::string message;
	std::string hash;
	LL numbers[16 / BYTE_PER_POINT];
	LL Sn[16 / BYTE_PER_POINT];//MD5的HASH结果为16字节
};

struct VerifyResult
{
	bool success;//是否签名成功
	std::string message;
};

struct PointPair
{
	Point first, second;
};

class ECC
{
private:// 成员变量
	
	LL r;//密钥

	//椭圆曲线 y^2 = x^3 + ax + b  mod p
	LL a;
	LL b;
	LL p;
	LL n;// G 的阶

	LL randomK;//随机数 -- 可能不会用到

	Point G;//基底
	Point P;//公钥 P = rG
public:// 成员函数
	Point add(Point p1, Point p2);// 返回p1 + p2 的结果
	Point minus(Point p1, Point p2);// 返回 p1 - p2 的结果
	Point mul(LL k, Point p);// 返回 k*p 的结果
	Point subMul(LL k, Point p, Point ps[], bool flags[]);// 给mul调用以求k*p
	
	PointPair encode(Point Pm); // 将消息Pm加密成{kG,Pm+kP}，返回值的第一个点是kG，第二个是Pm+kP
	Point decode(PointPair Cm); // 将密文Cm = {kG,Pm+kP}解密成消息M对应的点Pm

	//消息的类型可以修改，不一定是char*  ,string什么的也行
	Point encodeMessage(char* message);//将消息转化为曲线上的点Pm
	char* decodeMessage(Point Pm);//将曲线上的点Pm转化为消息message，和encodeMessage互为逆过程

	LL getRandom(); // 获取随机数k , 1 <= k < p


private:// 静态工具函数区
	static LL mod(LL a, LL n); // 返回 a mod n，主要是处理a为负数的情况
	static LL inv(LL a, LL n); // 返回 a关于模n的逆元，如果不可逆则返回0
	static LL gcd(LL a, LL b); // 返回gcd(a,b)
	static Point extend_gcd(LL a, LL b);//扩展的欧几里得算法求(x,y)使得ax+by = gcd(a,b)
	
public:// 给前端调用的接口
	ECC(); // 默认使用默认的值进行构造

	//以下构造函数要通过公式 P=rG 计算出P的值
	ECC(LL r,LL a,LL b,LL p,Point G); // 传入自定义的值进行构造，但是必须计算出G的阶，通过O=nG计算
	ECC(LL r, LL a, LL b, LL p, Point G, LL n);

	
	//暂时不管返回值和参数类型  ---  每完成一个再解除注释一个
	/*
	void encode(); // 将明文加密为密文
	void decode(); // 将密文加密为明文
	
	
	*/

	SignedMessage sign(std::string message); // 对消息进行数字签名
	VerifyResult verify(SignedMessage signedMessage); // 数字签名验证
};

