// This header file was auto-generated by ClassMate++
// Created: 23 Jan 2018 7:09:22 pm
// Copyright (c) 2018, HurleyWorks

#pragma once

using LoadStrategyHandle = std::unique_ptr<class LoadStrategy>;
using sabi::SpaceTime;

class LoadStrategy
{

 public:
	virtual ~LoadStrategy() {}

	LoadStrategy & operator=(const LoadStrategy&) = delete;
	LoadStrategy(const LoadStrategy&) = delete;

	virtual void addNextItem(SpaceTime& spacetime) = 0;
	virtual void reset() = 0;
    virtual void incrementCount() = 0;
		
 protected:
	 LoadStrategy() {}

}; // end class AbstractLoadStrategy
