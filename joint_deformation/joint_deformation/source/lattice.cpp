#include "lattice.h"
#include <queue>


Point3::Point3()
{
}

Point3::Point3(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


Summation::Summation()
{
	connections[0] = &_children;
	connections[1] = &_parents;
}

// Find the extents of a particle bounding box in the given dimension
void Summation::FindParticleRange(int dimension, int *minDim, int *maxDim)
{
	unsigned int m;
	LatticeParticle *p;

	*minDim = INT_MAX;
	*maxDim = INT_MIN;

	for(m = 0; m < particles.size(); m++)
	{
		p = particles[m];

		if(p->lp->_index.val[dimension] < *minDim)
			*minDim = p->lp->_index.val[dimension];
		if(p->lp->_index.val[dimension] > *maxDim)
			*maxDim = p->lp->_index.val[dimension];
	}
}

// myLevel is 0 for XSums, 1 for XYSums
Summation* Summation::FindIdenticalSummation(std::vector<LatticeParticle*> &particles, int myLevel)
{
	unsigned int m, q;

	for(m = 0; m < particles.size(); m++)
	{
		LatticeLocation *checkLp = particles[m]->lp;

		for(q = 0; q < checkLp->sums[myLevel].size(); q++)
		{
			std::vector<LatticeParticle*> *checkParticles = &(checkLp->sums[myLevel][q]->particles);
			if(checkParticles->size() == particles.size() && equal(particles.begin(), particles.end(), checkParticles->begin()))
			{
				return checkLp->sums[myLevel][q];
			}
		}
	}

	return NULL;
}

std::vector<Summation*> Summation::GenerateChildSums(int childLevel)
{
	int i;
	unsigned int m;
	LatticeParticle *p;

	int splitDimension = childLevel;
	FindParticleRange(splitDimension, &minDim, &maxDim);

	std::vector<Summation*> newSums;

	// Generate the array of children, one per value of the sorting dimension
	Summation **childArray = new Summation*[maxDim - minDim + 1];
	for(i = 0; i < maxDim - minDim + 1; i++)
	{
		childArray[i] = new Summation();
	}

	// Sort the particles into their correct children
	for(m = 0; m < particles.size(); m++)
	{
		p = particles[m];

		childArray[p->lp->_index.val[splitDimension] - minDim]->particles.push_back(p);
	}

	// Now process each child
	for(i = minDim; i <= maxDim; i++)
	{
		Summation *child = childArray[i - minDim];

		if(child->particles.empty())
		{
			delete child;
		}
		else
		{
			sort(child->particles.begin(), child->particles.end());
			// Set the lp to the first particle's lp - it doesn't matter which particle, really, as long as it's
			//  the lp of one of the particles so it can be found when others are searching for identical sums
			child->lp = child->particles[0]->lp;

			// Now figure out if this sum is identical to any other
			Summation *identical = FindIdenticalSummation(child->particles, childLevel);

			if(identical != NULL)
			{
				delete child;
				// Use the guy we found instead
				_children.push_back(identical);
				identical->_parents.push_back(this);
			}
			else
			{
				newSums.push_back(child);
				// Finally, register it with me and with the lattice point
				_children.push_back(child);
				child->_parents.push_back(this);
				child->lp->sums[childLevel].push_back(child);
				lp->_body->sums[childLevel].push_back(child);

				// Have the child generate ITS child sums
				if(childLevel > 0)
					child->GenerateChildSums(childLevel - 1);
				else
				{
					// The child is a bar, so just connect it to its particles
					for(unsigned int m = 0; m < child->particles.size(); m++)
					{
						p = child->particles[m];
						child->_children.push_back(p);
						p->_parents.push_back(child);
					}
				}
			}
		}
	}

	delete[] childArray;

	return newSums;
}

void Summation::SumFromChildren()
{
	PerformSummation(0);
}

void Summation::SumFromParents()
{
	PerformSummation(1);
}

// Slightly optimized using MMX operations (sum 4 floats in one op)	// Actually, no, it's not. Problems with aligning the memory in this version.
void Summation::PerformSummation(int direction)
{
	std::vector<Summation*> *sources = connections[direction];
	if(sources->size() == 0)
	{
		return;
	}
	else if(sources->size() == 1)
	{
		_sumData._v = (*sources)[0]->_sumData._v;
		_sumData._M = (*sources)[0]->_sumData._M;
		_sumData.m1 = (*sources)[0]->_sumData.m1;
		//sumData.m2 = (*sources)[0]->sumData.m2;
		//sumData.m3 = (*sources)[0]->sumData.m3;
	}
	else
	{
		_sumData._v = (*sources)[0]->_sumData._v + (*sources)[1]->_sumData._v;
		_sumData._M = (*sources)[0]->_sumData._M + (*sources)[1]->_sumData._M;
		_sumData.m1 = (*sources)[0]->_sumData.m1 + (*sources)[1]->_sumData.m1;
		//sumData.m2 = _mm_add_ps((*sources)[0]->sumData.m2, (*sources)[1]->sumData.m2);
		//sumData.m3 = _mm_add_ps((*sources)[0]->sumData.m3, (*sources)[1]->sumData.m3);
		for(unsigned int j = 2; j < sources->size(); j++)
		{
			_sumData._v += (*sources)[j]->_sumData._v;
			_sumData._M += (*sources)[j]->_sumData._M;
			_sumData.m1 += (*sources)[j]->_sumData.m1;
			//sumData.m2 = _mm_add_ps(sumData.m2, (*sources)[j]->sumData.m2);
			//sumData.m3 = _mm_add_ps(sumData.m3, (*sources)[j]->sumData.m3);
		}
	}
}

void LatticeLocation::CalculateNeighborhood()
{
	// Generate the set of lattice locations up to w steps from each lattice location

	neighborhood.clear();

	if(_body->w == 1)
	{
		// We can just use the neighbor list
		neighborhood = immediateNeighbors;
		sort(neighborhood.begin(), neighborhood.end());
	}
	else
	{
		// Find the set of particles up to w steps away

		unsigned int newTouch = rand() % INT_MAX;
		std::queue<LatticeLocation*> next;
		int currentDepth = 0;
		int remainingAtThisDepth = 1;
		int elementsAtNextDepth = 0;

		next.push(this);

		// Rather than doing an expensive search to see if we've already added a given lattice location, we just mark it when we add it
		touch = newTouch;

		while(next.empty() == false)
		{
			// Get "u" from the queue, add it to neighbors
			LatticeLocation *u = next.front(); next.pop();
			neighborhood.push_back(u);

			if(currentDepth < _body->w)
			{
				std::vector<LatticeLocation*>::const_iterator neighborI;
				// Add all u's neighbors to next
				for(neighborI = u->immediateNeighbors.begin(); neighborI != u->immediateNeighbors.end(); neighborI++)
				{
					LatticeLocation *neighbor = *neighborI;
					if(neighbor->touch != newTouch)
					{
						neighbor->touch = newTouch;
						next.push(neighbor);
						elementsAtNextDepth++;
					}
				}
			}

			remainingAtThisDepth--;
			if(remainingAtThisDepth == 0)
			{
				currentDepth++;
				remainingAtThisDepth = elementsAtNextDepth;
				elementsAtNextDepth = 0;
			}
		}

		sort(neighborhood.begin(), neighborhood.end());
	}
	
	// Set whether the region should be generated or not
	if(_body->fracturing)
	{
		// We always generate the region if there is fracturing, because one region that starts out as identical to another may
		//  become different as a result of fracturing, and it's faster to just generate them all at the start rather than doing
		//  expensive tests for identicalness every time there is a fracture
		regionExists = true;
	}
	else
	{
		regionExists = true;
		// Check if we are a duplicate
		for each(LatticeLocation *check in _body->latticeLocationsWithExistentRegions)
		{
			if(check->neighborhood.size() == neighborhood.size() && equal(neighborhood.begin(), neighborhood.end(), check->neighborhood.begin()))
			{
				// We ARE a duplicate
				regionExists = false;
				break;
			}
		}
	}

	if(regionExists)
	{
		_body->latticeLocationsWithExistentRegions.push_back(this);
	}
	
}

//body
Body::Body()
{
	alpha = 1.0f;
	defaultParticleMass = 1.0f;
	w = 1;
	kRegionDamping = 0.5f;
	invariantsDirty = true;
	fracturing = false;
}

LatticeLocation* Body::GetLatticeLocation(Point3 index)
{
	std::map<Point3, LatticeLocation*>::iterator found = lattice.find(index);
	if(found == lattice.end()) return NULL;
	else return found->second;
}

void Body::AddParticle(Point3 index, Node * pNode)
{
	// Initialize lattice location
	LatticeLocation *l = new LatticeLocation();
	latticeLocations.push_back(l);
	lattice[index] = l;
	l->_index = index;
	l->_node = pNode;
	l->_body = this;
	l->_region = NULL;

	// Set up the immediate neighborhood
	for(int xo = -1; xo <= 1; xo++)
	{
		for(int yo = -1; yo <= 1; yo++)
		{
			for(int zo = -1; zo <= 1; zo++)
			{
				Point3 check = Point3(index.x + xo, index.y + yo, index.z + zo);
				if(!(xo == 0 && yo == 0 && zo == 0) && GetLatticeLocation(check) != NULL)
				{
					l->immediateNeighbors.push_back(GetLatticeLocation(check));
					l->immediateNeighborsGrid[xo + 1][yo + 1][zo + 1] = GetLatticeLocation(check);
					GetLatticeLocation(check)->immediateNeighbors.push_back(l);
					GetLatticeLocation(check)->immediateNeighborsGrid[-xo + 1][-yo + 1][-zo + 1] = l;
				}
				else
				{
					l->immediateNeighborsGrid[xo + 1][yo + 1][zo + 1] = NULL;
				}
			}
		}
	}

	// Initialize particle
	l->_particle = new LatticeParticle();
	_particles.push_back(l->_particle);
	l->_particle->lp = l;
	l->_particle->_x0 = l->_node->coordinate;
	l->_particle->_mass = defaultParticleMass;
	l->_particle->_x = l->_particle->_x0;
	l->_particle->_v = Vector3d::Zero();
	l->_particle->_f = Vector3d::Zero();
}

void Body::Finalize()
{
	// Set the lattice points' immediateNeighbors, and decide based on this whether or not it is an edge
	for each(LatticeLocation *l in latticeLocations)
	{
		// Set whether it is an edge - i.e., doesn't have a full set of immediateNeighbors
		l->edge = (l->immediateNeighbors.size() != 26);

		// Build the neighborhood by breadth-first search
		l->CalculateNeighborhood();
	}

	// Generate the regions
	GenerateSMRegions();

	// Set the parent regions
	for each(LatticeRegion *r in _regions)
	{
		for each(LatticeParticle *p in r->particles)
		{
			p->parentRegions.push_back(r->lp);
		}
	}

	CalculateInvariants();

}

void Body::GenerateSMRegions()
{
	// Generate the regions from the lattice locations' neighborhoods
	printf("Generating regions and intermediate summations...");
	for each(LatticeLocation *l in latticeLocationsWithExistentRegions)
	{
		l->_region = new LatticeRegion();
		_regions.push_back(l->_region);
		l->_region->lp = l;

		for each(LatticeLocation *l2 in l->neighborhood)
		{
			l->_region->particles.push_back(l2->_particle);
		}
		sort(l->_region->particles.begin(), l->_region->particles.end());

		// Initialize region
		l->_region->_eigenVectors = Matrix3d::Identity();
	}

	// Generate the intermediate sub-summations (plates and bars)
	for each(LatticeRegion *region in _regions)
	{
		region->GenerateChildSums(1);
	}

	printf(" done.\n");
}

void Body::CalculateInvariants()
{
	printf("Calculating invariants...");

	// Calculate perRegionMass
	for each(LatticeParticle *particle in _particles)
	{
		particle->_perRegionMass = particle->_mass / particle->parentRegions.size();
	}

	// Calculate region properties
	// Use fast summation
	for each(LatticeParticle *p in _particles)
	{
		p->_sumData._M(0,0) = p->_perRegionMass;
		p->_sumData._v = p->_perRegionMass * p->_x0;
	}
	SumParticlesToRegions();
	for each(LatticeRegion *r in _regions)
	{
		r->_M = r->_sumData._M(0, 0);
		r->_Ex0 = r->_sumData._v;
		r->_c0 = r->_Ex0 / r->_M;
	}

	printf(" done.\n");
}

void Body::SumParticlesToRegions()
{
	for each(Summation *bar in sums[0])
	{
		bar->SumFromChildren();
	}
	for each(Summation *plate in sums[1])
	{
		plate->SumFromChildren();
	}
	for each(LatticeRegion *region in _regions)
	{
		region->SumFromChildren();
	}
}

void Body::SumRegionsToParticles()
{
	for each(Summation *plate in sums[1])
	{
		plate->SumFromParents();
	}
	for each(Summation *bar in sums[0])
	{
		bar->SumFromParents();
	}
	for each(LatticeParticle *particle in _particles)
	{
		particle->SumFromParents();
	}
}