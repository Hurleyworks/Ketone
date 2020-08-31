// This source file was auto-generated by ClassMate++
// Created: 16 Sep 2017 12:15:26 pm
// Copyright (c) 2017, HurleyWorks

#include <filesystem>
#include <fast_obj.h>
#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/IStreamReader.h>
#include <GLTFSDK/IStreamWriter.h>
#include <GLTFSDK/GLTFResourceReader.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/BufferBuilder.h>
#include <GLTFSDK/GLTFResourceWriter.h>
#include <GLTFSDK/IStreamWriter.h>
#include <GLTFSDK/MeshPrimitiveUtils.h>
#include <GLTFSDK/Serialize.h>
#include <GLTFSDK/Deserialize.h>

#include "tools/GLTFSDKutil.h"
#include "tools//GLTFloader.h"
#include "tools/GLTFsaver.h"

#include "IO.h"
#include "ActiveIO.h"

using namespace qms;

// ctor
ActiveIO::ActiveIO() :
    impl (new IO()),
    stateThread()
{
    // https://rafalcieslak.wordpress.com/2014/05/16/c11-stdthreads-managed-by-a-designated-class/
    start();
}

// dtor
ActiveIO::~ActiveIO()
{
    done();
    if (stateThread.joinable())
        stateThread.join();
}

// start
void ActiveIO::start()
{
    stateThread = std::thread (&ActiveIO::executeState, this);
}

// executeState
void ActiveIO::executeState()
{
    LOG (DBUG) << "ActiveIO thread is starting up";

    state = &ActiveIO::waitingForMessages;

    while (!shutdown)
    {
        (this->*state)();
    }

    LOG (DBUG) << "ActiveIO thread is shutting down";
}

// waitingForMessages
void ActiveIO::waitingForMessages()
{
    incoming.wait()
        .handle<qms::clear_queue> ([&] (qms::clear_queue const& msg) {
            shutdown = true;
        })
        .handle<QMS::init> ([&] (QMS::init const& msg) {
            messengers = msg.messengers;
            properties = msg.properties;
            state = &ActiveIO::init;
        })
        .handle<QMS::loadMesh> ([&] (QMS::loadMesh const& msg) {
            meshPath = msg.path;
            meshOptions = msg.options;

            state = &ActiveIO::loadMesh;
        })
        .handle<QMS::tick> ([&] (QMS::tick const& msg) {
            frameNumber = msg.frame;
            state = &ActiveIO::onTick;
        });
}

// init
void ActiveIO::init()
{
    impl->init (messengers, properties);
    state = &ActiveIO::waitingForMessages;
}

// onTick
void ActiveIO::onTick()
{
    try
    {
        impl->tick (frameNumber);
    }
    catch (std::exception& e)
    {
        LOG (CRITICAL) << e.what();
        messengers.dreamer.send (QMS::onError (e.what()));
    }
    catch (...)
    {
        LOG (CRITICAL) << "Caught unknown exception!";
        messengers.dreamer.send (QMS::onError ("Caught unknown exception!"));
    }

    state = &ActiveIO::waitingForMessages;
}

// loadMesh
void ActiveIO::loadMesh()
{
    try
    {
        impl->loadMesh (meshPath, meshOptions);
    }
    catch (std::exception& e)
    {
        LOG (CRITICAL) << e.what();
        messengers.dreamer.send (QMS::onError (e.what()));
    }
    catch (...)
    {
        LOG (CRITICAL) << "Caught unknown exception!";
        messengers.dreamer.send (QMS::onError ("Caught unknown exception!"));
    }

    state = &ActiveIO::waitingForMessages;
}