#include <math.h>
#include "MathUtil.h"
#include "Logger.h"

using namespace Lynx;

#define LYNX_FLT_MAX         3.402823466e+38F        /* max value */

inline float 
Vector2::length() const
{
    return sqrt(mX * mX + mY * mY);
}

float 
Vector2::normalise(float aimlen)
{
    float length = sqrt(mX * mX + mY * mY);

    if (length > 1e-08)
    {
        float invLen = aimlen / length;
        mX *= invLen;
        mY *= invLen;
    }
    return length;
}

const Vector2 Vector2::ZERO(0.0f,0.0f);
const Vector2 Vector2::UNIT_X(1.0f,0.0f);
const Vector2 Vector2::UNIT_Y(0.0f,1.0f);
const Vector2 Vector2::UNIT_ALL(1.0f,1.0f);

const Vector3 Vector3::ZERO(0.0f,0.0f,0.0f);
const Vector3 Vector3::MAX(LYNX_FLT_MAX, LYNX_FLT_MAX, LYNX_FLT_MAX);
const Vector3 Vector3::UNIT_X(1.0f,0.0f,0.0f);
const Vector3 Vector3::UNIT_Y(0.0f,1.0f,0.0f);
const Vector3 Vector3::UNIT_Z(0.0f,0.0f,1.0f);
const Vector3 Vector3::UNIT_ALL(1.0f,1.0f,1.0f);

const Vector4 Vector4::ZERO(0.0f,0.0f,0.0f, 0.0f);
const Vector4 Vector4::MAX(LYNX_FLT_MAX, LYNX_FLT_MAX, LYNX_FLT_MAX, LYNX_FLT_MAX);
const Vector4 Vector4::UNIT_X(1.0f,0.0f,0.0f,0.0f);
const Vector4 Vector4::UNIT_Y(0.0f,1.0f,0.0f,0.0f);
const Vector4 Vector4::UNIT_Z(0.0f,0.0f,1.0f,0.0f);
const Vector4 Vector4::UNIT_W(0.0f,0.0f,0.0f,1.0f);
const Vector4 Vector4::UNIT_ALL(1.0f,1.0f,1.0f,1.0f);

float 
Vector3::length() const
{
    return sqrt(mX * mX + mY * mY + mZ * mZ);
}
        
float 
Vector3::normalise(float aimlen)
{
    float length = sqrt(mX * mX + mY * mY + mZ * mZ);

    if (length > 1e-08)
    {
        float invLen = aimlen / length;
        mX *= invLen;
        mY *= invLen;
        mZ *= invLen;
    }
    return length;
}

float 
MathUtil::getDistance(const Vector2& pos1, const Vector2& pos2)
{
    return sqrtf((pos1.mX - pos2.mX)*(pos1.mX - pos2.mX) + 
        (pos1.mY - pos2.mY) * (pos1.mY - pos2.mY));
}

float 
MathUtil::getDistance(const Vector3& pos1, const Vector3& pos2)
{
    return sqrtf((pos1.mX - pos2.mX) * (pos1.mX - pos2.mX) + 
        (pos1.mY - pos2.mY) * (pos1.mY - pos2.mY) + 
        (pos1.mZ - pos2.mZ) * (pos1.mZ - pos2.mZ));
}

float 
MathUtil::getDistanceSqrt(const Vector2& pos1, const Vector2& pos2)
{
    return (pos1.mX - pos2.mX) * (pos1.mX - pos2.mX) + 
        (pos1.mY - pos2.mY) * (pos1.mY - pos2.mY);
}

float 
MathUtil::getDistanceSqrt(const Vector3& pos1, const Vector3& pos2)
{
    return (pos1.mX - pos2.mX) * (pos1.mX - pos2.mX) + 
        (pos1.mY - pos2.mY) * (pos1.mY - pos2.mY) + 
        (pos1.mZ - pos2.mZ) * (pos1.mZ - pos1.mZ);
}

float 
MathUtil::dotProduce(const Vector2& v1, const Vector2& v2)
{
    return v1.mX * v2.mX + v1.mY * v2.mY;
}

float 
MathUtil::dotProduce(const Vector3& v1, const Vector3& v2)
{
    return v1.mX * v2.mX + v1.mY * v2.mY + v1.mZ * v2.mZ;
}

float 
MathUtil::getYAngle(const Vector2& pos1, const Vector2& pos2)
{
    double distance = (double)getDistance(pos1, pos2);
    if (distance <= 0.0f) return 0.0f;

    double aCos = (pos2.mY - pos1.mY) / distance;
    if (aCos > 1.0) aCos = 0.0;
    if (aCos < -1.0) aCos = LYNX_PI;

    if (aCos > -LYNX_MINFLOAT && aCos < LYNX_MINFLOAT)
    {
        if (pos2.mX > pos1.mX) 
        {
            return (float)LYNX_PI/2.0f;
        }
        else
        {
            return -(float)LYNX_PI/2.0f;
        }
    }

    aCos = ::acos(aCos);

    // [0~180]
    if (pos2.mX >= pos1.mX)
    {
        return (float)aCos;
    }
    else
    {
        return (float)(2 * LYNX_PI - aCos);
    }
}
  
Vector3 
MathUtil::getCenter(const Vector3& pos1, const Vector3& pos2)
{
    Vector3 ret;
    ret.mX = (pos1.mX + pos2.mX) / 2.0f;
    ret.mY = (pos1.mY + pos2.mY) / 2.0f;
    ret.mZ = (pos1.mZ + pos2.mZ) / 2.0f;
    return ret;
}
  
