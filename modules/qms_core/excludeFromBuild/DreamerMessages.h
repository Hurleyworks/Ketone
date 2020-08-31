#pragma once

using sabi::InputEventRef;

struct init
{
    init (const MessageService& messengers, const PropertyService& properties) :
        messengers (messengers),
        properties (properties)
    {
    }
    QmsID id = QmsID::Init;
    MessageService messengers;
    PropertyService properties;
};

struct onError
{
    onError (const std::string& message, ErrorSeverity severity = ErrorSeverity::Warning) :
        errorMessage (message),
        level (severity)
    {
    }

    QmsID id = QmsID::TopPriority;
    std::string errorMessage;
    ErrorSeverity level;
};

struct tick
{
    tick (uint32_t frameNumber = 0) :
        frame (frameNumber)
    {
    }

    QmsID id = QmsID::Tick;
    uint32_t frame = 0;
};

struct onInput
{
    onInput (const InputEventRef& input) :
        input (input)
    {
    }

    // must be responsive to user input!
    QmsID id = QmsID::TopPriority;
    InputEventRef input;
};