#ifndef __LYNX_UTIL_LIB_MATH_H__
#define __LYNX_UTIL_LIB_MATH_H__

#include "Compat.h"

namespace Lynx
{
    const double LYNX_PI                   = 3.1415926535897932384626433832795;
    const double LYNX_MINFLOAT             = 1.0E-9;

    class _PlatformExport Vector2
    {
    public:
        Vector2() : mX(0.0f), mY(0.0f) {}
        Vector2(float x, float y) { mX = x; mY = y;}

        float mX, mY;

        float length() const;
        float normalise(float len = 1.0f);

        inline Vector2& operator = (const Vector2& rhs)
        {
            mX = rhs.mX;
            mY = rhs.mY;
            return *this;
        }

        inline bool operator == (const Vector2& rhs) const
        {
            return (mX == rhs.mX && mY == rhs.mY);
        }

        inline bool operator != (const Vector2& rhs) const
        {
            return (mX != rhs.mX || mY != rhs.mY);
        }

        inline Vector2 operator + (const Vector2& rhs) const
        {
            Vector2 sum;
            sum.mX = mX + rhs.mX;
            sum.mY = mY + rhs.mY;
            return sum;
        }

        inline Vector2& operator += (const Vector2& rhs)
        {
            mX += rhs.mX;
            mY += rhs.mY;
            return *this;
        }
        inline Vector2& operator *= (const float scalar)
        {
            mX *= scalar;
            mY *= scalar;
            return *this;
        }

        inline Vector2 operator - (const Vector2& rhs) const
        {
            Vector2 diff;
            diff.mX = mX - rhs.mX;
            diff.mY = mY - rhs.mY;
            return diff;
        }

        inline Vector2 operator * (float scalar) const
        {
            Vector2 prod;
            prod.mX = scalar * mX;
            prod.mY = scalar * mY;
            return prod;
        }


        inline friend Vector2 operator * (float scalar, const Vector2& rhs)
        {
            Vector2 prod;
            prod.mX = scalar * rhs.mX;
            prod.mY = scalar * rhs.mY;
            return prod;
        }

        static const Vector2 ZERO;
        static const Vector2 UNIT_X;
        static const Vector2 UNIT_Y;
        static const Vector2 UNIT_ALL;
    };

    template <class Type> struct IsLynxVector2 { static const bool sValue = false; };
    template <> struct IsLynxVector2<Vector2> { static const bool sValue = true; };

    class _PlatformExport Vector3
    {
    public:
        Vector3() : mX(0.0f), mY(0.0f), mZ(0.0f) {}
        Vector3(float x, float y, float z) : mX(x), mY(y), mZ(z) {}

        float mX, mY, mZ;

        float length() const;
        float normalise(float aimlen= 1.0f);

        inline Vector3& operator = (const Vector3& rhs)
        {
            mX = rhs.mX;
            mY = rhs.mY;
            mZ = rhs.mZ;
            return *this;
        }

        inline bool operator == (const Vector3& rhs) const
        {
			return FLOAT_EQUAL(mX, rhs.mX) && FLOAT_EQUAL(mY, rhs.mY) && FLOAT_EQUAL(mZ, rhs.mZ);
        }

        inline bool operator != (const Vector3& rhs) const
        {
			return !FLOAT_EQUAL(mX, rhs.mX) || !FLOAT_EQUAL(mY, rhs.mY) || !FLOAT_EQUAL(mZ, rhs.mZ);
        }

        inline Vector3 operator + (const Vector3& rhs) const
        {
            Vector3 sum;
            sum.mX = mX + rhs.mX;
            sum.mY = mY + rhs.mY;
            sum.mZ = mZ + rhs.mZ;
            return sum;
        }

        inline Vector3 operator - (const Vector3& rhs) const
        {
            Vector3 diff;
            diff.mX = mX - rhs.mX;
            diff.mY = mY - rhs.mY;
            diff.mZ = mZ - rhs.mZ;
            return diff;
        }

        inline Vector3 operator * (const float mult) const
        {
            Vector3 tmpMult;
            tmpMult.mX = mX * mult;
            tmpMult.mY = mY * mult;
            tmpMult.mZ = mZ * mult;
            return tmpMult;
        }

        inline float operator*(const Vector3& pt) const
        {
            return mX*pt.mX + mY*pt.mY + mZ*pt.mZ;
        }

        inline Vector3 operator / (const float scalar) const
        {
            if (FLOAT_EQUAL(scalar, 0.0f))
            {
                return Vector3(mX, mY, mZ);
            } 
            
            float inv = 1.0f / scalar;

            return Vector3(
                mX * inv,
                mY * inv,
                mZ * inv);
        }

