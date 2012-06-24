/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"
#include "HeightfieldSerializerIncremental.h"
#include "HeightfieldBuffer/HeightfieldBuffer.h"
#include "HeightfieldBuffer/HeightfieldBufferPage.h"
#include "HeightfieldBuffer/HeightfieldBufferSet.h"

#include "Utils/RLECodec.h"
#include "Utils/SerialMemoryBuffer.h"

// ----------------------------------------------------------------------------
namespace ScapeEngine
{
	namespace _HeightfieldSerializerIncrementalNS
	{
		// ----------------------------------------------------------------------------
		template<class Type>
		void _serializeIncrementalPage(HeightfieldBufferPage* inPreviousPage, HeightfieldBufferPage* inNextPage, Utils::SerialMemoryBufferWriter& writer)
		{
			HeightfieldBufferPageVersion previousVersion = inPreviousPage->getVersion();
			HeightfieldBufferPageVersion nextVersion = inNextPage->getVersion();

			size_t writerStartPosition = writer.getPosition();
			writer.advancePosition(sizeof(size_t));

			writer.write(previousVersion);
			writer.write(nextVersion);

			Type defaultValue = 0;
			size_t flags = 0; // bit 0 = 'previous' contains data. bit 1 = 'next' contains data

			if (inPreviousPage->getVersion() != inNextPage->getVersion())
			{
				Utils::RLECompressor<Type, unsigned short> compressor(&writer);

				if (inPreviousPage->containsData())
				{
					if (inNextPage->containsData())
					{
						flags = 3; 
						writer.write(flags);
						const Ogre::PixelBox& previousBox = inPreviousPage->getRawHeightData();
						const Ogre::PixelBox& nextBox = inNextPage->getRawHeightData();
						
						assert(previousBox.getWidth() == nextBox.getWidth());
						assert(previousBox.getHeight() == nextBox.getHeight());
						assert(previousBox.format == nextBox.format);
						assert(previousBox.data && nextBox.data);

						size_t componentCount = Ogre::PixelUtil::getComponentCount(previousBox.format);
						size_t rowPtrDelta = previousBox.getWidth() * componentCount;
						size_t rowPtrSkip = previousBox.getRowSkip() * componentCount;

						size_t topLeftOffset = (rowPtrDelta + rowPtrSkip) * previousBox.top + componentCount * previousBox.left;

						Type* previousPtr = static_cast<Type*>(previousBox.data) + topLeftOffset;
						Type* nextPtr = static_cast<Type*>(nextBox.data) + topLeftOffset;
						Type lastDelta = 0;

						compressor.begin();
						for (size_t y = previousBox.top; y < previousBox.bottom; ++y)
						{
							Type* previousPtrEnd = previousPtr + rowPtrDelta;
							while (previousPtr != previousPtrEnd)
							{
								Type delta = *nextPtr - *previousPtr;
								compressor.write(delta - lastDelta);
								lastDelta = delta;

								previousPtr += componentCount;
								nextPtr += componentCount;
							}
							previousPtr += rowPtrSkip;
							nextPtr += rowPtrSkip;
						}
						compressor.end();
					}
					else
					{
						flags = 1;
						writer.write(flags);

						const Ogre::PixelBox& previousBox = inPreviousPage->getRawHeightData();
						
						assert(previousBox.data);

						size_t componentCount = Ogre::PixelUtil::getComponentCount(previousBox.format);
						size_t rowPtrDelta = previousBox.getWidth() * componentCount;
						size_t rowPtrSkip = previousBox.getRowSkip() * componentCount;

						size_t topLeftOffset = (rowPtrDelta + rowPtrSkip) * previousBox.top + componentCount * previousBox.left;
						Type* previousPtr = static_cast<Type*>(previousBox.data) + topLeftOffset;
						Type lastDelta = 0;

						compressor.begin();
						for (size_t y = previousBox.top; y < previousBox.bottom; ++y)
						{
							Type* previousPtrEnd = previousPtr + rowPtrDelta;
							while (previousPtr != previousPtrEnd)
							{
								Type delta = defaultValue - *previousPtr;
								compressor.write(delta - lastDelta);
								lastDelta = delta;

								previousPtr += componentCount;
							}
							previousPtr += rowPtrSkip;
						}
						compressor.end();
					}
				}
				else
				{
					flags = 2; 
					writer.write(flags);

					const Ogre::PixelBox& nextBox = inNextPage->getRawHeightData();
					
					assert(nextBox.data);

					size_t componentCount = Ogre::PixelUtil::getComponentCount(nextBox.format);
					size_t rowPtrDelta = nextBox.getWidth() * componentCount;
					size_t rowPtrSkip = nextBox.getRowSkip() * componentCount;
					size_t topLeftOffset = (rowPtrDelta + rowPtrSkip) * nextBox.top + componentCount * nextBox.left;
					Type* nextPtr = static_cast<Type*>(nextBox.data) + topLeftOffset;
					Type lastDelta = 0;

					compressor.begin();
					for (size_t y = nextBox.top; y < nextBox.bottom; ++y)
					{
						Type* nextPtrEnd = nextPtr + rowPtrDelta;
						while (nextPtr != nextPtrEnd)
						{
							Type delta = *nextPtr - defaultValue;
							compressor.write(delta - lastDelta);
							lastDelta = delta;

							nextPtr += componentCount;
						}
						nextPtr += rowPtrSkip;
					}
					compressor.end();
				}
			}
			else
			{
				flags = 0;
				writer.write(flags);
			}

			size_t lastPosition = writer.getPosition();
			size_t byteCount = lastPosition - writerStartPosition;

			writer.setPosition(writerStartPosition);
			writer.write(byteCount);
			writer.setPosition(lastPosition);
		}

