#pragma once

#include "Singleton.h"
#include "BaseType.h"

class ConnectServer : public Singleton<ConnectServer>
{
public:
	ConnectServer();
	~ConnectServer();

	virtual bool Init(int32 nArgc, char* argv[]);
};