        inline Vector3 operator / (const Vector3& rhs) const
        {
            return Vector3(
                mX / rhs.mX,
                mY / rhs.mY,
                mZ / rhs.mZ);
        }

		inline Vector3 revese() const
		{
			return Vector3(- mX, - mY, - mZ);
        }

        inline Vector3 unitCross(const Vector3& pt) const
        {
            Vector3 cross(
                mY*pt.mZ - mZ*pt.mY,
                mZ*pt.mX - mX*pt.mZ,
                mX*pt.mY - mY*pt.mX);

            float length = cross.length();
            if (length > 1e-06f)
                return cross/length;
            else
                return Vector3(0.0f,0.0f,0.0f);
        }

        static const Vector3 ZERO;
        static const Vector3 MAX;
        static const Vector3 UNIT_X;
        static const Vector3 UNIT_Y;
        static const Vector3 UNIT_Z;
        static const Vector3 UNIT_ALL;
    };

	template <class Type> struct IsLynxVector3 { static const bool sValue = false; };
    template <> struct IsLynxVector3<Vector3> { static const bool sValue = true; };

    class _PlatformExport Vector4
    {
    public:
        Vector4() : mX(0.0f), mY(0.0f), mZ(0.0f), mW(0.0f) {}
        Vector4(float x, float y, float z, float w) : mX(x), mY(y), mZ(z), mW(w) {}

        float mX, mY, mZ, mW;

        inline Vector4& operator = (const Vector4& rhs)
        {
            mX = rhs.mX;
            mY = rhs.mY;
            mZ = rhs.mZ;
            mW = rhs.mW;
            return *this;
        }

        inline bool operator == (const Vector4& rhs) const
        {
			return FLOAT_EQUAL(mX, rhs.mX) && FLOAT_EQUAL(mY, rhs.mY) && FLOAT_EQUAL(mZ, rhs.mZ) && FLOAT_EQUAL(mW, rhs.mW);
        }

        inline bool operator != (const Vector4& rhs) const
        {
			return !FLOAT_EQUAL(mX, rhs.mX) || !FLOAT_EQUAL(mY, rhs.mY) || !FLOAT_EQUAL(mZ, rhs.mZ) || !FLOAT_EQUAL(mW, rhs.mW);
        }

        inline Vector4 operator + (const Vector4& rhs) const
        {
            Vector4 sum;
            sum.mX = mX + rhs.mX;
            sum.mY = mY + rhs.mY;
            sum.mZ = mZ + rhs.mZ;
            sum.mW = mW + rhs.mW;
            return sum;
        }

        inline Vector4 operator - (const Vector4& rhs) const
        {
            Vector4 diff;
            diff.mX = mX - rhs.mX;
            diff.mY = mY - rhs.mY;
            diff.mZ = mZ - rhs.mZ;
            diff.mW = mW - rhs.mW;
            return diff;
        }

        inline Vector4 operator * (const float mult) const
        {
            Vector4 tmpMult;
            tmpMult.mX = mX * mult;
            tmpMult.mY = mY * mult;
            tmpMult.mZ = mZ * mult;
            tmpMult.mW = mW * mult;
            return tmpMult;
        }

        inline float dotProduce(const Vector4& pt) const
        {
            return mX*pt.mX + mY*pt.mY + mZ*pt.mZ + mW*pt.mW;
        }

        inline Vector4 operator / (const float scalar) const
        {
            if (FLOAT_EQUAL(scalar, 0.0f))
            {
                return Vector4(mX, mY, mZ, mW);
            } 
            
            float inv = 1.0f / scalar;

            return Vector4(
                mX * inv,
                mY * inv,
                mZ * inv,
                mW * inv);
        }

        inline Vector4 operator / (const Vector4& rhs) const
        {
            return Vector4(
                mX / rhs.mX,
                mY / rhs.mY,
                mZ / rhs.mZ,
                mW / rhs.mW);
        }

		inline Vector4 revese() const
		{
			return Vector4(- mX, - mY, - mZ, - mW);
        }

        static const Vector4 ZERO;
        static const Vector4 MAX;
        static const Vector4 UNIT_X;
        static const Vector4 UNIT_Y;
        static const Vector4 UNIT_Z;
        static const Vector4 UNIT_W;
        static const Vector4 UNIT_ALL;
    };

