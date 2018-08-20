/**
 * ScapeEngine::Utils::RLECompressor and RLEDecompressor template classes
 *
 * Simple RLE compress and decompress functionality
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __RLECODEC_H__
#define __RLECODEC_H__

#include "Utils/SerialMemoryBuffer.h"

namespace ScapeEngine {
    namespace Utils {
        // ----------------------------------------------------------------------------
        template <class DataType, class MarkerType> class RLEDecompressor
        {
        public:
            // ----------------------------------------------------------------------------
            RLEDecompressor(SerialMemoryBufferReader* reader) : mReader(reader)
            {
                mMarkerStateNonzeroBitMask = (1 << ((sizeof(MarkerType) << 3) - 1));
                mMarkerMaxCount = mMarkerStateNonzeroBitMask - 1;
            }

            // ----------------------------------------------------------------------------
            void begin()
            {
                mRemain = 0;
                mZeroState = true;
            }

            // ----------------------------------------------------------------------------
            DataType read()
            {
                if (mRemain == 0)
                {
                    MarkerType count;

                    mReader->read(count);

                    mZeroState = (count & mMarkerStateNonzeroBitMask) == 0;
                    mRemain = count & mMarkerMaxCount;
                }

                DataType ret = 0;

                if (mRemain > 0 && !mZeroState)
                {
                    mReader->read(ret);
                }
                --mRemain;

                return ret;
            }

            // ----------------------------------------------------------------------------
            void end() { assert(mRemain == 0); }
        protected:
            // ----------------------------------------------------------------------------
            SerialMemoryBufferReader* mReader;
            size_t mRemain;
            size_t mNextMarkerPosition;
            MarkerType mMarkerMaxCount;
            MarkerType mMarkerStateNonzeroBitMask;
            bool mZeroState;
        };

        // ----------------------------------------------------------------------------
        template <class DataType, class MarkerType> class RLECompressor
        {
        public:
            // ----------------------------------------------------------------------------
            RLECompressor(SerialMemoryBufferWriter* writer) : mWriter(writer)
            {
                mMarkerStateNonzeroBitMask = (1 << ((sizeof(MarkerType) << 3) - 1));
                mMarkerMaxCount = mMarkerStateNonzeroBitMask - 1;

                mMinZeroCount = (sizeof(MarkerType) + sizeof(DataType) - 1) / sizeof(DataType);
            }

            // ----------------------------------------------------------------------------
            void begin()
            {
                mZeroCount = mNonzeroCount = 0;
                mNonzeroMarkerPosition = mZeroMarkerPosition = mWriter->getPosition();
                mWriter->advancePosition(sizeof(MarkerType));
                mHasWritten = false;
            }

            // ----------------------------------------------------------------------------
            void write(DataType value)
            {
                if (!mHasWritten)
                {
                    mZeroState = (value == 0); // start with correct state
                    mHasWritten = true;
                }

                if (mZeroState)
                {
                    if (value == 0) // was and is zero
                    {
                        ++mZeroCount;
                        if (mZeroCount > mMarkerMaxCount) // make sure a marker's count field isn't overflown
                        {
                            writeMarker(mZeroMarkerPosition, (MarkerType)mMarkerMaxCount);
                            mZeroMarkerPosition = mWriter->getPosition();
                            mWriter->advancePosition(sizeof(MarkerType));
                            mZeroCount -= mMarkerMaxCount;
                        }
                    }
                    else // was zero but is nonzero: write zero-count and change state
                    {
                        writeMarker(mZeroMarkerPosition, (MarkerType)mZeroCount);

                        mZeroCount = 0;
                        mZeroState = false;

                        mNonzeroMarkerPosition = mWriter->getPosition();
                        mWriter->advancePosition(sizeof(MarkerType));

                        ++mNonzeroCount;
                        mWriter->write(value);
                    }
                }
                else
                {
                    if (value == 0) // was nonzero but is now zero
                    {
                        if (mZeroCount == 0) // store memory offset for zero marker
                        {
                            mZeroMarkerPosition = mWriter->getPosition();
                        }
                        if (mZeroCount < mMinZeroCount) // keep treating as nonzero as long as
                        { // zero count is smaller than minzerocount
                            mWriter->write(value);
                        }
                        else // else: discard appended zero string, write nonzero marker and switch state
                        {
                            writeMarker(
                                mNonzeroMarkerPosition, (MarkerType)(mMarkerStateNonzeroBitMask | mNonzeroCount));
                            mWriter->setPosition(mZeroMarkerPosition + sizeof(MarkerType));

                            mNonzeroCount = 0;
                            mZeroState = true;
                        }
                        ++mZeroCount;
                    }
                    else // was and is nonzero
                    {
                        mNonzeroCount += mZeroCount + 1;
                        mZeroCount = 0;
                        if (mNonzeroCount > mMarkerMaxCount) // make sure a marker's count field isn't overflown
                        {
                            writeMarker(mNonzeroMarkerPosition, mMarkerStateNonzeroBitMask | mMarkerMaxCount);
                            mNonzeroMarkerPosition = mWriter->getPosition();
                            mWriter->advancePosition(sizeof(MarkerType));
                            mNonzeroCount -= mMarkerMaxCount;
                        }
                        mWriter->write(value);
                    }
                }
            }

            // ----------------------------------------------------------------------------
            void end()
            {
                if (mZeroState)
                {
                    writeMarker(mZeroMarkerPosition, (MarkerType)(mZeroCount + mNonzeroCount));
                }
                else
                {
                    writeMarker(mNonzeroMarkerPosition,
                        (MarkerType)(mMarkerStateNonzeroBitMask | (mZeroCount + mNonzeroCount)));
                }
            }

        protected:
            // ----------------------------------------------------------------------------
            SerialMemoryBufferWriter* mWriter;
            size_t mZeroCount;
            size_t mNonzeroCount;
            size_t mZeroMarkerPosition;
            size_t mNonzeroMarkerPosition;
            size_t mMinZeroCount;
            MarkerType mMarkerMaxCount;
            MarkerType mMarkerStateNonzeroBitMask;
            bool mZeroState;
            bool mHasWritten;

            // ----------------------------------------------------------------------------
            void writeMarker(size_t position, MarkerType marker)
            {
                size_t oldPosition = mWriter->getPosition();
                mWriter->setPosition(position);

                // size_t doubleMarkerBit = (sizeof(MarkerType) << 3) - 1;
                // size_t doubleMarkerBitMask = 1 << doubleMarkerBit;

                // if (count < doubleMarkerBitMask)
                //{
                // MarkerType c = (MarkerType)count;
                mWriter->write(marker);
                //}
                // else
                //{
                // MarkerType c = (MarkerType)(doubleMarkerBitMask | (count & (doubleMarkerBitMask - 1)));
                // mWriter->write(c);
                //}
                mWriter->setPosition(oldPosition);
            }
        };
    }
}

#endif // __RLECODEC_H__