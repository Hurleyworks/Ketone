
// This header file was auto-generated by ClassMate++
// Created: 1 Aug 2019 10:48:43 am
// Copyright (c) 2019, HurleyWorks

#pragma once

#include "../AppConfig.h"

using sabi::InputEvent;

class InputHandler : public CsSignal::SignalBase
{
 public:
    SIGNAL_1 (void onEvent (InputEvent input))
    SIGNAL_2 (onEvent, input)

    SIGNAL_1 (void onDragAndDrop (const std::vector<std::string>& paths))
    SIGNAL_2 (onDragAndDrop, paths)

    SIGNAL_1 (void windowResize (int newWidth, int newHeight))
    SIGNAL_2 (windowResize, newWidth, newHeight)

 public:
    InputHandler() 
    {
  
    }
    ~InputHandler() = default;

    bool windowIsOpen() const { return windowOpen; }
    void onWindowClose() { windowOpen = false; }

    void consumeCharacter (uint32_t codepoint)
    {
        // LOG (DBUG) << "Codepoint: " << codepoint;

        // input.setKey (codepoint);

        // emit the event
        // onEvent (input);
    }
    void consumeKey (int key, int scanCode, int action, int mods)
    {
        /*LOG (DBUG) << "Key: " << key << "  "
                   << "Action: " << action;*/
        InputEvent::Type type = InputEvent::Idle;

        switch (action)
        {
            case 0:
                type = InputEvent::KeyRelease;
                break;

            case 1:
                type = InputEvent::KeyPress;
                break;

            case 2:
                type = InputEvent::KeyRepeat;
                break;
        }

        input.setKey (key);
        input.setScanCode (scanCode);
        input.setType (type);
        input.setKeyboardModifiers (mods);
        onEvent (input);
    }

    void onCursorPos (double xPos, double yPos)
    {
        input.setX (xPos);
        input.setY (yPos);

        InputEvent::Type type = input.getType();
        if (type == InputEvent::Type::Press || type == InputEvent::Drag)
        {
            input.setType (InputEvent::Drag);
        }
        else
        {
           // LOG(DBUG) << "MOVING ";
            input.setType (InputEvent::Move);
        }

        // emit the event
        onEvent (input);
    }

    void onMouseButtonEvent (int button, int action, int mods)
    {
        switch (button)
        {
            case MOUSE_BUTTON_LEFT:
                input.setButton (InputEvent::MouseButton::Left);
                break;
            case MOUSE_BUTTON_RIGHT:
                input.setButton (InputEvent::MouseButton::Right);
                break;
            case MOUSE_BUTTON_MIDDLE:
                input.setButton (InputEvent::MouseButton::Middle);
                break;
            default:
                break;
        }

        switch (action)
        {
            case MOUSE_PRESS:
                input.setType (InputEvent::Press);
                break;
            case MOUSE_RELEASE:
                input.setType (InputEvent::Release);
                break;
            case MOUSE_REPEAT:
                input.setType (InputEvent::DblClick); // FIXME is this correct?
                break;
            default:
                break;
        }

        // emit the event
        onEvent (input);
    }

    void onDrop (const std::vector<std::string>& fileList)
    {
        // emit the drop
        onDragAndDrop (fileList);
    }

    void onScroll (double xOffset, double yOffset)
    {
        input.setType (yOffset > 0.0 ? InputEvent::ScrollUp : InputEvent::ScrollDown);

        // emit the event
        onEvent (input);
    }

    void onWindowResize (int newWidth, int newHeight)
    {
        windowResize (newWidth, newHeight);
    }

 private:
     InputEvent input;
    bool windowOpen = true;
    uint32_t lastChar = 0;
};