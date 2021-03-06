// This header file was auto-generated by ClassMate++
// Created: 16 Sep 2017 12:15:26 pm
// Copyright (c) 2017, HurleyWorks

#pragma once

using Eigen::Vector2f;
using Eigen::Vector3f;
using sabi::LoadData;

class GeoCore; // forward decl for impl

class ActiveGeo
{
    // The Active Object design pattern decouples method execution from
    // method invocation to enhance concurrency and simplify synchronized
    // access to an object that resides in its own thread of control.

 public:
    ActiveGeo();
    ~ActiveGeo();

    // messaging
    MsgSender getMessenger() { return incoming; }
    void done() { getMessenger().send (qms::clear_queue()); }

 private:
    std::unique_ptr<GeoCore> impl;

    // messaging
    MsgReceiver incoming;

    // message arguments
    size_t frameNumber = 0;
    MessageService messengers;
    PropertyService properties;
    LoadData loadData;
    Vector2f size2D;
    Vector3f position;
    Vector3f direction;

    // state functions
    std::thread stateThread;
    void (ActiveGeo::*state)();
    void waitingForMessages();
    void init();
    void onTick();
    void processLoadData();
    void createGround();

    // state thread function
    void executeState();
    void start();

    bool shutdown = false;

}; // end class ActiveGeo
