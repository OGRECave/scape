#ifndef __SCAPEENGINEPREREQUISITES_H__
#define __SCAPEENGINEPREREQUISITES_H__

#include "ScapeEnginePlatform.h"

#include <Ogre.h>
#include <OgreConfigFile.h>
#include <OgreException.h>
#include <OgreStringConverter.h>
#include <OgreBitwise.h>

#include <string>
#include <list>
#include <map>
#include <vector>

typedef std::string string;
#ifndef _T
#define _T(x) x
#endif
#ifndef _TCHAR
#define _TCHAR char
#endif

namespace ScapeEngine
{
struct BrushInstance;
class Button;
class ButtonDefinition;
class ButtonDefinitionDataAccessObject;
class CameraController;
class Console;
class EngineCore;
class EngineInterface;
template <class FactoryBaseClass> class FactoryClass;
class GPU2DOperation;
class GPU2DOperationManager;
class GPU2DOperationQuad;
class GPU2DOperationQuadBrush;
class GPU2DOperationQuadCustom;
class GPU2DOperationRenderableQuad;
class GPU2DOperationRenderableQuadManager;
class HeightfieldBrush;
class HeightfieldBrushManager;
class HeightfieldBrushSettings;
class HeightfieldBuffer;
class HeightfieldBufferPage;
class HeightfieldBufferSet;
class HeightfieldBufferSetManager;
class HeightfieldFileDecoder;
class HeightfieldFileDecoderImageBase;
class HeightfieldFileDecoderManager;
class HeightfieldFileDecoderRaw;
class HeightfieldFileEncoder;
class HeightfieldFileEncoderImageBase;
class HeightfieldFileEncoderManager;
class HeightfieldFileEncoderRawBase;
class HeightfieldFileEncoderUnrealBMPG16;
class HeightfieldGeom;
class HeightfieldGeomManager;
class HeightfieldGeomMaterial;
class HeightfieldGeomMaterialSplat;
class HeightfieldGeomTile;
class HeightfieldGeomTileBufferFactory;
class HeightfieldGeomTileIndexBufferManager;
class HeightfieldGeomTileVertexUVBufferManager;
class HeightfieldOperation;
class HeightfieldOperationBrush;
class HeightfieldOperationBrushBase;
class HeightfieldOperationCPU;
class HeightfieldOperationCPUBrush2;
class HeightfieldOperationCPUBrush;
class HeightfieldOperationCPUErosion;
class HeightfieldOperationCPULevel;
class HeightfieldOperationCPUNoise;
class HeightfieldOperationCPUSmooth;
class HeightfieldOperationFactory;
class HeightfieldOperationGPUBrush;
class HeightfieldOperationGPUNoise;
class HeightfieldOperationGPUNoiseDrag;
class HeightfieldOperationGPUPencil;
class HeightfieldOperationPolicyBrushInstancer;
template <class SubClass, class ParentClass> class HeightfieldOperationPolicyBrushInstancerSpacer;
class HeightfieldOperationPolicyScheduler;
template <class SubClass, class ParentClass> class HeightfieldOperationPolicySchedulerTaskDirect;
template <class SubClass, class ParentClass> class HeightfieldOperationPolicySchedulerTaskPageBatch;
class HeightfieldOperationStack;
class HeightfieldOperationTaskDirect;
class HeightfieldOperationTaskPage;
class HeightfieldOperationTaskPageGPU;
class HeightfieldOperationTaskPageGPUBrush;
class HeightfieldOperationTaskPageGPUListener;
class InputListener;
class InputManager;
class InputPointer;
class PointerState;
class QtJSONButtonDefinitionDataAccesObject;
class QtJSONSettingsDatasetDataAccessObject;
class RenderView;
class RenderViewManager;
class SettingsDataset;
class SettingsDatasetDataAccessObject;
class SettingsDatasetManager;
class ShaderCustomAutoConstants;
class SkySettings;
class SkySettingsListener;
class TabletInput;
class TabletInputWin32;
class Tickable;
class TickableManager;
/*template <class T> class TMatrix4;
template <class T> class TVector2;
template <class T> class TVector3;
template <class T> class TVector4;*/
class UIElementContainer;
class UIElementContainerSimple;
class UIElementPresetContainer;
class UIElementPresetContainerSimple;

namespace HeightfieldGeomTileIndexBufferManagerNS
{
struct CachedBufferDescription;
struct CachedBufferReference;
}

namespace HeightfieldGeomTileVertexUVBufferManagerNS
{
struct CachedBufferDescription;
struct CachedBufferReference;
}

namespace Utils
{
struct BitmapFileHeader;
struct BitmapInfoHeader;
/*template <class InType, class OutType, class ConversionType = OutType> class ConverterFunctorBias;
template <class Type> class ConverterFunctorPassThrough;
template <class InType, class OutType, class ConversionType = OutType> class ConverterFunctorScale;
template <class InType, class OutType, class ConversionType = OutType> class ConverterFunctorScaleBias;*/
template <class T1, class T2> struct MapStruct;
template <class MapStructType, class ValueType> class MapStructReadIterator;
class MooreNeighborhood;
/*template <class InType, class OutType> class NormalizedScalarConverterFunctor;
template <class InOutType> class NormalizedScalarConverterFunctor<InOutType, InOutType>;
template <> class NormalizedScalarConverterFunctor<Ogre::Real, Ogre::uint16>;
template <> class NormalizedScalarConverterFunctor<Ogre::Real, short>;
template <> class NormalizedScalarConverterFunctor<Ogre::uint16, Ogre::Real>;
template <> class NormalizedScalarConverterFunctor<Ogre::uint16, short>;
template <> class NormalizedScalarConverterFunctor<short, Ogre::Real>;
template <> class NormalizedScalarConverterFunctor<short, Ogre::uint16>;*/
template <class DataType, class MarkerType> class RLECompressor;
template <class DataType, class MarkerType> class RLEDecompressor;
class SerialMemoryBuffer;
class SerialMemoryBufferReader;
class SerialMemoryBufferWriter;
class VonNeumannNeighborhood;
}
}

#endif // __SCAPEENGINEPREREQUISITES_H__
