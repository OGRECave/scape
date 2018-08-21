/**
 * ScapeEngine::ConverterFunctor conversion utility functions
 *
 * Collection of useful templated conversions between different formats
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __CONVERTERFUNCTOR_H__
#define __CONVERTERFUNCTOR_H__

namespace ScapeEngine
{
namespace Utils
{
// ----------------------------------------------------------------------------
template <class Type> class ConverterFunctorPassThrough
{
public:
    typedef Type TypeIn;
    typedef Type TypeOut;

    TypeOut operator()(const TypeIn& in) const { return in; }
};

// ----------------------------------------------------------------------------
template <class InType, class OutType, class ConversionType = OutType> class ConverterFunctorScaleBias
{
public:
    typedef InType TypeIn;
    typedef OutType TypeOut;
    ConversionType mScale;
    ConversionType mBias;

    ConverterFunctorScaleBias(ConversionType scale, ConversionType bias) : mScale(scale), mBias(bias) {}

    TypeOut operator()(const TypeIn in) const { return (TypeOut)((ConversionType)in * mScale + mBias); }

public:
};

// ----------------------------------------------------------------------------
template <class InType, class OutType, class ConversionType = OutType> class ConverterFunctorScale
{
public:
    typedef InType TypeIn;
    typedef OutType TypeOut;
    ConversionType mScale;

    ConverterFunctorScale(ConversionType scale) : mScale(scale) {}

    TypeOut operator()(const TypeIn in) const { return (TypeOut)((ConversionType)in * mScale); }

public:
};

// ----------------------------------------------------------------------------
template <class InType, class OutType, class ConversionType = OutType> class ConverterFunctorBias
{
public:
    typedef InType TypeIn;
    typedef OutType TypeOut;
    ConversionType mBias;

    ConverterFunctorBias(ConversionType bias) : mBias(bias) {}

    TypeOut operator()(const TypeIn in) const { return (TypeOut)((ConversionType)in + mBias); }

public:
};

// ----------------------------------------------------------------------------
template <class InType, class OutType> class NormalizedScalarConverterFunctor
{
};

// ----------------------------------------------------------------------------
template <class InOutType>
class NormalizedScalarConverterFunctor<InOutType, InOutType> : public ConverterFunctorPassThrough<InOutType>
{
};

// ----------------------------------------------------------------------------
template <>
class NormalizedScalarConverterFunctor<Ogre::uint16, short>
    : public ConverterFunctorBias<Ogre::uint16, short>
{
public:
    NormalizedScalarConverterFunctor() : ConverterFunctorBias(-32768) {}
};

template <>
class NormalizedScalarConverterFunctor<Ogre::uint16, Ogre::Real>
    : public ConverterFunctorScale<Ogre::uint16, Ogre::Real>
{
public:
    NormalizedScalarConverterFunctor() : ConverterFunctorScale(1.0f / 65535.0f) {}
};

// ----------------------------------------------------------------------------
template <>
class NormalizedScalarConverterFunctor<short, Ogre::uint16>
    : public ConverterFunctorBias<short, Ogre::uint16>
{
public:
    NormalizedScalarConverterFunctor() : ConverterFunctorBias(32768) {}
};

template <>
class NormalizedScalarConverterFunctor<short, Ogre::Real>
    : public ConverterFunctorScaleBias<short, Ogre::Real>
{
public:
    NormalizedScalarConverterFunctor() : ConverterFunctorScaleBias(1.0f / 65535.0f, -32768.0f / 65535.0f) {}
};

// ----------------------------------------------------------------------------
template <>
class NormalizedScalarConverterFunctor<Ogre::Real, short>
    : public ConverterFunctorScaleBias<Ogre::Real, short, Ogre::Real>
{
public:
    NormalizedScalarConverterFunctor() : ConverterFunctorScaleBias(65535.0f, -32768.0f) {}
};

template <>
class NormalizedScalarConverterFunctor<Ogre::Real, Ogre::uint16>
    : public ConverterFunctorScale<Ogre::Real, Ogre::uint16, Ogre::Real>
{
public:
    NormalizedScalarConverterFunctor() : ConverterFunctorScale(65535.0f) {}
};
}
}

#endif // __CONVERTERFUNCTOR_H__
