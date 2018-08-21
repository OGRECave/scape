#ifndef __TQUATERNION_H__
#define __TQUATERNION_H__

namespace Engine
{
	template<class T>
	class TQuaternion
	{
	public:
		// ----------------------------------------------------------------------------
		T x, y, z, w;


		// ----------------------------------------------------------------------------
		static const TQuaternion<T> AXIS_X;
		static const TQuaternion<T> AXIS_Y;
		static const TQuaternion<T> AXIS_Z;
		static const TQuaternion<T> AXIS_W;
		static const TQuaternion<T> ONES;
		static const TQuaternion<T> IDENTITY;
		static const TQuaternion<T> ZERO;


		// ----------------------------------------------------------------------------
		TQuaternion<T>() {}


		// ----------------------------------------------------------------------------
		TQuaternion<T>(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}


		// ----------------------------------------------------------------------------
		TQuaternion<T>(const Ogre::Quaternion &quat) : x(quat.x), y(quat.y), z(quat.z), w(quat.w) {}


		// ----------------------------------------------------------------------------
		TQuaternion<T> operator+ (const TQuaternion<T>& v) const {return TQuaternion<T>(x + v.x, y + v.y, z + v.z, w + v.w);}


		// ----------------------------------------------------------------------------
		TQuaternion<T> operator- (const TQuaternion<T>& v) const {return TQuaternion<T>(x - v.x, y - v.y, z - v.z, w - v.w);}


		// ----------------------------------------------------------------------------
		TQuaternion<T> operator* (const T s) const {return TQuaternion<T>(x * s, y * s, z * s, w * s);}


		// ----------------------------------------------------------------------------
		friend TQuaternion<T> operator* (const double s, const TQuaternion<T>& v) {return TQuaternion<T>(v.x * s, v.y * s, v.z * s, v.w * s);}


		// ----------------------------------------------------------------------------
		TQuaternion<T> operator* (const TQuaternion<T>& q) const
		{
			return TQuaternion<T>(
				w * q.x + x * q.w + y * q.z - z * q.y,
				w * q.y + y * q.w + z * q.x - x * q.z,
				w * q.z + z * q.w + x * q.y - y * q.x,
				w * q.w - x * q.x - y * q.y - z * q.z
			);
		}


		// ----------------------------------------------------------------------------
		TVector3<T> operator* (const TVector3<T>& v) const
		{
			TVector3<T> qvec(x, y, z);
			TVector3<T> uv = qvec.getCross(v) * 2.0;
			TVector3<T> uuv = qvec.getCross(uv);
			return v + uv * w + uuv;
		}


		// ----------------------------------------------------------------------------
		TQuaternion<T> getNormalized() const {return *this * (1 / sqrt(x * x + y * y + z * z + w * w)); }


		// ----------------------------------------------------------------------------
		TQuaternion<T> getInverse() const {return TQuaternion<T>(-x, -y, -z, w);}


		// ----------------------------------------------------------------------------
		Ogre::Quaternion toQuaternion() const {return Ogre::Quaternion(Ogre::Real(w), Ogre::Real(x), Ogre::Real(y), Ogre::Real(z));}


		// ----------------------------------------------------------------------------
		static inline TQuaternion<T> fromVectorDifference(const TVector3<T> v1, const TVector3<T> v2)
		{
			TVector3<T> v1Norm = v1.getNormalized();
			TVector3<T> v2Norm = v2.getNormalized();

			TVector3<T> cross = v1Norm.getCross(v2Norm);
			T dot = v1Norm.getDot(v2Norm);

            T twoCos = sqrt( (1+dot)*2 );
            T invTwoCos = 1 / twoCos;

			TQuaternion<T> quat;
			quat.x = cross.x * invTwoCos;
    	    quat.y = cross.y * invTwoCos;
        	quat.z = cross.z * invTwoCos;
        	quat.w = twoCos * 0.5;

			return quat.getNormalized();						
		}


		// ----------------------------------------------------------------------------
		static TQuaternion<T> fromAngleAxisPair(double angle, const TVector3<T> &axis)
		{
			TQuaternion<T> result;
			double halfAngle = angle * 0.5;
			double sinHalfAngle = sin(halfAngle);
			result.x = sinHalfAngle * axis.x;
			result.y = sinHalfAngle * axis.y;
			result.z = sinHalfAngle * axis.z;
			result.w = cos(halfAngle);
			return result;
		}


