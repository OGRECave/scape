/**
 * ScapeEngine::HeightfieldBuffer class
 *
 * Manages HeightfieldBufferPage objects
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#ifndef __HEIGHTFIELDBUFFER_H__
#define __HEIGHTFIELDBUFFER_H__

//#include "HeightfieldBufferSet.h"

namespace ScapeEngine
{
	class HeightfieldBufferPage;
	class HeightfieldBufferSet;
	class HeightfieldGeom;

	typedef Utils::GUID HeightfieldBufferHandle;
	typedef Utils::GUID HeightfieldBufferVersion;

	class HeightfieldBuffer
	{
	public:
		HeightfieldBuffer(HeightfieldBufferSet* set);

		HeightfieldBuffer(
			HeightfieldBufferSet* set,
			const string& name,
			Ogre::PixelFormat pixelFormat,
			int elementColumnCountPerPage, 
			int elementRowCountPerPage, 
			int elementColumnCount,
			int elementRowCount,
			Ogre::Real heightRangeMin, 
			Ogre::Real heightRangeMax);

		enum EPageAccessMode
		{
			PAGEACCESSMODE_READONLY,
			PAGEACCESSMODE_WRITE_DISCARD,
			PAGEACCESSMODE_READWRITE
		};

		~HeightfieldBuffer();

		inline string getName() const {return mName;}

		inline HeightfieldBufferHandle getHandle() const {return mHandle;}

		inline HeightfieldBufferVersion getVersion() const {return mVersion;}

		inline void increaseVersion() {mVersion = Utils::createGUID();}

		inline void setName(const string& name) {mName = name;}

		void cloneFrom(HeightfieldBuffer* heightfieldBuffer, bool sharePages);
		inline Ogre::PixelFormat getHeightElementFormat() const {return mHeightElementFormat;}

		void registerHeightfieldGeom(HeightfieldGeom* heightfieldGeom);

		void unregisterHeightfieldGeom(HeightfieldGeom* heightfieldGeom);

		void updateGeoms(const Ogre::Rect& rect);

		// Update some area of this HeightfieldBuffer with the given pixelbox
		void updateFrom(const Ogre::PixelBox& relativePixelBox);

		// Get some area of this HeightfieldBuffer with the given pixelbox. 
		// The specified pixelbox should be fully initialized.
		void HeightfieldBuffer::copyTo(Ogre::PixelBox& relativePixelBox);

		// Make internal page data consistent after possibly being invalidated by heightfieldbufferpage updates.
		void revalidate();		

		// Return HeightfieldBufferPage at (pageColumnIndex, pageRowIndex), or NULL when outside boundaries
		HeightfieldBufferPage *getPage(int pageColumnIndex, int pageRowIndex, EPageAccessMode accessMode);

		void replacePage(int pageColumnIndex, int pageRowIndex, HeightfieldBuffer* srcBuffer);
		void swapPage(int pageColumnIndex, int pageRowIndex, HeightfieldBuffer* srcBuffer);
		
		int getPageColumnFromElement(int elementColumnIndex);
		int getPageRowFromElement(int elementRowIndex);

		void unlockAll();	

		inline Ogre::PixelFormat getHeightElementFormat() {return mHeightElementFormat;}

		inline HeightfieldBufferSet* getHeightfieldBufferSet() const {return mHeightfieldBufferSet;}


	private:
		HeightfieldBufferHandle mHandle;

		HeightfieldBufferVersion mVersion;

		string mName;

		Ogre::PixelFormat mHeightElementFormat;

		/**

		int mElementColumnCountPerPage;

		int mElementRowCountPerPage;

		int mElementColumnCount;

		int mElementRowCount;

		int mPageRowCount;

		int mPageColumnCount;


		Ogre::Real mHeightRangeMin;

		Ogre::Real mHeightRangeMax;

		 *******/

		HeightfieldBufferSet* mHeightfieldBufferSet;


		typedef std::vector<HeightfieldBufferPage*> Pages;

		Pages mPages;

		typedef std::list<HeightfieldGeom*> HeightfieldGeoms;
		HeightfieldGeoms mHeightfieldGeoms;

		void unsharePage(int index, bool copyData);

		int getPageIndexFromCoords(int pageColumnIndex, int pageRowIndex);
	};

}

#endif // __HEIGHTFIELDBUFFER_H__
