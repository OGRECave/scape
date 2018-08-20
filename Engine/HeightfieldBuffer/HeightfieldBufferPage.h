/**
 * ScapeEngine::HeightfieldBufferPage class
 *
 * HeightfieldBuffer page. A page is a fixed-sized square tile of data
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDBUFFERPAGE_H__
#define __HEIGHTFIELDBUFFERPAGE_H__

namespace ScapeEngine {
    class HeightfieldBuffer;
    class HeightfieldBufferSet;

    typedef Utils::GUID HeightfieldBufferPageVersion;

    class HeightfieldBufferPage
    {
    public:
        HeightfieldBufferPage(HeightfieldBuffer* heightfieldBuffer, int pageColumn, int pageRow);

        ~HeightfieldBufferPage();

        inline HeightfieldBufferSet* getHeightfieldBufferSet() const { return mHeightfieldBufferSet; }
        Ogre::PixelFormat getHeightElementFormat() const { return mHeightElementFormat; }
        inline int getHeightfieldBufferCount() const { return mHeightfieldBuffers.size(); }
        inline HeightfieldBufferPageVersion getVersion() const { return mVersion; }
        inline void setVersion(HeightfieldBufferPageVersion version) { mVersion = version; }
        void increaseVersion();

        void addHeightfieldBuffer(HeightfieldBuffer* heightfieldBuffer);

        void removeHeightfieldBuffer(HeightfieldBuffer* heightfieldBuffer);

        void removeAllHeightfieldBuffers();

        // Update invalidated data. Returns true if it requires another pass after this one
        bool revalidatePass();

        // Update some area of this HeightfieldBufferPage with the given pixelbox
        void updateFrom(const Ogre::PixelBox& relativePixelBox);

        // Update some area of this HeightfieldBufferPage with the given pixelbox
        void copyTo(Ogre::PixelBox& relativePixelBox);

        // Get access to texture memory (or a shadow copy to it). Don't forget to increase the version number if
        // the texture memory is written to.
        Ogre::PixelBox getRawHeightData();

        Ogre::Rect getAbsoluteRect();

        void cloneDataFrom(HeightfieldBufferPage* const heightfieldBufferPage);

        bool containsData();

        void clearData();

        void unlockAll();

        inline int getPageColumn() { return mPageColumn; }
        inline int getPageRow() { return mPageRow; }
        void updateGeoms(const Ogre::Rect& rect);

        Ogre::TexturePtr getHeightTexture();

    private:
        HeightfieldBufferPageVersion mVersion;

        std::list<HeightfieldBuffer*> mHeightfieldBuffers;

        int mPageColumn, mPageRow;

        Ogre::TexturePtr _mHeightTexture;

        HeightfieldBufferSet* mHeightfieldBufferSet;

        Ogre::PixelFormat mHeightElementFormat;

        Ogre::PixelFormat getPackedFormat();
    };
}

#endif // __HEIGHTFIELDBUFFERPAGE_H__
