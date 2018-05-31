/**
 * ScapeEngine::Utils class
 *
 * Collection of useful constants, macros, templates and functions
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __UTILS_H__
#define __UTILS_H__

#include "TVector2.h"
#include "TVector3.h"
#include "TVector4.h"
#include "TMatrix4.h"

// Delete and reset pointer
#define SAFE_DELETE(a)	{delete (a); a = NULL;}

// Dispose and reset pointer
#define SAFE_DISPOSE(a) {if (a != NULL) {(a)->dispose(); (a) = NULL;}}

// log(0.5)/log(e)
#define LN_HALF			0.69314718055994530941723212145818

// Signed 8 bytes
#define SQWORD			long long

// Unsigned 8 bytes
#define QWORD			unsigned SQWORD

// Signed 4 bytes
#define SDWORD			int

// Unsigned 4 bytes
#define DWORD			unsigned SDWORD

// max difference in single precision float units for fpAlmostZero() and fpAlmostEqual()
#define FLOAT_ALMOSTEQUAL_MAXULPS	20

// max difference in double precision float units for fpAlmostZero() and fpAlmostEqual()
#define DOUBLE_ALMOSTEQUAL_MAXULPS	20


#define STRINGIFY(a) #a
#define ADD_STRINGENUM(map, enumprefix, val) {map[#val] = enumprefix##val;}

// ----------------------------------------------------------------------------
namespace ScapeEngine
{
	typedef std::map<string, int> StringEnumMap;
	typedef std::list<string> StringList;
	typedef std::map<string, string> StringStringMap;
	typedef std::map<string, StringList> StringStringListMap;
	typedef std::map<string, std::pair<string, string>> StringStringStringPairMap;


	namespace Utils
	{
		// ----------------------------------------------------------------------------
		inline int findEnumFromStringEnumMap(const StringEnumMap& stringEnumMap, const string& name, int notFoundValue = -1)
		{
			StringEnumMap::const_iterator it = stringEnumMap.find(name); 
			if (it != stringEnumMap.end()) return it->second;
			return notFoundValue;
		}


		// ----------------------------------------------------------------------------
		// Get the smaller of the two values/objects
		template<class T> 
		inline const T &min(const T &left, const T &right)
		{
			return (left < right ? left : right);
		}


		// ----------------------------------------------------------------------------
		// Get the larger of the two values/objects
		template<class T> 
		inline const T &max(const T &left, const T &right)
		{
			return (left > right ? left : right);
		}


		// ----------------------------------------------------------------------------
		// Clamp value/object between min and max.
		// pre: min <= max
		template<class T> 
		inline const T &clamp(const T &value, const T &min, const T &max)
		{
			return (value > max ? max : (value < min ? min : value));
		}

		
		// ----------------------------------------------------------------------------
		// Linearly interpolates src and dest.
		// blendFactor = 0 returns src. blendFactor = 1 returns dest.
		template<class T> 
		inline T lerp(const T &src, const T &dest, Ogre::Real blendFactor)
		{
			return src + (dest - src) * blendFactor;
		}

		
		// ----------------------------------------------------------------------------
		// Get remaining halflife ratio after deltaTime given the halfLife
		template<class T> 
		inline T getHalfLifeBlendFactor(T halfLife, T deltaTime)
		{
			return (T)1.0 - expf((T)-LN_HALF * deltaTime / halfLife);
		}

		
		// ----------------------------------------------------------------------------
		// Calculate Euler yaw and pitch angles between two vectors
		template<class T>
		TVector3<T> getYawPitchFromVectorDifference(const TVector3<T> &first, const TVector3<T> &second)
		{
			TVector3<T> eulerAngles;
			eulerAngles.z = 0;

			T projYSin = first.z * second.x - first.x * second.z;
			T projYCos = first.x * second.x + first.z * second.z;
			eulerAngles.y = atan2(projYSin, projYCos);

			T firstZ2D = sqrt(first.x * first.x + first.z * first.z);
			T secondZ2D = sqrt(second.x * second.x + second.z * second.z);
			T projXSin = firstZ2D * second.y - first.y * secondZ2D;
			T projXCos = first.y * second.y + firstZ2D * secondZ2D;
			eulerAngles.x = atan2(projXSin, projXCos);

			return eulerAngles;
		}

		
		// ----------------------------------------------------------------------------
		// Return absolute value of the given integer
		// pre: T is an integer type (e.g. int, short, char)
		template<class T>
		inline T intAbs(T x)
		{
			const T shifts = ((sizeof(T) << 3) - 1);
			T shifted = x >> shifts;
			return (x ^ shifted) - shifted;
		}


		// ----------------------------------------------------------------------------
		template<class T>
		inline bool fpAlmostZero(T x)
		{
			if (sizeof(T) == sizeof(float)) 
				return intAbs(*(SDWORD*)&x) <= FLOAT_ALMOSTEQUAL_MAXULPS;
			else 
				return intAbs(*(SQWORD*)&x) <= DOUBLE_ALMOSTEQUAL_MAXULPS;
		}

		
		// ----------------------------------------------------------------------------
		template<class T>
		inline bool fpAlmostEqual(T a, T b)
		{
			if (sizeof(T) == sizeof(float))
				return intAbs(*(SDWORD*)&a - *(SDWORD*)&b) <= FLOAT_ALMOSTEQUAL_MAXULPS;
			else
				return intAbs(*(SQWORD*)&a - *(SQWORD*)&b) <= DOUBLE_ALMOSTEQUAL_MAXULPS;
		}


		// ----------------------------------------------------------------------------
		template<class T1, class T2> 
		struct MapStruct 
		{
			typedef std::map<T1, T2> MapType; 
			MapType map;
		};


		// ----------------------------------------------------------------------------
		template<class MapStructType, class ValueType>
		class MapStructReadIterator
		{
		public:
			MapStructReadIterator() : mValid(false) {}
			MapStructReadIterator(const MapStructType& mapStruct) : mValid(true), mBeginIt(mapStruct.map.begin()), mCurrentIt(mapStruct.map.begin()), mEndIt(mapStruct.map.end()) {}
			const typename MapStructType::MapType::key_type& getKey() const {return mCurrentIt->first;}
			ValueType getValue() const {return *mCurrentIt->second;}
			void next() {++mCurrentIt;}
			void rewind() {mCurrentIt = mBeginIt;}
			bool isEnd() const {return !mValid || mCurrentIt == mEndIt;}

		protected:
			typename MapStructType::MapType::const_iterator mBeginIt, mCurrentIt, mEndIt;
			bool mValid;
		};


		// ----------------------------------------------------------------------------
		class MooreNeighborhood
		{
		public:
			enum EDirection
			{
				DIRECTION_N, DIRECTION_NE, DIRECTION_E, DIRECTION_SE, 
				DIRECTION_S, DIRECTION_SW, DIRECTION_W, DIRECTION_NW,
				DIRECTION_ENUM_LENGTH
			};

			static inline EDirection oppositeDirection(EDirection direction) {return mOppositeDirection[(int)direction];}
			static inline int directionToX(EDirection direction) {return mDirectionToX[(int)direction];}
			static inline int directionToY(EDirection direction) {return mDirectionToY[(int)direction];}
			static inline const char* directionToName(EDirection direction) {return mDirectionToName[(int)direction];}

		private:
			static const EDirection mOppositeDirection[DIRECTION_ENUM_LENGTH];
			static const int mDirectionToX[DIRECTION_ENUM_LENGTH];
			static const int mDirectionToY[DIRECTION_ENUM_LENGTH];
			static const char* mDirectionToName[DIRECTION_ENUM_LENGTH];
		};

		// ----------------------------------------------------------------------------
		class VonNeumannNeighborhood
		{
		public:
			enum EDirection	{DIRECTION_N, DIRECTION_E, DIRECTION_S, DIRECTION_W, DIRECTION_ENUM_LENGTH};

			static inline EDirection oppositeDirection(EDirection direction)  {return mOppositeDirection[(int)direction];}
			static inline int directionToX(EDirection direction) {return mDirectionToX[(int)direction];}
			static inline int directionToY(EDirection direction) {return mDirectionToY[(int)direction];}
			static inline const char* directionToName(EDirection direction) {return mDirectionToName[(int)direction];}

		private:
			static const EDirection mOppositeDirection[DIRECTION_ENUM_LENGTH];
			static const int mDirectionToX[DIRECTION_ENUM_LENGTH];
			static const int mDirectionToY[DIRECTION_ENUM_LENGTH];
			static const char* mDirectionToName[DIRECTION_ENUM_LENGTH];
		};

		// ----------------------------------------------------------------------------
		template<class T> 
		Ogre::TRect<T> unionTRect(const Ogre::TRect<T>& a, const Ogre::TRect<T>& b)
		{
			if (a.width() == 0 || a.height() == 0) return b;
			if (b.width() == 0 || b.height() == 0) return a;

			Ogre::TRect<T> rect;
			rect.left = std::min(a.left, b.left);
			rect.right = std::max(a.right, b.right);
			rect.top = std::min(a.top, b.top);
			rect.bottom = std::max(a.bottom, b.bottom);

			return rect;
		}

		// ----------------------------------------------------------------------------
		template<class T> 
		Ogre::TRect<T> intersectTRect(const Ogre::TRect<T>& a, const Ogre::TRect<T>& b)
		{
			Ogre::TRect<T> rect;
			rect.left = std::max(a.left, b.left);
			rect.right = std::min(a.right, b.right);
			rect.top = std::max(a.top, b.top);
			rect.bottom = std::min(a.bottom, b.bottom);

			rect.left = std::min(rect.left, rect.right);
			rect.top = std::min(rect.top, rect.bottom);

			return rect;
		}

		// ----------------------------------------------------------------------------
		template<class T> 
		Ogre::TRect<T> subtractTRect(const Ogre::TRect<T>& a, const Ogre::TRect<T>& b)
		{
			Ogre::TRect<T> rect(a);
			if (rect.top >= b.top && rect.bottom <= b.bottom) 
			{
				if (rect.left >= b.left && rect.right <= b.right) rect.right = rect.left;
				else if (rect.left <= b.left && rect.right <= b.right && rect.right > b.left) rect.right = b.left;
				else if (rect.left >= b.left && rect.right >= b.right && rect.left < b.right) rect.left = b.right;
			}
			else if (rect.left >= b.left && rect.right <= b.right) 
			{
				if (rect.top <= b.top && rect.bottom <= b.right && rect.bottom > b.top) rect.bottom = b.top;
				else if (rect.top >= b.top && rect.bottom >= b.bottom && rect.top < b.bottom) rect.top = b.bottom;
			}
			return rect;
		}

		// ----------------------------------------------------------------------------
		inline Ogre::Box rectToBox(const Ogre::Rect& rect)
		{
			return Ogre::Box(rect.left, rect.top, 0, rect.right, rect.bottom, 1);
		}

		// ----------------------------------------------------------------------------
		inline void translateRect(Ogre::Rect& rect, int offsetX = 0, int offsetY = 0)
		{
			rect.left += offsetX;
			rect.top += offsetY;
			rect.right += offsetX;
			rect.bottom += offsetY;
		}

		// ----------------------------------------------------------------------------
		inline Ogre::Rect translatedRect(const Ogre::Rect& rect, int offsetX = 0, int offsetY = 0)
		{
			Ogre::Rect out = rect;
			translateRect(out, offsetX, offsetY);
			return out;
		}

		// ----------------------------------------------------------------------------
		inline void translateBox(Ogre::Box& box, int offsetX = 0, int offsetY = 0, int offsetZ = 0)
		{
			box.left += offsetX;
			box.top += offsetY;
			box.front += offsetZ;
			box.right += offsetX;
			box.bottom += offsetY;
			box.back += offsetZ;
		}

		// ----------------------------------------------------------------------------
		inline Ogre::Rect boxToRect(const Ogre::Box& box)
		{
			assert(box.getDepth() == 1);
			return Ogre::Rect(box.left, box.top, box.right, box.bottom);
		}

		Ogre::Vector4 getCatmullRomeSplineWeights(Ogre::Real param);

		// ----------------------------------------------------------------------------
		Ogre::PixelBox clipPixelBox(const Ogre::PixelBox& pixelBox, const Ogre::Rect& relativeRect, int offsetX = 0, int offsetY = 0);


		// ----------------------------------------------------------------------------
		Ogre::Real getTypicalVertexElementRangeMin(Ogre::VertexElementType type);


		// ----------------------------------------------------------------------------
		Ogre::Real getTypicalVertexElementRangeMax(Ogre::VertexElementType type);

		// ----------------------------------------------------------------------------
		bool bindShaderCustomAutoConstant(Ogre::Technique* technique, size_t constantIndex, const string& constantName);

		// ----------------------------------------------------------------------------
		bool readResource(const string& fileName, const string& groupName, string& contents);

		// ----------------------------------------------------------------------------
		typedef long long GUID;
		GUID createGUID();


		// ----------------------------------------------------------------------------
		Ogre::uint32 randHash(int in);


		// ----------------------------------------------------------------------------
		extern const string emptyString;


		// ----------------------------------------------------------------------------
		void writeBinaryFile(const string& fileName, const void* data, int bytes, bool append = false);


		// ----------------------------------------------------------------------------
		std::pair<void*, size_t> readBinaryFile(const string& fileName);

		// ----------------------------------------------------------------------------
		string makeUniqueName(const string& baseName, const std::list<string>& existingNames);

		// ----------------------------------------------------------------------------
		inline void flipByteOrder(void *data, size_t totalBytes)
		{
			char left;
			char *leftPtr = (char*)data;
			char *rightPtr = (char*)data + (totalBytes - 1);
			char *endPtr = (char*)data + (totalBytes >> 1);
			for (; leftPtr <= endPtr; ++leftPtr, --rightPtr)
			{
				left = *leftPtr;
				*leftPtr = *rightPtr;
				*rightPtr = left;
			}
		}

		// ----------------------------------------------------------------------------
		inline void flipByteOrder(unsigned char *data) {flipByteOrder(data, sizeof(unsigned char));}
		inline void flipByteOrder(unsigned short *data) {flipByteOrder(data, sizeof(unsigned short));}
		inline void flipByteOrder(unsigned int *data) {flipByteOrder(data, sizeof(unsigned int));}
		inline void flipByteOrder(unsigned long *data) {flipByteOrder(data, sizeof(unsigned long));}
		inline void flipByteOrder(unsigned long long *data) {flipByteOrder(data, sizeof(unsigned long long));}
		inline void flipByteOrder(signed char *data) {flipByteOrder(data, sizeof(signed char));}
		inline void flipByteOrder(signed short *data) {flipByteOrder(data, sizeof(signed short));}
		inline void flipByteOrder(signed int *data) {flipByteOrder(data, sizeof(signed int));}
		inline void flipByteOrder(signed long *data) {flipByteOrder(data, sizeof(signed long));}
		inline void flipByteOrder(signed long long *data) {flipByteOrder(data, sizeof(signed long long));}


		// ----------------------------------------------------------------------------
		inline void flipSubWordOrder(void *data, size_t totalWords, size_t wordBytes)
		{
			char left;
			char *leftPtr = (char*)data;
			char *rightPtr = (char*)data + (wordBytes - 1);
			char *endInnerPtr = (char*)data + (wordBytes >> 1);
			char *endOuterPtr = (char*)data + (totalWords * wordBytes);
			size_t step = wordBytes >> 1;
			for (; leftPtr < endOuterPtr; leftPtr += step, rightPtr += step + wordBytes, endInnerPtr += wordBytes)
			{
				for (; leftPtr < endInnerPtr; ++leftPtr, --rightPtr)
				{
					left = *leftPtr;
					*leftPtr = *rightPtr;
					*rightPtr = left;
				}
			}
		}

		// ----------------------------------------------------------------------------
		inline void flipSuperWordOrder(void *data, size_t totalWords, size_t wordBytes)
		{
			char left;
			char *leftPtr = (char*)data;
			char *rightPtr = (char*)data + ((totalWords - 1) * wordBytes);
			char *endInnerPtr = leftPtr + wordBytes;
			char *endOuterPtr = (char*)data + (totalWords >> 1) * wordBytes;
			size_t step = wordBytes << 1;
			for (; leftPtr < endOuterPtr; rightPtr -= step, endInnerPtr += wordBytes)
			{
				for (; leftPtr < endInnerPtr; ++leftPtr, ++rightPtr)
				{
					left = *leftPtr;
					*leftPtr = *rightPtr;
					*rightPtr = left;
				}
			}
		}

		// ----------------------------------------------------------------------------
		Ogre::ColourValue getColourValueFromString(const string& colorString);

		// ----------------------------------------------------------------------------
		bool getTextureFromInternalFile(const string& textureName, const string& group, const string& fileName);
		bool getTextureFromExternalFile(const string& textureName, const string& group, const string& fileName);

		// ----------------------------------------------------------------------------
		string getRelativePath(const string& absolutePath, const string& basePath);

		// ----------------------------------------------------------------------------
		string getWorkingPath();

		void reloadMaterial(const Ogre::String& materialName);
	}
}

#endif // __UTILS_H__
