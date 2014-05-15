/*
Implementation of data structure for Fast Lattice Shape Matching

Fundamentally, a cell is a rendering concept. It means a volume of space mapped from
object space to world space. These are the cubes in the lattice.
One kind of cell has particles at the vertices; this is one way of mapping the space.
Another kind has a particle at the center of each cell, and projects out from that
particle based on its rotation. I will use the first rendering concept.

Class body represents one deformable object including the whole bunch of particles;
Class LatticeLocation represents the location of lattice which includes the pointer to center article.



by Yuan Tian
*/
#pragma once
#include <map>
#include "mesh.h"
using namespace std;

class Point3;
class Summation;
class LatticeRegion;
class LatticeLocation;
class LatticeParticle;
class Body;

// The actual elements of data that must be summmed. These elements will be
//  used to store different things - v will be mixi
//  during the shape matching, and (cr - Rrcr) during goal position calculation;
//  M will be EpmiT during shape matching but Rr
//  during goal position calculation.
struct SumData{
	Vector3d _v;
	Matrix3d _M;
	float m1;
};

class Point3
{
public:
	union
	{
		struct
		{
			int x, y, z;
		};
		int val[3];
	};

	Point3();
	Point3(int x, int y, int z);
	inline bool operator == ( const Point3& r ) const
	{
		return ( x == r.x && y == r.y && z == r.z );
	}

	inline Point3& operator = ( const Point3& rkVector )
	{
		x = rkVector.x;
		y = rkVector.y;
		z = rkVector.z;

		return *this;
	}

	inline bool operator != ( const Point3& rkVector ) const
	{
		return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
	}

	// arithmetic operations
	inline Point3 operator * ( const int rkVector ) const
	{
		Point3 kSum;

		kSum.x = x * rkVector;
		kSum.y = y * rkVector;
		kSum.z = z * rkVector;

		return kSum;
	}

	// arithmetic operations
	inline Point3 operator + ( const Point3& rkVector ) const
	{
		Point3 kSum;

		kSum.x = x + rkVector.x;
		kSum.y = y + rkVector.y;
		kSum.z = z + rkVector.z;

		return kSum;
	}

	inline Point3 operator - ( const Point3& rkVector ) const
	{
		Point3 kDiff;

		kDiff.x = x - rkVector.x;
		kDiff.y = y - rkVector.y;
		kDiff.z = z - rkVector.z;

		return kDiff;
	}

	// arithmetic updates
	inline Point3& operator += ( const Point3& rkVector )
	{
		x += rkVector.x;
		y += rkVector.y;
		z += rkVector.z;

		return *this;
	}

	inline Point3& operator -= ( const Point3& rkVector )
	{
		x -= rkVector.x;
		y -= rkVector.y;
		z -= rkVector.z;

		return *this;
	}

	inline bool operator< (const Point3 &b) const
	{
		if (x < b.x)
			return true;

		if (b.x < x)
			return false;

		// now, we know that a.x == b.x
		// compare on y

		if (y < b.y)
			return true;

		if (b.y < y)
			return false;

		// now, we know that (a.x == b.x) && (a.y == b.y)
		// compare on y

		if (z < b.z)
			return true;

		return false;
	}
};

class Summation{//Particle, XSum, XYSum, Region
public:
	LatticeLocation * lp;//for optimization
	std::vector<LatticeParticle*> particles;
	vector<Summation *>_children, _parents;
	vector<Summation *> * connections[2];

	SumData _sumData;

	Summation();
	void FindParticleRange(int dimension, int *minDim, int *maxDim);
	Summation* FindIdenticalSummation(std::vector<LatticeParticle*> &particles, int myLevel);
	std::vector<Summation*> GenerateChildSums(int childLevel);		// Returns the child summations that were generated, 0 for Xsum, 1 for XYsum
	int minDim, maxDim;

	void SumFromChildren();
	void SumFromParents();
	void PerformSummation(int direction); //generalization for summation, 0 for bottom-up, 1 for top-down;
};

class LatticeRegion: public Summation
{
public:
	//precomputed properties
	Vector3d _Ex0;
	Vector3d _c0;
	float _M;
	
	//dynamic properties
	Vector3d _c;
	Matrix3d A;
	Matrix3d _eigenVectors; // Cached to warm-start Jacobi iterations for polar decomposition - see Extensions section of FastLSM paper
	Vector3d _t;
	Matrix3d _R;
};

class LatticeParticle:public Summation{
public:
	//properties
	float _mass;
	Vector3d _x0;
	float _perRegionMass;

	//Dynamic values;
	Vector3d _x, _v, _f;
	Vector3d _g;
	Matrix3d R;

	//Relations
	vector<LatticeLocation *> parentRegions;
};

class LatticeLocation{
public:
	Body * _body;
	Point3 _index;
	Node * _node;

	//properties
	bool regionExists;
	bool edge;

	// The IMMEDIATE immediateNeighbors
	std::vector<LatticeLocation*> immediateNeighbors;
	LatticeLocation *immediateNeighborsGrid[3][3][3];	// Same as the pointers in the array, just indexed differently

	// Generated
	std::vector<LatticeLocation*> neighborhood;			// All particles up to w links away

	// The elements centered/living here
	LatticeParticle *_particle;
	LatticeRegion *_region;							// MAY BE NULL
	std::vector<Summation*> sums[2];		// Sums that live here (not necc. centered). sums[0] = xSums, sums[1] = xySums

	// Used in some algorithms
	unsigned int touch;

	void CalculateNeighborhood(); // Will use a BFS to fill in neighborhood. Also sets regionExists
};

class Body{
public:
	string name;
	float alpha;

	//initial parameters
	int w;
	float defaultParticleMass;
	std::map<Point3, LatticeLocation*> lattice;
	vector<LatticeLocation*> latticeLocations;
	std::vector<LatticeLocation*> latticeLocationsWithExistentRegions;
	
	// Fracture
	bool fracturing;

	//Damping
	float kRegionDamping;
	
	//Elements
	std::vector<LatticeParticle*> _particles;
	std::vector<LatticeRegion*> _regions;
	// Intermediate summations
	std::vector<Summation*> sums[2];	// sums[0] = bars; sums[1] = plates

	bool invariantsDirty;		// Whether the invariants need to be recalculated -- simply set this to true after changing an invariant (e.g. particle mass) and the appropriate values will be recomputed automatically next time step

	// Generation
	Body();
	void AddParticle(Point3 index, Node * pNode);
	void Finalize();			// After you add all the particles where you want them to be, you must finalize the object

	// Simulation
	void ShapeMatch();
	void CalculateParticleVelocities(float h);
	void PerformRegionDamping();
	void ApplyParticleVelocities(float h);
	void ApplyGravity();
	void SetNodePosition();
	//void DoFracturing();

	// Fast summation
	void SumParticlesToRegions();
	void SumRegionsToParticles();

	// Helper functions;
	void GenerateSMRegions();
	void CalculateInvariants();
	//void RebuildRegions(std::vector<LatticeLocation*> &regen);		// Used in fracturing
	LatticeLocation *GetLatticeLocation(Point3 index);
	Matrix3d MrMatrix(Vector3d v);//return v*v'

	//clear
	void clearParticles();
	void clearRegions();
	void resetW();
};

