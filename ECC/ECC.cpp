#pragma warning(disable:4996)
#include "ECC.h"
#include "MD5.h"
/*
	返回gcd(a,b)
	a,b可以为0
*/

LL ECC::gcd(LL a, LL b)
{
	return b > 0 ? gcd(b, a % b) : a;
}


/*
	扩展的欧几里得算法求(x,y)使得ax+by = gcd(a,b)
*/
Point ECC::extend_gcd(LL a, LL b)
{
	Point ans;
	if (a % b == 0)
	{
		ans.x = 0; ans.y = 1; return ans;
	}
	ans = extend_gcd(b, a % b);
	LL x = ans.y;
	LL y = ans.x - (a / b) * ans.y;
	ans.x = x;
	ans.y = y;
	return ans;
}
/*
	求勒让德记号的值
*/
int ECC::quadraticresidue(LL p, LL c)
{
	LL T = 0;
	int L = 1;
	while (c != 1)
	{
		if (c % 2 == 0) // c is even
		{
			LL condition = (p * p - 1) / 8;
			if (condition % 2 == 1) //is odd
			{
				L = L * -1;
			}
			c = c >> 1;
		}
		else {
			LL condition = (p - 1) * (c - 1) / 4;
			if (condition % 2 == 1) //is odd
			{
				L = L * -1;
			}
			T = c;
			c = p;
			p = T;
			c = c % T;
		}
	}
	return L;
}
/*
	求b^n (mod) m  b为底数  n为指数  m为模数 
*/
LL ECC::powerMod(LL b, LL n, LL m)
{
	// a存放计算结果，初始化为1. 
	LL a = 1;
	LL i, k = 0, num = n;
	/*计算指数的二进制位数k.
	*/
	while (num)
	{
		num = num >> 1;
		++k;
	}

	// 也可以将指数的二进制用一个数组或队列存放，方便取值.
	for (i = 0; i < k; ++i)
	{
		// 取n的二进制的第i位，判断是否为1.
		if ((n >> i) & 1)
			a = a * b % m;
		b = b * b % m;
	}

	return a;
}

bool ECC::judgePoint(Point point) {
	return (point.x * point.x * point.x + a * point.x + b)%p == (point.y * point.y)%p;
}

/*
	返回 a mod n，主要是处理a为负数的情况
*/
LL ECC::mod(LL a, LL n)
{
	LL ans = a % n;
	return ans >= 0 ? ans : ans + n;
}

/*
	返回a关于模n的逆,如果不可逆，则返回0
*/
LL ECC::inv(LL a, LL n)
{
	LL flag = gcd(a, n);
	if (flag != 1)return 0;
	int m = n >= 0 ? n : -n;
	Point sol = extend_gcd(a, n);
	return mod(sol.x,m);
}


/*
	返回点p1+p2的结果，p1和p2必须是曲线上的点
*/
Point ECC::add(Point p1, Point p2)
{
	if (p1.O()) return p2; // 如果p1是O，则只需返回p2
	if (p2.O()) return p1; // 如果p2是O，则只需返回p1
	Point ans(0, 0);

	LL lambda = 0;
	if (p1 == p2)
	{
		LL Inv = inv(mod(2 * p1.y, p), p);
		if (Inv == 0)
		{
			ans.x = ans.y = INF;
			return ans;
		}
		lambda = mod(mod(mod(mod(p1.x * p1.x, p) * 3 , p) + a , p) * Inv, p);
	}
	else
	{
		LL Inv = inv(mod(p2.x - p1.x, p), p);
		if (Inv == 0)
		{
			ans.x = ans.y = INF;
			return ans;
		}
		lambda = mod(mod(p2.y - p1.y, p) * Inv, p);
	}
	ans.x = mod((mod(lambda * lambda , p) - p1.x - p2.x) , p);
	ans.y = mod((mod(lambda * (p1.x - ans.x) , p) - p1.y) , p);
	return ans;
}

/*
	返回 p1 - p2 的结果
*/
Point ECC::minus(Point p1, Point p2)
{
	if (p2.O()) return p1;
	p2.y = -p2.y;
	return add(p1, p2);
}

/*
	在有ps和flags的情况下，返回k*p的值
	其中：ps[i]表示k*p的结果，flags[i]表示是否已经求出ps[i]
*/
Point ECC::subMul(LL k, Point p, Point ps[], bool flags[])
{
	if (flags[k]) return ps[k];
	else if (k % 2)//奇数
	{
		Point p1 = subMul(k / 2, p, ps, flags), p2 = subMul(k / 2 + 1, p, ps, flags);
		flags[k] = true;
		return ps[k] = add(p1, p2);
	}
	else//偶数
	{
		Point phalf = subMul(k / 2, p, ps, flags);
		flags[k] = true;
		return ps[k] = add(phalf, phalf);
	}
}

