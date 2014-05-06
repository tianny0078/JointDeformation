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
#ifndef LATTICE_H
#define LATTICE_H

#include "mesh.h"

Class Body{
	string name;
	float alpha;

	//initial parameters
	int w;
	float defaultParticleMass;
	std::map<Point3, LatticeLocation*> lattice;
	std::vector<LatticeLocation*> latticeLocations;


};

Class Summation{

};

Class Region: public Summation
{

};

Class Particle:public Summation{

};

Class LatticeLocation{

};

#endif