#include "MessageDispatcher.h"

void MessageDispatcher::OnLoad()
{
	localTick = 0;
}

void MessageDispatcher::DispatchMessage(const Ptr<Entity>& receiver, const Ptr<Message>& message, int delay)
{
	if (delay <= 0)
	{
		receiver->OnReceiveMessage(message);
	}
	else
	{
		MessageContainer msgC;
		msgC.message = message;
		msgC.receiver = receiver;
		msgC.timeToSend = localTick + delay * EntityManager::TickRate;
		this->pending.Add(msgC);
	}
}

void MessageDispatcher::FixedUpdate()
{
	localTick++;

	for (int i = 0; i < pending.Size(); i++)
	{
		auto& item = pending[i];
		if (item.timeToSend <= localTick)
		{
			item.receiver->OnReceiveMessage(item.message);
			pending.Erase(i);
		}
	}
}
