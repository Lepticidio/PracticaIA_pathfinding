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
	m_tPathPoints.clear();
	USVec2D vEndPosition = GetEndPosition();
	USVec2D vStartPosition = GetStartPosition();
	Node* targetNode = NodeFromPosition(vEndPosition.mX, vEndPosition.mY);
	Node* currentNode = NodeFromPosition(vStartPosition.mX, vStartPosition.mY);

	for (int i = 0; i < m_tNodes.size(); i++)
	{
		Node* pNode = &m_tNodes[i];
		USVec2D vPositionNode = PositionFromNode(pNode);
		pNode->UpdateH((vEndPosition - vPositionNode).Length());
	}
	std::vector<Node*> tOpenNodes;
	std::vector<Node*> tClosedNodes;
	tOpenNodes.push_back(currentNode);
	currentNode->parent = nullptr;
	currentNode->UpdateG(0);
	while (tOpenNodes.size() > 0)
	{
		float fSmallestF= std::numeric_limits<float>::max();
		int iIndexCurrent = 0;
		for (int i = 0; i < tOpenNodes.size(); i++)
		{
			Node* pNode = tOpenNodes[i];
			if (pNode->m_fF < fSmallestF)
			{
				fSmallestF = pNode->m_fF;
				currentNode = pNode;
				iIndexCurrent = i;
			}
		}
		tOpenNodes.erase(tOpenNodes.begin()+iIndexCurrent);
		tClosedNodes.push_back(currentNode);
		USVec2D vCurrentPosition = PositionFromNode(currentNode);
		if (currentNode == targetNode)
		{
			break;
		}
		int iCounter = 0;
		while (iCounter < currentNode->tNeighbours.size())
		{
			Node* pNode = currentNode->tNeighbours[iCounter];
			if (!pNode->m_bBlocked && std::count(tClosedNodes.begin(), tClosedNodes.end(), pNode) == 0)
			{
				USVec2D vNodePosition = PositionFromNode(pNode);
				float fGValue = (vNodePosition - vCurrentPosition).Length() * (float)pNode->m_iCost + currentNode->m_fG;
				if (std::count(tOpenNodes.begin(), tOpenNodes.end(), pNode) == 0 || fGValue < pNode->m_fG)
				{
					pNode->UpdateG(fGValue);
					pNode->parent = currentNode;
					if (std::count(tOpenNodes.begin(), tOpenNodes.end(), pNode) == 0)
					{
						tOpenNodes.push_back(pNode);
					}
				}
			}
			iCounter++;
		}
	}
	while (currentNode->parent != nullptr)
	{
		USVec2D vNodePosition = PositionFromNode(currentNode);
		m_tPathPoints.insert(m_tPathPoints.begin(), vNodePosition);
		currentNode = currentNode->parent;
	}

	USVec2D vNodePosition = PositionFromNode(currentNode);
	m_tPathPoints.insert(m_tPathPoints.begin(), vNodePosition);
	currentNode = currentNode->parent;
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
			USVec2D vOffset(m_iTileSize / 2, m_iTileSize / 2);
			USVec2D vPreviousPoint = m_tPathPoints[0] + vOffset;
			for (int i = 1; i < m_tPathPoints.size(); i++)
			{
				MOAIDraw::DrawLine(vPreviousPoint, m_tPathPoints[i] + vOffset);
				vPreviousPoint = m_tPathPoints[i] + vOffset;
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