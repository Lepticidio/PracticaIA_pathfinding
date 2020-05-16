#include <stdafx.h>
#include <ctype.h>

#include "pathfinder.h"

bool Pathfinder::m_bPathInitialized = false;
bool Pathfinder::m_bStartInitialized = false;
bool Pathfinder::m_bEndInitialized = false;
bool Pathfinder::m_bPathFound = false;

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
	if(Pathfinder::m_bStartInitialized && Pathfinder::m_bEndInitialized)
	{
		if (!Pathfinder::m_bPathInitialized)
		{
			m_tPathPoints.clear();
			m_tOpenNodes.clear();
			m_tClosedNodes.clear();
			m_vEndPosition = GetEndPosition();
			m_vStartPosition = GetStartPosition();
			m_targetNode = NodeFromPosition(m_vEndPosition.mX, m_vEndPosition.mY);
			m_currentNode = NodeFromPosition(m_vStartPosition.mX, m_vStartPosition.mY);

			for (int i = 0; i < m_tNodes.size(); i++)
			{
				Node* pNode = &m_tNodes[i];
				USVec2D vPositionNode = PositionFromNode(pNode);
				pNode->UpdateH((m_vEndPosition - vPositionNode).Length());
			}
			m_tOpenNodes.push_back(m_currentNode);
			m_currentNode->parent = nullptr;
			m_currentNode->UpdateG(0);
			Pathfinder::m_bPathInitialized = true;
		}
		if (PathfindStep())
		{
			while (m_currentNode->parent != nullptr)
			{
				USVec2D vNodePosition = PositionFromNode(m_currentNode);
				m_tPathPoints.insert(m_tPathPoints.begin(), vNodePosition);
				m_currentNode = m_currentNode->parent;
			}

			USVec2D vNodePosition = PositionFromNode(m_currentNode);
			m_tPathPoints.insert(m_tPathPoints.begin(), vNodePosition);
			m_currentNode = m_currentNode->parent;
		}

	}
}

