#include "Terrain.h"

Terrain::Terrain(void)
{
}

Terrain::~Terrain(void)
{
}

float roughness = 3.0f;
signed char* map;
int mapDimension;
int mapSize;

int determineLength( int n )
{
	return (int)pow(pow((float)2, n) + 1, 2 );
}

int min(float a, float b)
{
	return (a < b) ? a : b;
}

int max(float a, float b)
{
	return (a > b) ? a : b;
}

int diamondStep(int n, signed char* heights, int rows, int columns, int h) {
	int inc;
	if (n == 1) {
		inc = 2;
	} else {
		inc = (int)pow(2.0f, n);
	}

	int colInc;
	int rowInc = (rows - 1)/inc;

	for (int r = 0; rowInc > 0 && r < rows - rowInc; r += rowInc) {
		int rOff = 0;
		colInc = (columns - 1)/inc;
		int midR = (r + (r+rowInc))/2;
		for (int c = 0; colInc > 0 && c < columns - colInc; c += colInc) {
			int midC = (c + (c+colInc))/2;
			if (heights[midR * columns + midC] == 0) {
				int avg = 0;

				avg += heights[r * columns + c];
				avg += heights[r * columns + c + colInc];
				avg += heights[(r + rowInc) * columns + c];
				avg += heights[(r + rowInc) * columns + (c+colInc)];
				avg /= 4;

				avg += (int)((float)rand()/(float)RAND_MAX * h - (float)rand()/(float)RAND_MAX * h);

				//if (avg == 0) {
				//	avg = .001;
				//}
				heights[midR * columns + midC] = avg;
			}
		}
	}

	return max(rowInc, colInc);
}

int squareStep(int n, signed char* heights, int rows, int columns, int h) {
	int inc;
	n += 1;
	if (n == 1) {
		inc = 2;
	} else {
		inc = (int)pow(2.0f, n);
	}

	int colInc;
	int rowInc = (rows - 1)/inc;

	int rCount = 0;
	for (int r = 0; r < rows; r += rowInc) {
		colInc = (columns - 1)/inc;
		int startC = 0;
		if (rCount % 2 == 0) {
			startC = colInc;
		}
		for (int c = startC; c < columns; c += colInc) {
			int val = r * columns + c;

			if (heights[(int)r * columns + (int)c] == 0) {
				int roi[4] = {r-rowInc, r, r, r+rowInc};
				int coi[4] = {c, c-colInc, c+colInc, c};


				if (roi[0] < 0) {
					roi[0] += rows - 1;
				} 
				if (roi[3] > rows - 1) {
					roi[3] %= rows - 1;
				}


				if (coi[1] < 0) {
					coi[1] += columns - 1;
				}
				if (coi[2] > columns - 1) {
					coi[2] %= columns - 1;
				}

				int avg = 0;
				for (int i = 0; i < 4; i++) {
					int ind = roi[i] * columns + coi[i];
					avg += heights[ind];
				}

				avg /= 4;
				avg += (int)((float)rand()/(float)RAND_MAX * h - (float)rand()/(float)RAND_MAX * h);
				//if (avg == 0) {
				//	avg = .001;
				//}
				heights[(int)r * columns + (int)c] = avg;
			}
		}
		rCount++;
	}

	return max(rowInc, colInc);
}

void diamondSquare(signed char* heights, int* length, int h) {
	int rows = (int)sqrt((float)*length);
	int n = 0;
	int gap;

	do {
		gap = diamondStep(n, heights, rows, rows, h);
		gap = min(squareStep(n, heights, rows, rows, h), gap);
		h = (int)(h * pow(2, -roughness));
		n++;
	} while (gap > 1);
}

void Terrain::fillTerrainGaps(int mapLen)
{
	vector<float> tPoints(points);
	points.clear();

	//Now that have all the points on the surface, find out the depth required of the 4 surrounding neighbors
	//to fill the holes in with blocks.
	for( GLuint i = 0; i < tPoints.size(); i += 4 )
	{
		points.push_back(tPoints[i+0]);
		points.push_back(tPoints[i+1]);
		points.push_back(tPoints[i+2]);
		points.push_back(tPoints[i+3]);

		normals.push_back(0.0);
		normals.push_back(1.0);
		normals.push_back(0.0);
		normals.push_back(0.0);
		
		int depth = 0;
		int tempDepth = 0;

		//check x-1
		if( i % mapLen != 0 )
		{
			tempDepth = (int)(tPoints[i+1] - tPoints[(i-4)+1]);
			if( tempDepth > depth) depth = tempDepth;
		}

		//check x+1
		if( (i+1) % mapLen != 0 )
		{
			tempDepth = (int)(tPoints[i+1] - tPoints[(i+4)+1]);
			if( tempDepth > depth) depth = tempDepth;
		}
		
		//check z-1
		if( i >= (GLuint)mapLen*4 )
		{
			tempDepth = (int)(tPoints[i+1] - tPoints[(i-(mapLen*4))+1]);
			if( tempDepth > depth) depth = tempDepth;
		}

		//check z+1
		if( i < (GLuint)mapLen * (mapLen-1) * 4 )
		{
			tempDepth = (int)(tPoints[i+1] - tPoints[(i+(mapLen*4))+1]);
			if( tempDepth > depth) depth = tempDepth;
		}

		//Fill in the holes in the depth ideally
		if( depth > 0 )
			for( int j = 1; j <= depth; j++ )
			{
				points.push_back(tPoints[i]);
				points.push_back(tPoints[i+1] - j);
				points.push_back(tPoints[i+2]);
				points.push_back(tPoints[i+3]);

				normals.push_back(0.0);
				normals.push_back(1.0);
				normals.push_back(0.0);
				normals.push_back(0.0);
			}
	}
}

void Terrain::makePointTerrain( float* left, float* top )
{
	mapSize = determineLength( 6 );
	map = new signed char[mapSize];

	for( int i = 0; i < mapSize; i++ )
	{
		map[i] = 0;
	}

	if( top )
		for( int i = 0; i < 65; i++)
			map[i] = (signed char)top[(65*64*4) + (i*4) + 1];

	if( left )
		for( int i = 0; i < mapSize; i += 65 )
			map[i] = (signed char)left[((i+64)*4) + 1];


	diamondSquare(map, &mapSize, 64 );
	mapDimension = (int)sqrt((float)mapSize);

	//Generate the surface
	int cube_width = 1;
	float radius = (cube_width / 2.0f ) * (float)SQRT3;
	for (int r = 0; r < mapDimension; r++)
		for (int c = 0; c < mapDimension; c++)
		{
			float pos[3] = { (float)cube_width*(c-mapDimension/2.0f), 
							 (float)cube_width*map[r * mapDimension + c], 
							 (float)cube_width*(r-mapDimension/2.0f) };

			points.push_back( pos[0] );
			points.push_back( pos[1] );
			points.push_back( pos[2] );
			points.push_back( 1.0f );
		}
}

void Terrain::makePointsFromFile( FILE* inFile )
{
	char* buff = new char[60];

	int y = 185;
	int z = 0;

	while( fgets(buff, 61, inFile) != NULL )
	{
		if( z == 59 )
		{
			y--;
			z = 0;
		}
		else
			z++;
		
		for( int x = 0; x < 61; x++ )
			if( buff[x] == '1' )
			{
				points.push_back( (float)x );
				points.push_back( (float)y );
				points.push_back( (float)z );
				points.push_back( 1.0f );
			}

	}
}