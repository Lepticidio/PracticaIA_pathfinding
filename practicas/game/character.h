#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <moaicore/MOAIEntity2D.h>
#include "pathfinding/pathfinder.h"
#include "PathFollowingSteering.h"

class Character: public MOAIEntity2D
{
public:
    DECL_LUA_FACTORY(Character)
protected:
	virtual void OnStart();
	virtual void OnStop();
	virtual void OnUpdate(float step);
public:
	virtual void DrawDebug();

	Character();
	~Character();
	
	void SetLinearVelocity(float x, float y) { mLinearVelocity.mX = x; mLinearVelocity.mY = y;}
	void SetAngularVelocity(float angle) { mAngularVelocity = angle;}
	
	USVec2D GetLinearVelocity() const { return mLinearVelocity;}
	float GetAngularVelocity() const { return mAngularVelocity;}
	float GetMaxSpeed() { return m_fMaxSpeed; }
	float GetMaxAcceleration() { return m_fMaxAcceleration; }
	float GetLookAhead() { return m_fLookAhead; }
private:
	USVec2D mLinearVelocity;
	float mAngularVelocity;
	float m_fMaxSpeed = 2;
	float m_fMaxAcceleration = 2;
	float m_fLookAhead = 60;
	SeekSteering* m_pSeek;
	PathFollowingSteering* m_pPathFollowing;
	
	
	// Lua configuration
public:
	virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
	static int _setLinearVel(lua_State* L);
	static int _setAngularVel(lua_State* L);
};

#endif