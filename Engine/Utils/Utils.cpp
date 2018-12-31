/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "Utils.h"

using namespace ScapeEngine;

const string Utils::emptyString = _T("");

// ----------------------------------------------------------------------------
const Utils::MooreNeighborhood::EDirection Utils::MooreNeighborhood::mOppositeDirection[] = {
    DIRECTION_S, DIRECTION_SW, DIRECTION_W, DIRECTION_NW,
    DIRECTION_N, DIRECTION_NE, DIRECTION_E, DIRECTION_SE};

const int Utils::MooreNeighborhood::mDirectionToX[] = {0, 1, 1, 1, 0, -1, -1, -1};
const int Utils::MooreNeighborhood::mDirectionToY[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const char* Utils::MooreNeighborhood::mDirectionToName[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};

// ----------------------------------------------------------------------------
const Utils::VonNeumannNeighborhood::EDirection Utils::VonNeumannNeighborhood::mOppositeDirection[] = {
    DIRECTION_S, DIRECTION_W, DIRECTION_N, DIRECTION_E};

const int Utils::VonNeumannNeighborhood::mDirectionToX[] = {0, 1, 0, -1};
const int Utils::VonNeumannNeighborhood::mDirectionToY[] = {-1, 0, 1, 0};
const char* Utils::VonNeumannNeighborhood::mDirectionToName[] = {"N", "E", "S", "W"};

// ----------------------------------------------------------------------------
Ogre::Vector4 Utils::getCatmullRomeSplineWeights(Ogre::Real param)
{
    Ogre::Real param2 = param * param;
    Ogre::Real param3 = param2 * param;

    return Ogre::Vector4(-0.166666667f * param3 + 0.500000000f * param2 - 0.333333333f * param,
                         0.500000000f * param3 - param2 - 0.500000000f * param + 1.000000000f,
                         -0.500000000f * param3 + 0.500000000f * param2 + param,
                         0.166666667f * param3 - 0.166666667f * param);
}

// ----------------------------------------------------------------------------
Ogre::PixelBox Utils::clipPixelBox(const Ogre::PixelBox& pixelBox, const Ogre::Rect& relativeRect,
                                   int offsetX, int offsetY)
{
    assert(pixelBox.data);
    assert(pixelBox.getDepth() == 1);
    assert(relativeRect.left >= (long)pixelBox.left);
    assert(relativeRect.top >= (long)pixelBox.top);
    assert(relativeRect.right <= (long)pixelBox.right);
    assert(relativeRect.bottom <= (long)pixelBox.bottom);
    assert(offsetX >= 0);
    assert(offsetY >= 0);

    int bytesPerElement = Ogre::PixelUtil::getNumElemBytes(pixelBox.format);
    int elementOffset =
        (relativeRect.top - pixelBox.top) * pixelBox.rowPitch + (relativeRect.left - pixelBox.left);

    Ogre::PixelBox clipBox;
    clipBox.left = offsetX;
    clipBox.top = offsetY;
    clipBox.front = 0;
    clipBox.right = offsetX + relativeRect.width();
    clipBox.bottom = offsetY + relativeRect.height();
    clipBox.back = 1;
    clipBox.rowPitch = pixelBox.rowPitch;
    clipBox.slicePitch = pixelBox.rowPitch * pixelBox.bottom;
    clipBox.format = pixelBox.format;
    clipBox.data = static_cast<Ogre::uint8*>(pixelBox.data) + elementOffset * bytesPerElement;

    return clipBox;
}

// ----------------------------------------------------------------------------
Ogre::Real Utils::getTypicalVertexElementRangeMin(Ogre::VertexElementType type)
{
    Ogre::VertexElementType baseType = Ogre::VertexElement::getBaseType(type);
    switch (baseType)
    {
    case Ogre::VET_FLOAT1:
        return 0.0f;

    case Ogre::VET_SHORT1:
        return -32768.0f;

    default:
        assert(false);
    }
    return 0.0f;
}

// ----------------------------------------------------------------------------
Ogre::Real Utils::getTypicalVertexElementRangeMax(Ogre::VertexElementType type)
{
    Ogre::VertexElementType baseType = Ogre::VertexElement::getBaseType(type);
    switch (baseType)
    {
    case Ogre::VET_FLOAT1:
        return 1.0f;

    case Ogre::VET_SHORT1:
        return 32767.0f;

    default:
        assert(false);
    }
    return 1.0f;
}

// ----------------------------------------------------------------------------
bool Utils::bindShaderCustomAutoConstant(Ogre::Technique* technique, size_t constantIndex,
                                         const string& constantName)
{

    for (Ogre::ushort i = 0; i < technique->getNumPasses(); ++i)
    {
        Ogre::Pass* pass = technique->getPass(i);
        if (pass->hasFragmentProgram())
        {
            Ogre::GpuProgramParametersSharedPtr params = pass->getFragmentProgramParameters();
            const Ogre::GpuProgramParameters::AutoConstantList& constants = params->getAutoConstants();
            for (Ogre::GpuProgramParameters::AutoConstantList::const_iterator it = constants.begin(); it != constants.end(); it++)
            {
                const Ogre::GpuProgramParameters::AutoConstantEntry& constantEntry = *it;
                if (constantEntry.paramType == Ogre::GpuProgramParameters::ACT_CUSTOM &&
                    constantEntry.data == constantIndex)
                {
                    return true;
                }
            }
            const Ogre::GpuConstantDefinition* definition =
                params->_findNamedConstantDefinition(constantName, false);
            if (definition)
            {
                // GDC port
                // before: params->_setRawAutoConstant(definition->physicalIndex,
                // Ogre::GpuProgramParameters::ACT_CUSTOM, constantIndex, definition->elementSize);
                size_t physicalIndex = params->_getFloatConstantPhysicalIndex(
                    definition->logicalIndex, definition->elementSize, Ogre::GPV_PER_OBJECT);
                params->_setRawAutoConstant(physicalIndex, Ogre::GpuProgramParameters::ACT_CUSTOM,
                                            constantIndex, Ogre::GPV_PER_OBJECT, definition->elementSize);

                return true;
            }
        }

        if (pass->hasVertexProgram())
        {
            Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
            const Ogre::GpuProgramParameters::AutoConstantList& constants = params->getAutoConstants();

            for (Ogre::GpuProgramParameters::AutoConstantList::const_iterator it = constants.begin(); it != constants.end(); it++)
            {
                const Ogre::GpuProgramParameters::AutoConstantEntry& constantEntry = *it;
                if (constantEntry.paramType == Ogre::GpuProgramParameters::ACT_CUSTOM &&
                    constantEntry.data == constantIndex)
                {
                    return true;
                }
            }
            const Ogre::GpuConstantDefinition* definition =
                params->_findNamedConstantDefinition(constantName, false);
            if (definition)
            {
                // GDC port
                // before: params->_setRawAutoConstant(definition->physicalIndex,
                // Ogre::GpuProgramParameters::ACT_CUSTOM, constantIndex, definition->elementSize);
                size_t physicalIndex = params->_getFloatConstantPhysicalIndex(
                    definition->logicalIndex, definition->elementSize, Ogre::GPV_PER_OBJECT);
                params->_setRawAutoConstant(physicalIndex, Ogre::GpuProgramParameters::ACT_CUSTOM,
                                            constantIndex, Ogre::GPV_PER_OBJECT, definition->elementSize);

                return true;
            }
        }
    }

    return false;
}

// ----------------------------------------------------------------------------
bool Utils::readResource(const string& fileName, const string& groupName, string& contents)
{
    Ogre::DataStreamPtr pStream =
        Ogre::ResourceGroupManager::getSingleton().openResource(fileName, groupName);
    if (pStream->size())
    {
        contents = pStream->getAsString() + _T("\0");
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------
Utils::GUID Utils::createGUID()
{
    static Utils::GUID nextId = 0;
    return ++nextId;
}

// ----------------------------------------------------------------------------
Ogre::uint32 Utils::randHash(int in) { return ((in * 1103515245 + 12345) >> 16) & 0x7FFF; }

// ----------------------------------------------------------------------------
void Utils::writeBinaryFile(const string& fileName, const void* data, int bytes, bool append)
{
    FILE* file = fopen(fileName.c_str(), (append ? _T("ab") : _T("wb")));

    if (file)
    {
        fwrite(data, bytes, 1, file);
        fclose(file);
    }
}

// ----------------------------------------------------------------------------
std::pair<void*, size_t> Utils::readBinaryFile(const string& fileName)
{
    FILE* file = fopen(fileName.c_str(), _T("rb"));
    char* data = NULL;
    int byteCount = 0;

    if (file)
    {
        fseek(file, 0, SEEK_END);
        byteCount = ftell(file);
        fseek(file, 0, SEEK_SET);
        if (byteCount > 0)
        {
            data = new char[byteCount + 1];
            byteCount = fread(data, 1, byteCount, file);
            *((char*)data + byteCount) = 0;
        }
        fclose(file);
    }

    return std::pair<void*, int>(data, byteCount);
}

// ----------------------------------------------------------------------------
string Utils::makeUniqueName(const string& baseName, const std::list<string>& existingNames)
{
    Ogre::uint32 nextIndex = 1;
    size_t indexStart = baseName.find_last_not_of(_T("0123456789"));

    if (indexStart == string::npos)
    {
        indexStart = 0; // so must consist solely of numbers, or is emptystring
    }
    else
    {
        ++indexStart; // next index is first digit of last block of digits
    }

    const Ogre::String indexString = Ogre::String(baseName.substr(indexStart));
    if (indexString.length() > 0)
    {
        Ogre::uint32 convIndex;
        nextIndex = Ogre::StringConverter::parse(indexString, convIndex) ? convIndex : nextIndex;
    }

    string base = baseName.substr(0, indexStart);
    string uniqueName;

    std::list<string>::const_iterator it, itEnd = existingNames.end();
    for (bool unique = false; !unique; ++nextIndex)
    {
        uniqueName = base + std::to_string(nextIndex);

        unique = true;
        for (it = existingNames.begin(); it != itEnd; ++it)
        {
            if ((*it).compare(uniqueName) == 0)
            {
                unique = false;
            }
        }
    }

    return uniqueName;
}

// ----------------------------------------------------------------------------
Ogre::ColourValue Utils::getColourValueFromString(const string& colorString)
{
    const string numericChars = _T("0123456789");
    Ogre::ColourValue color = Ogre::ColourValue::Black;

    size_t first = colorString.find_first_of(numericChars);
    size_t last = colorString.find_last_not_of(numericChars);
    if (first < last)
    {
        if (last != string::npos)
            last -= first;
        string trimmedString = colorString.substr(first, last);

        std::vector<Ogre::String> vec = Ogre::StringUtil::split(trimmedString, _T("\t\n;, "));

        for (size_t i = 0; i < 4 && i < vec.size(); ++i)
        {
            color[i] = Ogre::StringConverter::parseReal(vec[i]) / 255.0f;
        }
    }
    return color;
}

// ----------------------------------------------------------------------------
bool Utils::getTextureFromInternalFile(const string& textureName, const string& group,
                                       const string& fileName)
{
    Ogre::ResourcePtr resourcePtr = Ogre::TextureManager::getSingleton().getByName(textureName);
    bool successful = true;

    if (!resourcePtr)
    {
        successful = false;

        Ogre::Image image;
        try
        {
            image.load(fileName, group);
            Ogre::TexturePtr texturePtr =
                Ogre::TextureManager::getSingleton().loadImage(textureName, group, image);
            texturePtr->load();
            successful = true;
        }
        catch (...)
        {
        }

        if (!image.getSize())
        {
            Ogre::LogManager::getSingleton().logMessage(_T("Failed to load internal texture '" + fileName +
                                                           _T("'. Loading midgray texture for resource'") +
                                                           textureName + _T("' instead.")));
            image.load(_T("midgrey.png"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
            Ogre::TextureManager::getSingleton().loadImage(textureName, group, image);
        }
    }
    return true;
}

// ----------------------------------------------------------------------------
bool Utils::getTextureFromExternalFile(const string& textureName, const string& group,
                                       const string& fileName)
{
    Ogre::ResourcePtr resourcePtr = Ogre::TextureManager::getSingleton().getByName(textureName);
    bool successful = true;

    if (!resourcePtr)
    {
        successful = false;

        Ogre::Image image;
        std::pair<void*, size_t> fileData = Utils::readBinaryFile(fileName);
        if (fileData.first)
        {
            string splitFileName, splitExtension;
            Ogre::StringUtil::splitBaseFilename(fileName, splitFileName, splitExtension);
            Ogre::DataStreamPtr stream(new Ogre::MemoryDataStream(fileData.first, fileData.second));

            try
            {
                image.load(stream, splitExtension);
                Ogre::TexturePtr texturePtr =
                    Ogre::TextureManager::getSingleton().loadImage(textureName, group, image);
                texturePtr->load();
                successful = true;
            }
            catch (...)
            {
            }

            delete fileData.first;
        }

        if (!successful)
        {
            Ogre::LogManager::getSingleton().logMessage(_T("Failed to load external texture '" + fileName +
                                                           _T("'. Loading midgray texture for resource'") +
                                                           textureName + _T("' instead.")));
            image.load(_T("midgrey.png"), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
            Ogre::TextureManager::getSingleton().loadImage(textureName, group, image);
        }
    }
    return true;
}

// ----------------------------------------------------------------------------
string Utils::getRelativePath(const string& absolutePath, const string& basePath)
{
    string fileName, splitPath;
    Ogre::StringUtil::splitFilename(absolutePath, fileName, splitPath);
    string givenBasePath = Ogre::StringUtil::standardisePath(basePath);
    size_t basePathLength = givenBasePath.length();
    string absoluteBasePath = splitPath.substr(0, basePathLength);

    // case insensitive compare. might be platform specific
    string givenBasePathCmp = givenBasePath;
    Ogre::StringUtil::toLowerCase(givenBasePathCmp);
    string absoluteBasePathCmp = absoluteBasePath;
    Ogre::StringUtil::toLowerCase(absoluteBasePathCmp);

    if (givenBasePathCmp.compare(absoluteBasePathCmp) == 0)
    {
        return _T("./") + splitPath.substr(basePathLength) + fileName;
    }
    return splitPath + fileName;
}

// ----------------------------------------------------------------------------
string Utils::getWorkingPath()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
    typedef char TCHAR;
#define MAX_PATH 1024
#define _getcwd getcwd
#endif

    TCHAR path[MAX_PATH];
    _getcwd(path, MAX_PATH);
    return path;
}

// ----------------------------------------------------------------------------
void Utils::reloadMaterial(const Ogre::String& materialName)
{
    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(materialName);
    if (material)
    {
        const Ogre::Material::Techniques& techniques = material->getTechniques();
        for (Ogre::Material::Techniques::const_iterator it = techniques.begin(); it != techniques.end(); it++)
        {
            Ogre::Technique* technique = (*it);

            const Ogre::Technique::Passes& passes = technique->getPasses();
            for (Ogre::Technique::Passes::const_iterator pit = passes.begin(); pit != passes.end(); pit++)
            {
                Ogre::Pass* pass = (*pit);

                const Ogre::GpuProgramPtr& fragmentProgramPtr = pass->getFragmentProgram();
                if (fragmentProgramPtr)
                {
                    fragmentProgramPtr->reload();
                    fragmentProgramPtr->createParameters();
                }

                const Ogre::GpuProgramPtr& vertexProgramPtr = pass->getVertexProgram();
                if (vertexProgramPtr)
                {
                    vertexProgramPtr->reload();
                    vertexProgramPtr->createParameters();
                }
            }
        }
        material->reload();
    }
}