/*
	求k*p的结果
*/
Point ECC::mul(LL k, Point p)
{
	if (k == 0 || p.O()) return Point(INF, INF);
	bool* flags = new bool[k + 1];
	for (int i = 0; i < k + 1; ++i)
	{
		flags[i] = false;
	}
	//memset(flags, false, sizeof(bool) * (k + 1));
	Point* ps = new Point[k + 1];
	ps[0] = Point(INF, INF);
	ps[1] = p;
	flags[0] = flags[1] = true;
	Point ans = subMul(k, p, ps, flags);
	delete[] flags;
	delete[] ps;
	return ans;
}

/*
	将消息Pm加密成{kG,Pm+kP}，返回值的第一个点是kG，第二个是Pm+kP
*/
PointPair ECC::encode(Point Pm)
{
	PointPair ans;
	bool loop = true;
	while (loop)
	{
		LL k = getRandom();
		Point p1 = mul(k, G);
		Point p2 = mul(k, P);
		if (!p1.O() && !p2.O())
		{
			ans.first = p1;
			ans.second = add(p2, Pm);
			loop = false;
		}
	}
	return ans;
}

/*
	将密文Cm = {kG,Pm+kP}解密成消息M对应的点Pm
*/
Point ECC::decode(PointPair Cm)
{
	Point pMinus = mul(r, Cm.first);
	return minus(Cm.second, pMinus);
}

/*
	暂用一个字节加密成点
*/
Point ECC::encodeMessage(unsigned char message)
{
	LL C = (LL)message; //message 为 x
	C = (((C * C * C) % this->p) + (C * a) % p + b) % p; //y^2 = C mod p
	int L = quadraticresidue(this->p, C);
	Point ans(message,0);
	ans.offset = 0;
	while (L < 0)
	{
		ans.x = (ans.x + 1) % this->p;
		ans.offset = (ans.offset + 1) % this->p;
		C = ans.x;
		C = (((C * C * C) % this->p) + (C * a) % p + b) % p;
		L = quadraticresidue(this->p, C);
	}
	C = ans.x;
	C = (((C * C * C) % this->p) + (C * a) % p + b) % p;
	ans.y = powerMod(C, (this->p + 1) / 4, this->p);
	return ans;
	/*
		字符串的预处理部分放在这里保存
		//std::string teststr = "无情铁手";

		//const char* a = teststr.c_str();

		//char* b = new char[teststr.length() + 1];

		//memset(b, 0, teststr.length() + 1);

		//memcpy(b, a, teststr.length());

		//unsigned char* c = (unsigned char*)b; //  byte与  unsigned char*相同

		//for(int i = 0;i < teststr.length();i++)
		//{
		//    std::cout << i << ":" << (int)(c[i]) << std::endl;
		//}

		//std::string f((char*)c);
		//std::cout << f;

	*/

}

/*
	将曲线上的点Pm转化为消息message，和encodeMessage互为逆过程
*/
unsigned char ECC::decodeMessage(Point Pm)
{
	unsigned char ans = (Pm.x + this->p - Pm.offset) % this->p;
	return ans;
}

/*
	构造函数
*/

ECC::ECC()//ECC ecc(6,188, 9220,9967, Point(8, 106));
{
	srand(time(0));
	//设置选好的参数
	this->r = 6;
	this->a = 188;
	this->b = 9220;
	this->p = 9967;
	this->G = Point(8, 106);
	this->P = mul(r, G);
	this->n = 10110;
}

void ECC::setRK(LL key) // 设置密钥
{
	this->r = key;
	this->P = mul(r, G);
}

Point ECC::getPK() // 获得公钥P
{
	return this->P;
}

ECC::ECC(LL r, LL a, LL b, LL p, Point G)
{
	srand(time(0));
	this->r = r;
	this->a = a;
	this->b = b;
	this->p = p;
	this->G = G;
	this->P = mul(r, G);
	//计算n
	Point tG = G;
	this->n = 1;
	while (!tG.O())
	{
		this->n++;
		tG = add(tG, G);
	}
}

void ECC::setEllipticParameter(LL r, LL a, LL b, LL p, Point G)
{
	srand(time(0));
	this->r = r;
	this->a = a;
	this->b = b;
	this->p = p;
	this->G = G;
	this->P = mul(r, G);
	//计算n
	Point tG = G;
	this->n = 1;
	while (!tG.O())
	{
		this->n++;
		tG = add(tG, G);
	}
}

ECC::ECC(LL r, LL a, LL b, LL p, Point G, LL n)
{
	this->r = r;
	this->a = a;
	this->b = b;
	this->p = p;
	this->G = G;
	this->P = mul(r, G);
	this->n = n;
}

void ECC::setEllipticParameter(LL r, LL a, LL b, LL p, Point G, LL n)
{
	this->r = r;
	this->a = a;
	this->b = b;
	this->p = p;
	this->G = G;
	this->P = mul(r, G);
	this->n = n;
}