Vector2 
MathUtil::getReflect(const Vector2& pos1, const Vector2& pos2, const Vector2& v1)
{
    // pos1 -> pos2
    Vector2 normal = pos2 - pos1;
    normal.normalise();
    // pos1 -> v1
    Vector2 line1 = v1 - pos1;
    // pos1 -> v2
    Vector2 line2 = 2 * dotProduce(line1, normal) * normal - line1;
    // return v2
    return line2 + pos1;
}

float
MathUtil::getSqrtLen(const Vector3& pos1, const Vector3& pos2)
{
    return sqrtf((pos1.mX - pos2.mX) * (pos1.mX - pos2.mX) +
           (pos1.mZ - pos2.mZ) * (pos1.mZ - pos2.mZ)); 
}

float
MathUtil::getRoughLen(const Vector3& pos1, const Vector3& pos2)
{
    return KMAX(abs(pos1.mX - pos2.mX), abs(pos1.mZ - pos2.mZ));
}

Vector3  
MathUtil::getInteractPos(const Vector3& fromPos, 
                         const Vector3& toPos, float radius)
{
    const float len = getSqrtLen(fromPos, toPos);
    if (len <= 0.01f)
    {
        Vector3 newPos = toPos;
        return newPos;
    }
    const float fPerc = radius / len;
    if(fPerc >= 1.0f)
    {
        Vector3 newPos = fromPos;
        return newPos;
    }
    else 
    {
        if(fPerc > 0.0f)
        {
            const float deltaX = fromPos.mX - toPos.mX;
            const float deltaY = fromPos.mY - toPos.mY;
            return Vector3(toPos.mX + deltaX * fPerc, 
                toPos.mZ, toPos.mY + deltaY * fPerc);
        }
        else
        {
            Vector3 newPos = toPos;
            return newPos;
        }
    }

#if 0

     const float len = getSqrtLen(fromPos, toPos);
     if (len <= 0.01f)
     {
         Vector3 newPos = toPos;
         return newPos;
     }
     if (radius >= len)
     {
         Vector3 newPos = toPos;
         return newPos;   
     }
     else
     {
         const float detaX = toPos.mX - fromPos.mX;
         const float detaZ = toPos.mZ - fromPos.mZ;
         const float fPerc = len - radius / len;
         return Vector3(fromPos.mX + detaX * fPerc, toPos.mY, fromPos.mZ + detaZ * fPerc);
     }
#endif
}


bool     
MathUtil::getRandPosOfCircle(const Vector3& pPos, 
                             float& radius, Vector3& randPos)
{
    if (radius <= 0.000001F ||
        radius >= -0.000001F) 
    { 
        return false; 
    }

    if ((Int32)radius == 0) {return false; }
	float Vx = (float)(rand() % (Int32)radius) ;
	Int32 nDir = rand() % 2 ;
    if (!nDir){ Vx = -Vx ;}

	float Vy = (float)sqrt(radius * radius - Vx * Vx);
	nDir = rand() % 2 ;
    if (!nDir){ Vy = - Vy ;}

    randPos.mX = pPos.mX + (Vx / radius) * radius;
    randPos.mY = pPos.mY + (Vy / radius) * radius;
	return true;
}

float 
MathUtil::myAngle(const Vector3& pCur, const Vector3& pTar)
{
    double Sqrt = (double)sqrt((pCur.mX - pTar.mX)*(pCur.mX - pTar.mX)+
        (pCur.mZ - pTar.mZ)*(pCur.mZ - pTar.mZ));

    if(Sqrt <= 0.000001) return 0.0f;

    double fACos = (pTar.mZ - pCur.mZ)/ Sqrt;

    if (fACos > 1.0)
    {
        fACos = 0.0;
    }
    else if (fACos < -1.0)
    {
        fACos = LYNX_PI;
    }
    else
    {
        fACos = ::acos(fACos);
    }

    if (fACos != fACos)
    {
        LOG_WARN("myAngle calculate error!");
    }

    float radian = fACos;

    if (pCur.mX > pTar.mX)
    {
        radian = 2.0f * LYNX_PI - radian;
    }
    

    return radian;
}

float 
MathUtil::myAngleR(const Vector3& pCur, const Vector3& pTar)
{
    return 2 * LYNX_PI - myAngle(pCur, pTar);
}

float
MathUtil::myAngleR(const Vector2& pCur, const Vector2& pTar)
{
    return 2 * LYNX_PI - myAngle(Vector3(pCur.mX, 0.0f, pCur.mY), Vector3(pTar.mX, 0.0f, pTar.mY));
}

float 
MathUtil::myAngleL(const Vector3& pCur, const Vector3& pTar)
{
    return myAngle(pCur, pTar);
}

float 
MathUtil::myAngleL(const Vector2& pCur, const Vector2& pTar)
{
    return myAngle(Vector3(pCur.mX, 0.0f, pCur.mY), Vector3(pTar.mX, 0.0f, pTar.mY));
}

float
MathUtil::getDisOfPoint2Segment(const Vector2& point, const Vector2& segment1, const Vector2& segment2)
{
    //点和线段端点重合 
    float a,b,c;   
    a=getDistance(segment2,point);   
    if(a<=0.00001)   
        return 0.0f;   
    b=getDistance(segment1,point);   
    if(b<=0.00001)   
        return 0.0f;   
    c=getDistance(segment1,segment2);   
    if(c<=0.00001)   
        return a;//如果segment1和segment2坐标相同，则退出函数，并返回距离    
    //------------------------------    

    if(a*a>=b*b+c*c)//垂线在s1点外  
        return b;      //如果是钝角返回b    
    if(b*b>=a*a+c*c)//垂线在s2点外  
        return a;      //如果是钝角返回a    

    //垂线过线段    
    float l=(a+b+c)/2;     //周长的一半    
    float s=sqrt(l*(l-a)*(l-b)*(l-c));  //海伦公式求面积，也可以用矢量求    
    return 2*s/c;   
}