/**
 * ScapeEngine::TVector4 class
 *
 * 4D template of which IVector4 (4D int matrix), FVector4 (4D float matrix) and
 * DVector4 (4D double precision float matrix) are specialized.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __TVECTOR4_H__
#define __TVECTOR4_H__

namespace ScapeEngine {

    template <class T> class TVector4
    {
    public:
        // ----------------------------------------------------------------------------
        // The four data components of the vector
        T x, y, z, w;

        // ----------------------------------------------------------------------------
        // Predefined 3D vectors
        static const TVector4<T> UNIT_X;
        static const TVector4<T> UNIT_Y;
        static const TVector4<T> UNIT_Z;
        static const TVector4<T> UNIT_W;
        static const TVector4<T> ONES;
        static const TVector4<T> ZERO;

        // ----------------------------------------------------------------------------
        // Non-initializing constructor
        TVector4<T>() {}
        // ----------------------------------------------------------------------------
        // Component-filling constructor
        TVector4<T>(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
        // ----------------------------------------------------------------------------
        // Return the sum vector of the given 4D vector and this vector
        inline TVector4<T> operator+(const TVector4<T>& v) const
        {
            return TVector4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
        }

        // ----------------------------------------------------------------------------
        // Return the difference vector of the given 3D vector and this vector
        inline TVector4<T> operator-(const TVector4<T>& v) const
        {
            return TVector4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
        }

        // ----------------------------------------------------------------------------
        // Return the negated version of this vector
        inline TVector4<T> operator-() const { return TVector4<T>(-x, -y, -z, -w); }
        // ----------------------------------------------------------------------------
        // Return a component-wise product of the scalar and this vector
        inline TVector4<T> operator*(const T s) const { return TVector4<T>(x * s, y * s, z * s, w * s); }
        // ----------------------------------------------------------------------------
        // Return a component-wise product of the given 2D vector and this vector
        inline friend TVector4<T> operator*(const T s, const TVector4<T>& v)
        {
            return TVector4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
        }

        // ----------------------------------------------------------------------------
        // Construct a TVector3 from the x, y and z components
        inline TVector3<T>& asVector3() { return *reinterpret_cast<TVector3<T>*>(this); }
    };

    // ----------------------------------------------------------------------------
    // Define constant predined 4D vectors
    template <class T> const TVector4<T> TVector4<T>::UNIT_X(1, 0, 0, 0);
    template <class T> const TVector4<T> TVector4<T>::UNIT_Y(0, 1, 0, 0);
    template <class T> const TVector4<T> TVector4<T>::UNIT_Z(0, 0, 1, 0);
    template <class T> const TVector4<T> TVector4<T>::UNIT_W(0, 0, 0, 1);
    template <class T> const TVector4<T> TVector4<T>::ONES(1, 1, 1, 1);
    template <class T> const TVector4<T> TVector4<T>::ZERO(0, 0, 0, 0);

    // ----------------------------------------------------------------------------
    // TVector4 class specialization for integers
    typedef TVector4<int> IVector4;

    // TVector4 class specialization for single precision floats
    typedef TVector4<float> FVector4;

    // TVector4 class specialization for double precision floats
    typedef TVector4<double> DVector4;
}

#endif // __TVECTOR4_H__