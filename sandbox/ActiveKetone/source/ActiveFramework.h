#pragma once

#include "ActiveDreamer.h"
#include <render_core/render_core.h>
#include <io_core/io_core.h>
#include <geo_core/geo_core.h>
#include <world_core/world_core.h>

struct ActiveFramework
{
    ActiveFramework() = default;
    ~ActiveFramework() = default;

    void init (PropertyService& properties)
    {
        // initialize messengers
        messengers.dreamer = dreamer.getMessenger();
        messengers.render = render.getMessenger();
        messengers.io = io.getMessenger();
        messengers.geometry = geometry.getMessenger();
        messengers.world = world.getMessenger();

        // initialize modules
        messengers.dreamer.send (QMS::init (messengers, properties));
        messengers.render.send (QMS::init (messengers, properties));
        messengers.io.send (QMS::init (messengers, properties));
        messengers.geometry.send (QMS::init (messengers, properties));
        messengers.world.send (QMS::init (messengers, properties));
    }

    // active object modules
    ActiveDreamer dreamer;
    ActiveRender render;
    ActiveIO io;
    ActiveGeo geometry;
    ActiveWorld world;

    MessageService messengers;

    void shutdown();
};
