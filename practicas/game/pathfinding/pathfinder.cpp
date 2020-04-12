#include <stdafx.h>
#include <ctype.h>

#include "pathfinder.h"



Pathfinder::Pathfinder() : MOAIEntity2D()
{
	RTTI_BEGIN
		RTTI_EXTEND(MOAIEntity2D)
	RTTI_END
	LoadGrid("map.txt");
	InitializeNodes();
}

Pathfinder::~Pathfinder()
{

}

void Pathfinder::UpdatePath()
{
	std::vector<Node> tOpenNodes;
	std::vector<Node> tClosedNodes;
}

void Pathfinder::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	for (int i = 0; i < m_iGridWidth; i++)
	{
		for (int j = 0; j < m_tGrid.size() / m_iGridWidth; j++)
		{
			char c = GetTileChar(i, j);
			if (c == '#')
			{
				gfxDevice.SetPenColor(0.2f, 0.2f, 0.8f, 0.2f);
				USRect tileRect = GetTileRect(i, j);
				MOAIDraw::DrawRectFill(tileRect);
			}
			if (c == 'A')
			{
				gfxDevice.SetPenColor(0.2f, 0.8f, 0.2f, 0.2f);
				USRect tileRect = GetTileRect(i, j);
				MOAIDraw::DrawRectFill(tileRect);
			}
			if (c == 'B')
			{
				gfxDevice.SetPenColor(0.4f, 0.6f, 0.2f, 0.2f);
				USRect tileRect = GetTileRect(i, j);
				MOAIDraw::DrawRectFill(tileRect);
			}
			if (c == 'C')
			{
				gfxDevice.SetPenColor(0.6f, 0.4f, 0.2f, 0.2f);
				USRect tileRect = GetTileRect(i, j);
				MOAIDraw::DrawRectFill(tileRect);
			}
			if (c == 'D')
			{
				gfxDevice.SetPenColor(0.8f, 0.2f, 0.2f, 0.2f);
				USRect tileRect = GetTileRect(i, j);
				MOAIDraw::DrawRectFill(tileRect);
			}
		}
		USVec2D vEndPosition = GetEndPosition();
		gfxDevice.SetPenColor(0.8f, 0.2f, 0.8f, 0.2f);
		MOAIDraw::DrawEllipseFill(vEndPosition.mX + m_iTileSize/2, vEndPosition.mY + m_iTileSize/2, m_iTileSize/2, m_iTileSize/2, 16);

		if (m_tPathPoints.size() > 0)
		{
			gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.5f);
			USVec2D vPreviousPoint = m_tPathPoints[0];
			for (int i = 1; i < m_tPathPoints.size(); i++)
			{
				MOAIDraw::DrawLine(vPreviousPoint, m_tPathPoints[i]);
				vPreviousPoint = m_tPathPoints[i];
			}

		}
	}

}

bool Pathfinder::PathfindStep()
{
    // returns true if pathfinding process finished
    return true;
}

void Pathfinder::LoadGrid(string _sPath)
{
	ifstream myReadFile;
	myReadFile.open(_sPath);
	if (myReadFile.is_open())
	{
		char c;
		while (myReadFile.get(c))
		{
			if (c == 'A'|| c == 'B' || c == 'C' || c == 'D' || c == '#')
			{
				m_tGrid.push_back(c);
			}
		}
	}
	myReadFile.close();
}
void Pathfinder::InitializeNodes()
{
	m_tNodes.clear();
	for (int i = 0; i < m_tGrid.size(); i++)
	{
		char c = m_tGrid[i];
		Node node;
		node.m_iX = i % m_iGridWidth;
		node.m_iY = i / m_iGridWidth;
		if (c == '#')
		{
			node.m_bBlocked = true;
		}
		else if (c == 'A')
		{
			node.m_bBlocked = false;
			node.m_iCost = 10;
		}
		else if (c == 'B')
		{
			node.m_bBlocked = false;
			node.m_iCost = 20;
		}
		else if (c == 'C')
		{
			node.m_bBlocked = false;
			node.m_iCost = 30;
		}
		else if (c == 'D')
		{
			node.m_bBlocked = false;
			node.m_iCost = 40;
		}
		m_tNodes.push_back(node);
	}
}
char Pathfinder::GetTileChar(int _iX, int _iY)
{
	if (_iX > m_iGridWidth || _iX < 0 || m_iGridWidth == 0)
	{
		return 'x';
	}
	else if (_iY > m_tGrid.size() / m_iGridWidth || _iY < 0)
	{
		return 'y';
	}
	else
	{
		int iCharLocation = _iY * m_iGridWidth + _iX;
		return m_tGrid[iCharLocation];
	}
}
USRect Pathfinder::GetTileRect(int _iX, int _iY)
{
	USRect tileRect;
	tileRect.mXMin = m_iTileSize * _iX;
	tileRect.mXMax = m_iTileSize * (_iX + 1);
	tileRect.mYMin = m_iTileSize * _iY;
	tileRect.mYMax = m_iTileSize * (_iY + 1);
	return tileRect;

}













//lua configuration ----------------------------------------------------------------//
void Pathfinder::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIEntity::RegisterLuaFuncs(state);

	luaL_Reg regTable [] = {
		{ "setStartPosition",		_setStartPosition},
		{ "setEndPosition",			_setEndPosition},
        { "pathfindStep",           _pathfindStep},
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

int Pathfinder::_setStartPosition(lua_State* L)
{
	MOAI_LUA_SETUP(Pathfinder, "U")

	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetStartPosition(pX, pY);
	return 0;
}

int Pathfinder::_setEndPosition(lua_State* L)
{
	MOAI_LUA_SETUP(Pathfinder, "U")

	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetEndPosition(pX, pY);
	return 0;
}


int Pathfinder::_pathfindStep(lua_State* L)
{
    MOAI_LUA_SETUP(Pathfinder, "U")

    self->PathfindStep();
    return 0;
}