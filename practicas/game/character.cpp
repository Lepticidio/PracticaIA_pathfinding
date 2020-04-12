#include <stdafx.h>
#include "character.h"

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f)
{
	RTTI_BEGIN
		RTTI_EXTEND (MOAIEntity2D)
	RTTI_END
}

Character::~Character()
{

}

void Character::OnStart()
{
	m_pSeek = new SeekSteering(this);
	m_pPathSteering = new PathFollowingSteering(m_pSeek, this);
}

void Character::OnStop()
{

}

void Character::OnUpdate(float step)
{

	USVec2D vAcceleration = m_pPathSteering->GetSteering();
	USVec2D vCurrentVelocity = GetLinearVelocity() + vAcceleration * step;
	SetLinearVelocity(vCurrentVelocity.mX, vCurrentVelocity.mY);
	SetLoc(GetLoc() + GetLinearVelocity() * step);

}

void Character::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.1f);

	//MOAIDraw::DrawPoint(0.0f, 0.0f);
}





// Lua configuration

void Character::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIEntity2D::RegisterLuaFuncs(state);
	
	luaL_Reg regTable [] = {
		{ "setLinearVel",			_setLinearVel},
		{ "setAngularVel",			_setAngularVel},
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

int Character::_setLinearVel(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")
	
	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(2, 0.0f);
	self->SetLinearVelocity(pX, pY);
	return 0;	
}

int Character::_setAngularVel(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")
	
	float angle = state.GetValue<float>(2, 0.0f);
	self->SetAngularVelocity(angle);

	return 0;
}
	