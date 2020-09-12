
// This source file was auto-generated by ClassMate++
// Created: 28 Feb 2018 8:30:14 am
// Copyright (c) 2018, HurleyWorks

using sabi::InputEvent;
using sabi::PixelBufferHandle;
using sabi::PRenderableState;

RenderCore::~RenderCore()
{
    // order is important!
    renderer.reset();
    material.reset();
    mesh.reset();
    pipeline.reset();
    state.reset();
}

// init
void RenderCore::init (MessageService messengers, const PropertyService& properties)
{
    this->messengers = messengers;
    this->properties = properties;

    state = OptiXEnv::create();

    mesh = MeshHandler::create (state);
    material = MaterialHandler::create (state);
    motion = MotionHandler::create (state);
    noise = NoiseHandler::create (state);
    pipeline = SimplestPipeline::create (state);
    scene = SceneHandler::create (state);
    renderer = SimpleRenderer::create (state);
}

void RenderCore::initEngine (CameraHandle& camera)
{
    nvcc.compile (properties);

    const std::string& ptxFolder = properties.renderProps->getVal<std::string> (RenderKey::PtxFolder);
    pipeline->createPipeline (ptxFolder);
    renderer->initialize (camera);

    scene->updateState (pipeline, true, true, false);
}

void RenderCore::render (CameraHandle& camera, uint32_t frameNumber)
{
    if (camera->isDirty())
    {
        renderer->updateCamera (camera);
        camera->setDirty (false);
    }
    if (camera->getPixelBuffer().spec.dirty)
    {
        renderer->onResize (camera);
        camera->getPixelBuffer().spec.dirty = false;
        camera->setDirty (false);
    }
    float renderTime = renderer->render (camera, pipeline);
    properties.renderProps->setValue (RenderKey::RenderTime, static_cast<double> (renderTime * 0.001f));
}

void RenderCore::onInput (InputEvent& input)
{
    // picking is with RMB press or drag
    if (input.getButton() == InputEvent::Right && (input.getType() == InputEvent::Press || input.getType() == InputEvent::Drag))
    {
        RenderableNode picked = renderer->pick (input.getX(), input.getY(), pipeline);
        if (picked)
        {
            LOG (DBUG) << picked->getName() << " was picked";

            // toggle selection state
            if (picked->getState().isSelected())
            {
                // lower the selected flag
                picked->getState().state ^= PRenderableState::Selected;
            }
            else
            {
                // raise the selected flag
                picked->getState().state |= PRenderableState::Selected;
            }

            mesh->onSelectionStateChanged (picked);
            scene->updateState(pipeline, true, true, false);
            input.setPickedNode (picked);
        }
    }
}

void RenderCore::addRenderableNode (RenderableNode& node)
{
    if (!node->isInstance())
    {
        GeometryInstanceRef geomInst = mesh->addRenderableGeometry (node);
        uint32_t materialSetCount = material->addRenderableMaterials (node, geomInst, pipeline);
        // FIXME maybe we want to just add a GeometryInstance to an existing Geometry group ???
        GeometryGroupRef geomGroup = mesh->createGeometryGroup (node, materialSetCount, geomInst, pipeline);
        InstanceRef inst = mesh->createInstance (node, geomGroup);
        scene->addInstance (node, inst, pipeline);
    }
    else
    {
        // lower the selected flag if neccessary
        if (node->getState().isSelected())
            node->getState().state ^= PRenderableState::Selected;
        
        // instances use their "instanced from" geometry data
        RenderableNode source = node->getInstancedFrom();
        if (!source) throw std::runtime_error ("Not a valid instance: " + node->getName());

        auto it = state->geomGroups.find (source->getClientID());
        if (it != state->geomGroups.end())
        {
            GeometryGroupRef geomGroup = it->second;
            InstanceRef inst = mesh->createInstance (node, geomGroup);
            scene->addInstance (node, inst, pipeline);
        }
        else
        {
            throw std::runtime_error ("Could not find geometry group for " + source->getName());
        }
    }
}