    template <class Type> struct IsLynxVector4 { static const bool sValue = false; };
    template <> struct IsLynxVector4<Vector4> { static const bool sValue = true; };

    /*
	class _PlatformExport Color
    {
    public:
        Color() : mR(0.0f), mG(0.0f), mB(0.0f), mA(0.0f) {}
        Color(float r, float g, float b, float a) : mR(r), mG(g), mB(b), mA(a) {}

        float mR, mG, mB, mA;

        inline Color& operator = (const Color& rhs)
        {
            mR = rhs.mR;
            mG = rhs.mG;
            mB = rhs.mB;
            mA = rhs.mA;
            return *this;
        }

        inline bool operator == (const Color& rhs) const
        {
            return (mR == rhs.mR && mG == rhs.mG && mB == rhs.mB && mA == rhs.mA);
        }

        inline bool operator != (const Color& rhs) const
        {
            return (mR != rhs.mR || mG != rhs.mG || mB != rhs.mB || mA != rhs.mA);
        }

        inline Color operator + (const Color& rhs) const
        {
            Color sum;

            sum.mR = mR + rhs.mR;
            sum.mG = mG + rhs.mG;
            sum.mB = mB + rhs.mB;
            sum.mA = mA + rhs.mA;

            return sum;
        }

        inline Color operator - (const Color& rhs) const
        {
            Color diff;

            diff.mR = mR - rhs.mR;
            diff.mG = mG - rhs.mG;
            diff.mB = mB - rhs.mB;
            diff.mA = mA - rhs.mA;

            return diff;
        }

        inline friend Color operator * (float scalar, const Color& rhs)
        {
            Color prod;

            prod.mR = scalar * rhs.mR;
            prod.mG = scalar * rhs.mG;
            prod.mB = scalar * rhs.mB;
            prod.mA = scalar * rhs.mA;

            return prod;
        }

        inline Color operator / (float scalar) const
        {
            Color prod;
            float inv = 1.0f / scalar;

            prod.mR = inv * mR;
            prod.mG = inv * mG;
            prod.mB = inv * mB;
            prod.mA = inv * mA;

            return prod;
        }
        inline Color operator * (float scalar) const
        {
            Color prod;

            prod.mR = scalar * mR;
            prod.mG = scalar * mG;
            prod.mB = scalar * mB;
            prod.mA = scalar * mA;

            return prod;
        }
    };

    template <class Type> struct IsLynxColor { static const bool sValue = false; };
    template <> struct IsLynxColor<Color> { static const bool sValue = true; };
	*/

    //struct Rect
    //{
    //    Rect(): mCosDir(0.0F), mSinDir(0.0F), mLen(0.0F), mWidth(0.0F) {}
    //    ~Rect() {}

    //    void initial(float len, float width, const Vector3& ori, const Vector3& centerPos)
    //    {
    //        mLen = len;
    //        mWidth = width;
    //        mOri = ori;
    //        mCenterPos = centerPos;
    //        float dir = oriToDir(ori);
    //        mCosDir = ::cos(dir);
    //        mSinDir = ::sin(dir);
    //    }

    //    bool isInRect(const Lynx::Vector3& pos,  double rang = 0.000001)
    //    {
    //        // 平移坐标系到矩形中心点位置
    //        float xCenter = pos.mX - mCenterPos.mX;
    //        float zCenter = pos.mZ - mCenterPos.mZ;

    //        // 旋转坐标系dir弧度
    //        //x'=x*cos(n)+z*sin(n)
    //        //z'=z*cos(n)-x*sin(n)
    //        //float cosDir = ::cos(dir);
    //        //float sinDir = ::sin(dir);
    //        double x = xCenter * mCosDir + zCenter * mSinDir;
    //        double z = zCenter * mCosDir - xCenter * mSinDir;
    //        return ((2 * ::fabs(x) <= mWidth + rang) && (2 * ::fabs(z) <= mLen + rang));        
    //    }

    //private:
    //    float oriToDir(const Vector3& ori)
    //    {
    //        float result = ::asin(ori.mZ / ::sqrt(ori.mZ * ori.mZ + ori.mX * ori.mX));
    //        return result;
    //    }

    //    float           mLen;
    //    float           mWidth;
    //    Vector3         mOri;
    //    Vector3         mCenterPos;
    //    float           mCosDir;
    //    float           mSinDir;
    //};