bool Pathfinder::PathfindStep()
{
	m_iSpaceCounter++;
  	bool bFound = false;
	if (m_tOpenNodes.size() > 0)
	{
		float fSmallestF = std::numeric_limits<float>::max();
		int iIndexCurrent = 0;
		for (int i = 0; i < m_tOpenNodes.size(); i++)
		{
			Node* pNode = m_tOpenNodes[i];
			if (pNode->m_fF < fSmallestF)
			{
				fSmallestF = pNode->m_fF;
				m_currentNode = pNode;
				iIndexCurrent = i;
			}
		}
		m_tOpenNodes.erase(m_tOpenNodes.begin() + iIndexCurrent);
		m_tClosedNodes.push_back(m_currentNode);
		USVec2D vCurrentPosition = PositionFromNode(m_currentNode);
		if (m_currentNode == m_targetNode)
		{

			while (m_currentNode->parent != nullptr)
			{
				USVec2D vNodePosition = PositionFromNode(m_currentNode);
				m_tPathPoints.insert(m_tPathPoints.begin(), vNodePosition);
				m_currentNode = m_currentNode->parent;
			}

			USVec2D vNodePosition = PositionFromNode(m_currentNode);
			m_tPathPoints.insert(m_tPathPoints.begin(), vNodePosition);
			m_currentNode = m_currentNode->parent;
			Pathfinder::m_bPathFound = true;
		}
		int iCounter = 0;
		while (!Pathfinder::m_bPathFound && iCounter < m_currentNode->tNeighbours.size())
		{
			Node* pNode = m_currentNode->tNeighbours[iCounter];
			if (!pNode->m_bBlocked && std::count(m_tClosedNodes.begin(), m_tClosedNodes.end(), pNode) == 0)
			{
				USVec2D vNodePosition = PositionFromNode(pNode);
				float fGValue = (vNodePosition - vCurrentPosition).Length() * (float)pNode->m_iCost + m_currentNode->m_fG;
				if (std::count(m_tOpenNodes.begin(), m_tOpenNodes.end(), pNode) == 0 || fGValue < pNode->m_fG)
				{
					pNode->UpdateG(fGValue);
					pNode->parent = m_currentNode;
					if (std::count(m_tOpenNodes.begin(), m_tOpenNodes.end(), pNode) == 0)
					{
						m_tOpenNodes.push_back(pNode);
					}
				}
			}
			iCounter++;
		}
		return false;
	}
	/*
	else
	{

		while (m_currentNode->parent != nullptr)
		{
			USVec2D vNodePosition = PositionFromNode(m_currentNode);
			m_tPathPoints.insert(m_tPathPoints.begin(), vNodePosition);
			m_currentNode = m_currentNode->parent;
		}

		USVec2D vNodePosition = PositionFromNode(m_currentNode);
		m_tPathPoints.insert(m_tPathPoints.begin(), vNodePosition);
		m_currentNode = m_currentNode->parent;
		Pathfinder::m_bPathFound = true;

	}
	*/
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
				USRect tileRect = GetTileRect(i, j, m_iTileSize, 0);
				MOAIDraw::DrawRectFill(tileRect);
			}
			if (c == 'A')
			{
				gfxDevice.SetPenColor(0.2f, 0.8f, 0.2f, 0.2f);
				USRect tileRect = GetTileRect(i, j, m_iTileSize, 0);
				MOAIDraw::DrawRectFill(tileRect);
			}
			if (c == 'B')
			{
				gfxDevice.SetPenColor(0.4f, 0.6f, 0.2f, 0.2f);
				USRect tileRect = GetTileRect(i, j, m_iTileSize, 0);
				MOAIDraw::DrawRectFill(tileRect);
			}
			if (c == 'C')
			{
				gfxDevice.SetPenColor(0.6f, 0.4f, 0.2f, 0.2f);
				USRect tileRect = GetTileRect(i, j, m_iTileSize, 0);
				MOAIDraw::DrawRectFill(tileRect);
			}
			if (c == 'D')
			{
				gfxDevice.SetPenColor(0.8f, 0.2f, 0.2f, 0.2f);
				USRect tileRect = GetTileRect(i, j, m_iTileSize, 0);
				MOAIDraw::DrawRectFill(tileRect);
			}
		}
		USVec2D vEndPosition = GetEndPosition();
		gfxDevice.SetPenColor(0.8f, 0.2f, 0.8f, 0.8f);
		MOAIDraw::DrawEllipseFill(vEndPosition.mX + m_iTileSize / 2, vEndPosition.mY + m_iTileSize / 2, m_iTileSize / 2, m_iTileSize / 2, 16);

		USVec2D vStartPostion = GetStartPosition();
		gfxDevice.SetPenColor(0.4f, 0.2f, 0.8f, 0.8f);
		MOAIDraw::DrawEllipseFill(vStartPostion.mX + m_iTileSize / 2, vStartPostion.mY + m_iTileSize / 2, m_iTileSize / 2, m_iTileSize / 2, 16);


		if (m_tPathPoints.size() > 0)
		{
			gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.5f);
			USVec2D vOffset(m_iTileSize / 2, m_iTileSize / 2);
			USVec2D vPreviousPoint = m_tPathPoints[0] + vOffset;
			for (int i = 1; i < m_tPathPoints.size(); i++)
			{
				MOAIDraw::DrawLine(vPreviousPoint, m_tPathPoints[i] + vOffset);
				vPreviousPoint = m_tPathPoints[i] + vOffset;
			}

		}
	}

	for (int i = 0; i < m_tOpenNodes.size(); i++)
	{
		Node* pNode = m_tOpenNodes[i];
		gfxDevice.SetPenColor(0.8f, 0.8f, 0.8f, 0.8f);
		USRect tileRect = GetTileRect(pNode->m_iX, pNode->m_iY, m_iTileSize, m_iTileSize / 4);
		MOAIDraw::DrawRectFill(tileRect);
	}

	for (int i = 0; i < m_tClosedNodes.size(); i++)
	{
		Node* pNode = m_tClosedNodes[i];
		gfxDevice.SetPenColor(0.2f, 0.2f, 0.2f, 0.8f);
		USRect tileRect = GetTileRect(pNode->m_iX, pNode->m_iY, m_iTileSize, m_iTileSize / 4);
		MOAIDraw::DrawRectFill(tileRect);
	}
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

	for (int i = 0; i < m_tNodes.size(); i++)
	{
		Node* pNode = &m_tNodes[i];
		if (pNode->m_iX > 0)
		{
			pNode->tNeighbours.push_back(NodeFromIndexes(pNode->m_iX - 1, pNode->m_iY));
			if (pNode->m_iY > 0)
			{
				pNode->tNeighbours.push_back(NodeFromIndexes(pNode->m_iX - 1, pNode->m_iY - 1));
			}
			if (pNode->m_iY < m_tNodes.size() / m_iGridWidth - 1)
			{
				pNode->tNeighbours.push_back(NodeFromIndexes(pNode->m_iX - 1, pNode->m_iY + 1));
			}
		}
		if (pNode->m_iX < m_iGridWidth - 1)
		{
			pNode->tNeighbours.push_back(NodeFromIndexes(pNode->m_iX + 1, pNode->m_iY));
			if (pNode->m_iY > 0)
			{
				pNode->tNeighbours.push_back(NodeFromIndexes(pNode->m_iX + 1, pNode->m_iY - 1));
			}
			if (pNode->m_iY < m_tNodes.size() / m_iGridWidth - 1)
			{
				pNode->tNeighbours.push_back(NodeFromIndexes(pNode->m_iX + 1, pNode->m_iY + 1));
			}
		}
		if (pNode->m_iY > 0)
		{
			pNode->tNeighbours.push_back(NodeFromIndexes(pNode->m_iX, pNode->m_iY - 1));
		}
		if (pNode->m_iY < m_tNodes.size() / m_iGridWidth - 1)
		{
			pNode->tNeighbours.push_back(NodeFromIndexes(pNode->m_iX, pNode->m_iY + 1));
		}
	}
}
Node* Pathfinder::NodeFromIndexes(int _iX, int _iY)
{
	int iLocation = _iY * m_iGridWidth + _iX;
	if (iLocation > 0 && iLocation < m_tNodes.size())
	{
		return &m_tNodes[iLocation];
	}
}
Node* Pathfinder::NodeFromPosition(float _fX, float _fY)
{
	int iX = _fX / m_iTileSize;
	int iY = _fY / m_iTileSize;
	int iLocation = iY * m_iGridWidth + iX;
	if (iLocation > 0 && iLocation < m_tNodes.size())
	{
		return &m_tNodes[iLocation];
	}
}
USVec2D Pathfinder::PositionFromNode(Node* _node)
{
	float fX = _node->m_iX * m_iTileSize;
	float fY = _node->m_iY * m_iTileSize;
	return USVec2D(fX, fY);
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
USRect Pathfinder::GetTileRect(int _iX, int _iY, int _iTileSize, int _iOffset)
{
	USRect tileRect;
	tileRect.mXMin = _iOffset + _iTileSize * _iX;
	tileRect.mXMax = -_iOffset + _iTileSize * (_iX + 1);
	tileRect.mYMin = _iOffset + _iTileSize * _iY;
	tileRect.mYMax = -_iOffset + _iTileSize * (_iY + 1);
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
	Pathfinder::m_bStartInitialized = true;
	Pathfinder::m_bPathFound = false;
	Pathfinder::m_bPathInitialized = false;
	self->SetStartPosition(pX, pY);
	return 0;
}

int Pathfinder::_setEndPosition(lua_State* L)
{
	MOAI_LUA_SETUP(Pathfinder, "U")

	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	Pathfinder::m_bEndInitialized = true;
	Pathfinder::m_bPathFound = false;
	Pathfinder::m_bPathInitialized = false;
	self->SetEndPosition(pX, pY);
	return 0;
}


int Pathfinder::_pathfindStep(lua_State* L)
{
    MOAI_LUA_SETUP(Pathfinder, "U")

    self->PathfindStep();
    return 0;
}