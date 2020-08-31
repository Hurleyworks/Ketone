#pragma once

struct PropertyService
{
    WorldPropsRef worldProps = std::make_shared<WorldProperties>();
    RenderPropsRef renderProps = std::make_shared<RenderProperties>();
    IOPropsRef ioProps = std::make_shared<IOProperties>();
};