		// ----------------------------------------------------------------------------
		template<class Type>
		bool _deserializeIncrementalNextPage(HeightfieldBufferPage* page, Utils::SerialMemoryBufferReader& reader)
		{
			size_t byteCount;
			reader.read(byteCount);

			HeightfieldBufferPageVersion previousVersion, nextVersion;
			reader.read(previousVersion);
			reader.read(nextVersion);

			size_t flags;
			reader.read(flags);

			assert(page->getVersion() == previousVersion);

			if (previousVersion != nextVersion)
			{
				if (flags & 2) 
				{
					Utils::RLEDecompressor<Type, unsigned short> decompressor(&reader);
					decompressor.begin();

					const Ogre::PixelBox& box = page->getRawHeightData();
					
					assert(box.data);

					size_t componentCount = Ogre::PixelUtil::getComponentCount(box.format);
					size_t rowPtrDelta = box.getWidth() * componentCount;
					size_t rowPtrSkip = box.getRowSkip() * componentCount;
					size_t topLeftOffset = (rowPtrDelta + rowPtrSkip) * box.top + componentCount * box.left;
					Type* ptr = static_cast<Type*>(box.data) + topLeftOffset;
					Type delta = 0;

					for (size_t y = box.top; y < box.bottom; ++y)
					{
						Type* ptrEnd = ptr + rowPtrDelta;
						while (ptr != ptrEnd)
						{
							delta = delta + decompressor.read();
							*ptr = *ptr + delta;
							ptr += componentCount;
						}
						ptr += rowPtrSkip;
					}

					decompressor.end();
				}
				else
				{	
					page->clearData();
				}

				page->setVersion(nextVersion);

				return true;
			}
			return false;
		}

		// ----------------------------------------------------------------------------
		template<class Type>
		bool _deserializeIncrementalPreviousPage(HeightfieldBufferPage* page, Utils::SerialMemoryBufferReader& reader)
		{
			size_t byteCount;
			reader.read(byteCount);

			HeightfieldBufferPageVersion previousVersion, nextVersion;
			reader.read(previousVersion);
			reader.read(nextVersion);

			assert(page->getVersion() == nextVersion);

			size_t flags;
			reader.read(flags);

			if (previousVersion != nextVersion)
			{
				if (flags & 1)
				{
					Utils::RLEDecompressor<Type, unsigned short> decompressor(&reader);
					decompressor.begin();

					const Ogre::PixelBox& box = page->getRawHeightData();
					
					assert(box.data);

					size_t componentCount = Ogre::PixelUtil::getComponentCount(box.format);
					size_t rowPtrDelta = box.getWidth() * componentCount;
					size_t rowPtrSkip = box.getRowSkip() * componentCount;
					size_t topLeftOffset = (rowPtrDelta + rowPtrSkip) * box.top + componentCount * box.left;
					Type* ptr = static_cast<Type*>(box.data) + topLeftOffset;
					Type delta = 0;

					for (size_t y = box.top; y < box.bottom; ++y)
					{
						Type* ptrEnd = ptr + rowPtrDelta;
						while (ptr != ptrEnd)
						{
							delta = delta + decompressor.read();
							*ptr = *ptr - delta;

							ptr += componentCount;
						}
						ptr += rowPtrSkip;
					}

					decompressor.end();
				}
				else
				{
					page->clearData();
				}

				page->setVersion(previousVersion);

				return true;
			}
			return false;
		}
	}
};