/*
	获取随机数k , 1 <= k < p
*/
LL ECC::getRandom()
{
	LL low = 1, up = p;
	return (rand() % (up - low)) + low;
}

/*
	对消息进行数字签名
*/
SignedMessage ECC::sign(std::string message)
{
	SignedMessage ret;
	const int pointNum = 16 / BYTE_PER_POINT;
	char s1[100], s2[100];
	LL k = getRandom();
	Point R = mul(k, G);
	sprintf(s1, "%x", R.x);
	sprintf(s2, "%x", R.y);
	ret.message = message;
	message += s1;
	message += s2;
	ret.hash = MD5(message).toString();
	const int strSize = BYTE_PER_POINT * 2;
	for (int i = 0; i < pointNum; ++i) // 获得各个数字
	{
		std::string tStr = ret.hash.substr(strSize * i, strSize);
		ret.numbers[i] = strtoll(tStr.c_str(), NULL, 16); // 16进制的数字字符串转为long long
	}

	for (int i = 0; i < pointNum; ++i) // 加密
	{
		ret.Sn[i] = mod(k - mod(ret.numbers[i] * r, n), n);
	}
	return ret;
}

/*
	对数字签名进行验证
*/
VerifyResult ECC::verify(SignedMessage signedMessage)
{
	VerifyResult ret;
	ret.message = signedMessage.message;
	const int pointNum = 16 / BYTE_PER_POINT;
	Point Rs[pointNum];

	for (int i = 0; i < pointNum; ++i)
	{
		Point p1 = mul(signedMessage.Sn[i], G);
		Point p2 = mul(signedMessage.numbers[i], P);
		Rs[i] = add(p1, p2);
	}
	bool flag = true;
	for (int i = 1; i < pointNum; ++i)
		if (!(Rs[0] == Rs[i])) {
			flag = false;
			break;
		}
	if (!flag)
	{
		ret.success = false; return ret;
	}

	std::string verifyMessage = signedMessage.message;
	char s1[100], s2[100];
	sprintf(s1, "%x", Rs[0].x);
	sprintf(s2, "%x", Rs[0].y);
	verifyMessage += s1;
	verifyMessage += s2;

	std::string H = MD5(verifyMessage).toString();
	ret.success = H == signedMessage.hash;
	return ret;
}


/*
 * author:zfq
 */
bool ECC::encodefile(std::string inputFilePath, std::string outputFilePath) {

	if (inputFilePath == "" || outputFilePath == "")
		return false;

	FILE * in = fopen(inputFilePath.c_str(), "rb");
	FILE * out = fopen(outputFilePath.c_str(), "wb");
	fseek(in, 0, SEEK_END);
	LL size = ftell(in);//文件的字节数
	fseek(in, 0, SEEK_SET);

	byte * buf = new byte[size];
	PointPair * pointPairs = new PointPair[size];

	Point m;

	fread(buf, sizeof(byte), size, in);
	for (LL i = 0; i < size; i++) {
		//printf("%d\n", buf[i]);
		m = this->encodeMessage(buf[i]); //明文嵌入 99 8299 1
		if (!judgePoint(m)) {
			printf("点不在椭圆上");
		}
		pointPairs[i] = encode(m);
		pointPairs[i].first.offset = m.offset;
		// buf[i] = this->decodeMessage(points[i]);
	}

	fwrite(pointPairs, sizeof(PointPair), size, out);

	fclose(in);
	fclose(out);
	delete[] pointPairs;
	delete[] buf;
}

/*
 * author:zfq
 */
bool ECC::decodefile(std::string inputFilePath, std::string outputFilePath) {

	if (inputFilePath == "" || outputFilePath == "")
		return false;

	FILE* in = fopen(inputFilePath.c_str(), "rb");
	FILE* out = fopen(outputFilePath.c_str(), "wb");
	fseek(in, 0, SEEK_END);
	LL len = ftell(in); // 该文件有多少字节
	fseek(in, 0, SEEK_SET);

	if (len % sizeof(PointPair) != 0) {
		printf("加密文件损坏");
		return false;
	}

	LL size = len / sizeof(PointPair);
	PointPair* pointPairs = new PointPair[size];
	byte * buf = new byte[size];

	Point m;
	fread(pointPairs, sizeof(PointPair), size, in);
	for (LL i = 0; i < size; i++) {
		//printf("%c", buf[i]);
		// points[i] = this->encodeMessage(buf[i]);
		m = decode(pointPairs[i]);
		m.offset = pointPairs[i].first.offset;
		buf[i] = this->decodeMessage(m);
		//printf("%d\n", buf[i]);
	}

	fwrite(buf, sizeof(byte), size, out);

	fclose(in);
	fclose(out);
	delete[] pointPairs;
	delete[] buf;
}

