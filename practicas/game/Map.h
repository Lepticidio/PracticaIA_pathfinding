#pragma once
class Map
{
	std::vector<char> m_tGrid;
public:
	Map();
	Map(string _sPath);
	char GetTile(int _iX, int _iY);

};

