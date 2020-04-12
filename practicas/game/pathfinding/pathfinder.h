#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <moaicore/MOAIEntity2D.h>
#include "Map.h"

class Pathfinder: public virtual MOAIEntity2D
{
public:
	Pathfinder();
	~Pathfinder();

	virtual void DrawDebug();

	void SetStartPosition(float x, float y) { m_StartPosition = USVec2D(x, y); UpdatePath();}
	void SetEndPosition(float x, float y) { m_EndPosition = USVec2D(x, y); UpdatePath();}
	const USVec2D& GetStartPosition() const { return m_StartPosition;}
	const USVec2D& GetEndPosition() const { return m_EndPosition;}

    bool PathfindStep();
private:
	void UpdatePath();
	//Map m_map;
private:
	USVec2D m_StartPosition;
	USVec2D m_EndPosition;
	int m_iGridWidth = 16;
	int m_iTileSize = 32;
	std::vector<char> m_tGrid;
	void LoadGrid(string _sPath);
	char GetTile(int _iX, int _iY);
	USRect GetTileRect(int _iX, int _iY);

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