// ----------------------------------------------------------------------------
using namespace ScapeEngine;
using namespace ScapeEngine::_HeightfieldSerializerIncrementalNS;



// ----------------------------------------------------------------------------
Utils::SerialMemoryBufferPtr HeightfieldSerializer::serializeIncremental(HeightfieldBuffer* inPreviousBuffer, HeightfieldBuffer *inNextBuffer)
{
	assert(inPreviousBuffer);
	assert(inNextBuffer);

	int elementCount = inPreviousBuffer->getHeightfieldBufferSet()->getElementColumnCount() * inPreviousBuffer->getHeightfieldBufferSet()->getElementRowCount();
	int bytesPerElement = Ogre::PixelUtil::getNumElemBytes(inPreviousBuffer->getHeightElementFormat());
	int dataByteCount = elementCount * bytesPerElement;
	int headerByteCount = 1000;

	assert(elementCount == inPreviousBuffer->getHeightfieldBufferSet()->getElementColumnCount() * inPreviousBuffer->getHeightfieldBufferSet()->getElementRowCount());
	assert((size_t)bytesPerElement == Ogre::PixelUtil::getNumElemBytes(inPreviousBuffer->getHeightElementFormat()));
	assert(inPreviousBuffer->getHeightfieldBufferSet()->getPageColumnCount() == inNextBuffer->getHeightfieldBufferSet()->getPageColumnCount());
	assert(inPreviousBuffer->getHeightfieldBufferSet()->getPageRowCount() == inNextBuffer->getHeightfieldBufferSet()->getPageRowCount());
	
	Utils::SerialMemoryBufferPtr serialBufferPtr(new Utils::SerialMemoryBuffer());
	serialBufferPtr.get()->setSize(headerByteCount + dataByteCount, false);

	Utils::SerialMemoryBufferWriter writer(serialBufferPtr);

	for (int pageRowIndex = 0; pageRowIndex < inPreviousBuffer->getHeightfieldBufferSet()->getPageRowCount(); ++pageRowIndex)
	{
		for (int pageColumnIndex = 0; pageColumnIndex < inPreviousBuffer->getHeightfieldBufferSet()->getPageColumnCount(); ++pageColumnIndex)
		{
			HeightfieldBufferPage* previousPage = inPreviousBuffer->getPage(pageColumnIndex, pageRowIndex, HeightfieldBuffer::PAGEACCESSMODE_READONLY);
			HeightfieldBufferPage* nextPage = inNextBuffer->getPage(pageColumnIndex, pageRowIndex, HeightfieldBuffer::PAGEACCESSMODE_READONLY);

			if (Ogre::PixelUtil::getComponentType(inPreviousBuffer->getHeightElementFormat()) == Ogre::PCT_FLOAT32)
			{
				_serializeIncrementalPage<Ogre::Real>(previousPage, nextPage, writer);
			}
			else if (Ogre::PixelUtil::getComponentType(inPreviousBuffer->getHeightElementFormat()) == Ogre::PCT_SHORT)
			{
				_serializeIncrementalPage<Ogre::uint16>(previousPage, nextPage, writer);
			}
			else
			{
				OGRE_EXCEPT(
					Ogre::Exception::ERR_ITEM_NOT_FOUND, 
					_T("Tried to create icnremental serialization of unknown vertex element type '") + 
					Ogre::StringConverter::toString(inPreviousBuffer->getHeightElementFormat()) + _T("'"),
					_T("HeightfieldSerializer::serializeIncremental"));
			}
		}
	}

	serialBufferPtr->setSize(writer.getPosition(), true);

	{
		size_t uncompressedSize = Ogre::PixelUtil::getMemorySize(inPreviousBuffer->getHeightfieldBufferSet()->getElementColumnCount(), inPreviousBuffer->getHeightfieldBufferSet()->getElementRowCount(), 1, inPreviousBuffer->getHeightElementFormat());
		size_t compressedSize = serialBufferPtr->getSize();
		printf(_T("serializeIncremental: compressed %d to %d (%d%%)\n"), uncompressedSize, compressedSize, (int)(100.0f * compressedSize / uncompressedSize));
	}

	return serialBufferPtr;
}