		/* NOT YET TESTED
		// ----------------------------------------------------------------------------
		TQuaternion<T> fromEulerAngles(const TVector3<T> &eulerAngles)
		{
			TQuaternion<T> result(TQuaternion<T>::IDENTITY);
			if (!fpAlmostZero(eulerAngles.y))
			{
				TQuaternion<T> quat = TQuaternion<T>::fromAngleAxisPair(eulerAngles.y, TVector3<T>::AXIS_Y);
				result = quat;
			}
			if (!fpAlmostZero(eulerAngles.x))
			{
				TQuaternion<T> quat = TQuaternion<T>::fromAngleAxisPair(eulerAngles.x, TVector3<T>::AXIS_X);
				if (fpAlmostEqual(result.w, 1)) result = quat; else result = result * quat;
			}
			if (!fpAlmostZero(eulerAngles.z))
			{
				TQuaternion<T> quat = TQuaternion<T>::fromAngleAxisPair(eulerAngles.z, TVector3<T>::AXIS_Z);
				if (fpAlmostEqual(result.w, 1)) result = quat; else result = result * quat;
			}
			return result;
		}

		/* NOT YET TESTED
		// ----------------------------------------------------------------------------
		inline TVector3<T> toEulerAngles() const
		{
			//			|	cosy*cosr-sinp*siny*sinr	cosy*sinr+sinp*siny*cosr	-cosp*siny		|
			// mtx33 =	|	-cosp*sinr					cosp*cosr					sinp			|
			//			|	siny*cosr+sinp*cosy*sinr	siny*sinr-sinp*cosy*cosr	cosp*cosy		|

			TMatrix4<T> mtx = toMatrix4();

			T pitch, roll, yaw;

			T pitchSin = clamp<T>(mtx.col2.y, -1, 1);
			pitch = asin(pitchSin);

			if (fpAlmostEqual<T>(pitchSin, -1) || fpAlmostEqual<T>(pitchSin, 1))
			{
				// gimbal lock: use one of infinite possibilites
				roll = 0; 
				yaw = atan2(mtx.col0.z, mtx.col0.x);
			}
			else
			{
				roll = atan2(-mtx.col0.y, mtx.col1.y);
				yaw = atan2(-mtx.col2.x, mtx.col2.z);
			}

			return TVector3<T>(pitch, yaw, roll);
		}
		*/


		// ----------------------------------------------------------------------------
		inline TMatrix4<T> toMatrix4(const TVector3<T> translation = TVector3<T>::ZERO) const
		{
			//			|	1-2yy-2zz		2xy+2zw			2xz-2yw		|
			// mtx33 =	|	2xy-2zw			1-2xx-2zz		2yz+2xw		|
			//			|	2xz+2yw			2yz-2xw			1-2xx-2yy	|

			double twoX  = 2.0*x;
			double twoY  = 2.0*y;
			double twoZ  = 2.0*z;
			double twoXW = twoX*w;
			double twoYW = twoY*w;
			double twoZW = twoZ*w;
			double twoXX = twoX*x;
			double twoXY = twoY*x;
			double twoXZ = twoZ*x;
			double twoYY = twoY*y;
			double twoYZ = twoZ*y;
			double twoZZ = twoZ*z;

			return TMatrix4<T>(
				DVector4(1.0-twoYY-twoZZ,	twoXY+twoZW,		twoXZ-twoYW,		0.0),
				DVector4(twoXY-twoZW,		1.0-twoXX-twoZZ,	twoYZ+twoXW,		0.0),
				DVector4(twoXZ+twoYW,		twoYZ-twoXW,		1.0-twoXX-twoYY,	0.0),
				DVector4(translation.x,		translation.y,		translation.z,		1.0));
		}
		
	};


	// ----------------------------------------------------------------------------
	template<class T> const TQuaternion<T> TQuaternion<T>::AXIS_X	(1, 0, 0, 0);
	template<class T> const TQuaternion<T> TQuaternion<T>::AXIS_Y	(0, 1, 0, 0);
	template<class T> const TQuaternion<T> TQuaternion<T>::AXIS_Z	(0, 0, 1, 0);
	template<class T> const TQuaternion<T> TQuaternion<T>::AXIS_W	(0, 0, 0, 1);
	template<class T> const TQuaternion<T> TQuaternion<T>::IDENTITY	(0, 0, 0, 1);
	template<class T> const TQuaternion<T> TQuaternion<T>::ONES		(1, 1, 1, 1);
	template<class T> const TQuaternion<T> TQuaternion<T>::ZERO		(0, 0, 0, 0);


	// ----------------------------------------------------------------------------
	typedef TQuaternion<int> IQuaternion;
	typedef TQuaternion<float> FQuaternion;
	typedef TQuaternion<double> DQuaternion;
}

#endif // __TQUATERNION_H__