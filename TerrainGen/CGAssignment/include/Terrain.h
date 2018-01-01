#pragma once
#include "simpleshape.h"

class Terrain :
	public SimpleShape
{

public:
	Terrain(void);
	~Terrain(void);

	void makePointTerrain( float* top, float* left );
	void makePointsFromFile( FILE* inFile );
	void fillTerrainGaps(int mapLen);

	void max(float a, float b);
	void min(float a, float b);
};

