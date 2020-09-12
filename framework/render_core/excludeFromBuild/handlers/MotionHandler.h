// This header file was auto-generated by ClassMate++
// Created: 8 Sep 2020 8:44:40 pm
// Copyright (c) 2020, HurleyWorks

#pragma once

using MotionHandlerRef = std::shared_ptr<class MotionHandler>;

class MotionHandler
{
 public:
    static MotionHandlerRef create (OptiXStateRef state) { return std::make_shared<MotionHandler> (state); }

 public:
    MotionHandler (OptiXStateRef state);
    ~MotionHandler();

 private:
    OptiXStateRef state = nullptr;

}; // end class MotionHandler
