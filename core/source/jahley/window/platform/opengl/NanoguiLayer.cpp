// This source file was auto-generated by ClassMate++
// Created: 17 Aug 2019 3:42:21 pm
// Copyright (c) 2019, HurleyWorks

#include "berserkpch.h"
#include "NanoguiLayer.h"

using namespace nanogui;

// ctor
NanoguiLayer::NanoguiLayer (GLFWwindow* window, const PropertyService& properties, CameraHandle camera) :
    RenderLayer (properties, camera),
    window (window)
{
    initGraph (&fpsGraph, GRAPH_RENDER_FPS, "App Time");
    initGraph (&cpuGraph, GRAPH_RENDER_FPS, "Render Time");

    init();
}

// dtor
NanoguiLayer::~NanoguiLayer()
{
}

void NanoguiLayer::init()
{
    Screen::initialize (window, false);

    Screen::drawAll();
    setVisible (true);
}

void NanoguiLayer::onUpdate()
{
    drawContents();
    drawWidgets();
}

bool NanoguiLayer::onInput (const InputEvent& input)
{
    switch (input.getType())
    {
        case InputEvent::Press:
            //LOG(DBUG) << "PRESS";
            return Screen::mouseButtonCallbackEvent (input.getButton(), MOUSE_PRESS, 0);

            break;

        case InputEvent::Release:
            //LOG(DBUG) << "RELEASE";
            return Screen::mouseButtonCallbackEvent (input.getButton(), MOUSE_RELEASE, 0);

        case InputEvent::Move:
            //LOG(DBUG) << "MOVE";
            return Screen::cursorPosCallbackEvent ((float)input.getX(), (float)input.getY());

        case InputEvent::Drag:
            //LOG(DBUG) << "DRAG";
            Screen::cursorPosCallbackEvent ((float)input.getX(), (float)input.getY());

            // calling just the cursorPosCallbackEvent() was the cause of the gui event mess
            // we need to call mouseButtonCallbackEvent() during drags too
            return Screen::mouseButtonCallbackEvent (input.getButton(), MOUSE_PRESS, 0);


        case InputEvent::KeyPress:
        {
            if (Screen::keyboardEvent (input.getKey(), input.getScanCode(), MOUSE_PRESS, input.getKeyboardModifiers()))
            {
                return Screen::keyboardCharacterEvent (input.getKey());
            }
            else
                return false;
        }
        case InputEvent::KeyRelease:
        {
            return Screen::keyboardEvent (input.getKey(), input.getScanCode(), MOUSE_RELEASE, input.getKeyboardModifiers());
        }
    }

    return false;
}

// postInfoMessage
void NanoguiLayer::postInfoMessage (const std::string& title, const std::string& msg)
{
    auto dlg = new MessageDialog (this, MessageDialog::Type::Information, title, msg);
    dlg->setCallback ([] (int result) {

    });
}

// postWarningMessage
void NanoguiLayer::postWarningMessage (const std::string& title, const std::string& msg)
{
    auto dlg = new MessageDialog (this, MessageDialog::Type::Warning, title, msg);
    dlg->setCallback ([] (int result) {

    });
}

// postQuestiongMessage
void NanoguiLayer::postQuestiongMessage (const std::string& title, const std::string& msg, int& answer)
{
    auto dlg = new MessageDialog (this, MessageDialog::Type::Warning, title, msg, "Yes", "No", true);
    dlg->setCallback ([&] (int result) {
        answer = result;
    });
}

void NanoguiLayer::drawPerformanceGraph (NVGcontext* const ctx)
{
    double t, dt;

    t = glfwGetTime();
    dt = t - previousTime;
    previousTime = t;

    int x = size().x() - 600;
    if (x < 5) x = 5;
    int y = size().y() - 35;
    renderGraph (ctx, x, y, &fpsGraph);
    renderGraph (ctx, x + 200 + 5, y, &cpuGraph);

    // render time is in milliseconds
    double renderTime = properties.renderProps->getVal<double> (RenderKey::RenderTime);
    // if it's not hooked up yet make it 60 fps
    if (renderTime <= 0)
        renderTime = .016; 
    
    // Measure the CPU time taken excluding swap buffers (as the swap may wait for GPU)
    cpuTime = glfwGetTime() - t;
    updateGraph (&fpsGraph, dt);
    updateGraph (&cpuGraph, renderTime);

}
