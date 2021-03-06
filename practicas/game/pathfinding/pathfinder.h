#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <moaicore/MOAIEntity2D.h>

struct Node
{
	bool m_bBlocked;
	int m_iX;
	int m_iY;
	int m_iCost;
	float m_fH;
	float m_fG;
	float m_fF;
	void UpdateH(float _fH) { m_fH = _fH; m_fF = m_fH + m_fG; }
	void UpdateG(float _fG) { m_fG = _fG; m_fF = m_fH + m_fG; }
	std::vector<Node*> tNeighbours;
	Node* parent = nullptr;
};
class Pathfinder: public virtual MOAIEntity2D
{
public:
	Pathfinder();
	~Pathfinder();

	virtual void DrawDebug();
	static bool m_bPathInitialized;
	static bool m_bStartInitialized;
	static bool m_bEndInitialized;
	static bool m_bPathFound;
	void SetStartPosition(float x, float y) { m_StartPosition = USVec2D(x, y); UpdatePath();}
	void SetEndPosition(float x, float y) { m_EndPosition = USVec2D(x, y); UpdatePath();}
	const USVec2D& GetStartPosition() const { return m_StartPosition;}
	const USVec2D& GetEndPosition() const { return m_EndPosition;}

    bool PathfindStep();
	std::vector<USVec2D> GetPath() { return m_tPathPoints; }
private:
	void UpdatePath();
	//Map m_map;
private:

	USVec2D m_StartPosition;
	USVec2D m_EndPosition;
	int m_iGridWidth = 16;
	int m_iTileSize = 32;
	int m_iSpaceCounter = 0;
	std::vector<char> m_tGrid;
	void LoadGrid(string _sPath);
	void InitializeNodes();
	char GetTileChar(int _iX, int _iY);
	USRect GetTileRect(int _iX, int _iY, int _iTileSize, int _iOffset);
	std::vector<USVec2D> m_tPathPoints;
	std::vector<Node> m_tNodes;
	Node* NodeFromPosition(float _fX, float _fY);
	Node* NodeFromIndexes(int _iX, int _iY);
	USVec2D PositionFromNode(Node* _node);
	USVec2D m_vEndPosition = GetEndPosition();
	USVec2D m_vStartPosition = GetStartPosition();
	Node* m_targetNode;
	Node* m_currentNode;
	std::vector<Node*> m_tOpenNodes;
	std::vector<Node*> m_tClosedNodes;

	// Lua configuration
public:
	DECL_LUA_FACTORY(Pathfinder)
public:
	virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
	static int _setStartPosition(lua_State* L);
	static int _setEndPosition(lua_State* L);
    static int _pathfindStep(lua_State* L);
};


#endif