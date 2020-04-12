#pragma once
#include <stdafx.h>
#include <tinyxml.h>
#include <vector>
class Path
{
public:
	std::vector<USVec2D> m_tPoints;
	Path(const char* filename);
	void DrawDebug();
};