// ----------------------------------------------------------------------------
bool HeightfieldSerializer::deserializeIncrementalNext(Utils::SerialMemoryBufferConstPtr inSerial, HeightfieldBuffer* buffer, string *error)
{
	assert(buffer);

	Utils::SerialMemoryBufferReader reader(inSerial);

	for (int pageRowIndex = 0; pageRowIndex < buffer->getHeightfieldBufferSet()->getPageRowCount(); ++pageRowIndex)
	{
		for (int pageColumnIndex = 0; pageColumnIndex < buffer->getHeightfieldBufferSet()->getPageColumnCount(); ++pageColumnIndex)
		{
			HeightfieldBufferPage* page = buffer->getPage(pageColumnIndex, pageRowIndex, HeightfieldBuffer::PAGEACCESSMODE_READWRITE);
			Ogre::Rect dirtyRect(
				pageColumnIndex * buffer->getHeightfieldBufferSet()->getElementColumnCountPerPage(),
				pageRowIndex * buffer->getHeightfieldBufferSet()->getElementRowCountPerPage(),
				(pageColumnIndex + 1) * buffer->getHeightfieldBufferSet()->getElementColumnCountPerPage(),
				(pageRowIndex + 1) * buffer->getHeightfieldBufferSet()->getElementRowCountPerPage());

			if (Ogre::PixelUtil::getComponentType(buffer->getHeightElementFormat()) == Ogre::PCT_FLOAT32)
			{
				if (_deserializeIncrementalNextPage<Ogre::Real>(page, reader))
				{
					buffer->updateGeoms(dirtyRect);
				}
			}
			else if (Ogre::PixelUtil::getComponentType(buffer->getHeightElementFormat()) == Ogre::PCT_SHORT)
			{
				if (_deserializeIncrementalNextPage<Ogre::uint16>(page, reader))
				{
					buffer->updateGeoms(dirtyRect);
				}
			}
			else
			{
				OGRE_EXCEPT(
					Ogre::Exception::ERR_ITEM_NOT_FOUND, 
					_T("Tried to decode icnremental serialization of unknown vertex element type '") + 
					Ogre::StringConverter::toString(buffer->getHeightElementFormat()) + _T("'"),
					_T("HeightfieldSerializer::deserializeIncrementalPrevious"));
			}
		}
	}

	return true;
}


// ----------------------------------------------------------------------------
bool HeightfieldSerializer::deserializeIncrementalPrevious(Utils::SerialMemoryBufferConstPtr inSerial, HeightfieldBuffer* buffer, string *error)
{
	assert(buffer);

	Utils::SerialMemoryBufferReader reader(inSerial);

	for (int pageRowIndex = 0; pageRowIndex < buffer->getHeightfieldBufferSet()->getPageRowCount(); ++pageRowIndex)
	{
		for (int pageColumnIndex = 0; pageColumnIndex < buffer->getHeightfieldBufferSet()->getPageColumnCount(); ++pageColumnIndex)
		{
			HeightfieldBufferPage* page = buffer->getPage(pageColumnIndex, pageRowIndex, HeightfieldBuffer::PAGEACCESSMODE_READWRITE);
			Ogre::Rect dirtyRect(
				pageColumnIndex * buffer->getHeightfieldBufferSet()->getElementColumnCountPerPage(),
				pageRowIndex * buffer->getHeightfieldBufferSet()->getElementRowCountPerPage(),
				(pageColumnIndex + 1) * buffer->getHeightfieldBufferSet()->getElementColumnCountPerPage(),
				(pageRowIndex + 1) * buffer->getHeightfieldBufferSet()->getElementRowCountPerPage());

			if (Ogre::PixelUtil::getComponentType(buffer->getHeightElementFormat()) == Ogre::PCT_FLOAT32)
			{
				if (_deserializeIncrementalPreviousPage<Ogre::Real>(page, reader))
				{
					buffer->updateGeoms(dirtyRect);
				}
			}
			else if (Ogre::PixelUtil::getComponentType(buffer->getHeightElementFormat()) == Ogre::PCT_SHORT)
			{
				if (_deserializeIncrementalPreviousPage<Ogre::uint16>(page, reader))
				{
					buffer->updateGeoms(dirtyRect);
				}
			}
			else
			{
				OGRE_EXCEPT(
					Ogre::Exception::ERR_ITEM_NOT_FOUND, 
					_T("Tried to decode icnremental serialization of unknown vertex element type '") + 
					Ogre::StringConverter::toString(buffer->getHeightElementFormat()) + _T("'"),
					_T("HeightfieldSerializer::deserializeIncrementalPrevious"));
			}
		}
	}

	return true;
}
