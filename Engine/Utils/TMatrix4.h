/**
 * ScapeEngine::TMatrix4 class
 *
 * 4 x 4 matrix template of which IMatrix4 (4x4 int matrix), FMatrix4 (4x4 float matrix) and
 * DMatrix4 (4x4 double precision float matrix) are specialized.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __TMATRIX4__
#define __TMATRIX4__

#include "ScapeEnginePrerequisites.h"

namespace ScapeEngine
{

template <class T> class TMatrix4
{

public:
    // ----------------------------------------------------------------------------
    // The four columns of this matrix
    TVector4<T> col0, col1, col2, col3;

    // ----------------------------------------------------------------------------
    // Predefined zero and identity matrix
    static const TMatrix4<T> IDENTITY;
    static const TMatrix4<T> ZERO;

    // ----------------------------------------------------------------------------
    // Non-initializing constructor
    inline TMatrix4<T>() {}

    // ----------------------------------------------------------------------------
    // Column-filling constructor
    inline TMatrix4<T>(const TVector4<T>& _col0, const TVector4<T>& _col1, const TVector4<T>& _col2,
                       const TVector4<T>& _col3)
        : col0(_col0), col1(_col1), col2(_col2), col3(_col3)
    {
    }

    // ----------------------------------------------------------------------------
    // Cell-filling constructor
    inline TMatrix4<T>(const Ogre::Matrix4& mtx)
        : col0(mtx[0][0], mtx[1][0], mtx[2][0], mtx[3][0]),
          col1(mtx[0][1], mtx[1][1], mtx[2][1], mtx[3][1]),
          col2(mtx[0][2], mtx[1][2], mtx[2][2], mtx[3][2]), col3(mtx[0][3], mtx[1][3], mtx[2][3], mtx[3][3])
    {
    }

    // ----------------------------------------------------------------------------
    // Convert TMatrix4 to an Ogre::Matrix4 matrix
    inline Ogre::Matrix4 toOgreMatrix4() const
    {
        return Ogre::Matrix4(static_cast<Ogre::Real>(col0.x), static_cast<Ogre::Real>(col1.x),
                             static_cast<Ogre::Real>(col2.x), static_cast<Ogre::Real>(col3.x),

                             static_cast<Ogre::Real>(col0.y), static_cast<Ogre::Real>(col1.y),
                             static_cast<Ogre::Real>(col2.y), static_cast<Ogre::Real>(col3.y),

                             static_cast<Ogre::Real>(col0.z), static_cast<Ogre::Real>(col1.z),
                             static_cast<Ogre::Real>(col2.z), static_cast<Ogre::Real>(col3.z),

                             static_cast<Ogre::Real>(col0.w), static_cast<Ogre::Real>(col1.w),
                             static_cast<Ogre::Real>(col2.w), static_cast<Ogre::Real>(col3.w));
    }

    // ----------------------------------------------------------------------------
    // Concatenate the given matrix with this matrix
    inline TMatrix4<T> operator*(const TMatrix4<T>& m2) const
    {
        TMatrix4<T> r;

        r.col0.x = col0.x * m2.col0.x + col1.x * m2.col0.y + col2.x * m2.col0.z + col3.x * m2.col0.w;
        r.col1.x = col0.x * m2.col1.x + col1.x * m2.col1.y + col2.x * m2.col1.z + col3.x * m2.col1.w;
        r.col2.x = col0.x * m2.col2.x + col1.x * m2.col2.y + col2.x * m2.col2.z + col3.x * m2.col2.w;
        r.col3.x = col0.x * m2.col3.x + col1.x * m2.col3.y + col2.x * m2.col3.z + col3.x * m2.col3.w;

        r.col0.y = col0.y * m2.col0.x + col1.y * m2.col0.y + col2.y * m2.col0.z + col3.y * m2.col0.w;
        r.col1.y = col0.y * m2.col1.x + col1.y * m2.col1.y + col2.y * m2.col1.z + col3.y * m2.col1.w;
        r.col2.y = col0.y * m2.col2.x + col1.y * m2.col2.y + col2.y * m2.col2.z + col3.y * m2.col2.w;
        r.col3.y = col0.y * m2.col3.x + col1.y * m2.col3.y + col2.y * m2.col3.z + col3.y * m2.col3.w;

        r.col0.z = col0.z * m2.col0.x + col1.z * m2.col0.y + col2.z * m2.col0.z + col3.z * m2.col0.w;
        r.col1.z = col0.z * m2.col1.x + col1.z * m2.col1.y + col2.z * m2.col1.z + col3.z * m2.col1.w;
        r.col2.z = col0.z * m2.col2.x + col1.z * m2.col2.y + col2.z * m2.col2.z + col3.z * m2.col2.w;
        r.col3.z = col0.z * m2.col3.x + col1.z * m2.col3.y + col2.z * m2.col3.z + col3.z * m2.col3.w;

        r.col0.w = col0.w * m2.col0.x + col1.w * m2.col0.y + col2.w * m2.col0.z + col3.w * m2.col0.w;
        r.col1.w = col0.w * m2.col1.x + col1.w * m2.col1.y + col2.w * m2.col1.z + col3.w * m2.col1.w;
        r.col2.w = col0.w * m2.col2.x + col1.w * m2.col2.y + col2.w * m2.col2.z + col3.w * m2.col2.w;
        r.col3.w = col0.w * m2.col3.x + col1.w * m2.col3.y + col2.w * m2.col3.z + col3.w * m2.col3.w;

        return r;
    }

    // ----------------------------------------------------------------------------
    // Transform (and normalize) a 3D vector with this homogeneous matrix.
    inline TVector3<T> operator*(const TVector3<T>& v) const
    {
        double fInvW = 1.0 / (col0.w * v.x + col1.w * v.y + col2.w * v.z + col3.w);

        return TVector3<T>((col0.x * v.x + col1.x * v.y + col2.x * v.z + col3.x) * fInvW,
                           (col0.y * v.x + col1.y * v.y + col2.y * v.z + col3.y) * fInvW,
                           (col0.z * v.x + col1.z * v.y + col2.z * v.z + col3.z) * fInvW);
    }

    // ----------------------------------------------------------------------------
    // Inverse this matrix
    inline void inverse()
    {
        TMatrix4<T> result;

        T m00 = col0.x, m01 = col1.x, m02 = col2.x, m03 = col3.x;
        T m10 = col0.y, m11 = col1.y, m12 = col2.y, m13 = col3.y;
        T m20 = col0.z, m21 = col1.z, m22 = col2.z, m23 = col3.z;
        T m30 = col0.w, m31 = col1.w, m32 = col2.w, m33 = col3.w;

        T v0 = m20 * m31 - m21 * m30;
        T v1 = m20 * m32 - m22 * m30;
        T v2 = m20 * m33 - m23 * m30;
        T v3 = m21 * m32 - m22 * m31;
        T v4 = m21 * m33 - m23 * m31;
        T v5 = m22 * m33 - m23 * m32;

        T t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
        T t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
        T t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
        T t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

        T invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

        result.col0.x = t00 * invDet;
        result.col0.y = t10 * invDet;
        result.col0.z = t20 * invDet;
        result.col0.w = t30 * invDet;

        result.col1.x = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        result.col1.y = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        result.col1.z = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        result.col1.w = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m10 * m31 - m11 * m30;
        v1 = m10 * m32 - m12 * m30;
        v2 = m10 * m33 - m13 * m30;
        v3 = m11 * m32 - m12 * m31;
        v4 = m11 * m33 - m13 * m31;
        v5 = m12 * m33 - m13 * m32;

        result.col2.x = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        result.col2.y = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        result.col2.z = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        result.col2.w = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m21 * m10 - m20 * m11;
        v1 = m22 * m10 - m20 * m12;
        v2 = m23 * m10 - m20 * m13;
        v3 = m22 * m11 - m21 * m12;
        v4 = m23 * m11 - m21 * m13;
        v5 = m23 * m12 - m22 * m13;

        result.col3.x = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        result.col3.y = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        result.col3.z = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        result.col3.w = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        *this = result;
    }

    // ----------------------------------------------------------------------------
    // Fill this matrix with a rotation-translation matrix, specified by a translation vector,
    // a yaw and a pitch angle.
    inline void fromEulerAngles(const TVector3<T>& eulerAngles, TVector3<T> translation = TVector3<T>::ZERO)
    {
        T cosp = cos(eulerAngles.x), sinp = sin(eulerAngles.x);
        T cosy = cos(eulerAngles.y), siny = sin(eulerAngles.y);
        T cosr = cos(eulerAngles.z), sinr = sin(eulerAngles.z);
        T sinpsiny = sinp * siny;
        T sinpcosy = sinp * cosy;

        TMatrix4<T> yawMat(TVector4<T>(cosy, 0, -siny, 0), TVector4<T>(0, 1, 0, 0),
                           TVector4<T>(siny, 0, cosy, 0), TVector4<T>::UNIT_W);

        TMatrix4<T> pitchMat(TVector4<T>(1, 0, 0, 0), TVector4<T>(0, cosp, sinp, 0),
                             TVector4<T>(0, -sinp, cosp, 0), TVector4<T>::UNIT_W);

        *this = yawMat * pitchMat;
        col3 = TVector4<T>(translation.x, translation.y, translation.z, 1);
    }
};

// ----------------------------------------------------------------------------
// Constant identity matrix
template <class T>
const TMatrix4<T> TMatrix4<T>::IDENTITY(TVector4<T>(1, 0, 0, 0), TVector4<T>(0, 1, 0, 0),
                                        TVector4<T>(0, 0, 1, 0), TVector4<T>(0, 0, 0, 1));

// Constant zero matrix
template <class T>
const TMatrix4<T> TMatrix4<T>::ZERO(TVector4<T>(0, 0, 0, 0), TVector4<T>(0, 0, 0, 0),
                                    TVector4<T>(0, 0, 0, 0), TVector4<T>(0, 0, 0, 0));

// ----------------------------------------------------------------------------
// TMatrix class specialization for integers
typedef TMatrix4<int> IMatrix4;

// TMatrix class specialization for single precision floats
typedef TMatrix4<float> FMatrix4;

// TMatrix class specialization for double precision floats
typedef TMatrix4<double> DMatrix4;
}
#endif // __TMATRIX4__
