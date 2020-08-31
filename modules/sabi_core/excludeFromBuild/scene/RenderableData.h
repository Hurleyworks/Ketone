// This header file was auto-generated by ClassMate++
// Created: 16 Sep 2017 5:37:16 pm
// Copyright (c) 2017, HurleyWorks

#pragma once

static const char* RenderableBodyTypeTable[] =
    {
        "None",
        "Invalid"};

struct RenderableBodyType
{
    enum ERenderableBodyType
    {
        None,
        Fluid,

        Count,
        Invalid = Count
    };

    union {
        ERenderableBodyType name;
        unsigned int value;
    };

    RenderableBodyType (ERenderableBodyType name) :
        name (name) {}
    RenderableBodyType (unsigned int value) :
        value (value) {}
    RenderableBodyType() :
        value (Invalid) {}
    operator ERenderableBodyType() const { return name; }
    const char* toString() const { return RenderableBodyTypeTable[value]; }
};

static const char* RenderableShapeTable[] =
    {
        "Ball",
        "Box",
        "ConvexHull",
        "Mesh",
        "Composite",
        "Compound",
        "Invalid"};

struct RenderableShape
{
    enum ERenderableShape
    {
        Ball,
        Box,
        ConvexHull,
        Mesh,
        Composite,
        Compound,
        Count,
        Invalid = Count
    };

    union {
        ERenderableShape name;
        unsigned int value;
    };

    RenderableShape (ERenderableShape name) :
        name (name) {}
    RenderableShape (unsigned int value) :
        value (value) {}
    RenderableShape() :
        value (Invalid) {}
    operator ERenderableShape() const { return name; }
    const char* toString() const { return RenderableShapeTable[value]; }
};

static const char* PrimitiveTypeTable[] =
    {
        "Box",
        "Ball",
        "Cylinder",
        "Capsule",
        "Torus",
        "Bunny",
        "Robot"
        "Nozzle",
        "Invalid"};

struct PrimitiveType
{
    enum EPrimitiveType
    {
        Box,
        Ball,
        Cylinder,
        Capsule,
        Torus,
        Bunny,
        Robot,
        Nozzle,
        Count,
        Invalid = Count
    };

    union {
        EPrimitiveType name;
        unsigned int value;
    };

    PrimitiveType (EPrimitiveType name) :
        name (name) {}
    PrimitiveType (unsigned int value) :
        value (value) {}
    PrimitiveType() :
        value (Invalid) {}
    operator EPrimitiveType() const { return name; }
    const char* toString() const { return PrimitiveTypeTable[value]; }
};

static const char* EmitterTypeTable[] =
    {
        "Zaxis",
        "Vertices",
        "None",
        "Invalid"};

struct EmitterType
{
    enum EEmitterType
    {
        Zaxis,
        Vertices,
        None,
        Count,
        Invalid = Count
    };

    union {
        EEmitterType name;
        unsigned int value;
    };

    EmitterType (EEmitterType name) :
        name (name) {}
    EmitterType (unsigned int value) :
        value (value) {}
    EmitterType() :
        value (Invalid) {}
    operator EEmitterType() const { return name; }
    const char* ToString() const { return EmitterTypeTable[value]; }
};

const EmitterType DEFAULT_EMITTER_TYPE = EmitterType::Zaxis;

// body  properties
const RenderableBodyType DEFAULT_BODY_TYPE = RenderableBodyType (RenderableBodyType::None);
const RenderableShape DEFAULT_COLLISION_SHAPE = RenderableShape (RenderableShape::ConvexHull);
const double DEFAULT_DYNAMIC_MASS = 2.0;
const double DEFAULT_STATIC_MASS = 0.0;
const double DEFAULT_STATIC_FRICTION = 0.5;
const double DEFAULT_BOUNCINESS = 0.1;
const double DEFAULT_RIGID_ADHESION = 0.0;
const Vector3f DEFAULT_FORCE = Vector3f (0.0f, -9.8, 0.0f);
//
//const RenderableShape DEFAULT_SHAPE = RenderableShape::ConvexHull;
//const RenderableBodyType DEFAULT_TYPE = RenderableBodyType::Dynamic;

//const double DEFAULT_STATIC_MASS = 0.0;
//const double DEFAULT_RIGID_ADHESION = 0.0;
//const double DEFAULT_DYNAMIC_MASS = 1.0;
//const double DEFAULT_STATIC_FRICTION = .25;
//const double DEFAULT_BOUNCINESS = 0.0;
const double DEFAULT_EMIT_SPEED = 0.0;
const Eigen::Vector3d DEFAULT_RIGID_FORCE = Eigen::Vector3d (0.0, -10.0, 0.0);
const Eigen::Vector3d DEFAULT_EMIT_DIRECTION = Eigen::Vector3d (0.0, 0.0, 0.0);