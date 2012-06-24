/**
 * ScapeEngine::TVector2 class
 *
 * 2D template of which IVector2 (2D int matrix), FVector2 (2D float matrix) and
 * DVector2 (2D double precision float matrix) are specialized.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __TVECTOR2_H__
#define __TVECTOR2_H__


namespace ScapeEngine
{

	template<class T>
	class TVector2
	{

	public:
		// ----------------------------------------------------------------------------
		// The two data components of the vector
		T x, y;


		// ----------------------------------------------------------------------------
		// Predefined 2D vectors
		static const TVector2<T> AXIS_X;
		static const TVector2<T> AXIS_Y;
		static const TVector2<T> ONES;
		static const TVector2<T> ZERO;


		// ----------------------------------------------------------------------------
		// Non-initializing constructor
		TVector2<T>() {}


		// ----------------------------------------------------------------------------
		// Component-filling constructor
		TVector2<T>(T _x, T _y) : 
			x(_x), y(_y)
		{
		}


		// ----------------------------------------------------------------------------
		// Construct TVector based on a Ogre::Vector2
		TVector2<T>(const Ogre::Vector2 &v) : x(v.x), y(v.y) {}


		// ----------------------------------------------------------------------------
		// Return the sum vector of the given 2D vector and this vector
		TVector2<T> operator+(const TVector2& v) const {return TVector2(x + v.x, y + v.y);}


		// ----------------------------------------------------------------------------
		// Return the difference vector of the given 2D vector and this vector
		TVector2<T> operator-(const TVector2& v) const {return TVector2(x - v.x, y - v.y);}


		// ----------------------------------------------------------------------------
		// Return the negated version of this vector
		TVector2<T> operator-() const {return TVector2(-x, -y);}


		// ----------------------------------------------------------------------------
		// Return a component-wise product of the scalar and this vector
		TVector2<T> operator*(const T s) const {return TVector2(x * s, y * s);}


		// ----------------------------------------------------------------------------
		// Return a component-wise product of the given 2D vector and this vector
		friend TVector2<T> operator*(const T s, const TVector2<T>& v) {return TVector3(v.x * s, v.y * s);}

	};

	// ----------------------------------------------------------------------------
	// Define constant predined 2D vectors
	template<class T> const TVector2<T> TVector2<T>::AXIS_X	(1, 0);
	template<class T> const TVector2<T> TVector2<T>::AXIS_Y	(1, 0);
	template<class T> const TVector2<T> TVector2<T>::ONES	(1, 1);
	template<class T> const TVector2<T> TVector2<T>::ZERO	(0, 0);


	// ----------------------------------------------------------------------------
	// TVector2 class specialization for integers
	typedef TVector2<int> IVector2;

	// TVector2 class specialization for single precision floats
	typedef TVector2<float> FVector2;

	// TVector2 class specialization for double precision floats
	typedef TVector2<double> DVector2;
}

#endif // __TVECTOR2_H__