    class _PlatformExport Plane
    {
    public:
        Plane() : mNormal(0,1.0f,0), mConstant(0){
            
        }
        Plane(float a, float b, float c, float d) : mNormal(a, b, c), mConstant(d) {}
        Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2)
        {
            Vector3 dif1 = p1 - p0;
            Vector3 dif2 = p2 - p1;
            mNormal = dif1.unitCross(dif2);
            mConstant = mNormal * p0;
        }

        inline float getConstant() const
        {
            return mConstant;
        }

        inline const Vector3& getNormal() const
        {
            return mNormal;
        }

    private:
        Vector3 mNormal;
        float mConstant;
    };

    class _PlatformExport Quaternion
    {
    public:
        float mW,mX,mY,mZ;
        Quaternion(float radian, const Vector3& axis)
        {
            float fHalfAngle = 0.5f * radian;
            float fSin = sin(fHalfAngle);
            mW = cos(fHalfAngle);
            mX = fSin * axis.mX;
            mY = fSin * axis.mY;
            mZ = fSin * axis.mZ;
        }
    protected:
    private:
    };
    

    struct _PlatformExport MathUtil
    {
        // 计算两点之间的距离
        static float getDistance(const Vector2& pos1, const Vector2& pos2);
        static float getDistance(const Vector3& pos1, const Vector3& pos2);

        // 计算两点之间的距离平方
        static float getDistanceSqrt(const Vector2& pos1, const Vector2& pos2);
        static float getDistanceSqrt(const Vector3& pos1, const Vector3& pos2);

        // 计算量个向量的点积
        static float dotProduce(const Vector2& v1, const Vector2& v2);
        static float dotProduce(const Vector3& v1, const Vector3& v2);
        // 计算两矢量相对于Y轴的旋转角度, 以Z轴为0度
        //           ->x
        //                   o pos1
        //           |       |\
        //        z  v       |  \
        //                   |    o pos2
        //
        // Return [0 ~ 2pi)
        static float getYAngle(const Vector2& pos1, const Vector2& pos2);

        // 计算量矢量点之间的中心点        
        //            ->x
        //                    o pos2
        //           |       /    
        //        z  v      o <- This point!    
        //                 /    
        //                o 
        //              pos1        
        static Vector3 getCenter(const Vector3& pos1, const Vector3& pos2);

        // 计算v1相对于从pos1与pos2之间的直线的镜像点
        //           ->x
        //                        o pos2
        //           |  v1       /    
        //        z  v    \    / 
        //                  \/     
        //                 /  \
        //                o     \ 
        //             pos1      v2  <- This point!   
        static Vector2 getReflect(const Vector2& pos1, const Vector2& pos2, const Vector2& v1);
        // 取两点平面坐标的长度,不包含高度
        static float getSqrtLen(const Vector3& pos1, const Vector3& pos2);
        // 取两点之间的粗略距离,不包括高度
        static float getRoughLen(const Vector3& pos1, const Vector3& pos2);
        // 用于计算和对象交互，取周围一个交互点
        static Vector3 getInteractPos(const Vector3& from, const Vector3& toPos, float radius);
        // 生成以pPos为圆心，radius为半径的圆周上的随机一点
        static bool getRandPosOfCircle(const Vector3& pPos, float& radius, Vector3& randPos);

        static float myAngle(const Vector3& pCur, const Vector3& pTar);
        static float myAngleR(const Vector3& pCur, const Vector3& pTar);
        static float myAngleR(const Vector2& pCur, const Vector2& pTar);
        static float myAngleL(const Vector3& pCur, const Vector3& pTar);
        static float myAngleL(const Vector2& pCur, const Vector2& pTar);

        // 点到线段的距离
        static float getDisOfPoint2Segment(const Vector2& point, const Vector2& segment1, const Vector2& segment2);

        // 三角函数
        static float sin(float x) { return ::sin(x); }
        static float cos(float x) { return ::cos(x); }

        static Vector2 transVector(const Vector3& vector3)
        {
            return Vector2(vector3.mX, vector3.mZ);
        }

        static Vector3 transVector(const Vector2& vector2)
        {
            return Vector3(vector2.mX, 0.0f, vector2.mY);
        }

        // 在2个值之间随机取一个值.
        static float unitRandom ()
        {
            return float( rand() ) / float( RAND_MAX );
        }

        static float rangeRandom(float low, float high)
        {
            return (high-low)*unitRandom() + low;
        }
    };

    template <typename T, typename U, typename V>
    inline void clamp(T& v, const U& min, const V& max)
    {
        v = (v < min) ? min : v;
        v = (v > max) ? max : v;
    }
} // namespace Lynx

#endif // __LYNX_UTIL_LIB_MATH_H__
