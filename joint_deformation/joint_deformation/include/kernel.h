/***********************************************************************

	Kernel data interface   

	for fast implementation, all member variables are public
	
	Yin Yang @ UTD

************************************************************************/

#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <cmath>
#include <string>
#include "HSMTransmitter.h"
#include "HSMReceiver.h"

// Eigen numerical library
#include "Eigen/Dense"
#include "Eigen//Sparse"
#include "Eigen/SVD"
#include "Eigen/Geometry"

#include "mesh.h"
#include <time.h>
#include <stdlib.h>

#include "performanceCounter.h"

using namespace std;
using namespace Eigen;

class Kernel
{

public:
	typedef enum {UNDEFINED, SHAPE_MATCHING, VELOCITY_MATCHING, PAIR_MATCHING, SINGLE_GRID,HIERARCHY, HIERARCHY_SHAPE_MATCHING, MULTIPLE_VELOCITY_MATCHING, SIMULATION_NETWORKING, SIMULATION_MOBILE} Simulator;
	typedef enum {NETWORK_ROLE_NONE, NETWORK_ROLE_SERVER, NETWORK_ROLE_CLIENT} NetworkRole;

	typedef Matrix <double, 12, 12> Matrix12d;
	typedef Matrix <double, 12, 1>  Vector12d;
	typedef Matrix <double, 9,  9> Matrix9d;
	typedef Matrix <double, 6,  6> Matrix6d;
	typedef Matrix <double, 9, 1>  Vector9d;
	typedef Matrix <double, 6, 1>  Vector6d;


	typedef vector<Face>::iterator face_iterator;
	typedef vector<Node>::iterator node_iterator;
	typedef vector<Vox>::iterator vox_iterator;

	typedef vector<Face>::const_iterator const_face_iterator;
	typedef vector<Node>::const_iterator const_node_iterator;
	typedef vector<Vox>::const_iterator const_vox_iterator;

		
public:
	Kernel();
	~Kernel();

	// mark the voxels based on input mesh
	int mark_preprocess4Voxel(Mesh* m, unsigned int * p_voxel, int d_current);																				
	int mark_fillInterior4Voxel(unsigned int * p_voxel, int d_current);
	int mark_getUnmarkGridSurface4Voxel(unsigned int * p_voxel, int d_current);
	int mark_markSurfaceVox4Voxel(unsigned int * p_voxel, int d_current);

	int mark_preprocess4VoxelLevel(Mesh* m, unsigned int * p_voxel, int d_current, int d_parent);																				
	int mark_fillInterior4VoxelLevel(unsigned int * p_voxel, int d_total, int d_parent);
	int mark_getUnmarkGridSurface4VoxelLevel(unsigned int * p_voxel, int d_current, int d_parent);
	int mark_markSurfaceVox4VoxelLevel(unsigned int * p_voxel, int d_total, int d_parent);

	void generateVoxMesh(const unsigned int* vol, int dim, VoxMesh* &v_mesh);
	void generatePerVoxCluster(VoxMesh* &vm);																// each voxel corresponding a cluster
	bool simulateNextStep();
	bool simulateNextStep4ShapeMatching();
	bool simulateNextStep4VelocityMatching();
	//newly
	bool simulateNextStepForMobile();
	bool simulateNextStep4MultipleVelocityMatching();
	bool simulateNextStep4HierarchyShapeMatching();
	//
	bool simulateNextStep4SingleGrid();
	bool simulateNextStep4Hierarchy();
	bool simulateNextStep4PairMatching();
	bool simulateNextStepThroughNetworking();
	void initializeSimulator();

	static double det33(Matrix3d& m33);
	static void skew_symmetric(Vector3d& v3, Matrix3d& m33);

	//Level related
	void clearAllLevel();
	void clearLevel(int i);
	void addLevel();
	void generateVoxMesh4Level(int level, int d);
	void generateVoxMeshFromParentMesh(const unsigned int* vol, VoxMesh* &v_mesh, VoxMesh* &parent_mesh, int d, int d_parent, int d_total);
	void connectNewVox4VoxMesh(Vox * new_vox, Vox * parent_Vox, VoxMesh * v_mesh, int parent_index, int d, int d_total, int x, int y, int z);
	void generatePerVoxCluster4Level(VoxMesh* &vm, VoxMesh* &parent_mesh);
	void removeConstraintOfAllLevel();
	void setConstraint4AllLevel(Node *&ni);
	void setAnchor4AllLevel();
	void updateFromLeaf(Cluster * pcluster, Matrix3d & R, Vector3d & T, int & count);
	void updateRTFromLeaf(Cluster * pcluster, vector<Vector3d> & T, vector<Matrix3d> & R, int & count);//hirarchy shape matching
	void linkMesh_VolMesh(Mesh* &m, VoxMesh* &vm, int grid_density_in);
	void linkMesh_VolMeshFromParentMesh(Mesh* &m, VoxMesh* &vm, VoxMesh* &parent_mesh, int d, int d_parent, int level_index_parent);
	void resetMass4Level();

	// file i/o
	void saveOutputData(vector<double>& data, const char* filename);
	void printVector3d(Vector3d & value);
	void exportToOBJ(const char *filename);

	//networking
	void setServerRole(){network_role = NETWORK_ROLE_SERVER;}
	void setClientRole(){network_role = NETWORK_ROLE_CLIENT;}
	void configNetwork();

	//test the point outside cube
	void test_findPointOutside();
	
public:
	Simulator used_simulator;
	PerformanceCounter * time_counter;

	// status 
	bool flag_mesh_ready;
	bool flag_vox_ready;
	bool flag_simulator_ready;
	bool flag_network_ready;
	
	Mesh* p_mesh;
	Mesh* p_mesh_low;
	VoxMesh* p_vox_mesh;
	unsigned int* p_voxel;

	int grid_density;
	int num_sim_vox;
	int num_surface_vox;
	int num_sim_node;
			
	double time_step_size;
	int time_step_index;

	//Level for Velocity Matching
	vector<Level *> level_list;
	int level_display;

	// for whatever analysis
	vector<double> out_data_1;
	vector<double> out_data_2;

	//for constraint fixed to test final result
	bool flag_constraint_fixed;
	
	//if using multigrid
	bool flag_multigrid;

	//if adding dynamics
	bool flag_dynamics;

	//if using gravity(assuming flag_dynamics = true)
	bool flag_gravity;
	double gravity_magnitude;

	//user defined force
	Vector3d const_force;

	//whether allow user to set static force
	bool flag_setForce;
	bool flag_setWindForce;
	double wind_magnitude;

	//force parameters
	double force_scalar;

	ofstream myEnergy;
	ofstream myHEnergy;
	ofstream testoutput;
	ifstream myMobile;

	//if 0, force node, if 1, constraint node
	bool flag_forceNode;



	//networking
	bool flag_network;
	NetworkRole network_role;
	int numOfClients;
	char * ip;
	int port;
	
	vector<HSMTransmitter *> pTransmitter;
	HSMReceiver * pReceiver;

	Data4Mobile * data4Mobile;
	Data4PC * data4PC;
	
	int countOfJacobian;
	double PI;
	int timestep_begin;
	int timestep_converge;
	bool flag_converge;
	int num_PE_perTimeStep;

	bool flag_exportTxt;
	
	double fps;
	bool flag_compressData;

	int timeSleep;
};

#endif