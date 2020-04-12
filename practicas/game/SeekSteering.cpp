#include "stdafx.h"
#include "SeekSteering.h"
#include "character.h"
USVec2D SeekSteering::GetSteering( USVec2D _vTarget)
{
	USVec2D vResult = USVec2D(0, 0);
	USVec2D vCurrentPos = m_pChararacter->GetLoc();

	m_vDesiredSpeed = _vTarget - vCurrentPos;
	m_vDesiredSpeed.SetLength(m_fMaxSpeed);
	vResult = m_vDesiredSpeed - m_pChararacter->GetLinearVelocity();
	if (vResult.Length() > m_fMaxAcceleration)
	{
		vResult.SetLength(m_fMaxAcceleration);
	}
	
	m_vCurrentAcceleration = vResult;
	return vResult;
}

void SeekSteering::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);
	MOAIDraw::DrawLine(m_pChararacter->GetLoc(), m_pChararacter->GetLoc() + m_vDesiredSpeed);

	gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.5f);
	MOAIDraw::DrawLine(m_pChararacter->GetLoc(), m_pChararacter->GetLoc() + m_vCurrentAcceleration);
}
SeekSteering::SeekSteering(Character* _pCharacter) : m_pChararacter(_pCharacter)
{
}
void SeekSteering::Initialize()
{
	m_fMaxSpeed = m_pChararacter->GetMaxSpeed();
	m_fMaxAcceleration = m_pChararacter->GetMaxAcceleration();
	printf("");
}