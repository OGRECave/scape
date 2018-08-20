/**
 * ScapeEngine::TVector3 class
 *
 * 3D template of which IVector3 (3D int matrix), FVector3 (3D float matrix) and
 * DVector3 (3D double precision float matrix) are specialized.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __TVECTOR3_H__
#define __TVECTOR3_H__

namespace ScapeEngine {

    template <class T> class TVector3
    {
    public:
        // ----------------------------------------------------------------------------
        // The three data components of the vector
        T x, y, z;

        // ----------------------------------------------------------------------------
        // Predefined 3D vectors
        static const TVector3<T> UNIT_X;
        static const TVector3<T> UNIT_Y;
        static const TVector3<T> UNIT_Z;
        static const TVector3<T> ONES;
        static const TVector3<T> ZERO;

        // ----------------------------------------------------------------------------
        // Non-initializing constructor
        TVector3<T>() {}
        // ----------------------------------------------------------------------------
        // Component-filling constructor
        TVector3<T>(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
        // ----------------------------------------------------------------------------
        // Construct TVector based on a Ogre::Vector3
        TVector3<T>(const Ogre::Vector3& v) : x(v.x), y(v.y), z(v.z) {}
        // ----------------------------------------------------------------------------
        // Return the sum vector of the given 3D vector and this vector
        TVector3<T> operator+(const TVector3& v) const { return TVector3(x + v.x, y + v.y, z + v.z); }
        // ----------------------------------------------------------------------------
        // Return the difference vector of the given 3D vector and this vector
        TVector3<T> operator-(const TVector3& v) const { return TVector3(x - v.x, y - v.y, z - v.z); }
        // ----------------------------------------------------------------------------
        // Return the negated version of this vector
        TVector3<T> operator-() const { return TVector3(-x, -y, -z); }
        // ----------------------------------------------------------------------------
        // Return a component-wise product of the scalar and this vector
        TVector3<T> operator*(const T s) const { return TVector3(x * s, y * s, z * s); }
        // ----------------------------------------------------------------------------
        // Return a component-wise product of the given 2D vector and this vector
        friend TVector3<T> operator*(const T s, const TVector3<T>& v) { return TVector3(v.x * s, v.y * s, v.z * s); }
        // ----------------------------------------------------------------------------
        // Get the dot product of the given vector and this vector
        T dot(const TVector3<T>& v) const { return x * v.x + y * v.y + z * v.z; }
        // ----------------------------------------------------------------------------
        // Get the cross product of the given vector and this vector
        TVector3<T> cross(const TVector3<T>& v) const
        {
            return TVector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        }

        // ----------------------------------------------------------------------------
        // Get the length of this vector
        T length() const { return sqrt(x * x + y * y + z * z); }
        // ----------------------------------------------------------------------------
        // Normalize this vector
        void normalise() { *this = operator*(1 / length()); }
        // ----------------------------------------------------------------------------
        // Construct an Ogre::Vector3 vector from this vector
        Ogre::Vector3 toOgreVector3() const { return Ogre::Vector3(Ogre::Real(x), Ogre::Real(y), Ogre::Real(z)); }
    };

    // ----------------------------------------------------------------------------
    // Define constant predined 3D vectors
    template <class T> const TVector3<T> TVector3<T>::UNIT_X(1, 0, 0);
    template <class T> const TVector3<T> TVector3<T>::UNIT_Y(0, 1, 0);
    template <class T> const TVector3<T> TVector3<T>::UNIT_Z(0, 0, 1);
    template <class T> const TVector3<T> TVector3<T>::ONES(1, 1, 1);
    template <class T> const TVector3<T> TVector3<T>::ZERO(0, 0, 0);

    // ----------------------------------------------------------------------------
    // TVector3 class specialization for integers
    typedef TVector3<int> IVector3;

    // TVector3 class specialization for single precision floats
    typedef TVector3<float> FVector3;

    // TVector3 class specialization for double precision floats
    typedef TVector3<double> DVector3;
}

#endif // __TVECTOR3_H__