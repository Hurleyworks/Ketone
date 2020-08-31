#pragma once

// quicksilver messenger service
#include "QmsData.h"
#include "QmsQueue.h"
#include "QmsSender.h"
#include "QmsDispatcherT.h"
#include "QmsDispatcher.h"
#include "QmsReceiver.h"

using MsgSender = qms::sender;
using MsgReceiver = qms::receiver;

struct MessageService
{
	MsgSender dreamer;
	MsgSender io;
	MsgSender geometry;
	MsgSender world;
    MsgSender render;
};

namespace qms 
{
	namespace activeMsg
	{
		#include "DreamerMessages.h"
		#include "IOmessages.h"
		#include "WorldMessages.h"
		#include "GeometryMessages.h"
		#include "RenderMessages.h"
	}
}

#define QMS qms::activeMsg