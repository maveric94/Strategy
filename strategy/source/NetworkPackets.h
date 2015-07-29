#ifndef NETWORKPACKETS_H
#define NETWORKPACKETS_H

namespace Client
{
	enum PacketType
	{
		UnitAction,
		Quit,
		GameStart,
		CreateUnit,
		DestroyUnit,
	};
};

namespace Server 
{
	enum PacketType
	{
		InitData,
		AddPlayer,
		Disconnect,
		GameStart,
		CreateUnit,
		UnitAction,
		DestroyUnit,
	};
};

#endif