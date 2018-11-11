/**
 * ScapeEngine::SerialMemoryBuffer utility class
 *
 *
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __SERIALMEMORYBUFFER_H__
#define __SERIALMEMORYBUFFER_H__

namespace ScapeEngine
{
namespace Utils
{
// ----------------------------------------------------------------------------
class SerialMemoryBuffer
{
public:
    SerialMemoryBuffer() : mData(NULL), mSize(0) {}

    ~SerialMemoryBuffer()
    {
        SAFE_DELETE(mData);
        mSize = 0;
    }

    void setSize(size_t bytes, bool copyContents)
    {
        void* newData = new Ogre::uint8[bytes];

        if (!newData)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        _T("Not enough memory left to continue. Tried to allocate") +
                            Ogre::StringConverter::toString(bytes) + _T("bytes."),
                        _T("SerialMemoryBuffer::setSize"));
        }

        if (copyContents && mData)
        {
            memcpy(newData, mData, std::min(mSize, bytes));
        }

        delete mData;
        mData = newData;
        mSize = bytes;
    }

    inline size_t getSize() const { return mSize; }
    inline void* getData() const { return mData; }
    inline void* getRelativeData(int bytePosition) const
    {
        return static_cast<unsigned char*>(mData) + bytePosition;
    }

protected:
    void* mData;
    size_t mSize;
};

// ----------------------------------------------------------------------------
typedef std::shared_ptr<SerialMemoryBuffer> SerialMemoryBufferPtr;
typedef std::shared_ptr<const SerialMemoryBuffer> SerialMemoryBufferConstPtr;

// ----------------------------------------------------------------------------
class SerialMemoryBufferReader
{
public:
    SerialMemoryBufferReader(SerialMemoryBufferPtr bufferPtr) : mBufferPtr(bufferPtr), mPosition(0)
    {
        assert(bufferPtr);
    }

    SerialMemoryBufferReader(SerialMemoryBufferConstPtr bufferPtr) : mBufferPtr(bufferPtr), mPosition(0)
    {
        assert(bufferPtr);
    }

    inline size_t getPosition() { return mPosition; }
    inline void setPosition(size_t position) { mPosition = position; }
    inline void advancePosition(size_t bytes) { mPosition += bytes; }

    inline void* getData() { return mBufferPtr->getRelativeData(mPosition); }

    template <class T> bool read(T& data, bool advancePosition = true)
    {
        if (mPosition + sizeof(T) <= mBufferPtr->getSize())
        {
            data = *static_cast<T*>(mBufferPtr->getRelativeData(mPosition));
            // printf("read %d: %d\n", mPosition, (int)data);
            if (advancePosition)
                mPosition += sizeof(T);
            return true;
        }
        return false;
    }

protected:
    SerialMemoryBufferConstPtr mBufferPtr;

    int mPosition;
};

// ----------------------------------------------------------------------------
class SerialMemoryBufferWriter
{
public:
    SerialMemoryBufferWriter(SerialMemoryBufferPtr bufferPtr) : mBufferPtr(bufferPtr), mPosition(0)
    {
        assert(bufferPtr);
    }

    inline size_t getPosition() { return mPosition; }
    inline void setPosition(size_t position) { mPosition = position; }
    inline void advancePosition(size_t bytes) { mPosition += bytes; }

    inline void* getData() { return mBufferPtr->getRelativeData(mPosition); }

    template <class T> bool write(T& data, bool advancePosition = true)
    {
        // printf("write %d: %d\n", mPosition, (int)data);

        if (mPosition + sizeof(T) <= mBufferPtr.get()->getSize())
        {
            *static_cast<T*>(mBufferPtr->getRelativeData(mPosition)) = data;
            if (advancePosition)
                mPosition += sizeof(T);
            return true;
        }
        return false;
    }

protected:
    SerialMemoryBufferPtr mBufferPtr;

    int mPosition;
};
}
}

#endif // __SERIALMEMORYBUFFER_H__
