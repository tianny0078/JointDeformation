/************************************************************************/
/* 
	The implementation of Kernel class
*/
/************************************************************************/

#include "kernel.h"
 
Kernel::Kernel()
{
	used_simulator = UNDEFINED;
	constraintType = FORCE_CONSTRAINT;
	p_mesh = new Mesh;
	p_mesh_low = new Mesh;
	p_vox_mesh = new VoxMesh;
	grid_density = 1;
	num_sim_vox = 0;
	num_surface_vox = 0;
	
	p_voxel = new unsigned int[grid_density*grid_density*grid_density];
	memset(p_voxel, 0, sizeof(unsigned int)*grid_density * grid_density * grid_density);

	flag_mesh_ready = false;
	flag_vox_ready = false;
	flag_simulator_ready = false;

	time_step_size = 1.0/30.0;
	time_step_index = 0;

	Level * p_level = new Level;
	level_list.push_back(p_level);
	p_level->gridDensity = grid_density;
	p_level->level_index = 0;
	p_level->voxmesh_level = p_vox_mesh;
	level_display = 0;

	out_data_1.reserve(10000);
	out_data_2.reserve(10000);

	flag_constraint_fixed = false;

	flag_multigrid = true;

	const_force = Vector3d::Zero();

	flag_setForce = false;
	flag_setWindForce = false;
	wind_magnitude = 10.0;

	force_scalar = 10.0;
	
	flag_dynamics = true;

	flag_gravity = false;
	gravity_magnitude = -0.01;



	flag_network = false;


	network_role = NETWORK_ROLE_NONE;
	numOfClients = 0;
	port = 20000;
	ip = NULL;

	data4Mobile = new Data4Mobile(p_mesh);
	data4PC = new Data4PC(p_mesh);

	countOfJacobian = 0;

	
	myEnergy.open("Energy.txt", ios::app);
	myHEnergy.open("HierarchyEnergy.txt", ios::app);
	testoutput.open("output.txt", ios::out);

	pReceiver = NULL;

	PI = 3.14159265359;

	flag_converge = false;
	timestep_begin = -1;
	timestep_converge = -1; 

	num_PE_perTimeStep = 0;

	flag_exportTxt = false;
	flag_exportObj = false;

	fps = 0;

	flag_compressData = true;

	timeSleep = 10;

	time_counter = new PerformanceCounter;

	energyThreshold = 0.005;

	num_obj = 1;

	surface_point_left = Vector3d::Zero();
	surface_point_right = Vector3d::Zero();
	para[0] = para[1] = para[2] = 0.0;
	para2[0] = para2[1] = para2[2] = 0.0;
	paraNode[0] = paraNode[1] = paraNode[2] = NULL;
	paraNode2[0] = paraNode2[1] = paraNode2[2] = NULL;

	idx_constraint = 1;

	sphere = new Mesh;
	sphere->read("sphere2.obj");

	flag_exportObj4Arrow = false;
	_posBegin.setZero(); 
	_posEnd.setZero();
	Vector3d temp;
	//box vertex
	temp(0) = -0.1;
	temp(1) = 0.0;
	temp(2) = -0.1;
	v_box_list.push_back(temp);
	temp(0) = 0.1;
	temp(1) = 0.0;
	temp(2) = -0.1;
	v_box_list.push_back(temp);
	temp(0) = 0.1;
	temp(1) = 0.0;
	temp(2) = 0.1;
	v_box_list.push_back(temp);
	temp(0) = -0.1;
	temp(1) = 0.0;
	temp(2) = 0.1;
	v_box_list.push_back(temp);
	temp(0) = -0.1;
	temp(1) = 10.0;
	temp(2) = -0.1;
	v_box_list.push_back(temp);
	temp(0) = 0.1;
	temp(1) = 10.0;
	temp(2) = -0.1;
	v_box_list.push_back(temp);
	temp(0) = 0.1;
	temp(1) = 10.0;
	temp(2) = 0.1;
	v_box_list.push_back(temp);
	temp(0) = -0.1;
	temp(1) = 10.0;
	temp(2) = 0.1;
	v_box_list.push_back(temp);

	//box face
	Vector3i temp2;
	temp2(0) = 0;
	temp2(1) = 1;
	temp2(2) = 2;
	f_box_list.push_back(temp2);
	temp2(0) = 0;
	temp2(1) = 2;
	temp2(2) = 3;
	f_box_list.push_back(temp2);
	temp2(0) = 4;
	temp2(1) = 6;
	temp2(2) = 5;
	f_box_list.push_back(temp2);
	temp2(0) = 4;
	temp2(1) = 7;
	temp2(2) = 6;
	f_box_list.push_back(temp2);
	temp2(0) = 5;
	temp2(1) = 6;
	temp2(2) = 1;
	f_box_list.push_back(temp2);
	temp2(0) = 6;
	temp2(1) = 2;
	temp2(2) = 1;
	f_box_list.push_back(temp2);
	temp2(0) = 6;
	temp2(1) = 3;
	temp2(2) = 2;
	f_box_list.push_back(temp2);
	temp2(0) = 6;
	temp2(1) = 7;
	temp2(2) = 3;
	f_box_list.push_back(temp2);
	temp2(0) = 4;
	temp2(1) = 3;
	temp2(2) = 7;
	f_box_list.push_back(temp2);
	temp2(0) = 4;
	temp2(1) = 0;
	temp2(2) = 3;
	f_box_list.push_back(temp2);
	temp2(0) = 4;
	temp2(1) = 5;
	temp2(2) = 0;
	f_box_list.push_back(temp2);
	temp2(0) = 5;
	temp2(1) = 1;
	temp2(2) = 0;
	f_box_list.push_back(temp2);

	//cone vertex
	temp(0) = -0.2;
	temp(1) = 0.0;
	temp(2) = -0.2;
	v_cone_list.push_back(temp);
	temp(0) = 0.2;
	temp(1) = 0.0;
	temp(2) = -0.2;
	v_cone_list.push_back(temp);
	temp(0) = 0.2;
	temp(1) = 0.0;
	temp(2) = 0.2;
	v_cone_list.push_back(temp);
	temp(0) = -0.2;
	temp(1) = 0.0;
	temp(2) = 0.2;
	v_cone_list.push_back(temp);
	temp(0) = 0.0;
	temp(1) = 0.5;
	temp(2) = 0.0;
	v_cone_list.push_back(temp);
	//cone face
	temp2(0) = 0;
	temp2(1) = 1;
	temp2(2) = 2;
	f_cone_list.push_back(temp2);
	temp2(0) = 0;
	temp2(1) = 2;
	temp2(2) = 3;
	f_cone_list.push_back(temp2);
	temp2(0) = 1;
	temp2(1) = 4;
	temp2(2) = 2;
	f_cone_list.push_back(temp2);
	temp2(0) = 2;
	temp2(1) = 4;
	temp2(2) = 3;
	f_cone_list.push_back(temp2);
	temp2(0) = 3;
	temp2(1) = 4;
	temp2(2) = 0;
	f_cone_list.push_back(temp2);
	temp2(0) = 0;
	temp2(1) = 4;
	temp2(2) = 1;
	f_cone_list.push_back(temp2);

	flag_exportForce = false;
	flag_exportPosConstraint = false;
	no_record = 0;
	current_force = Vector3d::Zero();
	constraint_first = Vector3d::Zero();
	constraint_second = Vector3d::Zero();
	flag_redo = false;
	flag_beating = false;
}

Kernel::~Kernel()
{
	myEnergy.close();
	myHEnergy.close();
	testoutput.close();

	if(myMobile.is_open())
		myMobile.close();

	if(!p_voxel)
		delete []p_voxel;
}
int Kernel::mark_preprocess4VoxelLevel(Mesh* m, unsigned int * p_voxel, int d_current, int d_parent)
{
	if (!m->flag_normalized)
	{
		return 0;
	}

	double voxel_dim = 1.0/double(d_current * d_parent);

	// voxelize each face
	for (face_iterator fi=m->face_list.begin(); fi<m->face_list.end(); ++fi)
	{
		double maxx = -1e10;
		double maxy = -1e10;
		double maxz = -1e10;
		double minx = 1e10;
		double miny = 1e10;
		double minz = 1e10;

		Vector3d n0, n1, n2, normal;
		
		n0 = fi->node0->coordinate;
		n1 = fi->node1->coordinate;
		n2 = fi->node2->coordinate;

		
		maxx = maxx > n0[0] ? maxx : n0[0];
		maxx = maxx > n1[0] ? maxx : n1[0];
		maxx = maxx > n2[0] ? maxx : n2[0];

		maxy = maxy > n0[1] ? maxy : n0[1];
		maxy = maxy > n1[1] ? maxy : n1[1];
		maxy = maxy > n2[1] ? maxy : n2[1];

		maxz = maxz > n0[2] ? maxz : n0[2];
		maxz = maxz > n1[2] ? maxz : n1[2];
		maxz = maxz > n2[2] ? maxz : n2[2];

		minx = minx < n0[0] ? minx : n0[0];
		minx = minx < n1[0] ? minx : n1[0];
		minx = minx < n2[0] ? minx : n2[0];

		miny = miny < n0[1] ? miny : n0[1];
		miny = miny < n1[1] ? miny : n1[1];
		miny = miny < n2[1] ? miny : n2[1];

		minz = minz < n0[2] ? minz : n0[2];
		minz = minz < n1[2] ? minz : n1[2];
		minz = minz < n2[2] ? minz : n2[2];

		int idx_x_begin = floor((minx + 0.5)/voxel_dim);
		int idx_y_begin = floor((miny + 0.5)/voxel_dim);
		int idx_z_begin = floor((minz + 0.5)/voxel_dim);

		int idx_x_end = floor((maxx + 0.5)/voxel_dim);
		int idx_y_end = floor((maxy + 0.5)/voxel_dim);
		int idx_z_end = floor((maxz + 0.5)/voxel_dim);

		if (idx_x_begin == d_current * d_parent)
			idx_x_begin --;

		if (idx_y_begin == d_current * d_parent)
			idx_y_begin --;

		if (idx_z_begin == d_current * d_parent)
			idx_z_begin --;

		if (idx_x_end == d_current * d_parent)
			idx_x_end --;

		if (idx_y_end == d_current * d_parent)
			idx_y_end --;

		if (idx_z_end == d_current * d_parent)
			idx_z_end --;
		
		int idx_pz;
		int idx_py;
		int idx_px;
		int dim0;
		int dim1;
		int dim2;
		int dim3;

		for (int idx_z = idx_z_begin; idx_z <= idx_z_end; ++idx_z)
		{
			idx_pz = idx_z / d_current;
			dim1 = (idx_z % d_current) *d_current*d_current;
			for (int idx_y = idx_y_begin; idx_y <= idx_y_end; ++idx_y)
			{
				idx_py = idx_y / d_current;
				dim2 = (idx_y%d_current)*d_current;
				for (int idx_x = idx_x_begin; idx_x <= idx_x_end; ++idx_x)
				{
					idx_px = idx_x / d_current;
					dim0 = idx_pz * d_parent * d_parent + idx_py * d_parent + idx_px;
					dim0 = dim0 * d_current * d_current * d_current; 
					dim3 = idx_x % d_current;
					if (!p_voxel[dim0 + dim1 + dim2 + dim3])
					{
						p_voxel[dim0 + dim1 + dim2 + dim3] = 1;
						++num_sim_vox;
						
						//cout << "z" << idx_z << "y" << idx_y << "x" << idx_x << endl;
					}
				}
			}
		}
	}
	return num_sim_vox;
	
}
int Kernel::mark_preprocess4Voxel(Mesh* m, unsigned int * p_voxel, int d_current)
{	
	if (!m->flag_normalized)
	{
		return 0;
	}

	double voxel_dim = 1.0/double(d_current);

	// voxelize each face
	for (face_iterator fi=m->face_list.begin(); fi<m->face_list.end(); ++fi)
	{
		double maxx = -1e10;
		double maxy = -1e10;
		double maxz = -1e10;
		double minx = 1e10;
		double miny = 1e10;
		double minz = 1e10;

		Vector3d n0, n1, n2, normal;
		
		n0 = fi->node0->coordinate;
		n1 = fi->node1->coordinate;
		n2 = fi->node2->coordinate;

		
		maxx = maxx > n0[0] ? maxx : n0[0];
		maxx = maxx > n1[0] ? maxx : n1[0];
		maxx = maxx > n2[0] ? maxx : n2[0];

		maxy = maxy > n0[1] ? maxy : n0[1];
		maxy = maxy > n1[1] ? maxy : n1[1];
		maxy = maxy > n2[1] ? maxy : n2[1];

		maxz = maxz > n0[2] ? maxz : n0[2];
		maxz = maxz > n1[2] ? maxz : n1[2];
		maxz = maxz > n2[2] ? maxz : n2[2];

		minx = minx < n0[0] ? minx : n0[0];
		minx = minx < n1[0] ? minx : n1[0];
		minx = minx < n2[0] ? minx : n2[0];

		miny = miny < n0[1] ? miny : n0[1];
		miny = miny < n1[1] ? miny : n1[1];
		miny = miny < n2[1] ? miny : n2[1];

		minz = minz < n0[2] ? minz : n0[2];
		minz = minz < n1[2] ? minz : n1[2];
		minz = minz < n2[2] ? minz : n2[2];

		int idx_x_begin = floor((minx + 0.5)/voxel_dim);
		int idx_y_begin = floor((miny + 0.5)/voxel_dim);
		int idx_z_begin = floor((minz + 0.5)/voxel_dim);

		int idx_x_end = floor((maxx + 0.5)/voxel_dim);
		int idx_y_end = floor((maxy + 0.5)/voxel_dim);
		int idx_z_end = floor((maxz + 0.5)/voxel_dim);

		if (idx_x_begin == d_current)
			idx_x_begin --;

		if (idx_y_begin == d_current)
			idx_y_begin --;

		if (idx_z_begin == d_current)
			idx_z_begin --;

		if (idx_x_end == d_current)
			idx_x_end --;

		if (idx_y_end == d_current)
			idx_y_end --;

		if (idx_z_end == d_current)
			idx_z_end --;
		
		for (int idx_z = idx_z_begin; idx_z <= idx_z_end; ++idx_z)
		{
			int dim1 = idx_z*d_current*d_current;
			for (int idx_y = idx_y_begin; idx_y <= idx_y_end; ++idx_y)
			{
				int dim2 = idx_y*d_current;
				for (int idx_x = idx_x_begin; idx_x <= idx_x_end; ++idx_x)
				{
					if (!p_voxel[dim1 + dim2 + idx_x])
					{
						p_voxel[dim1 + dim2 + idx_x] = 1;
						++num_sim_vox;
						
						//cout << "z" << idx_z << "y" << idx_y << "x" << idx_x << endl;
					}
				}
			}
		}
	}
	return num_sim_vox;
	//cout << "!!!! num of sim vox: " << num_sim_vox << endl;
}

void Kernel::linkMesh_VolMesh(Mesh* &m, VoxMesh* &vm, int grid_density_in)
{
	double voxel_dim = 1.0/double(grid_density_in);
	int dim = grid_density_in;
	double vox_cube = voxel_dim * voxel_dim * voxel_dim;
	//save index of voxel for each node in original mesh
	for (node_iterator ni = m->node_list.begin(); ni < m->node_list.end(); ++ni)
	{
		double x1 = (ni->coordinate[0] + 0.5)/voxel_dim;
		double y1 = (ni->coordinate[1] + 0.5)/voxel_dim;
		double z1 = (ni->coordinate[2] + 0.5)/voxel_dim;

		int x = floor((ni->coordinate.x() + 0.5)/voxel_dim);
		int y = floor((ni->coordinate.y() + 0.5)/voxel_dim);
		int z = floor((ni->coordinate.z() + 0.5)/voxel_dim);
		if( x == dim)
			x --;
		if( y == dim)
			y --;
		if( z == dim)
			z --;
		int vox_idx = z*dim*dim + y*dim + x;
		if(vm->vox_locator[vox_idx] != NULL)
		{
		int cluster_idx = vm->vox_locator[vox_idx]->clusterid;
		//cout << "cluster id:" << cluster_idx << endl;
		ni->clearPara();
		ni->incident_cluster.push_back(&vm->cluster_list[cluster_idx]);

		//also, calculate parameters for vertex interpolation
		//ni->list_interpolation_nodes.push_back();
		//{
		//	v_mesh->vox_list.back().para_interpolate[0] = ((line_cube -  x_cube) * (line_cube -  y_cube) * (line_cube -  z_cube))/ vol_cube;
		//	v_mesh->vox_list.back().para_interpolate[1] = (x_cube * y_cube * z_cube) / vol_cube;
		//	v_mesh->vox_list.back().para_interpolate[2] = ((line_cube - x_cube) * y_cube * (line_cube - z_cube)) / vol_cube;
		//	v_mesh->vox_list.back().para_interpolate[3] = (x_cube * (line_cube - y_cube) * z_cube) / vol_cube;
		//	v_mesh->vox_list.back().para_interpolate[4] = ((line_cube - x_cube) * (line_cube - y_cube) * z_cube) / vol_cube;
		//	v_mesh->vox_list.back().para_interpolate[5] = (x_cube * y_cube * (line_cube - z_cube)) / vol_cube;
		//	v_mesh->vox_list.back().para_interpolate[6] = (x_cube * (line_cube - y_cube) * (line_cube - z_cube)) / vol_cube;
		//	v_mesh->vox_list.back().para_interpolate[7] = ((line_cube - x_cube) * y_cube * z_cube) / vol_cube;
		//}		

		ni->list_interpolation_nodes[0].push_back(vm->vox_locator[vox_idx]->node_0);
		ni->list_interpolation_nodes[1].push_back(vm->vox_locator[vox_idx]->node_1);
		ni->list_interpolation_nodes[2].push_back(vm->vox_locator[vox_idx]->node_2);
		ni->list_interpolation_nodes[3].push_back(vm->vox_locator[vox_idx]->node_3);
		ni->list_interpolation_nodes[4].push_back(vm->vox_locator[vox_idx]->node_4);
		ni->list_interpolation_nodes[5].push_back(vm->vox_locator[vox_idx]->node_5);
		ni->list_interpolation_nodes[6].push_back(vm->vox_locator[vox_idx]->node_6);
		ni->list_interpolation_nodes[7].push_back(vm->vox_locator[vox_idx]->node_7);

		//cout << vox_line << endl;
		
		ni->computePara(vox_cube, 0);
		/*
		ni->para_interpolate[0] = (abs(ni->list_interpolation_nodes[6]->coordinate.x() - ni->coordinate.x()) 
			* abs(ni->list_interpolation_nodes[6]->coordinate.y() - ni->coordinate.y()) 
			* abs(ni->list_interpolation_nodes[6]->coordinate.z() - ni->coordinate.z())) / vox_cube;
		
		ni->para_interpolate[1] = (abs(ni->list_interpolation_nodes[7]->coordinate.x() - ni->coordinate.x()) 
			* abs(ni->list_interpolation_nodes[7]->coordinate.y() - ni->coordinate.y()) 
			* abs(ni->list_interpolation_nodes[7]->coordinate.z() - ni->coordinate.z())) / vox_cube;

		ni->para_interpolate[2] = (abs(ni->list_interpolation_nodes[4]->coordinate.x() - ni->coordinate.x()) 
			* abs(ni->list_interpolation_nodes[4]->coordinate.y() - ni->coordinate.y()) 
			* abs(ni->list_interpolation_nodes[4]->coordinate.z() - ni->coordinate.z())) / vox_cube;

		ni->para_interpolate[3] = (abs(ni->list_interpolation_nodes[5]->coordinate.x() - ni->coordinate.x()) 
			* abs(ni->list_interpolation_nodes[5]->coordinate.y() - ni->coordinate.y()) 
			* abs(ni->list_interpolation_nodes[5]->coordinate.z() - ni->coordinate.z())) / vox_cube;

		ni->para_interpolate[4] = (abs(ni->list_interpolation_nodes[2]->coordinate.x() - ni->coordinate.x()) 
			* abs(ni->list_interpolation_nodes[2]->coordinate.y() - ni->coordinate.y()) 
			* abs(ni->list_interpolation_nodes[2]->coordinate.z() - ni->coordinate.z())) / vox_cube;
		
		ni->para_interpolate[5] = (abs(ni->list_interpolation_nodes[3]->coordinate.x() - ni->coordinate.x()) 
			* abs(ni->list_interpolation_nodes[3]->coordinate.y() - ni->coordinate.y()) 
			* abs(ni->list_interpolation_nodes[3]->coordinate.z() - ni->coordinate.z())) / vox_cube;

		ni->para_interpolate[6] = (abs(ni->list_interpolation_nodes[0]->coordinate.x() - ni->coordinate.x()) 
			* abs(ni->list_interpolation_nodes[0]->coordinate.y() - ni->coordinate.y()) 
			* abs(ni->list_interpolation_nodes[0]->coordinate.z() - ni->coordinate.z())) / vox_cube;

		ni->para_interpolate[7] = (abs(ni->list_interpolation_nodes[1]->coordinate.x() - ni->coordinate.x()) 
			* abs(ni->list_interpolation_nodes[1]->coordinate.y() - ni->coordinate.y()) 
			* abs(ni->list_interpolation_nodes[1]->coordinate.z() - ni->coordinate.z())) / vox_cube;
*/

		}
		else
		{
			ni->clearPara();
		}
			//cout << "node inx:" << x1 << " " << y1 << " " << z1 << endl;
		//double sum = 0.0;
		//for(int i = 0; i < 8; i++)
		//	sum += ni->para_interpolate[i];
		//cout << sum << endl;
	}
}

void Kernel::linkMesh_VolMeshFromParentMesh(Mesh* &m, VoxMesh* &vm, VoxMesh* &parent_mesh, int d, int d_parent, int level_index_parent)
{
	double voxel_dim = 1.0/double(d * d_parent);
	double vox_cube = voxel_dim * voxel_dim * voxel_dim;
	//save index of voxel for each node in original mesh
	for (node_iterator ni = m->node_list.begin(); ni < m->node_list.end(); ++ni)
	{
		int xp = floor((ni->coordinate.x() + 0.5) * d * d_parent);
		int yp = floor((ni->coordinate.y() + 0.5) * d * d_parent);
		int zp = floor((ni->coordinate.z() + 0.5) * d * d_parent);
		if( xp == (d * d_parent))
			xp --;
		if( yp == (d * d_parent))
			yp --;
		if( zp == (d * d_parent))
			zp --;
		if(!ni->incident_cluster.empty())//[level_index_parent]->vox_list.size() > 0 && ni->incident_cluster[level_index_parent]->vox_list[0] != NULL)
		{
			//ni->incident_cluster.erase(ni->incident_cluster.begin()+level_index_parent+1, ni->incident_cluster.end());
			ni->clearPara(level_index_parent+1);

			Vox * parentVox = ni->incident_cluster[level_index_parent]->vox_list[0];

			int z = zp - parentVox->coord_grid[0] * d;
			int y = yp - parentVox->coord_grid[1] * d;
			int x = xp - parentVox->coord_grid[2] * d;

			if (x == d)
				x --;
			if (y == d)
				y --;
			if (z == d)
				z --;

			int parent_index = parentVox->coord_grid[0]*d_parent*d_parent + parentVox->coord_grid[1]*d_parent + parentVox->coord_grid[2];
			int vox_idx = parent_index *d*d*d + z*d*d + y*d + x;

			if(vm->vox_locator[vox_idx] != NULL)
			{
			//testoutput << vox_idx << endl;
			int cluster_idx = vm->vox_locator[vox_idx]->clusterid;
			//cout << "cluster id:" << cluster_idx << endl;
		
			ni->incident_cluster.push_back(&vm->cluster_list[cluster_idx]);

			ni->list_interpolation_nodes[0].push_back(vm->vox_locator[vox_idx]->node_0);
			ni->list_interpolation_nodes[1].push_back(vm->vox_locator[vox_idx]->node_1);
			ni->list_interpolation_nodes[2].push_back(vm->vox_locator[vox_idx]->node_2);
			ni->list_interpolation_nodes[3].push_back(vm->vox_locator[vox_idx]->node_3);
			ni->list_interpolation_nodes[4].push_back(vm->vox_locator[vox_idx]->node_4);
			ni->list_interpolation_nodes[5].push_back(vm->vox_locator[vox_idx]->node_5);
			ni->list_interpolation_nodes[6].push_back(vm->vox_locator[vox_idx]->node_6);
			ni->list_interpolation_nodes[7].push_back(vm->vox_locator[vox_idx]->node_7);

			//cout << vox_line << endl;
		
			ni->computePara(vox_cube, ni->incident_cluster.size()-1);
			/*
			ni->para_interpolate[0] = (abs(ni->list_interpolation_nodes[6]->coordinate.x() - ni->coordinate.x()) 
				* abs(ni->list_interpolation_nodes[6]->coordinate.y() - ni->coordinate.y()) 
				* abs(ni->list_interpolation_nodes[6]->coordinate.z() - ni->coordinate.z())) / vox_cube;
		
			ni->para_interpolate[1] = (abs(ni->list_interpolation_nodes[7]->coordinate.x() - ni->coordinate.x()) 
				* abs(ni->list_interpolation_nodes[7]->coordinate.y() - ni->coordinate.y()) 
				* abs(ni->list_interpolation_nodes[7]->coordinate.z() - ni->coordinate.z())) / vox_cube;

			ni->para_interpolate[2] = (abs(ni->list_interpolation_nodes[4]->coordinate.x() - ni->coordinate.x()) 
				* abs(ni->list_interpolation_nodes[4]->coordinate.y() - ni->coordinate.y()) 
				* abs(ni->list_interpolation_nodes[4]->coordinate.z() - ni->coordinate.z())) / vox_cube;

			ni->para_interpolate[3] = (abs(ni->list_interpolation_nodes[5]->coordinate.x() - ni->coordinate.x()) 
				* abs(ni->list_interpolation_nodes[5]->coordinate.y() - ni->coordinate.y()) 
				* abs(ni->list_interpolation_nodes[5]->coordinate.z() - ni->coordinate.z())) / vox_cube;

			ni->para_interpolate[4] = (abs(ni->list_interpolation_nodes[2]->coordinate.x() - ni->coordinate.x()) 
				* abs(ni->list_interpolation_nodes[2]->coordinate.y() - ni->coordinate.y()) 
				* abs(ni->list_interpolation_nodes[2]->coordinate.z() - ni->coordinate.z())) / vox_cube;
		
			ni->para_interpolate[5] = (abs(ni->list_interpolation_nodes[3]->coordinate.x() - ni->coordinate.x()) 
				* abs(ni->list_interpolation_nodes[3]->coordinate.y() - ni->coordinate.y()) 
				* abs(ni->list_interpolation_nodes[3]->coordinate.z() - ni->coordinate.z())) / vox_cube;

			ni->para_interpolate[6] = (abs(ni->list_interpolation_nodes[0]->coordinate.x() - ni->coordinate.x()) 
				* abs(ni->list_interpolation_nodes[0]->coordinate.y() - ni->coordinate.y()) 
				* abs(ni->list_interpolation_nodes[0]->coordinate.z() - ni->coordinate.z())) / vox_cube;

			ni->para_interpolate[7] = (abs(ni->list_interpolation_nodes[1]->coordinate.x() - ni->coordinate.x()) 
				* abs(ni->list_interpolation_nodes[1]->coordinate.y() - ni->coordinate.y()) 
				* abs(ni->list_interpolation_nodes[1]->coordinate.z() - ni->coordinate.z())) / vox_cube;
				*/
			}
			else
				ni->clearPara();
		}
		else
			ni->clearPara();
	
	}
}

int Kernel::mark_fillInterior4Voxel(unsigned int * p_voxel, int d_current)
{
	// instead of searching the interior unmarked voxel, we flood exterior voxel
	// as long as the surface mesh does not have hole
	int vox_idx = mark_getUnmarkGridSurface4Voxel(p_voxel, d_current);

	int dim = d_current;
	int dim2 = dim*dim;

	// there exits a surface voxel
	while (vox_idx != -1)
	{
		stack<int> vox_stack;
		vox_stack.push(vox_idx);

		while (!vox_stack.empty())
		{
			int idx  = vox_stack.top();
			vox_stack.pop();

			p_voxel[idx] = 2;

			int idx_z = int (idx / dim2);
			int idx_y = int ((idx - idx_z*dim2) / dim);
			int idx_x = idx - idx_z*dim2 - idx_y*dim;

			// right
			if (idx_x + 1 < dim)
			{
				if (!p_voxel[idx_z*dim2 + idx_y*dim + idx_x + 1])
				{
					vox_stack.push(idx_z*dim2 + idx_y*dim + idx_x + 1);
				}
			}
			// left
			if (idx_x - 1 >= 0)
			{
				if (!p_voxel[idx_z*dim2 + idx_y*dim + idx_x - 1])
				{
					vox_stack.push(idx_z*dim2 + idx_y*dim + idx_x - 1);
				}
			}
			// up
			if (idx_y + 1 < dim)
			{
				if (!p_voxel[idx_z*dim2 + (idx_y+1)*dim + idx_x])
				{
					vox_stack.push(idx_z*dim2 + (idx_y+1)*dim + idx_x);
				}
			}
			// down
			if (idx_y - 1 >= 0)
			{
				if (!p_voxel[idx_z*dim2 + (idx_y-1)*dim + idx_x])
				{
					vox_stack.push(idx_z*dim2 + (idx_y-1)*dim + idx_x);
				}
			}
			// front
			if (idx_z + 1 < dim)
			{
				if (!p_voxel[(idx_z+1)*dim2 + idx_y*dim + idx_x])
				{
					vox_stack.push((idx_z+1)*dim2 + idx_y*dim + idx_x);
				}
			}
			// back
			if (idx_z - 1 >= 0)
			{
				if (!p_voxel[(idx_z-1)*dim2 + idx_y*dim + idx_x])
				{
					vox_stack.push((idx_z-1)*dim2 + idx_y*dim + idx_x);
				}
			}
		}

		vox_idx = mark_getUnmarkGridSurface4Voxel(p_voxel, d_current);

	} // end while voxel = -1

	// the rest zeros are interior 
	for (int i=0; i<dim*dim*dim; ++i)
	{
		if (!p_voxel[i])
		{
			p_voxel[i] = 1;
			++num_sim_vox;
		}
	}

	return num_sim_vox;
}

int Kernel::mark_fillInterior4VoxelLevel(unsigned int * p_voxel, int d_current, int d_parent)
{
	// instead of searching the interior unmarked voxel, we flood exterior voxel
	// as long as the surface mesh does not have hole
	int vox_idx = mark_getUnmarkGridSurface4VoxelLevel(p_voxel, d_current, d_parent);
	//int vox_idx = 0;

	int dimp = d_parent;
	int dimp2 = dimp * dimp;

	int dim = d_current;
	int dim2 = dim*dim;
	int dim3 = dim2*dim;

	int idx_p;
	int idx_pz;
	int idx_py;
	int idx_px;

	int idx_z;
	int idx_y;
	int idx_x;

	int idx_p2;
	int value;
	int count = 0;

	//vector<int> vox_vector;
	// there exits a surface voxel
	while (vox_idx != -1)
	{
		stack<int> vox_stack;
		vox_stack.push(vox_idx);
		//vox_vector.push_back(vox_idx);
		//test
		//for(int i = 0; i < 10000; i++)
		//	vox_stack.push(i);
		//while(!vox_stack.empty())
		//	vox_stack.pop();
		//vox_idx = -1;
		//test

		while (!vox_stack.empty())
		//while (!vox_vector.empty())
		{
			int idx  = vox_stack.top();
			vox_stack.pop();
			//int idx = vox_vector.back();
			//vox_vector.erase(vox_vector.end()-1);

			p_voxel[idx] = 2;

			idx_p = idx / dim3;
			idx_pz = idx_p / dimp2;
			idx_py = (idx_p - dimp2 * idx_pz) / dimp;
			idx_px = idx_p - dimp2 * idx_pz - idx_py * dimp;

			idx_z = int ((idx - idx_p * dim3) / dim2);
			idx_y = int ((idx - idx_p * dim3 - idx_z * dim2) / dim);
			idx_x = idx - idx_p * dim3 - idx_z * dim2 - idx_y * dim;

			idx_p2 = 0;

			// right
			if (idx_x + 1 < dim)
			{
				if (!p_voxel[idx_p * dim3 + idx_z*dim2 + idx_y*dim + idx_x + 1])
				{
					value = idx_p * dim3 + idx_z*dim2 + idx_y*dim + idx_x + 1;
					vox_stack.push(value);
					//cout << value << endl;
					//vox_vector.push_back(idx_p * dim3 + idx_z*dim2 + idx_y*dim + idx_x + 1);
				}
			}
			else if(idx_x + 1 == dim && idx_px + 1 < dimp)
			{
				idx_p2 = idx_pz * dimp2 + idx_py * dimp + idx_px + 1;
				if (!p_voxel[idx_p2 * dim3 + idx_z*dim2 + idx_y*dim])
				{
					value = idx_p2 * dim3 + idx_z*dim2 + idx_y*dim;
					vox_stack.push(value);
					//cout << value << endl;
					//vox_vector.push_back(idx_p2 * dim3 + idx_z*dim2 + idx_y*dim);
				}
			}
			if(value > (dim3 * dimp2 * dimp - 1))
				value = 0;
			
			// left
			if (idx_x - 1 >= 0)
			{
				if (!p_voxel[idx_p * dim3 + idx_z*dim2 + idx_y*dim + idx_x - 1])
				{
					value = idx_p * dim3 + idx_z*dim2 + idx_y*dim + idx_x - 1;
					vox_stack.push(value);
					//cout << value << endl;
					//vox_vector.push_back(idx_p * dim3 + idx_z*dim2 + idx_y*dim + idx_x - 1);
				}
			}
			else if (idx_x == 0 && idx_px - 1 >= 0)
			{
				idx_p2 = idx_pz * dimp2 + idx_py * dimp + idx_px - 1;
				if (!p_voxel[idx_p2 * dim3 + idx_z*dim2 + idx_y*dim + dim - 1])
				{
					value = idx_p2 * dim3 + idx_z*dim2 + idx_y*dim + dim - 1;
					vox_stack.push(value);
					//cout << value << endl;
					//vox_vector.push_back(idx_p2 * dim3 + idx_z*dim2 + idx_y*dim + dim - 1);
				}
			}
			if(value > (dim3 * dimp2 * dimp - 1))
				value = 0;
			// up
			if (idx_y + 1 < dim)
			{
				if (!p_voxel[idx_p * dim3 + idx_z*dim2 + (idx_y+1)*dim + idx_x])
				{
					value = idx_p * dim3 + idx_z*dim2 + (idx_y+1)*dim + idx_x;
					vox_stack.push(value);
					//cout << value << endl;
					//vox_vector.push_back(idx_p * dim3 + idx_z*dim2 + (idx_y+1)*dim + idx_x);
				}
			}
			else 
			if (idx_y + 1 == dim && idx_py + 1 < dimp)
			{
				idx_p2 = idx_pz * dimp2 + (idx_py + 1) * dimp + idx_px;
				if (!p_voxel[idx_p2 * dim3 + idx_z*dim2 + idx_x])
				{
					value = idx_p2 * dim3 + idx_z*dim2 + idx_x;
					vox_stack.push(value);
					//cout << value << endl;
					//vox_vector.push_back(idx_p2 * dim3 + idx_z*dim2 + idx_x);
				}
			}
			if(value > (dim3 * dimp2 * dimp - 1))
				value = 0;
			// down
			if (idx_y - 1 >= 0)
			{
				if (!p_voxel[idx_p * dim3 + idx_z*dim2 + (idx_y-1)*dim + idx_x])
				{
					value = idx_p * dim3 + idx_z*dim2 + (idx_y-1)*dim + idx_x;
					vox_stack.push(value);
					//cout << value << endl;
					//vox_vector.push_back(idx_p * dim3 + idx_z*dim2 + (idx_y-1)*dim + idx_x);
				}
			}
			else if (idx_y == 0 && idx_py - 1 >= 0)
			{
				idx_p2 = idx_pz * dimp2 + (idx_py - 1) * dimp + idx_px;
				if (!p_voxel[idx_p2 * dim3 +idx_z*dim2 + dim2 - dim + idx_x])
				{
					value = idx_p2 * dim3 +idx_z*dim2 + dim2 - dim + idx_x;
					vox_stack.push(value);
					//cout << value << endl;
					//vox_vector.push_back(idx_p2 * dim3 +idx_z*dim2 + dim2 - dim + idx_x);
				}
			}
			if(value > (dim3 * dimp2 * dimp - 1))
				value = 0;
			// front
			if (idx_z + 1 < dim)
			{
				if (!p_voxel[idx_p * dim3 + (idx_z+1)*dim2 + idx_y*dim + idx_x])
				{
					value = idx_p * dim3 + (idx_z+1)*dim2 + idx_y*dim + idx_x;
					vox_stack.push(value);
					//cout << value << endl;
					//vox_vector.push_back(idx_p * dim3 + (idx_z+1)*dim2 + idx_y*dim + idx_x);
				}
			}
			else if (idx_z + 1 == dim && idx_pz + 1 < dimp)
			{
				idx_p2 = (idx_pz + 1) * dimp2 + idx_py * dimp + idx_px;
				if (!p_voxel[idx_p2 * dim3 + idx_y*dim + idx_x])
				{
					value = idx_p2 * dim3 + idx_y*dim + idx_x;
					vox_stack.push(value);
					//cout << value << endl;
					//vox_vector.push_back(idx_p2 * dim3 + idx_y*dim + idx_x);
				}
			}
			
			if(value > (dim3 * dimp2 * dimp - 1))
				value = 0;
			// back
			if (idx_z - 1 >= 0)
			{
				if (!p_voxel[idx_p * dim3 + (idx_z-1)*dim2 + idx_y*dim + idx_x])
				{
					value = idx_p * dim3 + (idx_z-1)*dim2 + idx_y*dim + idx_x;
					vox_stack.push(value);
					//cout << value << endl;
					//vox_vector.push_back(idx_p * dim3 + (idx_z-1)*dim2 + idx_y*dim + idx_x);
				}
			}
			else if (idx_z == 0 && idx_pz - 1 >= 0)
			{
				idx_p2 = (idx_pz - 1) * dimp2 + idx_py * dimp + idx_px;
				if (!p_voxel[idx_p2 * dim3 + dim3 - dim2 + idx_y*dim + idx_x])
				{
					value = idx_p2 * dim3 + dim3 - dim2 + idx_y*dim + idx_x;
					vox_stack.push(value);
					//cout << value << endl;
					//vox_vector.push_back(idx_p2 * dim3 + dim3 - dim2 + idx_y*dim + idx_x);
				}
			}
			if(value > (dim3 * dimp2 * dimp - 1))
				value = 0;

			//cout << "size:" << vox_stack.size() << endl;
			//count ++;
			//if(count == 1)
			//	break;
		}
		
		vox_idx = mark_getUnmarkGridSurface4VoxelLevel(p_voxel, d_current, d_parent);
		
		//vox_idx = -1;
	} // end while voxel = -1

	// the rest zeros are interior 
	for (int i=0; i<dim*dim*dim*dimp*dimp*dimp; ++i)
	{
		if (!p_voxel[i])
		{
			p_voxel[i] = 1;
			++num_sim_vox;
		}
	}

	return num_sim_vox;
}

int Kernel::mark_getUnmarkGridSurface4Voxel(unsigned int * p_voxel, int d_current)
{
	// search for unmarked voxel at the surface of the grid
	// voxel which p_voxel == 0

	int dim = d_current;
	int dim2 = d_current * d_current;

	// up face 
	for (int idx_x = 0; idx_x < dim; ++idx_x)
	{
		for (int idx_z = 0; idx_z < dim; ++idx_z)
		{
			if (!p_voxel[idx_x + idx_z*dim2 + dim2 - dim])
				return (idx_x + idx_z*dim2 + dim2 - dim);
		}
	}

	// bottom face
	for (int idx_x = 0; idx_x < dim; ++idx_x)
	{
		for (int idx_z = 0; idx_z < dim; ++idx_z)
		{
			if (!p_voxel[idx_x + idx_z*dim2])
				return (idx_x + idx_z*dim2);
		}
	}

	// left face
	for (int idx_y = 0; idx_y < dim; ++idx_y)
	{
		for (int idx_z = 0; idx_z < dim; ++idx_z)
		{
			if (!p_voxel[idx_y*dim + idx_z*dim2])
				return (idx_y*dim + idx_z*dim2);
		}
	}

	// right face
	for (int idx_y = 0; idx_y < dim; ++idx_y)
	{
		for (int idx_z = 0; idx_z < dim; ++idx_z)
		{
			if (!p_voxel[idx_y*dim + idx_z*dim2 + dim - 1])
				return (idx_y*dim + idx_z*dim2 + dim - 1);
		}
	}

	// front face
	for (int idx_x = 0; idx_x < dim; ++idx_x)
	{
		for (int idx_y = 0; idx_y < dim; ++idx_y)
		{
			if (!p_voxel[dim2*(dim-1) + idx_y*dim + idx_x])
				return (dim2*(dim-1) + idx_y*dim + idx_x);
		}
	}

	// back face
	for (int idx_x = 0; idx_x < dim; ++idx_x)
	{
		for (int idx_y = 0; idx_y < dim; ++idx_y)
		{
			if (!p_voxel[idx_y*dim + idx_x])
				return (idx_y*dim + idx_x);
		}
	}

	return -1;
}

int Kernel::mark_getUnmarkGridSurface4VoxelLevel(unsigned int * p_voxel, int d_current, int d_parent)
{
	// search for unmarked voxel at the surface of the grid
	// voxel which p_voxel == 0

	int dim = d_current;
	int dim2 = d_current * d_current;
	int dim3 = dim2 * dim;
	int dimp = d_parent;
	int dimp2 = d_parent * d_parent;

	// up face 
	for(int idx_px = 0; idx_px < dimp; ++ idx_px)
	{	
		for(int idx_pz = 0; idx_pz < dimp; ++ idx_pz)
		{
			int p = idx_px + idx_pz * dimp2 + dimp2 - dimp;
			for (int idx_x = 0; idx_x < dim; ++idx_x)
			{
				for (int idx_z = 0; idx_z < dim; ++idx_z)
				{
					if (!p_voxel[p*dim3 + idx_x + idx_z*dim2 + dim2 - dim])
						return (p*dim3 + idx_x + idx_z*dim2 + dim2 - dim);
				}
			}
		}
	}
	// bottom face
	for (int idx_px = 0; idx_px < dimp; ++idx_px)
	{
		for (int idx_pz = 0; idx_pz < dimp; ++idx_pz)
		{
			int p = idx_px + idx_pz * dimp2;
			for (int idx_x = 0; idx_x < dim; ++idx_x)
			{
				for (int idx_z = 0; idx_z < dim; ++idx_z)
				{
					if (!p_voxel[p*dim3 + idx_x + idx_z*dim2])
						return (p*dim3 + idx_x + idx_z*dim2);
				}
			}
		}
	}

	// left face
	for (int idx_py = 0; idx_py < dimp; ++idx_py)
	{
		for (int idx_pz = 0; idx_pz < dimp; ++idx_pz)
		{
			int p = idx_py * dimp + idx_pz * dimp2;
			for (int idx_y = 0; idx_y < dim; ++idx_y)
			{
				for (int idx_z = 0; idx_z < dim; ++idx_z)
				{
					if (!p_voxel[p*dim3 + idx_y*dim + idx_z*dim2])
						return (p*dim3 + idx_y*dim + idx_z*dim2);
				}
			}
		}
	}

	// right face
	for (int idx_py = 0; idx_py < dimp; ++idx_py)
	{
		for (int idx_pz = 0; idx_pz < dimp; ++idx_pz)
		{
			int p = idx_py * dimp + idx_pz * dimp2 + dimp - 1;
			for (int idx_y = 0; idx_y < dim; ++idx_y)
			{
				for (int idx_z = 0; idx_z < dim; ++idx_z)
				{
					if (!p_voxel[p*dim3 + idx_y*dim + idx_z*dim2 + dim - 1])
						return (p*dim3 + idx_y*dim + idx_z*dim2 + dim - 1);
				}
			}
		}
	}

	// front face
	for (int idx_px = 0; idx_px < dimp; ++idx_px)
	{
		for (int idx_py = 0; idx_py < dimp; ++idx_py)
		{
			int p = dimp2 * (dimp - 1) + idx_py * dimp + idx_px;
			for (int idx_x = 0; idx_x < dim; ++idx_x)
			{
				for (int idx_y = 0; idx_y < dim; ++idx_y)
				{
					if (!p_voxel[p*dim3 + dim2*(dim-1) + idx_y*dim + idx_x])
						return (p*dim3 + dim2*(dim-1) + idx_y*dim + idx_x);
				}
			}
		}
	}

	// back face
	for (int idx_px = 0; idx_px < dimp; ++idx_px)
	{
		for (int idx_py = 0; idx_py < dimp; ++idx_py)
		{
			int p = idx_py * dimp + idx_px;
			for (int idx_x = 0; idx_x < dim; ++idx_x)
			{
				for (int idx_y = 0; idx_y < dim; ++idx_y)
				{
					if (!p_voxel[p*dim3 + idx_y*dim + idx_x])
						return (p*dim3 + idx_y*dim + idx_x);
				}
			}
		}
	}

	return -1;
}

int Kernel::mark_markSurfaceVox4Voxel(unsigned int * p_voxel, int d_current)
{
	//int vox_idx = mark_getUnmarkGridSurface4Voxel(p_voxel, d_current);

	int dim = d_current;
	int dim2 = dim*dim;
	int dim3 = dim2*dim;
		
	for (int i=0; i<dim3; ++i)
	{
		if (p_voxel[i] == 1)
		{
			int idx_z = int (i / dim2);
			int idx_y = int ((i - idx_z*dim2) / dim);
			int idx_x = i - idx_z*dim2 - idx_y*dim;

			if (idx_x == 0 || idx_y == 0 || idx_z == 0 || idx_x == dim - 1 || idx_y == dim - 1 || idx_z == dim - 1)
			{
				p_voxel[i] = 3;
				++num_surface_vox;
				continue;
			}

			// right
			if (idx_x + 1 < dim)
			{
				if (p_voxel[idx_z*dim2 + idx_y*dim + idx_x + 1] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			// left
			if (idx_x - 1 >= 0)
			{
				if (p_voxel[idx_z*dim2 + idx_y*dim + idx_x - 1] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			// up
			if (idx_y + 1 < dim)
			{
				if (p_voxel[idx_z*dim2 + (idx_y-1)*dim + idx_x] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			// down
			if (idx_y - 1 >= 0)
			{
				if (p_voxel[idx_z*dim2 + (idx_y+1)*dim + idx_x] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			// front
			if (idx_z + 1 < dim)
			{
				if (p_voxel[(idx_z+1)*dim2 + idx_y*dim + idx_x] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			// back
			if (idx_z - 1 >= 0)
			{
				if (p_voxel[(idx_z-1)*dim2 + idx_y*dim + idx_x] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
		}
	}

	return num_surface_vox;
}

int Kernel::mark_markSurfaceVox4VoxelLevel(unsigned int * p_voxel, int d_current, int d_parent)
{
	int dimp = d_parent;
	int dimp2 = dimp * dimp;

	int dim = d_current;
	int dim2 = dim*dim;
	int dim3 = dim2*dim;
		
	for (int i=0; i<dim3; ++i)
	{
		if (p_voxel[i] == 1)
		{
			
			int idx_p = i / dim3;
			int idx_pz = idx_p / dimp2;
			int idx_py = (idx_p - dimp2 * idx_pz) / dimp2;
			int idx_px = idx_p - dimp2 * idx_pz - idx_py * dimp2;

			int idx_z = int ((i - idx_p * dim3) / dim2);
			int idx_y = int ((i - idx_p * dim3 - idx_z * dim2) / dim);
			int idx_x = i - idx_p * dim3 - idx_z * dim2 - idx_y * dim;

			int idx_p2 = 0;

			if ((idx_x == 0 && idx_px == 0)
				|| (idx_y == 0 && idx_py == 0)
				|| (idx_z == 0 && idx_pz == 0)
				|| (idx_x == dim - 1 && idx_px == dimp - 1) 
				|| (idx_y == dim - 1 && idx_py == dimp - 1)
				|| (idx_z == dim - 1 && idx_pz == dimp - 1))
			{
				p_voxel[i] = 3;
				++num_surface_vox;
				continue;
			}

			// right
			if (idx_x + 1 < dim)
			{
				if (p_voxel[idx_p * dim3 + idx_z*dim2 + idx_y*dim + idx_x + 1] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			else if(idx_x + 1 == dim && idx_px + 1 < dimp)
			{
				idx_p2 = idx_pz * dimp2 + idx_py * dimp + idx_px + 1;
				if (p_voxel[idx_p2 * dim3 + idx_z*dim2 + idx_y*dim] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			// left
			if (idx_x - 1 >= 0)
			{
				if (p_voxel[idx_p * dim3 + idx_z*dim2 + idx_y*dim + idx_x - 1] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			else if (idx_x == 0 && idx_px - 1 >= 0)
			{
				idx_p2 = idx_pz * dimp2 + idx_py * dimp + idx_px - 1;
				if (p_voxel[idx_p2 * dim3 + idx_z*dim2 + idx_y*dim + dim - 1] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			// up
			if (idx_y + 1 < dim)
			{
				if (p_voxel[idx_p * dim3 + idx_z*dim2 + (idx_y-1)*dim + idx_x] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			else if (idx_y + 1 == dim && idx_py + 1 < dim)
			{
				idx_p2 = idx_pz * dimp2 + (idx_py + 1) * dimp + idx_px;
				if (p_voxel[idx_p2 * dim3 + idx_z*dim2 + idx_x] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			// down
			if (idx_y - 1 >= 0)
			{
				if (p_voxel[idx_p * dim3 + idx_z*dim2 + (idx_y+1)*dim + idx_x] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			else if (idx_y == 0 && idx_py - 1 >= 0)
			{
				idx_p2 = idx_pz * dimp2 + (idx_py - 1) * dimp + idx_px;
				if (p_voxel[idx_p2 * dim3 + idx_z*dim2 + dim2 - dim + idx_x] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			// front
			if (idx_z + 1 < dim)
			{
				if (p_voxel[idx_p * dim3 + (idx_z+1)*dim2 + idx_y*dim + idx_x] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			else if (idx_z + 1 == dim && idx_pz + 1 < dim)
			{
				idx_p2 = (idx_pz + 1) * dimp2 + idx_py * dimp + idx_px;
				if (p_voxel[idx_p2 * dim3 + idx_y*dim + idx_x] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			// back
			if (idx_z - 1 >= 0)
			{
				if (p_voxel[idx_p * dim3 + (idx_z-1)*dim2 + idx_y*dim + idx_x] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
			else if (idx_z == 0 && idx_pz - 1 >= 0)
			{
				idx_p2 = (idx_pz - 1) * dimp2 + idx_py * dimp + idx_px;
				if (p_voxel[idx_p2 * dim3 + dim3 - dim2 + idx_y*dim + idx_x] == 2)
				{
					p_voxel[i] = 3;
					++num_surface_vox;
					continue;
				}
			}
		}
	}

	return num_surface_vox;
}

void Kernel::generateVoxMesh(const unsigned int* vol, int dim, VoxMesh* &v_mesh)
{
	v_mesh->clear();
	v_mesh->vox_list.reserve(num_sim_vox);
	v_mesh->node_list.reserve(num_sim_vox * 8);
	v_mesh->surface_vox_list.reserve(num_surface_vox);
	v_mesh->surface_node_list.reserve(num_surface_vox * 8);
	v_mesh->vox_locator.assign(dim*dim*dim, NULL);

	v_mesh->num_node = 0;
	v_mesh->num_vox = 0;
	v_mesh->num_surface_vox = 0;
	v_mesh->num_surface_node = 0;

	//vector<Vox*> aux_vox_list(dim*dim*dim, NULL);

	double half_size = 0.5 * 1.0 / double(dim);

	// 1 means internal voxel; 2 means external voxel; and 3 means surface voxel
	for (int z = 0; z < dim; ++z)
	{
		for (int y = 0; y < dim; ++y)
		{
			for (int x = 0; x < dim; ++x)
			{
				if (vol[z*dim*dim + y*dim + x] != 2)
				{
					// a new voxel anyway
					Vox new_vox;
					new_vox.half_size = half_size;
					new_vox.vox_center(0) = double(x)*half_size*2.0 + half_size - 0.5;
					new_vox.vox_center(1) = double(y)*half_size*2.0 + half_size - 0.5;
					new_vox.vox_center(2) = double(z)*half_size*2.0 + half_size - 0.5;
					new_vox.coord_grid(0) = z;
					new_vox.coord_grid(1) = y;
					new_vox.coord_grid(2) = x;
					
					if (x-1>=0 && y-1>=0 && z-1>=0)
					{
						if (v_mesh->vox_locator[(z-1)*dim*dim + (y-1)*dim + x-1] != NULL)
						{
							new_vox.node_4 = v_mesh->vox_locator[(z-1)*dim*dim + (y-1)*dim + x-1]->node_2;
						}
					}

					if (y-1>=0 && z-1>=0)
					{
						if (v_mesh->vox_locator[(z-1)*dim*dim + (y-1)*dim + x] != NULL)
						{
							new_vox.node_4 = v_mesh->vox_locator[(z-1)*dim*dim + (y-1)*dim + x]->node_3;
							new_vox.node_5 = v_mesh->vox_locator[(z-1)*dim*dim + (y-1)*dim + x]->node_2;
						}
					}

					if (x+1<dim && y-1>=0 && z-1>=0)
					{
						if (v_mesh->vox_locator[(z-1)*dim*dim + (y-1)*dim + x+1] != NULL)
						{
							new_vox.node_5 = v_mesh->vox_locator[(z-1)*dim*dim + (y-1)*dim + x+1]->node_3;
						}
					}

					if (x-1>=0 && z-1>=0)
					{
						if (v_mesh->vox_locator[(z-1)*dim*dim + y*dim + x-1] != NULL)
						{
							new_vox.node_0 = v_mesh->vox_locator[(z-1)*dim*dim + y*dim + x-1]->node_2;
							new_vox.node_4 = v_mesh->vox_locator[(z-1)*dim*dim + y*dim + x-1]->node_6;
						}
					}

					if (z-1>=0)
					{
						if (v_mesh->vox_locator[(z-1)*dim*dim + y*dim + x] != NULL)
						{
							new_vox.node_0 = v_mesh->vox_locator[(z-1)*dim*dim + y*dim + x]->node_3;
							new_vox.node_1 = v_mesh->vox_locator[(z-1)*dim*dim + y*dim + x]->node_2;
							new_vox.node_5 = v_mesh->vox_locator[(z-1)*dim*dim + y*dim + x]->node_6;
							new_vox.node_4 = v_mesh->vox_locator[(z-1)*dim*dim + y*dim + x]->node_7;
						}
					}

					if (x+1<dim && z-1>=0)
					{
						if (v_mesh->vox_locator[(z-1)*dim*dim + y*dim + x+1] != NULL)
						{
							new_vox.node_1 = v_mesh->vox_locator[(z-1)*dim*dim + y*dim + x+1]->node_3;
							new_vox.node_5 = v_mesh->vox_locator[(z-1)*dim*dim + y*dim + x+1]->node_7;
						}
					}

					if (x-1>=0 && y+1<dim && z-1>=0)
					{
						if (v_mesh->vox_locator[(z-1)*dim*dim + (y+1)*dim + x-1] != NULL)
						{
							new_vox.node_0 = v_mesh->vox_locator[(z-1)*dim*dim + (y+1)*dim + x-1]->node_6;
						}
					}

					if (y+1<dim && z-1>=0)
					{
						if (v_mesh->vox_locator[(z-1)*dim*dim + (y+1)*dim + x] != NULL)
						{
							new_vox.node_0 = v_mesh->vox_locator[(z-1)*dim*dim + (y+1)*dim + x]->node_7;
							new_vox.node_1 = v_mesh->vox_locator[(z-1)*dim*dim + (y+1)*dim + x]->node_6;
						}
					}

					if (x+1<dim && y+1<dim && z-1>=0)
					{
						if (v_mesh->vox_locator[(z-1)*dim*dim + (y+1)*dim + x+1] != NULL)
						{
							new_vox.node_1 = v_mesh->vox_locator[(z-1)*dim*dim + (y+1)*dim + x+1]->node_7;
						}
					}

					if (x-1>=0 && y-1>=0)
					{
						if (v_mesh->vox_locator[z*dim*dim + (y-1)*dim + x-1] != NULL)
						{
							new_vox.node_4 = v_mesh->vox_locator[z*dim*dim + (y-1)*dim + x-1]->node_1;
							new_vox.node_7 = v_mesh->vox_locator[z*dim*dim + (y-1)*dim + x-1]->node_2;
						}
					}

					if (y-1>=0)
					{
						if (v_mesh->vox_locator[z*dim*dim + (y-1)*dim + x] != NULL)
						{
							new_vox.node_4 = v_mesh->vox_locator[z*dim*dim + (y-1)*dim + x]->node_0;
							new_vox.node_5 = v_mesh->vox_locator[z*dim*dim + (y-1)*dim + x]->node_1;
							new_vox.node_6 = v_mesh->vox_locator[z*dim*dim + (y-1)*dim + x]->node_2;
							new_vox.node_7 = v_mesh->vox_locator[z*dim*dim + (y-1)*dim + x]->node_3;
						}
					}

					if (x+1<dim && y-1>=0)
					{
						if (v_mesh->vox_locator[z*dim*dim + (y-1)*dim + x+1] != NULL)
						{
							new_vox.node_5 = v_mesh->vox_locator[z*dim*dim + (y-1)*dim + x+1]->node_0;
							new_vox.node_6 = v_mesh->vox_locator[z*dim*dim + (y-1)*dim + x+1]->node_3;
						}
					}

					if (x-1>=0)
					{
						if (v_mesh->vox_locator[z*dim*dim + y*dim + x-1] != NULL)
						{
							new_vox.node_0 = v_mesh->vox_locator[z*dim*dim + y*dim + x-1]->node_1;
							new_vox.node_4 = v_mesh->vox_locator[z*dim*dim + y*dim + x-1]->node_5;
							new_vox.node_7 = v_mesh->vox_locator[z*dim*dim + y*dim + x-1]->node_6;
							new_vox.node_3 = v_mesh->vox_locator[z*dim*dim + y*dim + x-1]->node_2;
						}
					}

					// create node
					if (new_vox.node_0 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) -= half_size;
						new_node.coordinate(1) += half_size;
						new_node.coordinate(2) -= half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_0 = &v_mesh->node_list.back();
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
					}

					if (new_vox.node_1 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) += half_size;
						new_node.coordinate(1) += half_size;
						new_node.coordinate(2) -= half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_1 = &v_mesh->node_list.back();
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
					}

					if (new_vox.node_2 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) += half_size;
						new_node.coordinate(1) += half_size;
						new_node.coordinate(2) += half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_2 = &v_mesh->node_list.back();
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
					}

					if (new_vox.node_3 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) -= half_size;
						new_node.coordinate(1) += half_size;
						new_node.coordinate(2) += half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_3 = &v_mesh->node_list.back();
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
					}

					if (new_vox.node_4 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) -= half_size;
						new_node.coordinate(1) -= half_size;
						new_node.coordinate(2) -= half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_4 = &v_mesh->node_list.back();
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
					}

					if (new_vox.node_5 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) += half_size;
						new_node.coordinate(1) -= half_size;
						new_node.coordinate(2) -= half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_5 = &v_mesh->node_list.back();
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
					}

					if (new_vox.node_6 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) += half_size;
						new_node.coordinate(1) -= half_size;
						new_node.coordinate(2) += half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_6 = &v_mesh->node_list.back();
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
					}

					if (new_vox.node_7 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) -= half_size;
						new_node.coordinate(1) -= half_size;
						new_node.coordinate(2) += half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_7 = &v_mesh->node_list.back();
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
					}

					// detect render surface
					if (vol[z*dim*dim + y*dim + x] == 3)
					{
						new_vox.flag_surface_vox = true;

						if (x == 0)
						{
							new_vox.flag_left_face = true;
							new_vox.node_0->flag_surface_node = true;
							new_vox.node_4->flag_surface_node = true;
							new_vox.node_7->flag_surface_node = true;
							new_vox.node_3->flag_surface_node = true;
						}
						else if (vol[z*dim*dim + y*dim + x - 1] == 2)
						{
							new_vox.flag_left_face = true;
							new_vox.node_0->flag_surface_node = true;
							new_vox.node_4->flag_surface_node = true;
							new_vox.node_7->flag_surface_node = true;
							new_vox.node_3->flag_surface_node = true;
						}

						if (x == dim-1)
						{
							new_vox.flag_right_face = true;
							new_vox.node_1->flag_surface_node = true;
							new_vox.node_5->flag_surface_node = true;
							new_vox.node_2->flag_surface_node = true;
							new_vox.node_6->flag_surface_node = true;
						}
						else if (vol[z*dim*dim + y*dim + x + 1] == 2)
						{
							new_vox.flag_right_face = true;
							new_vox.node_1->flag_surface_node = true;
							new_vox.node_5->flag_surface_node = true;
							new_vox.node_2->flag_surface_node = true;
							new_vox.node_6->flag_surface_node = true;
						}

						if (y == 0)
						{
							new_vox.flag_bottom_face = true;
							new_vox.node_4->flag_surface_node = true;
							new_vox.node_5->flag_surface_node = true;
							new_vox.node_7->flag_surface_node = true;
							new_vox.node_6->flag_surface_node = true;
						}
						else if (vol[z*dim*dim + (y-1)*dim + x] == 2)
						{
							new_vox.flag_bottom_face = true;
							new_vox.node_4->flag_surface_node = true;
							new_vox.node_5->flag_surface_node = true;
							new_vox.node_7->flag_surface_node = true;
							new_vox.node_6->flag_surface_node = true;
						}

						if (y == dim-1)
						{
							new_vox.flag_top_face = true;
							new_vox.node_0->flag_surface_node = true;
							new_vox.node_1->flag_surface_node = true;
							new_vox.node_2->flag_surface_node = true;
							new_vox.node_3->flag_surface_node = true;
						}
						else if (vol[z*dim*dim + (y+1)*dim + x] == 2)
						{
							new_vox.flag_top_face = true;
							new_vox.node_0->flag_surface_node = true;
							new_vox.node_1->flag_surface_node = true;
							new_vox.node_2->flag_surface_node = true;
							new_vox.node_3->flag_surface_node = true;
						}

						if (z == 0)
						{
							new_vox.flag_back_face = true;
							new_vox.node_0->flag_surface_node = true;
							new_vox.node_1->flag_surface_node = true;
							new_vox.node_4->flag_surface_node = true;
							new_vox.node_5->flag_surface_node = true;
						}
						else if (vol[(z-1)*dim*dim + y*dim + x] == 2)
						{
							new_vox.flag_back_face = true;
							new_vox.node_0->flag_surface_node = true;
							new_vox.node_1->flag_surface_node = true;
							new_vox.node_4->flag_surface_node = true;
							new_vox.node_5->flag_surface_node = true;
						}

						if (z == dim-1)
						{
							new_vox.flag_front_face = true;
							new_vox.node_2->flag_surface_node = true;
							new_vox.node_3->flag_surface_node = true;
							new_vox.node_6->flag_surface_node = true;
							new_vox.node_7->flag_surface_node = true;
						}
						else if (vol[(z+1)*dim*dim + y*dim + x] == 2)
						{
							new_vox.flag_front_face = true;
							new_vox.node_2->flag_surface_node = true;
							new_vox.node_3->flag_surface_node = true;
							new_vox.node_6->flag_surface_node = true;
							new_vox.node_7->flag_surface_node = true;
						}
					}
					
					// insert new voxel
					v_mesh->vox_list.push_back(new_vox);
					v_mesh->vox_locator[z*dim*dim + y*dim + x] = &v_mesh->vox_list.back(); 

					if (new_vox.flag_surface_vox)
					{
						v_mesh->surface_vox_list.push_back(&v_mesh->vox_list.back());
					}
				}
			}
		}
	}
	
	for(int i=0; i<v_mesh->node_list.size(); ++i)
	{
		if (v_mesh->node_list[i].flag_surface_node)
		{
			v_mesh->surface_node_list.push_back(&v_mesh->node_list[i]);
		}
	}

	v_mesh->num_node = v_mesh->node_list.size();
	v_mesh->num_surface_vox = v_mesh->surface_vox_list.size();
	v_mesh->num_vox = v_mesh->vox_list.size();
	v_mesh->num_surface_node = v_mesh->surface_node_list.size();
	v_mesh->vox_size = 1.0 / double(dim);

	int vox_origin = 0;
	int vox_neighbor = 0;

	// update neighbor info.
	for (int z = 0; z < dim; ++z)
	{
		for (int y = 0; y < dim; ++y)
		{
			for (int x = 0; x < dim; ++x)
			{
				if (vol[z*dim*dim + y*dim + x] != 2)
				{
					vox_origin = z*dim*dim + y*dim + x;
					//back
					if (z-1>=0)
					{
						vox_neighbor = (z-1)*dim*dim + y*dim + x;
						if (v_mesh->vox_locator[vox_neighbor] != NULL)
						{
							v_mesh->vox_locator[vox_origin]->back_neighbor = v_mesh->vox_locator[vox_neighbor];
							v_mesh->vox_locator[vox_neighbor]->front_neighbor = v_mesh->vox_locator[vox_origin];
						}
					}
					//bottom
					if (y-1>=0)
					{
						vox_neighbor = z*dim*dim + (y-1)*dim + x;
						if (v_mesh->vox_locator[vox_neighbor] != NULL)
						{
							v_mesh->vox_locator[vox_origin]->top_neighbor = v_mesh->vox_locator[vox_neighbor];
							v_mesh->vox_locator[vox_neighbor]->bottom_neighbor = v_mesh->vox_locator[vox_origin];
						}
					}
					//left
					if (x-1>=0)
					{
						vox_neighbor = z*dim*dim + y*dim + x-1;
						if (v_mesh->vox_locator[vox_neighbor] != NULL)
						{
							v_mesh->vox_locator[vox_origin]->left_neighbor = v_mesh->vox_locator[vox_neighbor];
							v_mesh->vox_locator[vox_neighbor]->right_neighbor = v_mesh->vox_locator[vox_origin];
						}
					}
					//left back neighbor
					if (z-1>=0 && x-1>=0)
					{
						vox_neighbor = (z-1)*dim*dim + y*dim + x-1;
						if (v_mesh->vox_locator[vox_neighbor] != NULL)
						{
							v_mesh->vox_locator[vox_origin]->left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
							v_mesh->vox_locator[vox_neighbor]->right_front_neighbor = v_mesh->vox_locator[vox_origin];
						}
					}
					//right back neighbor
					if (z-1>=0 && x+1 < dim)
					{
						vox_neighbor = (z-1)*dim*dim + y*dim + x+1;
						if (v_mesh->vox_locator[vox_neighbor] != NULL)
						{
							v_mesh->vox_locator[vox_origin]->right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
							v_mesh->vox_locator[vox_neighbor]->left_front_neighbor = v_mesh->vox_locator[vox_origin];
						}
					}
					// bottom back neighbor
					if (z-1>=0 && y-1>=0)
					{
						vox_neighbor = (z-1)*dim*dim + (y-1)*dim + x;
						if (v_mesh->vox_locator[vox_neighbor] != NULL)
						{
							v_mesh->vox_locator[vox_origin]->bottom_back_neighbor = v_mesh->vox_locator[vox_neighbor];
							v_mesh->vox_locator[vox_neighbor]->top_front_neighbor = v_mesh->vox_locator[vox_origin];
						}
					}

					//bottom left neighbor
					if (x-1>=0 && y-1>=0)
					{
						vox_neighbor = z*dim*dim + (y-1)*dim + x-1;
						if (v_mesh->vox_locator[vox_neighbor] != NULL)
						{
							v_mesh->vox_locator[vox_origin]->bottom_left_neighbor = v_mesh->vox_locator[vox_neighbor];
							v_mesh->vox_locator[vox_neighbor]->top_right_neighbor = v_mesh->vox_locator[vox_origin];
						}
					}
					//bottom right neighbor
					if (y-1>=0 && x+1 < dim)
					{
						vox_neighbor = z*dim*dim + (y-1)*dim + x+1;
						if (v_mesh->vox_locator[vox_neighbor] != NULL)
						{
							v_mesh->vox_locator[vox_origin]->bottom_right_neighbor = v_mesh->vox_locator[vox_neighbor];
							v_mesh->vox_locator[vox_neighbor]->top_left_neighbor = v_mesh->vox_locator[vox_origin];
						}
					}
					//bottom left back
					if (x-1>=0 && y-1>=0 && z-1>=0)
					{
						vox_neighbor = (z-1)*dim*dim + (y-1)*dim + x-1;
						if (v_mesh->vox_locator[vox_neighbor] != NULL)
						{
							v_mesh->vox_locator[vox_origin]->bottom_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
							v_mesh->vox_locator[vox_neighbor]->top_right_front_neighbor = v_mesh->vox_locator[vox_origin];
						}
					}
					//bottom right back
					if (x+1<dim && y-1>=0 && z-1>=0)
					{
						vox_neighbor = (z-1)*dim*dim + (y-1)*dim + x+1;
						if (v_mesh->vox_locator[vox_neighbor] != NULL)
						{
							v_mesh->vox_locator[vox_origin]->bottom_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
							v_mesh->vox_locator[vox_neighbor]->top_left_front_neighbor = v_mesh->vox_locator[vox_origin];
						}
					}

					//top back
					if (y+1<dim && z-1>=0)
					{
						vox_neighbor = (z-1)*dim*dim + (y+1)*dim + x;
						if (v_mesh->vox_locator[vox_neighbor] != NULL)
						{
							v_mesh->vox_locator[vox_origin]->top_back_neighbor = v_mesh->vox_locator[vox_neighbor];
							v_mesh->vox_locator[vox_neighbor]->bottom_front_neighbor = v_mesh->vox_locator[vox_origin];
						}
					}

					//top left
					if (y+1<dim && x-1>=0)
					{
						vox_neighbor = z*dim*dim + (y+1)*dim + x-1;
						if (v_mesh->vox_locator[vox_neighbor] != NULL)
						{
							v_mesh->vox_locator[vox_origin]->top_left_neighbor = v_mesh->vox_locator[vox_neighbor];
							v_mesh->vox_locator[vox_neighbor]->bottom_right_neighbor = v_mesh->vox_locator[vox_origin];
						}
					}
					//top left back
					if (x-1>=0 && y+1<dim && z-1>=0)
					{
						vox_neighbor = (z-1)*dim*dim + (y+1)*dim + x-1;
						if (v_mesh->vox_locator[vox_neighbor] != NULL)
						{
							v_mesh->vox_locator[vox_origin]->top_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
							v_mesh->vox_locator[vox_neighbor]->bottom_right_front_neighbor = v_mesh->vox_locator[vox_origin];
						}
					}
					//top right back
					if (x+1<dim && y+1<dim && z-1>=0)
					{
						vox_neighbor = (z-1)*dim*dim + (y+1)*dim + x+1;
						if (v_mesh->vox_locator[vox_neighbor] != NULL)
						{
							v_mesh->vox_locator[vox_origin]->top_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
							v_mesh->vox_locator[vox_neighbor]->bottom_left_front_neighbor = v_mesh->vox_locator[vox_origin];
						}
					}



				}
			}
		}
	} // end updating neighbor

	//using vox_locator to locate vox for every vertex of original mesh
	

}

void Kernel::generatePerVoxCluster(VoxMesh* &vm)
{
	for (node_iterator ni=vm->node_list.begin(); ni!=vm->node_list.end(); ++ni)
	{
		ni->incident_cluster.clear();
	}

	vm->cluster_list.clear();
	vm->cluster_list.reserve(vm->num_vox);

	
	for (vox_iterator vi=vm->vox_list.begin(); vi!=vm->vox_list.end(); ++vi)
	{
		Cluster new_cluster;
		new_cluster.vox_list.push_back(&(*vi));

		//add cluster pointer into vox
		vi->clusterid = vm->cluster_list.size();
			
		DuplicatedNode new_node_0(vi->node_0);
		DuplicatedNode new_node_1(vi->node_1);
		DuplicatedNode new_node_2(vi->node_2);
		DuplicatedNode new_node_3(vi->node_3);
		DuplicatedNode new_node_4(vi->node_4);
		DuplicatedNode new_node_5(vi->node_5);
		DuplicatedNode new_node_6(vi->node_6);
		DuplicatedNode new_node_7(vi->node_7);

		new_cluster.node_list.reserve(8);
		new_cluster.node_list.push_back(new_node_0);
		new_cluster.node_list.push_back(new_node_1);
		new_cluster.node_list.push_back(new_node_2);
		new_cluster.node_list.push_back(new_node_3);
		new_cluster.node_list.push_back(new_node_4);
		new_cluster.node_list.push_back(new_node_5);
		new_cluster.node_list.push_back(new_node_6);
		new_cluster.node_list.push_back(new_node_7);
						
		new_cluster.num_node = 8;
		new_cluster.num_surface_quads = 6;

		vm->cluster_list.push_back(new_cluster);

		// for per-vox cluster, every face is to be rendered
		vm->cluster_list.back().surface_quads.clear();
		vm->cluster_list.back().surface_quads.reserve(24);

		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[0]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[3]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[2]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[1]);

		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[4]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[5]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[6]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[7]);

		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[0]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[4]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[7]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[3]);

		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[1]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[2]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[6]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[5]);

		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[2]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[3]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[7]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[6]);

		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[0]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[1]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[5]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[4]);
		
		vi->node_0->incident_cluster.push_back(&vm->cluster_list.back());
		vi->node_1->incident_cluster.push_back(&vm->cluster_list.back());
		vi->node_2->incident_cluster.push_back(&vm->cluster_list.back());
		vi->node_3->incident_cluster.push_back(&vm->cluster_list.back());
		vi->node_4->incident_cluster.push_back(&vm->cluster_list.back());
		vi->node_5->incident_cluster.push_back(&vm->cluster_list.back());
		vi->node_6->incident_cluster.push_back(&vm->cluster_list.back());
		vi->node_7->incident_cluster.push_back(&vm->cluster_list.back());

		vi->node_0->duplicates.push_back(&(vm->cluster_list.back().node_list[0]));
		vi->node_1->duplicates.push_back(&(vm->cluster_list.back().node_list[1]));
		vi->node_2->duplicates.push_back(&(vm->cluster_list.back().node_list[2]));
		vi->node_3->duplicates.push_back(&(vm->cluster_list.back().node_list[3]));
		vi->node_4->duplicates.push_back(&(vm->cluster_list.back().node_list[4]));
		vi->node_5->duplicates.push_back(&(vm->cluster_list.back().node_list[5]));
		vi->node_6->duplicates.push_back(&(vm->cluster_list.back().node_list[6]));
		vi->node_7->duplicates.push_back(&(vm->cluster_list.back().node_list[7]));
	}

	vm->num_cluster = vm->cluster_list.size();
	//cout << vm->vox_list[1].clusterid  << endl;
	vector<Node>::const_iterator niter = vm->node_list.begin();
	int temp = 0;
	for (; niter != vm->node_list.end(); niter ++)
	{
		temp += niter->duplicates.size();
	}
	vm->num_pair = temp;
}

void Kernel::initializeSimulator()
{
	vector<Cluster>::iterator ci;
	vector<Cluster*>::iterator pci;
	vector<Node>::iterator node_iter;

	switch (used_simulator)
	{
	case SHAPE_MATCHING:
		for (ci=p_vox_mesh->cluster_list.begin(); ci!=p_vox_mesh->cluster_list.end(); ++ci)
		{
			ci->computeRestMassCentroid();
			ci->current_center = ci->original_center;
			ci->computeAQQ();
		}

		// link duplicates
		for (node_iter=p_vox_mesh->node_list.begin(); node_iter!=p_vox_mesh->node_list.end(); ++node_iter)
		{
			node_iter->duplicates.clear();
			node_iter->duplicates.reserve(p_vox_mesh->num_cluster);
			node_iter->static_position = node_iter->coordinate;
			node_iter->target_position = node_iter->coordinate;
		}

		for (ci=p_vox_mesh->cluster_list.begin(); ci!=p_vox_mesh->cluster_list.end(); ++ci)
		{
			vector<DuplicatedNode>::iterator dn_iter = ci->node_list.begin();
			for (; dn_iter!=ci->node_list.end(); ++dn_iter)
			{
				dn_iter->mapped_node->duplicates.push_back(&(*dn_iter));
				dn_iter->static_position = dn_iter->coordinate;
				dn_iter->target_position = dn_iter->coordinate;
			}
		}

		flag_simulator_ready = true;
		break;
	case HSM_FORCE4ITERATION:
		// link duplicate nodes	for all level
		for(int i = 0; i < level_list.size(); i++)
		{
			for (node_iter = level_list[i]->voxmesh_level->node_list.begin(); node_iter != level_list[i]->voxmesh_level->node_list.end(); ++node_iter)
			{
				node_iter->duplicates.clear();
				node_iter->duplicates.reserve(level_list[0]->voxmesh_level->num_cluster);
				node_iter->static_position = node_iter->coordinate;
				node_iter->target_position = node_iter->coordinate;
			}

			for (ci=level_list[i]->voxmesh_level->cluster_list.begin(); ci!=level_list[i]->voxmesh_level->cluster_list.end(); ++ci)
			{
				ci->computeRestMassCentroid();
				ci->current_center = ci->original_center;
				ci->computeAQQ();

				//ci->velocity_matrix.resize(3*ci->num_node, 6);

				vector<DuplicatedNode>::iterator dn_iter = ci->node_list.begin();
				for (; dn_iter!=ci->node_list.end(); ++dn_iter)
				{
					dn_iter->mapped_node->duplicates.push_back(&(*dn_iter));
					dn_iter->static_position = dn_iter->coordinate;
					dn_iter->target_position = dn_iter->coordinate;
				}

			}
		}

		flag_simulator_ready = true;
		break;
	case VELOCITY_MATCHING:
		// link duplicate nodes		
		for (node_iter=p_vox_mesh->node_list.begin(); node_iter!=p_vox_mesh->node_list.end(); ++node_iter)
		{
			node_iter->duplicates.clear();
			node_iter->duplicates.reserve(p_vox_mesh->num_cluster);
			node_iter->static_position = node_iter->coordinate;
			node_iter->target_position = node_iter->coordinate;
		}

		for (ci=p_vox_mesh->cluster_list.begin(); ci!=p_vox_mesh->cluster_list.end(); ++ci)
		{
			ci->velocity_matrix.resize(3*ci->num_node, 6);

			vector<DuplicatedNode>::iterator dn_iter = ci->node_list.begin();
			for (; dn_iter!=ci->node_list.end(); ++dn_iter)
			{
				dn_iter->mapped_node->duplicates.push_back(&(*dn_iter));
				dn_iter->static_position = dn_iter->coordinate;
				dn_iter->target_position = dn_iter->coordinate;
			}

		}
								
		flag_simulator_ready = true;
		break;
	case MULTIPLE_VELOCITY_MATCHING:
		// link duplicate nodes	for all level
		for(int i = 0; i < level_list.size(); i++)
		{
			for (node_iter = level_list[i]->voxmesh_level->node_list.begin(); node_iter != level_list[i]->voxmesh_level->node_list.end(); ++node_iter)
			{
				node_iter->duplicates.clear();
				node_iter->duplicates.reserve(level_list[0]->voxmesh_level->num_cluster);
				node_iter->static_position = node_iter->coordinate;
				node_iter->target_position = node_iter->coordinate;
			}

			for (ci=level_list[i]->voxmesh_level->cluster_list.begin(); ci!=level_list[i]->voxmesh_level->cluster_list.end(); ++ci)
			{
				ci->velocity_matrix.resize(3*ci->num_node, 6);

				vector<DuplicatedNode>::iterator dn_iter = ci->node_list.begin();
				for (; dn_iter!=ci->node_list.end(); ++dn_iter)
				{
					dn_iter->mapped_node->duplicates.push_back(&(*dn_iter));
					dn_iter->static_position = dn_iter->coordinate;
					dn_iter->target_position = dn_iter->coordinate;
				}

			}
		}						
		flag_simulator_ready = true;
		break;
	case PAIR_MATCHING:
		// link duplicate
		for (node_iter=p_vox_mesh->node_list.begin(); node_iter!=p_vox_mesh->node_list.end(); ++node_iter)
		{
			node_iter->duplicates.clear();
			node_iter->duplicates.reserve(p_vox_mesh->num_cluster);
			node_iter->static_position = node_iter->coordinate;
			node_iter->target_position = node_iter->coordinate;
		}

		for (ci=p_vox_mesh->cluster_list.begin(); ci!=p_vox_mesh->cluster_list.end(); ++ci)
		{
			vector<DuplicatedNode>::iterator dn_iter = ci->node_list.begin();
			for (; dn_iter!=ci->node_list.end(); ++dn_iter)
			{
				dn_iter->mapped_node->duplicates.push_back(&(*dn_iter));
				dn_iter->static_position = dn_iter->coordinate;
				dn_iter->target_position = dn_iter->coordinate;
			}

			ci->buildVelocityMatrix();
		}
		p_vox_mesh->initializeNeighborFlag();
		break;
		
	case SINGLE_GRID:
		for (ci=p_vox_mesh->cluster_list.begin(); ci!=p_vox_mesh->cluster_list.end(); ++ci)
		{
			ci->computeRestMassCentroid();
			ci->computeAQQ();
		}

		// link duplicates
		for (node_iter=p_vox_mesh->node_list.begin(); node_iter!=p_vox_mesh->node_list.end(); ++node_iter)
		{
			node_iter->duplicates.clear();
			node_iter->duplicates.reserve(p_vox_mesh->num_cluster);
			node_iter->static_position = node_iter->coordinate;
			node_iter->target_position = node_iter->coordinate;
		}

		for (ci=p_vox_mesh->cluster_list.begin(); ci!=p_vox_mesh->cluster_list.end(); ++ci)
		{
			vector<DuplicatedNode>::iterator dn_iter = ci->node_list.begin();
			for (; dn_iter!=ci->node_list.end(); ++dn_iter)
			{
				dn_iter->mapped_node->duplicates.push_back(&(*dn_iter));
				dn_iter->static_position = dn_iter->coordinate;
				dn_iter->target_position = dn_iter->coordinate;
			}
		}

		flag_simulator_ready = true;
		break;

	case HIERARCHY:
		p_vox_mesh->createBFSOrder();
		for (pci=p_vox_mesh->root_to_leaf_cluster_list.begin(); pci!=p_vox_mesh->root_to_leaf_cluster_list.end(); ++pci)
		{
			(*pci)->computeRestMassCentroid();
			(*pci)->computeAQQ();
		}

		// link duplicates
		for (node_iter=p_vox_mesh->node_list.begin(); node_iter!=p_vox_mesh->node_list.end(); ++node_iter)
		{
			node_iter->duplicates.clear();
			node_iter->duplicates.reserve(p_vox_mesh->root_to_leaf_cluster_list.size());
		}

		for (pci=p_vox_mesh->root_to_leaf_cluster_list.begin(); pci!=p_vox_mesh->root_to_leaf_cluster_list.end(); ++pci)
		{
			vector<DuplicatedNode>::iterator dn_iter = (*pci)->node_list.begin();
			for (; dn_iter!=(*pci)->node_list.end(); ++dn_iter)
			{
				dn_iter->mapped_node->duplicates.push_back(&(*dn_iter));
			}
		}

		// each duplicate node also links to its parent

		flag_simulator_ready = true;
		break;
		
	case SIMULATION_NETWORKING:


		flag_simulator_ready = true;
		break;
	case SIMULATION_MOBILE:
		flag_simulator_ready = true;
		break;
	case UNDEFINED:
		
		flag_simulator_ready = false;
		break;
	case HSM_ORIGINAL:
		for(int i = 0; i < level_list.size(); i++)
		{
			for (node_iter = level_list[i]->voxmesh_level->node_list.begin(); node_iter != level_list[i]->voxmesh_level->node_list.end(); ++node_iter)
			{
				node_iter->duplicates.clear();
				node_iter->duplicates.reserve(level_list[0]->voxmesh_level->num_cluster);
				node_iter->static_position = node_iter->coordinate;
				node_iter->target_position = node_iter->coordinate;
			}

			for (ci=level_list[i]->voxmesh_level->cluster_list.begin(); ci!=level_list[i]->voxmesh_level->cluster_list.end(); ++ci)
			{
				ci->computeRestMassCentroid();
				ci->current_center = ci->original_center;
				ci->computeAQQ();

				//ci->velocity_matrix.resize(3*ci->num_node, 6);

				vector<DuplicatedNode>::iterator dn_iter = ci->node_list.begin();
				for (; dn_iter!=ci->node_list.end(); ++dn_iter)
				{
					dn_iter->mapped_node->duplicates.push_back(&(*dn_iter));
					dn_iter->static_position = dn_iter->coordinate;
					dn_iter->target_position = dn_iter->coordinate;
				}

			}
		}

		flag_simulator_ready = true;
		break;
	case HSM_ONE_STEP:
		for(int i = 0; i < level_list.size(); i++)
		{
			for (node_iter = level_list[i]->voxmesh_level->node_list.begin(); node_iter != level_list[i]->voxmesh_level->node_list.end(); ++node_iter)
			{
				node_iter->duplicates.clear();
				node_iter->duplicates.reserve(level_list[0]->voxmesh_level->num_cluster);
				node_iter->static_position = node_iter->coordinate;
				node_iter->target_position = node_iter->coordinate;
			}

			for (ci=level_list[i]->voxmesh_level->cluster_list.begin(); ci!=level_list[i]->voxmesh_level->cluster_list.end(); ++ci)
			{
				ci->computeRestMassCentroid();
				ci->current_center = ci->original_center;
				ci->computeAQQ();

				//ci->velocity_matrix.resize(3*ci->num_node, 6);

				vector<DuplicatedNode>::iterator dn_iter = ci->node_list.begin();
				for (; dn_iter!=ci->node_list.end(); ++dn_iter)
				{
					dn_iter->mapped_node->duplicates.push_back(&(*dn_iter));
					dn_iter->static_position = dn_iter->coordinate;
					dn_iter->target_position = dn_iter->coordinate;
				}

			}
		}

		flag_simulator_ready = true;
		break;
	case HSM_FORCE4STEP:
	case HSM_FORCE4STEP_FIRST:
	case HSM_FORCE4STEP_FIRST1:
	case HSM_FORCE4STEP_FIRST2:
		for(int i = 0; i < level_list.size(); i++)
		{
			for (node_iter = level_list[i]->voxmesh_level->node_list.begin(); node_iter != level_list[i]->voxmesh_level->node_list.end(); ++node_iter)
			{
				node_iter->duplicates.clear();
				node_iter->duplicates.reserve(level_list[0]->voxmesh_level->num_cluster);
				node_iter->static_position = node_iter->coordinate;
				node_iter->target_position = node_iter->coordinate;
			}

			for (ci=level_list[i]->voxmesh_level->cluster_list.begin(); ci!=level_list[i]->voxmesh_level->cluster_list.end(); ++ci)
			{
				ci->computeRestMassCentroid();
				ci->current_center = ci->original_center;
				ci->computeAQQ();

				//ci->velocity_matrix.resize(3*ci->num_node, 6);

				vector<DuplicatedNode>::iterator dn_iter = ci->node_list.begin();
				for (; dn_iter!=ci->node_list.end(); ++dn_iter)
				{
					dn_iter->mapped_node->duplicates.push_back(&(*dn_iter));
					dn_iter->static_position = dn_iter->coordinate;
					dn_iter->static_position_timestep = dn_iter->coordinate;
					dn_iter->target_position = dn_iter->coordinate;
				}

			}
		}

		flag_simulator_ready = true;
		break;
	case HSM_ADAPTIVE_STEP:
		for(int i = 0; i < level_list.size(); i++)
		{
			for (node_iter = level_list[i]->voxmesh_level->node_list.begin(); node_iter != level_list[i]->voxmesh_level->node_list.end(); ++node_iter)
			{
				node_iter->duplicates.clear();
				node_iter->duplicates.reserve(level_list[0]->voxmesh_level->num_cluster);
				node_iter->static_position = node_iter->coordinate;
				node_iter->target_position = node_iter->coordinate;
			}

			for (ci=level_list[i]->voxmesh_level->cluster_list.begin(); ci!=level_list[i]->voxmesh_level->cluster_list.end(); ++ci)
			{
				ci->computeRestMassCentroid();
				ci->current_center = ci->original_center;
				ci->computeAQQ();

				//ci->velocity_matrix.resize(3*ci->num_node, 6);

				vector<DuplicatedNode>::iterator dn_iter = ci->node_list.begin();
				for (; dn_iter!=ci->node_list.end(); ++dn_iter)
				{
					dn_iter->mapped_node->duplicates.push_back(&(*dn_iter));
					dn_iter->static_position = dn_iter->coordinate;
					dn_iter->static_position_timestep = dn_iter->coordinate;
					dn_iter->target_position = dn_iter->coordinate;
				}

			}
		}

		flag_simulator_ready = true;
		break;
	}
}

double Kernel::getEnergyRatio(VoxMesh * vm, double &speed)
{
	double ratio = 0.0;
	speed = 0.0;
	if(vm->old_energy != 0)
	{
		speed = sqrt((vm->old_energy - vm->new_energy)/ ((double)(vm->num_pair) * vm->vox_size * vm->vox_size));
		ratio = (vm->old_energy - vm->new_energy)/ vm->old_energy;
	}
	return ratio;
}

bool Kernel::simulateNextStep4ShapeMatching()
{
	num_PE_perTimeStep = 0;
	Vector3d force_gravity = Vector3d::Zero();
	Vector3d force_wind = Vector3d::Zero();
	//set gravity
	if(flag_gravity)
	{
		force_gravity(1) = gravity_magnitude;
	}
	if(flag_setWindForce)
	{
		srand(time(NULL));
		wind_magnitude = (rand() % 100) * 0.01;
		force_wind = const_force * sin((time_step_index % 360) * PI / 180) * wind_magnitude;
	}

	if(flag_setForce && constraintType == Kernel::FORCE_CONSTRAINT)
	{
		if (!p_vox_mesh->constraint_node_list.empty())
		{
			int size_k = p_vox_mesh->constraint_node_list.size();
			for(int k = 0; k < size_k; k ++)
			{
				p_vox_mesh->constraint_node_list[k]->force = const_force;

				//revised here 0204
				for(int i=0; i < p_vox_mesh->constraint_node_list[k]->duplicates.size(); ++i)
				{
					p_vox_mesh->constraint_node_list[k]->duplicates[i]->force = const_force;
				}
			}
		}
	}
	time_counter->StartCounter();
	if(flag_setForce && constraintType != Kernel::FORCE_CONSTRAINT)
	{
		
		if (!p_vox_mesh->constraint_node_list.empty())
		{
			int size_k =p_vox_mesh->constraint_node_list.size();
			for(int k = 0; k < size_k; k ++)
			{
				p_vox_mesh->constraint_node_list[k]->prescribed_position = p_vox_mesh->constraint_node_list[k]->coordinate + const_force;
			}
		}
		
		/*
		if (!p_vox_mesh->constraint_node_list.empty())
		{
			int size_k =p_vox_mesh->constraint_node_list.size();
			for(int k = 0; k < size_k; k ++)
			{
				for (int l = 0; l < p_vox_mesh->constraint_node_list[k]->incident_cluster[0]->node_list.size(); l ++)
				{

					p_vox_mesh->constraint_node_list[k]->incident_cluster[0]->node_list[l].mapped_node->prescribed_position = const_force;
				}
			}
		}
		*/
	}
	double PI = 3.14159265;
	//if(flag_setWindForce)
	//{
	//	vector<Node>::iterator ni;
	//	for(ni = p_vox_mesh->node_list.begin(); ni != p_vox_mesh->node_list.end() ; ni ++)
	//	{
	//		ni->force = const_force* sin((time_step_index % 180) * PI / 180);

	//		vector<DuplicatedNode *>::iterator di;
	//		//revised here 0204
	//		for(di = ni->duplicates.begin() ; di != ni->duplicates.end(); ++ di)
	//		{
	//			(*di)->force = ni->force;
	//		}
	//	}
	//}
	if (flag_redo && constraintType == Kernel::FORCE_CONSTRAINT)
	{
		if (no_record < force_list.size())
		{
			if (!p_vox_mesh->constraint_node_list.empty())
			{
				int size_k = p_vox_mesh->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					p_vox_mesh->constraint_node_list[k]->force = force_list[no_record];

					for(int i=0; i < p_vox_mesh->constraint_node_list[k]->duplicates.size(); ++i)
					{
						p_vox_mesh->constraint_node_list[k]->duplicates[i]->force = force_list[no_record];
					}
				}
			}
			//flag_exportObj = true;
			//cout << force_list[no_record] << endl;
			no_record ++;
		}
		else
		{
			//
			//flag_exportObj = false;
			cout << "end of redo!" << endl;
			return true;
		}
	}
	vector<Cluster>::iterator ci = p_vox_mesh->cluster_list.begin();
	for (; ci!=p_vox_mesh->cluster_list.end(); ++ci)
	{
		ci->current_center.setZero();
		vector<DuplicatedNode>::const_iterator const_ni;
		double mass_sum = 0.0;
	
		for(const_ni=ci->node_list.begin(); const_ni!=ci->node_list.end(); ++const_ni)
		{
			ci->current_center += const_ni->target_position * const_ni->mass;
			mass_sum += const_ni->mass;
		}

		ci->current_center /= mass_sum;

		ci->a_pq = Matrix3d::Zero();
		Vector3d p, q;

		for(const_ni=ci->node_list.begin(); const_ni!=ci->node_list.end(); ++const_ni)
		{
			p = const_ni->target_position - ci->current_center;
			q = const_ni->coordinate - ci->original_center;
			ci->a_pq += const_ni->mass * p * q.transpose();
		}

		ci->a = ci->a_pq * ci->a_qq;
		

		JacobiSVD<Matrix3d> jacobi(ci->a_pq, ComputeFullU|ComputeFullV);
		ci->r = jacobi.matrixU() * jacobi.matrixV().transpose();

		//count for PE
		num_PE_perTimeStep++;
		
		double det_a = det33(ci->a);

		vector<DuplicatedNode>::iterator dni;
		for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
		{
			dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center;
			if (!dni->mapped_node->flag_anchor_node)
			{
				Vector3d _force = dni->force + force_gravity + force_wind;
				if(flag_dynamics)
				{
					dni->velocity = (1.0-ci->kappa)*dni->velocity + ci->alpha*(dni->static_position - dni->coordinate - dni->displacement) / time_step_size
						+ time_step_size * _force * force_scalar;

					dni->displacement += time_step_size*dni->velocity;

					dni->target_position = dni->coordinate + dni->displacement;
				}
				else
				{
					dni->target_position = dni->static_position + time_step_size * time_step_size * _force * force_scalar;
					dni->displacement = dni->target_position - dni->coordinate;
				}
				
			}
		}
	}

	//for rendering
	//p_vox_mesh->new_energy = 0.0;
	///////////////////////////////
	for (node_iterator ni=p_vox_mesh->node_list.begin(); ni!=p_vox_mesh->node_list.end(); ++ni)
	{
		if(flag_dynamics)
		{
			ni->displacement.setZero();
			ni->velocity.setZero();
		}
		ni->target_position.setZero();
		

		vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
		for (;dn!=ni->duplicates.end();++dn)
		{
			if(flag_dynamics)
			{
				ni->displacement += (*dn)->displacement;
				ni->velocity += (*dn)->velocity;
			}
			ni->target_position += (*dn)->target_position;
		}

		ni->target_position /= double(ni->duplicates.size());
		/*
		///////////////////////for rendering//////////////////////////////////////////////////////
		if(constraintType != Kernel::FORCE_CONSTRAINT  && ni->flag_constraint_node)
			ni->target_position = ni->prescribed_position;

		if(flag_dynamics)
		{
			ni->displacement /= double(ni->duplicates.size());
			ni->velocity /= double(ni->duplicates.size());
		}
		else
		{
			ni->displacement = ni->target_position - ni->coordinate;
		}

		dn = ni->duplicates.begin();
		for (;dn!=ni->duplicates.end();++dn)
		{
			if(!ni->flag_anchor_node)
				(*dn)->target_position = ni->target_position;

			if(flag_dynamics)
			{
				(*dn)->displacement = ni->displacement;
				(*dn)->velocity = ni->velocity;
			}
			else
			{
				(*dn)->displacement = ni->displacement;
			}
			p_vox_mesh->new_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
			p_vox_mesh->new_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
			p_vox_mesh->new_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		*/
	}


	///////////////////////////////////////////////////for experiment
	for (node_iterator nmi=p_mesh->node_list.begin(); nmi!=p_mesh->node_list.end(); ++nmi)
	{
		nmi->displacement.setZero();
		for(int i = 0; i < 8; i++)
		{
			//nmi->displacement += nmi->list_interpolation_nodes[i][0]->displacement * nmi->para_interpolate[i][0];
			nmi->displacement += (nmi->list_interpolation_nodes[i][level_display]->target_position - nmi->list_interpolation_nodes[i][level_display]->coordinate) * nmi->para_interpolate[i][level_display];
		}
	}
	p_vox_mesh->new_energy = 0.0;
	for (node_iterator ni=p_vox_mesh->node_list.begin(); ni!=p_vox_mesh->node_list.end(); ++ni)
	{
		if(constraintType != Kernel::FORCE_CONSTRAINT  && ni->flag_constraint_node)
			ni->target_position = ni->prescribed_position;

		if(flag_dynamics)
		{
			ni->displacement /= double(ni->duplicates.size());
			ni->velocity /= double(ni->duplicates.size());
		}
		else
		{
			ni->displacement = ni->target_position - ni->coordinate;
		}

		vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
		for (;dn!=ni->duplicates.end();++dn)
		{
			if(!ni->flag_anchor_node)
				(*dn)->target_position = ni->target_position;

			if(flag_dynamics)
			{
				(*dn)->displacement = ni->displacement;
				(*dn)->velocity = ni->velocity;
			}
			else
			{
				(*dn)->displacement = ni->displacement;
			}
			p_vox_mesh->new_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
			p_vox_mesh->new_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
			p_vox_mesh->new_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
		}
	}
	//cout << p_vox_mesh->new_energy << endl;
	//myEnergy << p_vox_mesh->new_energy << endl;
	////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////for rendering
	//for (node_iterator nmi=p_mesh->node_list.begin(); nmi!=p_mesh->node_list.end(); ++nmi)
	//{
	//	nmi->displacement.setZero();
	//	for(int i = 0; i < 8; i++)
	//	{
	//		nmi->displacement += nmi->list_interpolation_nodes[i][0]->displacement * nmi->para_interpolate[i][0];
	//	}
	//}
	////////////////////////////////////////////////////////////////////////////////

	//networking
	if(flag_network_ready && network_role == NETWORK_ROLE_SERVER)
	{
		for(int i = 0; i < pTransmitter.size(); i++)
		{
			if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_PC  && pTransmitter[i]->isClientReady)
				pTransmitter[i]->HSMupdate(data4PC);
			else if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_MOBILE)
				pTransmitter[i]->HSMupdate(data4Mobile);
		}
		
	}

	
	if(flag_setForce && !flag_setWindForce)
	{
		//double ratio, speed, threshold = 0.0;
		//ratio = getEnergyRatio(p_vox_mesh, speed);
		//threshold = abs(p_vox_mesh->new_energy - p_vox_mesh->old_energy);
		//if (!flag_converge)
		//{
		//	time_counter->StopCounter();
		//	testoutput << time_counter->GetElapsedTime() << endl;
		//	myEnergy << ratio << endl;
		//	myHEnergy << p_vox_mesh->new_energy << endl;
		//}
		////cout << threshold << endl;
		////if (p_vox_mesh->old_energy >= p_vox_mesh->new_energy && threshold < 0.0001 && !flag_converge)
		////{
		////	timestep_converge = time_step_index;
		////	flag_converge = true;
		////	cout << "Iteration #: " << timestep_converge - timestep_begin << endl;
		////	cout << "converge!" << endl;
		////}
		//p_vox_mesh->old_energy = p_vox_mesh->new_energy;
	}
	
	if(flag_exportTxt)
	{
		vector<Node>::iterator ni;
		for (ni = p_mesh->node_list.begin(); ni != p_mesh->node_list.end(); ++ni)
		{
			testoutput << "v " << ni->coordinate(0) + ni->displacement(0) 
				<< " " << ni->coordinate(1) + ni->displacement(1) 
				<< " " << ni->coordinate(2) + ni->displacement(2) << endl;
		}
	}
	if (flag_redo)
	{
		myEnergy << p_vox_mesh->new_energy << endl;
	}
	++time_step_index;
	return true;
}

bool Kernel::simulateNextStep4VelocityMatching()
{
	vector<Cluster>::iterator ci;
	//int count = 0;

	//double energy_before_iteration = 0;
	//double energy_after_iteration  = 0;

	//if (p_vox_mesh->constraint_node)
	//{
	//	p_vox_mesh->constraint_node->prescribed_position(0) = 0.3;
	//	p_vox_mesh->constraint_node->prescribed_position(1) = 0.8;
	//	p_vox_mesh->constraint_node->prescribed_position(2) = 0;
	//}
	
	for (ci=p_vox_mesh->cluster_list.begin(); ci!=p_vox_mesh->cluster_list.end(); ++ci)
	{
		VectorXd diff_vector(3*ci->num_node);
			
		for (int i=0; i<ci->num_node; ++i)
		{
			Vector3d p = ci->node_list[i].target_position;

			if (ci->node_list[i].flag_constraint_node)
			{
				diff_vector(3*i + 0) = ci->node_list[i].mapped_node->prescribed_position(0) - ci->node_list[i].static_position(0);
				diff_vector(3*i + 1) = ci->node_list[i].mapped_node->prescribed_position(1) - ci->node_list[i].static_position(1);
				diff_vector(3*i + 2) = ci->node_list[i].mapped_node->prescribed_position(2) - ci->node_list[i].static_position(2);
			}
			else
			{
				diff_vector(3*i + 0) = ci->node_list[i].target_position(0) - ci->node_list[i].static_position(0);
				diff_vector(3*i + 1) = ci->node_list[i].target_position(1) - ci->node_list[i].static_position(1);
				diff_vector(3*i + 2) = ci->node_list[i].target_position(2) - ci->node_list[i].static_position(2);
			}

			Matrix3d p_skew; 
			skew_symmetric(p, p_skew);

			ci->velocity_matrix.block<3, 3>(i*3, 0) = p_skew.transpose();
			ci->velocity_matrix.block<3, 3>(i*3, 3) = Matrix3d::Identity();

			if (ci->node_list[i].flag_constraint_node)
			{
				ci->constraint_node = &ci->node_list[i];
			}
		}

		//energy_before_iteration += diff_vector.norm();

		if(!ci->flag_constrained)
		{
			Matrix6d i66 = Matrix6d::Identity();
			Matrix6d matA = ci->velocity_matrix.transpose() * ci->velocity_matrix + ci->inertial_penalty * i66;
			Vector6d x = matA.ldlt().solve( ci->velocity_matrix.transpose() * diff_vector/time_step_size) ;

			ci->angular_velocity(0) = x(0);
			ci->angular_velocity(1) = x(1);
			ci->angular_velocity(2) = x(2);

			ci->linear_velocity(0) = x(3);
			ci->linear_velocity(1) = x(4);
			ci->linear_velocity(2) = x(5);
		}
		else
		{
			Vector3d c = (ci->constraint_node->mapped_node->prescribed_position - ci->constraint_node->target_position)/time_step_size;
			Vector3d p = ci->constraint_node->target_position;
			Matrix3d p_skew;
			skew_symmetric(p, p_skew);

			Matrix9d matM;
			Matrix6d i66 = Matrix6d::Identity();
			matM.block<6, 6>(0, 0) = ci->velocity_matrix.transpose() * ci->velocity_matrix + ci->inertial_penalty * i66;
			matM.block<3, 3>(6, 0) = p_skew.transpose();
			matM.block<3, 3>(6, 3) = Matrix3d::Identity();
			matM.block<3, 3>(0, 6) = p_skew;
			matM.block<3, 3>(3, 6) = Matrix3d::Identity();
			matM.block<3, 3>(6, 6) = Matrix3d::Zero();

			Vector9d d;
			d.segment<6>(0) = ci->velocity_matrix.transpose() * diff_vector / time_step_size; 
			d.segment<3>(6) = c;
			
			Vector9d x = matM.fullPivLu().solve(d);

			ci->angular_velocity(0) = x(0);
			ci->angular_velocity(1) = x(1);
			ci->angular_velocity(2) = x(2);

			ci->linear_velocity(0) = x(3);
			ci->linear_velocity(1) = x(4);
			ci->linear_velocity(2) = x(5);

			//cout << c << endl;
		}
		
		Vector3d delta_linear_displacement = ci->linear_velocity * time_step_size;
		Matrix3d delta_angular_displacement = Matrix3d::Zero();

		double angle = ci->angular_velocity.norm()*time_step_size;
			
		if (angle > 1e-3)
		{
			Matrix3d s;
			Vector3d axis_angle = ci->angular_velocity;
			axis_angle.normalize();
			skew_symmetric(axis_angle, s);
			delta_angular_displacement =  s*sin(angle) + s*s*(1.0 - cos(angle));
		}

		for (int i=0; i<ci->num_node; ++i)
		{
			ci->node_list[i].static_position += delta_linear_displacement + delta_angular_displacement*ci->node_list[i].static_position;

			//if (ci->node_list[i].flag_constraint_node)
			//{
			//	diff_vector(3*i + 0) = ci->node_list[i].mapped_node->prescribed_position(0) - ci->node_list[i].static_position(0);
			//	diff_vector(3*i + 1) = ci->node_list[i].mapped_node->prescribed_position(1) - ci->node_list[i].static_position(1);
			//	diff_vector(3*i + 2) = ci->node_list[i].mapped_node->prescribed_position(2) - ci->node_list[i].static_position(2);
			//}

			//else
			//{
			//	diff_vector(3*i + 0) = ci->node_list[i].target_position(0) - ci->node_list[i].static_position(0);
			//	diff_vector(3*i + 1) = ci->node_list[i].target_position(1) - ci->node_list[i].static_position(1);
			//	diff_vector(3*i + 2) = ci->node_list[i].target_position(2) - ci->node_list[i].static_position(2);
			//}
		}

		//energy_after_iteration += diff_vector.norm();
			
		//++ count;
	}

	// average at intersection and update target
	for (node_iterator ni=p_vox_mesh->node_list.begin(); ni!=p_vox_mesh->node_list.end(); ++ni)
	{
		ni->target_position.setZero();

		if (!ni->flag_anchor_node)
		{
			vector<DuplicatedNode*>::iterator dn;
			dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				ni->target_position += (*dn)->static_position;
			}

			ni->target_position /= double(ni->duplicates.size());

			dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				(*dn)->target_position = ni->target_position;
			}
		}
		else 
		{
			ni->target_position = ni->coordinate;
		}
	}
	
	//if (p_vox_mesh->constraint_node)
	//{
	//	out_data_1.push_back(energy_before_iteration);
	//	out_data_2.push_back(energy_after_iteration);
	//}

	//if (out_data_1.size() == 5000)
	//{
	//	saveOutputData(out_data_1, "B.txt");
	//	saveOutputData(out_data_2, "A.txt");
	//	cout << "data done!" << endl;
	//}
	
	//for other level save w, v and directly inherit from parent level

	++time_step_index;
	return true;
}

bool Kernel::simulateNextStep4MultipleVelocityMatching()
{
	vector<Cluster>::iterator ci;
	int count = 0;
	
	for (int n = 0; n < level_list.size(); n++)
	{
		//if level > 0, needs to inherit R, T from parent_level, and recompute static and target position
		if(n > 0)
		{
			vector<Cluster>::iterator ci;
			for (ci = level_list[n]->voxmesh_level->cluster_list.begin(); ci!=level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
			{
				////////////////////////////////////////////////////////////////////////////////////////
				//slerp to interpolate rotation
				////////////////////////////////////////////////////////////////////////////////////////
				Quaterniond q_c;
				vector<Quaterniond> list_q;
				vector<Vector3d> list_v;
				vector<double> t;
				double t_add = 0.0;
				if(ci->flag_constrained)
					continue;
				for(int p = 0; p < 8; p++)
				{
					if(ci->vox_list[0]->list_near_parentVox[p])
					{
						Quaterniond temp(level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].angular_displacement);
						list_q.push_back(temp);
						list_v.push_back(level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].linear_displacement);
						t.push_back(ci->vox_list[0]->para_interpolate[p]);
						ci->angular_displacement = level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].angular_displacement;
						ci->linear_displacement = level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].linear_displacement;
					}
				}
				if(list_q.size() > 1)
				{
					//// only consider 2 parent voxes
					//q_c = list_q[0].slerp(t[1], list_q[1]);
					//ci->angular_displacement = q_c.toRotationMatrix();
					//ci->linear_displacement = t[0] * list_v[0] + t[1] * list_v[1];

					q_c = list_q[0];
					ci->linear_displacement = t[0] * list_v[0];
					while(list_q.size() > 1)
					{
						t_add += t[0];
						double temp = (t_add * t_add) /(t_add * t_add + t[1] * t[1]);
						Quaterniond qtemp = list_q[1].slerp(temp, q_c);
						q_c = qtemp;

						ci->linear_displacement += t[1] * list_v[1];

						list_q.erase(list_q.begin());
						t.erase(t.begin());
						list_v.erase(list_v.begin());
					}
					ci->angular_displacement = q_c.toRotationMatrix();
				}
				for (int i=0; i<ci->num_node; ++i)
				{
					//computation based on original position
					ci->node_list[i].static_position = ci->angular_displacement * ci->node_list[i].mapped_node->coordinate + ci->linear_displacement;
				}
			}
			//compute target pos
			for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
			{
				ni->target_position.setZero();

				if (!ni->flag_anchor_node)
				{
					vector<DuplicatedNode*>::iterator dn;
					dn = ni->duplicates.begin();
					for (;dn!=ni->duplicates.end();++dn)
					{
						ni->target_position += (*dn)->static_position;
					}

					ni->target_position /= double(ni->duplicates.size());

					dn = ni->duplicates.begin();
					for (;dn!=ni->duplicates.end();++dn)
					{
						(*dn)->target_position = ni->target_position;
					}
				}
				else 
				{
					ni->target_position = ni->coordinate;

					vector<DuplicatedNode*>::iterator dn;
					dn = ni->duplicates.begin();
					for (;dn!=ni->duplicates.end();++dn)
					{
						(*dn)->target_position = ni->target_position;
					}
				}
			}



		}//end if
		//else{
		//begin velocity matching
		for (ci=level_list[n]->voxmesh_level->cluster_list.begin(); ci!=level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
		{
			VectorXd diff_vector(3*ci->num_node);
			ci->constraint_node_list.clear();
			////////////////////////test
			//cout << endl;
			///////////////////////////////
			for (int i=0; i<ci->num_node; ++i)
			{
				//Vector3d p = ci->node_list[i].target_position;
				Vector3d p = ci->node_list[i].static_position;

				if (ci->node_list[i].flag_constraint_node)
				{
					if(flag_constraint_fixed)
					{
						//for test, prescribed position would be fixed to one point
						ci->node_list[i].mapped_node->prescribed_position(0) = -0.128034;
						ci->node_list[i].mapped_node->prescribed_position(1) = 0.670210;
						ci->node_list[i].mapped_node->prescribed_position(2) = 0.217083;
					}

					diff_vector(3*i + 0) = ci->node_list[i].mapped_node->prescribed_position(0) - ci->node_list[i].static_position(0);
					diff_vector(3*i + 1) = ci->node_list[i].mapped_node->prescribed_position(1) - ci->node_list[i].static_position(1);
					diff_vector(3*i + 2) = ci->node_list[i].mapped_node->prescribed_position(2) - ci->node_list[i].static_position(2);

					//diff_vector(3*i + 0) = ci->node_list[i].static_position(0) - ci->node_list[i].mapped_node->prescribed_position(0);
					//diff_vector(3*i + 1) = ci->node_list[i].static_position(1) - ci->node_list[i].mapped_node->prescribed_position(1);
					//diff_vector(3*i + 2) = ci->node_list[i].static_position(2) - ci->node_list[i].mapped_node->prescribed_position(2);


				}
				else
				{
					diff_vector(3*i + 0) = ci->node_list[i].target_position(0) - ci->node_list[i].static_position(0);
					diff_vector(3*i + 1) = ci->node_list[i].target_position(1) - ci->node_list[i].static_position(1);
					diff_vector(3*i + 2) = ci->node_list[i].target_position(2) - ci->node_list[i].static_position(2);

					//diff_vector(3*i + 0) = ci->node_list[i].static_position(0) - ci->node_list[i].target_position(0);
					//diff_vector(3*i + 1) = ci->node_list[i].static_position(1) - ci->node_list[i].target_position(1);
					//diff_vector(3*i + 2) = ci->node_list[i].static_position(2) - ci->node_list[i].target_position(2);
					//if(diff_vector.isZero())
					//	cout << "same for" << i << endl;
				}

				Matrix3d p_skew; 
				skew_symmetric(p, p_skew);

				ci->velocity_matrix.block<3, 3>(i*3, 0) = p_skew.transpose();
				ci->velocity_matrix.block<3, 3>(i*3, 3) = Matrix3d::Identity();

				if (ci->node_list[i].flag_constraint_node)
				{
			 		//ci->constraint_node = &ci->node_list[i];
					ci->constraint_node_list.push_back(&ci->node_list[i]);
				}
			}

			//cout << diff_vector << endl;
			
			Matrix3d R4constraint = Matrix3d::Identity();
			if(!ci->flag_constrained)
			{
				Matrix6d i66 = Matrix6d::Identity();
				Matrix6d matA = ci->velocity_matrix.transpose() * ci->velocity_matrix + ci->inertial_penalty * i66;
				Vector6d x = matA.ldlt().solve( ci->velocity_matrix.transpose() * diff_vector/time_step_size) ;

				ci->angular_velocity(0) = x(0);
				ci->angular_velocity(1) = x(1);
				ci->angular_velocity(2) = x(2);

				ci->linear_velocity(0) = x(3);
				ci->linear_velocity(1) = x(4);
				ci->linear_velocity(2) = x(5);
			}
			else
			{
				count ++;
				if(ci->constraint_node_list.size() == 1)
				{
					//Vector3d c = (ci->constraint_node->mapped_node->prescribed_position - ci->constraint_node->target_position)/time_step_size;
					//Vector3d p = ci->constraint_node->target_position;

					//Vector3d c = (ci->constraint_node_list[0]->mapped_node->prescribed_position - ci->constraint_node_list[0]->target_position)/time_step_size;
					//Vector3d p = ci->constraint_node_list[0]->target_position;

					Vector3d c = (ci->constraint_node_list[0]->mapped_node->prescribed_position - ci->constraint_node_list[0]->static_position)/time_step_size;
					Vector3d p = ci->constraint_node_list[0]->static_position;

					Matrix3d p_skew;
					skew_symmetric(p, p_skew);

					Matrix9d matM;
					Matrix6d i66 = Matrix6d::Identity();
					matM.block<6, 6>(0, 0) = ci->velocity_matrix.transpose() * ci->velocity_matrix + ci->inertial_penalty * i66;
					matM.block<3, 3>(6, 0) = p_skew.transpose();
					matM.block<3, 3>(6, 3) = Matrix3d::Identity();
					matM.block<3, 3>(0, 6) = p_skew;
					matM.block<3, 3>(3, 6) = Matrix3d::Identity();
					matM.block<3, 3>(6, 6) = Matrix3d::Zero();

					Vector9d d;
					d.segment<6>(0) = ci->velocity_matrix.transpose() * diff_vector / time_step_size; 
					d.segment<3>(6) = c;
			
					Vector9d x = matM.fullPivLu().solve(d);

					ci->angular_velocity(0) = x(0);
					ci->angular_velocity(1) = x(1);
					ci->angular_velocity(2) = x(2);

					ci->linear_velocity(0) = x(3);
					ci->linear_velocity(1) = x(4);
					ci->linear_velocity(2) = x(5);
				}	
				else
				{
 					VectorXd c(3*ci->constraint_node_list.size());
					vector<Vector3d> P;
					vector<Matrix3d> P_skew;
					Matrix3d I = Matrix3d::Identity();
					for(int j = 0; j < ci->constraint_node_list.size(); j ++)
					{
						/*
						//c.segment<3>(3*j) = (ci->constraint_node_list[j]->mapped_node->prescribed_position - ci->constraint_node_list[j]->target_position)/time_step_size;
						//Vector3d p = ci->constraint_node_list[j]->target_position;
						
						c.segment<3>(3*j) = ( ci->constraint_node_list[j]->mapped_node->prescribed_position - ci->constraint_node_list[j]->static_position)/time_step_size;
						Vector3d p = ci->constraint_node_list[j]->static_position;

						Matrix3d p_skew;
						skew_symmetric(p, p_skew);
						 
						P.push_back(p);
						P_skew.push_back(p_skew);
						*/
						
						//newly added, by setting constraint's velocity as prescribed
						if(ci->constraint_node_list[j]->mapped_node->flag_constraint_type == 1)
						{
							c.segment<3>(3*j) = (ci->constraint_node_list[j]->mapped_node->prescribed_position - ci->constraint_node_list[j]->mapped_node->prescribed_preposition) / time_step_size;
							
							//c.segment<3>(3*j) = (ci->constraint_node_list[0]->mapped_node->prescribed_position - ci->constraint_node_list[0]->static_position) / time_step_size;
							//c.segment<3>(3*j) = ci->constraint_node_list[j]->mapped_node->prescribed_linear_velocity;
							
							P_skew.push_back(Matrix3d::Zero());
							I = Matrix3d::Identity();
						}
						else
						{
							c.segment<3>(3*j) = ci->constraint_node_list[j]->mapped_node->prescribed_angular_velocity;
							P_skew.push_back(Matrix3d::Identity());
							I = Matrix3d::Zero();

							R4constraint = ci->constraint_node_list[j]->mapped_node->prescribed_rotation * ci->constraint_node_list[j]->mapped_node->prescribed_prerotation.transpose();
						}
						
					}
					//cout << "one turn:" << endl;
					//cout << c << endl;

					MatrixXd matM(6+3*ci->constraint_node_list.size(), 6+3*ci->constraint_node_list.size());
					matM.setZero();
					Matrix6d i66 = Matrix6d::Identity();
					//if(ci->flag_constrained)
					//	ci->inertial_penalty = 0.0;
					matM.block<6, 6>(0, 0) = ci->velocity_matrix.transpose() * ci->velocity_matrix + ci->inertial_penalty * i66;
					
					int k = 0;
					for(; k < ci->constraint_node_list.size(); k++)
					{
						matM.block<3, 3>(6 + k*3, 0) = P_skew[k].transpose();
						//matM.block<3, 3>(6 + k*3, 3) = Matrix3d::Identity();
						matM.block<3, 3>(6 + k*3, 3) = I;
						matM.block<3, 3>(0, 6 + k*3) = P_skew[k];
						//matM.block<3, 3>(3, 6 + k*3) = Matrix3d::Identity();
						matM.block<3, 3>(3, 6 + k*3) = I;
					}

					VectorXd d(6 + 3*ci->constraint_node_list.size());
					d << (ci->velocity_matrix.transpose() * diff_vector / time_step_size), c; 

					VectorXd x(6 + 3*ci->constraint_node_list.size());
					x = matM.fullPivLu().solve(d);

					ci->angular_velocity(0) = x(0);
					ci->angular_velocity(1) = x(1);
					ci->angular_velocity(2) = x(2);

					ci->linear_velocity(0) = x(3);
					ci->linear_velocity(1) = x(4);
					ci->linear_velocity(2) = x(5);

					if(ci->constraint_node_list[0]->mapped_node->flag_constraint_type == 1)
					{
						ci->angular_velocity = Vector3d::Zero();
						ci->linear_velocity(0) = c(0);
						ci->linear_velocity(1) = c(1);
						ci->linear_velocity(2) = c(2);
					}
					//else
					//{
					//	ci->linear_velocity = Vector3d::Zero();
					//	ci->angular_velocity = Vector3d::Zero();
					//}
				}
				
				//cout << "level " << n << "constrained node "<< endl;
			}
		
			Vector3d delta_linear_displacement = ci->linear_velocity * time_step_size;
			Matrix3d delta_angular_displacement = Matrix3d::Zero();

			double angle = ci->angular_velocity.norm()*time_step_size;
			
			if (angle > 1e-3)
			{
				Matrix3d s;
				Vector3d axis_angle = ci->angular_velocity;
				axis_angle.normalize();
				skew_symmetric(axis_angle, s);
				delta_angular_displacement =  s*sin(angle) + s*s*(1.0 - cos(angle));
			}

			if(ci->flag_constrained && ci->constraint_node_list.size() > 1)
				delta_angular_displacement = R4constraint - Matrix3d::Identity();

			for (int i=0; i<ci->num_node; ++i)
			{
				//computation based on original position
				ci->node_list[i].static_position = ci->angular_displacement * ci->node_list[i].mapped_node->coordinate + ci->linear_displacement;
				ci->node_list[i].static_position += delta_linear_displacement + delta_angular_displacement*ci->node_list[i].static_position;
			}
			//save rotation information into this cluster
			ci->angular_displacement = (delta_angular_displacement + Matrix3d::Identity()) * ci->angular_displacement;
			ci->linear_displacement = (delta_angular_displacement + Matrix3d::Identity()) * ci->linear_displacement + delta_linear_displacement;

			//ci->angular_displacement = delta_angular_displacement * ci->angular_displacement;
			//ci->linear_displacement = delta_angular_displacement * ci->linear_displacement + delta_linear_displacement;
		}
		// average at intersection and update target
		for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
		{
			ni->target_position = Vector3d::Zero();

			if (!ni->flag_anchor_node)
			{
				vector<DuplicatedNode*>::iterator dn;
				dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					//if(ni->flag_constraint_node)
					//{
					//	(*dn)->static_position = ni->prescribed_position;
					//}
					ni->target_position += (*dn)->static_position;				
					//cout << endl;
				}
				
				ni->target_position /= double(ni->duplicates.size());

				if(ni->flag_constraint_node)
				{
					//ni->target_position = ni->prescribed_position;
					ni->prescribed_preposition = ni->prescribed_position;
					ni->prescribed_prerotation = ni->prescribed_rotation;
				}

				dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					(*dn)->target_position = ni->target_position;
				}
			}
			else 
			{
				ni->target_position = ni->coordinate;

				vector<DuplicatedNode*>::iterator dn;
				dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					(*dn)->target_position = ni->target_position;
				}
			}
		}
	}	
	//}

	
	//level 0 update by the last level
	if(level_list.size() > 1)
	{
		int size_c = level_list[0]->voxmesh_level->cluster_list.size();
		for(int i_c = 0; i_c < size_c; i_c ++)
		{
			int count = 0;
			Matrix3d R_all = Matrix3d::Zero();
			Vector3d T_all = Vector3d::Zero();
			updateFromLeaf(&level_list[0]->voxmesh_level->cluster_list[i_c], R_all, T_all, count);
			level_list[0]->voxmesh_level->cluster_list[i_c].angular_displacement = R_all / count;
			level_list[0]->voxmesh_level->cluster_list[i_c].linear_displacement = T_all / count;
			//update static position and target position for this cluster
			Cluster * pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];
			if(pcluster->flag_constrained)
				continue; 
			for (int i_n=0; i_n < pcluster->num_node; ++i_n)
			{
				//computation based on original position
				pcluster->node_list[i_n].static_position = pcluster->angular_displacement * pcluster->node_list[i_n].mapped_node->coordinate + pcluster->linear_displacement;
			}
		}
		// average at intersection and update target
		for (node_iterator ni=level_list[0]->voxmesh_level->node_list.begin(); ni!=level_list[0]->voxmesh_level->node_list.end(); ++ni)
		{
			ni->target_position = Vector3d::Zero();
			vector<DuplicatedNode*>::iterator dn;

			if (!ni->flag_anchor_node)
			{
				dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					ni->target_position += (*dn)->static_position;
				}

				ni->target_position /= double(ni->duplicates.size());

				dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					(*dn)->target_position = ni->target_position;
				}
			}
			else 
			{
				ni->target_position = ni->coordinate;

				dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					(*dn)->target_position = ni->target_position;
				}
			}
		}
	}
	
	++time_step_index;
	return true;
}

bool Kernel::simulateNextStep4HSMForce4Iteration()
{
	/////////////////////////////////////////////////////////////////////////////////
	//test whether quaternion is the same as rotation matrix.
	//Matrix3d r1 = level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].r;
	//Quaterniond test1(r1);
	//Matrix3d r2 = test1.toRotationMatrix();
	////////////////////////////////////////////////////////////////////////////////
	//test whether slerp could generate a reasonable result
	//double PI = 3.14159265;
	//Matrix3d r1 = Matrix3d::Identity();
	//r1(1, 1) = cos(0 * PI / 180);
	//r1(1, 2) = (-1) * sin(0 * PI / 180);
	//r1(2, 1) = sin(0 * PI / 180);
	//r1(2, 2) = cos(0 * PI / 180);

	//Matrix3d r2 = Matrix3d::Identity();
	//r2(1, 1) = cos(90 * PI / 180);
	//r2(1, 2) = (-1) * sin(90 * PI / 180);
	//r2(2, 1) = sin(90 * PI / 180);
	//r2(2, 2) = cos(90 * PI / 180);

	//Quaterniond test1(r1);
	//Quaterniond test2(r2);
	//Quaterniond test3 = test1.slerp(0.7, test2);
	//Matrix3d r3 = test3.toRotationMatrix();

	//// to save energy
	//flag_setForce = true;
	//const_force(0) = 0.15;
	//const_force(1) = 0.5;
	//const_force(2) = 0.0;
	

	double mass_sum = 0.0;
	num_PE_perTimeStep = 0;
	Vector3d force_gravity = Vector3d::Zero();
	Vector3d force_wind = Vector3d::Zero();
	//set gravity
	if(flag_gravity)
	{
		force_gravity(1) = gravity_magnitude;
	}
	if(flag_setWindForce)
	{
		srand(time(NULL));
		//wind_magnitude = (rand() % 100) * 0.01;
		wind_magnitude = 1;
		force_wind = const_force * sin((time_step_index % 360) * PI / 180) * wind_magnitude;
	}


	double PI = 3.14159265;
	int idx_bottom = level_list.size() - 1;
	double num_timestep = 0.0;
	for (int n = 0; n < level_list.size(); n++)
	{
		//position constraint
		if(flag_setForce && constraintType != Kernel::FORCE_CONSTRAINT)
		{
			if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
			{
				int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					level_list[n]->voxmesh_level->constraint_node_list[k]->prescribed_position = const_force;
				}
			}
		}
		//force constraint
		if(flag_setForce && constraintType == Kernel::FORCE_CONSTRAINT)
		{
			if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
			{
				int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					level_list[n]->voxmesh_level->constraint_node_list[k]->force = const_force;

					for(int i=0; i < level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates.size(); ++i)
					{
						level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates[i]->force = const_force;
					}
				}
			}
		}
		time_counter->StartCounter();
		cout << "level " << n << endl;
		//if level > 0, needs to inherit R, T from parent_level
		if(n > 0)
		{
			level_list[n]->voxmesh_level->old_energy = 0.0;
			vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
			for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
			{
				///////////////////////////////////////////////////////////////////////////////////////
				////compute current position
				//ci->computeCurrentMassCentroid4Target();
				
				////////////////////////////////////////////////////////////////////////////////////////
				//slerp to interpolate rotation q_c
				//linear interpolate translation t_ci(which is not used, so deleted)
				////////////////////////////////////////////////////////////////////////////////////////
				Quaterniond q_ci;
				vector<Quaterniond> list_q;
				vector<Vector3d> list_v;
				vector<double> t;
				double t_add = 0.0;
				Vector3d t_ci = Vector3d::Zero();

				for(int p = 0; p < 8; p++)
				{
					if(ci->vox_list[0]->list_near_parentVox[p])
					{
						Cluster * p_temp = &level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid];

						Quaterniond temp(p_temp->r);
						list_q.push_back(temp);
						
						p_temp->computeCurrentMassCentroid4Target();
						list_v.push_back(p_temp->r*(ci->original_center - p_temp->original_center) + p_temp->current_center);
						

						t.push_back(ci->vox_list[0]->para_interpolate[p]);
						
						ci->r = level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].r;
						t_ci = p_temp->r*(ci->original_center - p_temp->original_center) + p_temp->current_center;
					}
				}
				if(list_q.size() > 1)
				{
					q_ci = list_q[0];
					
					t_ci = t[0] * list_v[0];
					
					while(list_q.size() > 1)
					{
						t_add += t[0];
						double temp = t_add / (t_add + t[1]);
						Quaterniond qtemp = list_q[1].slerp(temp, q_ci);
						q_ci = qtemp;
						t_ci += t[1] * list_v[1];

						list_q.erase(list_q.begin());
						t.erase(t.begin());
						list_v.erase(list_v.begin());
					}
					ci->r = q_ci.toRotationMatrix();
				}


				double det_a = det33(ci->a);
				ci->current_center = t_ci;
				vector<DuplicatedNode>::iterator dni;
				for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
				{
					Vector3d _force = dni->force + force_gravity + force_wind;;
					dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center; // + t_ci;//ci->current_center;

					level_list[n]->voxmesh_level->old_energy += pow((dni->static_position(0) - dni->target_position(0)), 2);
					level_list[n]->voxmesh_level->old_energy += pow((dni->static_position(1) - dni->target_position(1)), 2);
					level_list[n]->voxmesh_level->old_energy += pow((dni->static_position(2) - dni->target_position(2)), 2);
					if (!dni->mapped_node->flag_anchor_node)
					{
						dni->target_position = dni->static_position;// + time_step_size * time_step_size * _force * force_scalar;
					}
				}
			}
			time_counter->StopCounter();
			//cout << time_counter->GetElapsedTime() << endl;
			time_counter->StartCounter();
			//cout << "next!! " << level_list[n]->voxmesh_level->old_energy << endl;

			// updating target position
			for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
			{
				ni->target_position.setZero();

				vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					ni->target_position += (*dn)->target_position;
				}
				
				ni->target_position /= double(ni->duplicates.size());
				if(constraintType != Kernel::FORCE_CONSTRAINT && ni->flag_constraint_node)
					ni->target_position = ni->prescribed_position;

				dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					(*dn)->target_position = ni->target_position;
				}
			}//for
		}
		time_counter->StopCounter();
		//cout << time_counter->GetElapsedTime() << endl;

		//else//if n == 0
		{
			//////////////////////////////////////////////////////////////////////////////////
			// level 0 to bottom+1, directly implementing shape matching
			//////////////////////////////////////////////////////////////////////////////////
			int iteration = level_list[n]->times_ShapeMatching;
			if(n < level_list.size())
			{
				int count = 0;
				for( int i = 0; i < iteration; i ++)
				//while(true)
				{
					time_counter->StartCounter();
					level_list[n]->voxmesh_level->new_energy = 0.0;
					//level_list[n]->voxmesh_level->old_energy = 0.0;
					double tempEnergy = 0.0;
					double tempEnergy2 = 0.0;
					vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
					for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
					{
						vector<DuplicatedNode>::const_iterator const_ni;

						ci->a_pq = Matrix3d::Zero();
						Vector3d p, q;
						ci->computeCurrentMassCentroid4Target();

						for(const_ni=ci->node_list.begin(); const_ni!=ci->node_list.end(); ++const_ni)
						{
							p = const_ni->target_position - ci->current_center;
							q = const_ni->coordinate - ci->original_center;
							ci->a_pq += const_ni->mass * p * q.transpose();
						}


						ci->a = ci->a_pq * ci->a_qq;
		

						JacobiSVD<Matrix3d> jacobi(ci->a_pq, ComputeFullU|ComputeFullV);
						ci->r = jacobi.matrixU() * jacobi.matrixV().transpose();
		
						num_PE_perTimeStep++;

						double det_a = det33(ci->a);

						vector<DuplicatedNode>::iterator dni;
						for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
						{
							tempEnergy2 += pow( dni->static_position(0) - dni->target_position(0), 2);
							tempEnergy2 += pow( dni->static_position(1) - dni->target_position(1), 2);
							tempEnergy2 += pow( dni->static_position(2) - dni->target_position(2), 2);
						
							dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center;

							level_list[n]->voxmesh_level->new_energy += pow( dni->static_position(0) - dni->target_position(0), 2);
							level_list[n]->voxmesh_level->new_energy += pow( dni->static_position(1) - dni->target_position(1), 2);
							level_list[n]->voxmesh_level->new_energy += pow( dni->static_position(2) - dni->target_position(2), 2);
							Vector3d target_position_temp;
							target_position_temp = dni->coordinate;
							if (!dni->mapped_node->flag_anchor_node)
							{
								Vector3d _force = dni->force + force_gravity + force_wind;

								if (n == idx_bottom)
								{
									if(flag_dynamics && i == (iteration - 1))
									{
										target_position_temp = dni->target_position;
										dni->velocity = (1.0-ci->kappa)*dni->velocity + ci->alpha*(dni->static_position - dni->coordinate - dni->displacement) / ((num_timestep + 1 )* time_step_size)
											+ time_step_size*_force*force_scalar/(num_timestep + 1);
										dni->displacement += (num_timestep + 1)*time_step_size*dni->velocity;

										dni->target_position = dni->coordinate + dni->displacement;
										if(_force != Vector3d::Zero())
											target_position_temp = dni->target_position;
									}
									else
									{
										target_position_temp = dni->target_position;
										dni->target_position = dni->static_position + time_step_size * time_step_size * _force * force_scalar;
										dni->displacement = dni->target_position - dni->coordinate;
										if(_force != Vector3d::Zero())
											target_position_temp = dni->target_position;
									}
								}
								else
								{
									target_position_temp = dni->target_position;
									dni->target_position = dni->static_position + time_step_size * time_step_size * _force * force_scalar;
									if(_force != Vector3d::Zero())
										target_position_temp = dni->target_position;
								}
							}
							tempEnergy += pow( dni->static_position(0) - target_position_temp(0), 2);
							tempEnergy += pow( dni->static_position(1) - target_position_temp(1), 2);
							tempEnergy += pow( dni->static_position(2) - target_position_temp(2), 2);
						}

					}//for
					//cout << "before sm: " <<  tempEnergy2 << endl;
					//cout << "last step energy" << level_list[n]->voxmesh_level->old_energy << endl;
					//cout << "after sm:" << level_list[n]->voxmesh_level->new_energy << endl;
					//cout << "force: " << tempEnergy << endl;
					time_counter->StopCounter();
					//cout << time_counter->GetElapsedTime() << endl;
					time_counter->StartCounter();

					VoxMesh * pVM = level_list[n]->voxmesh_level;
					double threshold = 0.0;
					double ratio = 0.0;
					double speed = 0.0;
					ratio = getEnergyRatio(pVM, speed);
					//cout << pVM->num_pair << endl;
					//threshold = pVM->new_energy - pVM->old_energy;
					myEnergy << ratio << endl;
					//cout << count << endl;
					//cout << ratio << endl;
					cout << speed << endl;
					pVM->old_energy = tempEnergy;

					// Averaging
					//level_list[n]->voxmesh_level->new_energy = 0.0;
					for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
					{
						if (n == idx_bottom)
						{
							if(flag_dynamics)
							{
								ni->displacement.setZero();
								//ni->velocity.setZero();
							}
						}
						ni->target_position.setZero();

						vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							if (n == idx_bottom)
							{
								if(flag_dynamics)
								{
									ni->displacement += (*dn)->displacement;
									//ni->velocity += (*dn)->velocity;
								}
							}

							ni->target_position += (*dn)->target_position;
						}

						ni->target_position /= double(ni->duplicates.size());
						if( constraintType != Kernel::FORCE_CONSTRAINT && ni->flag_constraint_node)
							ni->target_position = ni->prescribed_position;

						if (n == idx_bottom)
						{
							if(flag_dynamics)
							{
								ni->displacement /= double(ni->duplicates.size());
								//ni->velocity /= double(ni->duplicates.size());
							}
							else
							{
								ni->displacement = ni->target_position - ni->coordinate;
							}
						}

						dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							(*dn)->target_position = ni->target_position;

							if (n == idx_bottom)
							{
								if(flag_dynamics)
								{
									(*dn)->displacement = ni->displacement;
									//(*dn)->velocity = ni->velocity;
								}			
								else
								{
									(*dn)->displacement = ni->displacement;
								}
							}

							//level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
							//level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
							//level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
						}
					}//update for
					//cout << "avg" << level_list[n]->voxmesh_level->new_energy << endl;
					time_counter->StopCounter();
					//cout << time_counter->GetElapsedTime() << endl;
					count ++;
					
					//if (speed < energyThreshold && i < iteration - 1 && n == idx_bottom)
					//{
					//	num_timestep++;
					//	i = iteration - 2;
					//	cout << "stop~!" << endl;
					//	continue;
					//}
					if (speed < energyThreshold && n < idx_bottom)
					{
						num_timestep++;
						cout << "stop-!"<< endl;
						break;
					}
					
					num_timestep++;
				}//iteration
				testoutput << n <<": " << count << endl;
			}//if(n < size)
			////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}//else

	}
	///////////////////////////////////////////////////////////////////////////
	//level 0 update by the last level, multigrid
	///////////////////////////////////////////////////////////////////////////
	if(level_list.size() > 1 && flag_multigrid)
	{

		///////////////////////////////////////////////////////////////////////////////////////
		//compute current position
		//vector<Cluster>::iterator ci = level_list[idx_bottom]->voxmesh_level->cluster_list.begin();
		//for (; ci != level_list[idx_bottom]->voxmesh_level->cluster_list.end(); ++ci)
		//{
		//	ci->computeCurrentMassCentroid();
		//}
		///////////////////////////////////////////////////////////////////////////////////////

		time_counter->StartCounter();
		int size_c = level_list[0]->voxmesh_level->cluster_list.size();
		level_list[0]->voxmesh_level->old_energy = 0.0;
		for(int i_c = 0; i_c < size_c; i_c ++)
		{
			Cluster * m_pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];
			Vector3d translation = Vector3d::Zero();
			int sizeLeaf = m_pcluster->leaf_list.size() ;

			if (sizeLeaf > 1)
			{
				Quaterniond q(m_pcluster->leaf_list[0]->r);
				double t_add = 0.0;
				int count = 1;
				m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
				Vector3d t = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
				translation = (m_pcluster->leaf_parameter[0] / m_pcluster->term_normlize) * t;
				while(count < sizeLeaf)
				{
					m_pcluster->leaf_list[count]->computeCurrentMassCentroid4Target();
					Quaterniond q2(m_pcluster->leaf_list[count]->r);
					t_add += (m_pcluster->leaf_parameter[count-1] / m_pcluster->term_normlize);
					double temp = t_add / (t_add + (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize));
					Quaterniond qtemp = q2.slerp(temp, q);
					q = qtemp;

					t = m_pcluster->leaf_list[count]->current_center + m_pcluster->leaf_list[count]->r * (m_pcluster->original_center - m_pcluster->leaf_list[count]->original_center);
					translation += (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize) * t;

					count ++;
				}
				level_list[0]->voxmesh_level->cluster_list[i_c].r = q.toRotationMatrix();
			}
			else
			{
				m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
				level_list[0]->voxmesh_level->cluster_list[i_c].r = m_pcluster->leaf_list[0]->r;
				translation = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
			}


			//update static position and target position for this cluster
			//Cluster * pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];

			double det_a = det33(m_pcluster->a);
			m_pcluster->current_center = translation;
			vector<DuplicatedNode>::iterator dni;
			for (dni=m_pcluster->node_list.begin(); dni!=m_pcluster->node_list.end(); ++dni)
			{
				////dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*dni->coordinate + translation;
				dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*(dni->coordinate - m_pcluster->original_center) + m_pcluster->current_center;

				level_list[0]->voxmesh_level->old_energy += pow((dni->static_position(0) - dni->target_position(0)), 2);
				level_list[0]->voxmesh_level->old_energy += pow((dni->static_position(1) - dni->target_position(1)), 2);
				level_list[0]->voxmesh_level->old_energy += pow((dni->static_position(2) - dni->target_position(2)), 2);

				if (!dni->mapped_node->flag_anchor_node)
				{
					Vector3d _force = dni->force + force_gravity + force_wind;
					dni->target_position = dni->static_position;// + time_step_size * time_step_size * _force * force_scalar;
				}
			}
		}

		//cout << "feedback energy" << level_list[0]->voxmesh_level->old_energy << endl; 

		time_counter->StopCounter();
		//cout << time_counter->GetElapsedTime() << endl;
		time_counter->StartCounter();
		// updating displacement
		for (node_iterator ni=level_list[0]->voxmesh_level->node_list.begin(); ni!=level_list[0]->voxmesh_level->node_list.end(); ++ni)
		{
			//ni->static_position.setZero();
			ni->target_position.setZero();

			vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				//ni->static_position += (*dn)->static_position;
				ni->target_position += (*dn)->target_position;
			}

			//ni->static_position /= double(ni->duplicates.size());
			ni->target_position /= double(ni->duplicates.size());
			if(constraintType != FORCE_CONSTRAINT && ni->flag_constraint_node)
				ni->target_position = ni->prescribed_position;

			dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				//if(!ni->flag_anchor_node)
					(*dn)->target_position = ni->target_position;

				//else
					//(*dn)->target_position = ni->coordinate;
			}
		}//cluster for
	}

	time_counter->StopCounter();
	//cout << time_counter->GetElapsedTime() << endl;
	time_counter->StartCounter();
	int num_last = level_list.size()-1;
	for (node_iterator nmi=p_mesh->node_list.begin(); nmi!=p_mesh->node_list.end(); ++nmi)
	{
		nmi->displacement.setZero();
		for(int i = 0; i < 8; i++)
		{
			nmi->displacement += nmi->list_interpolation_nodes[i][num_last]->displacement * nmi->para_interpolate[i][num_last];
		}
	}

	time_counter->StopCounter();
	//cout << time_counter->GetElapsedTime() << endl;
	//cout << endl;
	cout << endl;

	//networking
	if(flag_network_ready && network_role == NETWORK_ROLE_SERVER)
	{
		for(int i = 0; i < pTransmitter.size(); i++)
		{
			if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_PC && pTransmitter[i]->isClientReady)
				pTransmitter[i]->HSMupdate(data4PC);
			else if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_MOBILE)
				pTransmitter[i]->HSMupdate(data4Mobile);
		}
		
	}
	////////////////////////////////////////////////////////////////////////////////////////////


	if(flag_setForce && !flag_setWindForce)
	{
		/*
		// compute the energy
		double match_energy = 0.0;
		for (int i=0; i< level_list[idx_bottom]->voxmesh_level->num_cluster; ++i)
		{
			for (int j=0; j<level_list[idx_bottom]->voxmesh_level->cluster_list[i].num_node; ++j)
			{
				match_energy += (level_list[idx_bottom]->voxmesh_level->cluster_list[i].node_list[j].static_position - level_list[idx_bottom]->voxmesh_level->cluster_list[i].node_list[j].target_position).norm();
			}
		}
		level_list[idx_bottom]->voxmesh_level->old_energy = level_list[idx_bottom]->voxmesh_level->new_energy;
		level_list[idx_bottom]->voxmesh_level->new_energy = match_energy;
		if(level_list[idx_bottom]->voxmesh_level->old_energy != 0)
		{
			double threshold = (level_list[idx_bottom]->voxmesh_level->new_energy - level_list[idx_bottom]->voxmesh_level->old_energy)/ level_list[idx_bottom]->voxmesh_level->old_energy;
			if(threshold < 0 && abs(threshold) < 0.01 && !flag_converge)
			{
				timestep_converge = time_step_index;
				flag_converge = true;
				cout << "TimeStep:" << time_step_index << " Converge!" << endl;
				cout << "Begin TimeStep: " << timestep_begin << endl;
				cout << "Converge TimeStep: " << timestep_converge << endl;
				cout << "Iteration #: " << timestep_converge - timestep_begin << endl;
				cout << "PE # per Iteration:" << num_PE_perTimeStep << endl;
			}
			myHEnergy << match_energy << endl;
		}
		*/
	}
	if(flag_exportTxt)
	{
		vector<Node>::iterator ni;
		for (ni = p_mesh->node_list.begin(); ni != p_mesh->node_list.end(); ++ni)
		{
			testoutput << "v " << ni->coordinate(0) + ni->displacement(0) 
				<< " " << ni->coordinate(1) + ni->displacement(1) 
				<< " " << ni->coordinate(2) + ni->displacement(2) << endl;
		}
	}
	++time_step_index;
	return true;
}


bool Kernel::simulateNextStep4HSMOneStep()
{
	double mass_sum = 0.0;
	num_PE_perTimeStep = 0;
	Vector3d force_gravity = Vector3d::Zero();
	Vector3d force_wind = Vector3d::Zero();
	//set gravity
	if(flag_gravity)
	{
		force_gravity(1) = gravity_magnitude;
	}
	if(flag_setWindForce)
	{
		srand(time(NULL));
		//wind_magnitude = (rand() % 100) * 0.01;
		wind_magnitude = 1;
		force_wind = const_force * sin((time_step_index % 360) * PI / 180) * wind_magnitude;
	}

	time_counter->StartCounter();
	double PI = 3.14159265;
	int idx_bottom = level_list.size() - 1;
	int n = level_display;
	double ratio = 0.0;
	//for (int n = 0; n < level_list.size(); n++)
	//{
		//force constraint
		if(flag_setForce && constraintType == Kernel::FORCE_CONSTRAINT)
		{
			if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
			{
				int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					level_list[n]->voxmesh_level->constraint_node_list[k]->force = const_force;

					for(int i=0; i < level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates.size(); ++i)
					{
						level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates[i]->force = const_force;
					}
				}
			}
		}
		//position constraint
		if(flag_setForce && constraintType != Kernel::FORCE_CONSTRAINT)
		{
			if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
			{
				int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					level_list[n]->voxmesh_level->constraint_node_list[k]->prescribed_position = level_list[n]->voxmesh_level->constraint_node_list[k]->coordinate + const_force;
				}
			}
		}

		//else//if n == 0
		{
			//////////////////////////////////////////////////////////////////////////////////
			// level 0 to bottom+1, directly implementing shape matching
			//////////////////////////////////////////////////////////////////////////////////
			int iteration = level_list[n]->times_ShapeMatching;
			//if (n == idx_bottom)
				iteration = 1;
			if(n < level_list.size())
			{
				for( int i = 0; i < iteration; i ++)
				{
					vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
					for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
					{
						vector<DuplicatedNode>::const_iterator const_ni;

						ci->a_pq = Matrix3d::Zero();
						Vector3d p, q;
						ci->computeCurrentMassCentroid4Target();

						for(const_ni=ci->node_list.begin(); const_ni!=ci->node_list.end(); ++const_ni)
						{
							p = const_ni->target_position - ci->current_center;
							q = const_ni->coordinate - ci->original_center;
							ci->a_pq += const_ni->mass * p * q.transpose();
						}


						ci->a = ci->a_pq * ci->a_qq;


						JacobiSVD<Matrix3d> jacobi(ci->a_pq, ComputeFullU|ComputeFullV);
						ci->r = jacobi.matrixU() * jacobi.matrixV().transpose();

						num_PE_perTimeStep++;

						double det_a = det33(ci->a);

						vector<DuplicatedNode>::iterator dni;
						for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
						{
							dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center;

							if (!dni->mapped_node->flag_anchor_node)
							{
								Vector3d _force = dni->force + force_gravity + force_wind;

								if (n == idx_bottom)
								{
									if(flag_dynamics)
									{
										dni->velocity = (1.0-ci->kappa)*dni->velocity + ci->alpha*(dni->static_position - dni->coordinate - dni->displacement) / time_step_size
											+ time_step_size*_force*force_scalar;
										dni->displacement += time_step_size*dni->velocity;
										dni->target_position = dni->coordinate + dni->displacement;
									}
									else
									{
										dni->target_position = dni->static_position + time_step_size * time_step_size * _force * force_scalar;
										dni->displacement = dni->target_position - dni->coordinate;
									}
								}
								else
									dni->target_position = dni->static_position + time_step_size * time_step_size * _force * force_scalar;
							}
						}

					}//for
					//level_list[n]->voxmesh_level->new_energy = 0.0;
					// updating displacement
					for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
					{
						if (n == idx_bottom)
						{
							if(flag_dynamics)
							{
								ni->displacement.setZero();
								ni->velocity.setZero();
							}
						}
						ni->target_position.setZero();

						vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							if (n == idx_bottom)
							{
								if(flag_dynamics)
								{
									ni->displacement += (*dn)->displacement;
									ni->velocity += (*dn)->velocity;
								}
							}

							ni->target_position += (*dn)->target_position;
						}

						ni->target_position /= double(ni->duplicates.size());
						/*
						if( constraintType != Kernel::FORCE_CONSTRAINT && ni->flag_constraint_node)
							ni->target_position = ni->prescribed_position;
						*/
						if (n == idx_bottom)
						{
							if(flag_dynamics)
							{
								ni->displacement /= double(ni->duplicates.size());
								ni->velocity /= double(ni->duplicates.size());
							}
							else
							{
								ni->displacement = ni->target_position - ni->coordinate;
							}
						}

						dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							(*dn)->target_position = ni->target_position;

							if (n == idx_bottom)
							{
								if(flag_dynamics)
								{
									(*dn)->displacement = ni->displacement;
									(*dn)->velocity = ni->velocity;
								}			
								else
								{
									(*dn)->displacement = ni->displacement;
								}
							}

							//level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
							//level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
							//level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);

						}
						
					}//update for
				}
			}//
			////////////////////////////////////////////////////////////////////////////////////////////////////////////


		}//else
		
		for (node_iterator nmi=p_mesh->node_list.begin(); nmi!=p_mesh->node_list.end(); ++nmi)
		{
			nmi->displacement.setZero();
			for(int i = 0; i < 8; i++)
			{
				//nmi->displacement += nmi->list_interpolation_nodes[i][level_display]->displacement * nmi->para_interpolate[i][level_display];
				nmi->displacement += (nmi->list_interpolation_nodes[i][level_display]->target_position - nmi->list_interpolation_nodes[i][level_display]->coordinate) * nmi->para_interpolate[i][level_display];
				//nmi->displacement += nmi->list_interpolation_nodes[i][0]->displacement * nmi->para_interpolate[i][0];
				//nmi->target_position +=  nmi->list_interpolation_nodes[i]->displacement * nmi->para_interpolate[i];
			}
			//nmi->displacement = nmi->target_position - nmi->coordinate;
		}
		
		// updating displacement
		level_list[n]->voxmesh_level->new_energy = 0.0;
		for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
		{
			if( constraintType != Kernel::FORCE_CONSTRAINT && ni->flag_constraint_node)
				ni->target_position = ni->prescribed_position;
			vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin(); 
			dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				(*dn)->target_position = ni->target_position;

				if (n == idx_bottom)
				{
					if(flag_dynamics)
					{
						(*dn)->displacement = ni->displacement;
						(*dn)->velocity = ni->velocity;
					}			
					else
					{
						(*dn)->displacement = ni->displacement;
					}
				}

				level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
				level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
				level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);

			}
		}//update for
		
		VoxMesh * pVM = level_list[n]->voxmesh_level;
		double threshold = 0.0;
		double speed;
		ratio = getEnergyRatio(pVM, speed);
		threshold = abs(pVM->new_energy - pVM->old_energy);
		if(flag_setForce && !flag_setWindForce)
		{
			//cout << ratio << endl;
			//if (!flag_converge)
			//{
			//	myEnergy << ratio << endl;
			//	myHEnergy << pVM->new_energy << endl;
			//}
			//cout << pVM->new_energy << endl;
			//if (pVM->old_energy >= pVM->new_energy && threshold < 0.0001 && !flag_converge)
			//{
			//	timestep_converge = time_step_index;
			//	flag_converge = true;
			//	cout << "Iteration #: " << timestep_converge - timestep_begin << endl;
			//	cout << "converge!" << endl;
			//}
		}
		double temp = pVM->old_energy - pVM->new_energy;
		pVM->old_energy = pVM->new_energy;
		//cout << ratio << endl;
		cout << pVM->new_energy << endl;
		myEnergy << pVM->new_energy << endl;
		
		//if level > 0, needs to inherit R, T from parent_level
		if(n < level_list.size() - 1 && ratio < energyThreshold && temp > 0.0)
		{
			cout << "next!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			n += 1;
			level_display += 1;
			//position constraint
			if(flag_setForce && constraintType != Kernel::FORCE_CONSTRAINT)
			{
				if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
				{
					int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
					for(int k = 0; k < size_k; k ++)
					{
						level_list[n]->voxmesh_level->constraint_node_list[k]->prescribed_position = level_list[n]->voxmesh_level->constraint_node_list[k]->coordinate + const_force;
					}
				}
			}
			vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
			for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
			{
				///////////////////////////////////////////////////////////////////////////////////////
				////compute current position
				//ci->computeCurrentMassCentroid4Target();
				////////////////////////////////////////////////////////////////////////////////////////
				//slerp to interpolate rotation q_c
				//linear interpolate translation t_ci(which is not used, so deleted)
				////////////////////////////////////////////////////////////////////////////////////////
				Quaterniond q_ci;
				vector<Quaterniond> list_q;
				vector<Vector3d> list_v;
				vector<double> t;
				double t_add = 0.0;
				Vector3d t_ci = Vector3d::Zero();

				for(int p = 0; p < 8; p++)
				{
					if(ci->vox_list[0]->list_near_parentVox[p])
					{
						Cluster * p_temp = &level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid];

						Quaterniond temp(p_temp->r);
						//cout << temp.w() << " " << temp.x() << " " << temp.y() << " " << temp.z() << endl;
						list_q.push_back(temp);
						
						//p_temp->computeCurrentMassCentroid4Target();
						//p_temp->computeCurrentMassCentroid4Static();
						//list_v.push_back((p_temp->current_center - p_temp->r * p_temp->original_center));
						list_v.push_back(p_temp->r*(ci->original_center - p_temp->original_center) + p_temp->current_center);
						

						t.push_back(ci->vox_list[0]->para_interpolate[p]);
						
						ci->r = level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].r;
						//t_ci = p_temp->current_center - p_temp->r * p_temp->original_center;
						t_ci = p_temp->r*(ci->original_center - p_temp->original_center) + p_temp->current_center;
					}
				}
				if(list_q.size() > 1)
				{
					q_ci = list_q[0];
					
					t_ci = t[0] * list_v[0];
					
					while(list_q.size() > 1)
					{
						t_add += t[0];
						double temp = t_add / (t_add + t[1]);
						Quaterniond qtemp = list_q[1].slerp(temp, q_ci);
						q_ci = qtemp;
						t_ci += t[1] * list_v[1];

						list_q.erase(list_q.begin());
						t.erase(t.begin());
						list_v.erase(list_v.begin());
					}
					ci->r = q_ci.toRotationMatrix();
				}


				double det_a = det33(ci->a);
				//ci->current_center = ci->parent_cluster->r * (ci->original_center - ci->parent_cluster->original_center) + ci->parent_cluster->current_center;
				ci->current_center = t_ci;
				vector<DuplicatedNode>::iterator dni;
				for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
				{
					Vector3d _force = dni->force + force_gravity + force_wind;;

					//dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)* dni->coordinate + t_ci;//ci->current_center;
					//dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->parent_cluster->original_center) + ci->parent_cluster->original_center + t_ci;
					
					dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center; // + t_ci;//ci->current_center;
					if (!dni->mapped_node->flag_anchor_node)
					{
						dni->target_position = dni->static_position + time_step_size * time_step_size * _force * force_scalar;
					}
				}
				//ci->original_center = ci->current_center;
			}

			// updating target position
			level_list[n]->voxmesh_level->old_energy = 0.0;
			for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
			{
				ni->target_position.setZero();

				vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					ni->target_position += (*dn)->target_position;
				}
				
				ni->target_position /= double(ni->duplicates.size());
				if(constraintType != Kernel::FORCE_CONSTRAINT && ni->flag_constraint_node)
					ni->target_position = ni->prescribed_position;

				dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					(*dn)->target_position = ni->target_position;
					level_list[n]->voxmesh_level->old_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
					level_list[n]->voxmesh_level->old_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
					level_list[n]->voxmesh_level->old_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
				}
			}//for
			
		}//if
		
		if(flag_setForce && !flag_setWindForce)
		{
			if (!flag_converge)
			{
				time_counter->StopCounter();
				testoutput << time_counter->GetElapsedTime() << endl;
			}
		}


	///////////////////////////////////////////////////////////////////////////
	//level 0 update by the last level, multigrid
	///////////////////////////////////////////////////////////////////////////
		// && ratio < energyThreshold 
		/*
	else if(n == level_list.size() - 1)
	{
		if(level_list.size() > 1 && flag_multigrid)
		{
			///////////////////////////////////////////////////////////////////////////////////////
			//compute current position
			//vector<Cluster>::iterator ci = level_list[idx_bottom]->voxmesh_level->cluster_list.begin();
			//for (; ci != level_list[idx_bottom]->voxmesh_level->cluster_list.end(); ++ci)
			//{
			//	ci->computeCurrentMassCentroid();
			//}
			///////////////////////////////////////////////////////////////////////////////////////

			int size_c = level_list[0]->voxmesh_level->cluster_list.size();
			for(int i_c = 0; i_c < size_c; i_c ++)
			{
				Cluster * m_pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];
				Vector3d translation = Vector3d::Zero();
				int sizeLeaf = m_pcluster->leaf_list.size() ;

				if (sizeLeaf > 1)
				{
					Quaterniond q(m_pcluster->leaf_list[0]->r);
					double t_add = 0.0;
					int count = 1;
					//m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
					//m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Static();
					////Vector3d t = m_pcluster->leaf_list[0]->current_center - m_pcluster->leaf_list[0]->r * m_pcluster->leaf_list[0]->original_center;
					Vector3d t = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
					translation = (m_pcluster->leaf_parameter[0] / m_pcluster->term_normlize) * t;
					while(count < sizeLeaf)
					{
						//m_pcluster->leaf_list[count]->computeCurrentMassCentroid4Target();
						//m_pcluster->leaf_list[count]->computeCurrentMassCentroid4Static();
						Quaterniond q2(m_pcluster->leaf_list[count]->r);
						t_add += (m_pcluster->leaf_parameter[count-1] / m_pcluster->term_normlize);
						//double temp = (t_add * t_add) /(t_add * t_add + interpolation * interpolation);
						double temp = t_add / (t_add + (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize));
						Quaterniond qtemp = q2.slerp(temp, q);
						q = qtemp;

						////t =  m_pcluster->leaf_list[count]->current_center - m_pcluster->leaf_list[count]->r * m_pcluster->leaf_list[count]->original_center;
						t = m_pcluster->leaf_list[count]->current_center + m_pcluster->leaf_list[count]->r * (m_pcluster->original_center - m_pcluster->leaf_list[count]->original_center);
						translation += (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize) * t;

						count ++;
					}
					level_list[0]->voxmesh_level->cluster_list[i_c].r = q.toRotationMatrix();
				}
				else
				{
					//m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
					level_list[0]->voxmesh_level->cluster_list[i_c].r = m_pcluster->leaf_list[0]->r;
					////translation = m_pcluster->leaf_list[0]->current_center - m_pcluster->leaf_list[0]->r * m_pcluster->leaf_list[0]->original_center;
					translation = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
				}


				//update static position and target position for this cluster
				//Cluster * pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];

				double det_a = det33(m_pcluster->a);
				m_pcluster->current_center = translation;
				vector<DuplicatedNode>::iterator dni;
				for (dni=m_pcluster->node_list.begin(); dni!=m_pcluster->node_list.end(); ++dni)
				{
					////dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*dni->coordinate + translation;
					dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*(dni->coordinate - m_pcluster->original_center) + m_pcluster->current_center;
					if (!dni->mapped_node->flag_anchor_node)
					{
						Vector3d _force = dni->force + force_gravity + force_wind;
						dni->target_position = dni->static_position + time_step_size * time_step_size * _force * force_scalar;
					}
				}
			}

			// updating displacement
			for (node_iterator ni=level_list[0]->voxmesh_level->node_list.begin(); ni!=level_list[0]->voxmesh_level->node_list.end(); ++ni)
			{
				//ni->static_position.setZero();
				ni->target_position.setZero();

				vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					//ni->static_position += (*dn)->static_position;
					ni->target_position += (*dn)->target_position;
				}

				//ni->static_position /= double(ni->duplicates.size());
				ni->target_position /= double(ni->duplicates.size());
				if(constraintType != FORCE_CONSTRAINT && ni->flag_constraint_node)
					ni->target_position = ni->prescribed_position;

				dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					//if(!ni->flag_anchor_node)
						(*dn)->target_position = ni->target_position;
					//else
						//(*dn)->target_position = ni->coordinate;
				}
			}//cluster for
		}
		n = 0;
		level_display = 0;
	}
	*/


	
	/*
	time_counter->StopCounter();
	cout << time_counter->GetElapsedTime() << endl;
	cout << endl;
	cout << endl;

	//networking
	if(flag_network_ready && network_role == NETWORK_ROLE_SERVER)
	{
		for(int i = 0; i < pTransmitter.size(); i++)
		{
			if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_PC && pTransmitter[i]->isClientReady)
				pTransmitter[i]->HSMupdate(data4PC);
			else if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_MOBILE)
				pTransmitter[i]->HSMupdate(data4Mobile);
		}
		
	}
	////////////////////////////////////////////////////////////////////////////////////////////


	if(flag_setForce && !flag_setWindForce)
	{
		// compute the energy
		double match_energy = 0.0;
		for (int i=0; i< level_list[idx_bottom]->voxmesh_level->num_cluster; ++i)
		{
			for (int j=0; j<level_list[idx_bottom]->voxmesh_level->cluster_list[i].num_node; ++j)
			{
				match_energy += (level_list[idx_bottom]->voxmesh_level->cluster_list[i].node_list[j].static_position - level_list[idx_bottom]->voxmesh_level->cluster_list[i].node_list[j].target_position).norm();
			}
		}
		level_list[idx_bottom]->voxmesh_level->old_energy = level_list[idx_bottom]->voxmesh_level->new_energy;
		level_list[idx_bottom]->voxmesh_level->new_energy = match_energy;
		if(level_list[idx_bottom]->voxmesh_level->old_energy != 0)
		{
			double threshold = (level_list[idx_bottom]->voxmesh_level->new_energy - level_list[idx_bottom]->voxmesh_level->old_energy)/ level_list[idx_bottom]->voxmesh_level->old_energy;
			if(threshold < 0 && abs(threshold) < 0.01 && !flag_converge)
			{
				timestep_converge = time_step_index;
				flag_converge = true;
				cout << "TimeStep:" << time_step_index << " Converge!" << endl;
				cout << "Begin TimeStep: " << timestep_begin << endl;
				cout << "Converge TimeStep: " << timestep_converge << endl;
				cout << "Iteration #: " << timestep_converge - timestep_begin << endl;
				cout << "PE # per Iteration:" << num_PE_perTimeStep << endl;
			}
			myHEnergy << match_energy << endl;
		}
	}
	if(flag_exportTxt)
	{
		vector<Node>::iterator ni;
		for (ni = p_mesh->node_list.begin(); ni != p_mesh->node_list.end(); ++ni)
		{
			testoutput << "v " << ni->coordinate(0) + ni->displacement(0) 
				<< " " << ni->coordinate(1) + ni->displacement(1) 
				<< " " << ni->coordinate(2) + ni->displacement(2) << endl;
		}
	}
	*/
	++time_step_index;
	return true;
}

bool Kernel::simulateNextStep4HSMOriginal()
{	
	double mass_sum = 0.0;
	num_PE_perTimeStep = 0;
	Vector3d force_gravity = Vector3d::Zero();
	Vector3d force_wind = Vector3d::Zero();
	//set gravity
	if(flag_gravity)
	{
		force_gravity(1) = gravity_magnitude;
	}
	if(flag_setWindForce)
	{
		srand(time(NULL));
		//wind_magnitude = (rand() % 100) * 0.01;
		wind_magnitude = 1;
		force_wind = const_force * sin((time_step_index % 360) * PI / 180) * wind_magnitude;
	}

	time_counter->StartCounter();

	double PI = 3.14159265;
	int idx_bottom = level_list.size() - 1;
	int num_timestep = 0;
	for (int n = 0; n < level_list.size(); n++)
	{
		//force constraint
		if(flag_setForce && constraintType == Kernel::FORCE_CONSTRAINT)
		{
			if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
			{
				int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					level_list[n]->voxmesh_level->constraint_node_list[k]->force = const_force;

					for(int i=0; i < level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates.size(); ++i)
					{
						level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates[i]->force = const_force;
					}
				}
			}
		}
		//position constraint
		if(flag_setForce && constraintType != Kernel::FORCE_CONSTRAINT)
		{
			if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
			{
				int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					level_list[n]->voxmesh_level->constraint_node_list[k]->prescribed_position = const_force;
				}
			}
		}
		//if level > 0, needs to inherit R, T from parent_level
		if(n > 0)
		{
			vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
			for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
			{
				///////////////////////////////////////////////////////////////////////////////////////
				////compute current position
				//ci->computeCurrentMassCentroid4Target();
				
				////////////////////////////////////////////////////////////////////////////////////////
				//slerp to interpolate rotation q_c
				//linear interpolate translation t_ci(which is not used, so deleted)
				////////////////////////////////////////////////////////////////////////////////////////
				Quaterniond q_ci;
				vector<Quaterniond> list_q;
				vector<Vector3d> list_v;
				vector<double> t;
				double t_add = 0.0;
				Vector3d t_ci = Vector3d::Zero();
				for(int p = 0; p < 8; p++)
				{
					if(ci->vox_list[0]->list_near_parentVox[p])
					{
						Cluster * p_temp = &level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid];

						Quaterniond temp(p_temp->r);
						list_q.push_back(temp);
						
						p_temp->computeCurrentMassCentroid4Target();
						//p_temp->computeCurrentMassCentroid4Static();
						//list_v.push_back((p_temp->current_center - p_temp->r * p_temp->original_center));
						list_v.push_back(p_temp->r*(ci->original_center - p_temp->original_center) + p_temp->current_center);
						

						t.push_back(ci->vox_list[0]->para_interpolate[p]);
						
						ci->r = level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].r;
						//t_ci = p_temp->current_center - p_temp->r * p_temp->original_center;
						t_ci = p_temp->r*(ci->original_center - p_temp->original_center) + p_temp->current_center;
					}
				}
				if(list_q.size() > 1)
				{
					q_ci = list_q[0];
					
					t_ci = t[0] * list_v[0];
					
					while(list_q.size() > 1)
					{
						t_add += t[0];
						double temp = t_add / (t_add + t[1]);
						Quaterniond qtemp = list_q[1].slerp(temp, q_ci);
						q_ci = qtemp;
						t_ci += t[1] * list_v[1];

						list_q.erase(list_q.begin());
						t.erase(t.begin());
						list_v.erase(list_v.begin());
					}
					ci->r = q_ci.toRotationMatrix();
				}


				double det_a = det33(ci->a);
				//ci->current_center = ci->parent_cluster->r * (ci->original_center - ci->parent_cluster->original_center) + ci->parent_cluster->current_center;
				ci->current_center = t_ci;
				vector<DuplicatedNode>::iterator dni;
				for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
				{
					Vector3d _force = dni->force + force_gravity + force_wind;;

					//dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)* dni->coordinate + t_ci;//ci->current_center;
					//dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->parent_cluster->original_center) + ci->parent_cluster->original_center + t_ci;
					
					dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center; // + t_ci;//ci->current_center;
					if (!dni->mapped_node->flag_anchor_node)
					{
						dni->target_position = dni->static_position;// + time_step_size * time_step_size * _force * force_scalar;
					}
				}
				//ci->original_center = ci->current_center;
			}

			// updating target position
			for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
			{
				ni->target_position.setZero();

				vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					ni->target_position += (*dn)->target_position;
				}
				
				ni->target_position /= double(ni->duplicates.size());
				if(constraintType != Kernel::FORCE_CONSTRAINT && ni->flag_constraint_node)
					ni->target_position = ni->prescribed_position;

				dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					(*dn)->target_position = ni->target_position;
				}
				
			}//for
		}

		//else//if n == 0
		{
			//////////////////////////////////////////////////////////////////////////////////
			// level 0 to bottom+1, directly implementing shape matching
			//////////////////////////////////////////////////////////////////////////////////
			int iteration = level_list[n]->times_ShapeMatching;
			if(n == 0 || n < level_list.size())
			{
				for( int i = 0; i < iteration; i ++)
				{
					vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
					for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
					{
						vector<DuplicatedNode>::const_iterator const_ni;

						ci->a_pq = Matrix3d::Zero();
						Vector3d p, q;
						ci->computeCurrentMassCentroid4Target();

						for(const_ni=ci->node_list.begin(); const_ni!=ci->node_list.end(); ++const_ni)
						{
							p = const_ni->target_position - ci->current_center;
							q = const_ni->coordinate - ci->original_center;
							ci->a_pq += const_ni->mass * p * q.transpose();
						}


						ci->a = ci->a_pq * ci->a_qq;
		

						JacobiSVD<Matrix3d> jacobi(ci->a_pq, ComputeFullU|ComputeFullV);
						ci->r = jacobi.matrixU() * jacobi.matrixV().transpose();
		
						num_PE_perTimeStep++;

						double det_a = det33(ci->a);

						vector<DuplicatedNode>::iterator dni;
						for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
						{
							dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center;
						
							if (!dni->mapped_node->flag_anchor_node)
							{
								Vector3d _force = dni->force + force_gravity + force_wind;

								if (n == idx_bottom)
								{
									if(flag_dynamics)
									{
										dni->velocity = (1.0-ci->kappa)*dni->velocity + ci->alpha*(dni->static_position - dni->coordinate - dni->displacement) / ((num_timestep + 1 )* time_step_size)
											+ time_step_size*_force*force_scalar/(num_timestep + 1);
										dni->displacement += (num_timestep + 1)*time_step_size*dni->velocity;
										dni->target_position = dni->coordinate + dni->displacement;
									}
									else
									{
										dni->target_position = dni->static_position + time_step_size * time_step_size * _force * force_scalar;
										dni->displacement = dni->target_position - dni->coordinate;
									}
								}
								else
									dni->target_position = dni->static_position + time_step_size * time_step_size * _force * force_scalar;
							}
						}


					}//for
					if (n == idx_bottom)
						level_list[n]->voxmesh_level->new_energy = 0.0;
					// updating displacement
					for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
					{
						if (n == idx_bottom)
						{
							if(flag_dynamics)
							{
								ni->displacement.setZero();
								//ni->velocity.setZero();
							}
						}
						ni->target_position.setZero();

						vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							if (n == idx_bottom)
							{
								if(flag_dynamics)
								{
									ni->displacement += (*dn)->displacement;
									//ni->velocity += (*dn)->velocity;
								}
							}

							ni->target_position += (*dn)->target_position;
						}

						ni->target_position /= double(ni->duplicates.size());
						if( constraintType != Kernel::FORCE_CONSTRAINT && ni->flag_constraint_node)
							ni->target_position = ni->prescribed_position;

						if (n == idx_bottom)
						{
							if(flag_dynamics)
							{
								ni->displacement /= double(ni->duplicates.size());
								//ni->velocity /= double(ni->duplicates.size());
							}
							else
							{
								ni->displacement = ni->target_position - ni->coordinate;
							}
						}

						dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							(*dn)->target_position = ni->target_position;

							if (n == idx_bottom)
							{
								if(flag_dynamics)
								{
									(*dn)->displacement = ni->displacement;
									//(*dn)->velocity = ni->velocity;
								}			
								else
								{
									(*dn)->displacement = ni->displacement;
								}
							}
							if(n == idx_bottom)
							{
								level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
								level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
								level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
							}

						}
					}//update for

					//ci = level_list[n]->voxmesh_level->cluster_list.begin();
					//for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
					//	ci->computeCurrentMassCentroid4Target();

					num_timestep++;
				}//iteration
			}//
			////////////////////////////////////////////////////////////////////////////////////////////////////////////

			
		}//else

	}
	
	///////////////////////////////////////////////////////////////////////////
	//level 0 update by the last level, multigrid
	///////////////////////////////////////////////////////////////////////////
	if(level_list.size() > 1 && flag_multigrid)
	{

		///////////////////////////////////////////////////////////////////////////////////////
		//compute current position
		//vector<Cluster>::iterator ci = level_list[idx_bottom]->voxmesh_level->cluster_list.begin();
		//for (; ci != level_list[idx_bottom]->voxmesh_level->cluster_list.end(); ++ci)
		//{
		//	ci->computeCurrentMassCentroid();
		//}
		///////////////////////////////////////////////////////////////////////////////////////
		int size_c = level_list[0]->voxmesh_level->cluster_list.size();
		for(int i_c = 0; i_c < size_c; i_c ++)
		{
			Cluster * m_pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];
			Vector3d translation = Vector3d::Zero();
			int sizeLeaf = m_pcluster->leaf_list.size() ;
			
		/*
			vector<Matrix3d> R_all;
			vector<Vector3d> T_all;
			vector<double> m_Interpolation;
			double sum = 0.0;

			//compute interpolation params
			int i_l = 0;
			for (i_l = 0; i_l < sizeLeaf; i_l++)
			{
				double para = 0.0;
				para += pow((m_pcluster->current_center(0) - m_pcluster->leaf_list[i_l]->current_center(0)), 2);
				para += pow((m_pcluster->current_center(1) - m_pcluster->leaf_list[i_l]->current_center(1)), 2);
				para += pow((m_pcluster->current_center(2) - m_pcluster->leaf_list[i_l]->current_center(2)), 2);

				if(para == 0.0)
					para = 0.0000001;

				para = 1.0 / para;

				m_Interpolation.push_back(para);
				sum += para;

				R_all.push_back(m_pcluster->leaf_list[i_l]->r);
				//Vector3d t = m_pcluster->leaf_list[i_l]->current_center - m_pcluster->leaf_list[i_l]->r * m_pcluster->leaf_list[i_l]->original_center;
				Vector3d t = m_pcluster->leaf_list[i_l]->current_center + m_pcluster->leaf_list[i_l]->r * (m_pcluster->original_center - m_pcluster->leaf_list[i_l]->original_center);
				T_all.push_back(t);
			}
			
			//sum = sqrt(sum);
			if(R_all.size() > 1)
			{
				Quaterniond q(R_all[0]);
				double t_add = 0.0;
				int count = 1;
				//translation = (sqrt(m_Interpolation[0]) / sum) * T_all[0];
				translation = (m_Interpolation[0] / sum) * T_all[0];
				while(count < R_all.size())
				{
					Quaterniond q2(R_all[count]);
					t_add += (m_Interpolation[count-1] / sum);
					//double temp = (t_add * t_add) /(t_add * t_add + interpolation * interpolation);
					double temp = t_add / (t_add + (m_Interpolation[count] / sum));
					Quaterniond qtemp = q2.slerp(temp, q);
					q = qtemp;

					translation += (m_Interpolation[count] / sum) * T_all[count];
					
					count ++;
				}
				level_list[0]->voxmesh_level->cluster_list[i_c].r = q.toRotationMatrix();
			}
			else
			{
				level_list[0]->voxmesh_level->cluster_list[i_c].r = R_all[0];
				translation = T_all[0];
			}
			*/
			
			if (sizeLeaf > 1)
			{
				Quaterniond q(m_pcluster->leaf_list[0]->r);
				double t_add = 0.0;
				int count = 1;
				m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
				//m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Static();
				////Vector3d t = m_pcluster->leaf_list[0]->current_center - m_pcluster->leaf_list[0]->r * m_pcluster->leaf_list[0]->original_center;
				Vector3d t = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
				translation = (m_pcluster->leaf_parameter[0] / m_pcluster->term_normlize) * t;
				while(count < sizeLeaf)
				{
					m_pcluster->leaf_list[count]->computeCurrentMassCentroid4Target();
					//m_pcluster->leaf_list[count]->computeCurrentMassCentroid4Static();
					Quaterniond q2(m_pcluster->leaf_list[count]->r);
					t_add += (m_pcluster->leaf_parameter[count-1] / m_pcluster->term_normlize);
					//double temp = (t_add * t_add) /(t_add * t_add + interpolation * interpolation);
					double temp = t_add / (t_add + (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize));
					Quaterniond qtemp = q2.slerp(temp, q);
					q = qtemp;

					////t =  m_pcluster->leaf_list[count]->current_center - m_pcluster->leaf_list[count]->r * m_pcluster->leaf_list[count]->original_center;
					t = m_pcluster->leaf_list[count]->current_center + m_pcluster->leaf_list[count]->r * (m_pcluster->original_center - m_pcluster->leaf_list[count]->original_center);
					translation += (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize) * t;

					count ++;
				}
				level_list[0]->voxmesh_level->cluster_list[i_c].r = q.toRotationMatrix();
			}
			else
			{
				m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
				level_list[0]->voxmesh_level->cluster_list[i_c].r = m_pcluster->leaf_list[0]->r;
				////translation = m_pcluster->leaf_list[0]->current_center - m_pcluster->leaf_list[0]->r * m_pcluster->leaf_list[0]->original_center;
				translation = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
			}
			

			//update static position and target position for this cluster
			//Cluster * pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];

			double det_a = det33(m_pcluster->a);
			m_pcluster->current_center = translation;
			vector<DuplicatedNode>::iterator dni;
			for (dni=m_pcluster->node_list.begin(); dni!=m_pcluster->node_list.end(); ++dni)
			{
				////dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*dni->coordinate + translation;
				dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*(dni->coordinate - m_pcluster->original_center) + m_pcluster->current_center;
				if (!dni->mapped_node->flag_anchor_node)
				{
					Vector3d _force = dni->force + force_gravity + force_wind;
					dni->target_position = dni->static_position;// + time_step_size * time_step_size * _force * force_scalar;
				}
			}
		}
		// updating displacement
		for (node_iterator ni=level_list[0]->voxmesh_level->node_list.begin(); ni!=level_list[0]->voxmesh_level->node_list.end(); ++ni)
		{
			//ni->static_position.setZero();
			ni->target_position.setZero();

			vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				//ni->static_position += (*dn)->static_position;
				ni->target_position += (*dn)->target_position;
			}

			//ni->static_position /= double(ni->duplicates.size());
			ni->target_position /= double(ni->duplicates.size());
			if(constraintType != FORCE_CONSTRAINT && ni->flag_constraint_node)
				ni->target_position = ni->prescribed_position;

			dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				//if(!ni->flag_anchor_node)
					(*dn)->target_position = ni->target_position;
				//else
					//(*dn)->target_position = ni->coordinate;
			}
		}//cluster for
	}

	//time_counter->StopCounter();
	//cout << time_counter->GetElapsedTime() << endl;
	//time_counter->StartCounter();
	int num_last = level_list.size()-1;
	for (node_iterator nmi=p_mesh->node_list.begin(); nmi!=p_mesh->node_list.end(); ++nmi)
	{
		nmi->displacement.setZero();
		for(int i = 0; i < 8; i++)
		{
			nmi->displacement += nmi->list_interpolation_nodes[i][num_last]->displacement * nmi->para_interpolate[i][num_last];
		}
	}

	//time_counter->StopCounter();
	//cout << time_counter->GetElapsedTime() << endl;

	//networking
	if(flag_network_ready && network_role == NETWORK_ROLE_SERVER)
	{
		for(int i = 0; i < pTransmitter.size(); i++)
		{
			if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_PC && pTransmitter[i]->isClientReady)
				pTransmitter[i]->HSMupdate(data4PC);
			else if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_MOBILE)
				pTransmitter[i]->HSMupdate(data4Mobile);
		}
		
	}
	////////////////////////////////////////////////////////////////////////////////////////////


	if(flag_setForce && !flag_setWindForce)
	{
		double ratio, speed, threshold = 0.0;
		VoxMesh * pVM = level_list[idx_bottom]->voxmesh_level;
		ratio = getEnergyRatio(pVM, speed);
		threshold = abs(pVM->new_energy - pVM->old_energy);
		if (!flag_converge)
		{
			time_counter->StopCounter();
			testoutput << time_counter->GetElapsedTime() << endl;
			myEnergy << ratio << endl;
			myHEnergy << pVM->new_energy << endl;
		}
		cout << pVM->new_energy << endl;
		//if (pVM->old_energy >= pVM->new_energy && threshold < 0.0001 && !flag_converge)
		//{
		//	timestep_converge = time_step_index;
		//	flag_converge = true;
		//	cout << "Iteration #: " << timestep_converge - timestep_begin << endl;
		//	cout << "converge!" << endl;
		//}
		pVM->old_energy = pVM->new_energy;

	}
	if(flag_exportTxt)
	{
		vector<Node>::iterator ni;
		for (ni = p_mesh->node_list.begin(); ni != p_mesh->node_list.end(); ++ni)
		{
			testoutput << "v " << ni->coordinate(0) + ni->displacement(0) 
				<< " " << ni->coordinate(1) + ni->displacement(1) 
				<< " " << ni->coordinate(2) + ni->displacement(2) << endl;
		}
	}
	++time_step_index;
	return true;
}

bool Kernel::simulateNextStep4HSMAdaptiveStep()
{
	return true;
}

bool Kernel::simulateNextStep4HSMForce4Step()
{
	/////////////////////////////////////////////////////////////////////////////////
	//test whether quaternion is the same as rotation matrix.
	//Matrix3d r1 = level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].r;
	//Quaterniond test1(r1);
	//Matrix3d r2 = test1.toRotationMatrix();
	////////////////////////////////////////////////////////////////////////////////
	//test whether slerp could generate a reasonable result
	//double PI = 3.14159265;
	//Matrix3d r1 = Matrix3d::Identity();
	//r1(1, 1) = cos(0 * PI / 180);
	//r1(1, 2) = (-1) * sin(0 * PI / 180);
	//r1(2, 1) = sin(0 * PI / 180);
	//r1(2, 2) = cos(0 * PI / 180);

	//Matrix3d r2 = Matrix3d::Identity();
	//r2(1, 1) = cos(90 * PI / 180);
	//r2(1, 2) = (-1) * sin(90 * PI / 180);
	//r2(2, 1) = sin(90 * PI / 180);
	//r2(2, 2) = cos(90 * PI / 180);

	//Quaterniond test1(r1);
	//Quaterniond test2(r2);
	//Quaterniond test3 = test1.slerp(0.7, test2);
	//Matrix3d r3 = test3.toRotationMatrix();

	//// to save energy
	//flag_setForce = true;
	//const_force(0) = 0.15;
	//const_force(1) = 0.5;
	//const_force(2) = 0.0;
	

	double mass_sum = 0.0;
	num_PE_perTimeStep = 0;
	Vector3d force_gravity = Vector3d::Zero();
	Vector3d force_wind = Vector3d::Zero();
	//set gravity
	if(flag_gravity)
	{
		force_gravity(1) = gravity_magnitude;
	}
	if(flag_setWindForce)
	{
		srand(time(NULL));
		//wind_magnitude = (rand() % 100) * 0.01;
		wind_magnitude = 1;
		force_wind = const_force * sin((time_step_index % 360) * PI / 180) * wind_magnitude;
	}


	double PI = 3.14159265;
	int idx_bottom = level_list.size() - 1;
	int n = level_list.size() - 1;

	{
			//////////////////////////////////////////////////////////////////////////////////
			// only last level do shape matching
			//////////////////////////////////////////////////////////////////////////////////
			int iteration = level_list[n]->times_ShapeMatching;
			if(n < level_list.size())
			{
				for( int i = 0; i < iteration; i ++)
				//while(true)
				{
					time_counter->StartCounter();
					vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
					for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
					{
						vector<DuplicatedNode>::const_iterator const_ni;

						ci->a_pq = Matrix3d::Zero();
						Vector3d p, q;
						ci->computeCurrentMassCentroid4Target();

						for(const_ni=ci->node_list.begin(); const_ni!=ci->node_list.end(); ++const_ni)
						{
							p = const_ni->target_position - ci->current_center;
							q = const_ni->coordinate - ci->original_center;
							ci->a_pq += const_ni->mass * p * q.transpose();
						}


						ci->a = ci->a_pq * ci->a_qq;
		

						JacobiSVD<Matrix3d> jacobi(ci->a_pq, ComputeFullU|ComputeFullV);
						ci->r = jacobi.matrixU() * jacobi.matrixV().transpose();
		
						num_PE_perTimeStep++;

						double det_a = det33(ci->a);

						vector<DuplicatedNode>::iterator dni;
						for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
						{
							dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center;
							if (!dni->flag_constraint_node)
								dni->target_position = dni->static_position;
						}


					}//for
					level_list[n]->voxmesh_level->new_energy = 0.0;
					// updating displacement
					for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
					{
						ni->target_position.setZero();

						vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							ni->target_position += (*dn)->target_position;
						}

						ni->target_position /= double(ni->duplicates.size());
						if( ni->flag_constraint_node)
							ni->target_position = ni->prescribed_position;

						dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							(*dn)->target_position = ni->target_position;
						}
					}//update for
				}//iteration
			}//if(n < size)
		}//else

	if(level_list.size() > 1 && flag_multigrid)
	{

		///////////////////////////////////////////////////////////////////////////////////////
		//compute current position
		//vector<Cluster>::iterator ci = level_list[idx_bottom]->voxmesh_level->cluster_list.begin();
		//for (; ci != level_list[idx_bottom]->voxmesh_level->cluster_list.end(); ++ci)
		//{
		//	ci->computeCurrentMassCentroid();
		//}
		///////////////////////////////////////////////////////////////////////////////////////

		time_counter->StartCounter();
		int size_c = level_list[0]->voxmesh_level->cluster_list.size();
		for(int i_c = 0; i_c < size_c; i_c ++)
		{
			Cluster * m_pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];
			Vector3d translation = Vector3d::Zero();
			int sizeLeaf = m_pcluster->leaf_list.size() ;

			if (sizeLeaf > 1)
			{
				Quaterniond q(m_pcluster->leaf_list[0]->r);
				double t_add = 0.0;
				int count = 1;
				m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
				Vector3d t = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
				translation = (m_pcluster->leaf_parameter[0] / m_pcluster->term_normlize) * t;
				while(count < sizeLeaf)
				{
					m_pcluster->leaf_list[count]->computeCurrentMassCentroid4Target();
					Quaterniond q2(m_pcluster->leaf_list[count]->r);
					t_add += (m_pcluster->leaf_parameter[count-1] / m_pcluster->term_normlize);
					double temp = t_add / (t_add + (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize));
					Quaterniond qtemp = q2.slerp(temp, q);
					q = qtemp;

					t = m_pcluster->leaf_list[count]->current_center + m_pcluster->leaf_list[count]->r * (m_pcluster->original_center - m_pcluster->leaf_list[count]->original_center);
					translation += (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize) * t;

					count ++;
				}
				level_list[0]->voxmesh_level->cluster_list[i_c].r = q.toRotationMatrix();
			}
			else
			{
				m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
				level_list[0]->voxmesh_level->cluster_list[i_c].r = m_pcluster->leaf_list[0]->r;
				translation = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
			}


			//update static position and target position for this cluster
			//Cluster * pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];

			double det_a = det33(m_pcluster->a);
			m_pcluster->current_center = translation;
			vector<DuplicatedNode>::iterator dni;
			for (dni=m_pcluster->node_list.begin(); dni!=m_pcluster->node_list.end(); ++dni)
			{
				////dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*dni->coordinate + translation;
				dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*(dni->coordinate - m_pcluster->original_center) + m_pcluster->current_center;
				if (!dni->mapped_node->flag_anchor_node)
				{
					Vector3d _force = dni->force + force_gravity + force_wind;
					dni->target_position = dni->static_position;
				}
			}
		}

		time_counter->StopCounter();
		//cout << time_counter->GetElapsedTime() << endl;
		time_counter->StartCounter();
		// updating displacement
		level_list[0]->voxmesh_level->old_energy = 0.0;
		for (node_iterator ni=level_list[0]->voxmesh_level->node_list.begin(); ni!=level_list[0]->voxmesh_level->node_list.end(); ++ni)
		{
			ni->target_position.setZero();

			vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				ni->target_position += (*dn)->target_position;
			}

			ni->target_position /= double(ni->duplicates.size());
			if(ni->flag_constraint_node)
				ni->target_position = ni->prescribed_position;

			dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				(*dn)->target_position = ni->target_position;
			}
		}//cluster for
	}

	for (n = 0; n < level_list.size(); n++)
	{

		//force constraint
		if(flag_setForce && constraintType == Kernel::FORCE_CONSTRAINT)
		{
			if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
			{
				int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					level_list[n]->voxmesh_level->constraint_node_list[k]->force = const_force;

					for(int i=0; i < level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates.size(); ++i)
					{
						level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates[i]->force = const_force;
					}
				}
			}
		}
		//position constraint
		if(flag_setForce && constraintType != Kernel::FORCE_CONSTRAINT)
		{
			if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
			{
				int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					level_list[n]->voxmesh_level->constraint_node_list[k]->prescribed_position = const_force;
				}
			}
		}
		time_counter->StartCounter();
		//if level > 0, needs to inherit R, T from parent_level
		if(n > 0)
		{

			vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
			for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
			{
				//slerp to interpolate rotation q_c
				//linear interpolate translation t_ci(which is not used, so deleted)
				////////////////////////////////////////////////////////////////////////////////////////
				Quaterniond q_ci;
				vector<Quaterniond> list_q;
				vector<Vector3d> list_v;
				vector<double> t;
				double t_add = 0.0;
				Vector3d t_ci = Vector3d::Zero();

				for(int p = 0; p < 8; p++)
				{
					if(ci->vox_list[0]->list_near_parentVox[p])
					{
						Cluster * p_temp = &level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid];

						Quaterniond temp(p_temp->r);
						list_q.push_back(temp);
						
						p_temp->computeCurrentMassCentroid4Target();
						list_v.push_back(p_temp->r*(ci->original_center - p_temp->original_center) + p_temp->current_center);
						

						t.push_back(ci->vox_list[0]->para_interpolate[p]);
						
						ci->r = level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].r;
						t_ci = p_temp->r*(ci->original_center - p_temp->original_center) + p_temp->current_center;
					}
				}
				if(list_q.size() > 1)
				{
					q_ci = list_q[0];
					
					t_ci = t[0] * list_v[0];
					
					while(list_q.size() > 1)
					{
						t_add += t[0];
						double temp = t_add / (t_add + t[1]);
						Quaterniond qtemp = list_q[1].slerp(temp, q_ci);
						q_ci = qtemp;
						t_ci += t[1] * list_v[1];

						list_q.erase(list_q.begin());
						t.erase(t.begin());
						list_v.erase(list_v.begin());
					}
					ci->r = q_ci.toRotationMatrix();
				}


				double det_a = det33(ci->a);
				ci->current_center = t_ci;
				vector<DuplicatedNode>::iterator dni;
				for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
				{
					Vector3d _force = dni->force + force_gravity + force_wind;;

					dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center; // + t_ci;//ci->current_center;
					if (!dni->mapped_node->flag_anchor_node)
					{
						dni->target_position = dni->static_position;// + time_step_size * time_step_size * _force * force_scalar;
					}
					
				}
			}
			time_counter->StopCounter();
			//cout << time_counter->GetElapsedTime() << endl;
			time_counter->StartCounter();

			// updating target position
			level_list[n]->voxmesh_level->old_energy = 0.0;
			for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
			{
				ni->target_position.setZero();

				vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					ni->target_position += (*dn)->target_position;
				}
				
				ni->target_position /= double(ni->duplicates.size());
				if(ni->flag_constraint_node)
				{
					ni->target_position = ni->prescribed_position;
				}
				
				dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					(*dn)->target_position = ni->target_position;
				}
			}//for
		}
		//else//if n == 0
		{
			//////////////////////////////////////////////////////////////////////////////////
			// level 0 to bottom+1, directly implementing shape matching
			//////////////////////////////////////////////////////////////////////////////////
			int iteration = level_list[n]->times_ShapeMatching;
			if(n < level_list.size())
			{
				for( int i = 0; i < iteration; i ++)
				//while(true)
				{
					time_counter->StartCounter();
					vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
					for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
					{
						vector<DuplicatedNode>::const_iterator const_ni;

						ci->a_pq = Matrix3d::Zero();
						Vector3d p, q;
						ci->computeCurrentMassCentroid4Target();

						for(const_ni=ci->node_list.begin(); const_ni!=ci->node_list.end(); ++const_ni)
						{
							p = const_ni->target_position - ci->current_center;
							q = const_ni->coordinate - ci->original_center;
							ci->a_pq += const_ni->mass * p * q.transpose();
						}


						ci->a = ci->a_pq * ci->a_qq;
		

						JacobiSVD<Matrix3d> jacobi(ci->a_pq, ComputeFullU|ComputeFullV);
						ci->r = jacobi.matrixU() * jacobi.matrixV().transpose();
		
						num_PE_perTimeStep++;

						double det_a = det33(ci->a);

						vector<DuplicatedNode>::iterator dni;
						for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
						{
							dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center;
						
							if (!dni->mapped_node->flag_anchor_node)
							{
								Vector3d _force = dni->force + force_gravity + force_wind;

								if (n == idx_bottom)
								{
									if(flag_dynamics)
									{
										dni->velocity = (1.0-ci->kappa)*dni->velocity + ci->alpha*(dni->static_position - dni->coordinate - dni->displacement) / time_step_size 
												+ time_step_size*_force*force_scalar;
										dni->displacement += time_step_size*dni->velocity;
										dni->target_position = dni->coordinate + dni->displacement;
									}
									else
									{
										dni->target_position = dni->static_position;
										dni->displacement = dni->target_position - dni->coordinate;
									}
								}
								else
								{		
									dni->target_position = dni->static_position;
								}
							}
						}


					}//for

					time_counter->StopCounter();
					//cout << time_counter->GetElapsedTime() << endl;
					time_counter->StartCounter();
					level_list[n]->voxmesh_level->new_energy = 0.0;
					// updating displacement
					for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
					{
						if (n == idx_bottom && i == iteration - 1)
						{
							if(flag_dynamics)
							{
								ni->displacement.setZero();
							}
						}
						ni->target_position.setZero();

						vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							if (n == idx_bottom && i == iteration - 1)
							{
								if(flag_dynamics)
								{
									ni->displacement += (*dn)->displacement;
								}
							}

							ni->target_position += (*dn)->target_position;
						}

						ni->target_position /= double(ni->duplicates.size());
						if( ni->flag_constraint_node && i < iteration - 1)
							ni->target_position = ni->prescribed_position;
						else if ( ni->flag_constraint_node && i == iteration - 1 )
							ni->prescribed_position = ni->target_position;

						if (n == idx_bottom && i == iteration - 1)
						{
							if(flag_dynamics)
							{
								ni->displacement /= double(ni->duplicates.size());
							}
							else
							{
								ni->displacement = ni->target_position - ni->coordinate;
							}
						}

						dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							(*dn)->target_position = ni->target_position;

							if (n == idx_bottom && i == iteration - 1)
							{
								if(flag_dynamics)
								{
									(*dn)->displacement = ni->displacement;
								}			
								else
								{
									(*dn)->displacement = ni->displacement;
								}
							}

							level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
							level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
							level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
						}
					}//update for
					time_counter->StopCounter();
					//cout << time_counter->GetElapsedTime() << endl;
					/*
					VoxMesh * pVM = level_list[n]->voxmesh_level;
					double threshold = 0.0;
					double ratio = 0.0;
					ratio = getEnergyRatio(pVM);
					threshold = abs(pVM->new_energy - pVM->old_energy);
					cout << count << endl;
					cout << ratio << endl;
					if (ratio < energyThreshold && pVM->old_energy > pVM->new_energy)
					{
						pVM->old_energy = pVM->new_energy;
						cout << "break" << endl;
						break;
					}
					if (ratio <= 0.0000001 || n == idx_bottom)
					{
						pVM->old_energy = pVM->new_energy;
						break;
					}
					pVM->old_energy = pVM->new_energy;
					*/
				}//iteration
				//testoutput << n <<": " << count << endl;
			}//if(n < size)
			////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}//else

	}
	
	///////////////////////////////////////////////////////////////////////////
	//level 0 update by the last level, multigrid
	///////////////////////////////////////////////////////////////////////////
	/*
	if(level_list.size() > 1 && flag_multigrid)
	{

		///////////////////////////////////////////////////////////////////////////////////////
		//compute current position
		//vector<Cluster>::iterator ci = level_list[idx_bottom]->voxmesh_level->cluster_list.begin();
		//for (; ci != level_list[idx_bottom]->voxmesh_level->cluster_list.end(); ++ci)
		//{
		//	ci->computeCurrentMassCentroid();
		//}
		///////////////////////////////////////////////////////////////////////////////////////

		time_counter->StartCounter();
		int size_c = level_list[0]->voxmesh_level->cluster_list.size();
		for(int i_c = 0; i_c < size_c; i_c ++)
		{
			Cluster * m_pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];
			Vector3d translation = Vector3d::Zero();
			int sizeLeaf = m_pcluster->leaf_list.size() ;

			if (sizeLeaf > 1)
			{
				Quaterniond q(m_pcluster->leaf_list[0]->r);
				double t_add = 0.0;
				int count = 1;
				m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
				Vector3d t = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
				translation = (m_pcluster->leaf_parameter[0] / m_pcluster->term_normlize) * t;
				while(count < sizeLeaf)
				{
					m_pcluster->leaf_list[count]->computeCurrentMassCentroid4Target();
					Quaterniond q2(m_pcluster->leaf_list[count]->r);
					t_add += (m_pcluster->leaf_parameter[count-1] / m_pcluster->term_normlize);
					double temp = t_add / (t_add + (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize));
					Quaterniond qtemp = q2.slerp(temp, q);
					q = qtemp;

					t = m_pcluster->leaf_list[count]->current_center + m_pcluster->leaf_list[count]->r * (m_pcluster->original_center - m_pcluster->leaf_list[count]->original_center);
					translation += (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize) * t;

					count ++;
				}
				level_list[0]->voxmesh_level->cluster_list[i_c].r = q.toRotationMatrix();
			}
			else
			{
				m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
				level_list[0]->voxmesh_level->cluster_list[i_c].r = m_pcluster->leaf_list[0]->r;
				translation = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
			}


			//update static position and target position for this cluster
			//Cluster * pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];

			double det_a = det33(m_pcluster->a);
			m_pcluster->current_center = translation;
			vector<DuplicatedNode>::iterator dni;
			for (dni=m_pcluster->node_list.begin(); dni!=m_pcluster->node_list.end(); ++dni)
			{
				////dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*dni->coordinate + translation;
				dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*(dni->coordinate - m_pcluster->original_center) + m_pcluster->current_center;
				if (!dni->mapped_node->flag_anchor_node)
				{
					Vector3d _force = dni->force + force_gravity + force_wind;
					dni->target_position = dni->static_position;
				}
			}
		}

		time_counter->StopCounter();
		//cout << time_counter->GetElapsedTime() << endl;
		time_counter->StartCounter();
		// updating displacement
		level_list[0]->voxmesh_level->old_energy = 0.0;
		for (node_iterator ni=level_list[0]->voxmesh_level->node_list.begin(); ni!=level_list[0]->voxmesh_level->node_list.end(); ++ni)
		{
			//ni->static_position.setZero();
			ni->target_position.setZero();

			vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				//ni->static_position += (*dn)->static_position;
				ni->target_position += (*dn)->target_position;
			}

			//ni->static_position /= double(ni->duplicates.size());
			ni->target_position /= double(ni->duplicates.size());
			if(ni->flag_constraint_node)
				ni->target_position = ni->prescribed_position;

			dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				//if(!ni->flag_anchor_node)
					(*dn)->target_position = ni->target_position;

					level_list[0]->voxmesh_level->old_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
					level_list[0]->voxmesh_level->old_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
					level_list[0]->voxmesh_level->old_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
				//else
					//(*dn)->target_position = ni->coordinate;
			}
		}//cluster for
	}
	*/
	time_counter->StopCounter();
	//cout << time_counter->GetElapsedTime() << endl;
	time_counter->StartCounter();
	int num_last = level_list.size()-1;
	for (node_iterator nmi=p_mesh->node_list.begin(); nmi!=p_mesh->node_list.end(); ++nmi)
	{
		nmi->displacement.setZero();
		for(int i = 0; i < 8; i++)
		{
			nmi->displacement += nmi->list_interpolation_nodes[i][num_last]->displacement * nmi->para_interpolate[i][num_last];
		}
	}

	time_counter->StopCounter();
	//cout << time_counter->GetElapsedTime() << endl;

	//networking
	if(flag_network_ready && network_role == NETWORK_ROLE_SERVER)
	{
		for(int i = 0; i < pTransmitter.size(); i++)
		{
			if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_PC && pTransmitter[i]->isClientReady)
				pTransmitter[i]->HSMupdate(data4PC);
			else if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_MOBILE)
				pTransmitter[i]->HSMupdate(data4Mobile);
		}
		
	}
	////////////////////////////////////////////////////////////////////////////////////////////


	if(flag_setForce && !flag_setWindForce)
	{
		// compute the energy
		double match_energy = 0.0;
		for (int i=0; i< level_list[idx_bottom]->voxmesh_level->num_cluster; ++i)
		{
			for (int j=0; j<level_list[idx_bottom]->voxmesh_level->cluster_list[i].num_node; ++j)
			{
				match_energy += (level_list[idx_bottom]->voxmesh_level->cluster_list[i].node_list[j].static_position - level_list[idx_bottom]->voxmesh_level->cluster_list[i].node_list[j].target_position).norm();
			}
		}
		level_list[idx_bottom]->voxmesh_level->old_energy = level_list[idx_bottom]->voxmesh_level->new_energy;
		level_list[idx_bottom]->voxmesh_level->new_energy = match_energy;
		if(level_list[idx_bottom]->voxmesh_level->old_energy != 0)
		{
			double threshold = (level_list[idx_bottom]->voxmesh_level->new_energy - level_list[idx_bottom]->voxmesh_level->old_energy)/ level_list[idx_bottom]->voxmesh_level->old_energy;
			if(threshold < 0 && abs(threshold) < 0.01 && !flag_converge)
			{
				timestep_converge = time_step_index;
				flag_converge = true;
				cout << "TimeStep:" << time_step_index << " Converge!" << endl;
				cout << "Begin TimeStep: " << timestep_begin << endl;
				cout << "Converge TimeStep: " << timestep_converge << endl;
				cout << "Iteration #: " << timestep_converge - timestep_begin << endl;
				cout << "PE # per Iteration:" << num_PE_perTimeStep << endl;
			}
			myHEnergy << match_energy << endl;
		}
	}
	if(flag_exportTxt)
	{
		vector<Node>::iterator ni;
		for (ni = p_mesh->node_list.begin(); ni != p_mesh->node_list.end(); ++ni)
		{
			testoutput << "v " << ni->coordinate(0) + ni->displacement(0) 
				<< " " << ni->coordinate(1) + ni->displacement(1) 
				<< " " << ni->coordinate(2) + ni->displacement(2) << endl;
		}
	}
	++time_step_index;
	return true;
}

bool Kernel::simulateNextStep4HSMForce4StepFirst()
{
	/////////////////////////////////////////////////////////////////////////////////
	//test whether quaternion is the same as rotation matrix.
	//Matrix3d r1 = level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].r;
	//Quaterniond test1(r1);
	//Matrix3d r2 = test1.toRotationMatrix();
	////////////////////////////////////////////////////////////////////////////////
	//test whether slerp could generate a reasonable result
	//double PI = 3.14159265;
	//Matrix3d r1 = Matrix3d::Identity();
	//r1(1, 1) = cos(0 * PI / 180);
	//r1(1, 2) = (-1) * sin(0 * PI / 180);
	//r1(2, 1) = sin(0 * PI / 180);
	//r1(2, 2) = cos(0 * PI / 180);

	//Matrix3d r2 = Matrix3d::Identity();
	//r2(1, 1) = cos(90 * PI / 180);
	//r2(1, 2) = (-1) * sin(90 * PI / 180);
	//r2(2, 1) = sin(90 * PI / 180);
	//r2(2, 2) = cos(90 * PI / 180);

	//Quaterniond test1(r1);
	//Quaterniond test2(r2);
	//Quaterniond test3 = test1.slerp(0.7, test2);
	//Matrix3d r3 = test3.toRotationMatrix();

	//// to save energy
	//flag_setForce = true;
	//const_force(0) = 0.15;
	//const_force(1) = 0.5;
	//const_force(2) = 0.0;
	

	double mass_sum = 0.0;
	num_PE_perTimeStep = 0;
	Vector3d force_gravity = Vector3d::Zero();
	Vector3d force_wind = Vector3d::Zero();
	//set gravity
	if(flag_gravity)
	{
		force_gravity(1) = gravity_magnitude;
	}
	if(flag_setWindForce)
	{
		srand(time(NULL));
		//wind_magnitude = (rand() % 100) * 0.01;
		wind_magnitude = 1;
		force_wind = const_force * sin((time_step_index % 360) * PI / 180) * wind_magnitude;
	}

	if (flag_exportForce)
	{
		myForce << current_force(0) << " "
			<< current_force(1) << " "
			<< current_force(2) << endl;
	}

	if (flag_exportPosConstraint)
	{
		myForce << constraint_first(0) << " "
			<< constraint_first(1) << " "
			<< constraint_first(2) << endl;
		myAnotherForce << constraint_second(0) << " "
			<< constraint_second(1) << " "
			<< constraint_second(2) << endl;
	}

	if (flag_beating)
	{
		int l = level_list.size() - 1;
		
		if (!level_list[l]->voxmesh_level->surface_node_list.empty())
		{

			int size_k = level_list[l]->voxmesh_level->surface_node_list.size();
			for(int k = 0; k < size_k; k ++)
			{
				if ( (time_step_index%10) == 0 && !level_list[l]->voxmesh_level->surface_node_list[k]->flag_anchor_node && !level_list[l]->voxmesh_level->surface_node_list[l]->flag_constraint_node)
				{
					level_list[l]->voxmesh_level->surface_node_list[k]->force = p_mesh->mesh_center - level_list[l]->voxmesh_level->surface_node_list[k]->target_position;

					for(int i=0; i < level_list[l]->voxmesh_level->surface_node_list[k]->duplicates.size(); ++i)
					{
						level_list[l]->voxmesh_level->surface_node_list[k]->duplicates[i]->force = p_mesh->mesh_center - level_list[l]->voxmesh_level->surface_node_list[k]->target_position;
					}
				}
				else
				{
					level_list[l]->voxmesh_level->surface_node_list[k]->force = Vector3d::Zero();

					for(int i=0; i < level_list[l]->voxmesh_level->surface_node_list[k]->duplicates.size(); ++i)
					{
						level_list[l]->voxmesh_level->surface_node_list[k]->duplicates[i]->force = Vector3d::Zero();
					}
				}
			}
		}
		
		/*
		if (!level_list[l]->voxmesh_level->surface_vox_list.empty())
		{

			int size_k = level_list[l]->voxmesh_level->surface_vox_list.size();
			for(int k = 0; k < size_k; k ++)
			{
				int idx = level_list[l]->voxmesh_level->surface_vox_list[k]->clusterid;

				for (int j = 0; j < level_list[l]->voxmesh_level->cluster_list[idx].node_list.size(); j ++)
				{
					if ( (time_step_index%15) == 0 && !level_list[l]->voxmesh_level->cluster_list[idx].node_list[j].mapped_node->flag_anchor_node && !level_list[l]->voxmesh_level->cluster_list[idx].node_list[j].mapped_node->flag_constraint_node)
					{
						//level_list[l]->voxmesh_level->surface_node_list[k]->force = p_mesh->mesh_center - level_list[l]->voxmesh_level->surface_node_list[k]->target_position;

						//for(int i=0; i < level_list[l]->voxmesh_level->surface_node_list[k]->duplicates.size(); ++i)
						//{
						//	level_list[l]->voxmesh_level->surface_node_list[k]->duplicates[i]->force = p_mesh->mesh_center - level_list[l]->voxmesh_level->surface_node_list[k]->target_position;
						//}
						level_list[l]->voxmesh_level->cluster_list[idx].node_list[j].force = p_mesh->mesh_center - level_list[l]->voxmesh_level->cluster_list[idx].node_list[j].target_position;
					}
					else
					{
						//level_list[l]->voxmesh_level->surface_node_list[k]->force = Vector3d::Zero();

						//for(int i=0; i < level_list[l]->voxmesh_level->surface_node_list[k]->duplicates.size(); ++i)
						//{
						//	level_list[l]->voxmesh_level->surface_node_list[k]->duplicates[i]->force = Vector3d::Zero();
						//}
						level_list[l]->voxmesh_level->cluster_list[idx].node_list[j].force = p_mesh->mesh_center - level_list[l]->voxmesh_level->cluster_list[idx].node_list[j].target_position;
					}
				}
			}
		}
		*/
	}

	if (flag_redo && constraintType == Kernel::FORCE_CONSTRAINT)
	{
		if (no_record < force_list.size())
		{
			for (int n = 0; n < level_list.size(); n++)
			{
				if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
				{
					int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
					for(int k = 0; k < size_k; k ++)
					{
						level_list[n]->voxmesh_level->constraint_node_list[k]->force = force_list[no_record];

						for(int i=0; i < level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates.size(); ++i)
						{
							level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates[i]->force = force_list[no_record];
						}
					}
				}
				//flag_exportObj = true;
				//cout << force_list[no_record] << endl;
			}
			no_record ++;
		}
		else
		{
			//
			//flag_exportObj = false;
			cout << "end of redo!" << endl;
			return true;
		}
	}
	if (flag_redo && constraintType == Kernel::POSITION_CONSTRAINT)
	{
		if (no_record < force_list.size())
		{
			for (int n = 0; n < level_list.size(); n++)
			{
				if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
				{
					int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
					for(int k = 0; k < size_k; k ++)
					{
						level_list[n]->voxmesh_level->constraint_node_list[k]->prescribed_position = level_list[n]->voxmesh_level->constraint_displacement[k] + force_list[no_record];
					}
				}
				//flag_exportObj = true;
				//cout << force_list[no_record] << endl;
			}
			for (int n = 0; n < level_list.size(); n++)
			{
				if (!level_list[n]->voxmesh_level->another_constraint_node_list.empty())
				{
					int size_k = level_list[n]->voxmesh_level->another_constraint_node_list.size();
					for(int k = 0; k < size_k; k ++)
					{
						level_list[n]->voxmesh_level->another_constraint_node_list[k]->prescribed_position = level_list[n]->voxmesh_level->another_constraint_displacement[k] + another_force_list[no_record];
					}
				}
				//flag_exportObj = true;
				//cout << force_list[no_record] << endl;
			}
			no_record ++;
			//flag_exportObj = true;
		}
		else
		{
			//
			//flag_exportObj = false;
			cout << "end of redo!" << endl;
			return true;
		}
	}
	double PI = 3.14159265;
	//int idx_bottom = level_list.size() - 1;
	//yuan 1223//
	int idx_bottom = 1;

	for (int n = 0; n < level_list.size(); n++)
	{

		//force constraint
		if(flag_setForce && constraintType == Kernel::FORCE_CONSTRAINT)
		{
			if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
			{
				int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					level_list[n]->voxmesh_level->constraint_node_list[k]->force = const_force;

					for(int i=0; i < level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates.size(); ++i)
					{
						level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates[i]->force = const_force;
					}
				}
			}
		}
		//position constraint
		if(flag_setForce && constraintType != Kernel::FORCE_CONSTRAINT)
		{
			if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
			{
				int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					level_list[n]->voxmesh_level->constraint_node_list[k]->prescribed_position = const_force;
				}
			}
		}
		time_counter->StartCounter();
		//if level > 0, needs to inherit R, T from parent_level
		if(n > 0)
		{

			vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
			for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
			{
				//slerp to interpolate rotation q_c
				//linear interpolate translation t_ci(which is not used, so deleted)
				////////////////////////////////////////////////////////////////////////////////////////
				Quaterniond q_ci;
				vector<Quaterniond> list_q;
				vector<Vector3d> list_v;
				vector<double> t;
				double t_add = 0.0;
				Vector3d t_ci = Vector3d::Zero();

				for(int p = 0; p < 8; p++)
				{
					if(ci->vox_list[0]->list_near_parentVox[p])
					{
						Cluster * p_temp = &level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid];

						Quaterniond temp(p_temp->r);
						list_q.push_back(temp);
						
						p_temp->computeCurrentMassCentroid4Target();
						list_v.push_back(p_temp->r*(ci->original_center - p_temp->original_center) + p_temp->current_center);
						

						t.push_back(ci->vox_list[0]->para_interpolate[p]);
						
						ci->r = level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].r;
						t_ci = p_temp->r*(ci->original_center - p_temp->original_center) + p_temp->current_center;
					}
				}
				if(list_q.size() > 1)
				{
					q_ci = list_q[0];
					
					t_ci = t[0] * list_v[0];
					
					while(list_q.size() > 1)
					{
						t_add += t[0];
						double temp = t_add / (t_add + t[1]);
						Quaterniond qtemp = list_q[1].slerp(temp, q_ci);
						q_ci = qtemp;
						t_ci += t[1] * list_v[1];

						list_q.erase(list_q.begin());
						t.erase(t.begin());
						list_v.erase(list_v.begin());
					}
					ci->r = q_ci.toRotationMatrix();
				}


				double det_a = det33(ci->a);
				ci->current_center = t_ci;
				vector<DuplicatedNode>::iterator dni;
				for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
				{
					Vector3d _force = dni->force + force_gravity + force_wind;;

					//dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center; // + t_ci;//ci->current_center;
					//apply directly by parent cluster
					dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->parent_cluster->r)*(dni->coordinate - ci->parent_cluster->original_center) + ci->parent_cluster->current_center; // + t_ci;//ci->current_center;
					if (!dni->mapped_node->flag_anchor_node)
					{
						dni->target_position = dni->static_position;// + time_step_size * time_step_size * _force * force_scalar;
					}
					//if (dni->flag_constraint_node)
					//{
					//	dni->mapped_node->prescribed_position = (1.0-ci->beta)*ci->super_parent_cluster->r*(dni->coordinate - ci->super_parent_cluster->original_center) + ci->super_parent_cluster->current_center;
					//}
					
				}
			}
			time_counter->StopCounter();
			//cout << time_counter->GetElapsedTime() << endl;
			time_counter->StartCounter();

			// updating target position
			level_list[n]->voxmesh_level->old_energy = 0.0;
			for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
			{
				ni->target_position.setZero();

				vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					ni->target_position += (*dn)->target_position;
				}
				
				ni->target_position /= double(ni->duplicates.size());
				if(ni->flag_constraint_node && constraintType != Kernel::FORCE_CONSTRAINT)
				{
					ni->target_position = ni->prescribed_position;
				}
				
				dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					(*dn)->target_position = ni->target_position;

					level_list[n]->voxmesh_level->old_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
					level_list[n]->voxmesh_level->old_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
					level_list[n]->voxmesh_level->old_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
				}
			}//for
			//cout << "inherit old:" << level_list[n]->voxmesh_level->old_energy << endl;
		}

		//else//if n == 0
		{
			//////////////////////////////////////////////////////////////////////////////////
			// level 0 to bottom+1, directly implementing shape matching
			//////////////////////////////////////////////////////////////////////////////////
			int iteration = level_list[n]->times_ShapeMatching;
			if(n < level_list.size())
			{
				for( int i = 0; i < iteration; i ++)
				//while(true)
				{
					time_counter->StartCounter();
					vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
					for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
					{
						vector<DuplicatedNode>::const_iterator const_ni;

						ci->a_pq = Matrix3d::Zero();
						Vector3d p, q;
						//yuan////////////////
						if (flag_redo && i == iteration-1)
						{
							if(step_list[no_record-1] > n)
								ci->flag_isRendering = false;
							else if(step_list[no_record-1] == n)
							{
								ci->flag_isRendering = true;
							}
						}
						//yuan////////////////
						//////////////////////////////////////////////////////////////////// only for force
						if(i == 0)
						{
							vector<DuplicatedNode>::iterator f_ni;
							for(f_ni=ci->node_list.begin(); f_ni!=ci->node_list.end(); ++f_ni)
							{
								Vector3d _force = f_ni->force + force_gravity + force_wind;
								f_ni->target_position = f_ni->target_position + time_step_size * time_step_size * _force * force_scalar;
								//f_ni->mapped_node->prescribed_position = f_ni->target_position;
							}
						}
						/////////////////////////////////////////////////////////////////////
						ci->computeCurrentMassCentroid4Target();

						for(const_ni=ci->node_list.begin(); const_ni!=ci->node_list.end(); ++const_ni)
						{
							p = const_ni->target_position - ci->current_center;
							q = const_ni->coordinate - ci->original_center;
							ci->a_pq += const_ni->mass * p * q.transpose();
						}

						ci->a = ci->a_pq * ci->a_qq;
		

						JacobiSVD<Matrix3d> jacobi(ci->a_pq, ComputeFullU|ComputeFullV);
						ci->r = jacobi.matrixU() * jacobi.matrixV().transpose();
		
						num_PE_perTimeStep++;

						double det_a = det33(ci->a);

						vector<DuplicatedNode>::iterator dni;
						for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
						{
							dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center;
						
							if (!dni->mapped_node->flag_anchor_node)
							{
								Vector3d _force = dni->force + force_gravity + force_wind;

								if (n == idx_bottom)
								{
									if(flag_dynamics)
									{
										if (i == iteration - 1)
										{
											//dni->velocity = dni->velocity + (dni->target_position - dni->coordinate - dni->displacement) / (level_list.size() * time_step_size);
											//dni->velocity = (1.0-ci->kappa)*dni->velocity + ci->alpha*(dni->static_position - dni->target_position) / (level_list.size() * time_step_size);
											//dni->displacement += (level_list.size() * time_step_size)*dni->velocity;
											dni->velocity = (1.0-ci->kappa)*dni->velocity + ci->alpha*(dni->static_position - dni->coordinate - dni->displacement) / time_step_size;
											dni->displacement += time_step_size*dni->velocity;
											dni->target_position = dni->coordinate + dni->displacement;
										}
										//else if (i == 0)
										//{
										//	dni->target_position = dni->static_position + time_step_size * time_step_size * _force * force_scalar;
										//}
										else
											dni->target_position = dni->static_position;
									}
									else
									{
										//if (i == 0)
										//{
										//	dni->target_position = dni->static_position + time_step_size * time_step_size * _force * force_scalar;
										//}
										/*else*/
											dni->target_position = dni->static_position;
									}
								}
								else
								{
									//if (i == 0)
									//{
									//	dni->target_position = dni->static_position + time_step_size * time_step_size * _force * force_scalar;
									//}
									//else
										dni->target_position = dni->static_position;
								}
								//if (dni->flag_constraint_node && n > 0 && i == 0)
									//dni->mapped_node->prescribed_position = dni->mapped_node->parent_node->prescribed_position;
									//dni->mapped_node->prescribed_position = (1.0-ci->beta)*ci->super_parent_cluster->r*(dni->coordinate - ci->super_parent_cluster->original_center) + ci->super_parent_cluster->current_center;
							}
						}


					}//for

					time_counter->StopCounter();
					//cout << time_counter->GetElapsedTime() << endl;
					time_counter->StartCounter();
					level_list[n]->voxmesh_level->new_energy = 0.0;
					// updating displacement
					for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
					{
						if (n == idx_bottom && i == iteration - 1)
						{
							if(flag_dynamics)
							{
								ni->displacement.setZero();
							}
						}
						ni->target_position.setZero();

						vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							if (n == idx_bottom && i == iteration - 1)
							{
								if(flag_dynamics)
								{
									ni->displacement += (*dn)->displacement;
								}
							}

							ni->target_position += (*dn)->target_position;
						}

						ni->target_position /= double(ni->duplicates.size());
						//if( ni->flag_constraint_node && i > 1 && i < iteration - 1)
							//ni->target_position = ni->prescribed_position;
						//else if( ni->flag_constraint_node && i == 1)
						//	ni->prescribed_position = ni->target_position;
						if( ni->flag_constraint_node  && constraintType != Kernel::FORCE_CONSTRAINT)//?? if not including last iteration?
							ni->target_position = ni->prescribed_position;
						if( ni->flag_constraint_node  && constraintType == Kernel::FORCE_CONSTRAINT && i < iteration - 1)//?? if not including last iteration?
							ni->target_position = ni->prescribed_position;

						if (n == idx_bottom && i == iteration - 1)
						{
							if(flag_dynamics)
							{
								ni->displacement /= double(ni->duplicates.size());
							}
							else
							{
								ni->displacement = ni->target_position - ni->coordinate;
							}
						}

						dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							(*dn)->target_position = ni->target_position;

							if (n == idx_bottom && i == iteration - 1)
							{
								if(flag_dynamics)
								{
									(*dn)->displacement = ni->displacement;
								}			
								else
								{
									(*dn)->displacement = ni->displacement;
								}
							}

							level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
							level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
							level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
						}
					}//update for
					time_counter->StopCounter();

					//yuan////////////////
					if (flag_redo && i == iteration-1)
					{
						vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
						for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
						{
							if(ci->flag_isRendering)
							{
							    //computing energy
								double cEnergy = 0.0;
								for(vector<DuplicatedNode>::const_iterator const_ni = ci->node_list.begin(); const_ni!=ci->node_list.end(); ++const_ni)
								{
									cEnergy += pow((const_ni->static_position(0) - const_ni->target_position(0)), 2);
									cEnergy += pow((const_ni->static_position(1) - const_ni->target_position(1)), 2);
									cEnergy += pow((const_ni->static_position(2) - const_ni->target_position(2)), 2);
								}
								if (cEnergy > energyThreshold)
								{
									if (ci->cluster_list_children.size() > 0)
									{
										ci->flag_isRendering = false;
										for (int i = 0; i < ci->cluster_list_children.size(); i++)
										{
											ci->cluster_list_children[i]->flag_isRendering = true;
										}
									}
								}
							}
						}
					}
					//yuan////////////////

					//cout << time_counter->GetElapsedTime() << endl;
					/*
					VoxMesh * pVM = level_list[n]->voxmesh_level;
					double threshold = 0.0;
					double ratio = 0.0;
					ratio = getEnergyRatio(pVM);
					threshold = abs(pVM->new_energy - pVM->old_energy);
					cout << count << endl;
					cout << ratio << endl;
					if (ratio < energyThreshold && pVM->old_energy > pVM->new_energy)
					{
						pVM->old_energy = pVM->new_energy;
						cout << "break" << endl;
						break;
					}
					if (ratio <= 0.0000001 || n == idx_bottom)
					{
						pVM->old_energy = pVM->new_energy;
						break;
					}
					pVM->old_energy = pVM->new_energy;
					*/
				}//iteration
				//testoutput << n <<": " << count << endl;
			}//if(n < size)
			////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}//else

	}
	
	///////////////////////////////////////////////////////////////////////////
	//level 0 update by the last level, multigrid
	///////////////////////////////////////////////////////////////////////////
	
	if(level_list.size() > 1 && flag_multigrid)
	{

		///////////////////////////////////////////////////////////////////////////////////////
		//compute current position
		//vector<Cluster>::iterator ci = level_list[idx_bottom]->voxmesh_level->cluster_list.begin();
		//for (; ci != level_list[idx_bottom]->voxmesh_level->cluster_list.end(); ++ci)
		//{
		//	ci->computeCurrentMassCentroid();
		//}
		///////////////////////////////////////////////////////////////////////////////////////

		time_counter->StartCounter();
		int size_c = level_list[0]->voxmesh_level->cluster_list.size();
		for(int i_c = 0; i_c < size_c; i_c ++)
		{
			Cluster * m_pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];
			Vector3d translation = Vector3d::Zero();
			int sizeLeaf = m_pcluster->leaf_list.size() ;

			if (sizeLeaf > 1)
			{
				Quaterniond q(m_pcluster->leaf_list[0]->r);
				double t_add = 0.0;
				int count = 1;
				m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
				Vector3d t = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
				translation = (m_pcluster->leaf_parameter[0] / m_pcluster->term_normlize) * t;
				while(count < sizeLeaf)
				{
					m_pcluster->leaf_list[count]->computeCurrentMassCentroid4Target();
					Quaterniond q2(m_pcluster->leaf_list[count]->r);
					t_add += (m_pcluster->leaf_parameter[count-1] / m_pcluster->term_normlize);
					double temp = t_add / (t_add + (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize));
					Quaterniond qtemp = q2.slerp(temp, q);
					q = qtemp;

					t = m_pcluster->leaf_list[count]->current_center + m_pcluster->leaf_list[count]->r * (m_pcluster->original_center - m_pcluster->leaf_list[count]->original_center);
					translation += (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize) * t;

					count ++;
				}
				level_list[0]->voxmesh_level->cluster_list[i_c].r = q.toRotationMatrix();
			}
			else
			{
				m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
				level_list[0]->voxmesh_level->cluster_list[i_c].r = m_pcluster->leaf_list[0]->r;
				translation = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
			}


			//update static position and target position for this cluster
			//Cluster * pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];

			double det_a = det33(m_pcluster->a);
			m_pcluster->current_center = translation;
			vector<DuplicatedNode>::iterator dni;
			for (dni=m_pcluster->node_list.begin(); dni!=m_pcluster->node_list.end(); ++dni)
			{
				////dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*dni->coordinate + translation;
				dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*(dni->coordinate - m_pcluster->original_center) + m_pcluster->current_center;
				if (!dni->mapped_node->flag_anchor_node)
				{
					Vector3d _force = dni->force + force_gravity + force_wind;
					dni->target_position = dni->static_position;
				}
			}
		}

		time_counter->StopCounter();
		//cout << time_counter->GetElapsedTime() << endl;
		time_counter->StartCounter();
		// updating displacement
		level_list[0]->voxmesh_level->old_energy = 0.0;
		for (node_iterator ni=level_list[0]->voxmesh_level->node_list.begin(); ni!=level_list[0]->voxmesh_level->node_list.end(); ++ni)
		{
			//ni->static_position.setZero();
			ni->target_position.setZero();

			vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				//ni->static_position += (*dn)->static_position;
				ni->target_position += (*dn)->target_position;
			}

			//ni->static_position /= double(ni->duplicates.size());
			ni->target_position /= double(ni->duplicates.size());
			if(ni->flag_constraint_node && constraintType != Kernel::FORCE_CONSTRAINT)
				ni->target_position = ni->prescribed_position;

			dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				//if(!ni->flag_anchor_node)
					(*dn)->target_position = ni->target_position;

					level_list[0]->voxmesh_level->old_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
					level_list[0]->voxmesh_level->old_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
					level_list[0]->voxmesh_level->old_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
				//else
					//(*dn)->target_position = ni->coordinate;
			}
		}//cluster for
	}
	
	time_counter->StopCounter();
	//cout << time_counter->GetElapsedTime() << endl;
	time_counter->StartCounter();
	//yuan//
	//int num_last = level_list.size()-1;
	int num_last = 1;
	for (node_iterator nmi=p_mesh->node_list.begin(); nmi!=p_mesh->node_list.end(); ++nmi)
	{
		nmi->displacement.setZero();
		for(int i = 0; i < 8; i++)
		{
			nmi->displacement += nmi->list_interpolation_nodes[i][num_last]->displacement * nmi->para_interpolate[i][num_last];
		}
	}

	//time_counter->StopCounter();
	////cout << time_counter->GetElapsedTime() << endl;
	//yuan

	//networking
	if(flag_network_ready && network_role == NETWORK_ROLE_SERVER)
	{
		for(int i = 0; i < pTransmitter.size(); i++)
		{
			if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_PC && pTransmitter[i]->isClientReady)
				pTransmitter[i]->HSMupdate(data4PC);
			else if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_MOBILE)
				pTransmitter[i]->HSMupdate(data4Mobile);
		}
		
	}
	////////////////////////////////////////////////////////////////////////////////////////////


	if(flag_setForce && !flag_setWindForce)
	{
		// compute the energy
		double match_energy = 0.0;
		for (int i=0; i< level_list[idx_bottom]->voxmesh_level->num_cluster; ++i)
		{
			for (int j=0; j<level_list[idx_bottom]->voxmesh_level->cluster_list[i].num_node; ++j)
			{
				match_energy += (level_list[idx_bottom]->voxmesh_level->cluster_list[i].node_list[j].static_position - level_list[idx_bottom]->voxmesh_level->cluster_list[i].node_list[j].target_position).norm();
			}
		}
		level_list[idx_bottom]->voxmesh_level->old_energy = level_list[idx_bottom]->voxmesh_level->new_energy;
		level_list[idx_bottom]->voxmesh_level->new_energy = match_energy;
		if(level_list[idx_bottom]->voxmesh_level->old_energy != 0)
		{
			double threshold = (level_list[idx_bottom]->voxmesh_level->new_energy - level_list[idx_bottom]->voxmesh_level->old_energy)/ level_list[idx_bottom]->voxmesh_level->old_energy;
			if(threshold < 0 && abs(threshold) < 0.01 && !flag_converge)
			{
				timestep_converge = time_step_index;
				flag_converge = true;
				cout << "TimeStep:" << time_step_index << " Converge!" << endl;
				cout << "Begin TimeStep: " << timestep_begin << endl;
				cout << "Converge TimeStep: " << timestep_converge << endl;
				cout << "Iteration #: " << timestep_converge - timestep_begin << endl;
				cout << "PE # per Iteration:" << num_PE_perTimeStep << endl;
			}
			myHEnergy << match_energy << endl;
		}
	}
	if(flag_exportTxt)
	{
		vector<Node>::iterator ni;
		for (ni = p_mesh->node_list.begin(); ni != p_mesh->node_list.end(); ++ni)
		{
			testoutput << "v " << ni->coordinate(0) + ni->displacement(0) 
				<< " " << ni->coordinate(1) + ni->displacement(1) 
				<< " " << ni->coordinate(2) + ni->displacement(2) << endl;
		}
	}

	if (flag_redo)
	{
		myHEnergy << level_list[idx_bottom]->voxmesh_level->new_energy << endl;
	}
	++time_step_index;
	return true;
}
bool Kernel::simulateNextStep4HSMForce4StepFirst1()
{
	/////////////////////////////////////////////////////////////////////////////////
	//test whether quaternion is the same as rotation matrix.
	//Matrix3d r1 = level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].r;
	//Quaterniond test1(r1);
	//Matrix3d r2 = test1.toRotationMatrix();
	////////////////////////////////////////////////////////////////////////////////
	//test whether slerp could generate a reasonable result
	//double PI = 3.14159265;
	//Matrix3d r1 = Matrix3d::Identity();
	//r1(1, 1) = cos(0 * PI / 180);
	//r1(1, 2) = (-1) * sin(0 * PI / 180);
	//r1(2, 1) = sin(0 * PI / 180);
	//r1(2, 2) = cos(0 * PI / 180);

	//Matrix3d r2 = Matrix3d::Identity();
	//r2(1, 1) = cos(90 * PI / 180);
	//r2(1, 2) = (-1) * sin(90 * PI / 180);
	//r2(2, 1) = sin(90 * PI / 180);
	//r2(2, 2) = cos(90 * PI / 180);

	//Quaterniond test1(r1);
	//Quaterniond test2(r2);
	//Quaterniond test3 = test1.slerp(0.7, test2);
	//Matrix3d r3 = test3.toRotationMatrix();

	//// to save energy
	//flag_setForce = true;
	//const_force(0) = 0.15;
	//const_force(1) = 0.5;
	//const_force(2) = 0.0;
	

	double mass_sum = 0.0;
	num_PE_perTimeStep = 0;
	Vector3d force_gravity = Vector3d::Zero();
	Vector3d force_wind = Vector3d::Zero();
	//set gravity
	if(flag_gravity)
	{
		force_gravity(1) = gravity_magnitude;
	}
	if(flag_setWindForce)
	{
		srand(time(NULL));
		//wind_magnitude = (rand() % 100) * 0.01;
		wind_magnitude = 1;
		force_wind = const_force * sin((time_step_index % 360) * PI / 180) * wind_magnitude;
	}


	double PI = 3.14159265;
	int idx_bottom = level_list.size() - 1;

	for (int n = 0; n < level_list.size(); n++)
	{

		//force constraint
		if(flag_setForce && constraintType == Kernel::FORCE_CONSTRAINT)
		{
			
			if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
			{
				int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					level_list[n]->voxmesh_level->constraint_node_list[k]->force = const_force;

					for(int i=0; i < level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates.size(); ++i)
					{
						level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates[i]->force = const_force;
					}
				}
			}
		}
		//position constraint
		if(flag_setForce && constraintType != Kernel::FORCE_CONSTRAINT)
		{
			if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
			{
				int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					level_list[n]->voxmesh_level->constraint_node_list[k]->prescribed_position = const_force;
				}
			}
		}

		//deal with constraint clusters
		if(!level_list[n]->voxmesh_level->constraint_cluster_list.empty())
		{
			if (n > 0 && n < level_list.size() - 1)
			{
				vector<Cluster *>::iterator cci = level_list[n]->voxmesh_level->constraint_cluster_list.begin();
				for (; cci !=  level_list[n]->voxmesh_level->constraint_cluster_list.end(); cci++)
				{
					Vector3d translation = Vector3d::Zero();
					int sizeLeaf = (*cci)->leaf_list.size() ;

					if (sizeLeaf > 1)
					{
						Quaterniond q((*cci)->leaf_list[0]->r);
						double t_add = 0.0;
						int count = 1;
						(*cci)->leaf_list[0]->computeCurrentMassCentroid4Target();
						Vector3d t = (*cci)->leaf_list[0]->current_center + (*cci)->leaf_list[0]->r * ((*cci)->original_center - (*cci)->leaf_list[0]->original_center);
						translation = ((*cci)->leaf_parameter[0] / (*cci)->term_normlize) * t;
						while(count < sizeLeaf)
						{
							(*cci)->leaf_list[count]->computeCurrentMassCentroid4Target();
							Quaterniond q2((*cci)->leaf_list[count]->r);
							t_add += ((*cci)->leaf_parameter[count-1] / (*cci)->term_normlize);
							double temp = t_add / (t_add + ((*cci)->leaf_parameter[count] / (*cci)->term_normlize));
							Quaterniond qtemp = q2.slerp(temp, q);
							q = qtemp;

							t = (*cci)->leaf_list[count]->current_center + (*cci)->leaf_list[count]->r * ((*cci)->original_center - (*cci)->leaf_list[count]->original_center);
							translation += ((*cci)->leaf_parameter[count] / (*cci)->term_normlize) * t;

							count ++;
						}
						(*cci)->r = q.toRotationMatrix();
					}
					else
					{
						(*cci)->leaf_list[0]->computeCurrentMassCentroid4Target();
						(*cci)->r = (*cci)->leaf_list[0]->r;
						translation = (*cci)->leaf_list[0]->current_center + (*cci)->leaf_list[0]->r * ((*cci)->original_center - (*cci)->leaf_list[0]->original_center);
					}


					double det_a = det33((*cci)->a);
					(*cci)->current_center = translation;
					vector<DuplicatedNode>::iterator dni;
					for (dni=(*cci)->node_list.begin(); dni!=(*cci)->node_list.end(); ++dni)
					{
						dni->static_position = ((*cci)->beta*(*cci)->a/det_a + (1.0-(*cci)->beta)*(*cci)->r)*(dni->coordinate - (*cci)->original_center) + (*cci)->current_center;
						if (!dni->mapped_node->flag_anchor_node)
						{
							dni->target_position = dni->static_position;
						}
					}
				}
			}//if n < level_list.size() - 1
		}
		//deal with constraint points, only average
		if(!level_list[n]->voxmesh_level->constraint_node_list.empty())
		{
			vector<Node *>::iterator nni = level_list[n]->voxmesh_level->constraint_node_list.begin();
			for(; nni != level_list[n]->voxmesh_level->constraint_node_list.end(); nni++)
			{
				(*nni)->target_position.setZero();
				vector<DuplicatedNode*>::iterator dn = (*nni)->duplicates.begin();
				for (;dn!=(*nni)->duplicates.end();++dn)
				{
					(*nni)->target_position += (*dn)->target_position;
				}
				(*nni)->target_position /= double((*nni)->duplicates.size());

				dn = (*nni)->duplicates.begin();
				for (;dn!=(*nni)->duplicates.end();++dn)
				{
					Vector3d _force = (*dn)->force + force_gravity + force_wind;
					(*dn)->target_position = (*nni)->target_position + time_step_size * time_step_size * _force * force_scalar;
					(*dn)->mapped_node->prescribed_position = (*dn)->target_position;
				}
			}

		}

		time_counter->StartCounter();
		//if level > 0, needs to inherit R, T from parent_level
		if(n > 0)
		{

			vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
			for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
			{
				//slerp to interpolate rotation q_c
				//linear interpolate translation t_ci(which is not used, so deleted)
				////////////////////////////////////////////////////////////////////////////////////////
				Quaterniond q_ci;
				vector<Quaterniond> list_q;
				vector<Vector3d> list_v;
				vector<double> t;
				double t_add = 0.0;
				Vector3d t_ci = Vector3d::Zero();

				for(int p = 0; p < 8; p++)
				{
					if(ci->vox_list[0]->list_near_parentVox[p])
					{
						Cluster * p_temp = &level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid];

						Quaterniond temp(p_temp->r);
						list_q.push_back(temp);
						
						p_temp->computeCurrentMassCentroid4Target();
						list_v.push_back(p_temp->r*(ci->original_center - p_temp->original_center) + p_temp->current_center);
						

						t.push_back(ci->vox_list[0]->para_interpolate[p]);
						
						ci->r = level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].r;
						t_ci = p_temp->r*(ci->original_center - p_temp->original_center) + p_temp->current_center;
					}
				}
				if(list_q.size() > 1)
				{
					q_ci = list_q[0];
					
					t_ci = t[0] * list_v[0];
					
					while(list_q.size() > 1)
					{
						t_add += t[0];
						double temp = t_add / (t_add + t[1]);
						Quaterniond qtemp = list_q[1].slerp(temp, q_ci);
						q_ci = qtemp;
						t_ci += t[1] * list_v[1];

						list_q.erase(list_q.begin());
						t.erase(t.begin());
						list_v.erase(list_v.begin());
					}
					ci->r = q_ci.toRotationMatrix();
				}


				double det_a = det33(ci->a);
				ci->current_center = t_ci;
				vector<DuplicatedNode>::iterator dni;
				for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
				{
					Vector3d _force = dni->force + force_gravity + force_wind;;

					dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center; // + t_ci;//ci->current_center;
					if (!dni->mapped_node->flag_anchor_node)
					{
						dni->target_position = dni->static_position;// + time_step_size * time_step_size * _force * force_scalar;
					}
					//if (dni->flag_constraint_node)
					//{
					//	dni->mapped_node->prescribed_position = (1.0-ci->beta)*ci->super_parent_cluster->r*(dni->coordinate - ci->super_parent_cluster->original_center) + ci->super_parent_cluster->current_center;
					//}
					
				}
			}
			time_counter->StopCounter();
			//cout << time_counter->GetElapsedTime() << endl;
			time_counter->StartCounter();

			// updating target position
			level_list[n]->voxmesh_level->old_energy = 0.0;
			for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
			{
				ni->target_position.setZero();

				vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					ni->target_position += (*dn)->target_position;
				}
				
				ni->target_position /= double(ni->duplicates.size());
				if(ni->flag_constraint_node)
				{
					ni->target_position = ni->prescribed_position;
				}
				
				dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					(*dn)->target_position = ni->target_position;

					level_list[n]->voxmesh_level->old_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
					level_list[n]->voxmesh_level->old_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
					level_list[n]->voxmesh_level->old_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
				}
			}//for
		}
		time_counter->StopCounter();
		//cout << time_counter->GetElapsedTime() << endl;

		//else//if n == 0
		{
			//////////////////////////////////////////////////////////////////////////////////
			// level 0 to bottom+1, directly implementing shape matching
			//////////////////////////////////////////////////////////////////////////////////
			int iteration = level_list[n]->times_ShapeMatching;
			if(n < level_list.size())
			{
				for( int i = 0; i < iteration; i ++)
				//while(true)
				{
					vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
					for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
					{
						vector<DuplicatedNode>::const_iterator const_ni;

						ci->a_pq = Matrix3d::Zero();
						Vector3d p, q;
						/*
						if(i == 0 && ci->flag_constrained)
						{
							//inherit R, T from last level
							vector<DuplicatedNode>::iterator f_ni;
							for(f_ni=ci->node_list.begin(); f_ni!=ci->node_list.end(); ++f_ni)
							{
								Vector3d _force = f_ni->force + force_gravity + force_wind;
								f_ni->target_position = f_ni->target_position + time_step_size * time_step_size * _force * force_scalar;
								f_ni->mapped_node->prescribed_position = f_ni->target_position;
							}
						}
						*/
						ci->computeCurrentMassCentroid4Target();

						for(const_ni=ci->node_list.begin(); const_ni!=ci->node_list.end(); ++const_ni)
						{
							p = const_ni->target_position - ci->current_center;
							q = const_ni->coordinate - ci->original_center;
							ci->a_pq += const_ni->mass * p * q.transpose();
						}


						ci->a = ci->a_pq * ci->a_qq;
		

						JacobiSVD<Matrix3d> jacobi(ci->a_pq, ComputeFullU|ComputeFullV);
						ci->r = jacobi.matrixU() * jacobi.matrixV().transpose();
		
						num_PE_perTimeStep++;

						double det_a = det33(ci->a);

						vector<DuplicatedNode>::iterator dni;
						for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
						{
							dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center;
						
							if (!dni->mapped_node->flag_anchor_node)
							{
								//Vector3d _force = dni->force + force_gravity + force_wind;

								if (n == idx_bottom)
								{
									if(flag_dynamics)
									{
										if (i == iteration - 1)//last level, last iteration dealing dynamics
										{
											//dni->velocity = dni->velocity + (dni->target_position - dni->coordinate - dni->displacement) / (level_list.size() * time_step_size);
											//dni->velocity = (1.0-ci->kappa)*dni->velocity + ci->alpha*(dni->static_position - dni->target_position) / (level_list.size() * time_step_size);
											//dni->displacement += (level_list.size() * time_step_size)*dni->velocity;
											dni->velocity = (1.0-ci->kappa)*dni->velocity + ci->alpha*(dni->static_position - dni->coordinate - dni->displacement) / time_step_size;
											dni->displacement += time_step_size*dni->velocity;
											dni->target_position = dni->coordinate + dni->displacement;
										}
										else
											dni->target_position = dni->static_position;
									}
									else
									{
										dni->target_position = dni->static_position;
									}
								}
								else
								{
									dni->target_position = dni->static_position;
								}
								//if (dni->flag_constraint_node && n > 0 && i == 0)
									//dni->mapped_node->prescribed_position = dni->mapped_node->parent_node->prescribed_position;
									//dni->mapped_node->prescribed_position = (1.0-ci->beta)*ci->super_parent_cluster->r*(dni->coordinate - ci->super_parent_cluster->original_center) + ci->super_parent_cluster->current_center;
							}
						}


					}//for

					time_counter->StopCounter();
					//cout << time_counter->GetElapsedTime() << endl;
					time_counter->StartCounter();
					level_list[n]->voxmesh_level->new_energy = 0.0;
					// updating displacement
					for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
					{
						if (n == idx_bottom && i == iteration - 1)
						{
							if(flag_dynamics)
							{
								ni->displacement.setZero();
							}
						}
						ni->target_position.setZero();

						vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							if (n == idx_bottom && i == iteration - 1)
							{
								if(flag_dynamics)
								{
									ni->displacement += (*dn)->displacement;
								}
							}

							ni->target_position += (*dn)->target_position;
						}

						ni->target_position /= double(ni->duplicates.size());
						if( ni->flag_constraint_node  && i < iteration - 1)//?? if not including last iteration?
							ni->target_position = ni->prescribed_position;

						if (n == idx_bottom && i == iteration - 1)
						{
							if(flag_dynamics)
							{
								ni->displacement /= double(ni->duplicates.size());
							}
							else
							{
								ni->displacement = ni->target_position - ni->coordinate;
							}
						}

						dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							(*dn)->target_position = ni->target_position;

							if (n == idx_bottom && i == iteration - 1)
							{
								if(flag_dynamics)
								{
									(*dn)->displacement = ni->displacement;
								}			
								else
								{
									(*dn)->displacement = ni->displacement;
								}
							}

							level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
							level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
							level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
						}
					}//update for
					time_counter->StopCounter();
					//cout << time_counter->GetElapsedTime() << endl;
					/*
					VoxMesh * pVM = level_list[n]->voxmesh_level;
					double threshold = 0.0;
					double ratio = 0.0;
					ratio = getEnergyRatio(pVM);
					threshold = abs(pVM->new_energy - pVM->old_energy);
					cout << count << endl;
					cout << ratio << endl;
					if (ratio < energyThreshold && pVM->old_energy > pVM->new_energy)
					{
						pVM->old_energy = pVM->new_energy;
						cout << "break" << endl;
						break;
					}
					if (ratio <= 0.0000001 || n == idx_bottom)
					{
						pVM->old_energy = pVM->new_energy;
						break;
					}
					pVM->old_energy = pVM->new_energy;
					*/
				}//iteration
				//testoutput << n <<": " << count << endl;
			}//if(n < size)
			////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}//else

	}
	
	///////////////////////////////////////////////////////////////////////////
	//level 0 update by the last level, multigrid
	///////////////////////////////////////////////////////////////////////////
	
	if(level_list.size() > 1 && flag_multigrid)
	{

		///////////////////////////////////////////////////////////////////////////////////////
		//compute current position
		//vector<Cluster>::iterator ci = level_list[idx_bottom]->voxmesh_level->cluster_list.begin();
		//for (; ci != level_list[idx_bottom]->voxmesh_level->cluster_list.end(); ++ci)
		//{
		//	ci->computeCurrentMassCentroid();
		//}
		///////////////////////////////////////////////////////////////////////////////////////

		time_counter->StartCounter();
		int size_c = level_list[0]->voxmesh_level->cluster_list.size();
		for(int i_c = 0; i_c < size_c; i_c ++)
		{
			Cluster * m_pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];
			Vector3d translation = Vector3d::Zero();
			int sizeLeaf = m_pcluster->leaf_list.size() ;

			if (sizeLeaf > 1)
			{
				Quaterniond q(m_pcluster->leaf_list[0]->r);
				double t_add = 0.0;
				int count = 1;
				m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
				Vector3d t = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
				translation = (m_pcluster->leaf_parameter[0] / m_pcluster->term_normlize) * t;
				while(count < sizeLeaf)
				{
					m_pcluster->leaf_list[count]->computeCurrentMassCentroid4Target();
					Quaterniond q2(m_pcluster->leaf_list[count]->r);
					t_add += (m_pcluster->leaf_parameter[count-1] / m_pcluster->term_normlize);
					double temp = t_add / (t_add + (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize));
					Quaterniond qtemp = q2.slerp(temp, q);
					q = qtemp;

					t = m_pcluster->leaf_list[count]->current_center + m_pcluster->leaf_list[count]->r * (m_pcluster->original_center - m_pcluster->leaf_list[count]->original_center);
					translation += (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize) * t;

					count ++;
				}
				level_list[0]->voxmesh_level->cluster_list[i_c].r = q.toRotationMatrix();
			}
			else
			{
				m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
				level_list[0]->voxmesh_level->cluster_list[i_c].r = m_pcluster->leaf_list[0]->r;
				translation = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
			}


			//update static position and target position for this cluster
			//Cluster * pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];

			double det_a = det33(m_pcluster->a);
			m_pcluster->current_center = translation;
			vector<DuplicatedNode>::iterator dni;
			for (dni=m_pcluster->node_list.begin(); dni!=m_pcluster->node_list.end(); ++dni)
			{
				////dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*dni->coordinate + translation;
				dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*(dni->coordinate - m_pcluster->original_center) + m_pcluster->current_center;
				if (!dni->mapped_node->flag_anchor_node)
				{
					//Vector3d _force = dni->force + force_gravity + force_wind;
					dni->target_position = dni->static_position;
				}
			}
		}

		time_counter->StopCounter();
		//cout << time_counter->GetElapsedTime() << endl;
		time_counter->StartCounter();
		// updating displacement
		level_list[0]->voxmesh_level->old_energy = 0.0;
		for (node_iterator ni=level_list[0]->voxmesh_level->node_list.begin(); ni!=level_list[0]->voxmesh_level->node_list.end(); ++ni)
		{
			//ni->static_position.setZero();
			ni->target_position.setZero();

			vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				//ni->static_position += (*dn)->static_position;
				ni->target_position += (*dn)->target_position;
			}

			//ni->static_position /= double(ni->duplicates.size());
			ni->target_position /= double(ni->duplicates.size());
			//if(ni->flag_constraint_node)
			//	ni->target_position = ni->prescribed_position;

			dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				//if(!ni->flag_anchor_node)
					(*dn)->target_position = ni->target_position;

					level_list[0]->voxmesh_level->old_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
					level_list[0]->voxmesh_level->old_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
					level_list[0]->voxmesh_level->old_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
				//else
					//(*dn)->target_position = ni->coordinate;
			}
		}//cluster for
	}

	//surface interpolation
	int num_last = level_list.size()-1;
	for (node_iterator nmi=p_mesh->node_list.begin(); nmi!=p_mesh->node_list.end(); ++nmi)
	{
		nmi->displacement.setZero();
		for(int i = 0; i < 8; i++)
		{
			nmi->displacement += nmi->list_interpolation_nodes[i][level_display]->displacement * nmi->para_interpolate[i][level_display];
		}
	}

	//networking
	if(flag_network_ready && network_role == NETWORK_ROLE_SERVER)
	{
		for(int i = 0; i < pTransmitter.size(); i++)
		{
			if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_PC && pTransmitter[i]->isClientReady)
				pTransmitter[i]->HSMupdate(data4PC);
			else if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_MOBILE)
				pTransmitter[i]->HSMupdate(data4Mobile);
		}
		
	}
	////////////////////////////////////////////////////////////////////////////////////////////


	if(flag_setForce && !flag_setWindForce)
	{
		// compute the energy
		double match_energy = 0.0;
		for (int i=0; i< level_list[idx_bottom]->voxmesh_level->num_cluster; ++i)
		{
			for (int j=0; j<level_list[idx_bottom]->voxmesh_level->cluster_list[i].num_node; ++j)
			{
				match_energy += (level_list[idx_bottom]->voxmesh_level->cluster_list[i].node_list[j].static_position - level_list[idx_bottom]->voxmesh_level->cluster_list[i].node_list[j].target_position).norm();
			}
		}
		level_list[idx_bottom]->voxmesh_level->old_energy = level_list[idx_bottom]->voxmesh_level->new_energy;
		level_list[idx_bottom]->voxmesh_level->new_energy = match_energy;
		if(level_list[idx_bottom]->voxmesh_level->old_energy != 0)
		{
			double threshold = (level_list[idx_bottom]->voxmesh_level->new_energy - level_list[idx_bottom]->voxmesh_level->old_energy)/ level_list[idx_bottom]->voxmesh_level->old_energy;
			if(threshold < 0 && abs(threshold) < 0.01 && !flag_converge)
			{
				timestep_converge = time_step_index;
				flag_converge = true;
				cout << "TimeStep:" << time_step_index << " Converge!" << endl;
				cout << "Begin TimeStep: " << timestep_begin << endl;
				cout << "Converge TimeStep: " << timestep_converge << endl;
				cout << "Iteration #: " << timestep_converge - timestep_begin << endl;
				cout << "PE # per Iteration:" << num_PE_perTimeStep << endl;
			}
			myHEnergy << match_energy << endl;
		}
	}
	if(flag_exportTxt)
	{
		vector<Node>::iterator ni;
		for (ni = p_mesh->node_list.begin(); ni != p_mesh->node_list.end(); ++ni)
		{
			testoutput << "v " << ni->coordinate(0) + ni->displacement(0) 
				<< " " << ni->coordinate(1) + ni->displacement(1) 
				<< " " << ni->coordinate(2) + ni->displacement(2) << endl;
		}
	}
	++time_step_index;
	return true;
}
bool Kernel::simulateNextStep4HSMForce4StepFirst2()
{
	/////////////////////////////////////////////////////////////////////////////////
	//test whether quaternion is the same as rotation matrix.
	//Matrix3d r1 = level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].r;
	//Quaterniond test1(r1);
	//Matrix3d r2 = test1.toRotationMatrix();
	////////////////////////////////////////////////////////////////////////////////
	//test whether slerp could generate a reasonable result
	//double PI = 3.14159265;
	//Matrix3d r1 = Matrix3d::Identity();
	//r1(1, 1) = cos(0 * PI / 180);
	//r1(1, 2) = (-1) * sin(0 * PI / 180);
	//r1(2, 1) = sin(0 * PI / 180);
	//r1(2, 2) = cos(0 * PI / 180);

	//Matrix3d r2 = Matrix3d::Identity();
	//r2(1, 1) = cos(90 * PI / 180);
	//r2(1, 2) = (-1) * sin(90 * PI / 180);
	//r2(2, 1) = sin(90 * PI / 180);
	//r2(2, 2) = cos(90 * PI / 180);

	//Quaterniond test1(r1);
	//Quaterniond test2(r2);
	//Quaterniond test3 = test1.slerp(0.7, test2);
	//Matrix3d r3 = test3.toRotationMatrix();

	//// to save energy
	//flag_setForce = true;
	//const_force(0) = 0.15;
	//const_force(1) = 0.5;
	//const_force(2) = 0.0;
	

	double mass_sum = 0.0;
	num_PE_perTimeStep = 0;
	Vector3d force_gravity = Vector3d::Zero();
	Vector3d force_wind = Vector3d::Zero();
	//set gravity
	if(flag_gravity)
	{
		force_gravity(1) = gravity_magnitude;
	}
	if(flag_setWindForce)
	{
		srand(time(NULL));
		//wind_magnitude = (rand() % 100) * 0.01;
		wind_magnitude = 1;
		force_wind = const_force * sin((time_step_index % 360) * PI / 180) * wind_magnitude;
	}


	double PI = 3.14159265;
	int idx_bottom = level_list.size() - 1;

	for (int n = 0; n < level_list.size(); n++)
	{

		//force constraint
		if(flag_setForce && constraintType == Kernel::FORCE_CONSTRAINT)
		{
			
			if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
			{
				int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					level_list[n]->voxmesh_level->constraint_node_list[k]->force = const_force;

					for(int i=0; i < level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates.size(); ++i)
					{
						level_list[n]->voxmesh_level->constraint_node_list[k]->duplicates[i]->force = const_force;
					}
				}
			}
		}
		//position constraint
		if(flag_setForce && constraintType != Kernel::FORCE_CONSTRAINT)
		{
			if (!level_list[n]->voxmesh_level->constraint_node_list.empty())
			{
				int size_k = level_list[n]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					level_list[n]->voxmesh_level->constraint_node_list[k]->prescribed_position = const_force;
				}
			}
		}

		//deal with constraint clusters
		if(!level_list[n]->voxmesh_level->constraint_cluster_list.empty())
		{
			if (n > 0 && n < level_list.size() - 1)
			{
				vector<Cluster *>::iterator cci = level_list[n]->voxmesh_level->constraint_cluster_list.begin();
				for (; cci !=  level_list[n]->voxmesh_level->constraint_cluster_list.end(); cci++)
				{
					Vector3d translation = Vector3d::Zero();
					int sizeLeaf = (*cci)->leaf_list.size() ;

					if (sizeLeaf > 1)
					{
						Quaterniond q((*cci)->leaf_list[0]->r);
						double t_add = 0.0;
						int count = 1;
						(*cci)->leaf_list[0]->computeCurrentMassCentroid4Target();
						Vector3d t = (*cci)->leaf_list[0]->current_center + (*cci)->leaf_list[0]->r * ((*cci)->original_center - (*cci)->leaf_list[0]->original_center);
						translation = ((*cci)->leaf_parameter[0] / (*cci)->term_normlize) * t;
						while(count < sizeLeaf)
						{
							(*cci)->leaf_list[count]->computeCurrentMassCentroid4Target();
							Quaterniond q2((*cci)->leaf_list[count]->r);
							t_add += ((*cci)->leaf_parameter[count-1] / (*cci)->term_normlize);
							double temp = t_add / (t_add + ((*cci)->leaf_parameter[count] / (*cci)->term_normlize));
							Quaterniond qtemp = q2.slerp(temp, q);
							q = qtemp;

							t = (*cci)->leaf_list[count]->current_center + (*cci)->leaf_list[count]->r * ((*cci)->original_center - (*cci)->leaf_list[count]->original_center);
							translation += ((*cci)->leaf_parameter[count] / (*cci)->term_normlize) * t;

							count ++;
						}
						(*cci)->r = q.toRotationMatrix();
					}
					else
					{
						(*cci)->leaf_list[0]->computeCurrentMassCentroid4Target();
						(*cci)->r = (*cci)->leaf_list[0]->r;
						translation = (*cci)->leaf_list[0]->current_center + (*cci)->leaf_list[0]->r * ((*cci)->original_center - (*cci)->leaf_list[0]->original_center);
					}


					double det_a = det33((*cci)->a);
					(*cci)->current_center = translation;
					vector<DuplicatedNode>::iterator dni;
					for (dni=(*cci)->node_list.begin(); dni!=(*cci)->node_list.end(); ++dni)
					{
						dni->static_position = ((*cci)->beta*(*cci)->a/det_a + (1.0-(*cci)->beta)*(*cci)->r)*(dni->coordinate - (*cci)->original_center) + (*cci)->current_center;
						if (!dni->mapped_node->flag_anchor_node)
						{
							dni->target_position = dni->static_position;
						}
					}
				}
			}//if n < level_list.size() - 1
		}
		//deal with constraint points, only average
		if(!level_list[n]->voxmesh_level->constraint_node_list.empty())
		{
			vector<Node *>::iterator nni = level_list[n]->voxmesh_level->constraint_node_list.begin();
			for(; nni != level_list[n]->voxmesh_level->constraint_node_list.end(); nni++)
			{
				(*nni)->target_position.setZero();
				vector<DuplicatedNode*>::iterator dn = (*nni)->duplicates.begin();
				for (;dn!=(*nni)->duplicates.end();++dn)
				{
					(*nni)->target_position += (*dn)->target_position;
				}
				(*nni)->target_position /= double((*nni)->duplicates.size());

				dn = (*nni)->duplicates.begin();
				for (;dn!=(*nni)->duplicates.end();++dn)
				{
					//Vector3d _force = (*dn)->force + force_gravity + force_wind;
					(*dn)->target_position = (*nni)->target_position;// + time_step_size * time_step_size * _force * force_scalar;
					if (constraintType == Kernel::FORCE_CONSTRAINT)
						(*dn)->mapped_node->prescribed_position = (*dn)->target_position;
				}
			}
		}

		time_counter->StartCounter();
		//if level > 0, needs to inherit R, T from parent_level
		if(n > 0)
		{

			vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
			for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
			{
				//slerp to interpolate rotation q_c
				//linear interpolate translation t_ci(which is not used, so deleted)
				////////////////////////////////////////////////////////////////////////////////////////
				Quaterniond q_ci;
				vector<Quaterniond> list_q;
				vector<Vector3d> list_v;
				vector<double> t;
				double t_add = 0.0;
				Vector3d t_ci = Vector3d::Zero();

				for(int p = 0; p < 8; p++)
				{
					if(ci->vox_list[0]->list_near_parentVox[p])
					{
						Cluster * p_temp = &level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid];

						Quaterniond temp(p_temp->r);
						list_q.push_back(temp);
						
						p_temp->computeCurrentMassCentroid4Target();
						list_v.push_back(p_temp->r*(ci->original_center - p_temp->original_center) + p_temp->current_center);
						

						t.push_back(ci->vox_list[0]->para_interpolate[p]);
						
						ci->r = level_list[n-1]->voxmesh_level->cluster_list[ci->vox_list[0]->list_near_parentVox[p]->clusterid].r;
						t_ci = p_temp->r*(ci->original_center - p_temp->original_center) + p_temp->current_center;
					}
				}
				if(list_q.size() > 1)
				{
					q_ci = list_q[0];
					
					t_ci = t[0] * list_v[0];
					
					while(list_q.size() > 1)
					{
						t_add += t[0];
						double temp = t_add / (t_add + t[1]);
						Quaterniond qtemp = list_q[1].slerp(temp, q_ci);
						q_ci = qtemp;
						t_ci += t[1] * list_v[1];

						list_q.erase(list_q.begin());
						t.erase(t.begin());
						list_v.erase(list_v.begin());
					}
					ci->r = q_ci.toRotationMatrix();
				}


				double det_a = det33(ci->a);
				ci->current_center = t_ci;
				vector<DuplicatedNode>::iterator dni;
				for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
				{
					Vector3d _force = dni->force + force_gravity + force_wind;;

					dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center; // + t_ci;//ci->current_center;
					if (!dni->mapped_node->flag_anchor_node)
					{
						dni->target_position = dni->static_position;// + time_step_size * time_step_size * _force * force_scalar;
					}
					//if (dni->flag_constraint_node)
					//{
					//	dni->mapped_node->prescribed_position = (1.0-ci->beta)*ci->super_parent_cluster->r*(dni->coordinate - ci->super_parent_cluster->original_center) + ci->super_parent_cluster->current_center;
					//}
					
				}
			}
			time_counter->StopCounter();
			//cout << time_counter->GetElapsedTime() << endl;
			time_counter->StartCounter();

			// updating target position
			level_list[n]->voxmesh_level->old_energy = 0.0;
			for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
			{
				ni->target_position.setZero();

				vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					ni->target_position += (*dn)->target_position;
				}
				
				ni->target_position /= double(ni->duplicates.size());
				if(ni->flag_constraint_node)//no matter force constraint or position constraint
				{
					ni->target_position = ni->prescribed_position;
				}
				
				dn = ni->duplicates.begin();
				for (;dn!=ni->duplicates.end();++dn)
				{
					(*dn)->target_position = ni->target_position;

					level_list[n]->voxmesh_level->old_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
					level_list[n]->voxmesh_level->old_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
					level_list[n]->voxmesh_level->old_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
				}
			}//for
			cout << n << ": " << level_list[n]->voxmesh_level->old_energy << endl;
		}

		//else//if n == 0
		{
			//////////////////////////////////////////////////////////////////////////////////
			// level 0 to bottom+1, directly implementing shape matching
			//////////////////////////////////////////////////////////////////////////////////
			int iteration = level_list[n]->times_ShapeMatching;
			if(n < level_list.size())
			{
				iteration = 2;
				for( int i = 0; i < iteration; i ++)
				//while(true)
				{
					vector<Cluster>::iterator ci = level_list[n]->voxmesh_level->cluster_list.begin();
					double energyBeforeSM = 0.0;
					double energyAfterSM = 0.0;
					level_list[n]->voxmesh_level->new_energy = 0.0;
					for (; ci != level_list[n]->voxmesh_level->cluster_list.end(); ++ci)
					{
						vector<DuplicatedNode>::const_iterator const_ni;

						ci->a_pq = Matrix3d::Zero();
						Vector3d p, q;
						/*
						if(i == 0 && ci->flag_constrained)
						{
							//inherit R, T from last level
							vector<DuplicatedNode>::iterator f_ni;
							for(f_ni=ci->node_list.begin(); f_ni!=ci->node_list.end(); ++f_ni)
							{
								Vector3d _force = f_ni->force + force_gravity + force_wind;
								f_ni->target_position = f_ni->target_position + time_step_size * time_step_size * _force * force_scalar;
								f_ni->mapped_node->prescribed_position = f_ni->target_position;
							}
						}
						*/
						ci->computeCurrentMassCentroid4Target();

						for(const_ni=ci->node_list.begin(); const_ni!=ci->node_list.end(); ++const_ni)
						{
							p = const_ni->target_position - ci->current_center;
							q = const_ni->coordinate - ci->original_center;
							ci->a_pq += const_ni->mass * p * q.transpose();
						}


						ci->a = ci->a_pq * ci->a_qq;
		

						JacobiSVD<Matrix3d> jacobi(ci->a_pq, ComputeFullU|ComputeFullV);
						ci->r = jacobi.matrixU() * jacobi.matrixV().transpose();
		
						num_PE_perTimeStep++;

						double det_a = det33(ci->a);

						vector<DuplicatedNode>::iterator dni;
						for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
						{
							energyBeforeSM += pow((dni->static_position(0) - dni->target_position(0)), 2);
							energyBeforeSM += pow((dni->static_position(1) - dni->target_position(1)), 2);
							energyBeforeSM += pow((dni->static_position(2) - dni->target_position(2)), 2);

							dni->static_position = (ci->beta*ci->a/det_a + (1.0-ci->beta)*ci->r)*(dni->coordinate - ci->original_center) + ci->current_center;
						
							energyAfterSM += pow((dni->static_position(0) - dni->target_position(0)), 2);
							energyAfterSM += pow((dni->static_position(1) - dni->target_position(1)), 2);
							energyAfterSM += pow((dni->static_position(2) - dni->target_position(2)), 2);

							if (!dni->mapped_node->flag_anchor_node)
							{
								Vector3d _force = dni->force + force_gravity + force_wind;

								if (n == idx_bottom)
								{
									if(flag_dynamics)
									{
										if (i == iteration - 1)//last level, last iteration dealing dynamics
										{
											dni->velocity = (1.0-ci->kappa)*dni->velocity + ci->alpha*(dni->static_position - dni->coordinate - dni->displacement) / time_step_size 
												+ time_step_size*_force*force_scalar;
											dni->displacement += time_step_size*dni->velocity;
											dni->target_position = dni->coordinate + dni->displacement;
										}
										else
											dni->target_position = dni->static_position;
									}
									else
									{
										dni->target_position = dni->static_position;
									}
								}
								else
								{
									dni->target_position = dni->static_position;
								}
								
							}
						}


					}//for

					level_list[n]->voxmesh_level->new_energy = 0.0;
					// updating displacement
					for (node_iterator ni=level_list[n]->voxmesh_level->node_list.begin(); ni!=level_list[n]->voxmesh_level->node_list.end(); ++ni)
					{
						if (n == idx_bottom && i == iteration - 1)
						{
							if(flag_dynamics)
							{
								ni->displacement.setZero();
							}
						}
						ni->target_position.setZero();

						vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							if (n == idx_bottom && i == iteration - 1)
							{
								if(flag_dynamics)
								{
									ni->displacement += (*dn)->displacement;
								}
							}

							ni->target_position += (*dn)->target_position;
						}

						ni->target_position /= double(ni->duplicates.size());
						if( ni->flag_constraint_node  && constraintType != Kernel::FORCE_CONSTRAINT)//?? if not including last iteration?
							ni->target_position = ni->prescribed_position;
						if( ni->flag_constraint_node  && constraintType == Kernel::FORCE_CONSTRAINT && i < iteration - 1)//?? if not including last iteration?
							ni->target_position = ni->prescribed_position;

						if (n == idx_bottom && i == iteration - 1)
						{
							if(flag_dynamics)
							{
								ni->displacement /= double(ni->duplicates.size());
							}
							else
							{
								ni->displacement = ni->target_position - ni->coordinate;
							}
						}

						dn = ni->duplicates.begin();
						for (;dn!=ni->duplicates.end();++dn)
						{
							(*dn)->target_position = ni->target_position;

							if (n == idx_bottom && i == iteration - 1)
							{
								if(flag_dynamics)
								{
									(*dn)->displacement = ni->displacement;
								}			
								else
								{
									(*dn)->displacement = ni->displacement;
								}
							}

							level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
							level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
							level_list[n]->voxmesh_level->new_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
						}
					}//update for

					cout << "test old:" <<level_list[n]->voxmesh_level->old_energy << endl;
					cout << "before sm: "<< energyBeforeSM << endl;
					cout << "after sm: "<< energyBeforeSM << endl;
					cout << "test new: "<< level_list[n]->voxmesh_level->new_energy << endl;

					VoxMesh * pVM = level_list[n]->voxmesh_level;
					if (i == iteration - 1)
					{
						cout << "last break;" << endl;
						break;
					}

					double speed = 0.0;
					double ratio = 0.0;
					ratio = getEnergyRatio(pVM, speed);
					cout << "speed" << speed << endl;
					if (n != idx_bottom && speed < energyThreshold && pVM->old_energy > pVM->new_energy)
					{
						pVM->old_energy = pVM->new_energy;
						cout << "speed break" << endl;
						break;
					}
					else if (n != idx_bottom && pVM->old_energy <= pVM->new_energy)
					{
						pVM->old_energy = pVM->new_energy;
						cout << "normal break" << endl;
						break;
					}
					else if (n == idx_bottom && speed < energyThreshold && pVM->old_energy > pVM->new_energy)
					{
						pVM->old_energy = pVM->new_energy;
						cout << "speed break" << endl;
						i = iteration - 2;
					}
					else if (n == idx_bottom && pVM->old_energy <= pVM->new_energy)
					{
						pVM->old_energy = pVM->new_energy;
						cout << "normal break" << endl;
						i = iteration - 2;
					}
					else
					{
						pVM->old_energy = pVM->new_energy;
						iteration ++;
					}

				}//iteration
				//testoutput << n <<": " << count << endl;
			}//if(n < size)
			////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}//else

	}
	cout << endl;
	///////////////////////////////////////////////////////////////////////////
	//level 0 update by the last level, multigrid
	///////////////////////////////////////////////////////////////////////////
	
	if(level_list.size() > 1 && flag_multigrid)
	{

		///////////////////////////////////////////////////////////////////////////////////////
		//compute current position
		//vector<Cluster>::iterator ci = level_list[idx_bottom]->voxmesh_level->cluster_list.begin();
		//for (; ci != level_list[idx_bottom]->voxmesh_level->cluster_list.end(); ++ci)
		//{
		//	ci->computeCurrentMassCentroid();
		//}
		///////////////////////////////////////////////////////////////////////////////////////

		time_counter->StartCounter();
		int size_c = level_list[0]->voxmesh_level->cluster_list.size();
		for(int i_c = 0; i_c < size_c; i_c ++)
		{
			Cluster * m_pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];
			Vector3d translation = Vector3d::Zero();
			int sizeLeaf = m_pcluster->leaf_list.size() ;

			if (sizeLeaf > 1)
			{
				Quaterniond q(m_pcluster->leaf_list[0]->r);
				double t_add = 0.0;
				int count = 1;
				m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
				Vector3d t = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
				translation = (m_pcluster->leaf_parameter[0] / m_pcluster->term_normlize) * t;
				while(count < sizeLeaf)
				{
					m_pcluster->leaf_list[count]->computeCurrentMassCentroid4Target();
					Quaterniond q2(m_pcluster->leaf_list[count]->r);
					t_add += (m_pcluster->leaf_parameter[count-1] / m_pcluster->term_normlize);
					double temp = t_add / (t_add + (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize));
					Quaterniond qtemp = q2.slerp(temp, q);
					q = qtemp;

					t = m_pcluster->leaf_list[count]->current_center + m_pcluster->leaf_list[count]->r * (m_pcluster->original_center - m_pcluster->leaf_list[count]->original_center);
					translation += (m_pcluster->leaf_parameter[count] / m_pcluster->term_normlize) * t;

					count ++;
				}
				level_list[0]->voxmesh_level->cluster_list[i_c].r = q.toRotationMatrix();
			}
			else
			{
				m_pcluster->leaf_list[0]->computeCurrentMassCentroid4Target();
				level_list[0]->voxmesh_level->cluster_list[i_c].r = m_pcluster->leaf_list[0]->r;
				translation = m_pcluster->leaf_list[0]->current_center + m_pcluster->leaf_list[0]->r * (m_pcluster->original_center - m_pcluster->leaf_list[0]->original_center);
			}


			//update static position and target position for this cluster
			//Cluster * pcluster = &level_list[0]->voxmesh_level->cluster_list[i_c];

			double det_a = det33(m_pcluster->a);
			m_pcluster->current_center = translation;
			vector<DuplicatedNode>::iterator dni;
			for (dni=m_pcluster->node_list.begin(); dni!=m_pcluster->node_list.end(); ++dni)
			{
				////dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*dni->coordinate + translation;
				dni->static_position = (m_pcluster->beta*m_pcluster->a/det_a + (1.0-m_pcluster->beta)*m_pcluster->r)*(dni->coordinate - m_pcluster->original_center) + m_pcluster->current_center;
				if (!dni->mapped_node->flag_anchor_node)
				{
					//Vector3d _force = dni->force + force_gravity + force_wind;
					dni->target_position = dni->static_position;
				}
			}
		}

		time_counter->StopCounter();
		//cout << time_counter->GetElapsedTime() << endl;
		time_counter->StartCounter();
		// updating displacement
		level_list[0]->voxmesh_level->old_energy = 0.0;
		for (node_iterator ni=level_list[0]->voxmesh_level->node_list.begin(); ni!=level_list[0]->voxmesh_level->node_list.end(); ++ni)
		{
			//ni->static_position.setZero();
			ni->target_position.setZero();

			vector<DuplicatedNode*>::iterator dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				//ni->static_position += (*dn)->static_position;
				ni->target_position += (*dn)->target_position;
			}

			//ni->static_position /= double(ni->duplicates.size());
			ni->target_position /= double(ni->duplicates.size());
			if(ni->flag_constraint_node && constraintType != Kernel::FORCE_CONSTRAINT)
				ni->target_position = ni->prescribed_position;

			dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				//if(!ni->flag_anchor_node)
					(*dn)->target_position = ni->target_position;

					level_list[0]->voxmesh_level->old_energy += pow(((*dn)->static_position(0) - (*dn)->target_position(0)), 2);
					level_list[0]->voxmesh_level->old_energy += pow(((*dn)->static_position(1) - (*dn)->target_position(1)), 2);
					level_list[0]->voxmesh_level->old_energy += pow(((*dn)->static_position(2) - (*dn)->target_position(2)), 2);
				//else
					//(*dn)->target_position = ni->coordinate;
			}
		}//cluster for
		cout << "0: " << level_list[0]->voxmesh_level->old_energy << endl;
	}

	//surface interpolation
	int num_last = level_list.size()-1;
	for (node_iterator nmi=p_mesh->node_list.begin(); nmi!=p_mesh->node_list.end(); ++nmi)
	{
		nmi->displacement.setZero();
		for(int i = 0; i < 8; i++)
		{
			nmi->displacement += nmi->list_interpolation_nodes[i][num_last]->displacement * nmi->para_interpolate[i][num_last];
		}
	}

	//networking
	if(flag_network_ready && network_role == NETWORK_ROLE_SERVER)
	{
		for(int i = 0; i < pTransmitter.size(); i++)
		{
			if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_PC && pTransmitter[i]->isClientReady)
				pTransmitter[i]->HSMupdate(data4PC);
			else if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_MOBILE)
				pTransmitter[i]->HSMupdate(data4Mobile);
		}
		
	}
	////////////////////////////////////////////////////////////////////////////////////////////


	if(flag_setForce && !flag_setWindForce)
	{
		// compute the energy
		double match_energy = 0.0;
		for (int i=0; i< level_list[idx_bottom]->voxmesh_level->num_cluster; ++i)
		{
			for (int j=0; j<level_list[idx_bottom]->voxmesh_level->cluster_list[i].num_node; ++j)
			{
				match_energy += (level_list[idx_bottom]->voxmesh_level->cluster_list[i].node_list[j].static_position - level_list[idx_bottom]->voxmesh_level->cluster_list[i].node_list[j].target_position).norm();
			}
		}
		level_list[idx_bottom]->voxmesh_level->old_energy = level_list[idx_bottom]->voxmesh_level->new_energy;
		level_list[idx_bottom]->voxmesh_level->new_energy = match_energy;
		if(level_list[idx_bottom]->voxmesh_level->old_energy != 0)
		{
			double threshold = (level_list[idx_bottom]->voxmesh_level->new_energy - level_list[idx_bottom]->voxmesh_level->old_energy)/ level_list[idx_bottom]->voxmesh_level->old_energy;
			if(threshold < 0 && abs(threshold) < 0.01 && !flag_converge)
			{
				timestep_converge = time_step_index;
				flag_converge = true;
				cout << "TimeStep:" << time_step_index << " Converge!" << endl;
				cout << "Begin TimeStep: " << timestep_begin << endl;
				cout << "Converge TimeStep: " << timestep_converge << endl;
				cout << "Iteration #: " << timestep_converge - timestep_begin << endl;
				cout << "PE # per Iteration:" << num_PE_perTimeStep << endl;
			}
			myHEnergy << match_energy << endl;
		}
	}
	if(flag_exportTxt)
	{
		vector<Node>::iterator ni;
		for (ni = p_mesh->node_list.begin(); ni != p_mesh->node_list.end(); ++ni)
		{
			testoutput << "v " << ni->coordinate(0) + ni->displacement(0) 
				<< " " << ni->coordinate(1) + ni->displacement(1) 
				<< " " << ni->coordinate(2) + ni->displacement(2) << endl;
		}
	}
	++time_step_index;
	return true;
}
bool Kernel::simulateNextStep4PairMatching()
{ 
	if (p_vox_mesh->active_node)
	{
		for (int i=0; i<p_vox_mesh->active_node->duplicates.size(); ++i)
		{
			p_vox_mesh->active_node->duplicates[i]->target_position = p_vox_mesh->active_node->duplicates[i]->coordinate + p_vox_mesh->active_node->force; 
		}
	}

	vector<Cluster>::iterator ci;

	for (ci=p_vox_mesh->cluster_list.begin(); ci!=p_vox_mesh->cluster_list.end(); ++ci)
	{
		VectorXd b(3*ci->num_node);

		for (int i=0; i<ci->num_node; ++i)
		{
			Vector3d p = ci->node_list[i].static_position;

			b(3*i + 0) = ci->node_list[i].static_position(0) - ci->node_list[i].target_position(0);
			b(3*i + 1) = ci->node_list[i].static_position(1) - ci->node_list[i].target_position(1);
			b(3*i + 2) = ci->node_list[i].static_position(2) - ci->node_list[i].target_position(2);

			Matrix3d p_skew; 
			skew_symmetric(p, p_skew);

			ci->velocity_matrix.block<3, 3>(i*3, 0) = p_skew.transpose();
			ci->velocity_matrix.block<3, 3>(i*3, 3) = Matrix3d::Identity();
		}

		Matrix6d mat66 = ci->velocity_matrix.transpose() * ci->velocity_matrix;
		Vector6d x = mat66.ldlt().solve( ci->velocity_matrix.transpose() * -b );

		ci->angular_velocity(0) = x(0);
		ci->angular_velocity(1) = x(1);
		ci->angular_velocity(2) = x(2);

		ci->linear_velocity(0) = x(3);
		ci->linear_velocity(1) = x(4);
		ci->linear_velocity(2) = x(5);

		Vector3d delta_linear_displacement = ci->linear_velocity * time_step_size;
		Matrix3d delta_angular_displacement = Matrix3d::Zero();

		double angle = 0.5 * ci->angular_velocity.norm()*time_step_size;

		if (angle > 1e-3)
		{
			Matrix3d s;
			Vector3d axis_angle = ci->angular_velocity;
			axis_angle.normalize();
			skew_symmetric(axis_angle, s);
			delta_angular_displacement =  s*sin(angle) + s*s*(1.0 - cos(angle));
		}

		for (int i=0; i<ci->num_node; ++i)
		{
			ci->node_list[i].static_position += delta_linear_displacement + delta_angular_displacement*ci->node_list[i].static_position;
		}
	}

	// average at intersection and update target
	for (node_iterator ni=p_vox_mesh->node_list.begin(); ni!=p_vox_mesh->node_list.end(); ++ni)
	{
		ni->target_position.setZero();

		if (!ni->flag_anchor_node)
		{
			vector<DuplicatedNode*>::iterator dn;
			dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				ni->target_position += (*dn)->static_position;
			}

			ni->target_position /= double(ni->duplicates.size());

			dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				(*dn)->target_position = ni->target_position;
			}
		}

		else
		{
			ni->target_position = ni->coordinate;
		}
	}

	++time_step_index;
	return true;
}

bool Kernel::simulateNextStepForMobile()
{
	//networking
	if(flag_network_ready && network_role == NETWORK_ROLE_SERVER)
	{
		//for(int j = 0; j < 5; j++)
		{
			for(int i = 0; i < pTransmitter.size(); i++)
			{
				if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_MOBILE)
					pTransmitter[i]->HSMupdate2(data4Mobile);
			}
		}
		//Sleep(200);
	}

	if(myMobile.eof())
	{
		myMobile.clear();
		myMobile.seekg(0);
	}
	int i = 0;
	char line[1024];
	char * token;
	while(i < p_mesh->number_node)
	{
		if(myMobile.eof())
		{
			myMobile.clear();
			myMobile.seekg(0);
			break;
		}
		myMobile.getline(line, 1024);
		if(strlen(line) == 0) continue;
		token = strtok(line, " ");
		char vertex[] = "v";

		if(strlen(line) == 0)
			continue;
		else if(strcmp(token, vertex) == 0)
		{
			token = strtok(NULL, " ");
			p_mesh->node_list[i].displacement(0) = atof(token) - p_mesh->node_list[i].coordinate(0);

			token = strtok(NULL, " ");
			p_mesh->node_list[i].displacement(1) = atof(token) - p_mesh->node_list[i].coordinate(1);

			token = strtok(NULL, " ");
			p_mesh->node_list[i].displacement(2) = atof(token) - p_mesh->node_list[i].coordinate(2);
		
			i++;
		}
	}
	Sleep(timeSleep);
	
		//networking
	if(flag_network_ready && network_role == NETWORK_ROLE_SERVER)
	{
		//for(int j = 0; j < 5; j++)
		{
			for(int i = 0; i < pTransmitter.size(); i++)
			{
				if(pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_MOBILE)
					pTransmitter[i]->HSMupdate2(data4Mobile);
			}
		}
		//Sleep(200);
	}

	++time_step_index;
	return true;
}

bool Kernel::simulateNextStep4SingleGrid()
{
	if (p_vox_mesh->active_node)
	{
		for (int i=0; i<p_vox_mesh->active_node->duplicates.size(); ++i)
		{
			p_vox_mesh->active_node->duplicates[i]->target_position = p_vox_mesh->active_node->duplicates[i]->coordinate + p_vox_mesh->active_node->force; 
		}
	}

	// do the shape matching
	vector<Cluster>::iterator ci = p_vox_mesh->cluster_list.begin();
	for (; ci!=p_vox_mesh->cluster_list.end(); ++ci)
	{
		ci->current_center.setZero();
		vector<DuplicatedNode>::const_iterator const_ni;

		for(const_ni=ci->node_list.begin(); const_ni!=ci->node_list.end(); ++const_ni)
		{
			ci->current_center += const_ni->target_position;
		}

		ci->current_center /= double(ci->num_node);

		ci->a_pq = Matrix3d::Zero();
		Vector3d p, q;

		for(const_ni=ci->node_list.begin(); const_ni!=ci->node_list.end(); ++const_ni)
		{
			p = const_ni->target_position - ci->current_center;
			q = const_ni->coordinate - ci->original_center;
			ci->a_pq += p*q.transpose();
		}

		ci->a = ci->a_pq * ci->a_qq;

		JacobiSVD<Matrix3d> jacobi(ci->a_pq, ComputeFullU|ComputeFullV);
		ci->r = jacobi.matrixU() * jacobi.matrixV().transpose();

		vector<DuplicatedNode>::iterator dni;
		for (dni=ci->node_list.begin(); dni!=ci->node_list.end(); ++dni)
		{
			dni->static_position = (ci->r)*(dni->coordinate - ci->original_center) + ci->current_center;
		}
	}

	// average at intersection and update target
	for (node_iterator ni=p_vox_mesh->node_list.begin(); ni!=p_vox_mesh->node_list.end(); ++ni)
	{
		ni->target_position.setZero();

		if (!ni->flag_anchor_node)
		{
			vector<DuplicatedNode*>::iterator dn;
			dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				ni->target_position += (*dn)->static_position;
			}

			ni->target_position /= double(ni->duplicates.size());

			dn = ni->duplicates.begin();
			for (;dn!=ni->duplicates.end();++dn)
			{
				(*dn)->target_position = ni->target_position;
			}
		}

		else
		{
			ni->target_position = ni->coordinate;
		}
	}
	

	// compute the energy
	double match_energy = 0.0;
	for (int i=0; i<p_vox_mesh->num_cluster; ++i)
	{
		for (int j=0; j<p_vox_mesh->cluster_list[i].num_node; ++j)
		{
			match_energy += (p_vox_mesh->cluster_list[i].node_list[j].static_position - p_vox_mesh->cluster_list[i].node_list[j].target_position).norm();
		}
	}

	cout << match_energy << endl;
		
	++time_step_index;
	return true;
}

bool Kernel::simulateNextStep4Hierarchy()
{
	vector<Cluster*>::iterator ci = p_vox_mesh->root_to_leaf_cluster_list.begin();
	++ci;

	for (; ci!=p_vox_mesh->root_to_leaf_cluster_list.end(); ++ci)
	{
		(*ci)->inheritParentMotion();
					
		(*ci)->computeCurrentMassCentroid();

		(*ci)->a_pq = Matrix3d::Zero();
		Vector3d p, q;

		vector<DuplicatedNode>::const_iterator const_ni;
		for(const_ni=(*ci)->node_list.begin(); const_ni!=(*ci)->node_list.end(); ++const_ni)
		{
			p = const_ni->coordinate + const_ni->displacement - (*ci)->current_center;
			q = const_ni->coordinate - (*ci)->original_center;
			(*ci)->a_pq += p*q.transpose();
		}

		(*ci)->a = (*ci)->a_pq * (*ci)->a_qq;
		

		JacobiSVD<Matrix3d> jacobi((*ci)->a_pq, ComputeFullU|ComputeFullV);
		(*ci)->r = jacobi.matrixU() * jacobi.matrixV().transpose();
				
		//double det_a = pow((*ci)->a.determinant(), 1.0/3.0);

		vector<DuplicatedNode>::iterator dup;
		for (dup=(*ci)->node_list.begin(); dup!=(*ci)->node_list.end(); ++dup)
		{
			dup->static_position = (/*(*ci)->beta*(*ci)->a/det_a + (1.0-(*ci)->beta)**/(*ci)->r)*(dup->coordinate - (*ci)->original_center) + (*ci)->current_center;

			if (!dup->mapped_node->flag_anchor_node)
			{
				dup->velocity = (1.0-(*ci)->kappa)*dup->velocity + (*ci)->alpha*(dup->static_position - dup->coordinate - dup->displacement) / time_step_size
					+ time_step_size*dup->force*10.0;

				dup->displacement += time_step_size*dup->velocity;
			}
		}
	}

	for (node_iterator ni=p_vox_mesh->node_list.begin(); ni!=p_vox_mesh->node_list.end(); ++ni)
	{
		ni->displacement.setZero();
		ni->velocity.setZero();

		double depth_sum = 0;
		for (int i=0; i<ni->duplicates.size(); ++i)
		{
			ni->displacement += double(i)*(ni->duplicates[i]->displacement);
			ni->velocity += double(i)*(ni->duplicates[i]->velocity);
			depth_sum += i;
		}
		
		ni->displacement /= depth_sum;
		ni->velocity /= depth_sum;

		vector<DuplicatedNode*>::iterator dup = ni->duplicates.begin();
		for (;dup!=ni->duplicates.end();++dup)
		{
			(*dup)->displacement = ni->displacement;
			(*dup)->velocity = ni->velocity;
		}
	}

	++time_step_index;
	return true;
}

bool Kernel::simulateNextStepThroughNetworking()
{
	++time_step_index;
	return true;
}

bool Kernel::simulateNextStep()
{
	switch (used_simulator)
	{
	case UNDEFINED:
		return false;
		break;

	case SHAPE_MATCHING:
		return simulateNextStep4ShapeMatching();
		break;

	case VELOCITY_MATCHING:
		return simulateNextStep4VelocityMatching();
		break;

	case PAIR_MATCHING:
		return simulateNextStep4PairMatching();
		break;
		 
	case SINGLE_GRID:
		return simulateNextStep4SingleGrid();
		break;

	case HIERARCHY:
		return simulateNextStep4Hierarchy();
		break;

	case MULTIPLE_VELOCITY_MATCHING:
		return simulateNextStep4MultipleVelocityMatching();
		break;
	case SIMULATION_NETWORKING:
		return simulateNextStepThroughNetworking();
		break;
	case SIMULATION_MOBILE:
		return simulateNextStepForMobile();
		break;
	case HSM_FORCE4ITERATION:
		return simulateNextStep4HSMForce4Iteration();
		break;
	case HSM_ORIGINAL:
		return simulateNextStep4HSMOriginal();
	case HSM_ONE_STEP:
		return simulateNextStep4HSMOneStep();
	case HSM_FORCE4STEP:
		return simulateNextStep4HSMForce4Step();
	case HSM_ADAPTIVE_STEP:
		return simulateNextStep4HSMAdaptiveStep();
	case HSM_FORCE4STEP_FIRST:
		return simulateNextStep4HSMForce4StepFirst();
	case HSM_FORCE4STEP_FIRST1:
		return simulateNextStep4HSMForce4StepFirst1();
	case HSM_FORCE4STEP_FIRST2:
		return simulateNextStep4HSMForce4StepFirst2();
	default:
		return false;
		break;
	}
}

double Kernel::det33(Matrix3d& a)
{
	return (a(0,0)*a(1,1)*a(2,2)+a(0,1)*a(1,2)*a(2,0)+a(1,0)*a(2,1)*a(0,2)-a(0,0)*a(1,2)*a(2,1)-a(0,1)*a(1,0)*a(2,2)-a(0,2)*a(1,1)*a(2,0));
}

void Kernel::skew_symmetric(Vector3d& a, Matrix3d& m33)
{
	m33.setZero();
	m33(0, 1) = -a(2); m33(0, 2) = a(1);
	m33(1, 0) = a(2);  m33(1, 2) = -a(0);
	m33(2, 0) = -a(1); m33(2, 1) = a(0);
}

void Kernel::clearLevel(int i)
{
	if(i < level_list.size() && i > -1)
	{
		for(int j = i; j < level_list.size(); j++)
		{
			Level * temp = level_list[j];
			level_list[j] = NULL;
			delete temp;
		}
		level_list.erase(level_list.begin() + i, level_list.end());
	}
	else
		cout << "Error! Index of Level is out of range!" << endl;
}

void Kernel::clearAllLevel()
{
	//set show level to be 0
	level_display = 0;

	if(level_list.size() < 1)
		return;
	//Level 0
	if(level_list.size() > 1)
	{
		clearLevel(1);
	}
}

void Kernel::addLevel()
{
	Level * p_level = new Level;
	p_level->gridDensity = 1;
	p_level->level_index = level_list.size();
	level_list.push_back(p_level);
}

void Kernel::generateVoxMesh4Level(int level, int d)
{
	if(level == 0)
	{
		grid_density = d;

		if (p_voxel)
		{
			delete [] p_voxel;
			p_voxel = NULL;
		}
	
		cout << "allocating memory for new grid information...";
		p_voxel = new unsigned int[d*d*d];
		memset(p_voxel, 0, sizeof(unsigned int)*d * d * d);
		cout << "done" << endl;

		cout << "marking voxels corresponding to mesh...";
		num_sim_vox = 0;
		num_surface_vox = 0;
		num_sim_node = 0;
		mark_preprocess4Voxel(p_mesh, p_voxel, grid_density);
		cout << "done" << endl;

		//for(int i = 0; i < d*d*d; i++)
		//	cout << i << ": " << p_voxel[i] << " ";
		//cout << endl;

		cout <<"filling the interior voxels...";
		mark_fillInterior4Voxel(p_voxel, grid_density);
		cout <<"done" << endl;

		cout << "marking surface voxel...";
		mark_markSurfaceVox4Voxel(p_voxel, grid_density);
		cout << "done" << endl;

		cout <<"generating voxel mesh...";
		generateVoxMesh(p_voxel, d, p_vox_mesh);
		cout << "done" << endl;

		num_sim_node = p_vox_mesh->num_node;
		flag_vox_ready = true;

		cout << "generate per-voxel cluster...";
		generatePerVoxCluster(p_vox_mesh);
		cout << "done" << endl;

		cout << "initialize cluster hierarchy...";
		p_vox_mesh->initializeClusterHierarchy();
		cout << "done" << endl;
	
		//initialize level
		cout << "initialize level information...";
		level_list[0]->gridDensity = d;
		level_list[0]->gridDensity_total = d;
		level_list[0]->num_sim_vox = num_sim_vox;
		level_list[0]->num_surface_vox = num_surface_vox;
		level_list[0]->num_sim_node = num_sim_node;
		cout << "done" << endl;

		
		cout << "linking mesh and volumetric mesh...";
		linkMesh_VolMesh(p_mesh, p_vox_mesh, d);
		cout << "done" << endl;
		//link also
		//if(p_mesh->mesh_simplified)
			//linkMesh_VolMesh(p_mesh->mesh_simplified, p_vox_mesh, d);
		
	}
	else{
		int parent_index = level - 1;
		if(level_list[parent_index]->voxmesh_level == NULL)
		{
			cout << "Error! The grid density of Level " << parent_index << " has not been set yet!" << endl;
			return;
		}
		
		level_list[level]->gridDensity = d;
		level_list[level]->gridDensity_total = level_list[parent_index]->gridDensity_total * d;


		if (p_voxel)
		{
			delete [] p_voxel;
			p_voxel = NULL;
		}
	
		cout << "allocating memory for new grid information for level " << level << "...";
		int size_temp = level_list[level]->gridDensity_total * level_list[level]->gridDensity_total * level_list[level]->gridDensity_total;
		p_voxel = new unsigned int[size_temp];
		memset(p_voxel, 0, sizeof(unsigned int)*size_temp);
		cout << "done" << endl;

		cout << "marking voxels corresponding to mesh for level " << level << "...";
		mark_preprocess4VoxelLevel(p_mesh, p_voxel, d, level_list[parent_index]->gridDensity_total);
		cout << "done" << endl;
		
		cout <<"filling the interior voxels for level " << level << "...";
		mark_fillInterior4VoxelLevel(p_voxel, d, level_list[parent_index]->gridDensity_total);
		cout <<"done" << endl;
		
		
		if(!p_voxel)
			delete []p_voxel;

		//for(int i = 0; i < size_temp; i++)
		//	cout << i << ": " << p_voxel[i] << " ";
		//cout << endl;

		
		
		cout << "marking surface voxel for level " << level << "...";
		mark_markSurfaceVox4Voxel(p_voxel, level_list[level]->gridDensity_total);
		cout << "done" << endl;

			
		//generate one volmesh for current level from parent level.
		VoxMesh * v_mesh;
		int num_sur_vox = (d > 1)? (8 + 12 * (d - 2) + 6 * (d - 2) * (d - 2)):1;
		num_sur_vox = num_sur_vox * level_list[parent_index]->voxmesh_level->num_cluster;
		if(level_list[level]->voxmesh_level!= NULL)
		{
			v_mesh = level_list[level]->voxmesh_level;	
			v_mesh->clear();
			int num_vox = level_list[parent_index]->voxmesh_level->num_vox * d * d * d;
			v_mesh->vox_list.reserve(num_vox);
			v_mesh->node_list.reserve(num_vox * 8);
			v_mesh->surface_vox_list.reserve(num_sur_vox);
			v_mesh->surface_node_list.reserve(num_sur_vox * 8);
			int size = level_list[parent_index]->voxmesh_level->vox_locator.size();
			int num_vox_locator=  size * d * d * d;
			v_mesh->vox_locator.assign(num_vox_locator, NULL);
			cout << "num of vox in locator:" << num_vox_locator << endl;

			v_mesh->num_node = 0;
			v_mesh->num_vox = 0;
			v_mesh->num_surface_vox = 0;
			v_mesh->num_surface_node = 0;
		}
		else
		{
			v_mesh = new VoxMesh;
			int num_vox = level_list[parent_index]->voxmesh_level->num_vox * d * d * d;
			v_mesh->vox_list.reserve(num_vox);
			v_mesh->node_list.reserve(num_vox * 8);
			v_mesh->surface_vox_list.reserve(num_sur_vox);
			v_mesh->surface_node_list.reserve(num_sur_vox * 8);
			int size = level_list[parent_index]->voxmesh_level->vox_locator.size();
			int num_vox_locator=  size  * d * d * d;
			v_mesh->vox_locator.assign(num_vox_locator, NULL);
			cout << "num of vox in locator:" << num_vox_locator << endl;

			v_mesh->num_node = 0;
			v_mesh->num_vox = 0;
			v_mesh->num_surface_vox = 0;
			v_mesh->num_surface_node = 0;

			level_list[level]->voxmesh_level = v_mesh;
			cout << p_vox_mesh->cluster_list[p_vox_mesh->vox_list[0].clusterid].num_node << endl;
		}
				
		generateVoxMeshFromParentMesh(p_voxel, v_mesh, level_list[parent_index]->voxmesh_level, d, level_list[parent_index]->gridDensity_total, level_list[parent_index]->gridDensity_total);
		//num_sim_node = p_vox_mesh->num_node;
		flag_vox_ready = true;

		generatePerVoxCluster4Level(v_mesh, level_list[parent_index]->voxmesh_level);
		linkMesh_VolMeshFromParentMesh(p_mesh, v_mesh, level_list[parent_index]->voxmesh_level, d, level_list[parent_index]->gridDensity_total, parent_index);
		//if(p_mesh->mesh_simplified)
			//linkMesh_VolMeshFromParentMesh(p_mesh->mesh_simplified, v_mesh, level_list[parent_index]->voxmesh_level, d, level_list[parent_index]->gridDensity_total, parent_index);
		//cout << level_list[level]->voxmesh_level->num_node << endl;

		level_list[level]->num_sim_vox = num_sim_vox;
		level_list[level]->num_surface_vox = num_surface_vox;
		level_list[level]->num_sim_node = level_list[level]->voxmesh_level->num_node;
		
	}
}


void Kernel::generateVoxMeshFromParentMesh(const unsigned int* vol, VoxMesh * &v_mesh, VoxMesh* &parent_mesh, int d, int d_parent, int d_total)// d_parent and d_total are the same.
{
	v_mesh->clear();
	v_mesh->vox_list.reserve(num_sim_vox);
	v_mesh->node_list.reserve(num_sim_vox * 8);
	v_mesh->surface_vox_list.reserve(num_surface_vox);
	v_mesh->surface_node_list.reserve(num_surface_vox * 8);
	v_mesh->vox_locator.assign(d_total*d_total*d_total*d*d*d, NULL);

	v_mesh->num_node = 0;
	v_mesh->num_vox = 0;
	v_mesh->num_surface_vox = 0;
	v_mesh->num_surface_node = 0;


	//to test error cluster
	int count_cluster = 0;
	
	double half_size = parent_mesh->cluster_list[0].vox_list[0]->half_size / double(d);
	//for every cluster save its coordinates for grid
	for(int i = 0; i < parent_mesh->num_cluster; i ++)
	{
		Vox * parent_Vox = parent_mesh->cluster_list[i].vox_list[0];
		double center_x = parent_Vox->vox_center(0);
		double center_y = parent_Vox->vox_center(1);
		double center_z = parent_Vox->vox_center(2);

		int parent_index = parent_Vox->coord_grid[0]*d_total*d_total + parent_Vox->coord_grid[1]*d_total + parent_Vox->coord_grid[2];
		int index = 0;

		//all voxels are clusters
		for (int z = 0; z < d; ++z)
		{
			for (int y = 0; y < d; ++y)
			{
				for (int x = 0; x < d; ++x)
				{
					if (vol[parent_index *d*d*d + z*d*d + y*d + x] != 2)
					{
					// a new voxel anyway
					Vox new_vox;
					new_vox.half_size = half_size;
					new_vox.parent_vox = &(*parent_mesh->cluster_list[i].vox_list[0]);
					//cout << new_vox.parent_vox->clusterid << endl;
					new_vox.coord_grid(0) = parent_Vox->coord_grid[0]*d + z;
					new_vox.coord_grid(1) = parent_Vox->coord_grid[1]*d + y;
					new_vox.coord_grid(2) = parent_Vox->coord_grid[2]*d + x;
					new_vox.vox_center(0) = double(new_vox.coord_grid(2))*half_size*2.0 + half_size - 0.5;
					new_vox.vox_center(1) = double(new_vox.coord_grid(1))*half_size*2.0 + half_size - 0.5;
					new_vox.vox_center(2) = double(new_vox.coord_grid(0))*half_size*2.0 + half_size - 0.5;
					
					if(count_cluster == 425 || count_cluster == 1340 ||count_cluster == 1482 ||count_cluster == 1533)
						count_cluster = count_cluster + 0;

					//if(parent_index == 363 && x == 7 && y == 3 && z == 0)
					//	count_cluster += 0;

					connectNewVox4VoxMesh(&new_vox, parent_Vox, v_mesh, parent_index, d, d_total, x, y, z);
					

					// create node
					if (new_vox.node_0 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) -= half_size;
						new_node.coordinate(1) += half_size;
						new_node.coordinate(2) -= half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_0 = &v_mesh->node_list.back();
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;							
						//if(count_cluster == 80)
						//	cout << "node 0 created!" << endl;
					}

					if (new_vox.node_1 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) += half_size;
						new_node.coordinate(1) += half_size;
						new_node.coordinate(2) -= half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_1 = &v_mesh->node_list.back();		
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
						//if(count_cluster == 80)
						//	cout << "node 1 created!" << endl;
					}

					if (new_vox.node_2 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) += half_size;
						new_node.coordinate(1) += half_size;
						new_node.coordinate(2) += half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_2 = &v_mesh->node_list.back();		
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
						//if(count_cluster == 80)
						//	cout << "node 2 created!" << endl;
					}

					if (new_vox.node_3 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) -= half_size;
						new_node.coordinate(1) += half_size;
						new_node.coordinate(2) += half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_3 = &v_mesh->node_list.back();	
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
						//if(count_cluster == 80)
						//	cout << "node 3 created!" << endl;
					}

					if (new_vox.node_4 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) -= half_size;
						new_node.coordinate(1) -= half_size;
						new_node.coordinate(2) -= half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_4 = &v_mesh->node_list.back();		
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
						//if(count_cluster == 80)
						//	cout << "node 4 created!" << endl;
					}

					if (new_vox.node_5 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) += half_size;
						new_node.coordinate(1) -= half_size;
						new_node.coordinate(2) -= half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_5 = &v_mesh->node_list.back();	
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
						//if(count_cluster == 80)
						//	cout << "node 5 created!" << endl;
					}

					if (new_vox.node_6 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) += half_size;
						new_node.coordinate(1) -= half_size;
						new_node.coordinate(2) += half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_6 = &v_mesh->node_list.back();	
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
						//if(count_cluster == 80)
						//	cout << "node 6 created!" << endl;
					}

					if (new_vox.node_7 == NULL)
					{
						Node new_node;
						new_node.coordinate = new_vox.vox_center;
						new_node.coordinate(0) -= half_size;
						new_node.coordinate(1) -= half_size;
						new_node.coordinate(2) += half_size;
						new_node.static_position = new_node.coordinate;
						new_node.target_position = new_node.coordinate;
						v_mesh->node_list.push_back(new_node);
						new_vox.node_7 = &v_mesh->node_list.back();	
						v_mesh->node_list.back().idx = v_mesh->node_list.size() - 1;
						//if(count_cluster == 80)
						//	cout << "node 7 created!" << endl;
					}

					// detect render surface
					if (vol[parent_index*d*d*d + z*d*d + y*d + x] == 3)
					{
						new_vox.flag_surface_vox = true;
						if (x == 0)
						{
							new_vox.flag_left_face = true;
							new_vox.node_0->flag_surface_node = true;
							new_vox.node_4->flag_surface_node = true;
							new_vox.node_7->flag_surface_node = true;
							new_vox.node_3->flag_surface_node = true;
						}
						else if(vol[parent_index *d*d*d + z*d*d + y*d + x-1] == 2)
						{
							new_vox.flag_left_face = true;
							new_vox.node_0->flag_surface_node = true;
							new_vox.node_4->flag_surface_node = true;
							new_vox.node_7->flag_surface_node = true;
							new_vox.node_3->flag_surface_node = true;
						}
						if (x == d-1)
						{
							new_vox.flag_right_face = true;
							new_vox.node_1->flag_surface_node = true;
							new_vox.node_5->flag_surface_node = true;
							new_vox.node_2->flag_surface_node = true;
							new_vox.node_6->flag_surface_node = true;
						}
						else if (vol[parent_index *d*d*d + z*d*d + y*d + x+1] == 2)
						{
							new_vox.flag_right_face = true;
							new_vox.node_1->flag_surface_node = true;
							new_vox.node_5->flag_surface_node = true;
							new_vox.node_2->flag_surface_node = true;
							new_vox.node_6->flag_surface_node = true;
						}
						if (y == 0)
						{
							new_vox.flag_bottom_face = true;
							new_vox.node_4->flag_surface_node = true;
							new_vox.node_5->flag_surface_node = true;
							new_vox.node_7->flag_surface_node = true;
							new_vox.node_6->flag_surface_node = true;
						}
						else if (vol[parent_index *d*d*d + z*d*d + (y-1)*d + x] == 2)
						{
							new_vox.flag_bottom_face = true;
							new_vox.node_4->flag_surface_node = true;
							new_vox.node_5->flag_surface_node = true;
							new_vox.node_7->flag_surface_node = true;
							new_vox.node_6->flag_surface_node = true;
						}
						if (y == d-1)
						{
							new_vox.flag_top_face = true;
							new_vox.node_0->flag_surface_node = true;
							new_vox.node_1->flag_surface_node = true;
							new_vox.node_2->flag_surface_node = true;
							new_vox.node_3->flag_surface_node = true;
						}
						else if (vol[parent_index *d*d*d + z*d*d + (y+1)*d + x] == 2)
						{
							new_vox.flag_top_face = true;
							new_vox.node_0->flag_surface_node = true;
							new_vox.node_1->flag_surface_node = true;
							new_vox.node_2->flag_surface_node = true;
							new_vox.node_3->flag_surface_node = true;
						}
						if (z == 0)
						{
							new_vox.flag_back_face = true;
							new_vox.node_0->flag_surface_node = true;
							new_vox.node_1->flag_surface_node = true;
							new_vox.node_4->flag_surface_node = true;
							new_vox.node_5->flag_surface_node = true;
						}
						else if (vol[parent_index *d*d*d + (z-1)*d*d + y*d + x] == 2)
						{
							new_vox.flag_back_face = true;
							new_vox.node_0->flag_surface_node = true;
							new_vox.node_1->flag_surface_node = true;
							new_vox.node_4->flag_surface_node = true;
							new_vox.node_5->flag_surface_node = true;
						}
						if (z == d-1)
						{
							new_vox.flag_front_face = true;
							new_vox.node_2->flag_surface_node = true;
							new_vox.node_3->flag_surface_node = true;
							new_vox.node_6->flag_surface_node = true;
							new_vox.node_7->flag_surface_node = true;
						}						
						else if (vol[parent_index *d*d*d + (z+1)*d*d + y*d + x] == 2)
						{
							new_vox.flag_front_face = true;
							new_vox.node_2->flag_surface_node = true;
							new_vox.node_3->flag_surface_node = true;
							new_vox.node_6->flag_surface_node = true;
							new_vox.node_7->flag_surface_node = true;
						}
					}
					// insert new voxel
					v_mesh->vox_list.push_back(new_vox);
					v_mesh->vox_locator[parent_index *d*d*d + z*d*d + y*d + x] = &v_mesh->vox_list.back(); 
					//int a = parent_index *d*d*d + z*d*d + y*d + x;
					//if(d == 2 && d_parent == 2 && z == 1 && y == 1 && x == 0)
					//	cout << a << endl;


					if (new_vox.flag_surface_vox)
					{
						v_mesh->surface_vox_list.push_back(&v_mesh->vox_list.back());
					}
					
					//connect same-position nodes between parent vox and this vox
					if(x == 0 && y == d-1 && z == 0 )
					{
						//parent_Vox->node_0_child = v_mesh->vox_list.back().node_0;
						parent_Vox->node_0->child_node = v_mesh->vox_list.back().node_0;
						v_mesh->vox_list.back().node_0->parent_node = parent_Vox->node_0;
					}
					if(x == d-1 && y == d-1 && z == 0 )
					{
						parent_Vox->node_1->child_node = v_mesh->vox_list.back().node_1;
						v_mesh->vox_list.back().node_1->parent_node = parent_Vox->node_1;
					}
					if(x == d-1 && y == d-1 && z == d-1 )
					{
						parent_Vox->node_2->child_node = v_mesh->vox_list.back().node_2;
						v_mesh->vox_list.back().node_2->parent_node = parent_Vox->node_2;
					}
					if(x == 0 && y == d-1 && z == d-1 )
					{
						parent_Vox->node_3->child_node = v_mesh->vox_list.back().node_3;
						v_mesh->vox_list.back().node_3->parent_node = parent_Vox->node_3;
					}
					if(x == 0 && y == 0 && z == 0 )
					{
						parent_Vox->node_4->child_node = v_mesh->vox_list.back().node_4;
						v_mesh->vox_list.back().node_4->parent_node = parent_Vox->node_4;
					}
					if(x == d-1 && y == 0 && z == 0 )
					{
						parent_Vox->node_5->child_node = v_mesh->vox_list.back().node_5;
						v_mesh->vox_list.back().node_5->parent_node = parent_Vox->node_5;
					}
					if(x == d-1 && y == 0 && z == d-1 )
					{
						parent_Vox->node_6->child_node = v_mesh->vox_list.back().node_6;
						v_mesh->vox_list.back().node_6->parent_node = parent_Vox->node_6;
					}
					if(x == 0 && y == 0 && z == d-1 )
					{
						parent_Vox->node_7->child_node = v_mesh->vox_list.back().node_7;
						v_mesh->vox_list.back().node_7->parent_node = parent_Vox->node_7;
					}
					count_cluster ++;
					//if(count_cluster == 55)
					//	cout << "bingo" << endl;
					
					double x_cube = 0.0;
					double y_cube = 0.0;
					double z_cube = 0.0;
					x_cube = parent_Vox->vox_center(0) - v_mesh->vox_list.back().vox_center(0);
					y_cube = parent_Vox->vox_center(1) - v_mesh->vox_list.back().vox_center(1);
					z_cube = parent_Vox->vox_center(2) - v_mesh->vox_list.back().vox_center(2);
					double line_cube = parent_Vox->half_size * 2;
					double vol_cube = pow(line_cube , 3);
					double area_cube = pow(line_cube, 2);
					vector<double> d_cluster;
					//double side_cube = parent_Vox->half_size * 2;
					
					//compute nearest parent vox list and parameters
					//if centers are equal, then paramenter set to be 1 and continue
					if(parent_Vox->vox_center == v_mesh->vox_list.back().vox_center)
					{
						v_mesh->vox_list.back().para_interpolate[0] = 1.0;
						v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
					}
					//zone 0, 8 neighbors
					else if(x_cube != 0 && y_cube != 0 && z_cube != 0)
					{
						if(x_cube > 0 && y_cube > 0 && z_cube > 0)
						{
							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->bottom_left_back_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->bottom_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->left_back_neighbor;
													
							v_mesh->vox_list.back().list_near_parentVox[4] = parent_Vox->back_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[5] = parent_Vox->bottom_left_neighbor;
							
							v_mesh->vox_list.back().list_near_parentVox[6] = parent_Vox->left_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[7] = parent_Vox->bottom_back_neighbor;
						}
						//zone 1, 8 neighbors
						else if(x_cube < 0 && y_cube > 0 && z_cube > 0)
						{
							x_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->bottom_right_back_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->bottom_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->right_back_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[4] = parent_Vox->back_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[5] = parent_Vox->bottom_right_neighbor;
							
							v_mesh->vox_list.back().list_near_parentVox[6] = parent_Vox->right_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[7] = parent_Vox->bottom_back_neighbor;
						}
						//zone 2, 8 neighbors
						else if(x_cube > 0 && y_cube < 0 && z_cube > 0)
						{
							y_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->top_left_back_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->top_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->left_back_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[4] = parent_Vox->back_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[5] = parent_Vox->top_left_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[6] = parent_Vox->left_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[7] = parent_Vox->top_back_neighbor;
						}
						//zone 3, 8 neighbors
						else if(x_cube < 0 && y_cube < 0 && z_cube > 0)
						{
							x_cube *= (-1);
							y_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->top_right_back_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->top_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->right_back_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[4] = parent_Vox->back_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[5] = parent_Vox->top_right_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[6] = parent_Vox->right_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[7] = parent_Vox->top_back_neighbor;
						}
						//zone 4, 8 neighbors
						else if(x_cube > 0 && y_cube > 0 && z_cube < 0)
						{
							z_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->bottom_left_front_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->bottom_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->left_front_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[4] = parent_Vox->front_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[5] = parent_Vox->bottom_left_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[6] = parent_Vox->left_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[7] = parent_Vox->bottom_front_neighbor;
						}
						//zone 5, 8 neighbors
						else if(x_cube < 0 && y_cube > 0 && z_cube < 0)
						{
							x_cube *= (-1);
							z_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->bottom_right_front_neighbor;
							
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->bottom_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->right_front_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[4] = parent_Vox->front_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[5] = parent_Vox->bottom_right_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[6] = parent_Vox->right_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[7] = parent_Vox->bottom_front_neighbor;
						}
						//zone 6, 8 neighbors
						else if(x_cube > 0 && y_cube < 0 && z_cube < 0)
						{
							y_cube *= (-1);
							z_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->top_left_front_neighbor;
							
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->top_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->left_front_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[4] = parent_Vox->front_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[5] = parent_Vox->top_left_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[6] = parent_Vox->left_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[7] = parent_Vox->top_front_neighbor;
						}
						//zone 7, 8 neighbors
						else if(x_cube < 0 && y_cube < 0 && z_cube < 0)
						{
							x_cube *= (-1);
							y_cube *= (-1);
							z_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->top_right_front_neighbor;
							
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->top_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->right_front_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[4] = parent_Vox->front_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[5] = parent_Vox->top_right_neighbor;

							v_mesh->vox_list.back().list_near_parentVox[6] = parent_Vox->right_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[7] = parent_Vox->top_front_neighbor;
						}
						//if(v_mesh->vox_list.back().para_interpolate[0] && v_mesh->vox_list.back().para_interpolate[1]
						//&& v_mesh->vox_list.back().para_interpolate[2] && v_mesh->vox_list.back().para_interpolate[3]
						//&& v_mesh->vox_list.back().para_interpolate[4] && v_mesh->vox_list.back().para_interpolate[5]
						//&& v_mesh->vox_list.back().para_interpolate[6] && v_mesh->vox_list.back().para_interpolate[7])
						//{
						//	v_mesh->vox_list.back().para_interpolate[0] = ((line_cube -  x_cube) * (line_cube -  y_cube) * (line_cube -  z_cube))/ vol_cube;
						//	v_mesh->vox_list.back().para_interpolate[1] = (x_cube * y_cube * z_cube) / vol_cube;
						//	v_mesh->vox_list.back().para_interpolate[2] = ((line_cube - x_cube) * y_cube * (line_cube - z_cube)) / vol_cube;
						//	v_mesh->vox_list.back().para_interpolate[3] = (x_cube * (line_cube - y_cube) * z_cube) / vol_cube;
						//	v_mesh->vox_list.back().para_interpolate[4] = ((line_cube - x_cube) * (line_cube - y_cube) * z_cube) / vol_cube;
						//	v_mesh->vox_list.back().para_interpolate[5] = (x_cube * y_cube * (line_cube - z_cube)) / vol_cube;
						//	v_mesh->vox_list.back().para_interpolate[6] = (x_cube * (line_cube - y_cube) * (line_cube - z_cube)) / vol_cube;
						//	v_mesh->vox_list.back().para_interpolate[7] = ((line_cube - x_cube) * y_cube * z_cube) / vol_cube;
						//}						
						//else
						//{
							d_cluster.clear();
							d_cluster.push_back(x_cube * x_cube + y_cube * y_cube + z_cube * z_cube);
							int i_vox = 1;
							double product = d_cluster.back();
							while (i_vox < 8)
							{
								if( v_mesh->vox_list.back().list_near_parentVox[i_vox])
								{
									double temp = 0.0;
									temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(0) - v_mesh->vox_list.back().vox_center(0)),2);
									temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(1) - v_mesh->vox_list.back().vox_center(1)),2);
									temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(2) - v_mesh->vox_list.back().vox_center(2)),2);
									d_cluster.push_back(temp);
									product *= temp;
								}
								i_vox++;
							}
							if(d_cluster.size() == 1)
								v_mesh->vox_list.back().para_interpolate[0] = 1.0;
							else
							{
								double sum = 0.0;
								for(i_vox = 0; i_vox < d_cluster.size(); i_vox++)
								{
									d_cluster[i_vox] = product / d_cluster[i_vox];
									sum += d_cluster[i_vox];
								}
								for(i_vox = 0; i_vox < 8; i_vox++)
								{
									if(v_mesh->vox_list.back().list_near_parentVox[i_vox])
									{
										v_mesh->vox_list.back().para_interpolate[i_vox] = d_cluster.front() / sum;
										d_cluster.erase(d_cluster.begin());
									}
								}
							}
						//}
					}
					else if(x_cube != 0 && y_cube == 0 && z_cube != 0)
					{
						//area 0, 4 neighbors
						if(x_cube > 0 && y_cube == 0 && z_cube > 0)
						{
							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->left_back_neighbor;
						
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->left_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->back_neighbor;
						}
						//area 1, 4 neighbors
						else if(x_cube < 0 && y_cube == 0 && z_cube > 0)
						{
							x_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->right_back_neighbor;
						
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->right_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->back_neighbor;
						}
						//area 2, 4 neighbors
						else if(x_cube > 0 && y_cube == 0 && z_cube < 0)
						{
							z_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->left_front_neighbor;
						
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->left_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->front_neighbor;
						}
						//area 3, 4 neighbors						
						else if(x_cube < 0 && y_cube == 0 && z_cube < 0)
						{
							x_cube *= (-1);
							z_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->right_front_neighbor;
						
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->right_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->front_neighbor;
						}
						// param
						//if(v_mesh->vox_list.back().list_near_parentVox[1] && v_mesh->vox_list.back().list_near_parentVox[2] && v_mesh->vox_list.back().list_near_parentVox[3])
						//{
						//	v_mesh->vox_list.back().para_interpolate[0] = ((line_cube -  x_cube) * (line_cube -  z_cube))/ area_cube;
						//	v_mesh->vox_list.back().para_interpolate[1] = (x_cube * z_cube) / area_cube;
						//	v_mesh->vox_list.back().para_interpolate[2] = (x_cube * (line_cube - z_cube)) / area_cube;
						//	v_mesh->vox_list.back().para_interpolate[3] = ((line_cube - x_cube) * z_cube) / area_cube;
						//}
						//else
						//{
							d_cluster.clear();
							d_cluster.push_back(x_cube * x_cube + y_cube * y_cube + z_cube * z_cube);
							int i_vox = 1;
							double product = d_cluster.back();
							while (i_vox < 4)
							{
								if(v_mesh->vox_list.back().list_near_parentVox[i_vox])
								{
									double temp = 0.0;
									temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(0) - v_mesh->vox_list.back().vox_center(0)),2);
									temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(1) - v_mesh->vox_list.back().vox_center(1)),2);
									temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(2) - v_mesh->vox_list.back().vox_center(2)),2);
									d_cluster.push_back(temp);
									product *= temp;
								}
								i_vox ++;
							}
							if(d_cluster.size() == 1)
								v_mesh->vox_list.back().para_interpolate[0] = 1.0;
							else
							{
								double sum = 0.0;
								for(i_vox = 0; i_vox < d_cluster.size(); i_vox++)
								{
									d_cluster[i_vox] = product / d_cluster[i_vox];
									sum += d_cluster[i_vox];
								}
								for(i_vox = 0; i_vox < 4; i_vox++)
								{
									if(v_mesh->vox_list.back().list_near_parentVox[i_vox])
									{
										v_mesh->vox_list.back().para_interpolate[i_vox] = d_cluster.front() / sum;
										d_cluster.erase(d_cluster.begin());
									}
								}
							}
						//}
					}
					else if(x_cube == 0 && y_cube != 0 && z_cube != 0)
					{
						//area 4, 4 neighbors
						if(x_cube == 0 && y_cube > 0 && z_cube > 0)
						{
							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->bottom_back_neighbor;
						
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->bottom_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->back_neighbor;
						}
						//area 5, 4 neighbors
						else if(x_cube == 0 && y_cube < 0 && z_cube > 0)
						{
							y_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->top_back_neighbor;
						
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->top_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->back_neighbor;
						}
						//area 6, 4 neighbors
						else if(x_cube == 0 && y_cube > 0 && z_cube < 0)
						{
							z_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->bottom_front_neighbor;
						
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->bottom_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->front_neighbor;
						}
						//area 7, 4 neighbors						
						else if(x_cube == 0 && y_cube < 0 && z_cube < 0)
						{
							y_cube *= (-1);
							z_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->top_front_neighbor;
						
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->top_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->front_neighbor;
						}
						//if(v_mesh->vox_list.back().list_near_parentVox[1] && v_mesh->vox_list.back().list_near_parentVox[2] && v_mesh->vox_list.back().list_near_parentVox[3])
						//{
						//	v_mesh->vox_list.back().para_interpolate[0] = ((line_cube -  y_cube) * (line_cube -  z_cube))/ area_cube;
						//	v_mesh->vox_list.back().para_interpolate[1] = (y_cube * z_cube) / area_cube;
						//	v_mesh->vox_list.back().para_interpolate[2] = (y_cube * (line_cube - z_cube)) / area_cube;
						//	v_mesh->vox_list.back().para_interpolate[3] = ((line_cube - y_cube) * z_cube) / area_cube;
						//}
						//else
						//{
							d_cluster.clear();
							d_cluster.push_back(x_cube * x_cube + y_cube * y_cube + z_cube * z_cube);
							int i_vox = 1;
							double product = d_cluster.back();
							while (i_vox < 4 )
							{
								if(v_mesh->vox_list.back().list_near_parentVox[i_vox])
								{
									double temp = 0.0;
									temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(0) - v_mesh->vox_list.back().vox_center(0)),2);
									temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(1) - v_mesh->vox_list.back().vox_center(1)),2);
									temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(2) - v_mesh->vox_list.back().vox_center(2)),2);
									d_cluster.push_back(temp);
									product *= temp;
								}
								i_vox ++;
							}							
							if(d_cluster.size() == 1)
								v_mesh->vox_list.back().para_interpolate[0] = 1.0;
							else
							{
								double sum = 0.0;
								for(i_vox = 0; i_vox < d_cluster.size(); i_vox++)
								{
									d_cluster[i_vox] = product / d_cluster[i_vox];
									sum += d_cluster[i_vox];
								}
								for(i_vox = 0; i_vox < 4; i_vox++)
								{
									if(v_mesh->vox_list.back().list_near_parentVox[i_vox])
									{
										v_mesh->vox_list.back().para_interpolate[i_vox] = d_cluster.front() / sum;
										d_cluster.erase(d_cluster.begin());
									}
								}
							}
						//}
					}
					else if(x_cube != 0 && y_cube != 0 && z_cube == 0)
					{
						//area 4, 4 neighbors
						if(x_cube > 0 && y_cube > 0 && z_cube == 0)
						{
							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->bottom_left_neighbor;
						
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->bottom_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->left_neighbor;
						}
						//area 5, 4 neighbors
						else if(x_cube > 0 && y_cube < 0 && z_cube == 0)
						{
							y_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->top_left_neighbor;
						
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->top_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->left_neighbor;
						}
						//area 6, 4 neighbors
						else if(x_cube < 0 && y_cube > 0 && z_cube == 0)
						{
							x_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->bottom_right_neighbor;
						
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->bottom_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->right_neighbor;
						}
						//area 7, 4 neighbors						
						else if(x_cube < 0 && y_cube < 0 && z_cube == 0)
						{
							x_cube *= (-1);
							y_cube *= (-1);

							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->top_right_neighbor;
						
							v_mesh->vox_list.back().list_near_parentVox[2] = parent_Vox->top_neighbor;
							v_mesh->vox_list.back().list_near_parentVox[3] = parent_Vox->right_neighbor;
						}
						//if(v_mesh->vox_list.back().list_near_parentVox[1] && v_mesh->vox_list.back().list_near_parentVox[2] && v_mesh->vox_list.back().list_near_parentVox[3])
						//{
						//	v_mesh->vox_list.back().para_interpolate[0] = ((line_cube -  x_cube) * (line_cube -  y_cube))/ area_cube;
						//	v_mesh->vox_list.back().para_interpolate[1] = (x_cube * y_cube) / area_cube;
						//	v_mesh->vox_list.back().para_interpolate[2] = (y_cube * (line_cube - x_cube)) / area_cube;
						//	v_mesh->vox_list.back().para_interpolate[3] = ((line_cube - y_cube) * x_cube) / area_cube;
						//}						
						//else
						//{
							d_cluster.clear();
							d_cluster.push_back(x_cube * x_cube + y_cube * y_cube + z_cube * z_cube);
							int i_vox = 1;
							double product = d_cluster.back();
							while (i_vox < 4 )
							{
								if(v_mesh->vox_list.back().list_near_parentVox[i_vox])
								{
									double temp = 0.0;
									temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(0) - v_mesh->vox_list.back().vox_center(0)),2);
									temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(1) - v_mesh->vox_list.back().vox_center(1)),2);
									temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(2) - v_mesh->vox_list.back().vox_center(2)),2);
									d_cluster.push_back(temp);
									product *= temp;
								}
								i_vox ++;
							}							
							if(d_cluster.size() == 1)
								v_mesh->vox_list.back().para_interpolate[0] = 1.0;
							else
							{
								double sum = 0.0;
								for(i_vox = 0; i_vox < d_cluster.size(); i_vox++)
								{
									d_cluster[i_vox] = product / d_cluster[i_vox];
									sum += d_cluster[i_vox];
								}
								for(i_vox = 0; i_vox < 4; i_vox++)
								{
									if(v_mesh->vox_list.back().list_near_parentVox[i_vox])
									{
										v_mesh->vox_list.back().para_interpolate[i_vox] = d_cluster.front() / sum;
										d_cluster.erase(d_cluster.begin());
									}
								}
							}
						//}
					}
					//side 0 1, 2 neighbors
					else if(x_cube == 0 && y_cube != 0 && z_cube == 0)
					{
						if(y_cube > 0)
						{
							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->bottom_neighbor;
						}
						else
						{
							y_cube *= (-1);
							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->top_neighbor;
						}

						if(v_mesh->vox_list.back().list_near_parentVox[1])
						{
							v_mesh->vox_list.back().para_interpolate[0] = (line_cube -  y_cube)/ line_cube;
							v_mesh->vox_list.back().para_interpolate[1] = y_cube / line_cube;
						}
						else
						{
							v_mesh->vox_list.back().para_interpolate[0] = 1.0;
							v_mesh->vox_list.back().para_interpolate[1] = 0.0;
						}
					}
					//side 1 2, 2 neighbors
					else if(x_cube != 0 && y_cube == 0 && z_cube == 0)
					{
						if(x_cube > 0)
						{
							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->left_neighbor;
						}
						else
						{
							x_cube *= (-1);
							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->right_neighbor;
						}
						
						if(v_mesh->vox_list.back().list_near_parentVox[1])
						{
							v_mesh->vox_list.back().para_interpolate[0] = (line_cube -  x_cube)/ line_cube;
							v_mesh->vox_list.back().para_interpolate[1] = x_cube / line_cube;
						}
						else
						{
							v_mesh->vox_list.back().para_interpolate[0] = 1.0;
							v_mesh->vox_list.back().para_interpolate[1] = 0.0;
						}
					}
					//side 3 4, 2 neighbors
					else if(x_cube == 0 && y_cube == 0 && z_cube != 0)
					{
						if(z_cube > 0)
						{
							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->back_neighbor;
						}
						else
						{
							z_cube *= (-1);
							v_mesh->vox_list.back().list_near_parentVox[0] = parent_Vox;
							v_mesh->vox_list.back().list_near_parentVox[1] = parent_Vox->front_neighbor;
						}
						if(v_mesh->vox_list.back().list_near_parentVox[1])
						{
							v_mesh->vox_list.back().para_interpolate[0] = (line_cube -  z_cube)/ line_cube;
							v_mesh->vox_list.back().para_interpolate[1] = z_cube / line_cube;
						}
						else
						{
							v_mesh->vox_list.back().para_interpolate[0] = 1.0;
							v_mesh->vox_list.back().para_interpolate[1] = 0.0;
						}
					}
					//6 neighbors
					v_mesh->vox_list.back().list_near_parentVox[0] = v_mesh->vox_list.back().parent_vox;
					v_mesh->vox_list.back().list_near_parentVox[1] = v_mesh->vox_list.back().parent_vox->top_neighbor;
					v_mesh->vox_list.back().list_near_parentVox[2] = v_mesh->vox_list.back().parent_vox->bottom_neighbor;
					v_mesh->vox_list.back().list_near_parentVox[3] = v_mesh->vox_list.back().parent_vox->front_neighbor;
					v_mesh->vox_list.back().list_near_parentVox[4] = v_mesh->vox_list.back().parent_vox->back_neighbor;
					v_mesh->vox_list.back().list_near_parentVox[5] = v_mesh->vox_list.back().parent_vox->left_neighbor;
					v_mesh->vox_list.back().list_near_parentVox[6] = v_mesh->vox_list.back().parent_vox->right_neighbor;
					v_mesh->vox_list.back().list_near_parentVox[7] = NULL;
					d_cluster.clear();
					int i_vox = 0;
					double product = 1.0;
					while (i_vox < 7)
					{
						if( v_mesh->vox_list.back().list_near_parentVox[i_vox])
						{
							double temp = 0.0;
							temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(0) - v_mesh->vox_list.back().vox_center(0)),2);
							temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(1) - v_mesh->vox_list.back().vox_center(1)),2);
							temp += pow((v_mesh->vox_list.back().list_near_parentVox[i_vox]->vox_center(2) - v_mesh->vox_list.back().vox_center(2)),2);
							temp += 0.00001;
							d_cluster.push_back(temp);
							product *= temp;
						}
						i_vox++;
					}
					if(d_cluster.size() == 1)
						v_mesh->vox_list.back().para_interpolate[0] = 1.0;
					else
					{
						double sum = 0.0;
						for(i_vox = 0; i_vox < d_cluster.size(); i_vox++)
						{
							d_cluster[i_vox] = product / d_cluster[i_vox];
							sum += d_cluster[i_vox];
						}
						for(i_vox = 0; i_vox < 8; i_vox++)
						{
							if(v_mesh->vox_list.back().list_near_parentVox[i_vox])
							{
								v_mesh->vox_list.back().para_interpolate[i_vox] = d_cluster.front() / sum;
								d_cluster.erase(d_cluster.begin());
							}
						}
					}
					//////////////////////////////////////////////////////////////////////
					}// for if


				}//for x
			}//for y
		}//for z
		}

		for(int i=0; i<v_mesh->node_list.size(); ++i)
		{
			if (v_mesh->node_list[i].flag_surface_node)
			{
				v_mesh->surface_node_list.push_back(&v_mesh->node_list[i]);
			}
		}

		v_mesh->num_node = v_mesh->node_list.size();
		v_mesh->num_surface_vox = v_mesh->surface_vox_list.size();
		v_mesh->num_vox = v_mesh->vox_list.size();
		v_mesh->num_surface_node = v_mesh->surface_node_list.size();
		v_mesh->vox_size = half_size * 2;


		// update neighbor info. & interpolatin info (26 neighbor)
		for(int i = 0; i < parent_mesh->num_cluster; i ++)
		{
			Vox * parent_Vox = parent_mesh->cluster_list[i].vox_list[0];
			double center_x = parent_Vox->vox_center(0);
			double center_y = parent_Vox->vox_center(1);
			double center_z = parent_Vox->vox_center(2);
	
			int parent_index = parent_Vox->coord_grid[0]*d_total*d_total + parent_Vox->coord_grid[1]*d_total + parent_Vox->coord_grid[2];
			int index = 0;
			int _x = 0;
			int _y = 0;
			int _z = 0;
			int vox_origin = 0;
			int vox_neighbor = 0;

			for (int z = 0; z < d; ++z)
			{
				for (int y = 0; y < d; ++y)
				{
					for (int x = 0; x < d; ++x)
					{
						//if(d == 2 && d_parent == 2 && z == 1 && y == 1 && x == 0)
						//	cout << "bingo" << endl;

						if (vol[parent_index *d*d*d + z*d*d + y*d + x] != 2)
						{
						vox_origin = parent_index *d*d*d + z*d*d + y*d + x;
						//back neighbor
						if (z-1>=0)
						{
							vox_neighbor = parent_index *d*d*d + (z-1)*d*d + y*d + x;
							if (v_mesh->vox_locator[vox_neighbor] != NULL)
							{
								v_mesh->vox_locator[vox_origin]->back_neighbor = v_mesh->vox_locator[vox_neighbor];
								v_mesh->vox_locator[vox_neighbor]->front_neighbor = v_mesh->vox_locator[vox_origin];
							}
						}
						if(z == 0)
						{
							if(parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0] - 1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (d-1)*d*d + y*d + x;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						//bottom neighbor
						if (y-1>=0)
						{
							vox_neighbor = parent_index *d*d*d + z*d*d + (y-1)*d + x;
							if (v_mesh->vox_locator[vox_neighbor] != NULL)
							{
								v_mesh->vox_locator[vox_origin]->bottom_neighbor = v_mesh->vox_locator[vox_neighbor];
								v_mesh->vox_locator[vox_neighbor]->top_neighbor = v_mesh->vox_locator[vox_origin];
							}
						}
						if(y == 0)
						{
							if(parent_Vox->coord_grid[1] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + z*d*d + (d-1)*d + x;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						//left neighbor
						if (x-1>=0)
						{
							vox_neighbor = parent_index *d*d*d + z*d*d + y*d + x-1;
							if (v_mesh->vox_locator[vox_neighbor] != NULL)
							{
								v_mesh->vox_locator[vox_origin]->left_neighbor = v_mesh->vox_locator[vox_neighbor];
								v_mesh->vox_locator[vox_neighbor]->right_neighbor = v_mesh->vox_locator[vox_origin];
							}
						}
						if(x == 0)
						{
							if(parent_Vox->coord_grid[2] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
								vox_neighbor = index *d*d*d + z*d*d + y*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->left_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->right_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						//left back neighbor
						if (z-1>=0 && x-1>=0)
						{
							vox_neighbor = parent_index *d*d*d + (z-1)*d*d + y*d + x-1;
							if (v_mesh->vox_locator[vox_neighbor] != NULL)
							{
								v_mesh->vox_locator[vox_origin]->left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
								v_mesh->vox_locator[vox_neighbor]->right_front_neighbor = v_mesh->vox_locator[vox_origin];
							}
						}
						else if(z == 0 && x == 0)
						{
							if(parent_Vox->coord_grid[0] >= 1 && parent_Vox->coord_grid[2] >= 1)
							{
								index = (parent_Vox->coord_grid[0] - 1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2] - 1;
								vox_neighbor = index *d*d*d + (d-1)*d*d + y*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(z == 0 && x >= 1)
						{
							if(parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0] - 1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (d-1)*d*d + y*d + x-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(z >= 1 && x == 0)
						{
							if(parent_Vox->coord_grid[2] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2] - 1;
								vox_neighbor = index *d*d*d + (z-1)*d*d + y*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						//right back neighbor
						if (z-1>=0 && x+1 < d)
						{
							vox_neighbor = parent_index *d*d*d + (z-1)*d*d + y*d + x+1;
							if (v_mesh->vox_locator[vox_neighbor] != NULL)
							{
								v_mesh->vox_locator[vox_origin]->right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
								v_mesh->vox_locator[vox_neighbor]->left_front_neighbor = v_mesh->vox_locator[vox_origin];
							}
						}
						else if(z == 0 && x == d-1)
						{
							if(parent_Vox->coord_grid[0] >= 1 && parent_Vox->coord_grid[2] < d_parent-1)
							{
								index = (parent_Vox->coord_grid[0] - 1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2] + 1;
								vox_neighbor = index *d*d*d + (d-1)*d*d + y*d;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(z >= 1 && x == d-1)
						{
							if(parent_Vox->coord_grid[2] < d_parent-1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2] + 1;
								vox_neighbor = index *d*d*d + (z-1)*d*d + y*d;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(z == 0 && x+1 < d)
						{
							if(parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0] - 1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (d-1)*d*d + y*d + x+1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						// bottom back neighbor
						if (z-1>=0 && y-1>=0)
						{
							vox_neighbor = parent_index *d*d*d + (z-1)*d*d + (y-1)*d + x;
							if (v_mesh->vox_locator[vox_neighbor] != NULL)
							{
								v_mesh->vox_locator[vox_origin]->bottom_back_neighbor = v_mesh->vox_locator[vox_neighbor];
								v_mesh->vox_locator[vox_neighbor]->top_front_neighbor = v_mesh->vox_locator[vox_origin];
							}
						}
						else if(z == 0 && y == 0)
						{
							if(parent_Vox->coord_grid[0] >= 1 && parent_Vox->coord_grid[1] >= 1)
							{
								index = (parent_Vox->coord_grid[0] - 1) *d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (d-1)*d*d + (d-1)*d + x;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(z == 0 && y-1 >= 0)
						{
							if(parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0] - 1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (d-1)*d*d + (y-1)*d + x;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(z-1 >= 0 && y == 0)
						{
							if(parent_Vox->coord_grid[1] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (z-1)*d*d + (d-1)*d + x;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						//bottom left neighbor
						if (x-1>=0 && y-1>=0)
						{
							vox_neighbor = parent_index *d*d*d + z*d*d + (y-1)*d + x-1;
							if (v_mesh->vox_locator[vox_neighbor] != NULL)
							{
								v_mesh->vox_locator[vox_origin]->bottom_left_neighbor = v_mesh->vox_locator[vox_neighbor];
								v_mesh->vox_locator[vox_neighbor]->top_right_neighbor = v_mesh->vox_locator[vox_origin];
							}
						}
						else if(x == 0 && y == 0)
						{
							if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[1] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2]-1;
								vox_neighbor = index *d*d*d + z*d*d + (d-1)*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_left_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_right_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x-1>=0 && y == 0)
						{
							if(parent_Vox->coord_grid[1] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + z*d*d + (d-1)*d + x-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_left_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_right_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x == 0 && y-1>=0)
						{
							if(parent_Vox->coord_grid[2] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
								vox_neighbor = index *d*d*d + z*d*d + (y-1)*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_left_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_right_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						//bottom right neighbor
						if (y-1>=0 && x+1 < d)
						{
							vox_neighbor = parent_index *d*d*d + z*d*d + (y-1)*d + x+1;
							if (v_mesh->vox_locator[vox_neighbor] != NULL)
							{
								v_mesh->vox_locator[vox_origin]->bottom_right_neighbor = v_mesh->vox_locator[vox_neighbor];
								v_mesh->vox_locator[vox_neighbor]->top_left_neighbor = v_mesh->vox_locator[vox_origin];
							}
						}
						else if(y == 0 && x == d-1)
						{
							if(parent_Vox->coord_grid[1] >= 1 && parent_Vox->coord_grid[2] < d_parent-1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2] + 1;
								vox_neighbor = index *d*d*d + z*d*d + (d-1)*d;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_right_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_left_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(y == 0 && x+1 < d)
						{
							if(parent_Vox->coord_grid[1] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + z*d*d + (d-1)*d + x+1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_right_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_left_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(y-1 >= 0 && x == d-1)
						{
							if(parent_Vox->coord_grid[2] < d_parent-1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]+1;
								vox_neighbor = index *d*d*d + z*d*d + (y-1)*d;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_right_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_left_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						//bottom left back
						if (x-1>=0 && y-1>=0 && z-1>=0)
						{
							vox_neighbor = parent_index *d*d*d + (z-1)*d*d + (y-1)*d + x-1;
							if (v_mesh->vox_locator[vox_neighbor] != NULL)
							{
								v_mesh->vox_locator[vox_origin]->bottom_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
								v_mesh->vox_locator[vox_neighbor]->top_right_front_neighbor = v_mesh->vox_locator[vox_origin];
							}
						}
						else if(x == 0 && y == 0 && z == 0)
						{
							if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[1] >= 1 && parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2]-1;
								vox_neighbor = index *d*d*d + (d-1)*d*d + (d-1)*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x == 0 && y >= 1 && z >= 1)
						{
							if(parent_Vox->coord_grid[2] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
								vox_neighbor = index *d*d*d + (z-1)*d*d + (y-1)*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x >= 1 && y == 0 && z >= 1)
						{
							if(parent_Vox->coord_grid[1] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (z-1)*d*d + (d-1)*d + x-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x >= 1 && y >= 1 && z == 0)
						{
							if(parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (d-1)*d*d + (y-1)*d + x-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}						
						else if(x == 0 && y == 0 && z >= 1)
						{
							if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[1] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2]-1;
								vox_neighbor = index *d*d*d + (z-1)*d*d + (d-1)*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x >= 1 && y == 0 && z == 0)
						{
							if(parent_Vox->coord_grid[1] >= 1 && parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (d-1)*d*d + (d-1)*d + x-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x == 0 && y >= 1 && z == 0)
						{
							if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
								vox_neighbor = index *d*d*d + (d-1)*d*d + (y-1)*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						//bottom right back
						if (x+1<d && y-1>=0 && z-1>=0)
						{
							vox_neighbor = parent_index *d*d*d + (z-1)*d*d + (y-1)*d + x+1;
							if (v_mesh->vox_locator[vox_neighbor] != NULL)
							{
								v_mesh->vox_locator[vox_origin]->bottom_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
								v_mesh->vox_locator[vox_neighbor]->top_left_front_neighbor = v_mesh->vox_locator[vox_origin];
							}
						}
						else if(x == d-1 && y == 0 && z == 0)
						{
							if(parent_Vox->coord_grid[2] < d_parent-1 && parent_Vox->coord_grid[1] >= 1 && parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2]+1;
								vox_neighbor = index *d*d*d + (d-1)*d*d + (d-1)*d;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x == d-1 && y >= 1 && z >= 1)
						{
							if(parent_Vox->coord_grid[2] < d_parent-1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]+1;
								vox_neighbor = index *d*d*d + (z-1)*d*d + (y-1)*d;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x+1 < d && y >= 1 && z == 0)
						{
							if(parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (d-1)*d*d + (y-1)*d + x+1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x+1 < d && y == 0 && z >= 1)
						{
							if(parent_Vox->coord_grid[1] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (z-1)*d*d + (d-1)*d + x+1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x+1 < d && y == 0 && z == 0)
						{
							if(parent_Vox->coord_grid[1] >= 1 && parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (d-1)*d*d + (d-1)*d + x+1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x == d-1 && y >= 1 && z == 0)
						{
							if(parent_Vox->coord_grid[2] < d_parent-1 && parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]+1;
								vox_neighbor = index *d*d*d + (d-1)*d*d + (y-1)*d;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x == d-1 && y == 0 && z >= 1)
						{
							if(parent_Vox->coord_grid[2] < d_parent-1 && parent_Vox->coord_grid[1] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2]+1;
								vox_neighbor = index *d*d*d + (z-1)*d*d + (d-1)*d;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->bottom_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->top_left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						//top back
						if (y+1<d && z-1>=0)
						{
							vox_neighbor = parent_index *d*d*d + (z-1)*d*d + (y+1)*d + x;
							if (v_mesh->vox_locator[vox_neighbor] != NULL)
							{
								v_mesh->vox_locator[vox_origin]->top_back_neighbor = v_mesh->vox_locator[vox_neighbor];
								v_mesh->vox_locator[vox_neighbor]->bottom_front_neighbor = v_mesh->vox_locator[vox_origin];
							}
						}
						else if(y == d-1 && z == 0)
						{
							if(parent_Vox->coord_grid[1] < d_parent-1 && parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + (parent_Vox->coord_grid[1]+1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor =  index *d*d*d + (d-1)*d*d + x; 
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(y+1 < d && z == 0)
						{
							if(parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
								vox_neighbor =  index *d*d*d + (d-1)*d*d + (y+1)*d + x; 
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(y == d-1 && z >= 1)
						{
							if(parent_Vox->coord_grid[1] < d_parent-1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]+1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor =  index *d*d*d + (z-1)*d*d + x; 
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						//top left
						if (y+1<d && x-1>=0)
						{
							vox_neighbor = parent_index *d*d*d + z*d*d + (y+1)*d + x-1;
							if (v_mesh->vox_locator[vox_neighbor] != NULL)
							{
								v_mesh->vox_locator[vox_origin]->top_left_neighbor = v_mesh->vox_locator[vox_neighbor];
								v_mesh->vox_locator[vox_neighbor]->bottom_right_neighbor = v_mesh->vox_locator[vox_origin];
							}
						}
						else if(y == d-1 && x == 0)
						{
							if(parent_Vox->coord_grid[1] < d_parent-1 && parent_Vox->coord_grid[2] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]+1) *d_total + parent_Vox->coord_grid[2]-1;
								vox_neighbor = index *d*d*d + z*d*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_left_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_right_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(y+1 < d && x == 0)
						{
							if(parent_Vox->coord_grid[2] >= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
								vox_neighbor = index *d*d*d + z*d*d +(y+1)*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_left_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_right_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(y == d-1 && x >= 1)
						{
							if(parent_Vox->coord_grid[1] < d_parent-1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]+1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + z*d*d + x-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_left_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_right_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						//top left back
						if (x-1>=0 && y+1<d && z-1>=0)
						{
							vox_neighbor = parent_index *d*d*d + (z-1)*d*d + (y+1)*d + x-1;
							if (v_mesh->vox_locator[vox_neighbor] != NULL)
							{
								v_mesh->vox_locator[vox_origin]->top_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
								v_mesh->vox_locator[vox_neighbor]->bottom_right_front_neighbor = v_mesh->vox_locator[vox_origin];
							}
						}
						else if(x == 0 && y == d-1 && z == 0)
						{
							if(parent_Vox->coord_grid[2]>= 1 && parent_Vox->coord_grid[1] < d_parent-1  && parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + (parent_Vox->coord_grid[1]+1) *d_total + parent_Vox->coord_grid[2]-1;
								vox_neighbor = index *d*d*d + (d-1)*d*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x >= 1 && y == d-1 && z == 0)
						{
							if(parent_Vox->coord_grid[1] < d_parent-1  && parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + (parent_Vox->coord_grid[1]+1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (d-1)*d*d + x-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x == 0 && y+1 < d && z == 0)
						{
							if(parent_Vox->coord_grid[2]>= 1 && parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
								vox_neighbor = index *d*d*d + (d-1)*d*d + (y+1)*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x == 0 && y == d-1 && z >= 1)
						{
							if(parent_Vox->coord_grid[2]>= 1 && parent_Vox->coord_grid[1] < d_parent-1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]+1) *d_total + parent_Vox->coord_grid[2]-1;
								vox_neighbor = index *d*d*d + (z-1)*d*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x >= 1 && y == d-1 && z >= 1)
						{
							if(parent_Vox->coord_grid[1] < d_parent-1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]+1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (z-1)*d*d + x-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x >= 1 && y+1 < d && z == 0)
						{
							if(parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (d-1)*d*d + (y+1)*d + x-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x == 0 && y+1 < d && z >= 1)
						{
							if(parent_Vox->coord_grid[2]>= 1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
								vox_neighbor = index *d*d*d + (z-1)*d*d +(y+1)*d + d-1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_left_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_right_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						//top right back
						if (x+1<d && y+1<d && z-1>=0)
						{
							vox_neighbor = parent_index *d*d*d + (z-1)*d*d + (y+1)*d + x+1;
							if (v_mesh->vox_locator[vox_neighbor] != NULL)
							{
								v_mesh->vox_locator[vox_origin]->top_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
								v_mesh->vox_locator[vox_neighbor]->bottom_left_front_neighbor = v_mesh->vox_locator[vox_origin];
							}
						}
						else if(x == d-1 && y == d-1 && z == 0)
						{
							if(parent_Vox->coord_grid[2] < d_parent-1 && parent_Vox->coord_grid[1] < d_parent-1  && parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + (parent_Vox->coord_grid[1]+1) *d_total + parent_Vox->coord_grid[2]+1;
								vox_neighbor = index *d*d*d + (d-1)*d*d;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x+1 < d && y == d-1 && z == 0)
						{
							if(parent_Vox->coord_grid[1] < d_parent-1  && parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + (parent_Vox->coord_grid[1]+1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (d-1)*d*d + x+1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x == d-1 && y+1 < d && z == 0)
						{
							if(parent_Vox->coord_grid[2] < d_parent-1 && parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]+1;
								vox_neighbor = index *d*d*d + (d-1)*d*d + (y+1)*d;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x == d-1 && y == d-1 && z >= 1)
						{
							if(parent_Vox->coord_grid[2] < d_parent-1 && parent_Vox->coord_grid[1] < d_parent-1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]+1) *d_total + parent_Vox->coord_grid[2]+1;
								vox_neighbor = index *d*d*d + (z-1)*d*d;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						//
						else if(x+1 < d && y+1 < d && z == 0)
						{
							if(parent_Vox->coord_grid[0] >= 1)
							{
								index = (parent_Vox->coord_grid[0]-1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (d-1)*d*d + (y+1)*d +x+1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x == d-1 && y+1 < d && z >= 1)
						{
							if(parent_Vox->coord_grid[2] < d_parent-1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]+1;
								vox_neighbor = index *d*d*d + (z-1)*d*d + (y+1)*d;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}
						else if(x+1 < d && y == d-1 && z >= 1)
						{
							if(parent_Vox->coord_grid[1] < d_parent-1)
							{
								index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]+1) *d_total + parent_Vox->coord_grid[2];
								vox_neighbor = index *d*d*d + (z-1)*d*d + x+1;
								if (v_mesh->vox_locator[vox_neighbor] != NULL)
								{
									v_mesh->vox_locator[vox_origin]->top_right_back_neighbor = v_mesh->vox_locator[vox_neighbor];
									v_mesh->vox_locator[vox_neighbor]->bottom_left_front_neighbor = v_mesh->vox_locator[vox_origin];
								}
							}
						}

						//update interpolation info
						/*Vector3d cnew = v_mesh->vox_locator[parent_index *d*d*d + z*d*d + y*d + x]->vox_center;
						if(cnew(0) < center_x && cnew(1) < center_y && cnew(2) < center_z)
						{
							if(parent)
						}*/
					}//for if
					}//x
				}//y
			} //z, end updating neighbor
		}

	
}

void Kernel::generatePerVoxCluster4Level(VoxMesh* &vm, VoxMesh* &parent_mesh)
{ 
	for (node_iterator ni=vm->node_list.begin(); ni!=vm->node_list.end(); ++ni)
	{
		if(!ni->incident_cluster.empty())
			ni->incident_cluster.clear();
	}
	vector<Cluster>::iterator ci;
	for (ci = parent_mesh->cluster_list.begin(); ci != parent_mesh->cluster_list.end(); ++ci)
	{
		ci->cluster_list_children.clear();
	}
	if (level_list.size() <= 2)
	{
		for (int i = 0; i < level_list.size() - 1; i ++)
		{
			for (ci = level_list[i]->voxmesh_level->cluster_list.begin(); ci != level_list[i]->voxmesh_level->cluster_list.end(); ++ci)
			{
				ci->leaf_list.clear();
				ci->leaf_parameter.clear();
				ci->term_normlize = 1.0;
			}

		}
	}

	if(!vm->cluster_list.empty())
		vm->cluster_list.clear();
	vm->cluster_list.reserve(vm->num_vox);

	for (vox_iterator vi=vm->vox_list.begin(); vi!=vm->vox_list.end(); ++vi)
	{	
		Cluster new_cluster;
		new_cluster.vox_list.push_back(&(*vi));
		//add cluster pointer into vox
		vi->clusterid = vm->cluster_list.size();

		DuplicatedNode new_node_0(vi->node_0);
		DuplicatedNode new_node_1(vi->node_1);
		DuplicatedNode new_node_2(vi->node_2);
		DuplicatedNode new_node_3(vi->node_3);
		DuplicatedNode new_node_4(vi->node_4);
		DuplicatedNode new_node_5(vi->node_5);
		DuplicatedNode new_node_6(vi->node_6);
		DuplicatedNode new_node_7(vi->node_7);

		new_cluster.node_list.reserve(8);
		new_cluster.node_list.push_back(new_node_0);
		new_cluster.node_list.push_back(new_node_1);
		new_cluster.node_list.push_back(new_node_2);
		new_cluster.node_list.push_back(new_node_3);
		new_cluster.node_list.push_back(new_node_4);
		new_cluster.node_list.push_back(new_node_5);
		new_cluster.node_list.push_back(new_node_6);
		new_cluster.node_list.push_back(new_node_7);
						
		new_cluster.num_node = 8;
		new_cluster.num_surface_quads = 6;

		vm->cluster_list.push_back(new_cluster);

		// for per-vox cluster, every face is to be rendered
		vm->cluster_list.back().surface_quads.clear();
		vm->cluster_list.back().surface_quads.reserve(24);

		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[0]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[3]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[2]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[1]);

		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[4]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[5]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[6]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[7]);

		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[0]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[4]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[7]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[3]);

		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[1]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[2]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[6]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[5]);

		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[2]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[3]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[7]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[6]);

		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[0]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[1]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[5]);
		vm->cluster_list.back().surface_quads.push_back(&vm->cluster_list.back().node_list[4]);
		
		vi->node_0->incident_cluster.push_back(&vm->cluster_list.back());
		vi->node_1->incident_cluster.push_back(&vm->cluster_list.back());
		vi->node_2->incident_cluster.push_back(&vm->cluster_list.back());
		vi->node_3->incident_cluster.push_back(&vm->cluster_list.back());
		vi->node_4->incident_cluster.push_back(&vm->cluster_list.back());
		vi->node_5->incident_cluster.push_back(&vm->cluster_list.back());
		vi->node_6->incident_cluster.push_back(&vm->cluster_list.back());
		vi->node_7->incident_cluster.push_back(&vm->cluster_list.back());

		vi->node_0->duplicates.push_back(&(vm->cluster_list.back().node_list[0]));
		vi->node_1->duplicates.push_back(&(vm->cluster_list.back().node_list[1]));
		vi->node_2->duplicates.push_back(&(vm->cluster_list.back().node_list[2]));
		vi->node_3->duplicates.push_back(&(vm->cluster_list.back().node_list[3]));
		vi->node_4->duplicates.push_back(&(vm->cluster_list.back().node_list[4]));
		vi->node_5->duplicates.push_back(&(vm->cluster_list.back().node_list[5]));
		vi->node_6->duplicates.push_back(&(vm->cluster_list.back().node_list[6]));
		vi->node_7->duplicates.push_back(&(vm->cluster_list.back().node_list[7]));


		//add cluster into children of ParentCluster
		parent_mesh->cluster_list[vi->parent_vox->clusterid].cluster_list_children.push_back(&vm->cluster_list.back());
		//vi->parent_vox->pcluster->cluster_list_children.push_back(&vm->cluster_list.back());
		////add ParentCluster to be parent of cluster
		vm->cluster_list.back().parent_cluster = &parent_mesh->cluster_list[vi->parent_vox->clusterid];
		/////add leaf list into super_parent
		if (parent_mesh->cluster_list[vi->parent_vox->clusterid].super_parent_cluster == NULL)
			vm->cluster_list.back().super_parent_cluster = &parent_mesh->cluster_list[vi->parent_vox->clusterid];
		else
			vm->cluster_list.back().super_parent_cluster = parent_mesh->cluster_list[vi->parent_vox->clusterid].super_parent_cluster;

		//vm->cluster_list.back().super_parent_cluster->leaf_list.push_back(&vm->cluster_list.back());
		//yuan//
		if (level_list.size() <= 2)
		{
			Cluster * pParent = vm->cluster_list.back().parent_cluster;
			while(pParent != NULL)
			{
				pParent->leaf_list.push_back(&vm->cluster_list.back());
				pParent = pParent->parent_cluster;
			}
		}
		/////
		//vm->cluster_list.back().parent_cluster = vi->parent_vox->pcluster;
		vm->cluster_list.back().level_index = parent_mesh->cluster_list[vi->parent_vox->clusterid].level_index + 1;
		//vm->cluster_list.back().level_index = vi->parent_vox->pcluster->level_index + 1;
		////set centronoid as vox center
		vm->cluster_list.back().current_center = vi->vox_center;

		//cout << vm->cluster_list.back().parent_cluster->num_node << endl;
		//cout << parent_mesh->cluster_list[vi->parent_vox->clusterid].cluster_list_children[0]->num_node << endl;

	}

	vm->num_cluster = vm->cluster_list.size();
	//cout << "Has produced " << vm->num_cluster << "Clusters" << endl;


	//compute parameters for every superparent cluster
	/*
			vector<Matrix3d> R_all;
			vector<Vector3d> T_all;
			vector<double> m_Interpolation;
			int sizeLeaf = m_pcluster->leaf_list.size() ;
			double sum = 0.0;

			//compute interpolation params
			int i_l = 0;
			for (i_l = 0; i_l < sizeLeaf; i_l++)
			{
				double para = 0.0;
				para += pow((m_pcluster->current_center(0) - m_pcluster->leaf_list[i_l]->current_center(0)), 2);
				para += pow((m_pcluster->current_center(1) - m_pcluster->leaf_list[i_l]->current_center(1)), 2);
				para += pow((m_pcluster->current_center(2) - m_pcluster->leaf_list[i_l]->current_center(2)), 2);

				if(para == 0.0)
					para = 0.0000001;

				para = 1.0 / para;

				m_Interpolation.push_back(para);
				sum += para;

				R_all.push_back(m_pcluster->leaf_list[i_l]->r);
				Vector3d t = m_pcluster->leaf_list[i_l]->current_center - m_pcluster->leaf_list[i_l]->r * m_pcluster->leaf_list[i_l]->original_center;
				T_all.push_back(t);
			}
			
			//sum = sqrt(sum);
			if(R_all.size() > 1)
			{
				Quaterniond q(R_all[0]);
				double t_add = 0.0;
				int count = 1;
				//translation = (sqrt(m_Interpolation[0]) / sum) * T_all[0];
				translation = (m_Interpolation[0] / sum) * T_all[0];
				while(count < R_all.size())
				{
					Quaterniond q2(R_all[count]);
					t_add += (m_Interpolation[count-1] / sum);
					//double temp = (t_add * t_add) /(t_add * t_add + interpolation * interpolation);
					double temp = t_add / (t_add + (m_Interpolation[count] / sum));
					Quaterniond qtemp = q2.slerp(temp, q);
					q = qtemp;

					translation += (m_Interpolation[count] / sum) * T_all[count];
					
					count ++;
				}
				level_list[0]->voxmesh_level->cluster_list[i_c].r = q.toRotationMatrix();
			}
			else
			{
				level_list[0]->voxmesh_level->cluster_list[i_c].r = R_all[0];
				translation = T_all[0];
			}
			*/
	//yuan//
	if (level_list.size() <= 2)
	{
		for(int li = 0; li < level_list.size()-1; li ++)
		{
			for (ci = level_list[li]->voxmesh_level->cluster_list.begin(); ci != level_list[li]->voxmesh_level->cluster_list.end(); ++ci)
			{
				double sum = 0.0;
				int sizeLeaf = ci->leaf_list.size();
				//compute interpolation params
				int i_l = 0;
				for (i_l = 0; i_l < sizeLeaf; i_l++)
				{
					double para = 0.0;
					para += pow((ci->current_center(0) - ci->leaf_list[i_l]->current_center(0)), 2);
					para += pow((ci->current_center(1) - ci->leaf_list[i_l]->current_center(1)), 2);
					para += pow((ci->current_center(2) - ci->leaf_list[i_l]->current_center(2)), 2);

					if(para == 0)
						para = 0.0001;

					para = 1.0 / para;

			
					ci->leaf_parameter.push_back(para);
					sum += para;
				}
				ci->term_normlize = sum;
			}
		}
	}
	//yuan//


	vector<Node>::const_iterator niter = vm->node_list.begin();
	int temp = 0;
	for (; niter != vm->node_list.end(); niter ++)
	{
		temp += niter->duplicates.size();
	}
	vm->num_pair = temp;
}


void Kernel::saveOutputData(vector<double>& data, const char* filename)
{
	ofstream ofs(filename);
	for (int i=0; i<data.size(); ++i)
	{
		ofs << data[i] << endl;
	}
	ofs.close();
}

void Kernel::exportToOBJ(const char *filename)
{
	/*
	ofstream ofs(filename);
	//ofs << "# timestep " << time_step_index << "OBJ" << endl;
	ofs << "g " << "object1" << endl;
	ofs << "o " << "first" << endl;
	vector<Node>::iterator ni;
	for (ni = p_mesh->node_list.begin(); ni != p_mesh->node_list.end(); ++ni)
	{
		ofs << "v " << ni->coordinate(0) + ni->displacement(0) 
			<< " " << ni->coordinate(1) + ni->displacement(1) 
			<< " " << ni->coordinate(2) + ni->displacement(2) << endl;
	}

	vector<Face>::iterator fi;
	for (fi = p_mesh->face_list.begin(); fi != p_mesh->face_list.end(); ++fi)
	{
		ofs << "f " << fi->idx_node0+1 << " " << fi->idx_node1+1 << " " << fi->idx_node2+1 << endl;
	}
	ofs << "g " << "object2" << endl;
	ofs << "o " << "second" << endl;
	for (ni = p_mesh->node_list.begin(); ni != p_mesh->node_list.end(); ++ni)
	{
		ofs << "v " << ni->coordinate(0) + ni->displacement(0) + 1.0 
			<< " " << ni->coordinate(1) + ni->displacement(1) + 1.0
			<< " " << ni->coordinate(2) + ni->displacement(2) + 1.0 << endl;
	}
	for (fi = p_mesh->face_list.begin(); fi != p_mesh->face_list.end(); ++fi)
	{
		ofs << "f " << p_mesh->number_node+fi->idx_node0+1 << " " << p_mesh->number_node+fi->idx_node1+1 << " " << p_mesh->number_node+fi->idx_node2+1 << endl;
	}

	ofs.close();
	*/
	ofstream ofs(filename);
	ofs << "o " << "first" << endl;
	ofs << "g " << "object1" << endl;
	vector<Node>::iterator ni;
	for (ni = p_mesh->node_list.begin(); ni != p_mesh->node_list.end(); ++ni)
	{
		ofs << "v " << ni->coordinate(0) + ni->displacement(0) 
			<< " " << ni->coordinate(1) + ni->displacement(1) 
			<< " " << ni->coordinate(2) + ni->displacement(2) << endl;

	}
	vector<Face>::iterator fi;
	for (fi = p_mesh->face_list.begin(); fi != p_mesh->face_list.end(); ++fi)
	{
		ofs << "f " << fi->idx_node0+1 << " " << fi->idx_node1+1 << " " << fi->idx_node2+1 << endl;
	}
	if(flag_exportObj4Arrow)
	{
		ofs << "g " << "object2" << endl;
		ofs << "o " << "second" << endl;
		vector<Vector3d>::iterator vi;
		for (vi = v_box_list.begin(); vi != v_box_list.end(); ++vi)
		{
			Vector3d temp = _rotation * (*vi) + _posBegin;
			ofs << "v " << temp(0) 
				<< " " << temp(1)
				<< " " << temp(2) << endl;
		}
		for (vi = v_cone_list.begin(); vi != v_cone_list.end(); ++vi)
		{
			Vector3d temp = _rotation * (*vi) + _posEnd;
			ofs << "v " << temp(0) 
				<< " " << temp(1)
				<< " " << temp(2) << endl;
		}
		int noBefore = p_mesh->number_node;
		vector<Vector3i>::iterator ii;
		for (ii = f_box_list.begin(); ii != f_box_list.end(); ++ii)
		{
			ofs << "f " << noBefore+(*ii)(0)+1 << " " << noBefore+(*ii)(1)+1 << " " << noBefore+(*ii)(2)+1 << endl;
		}
		noBefore += v_box_list.size();
		for (ii = f_cone_list.begin(); ii != f_cone_list.end(); ++ii)
		{
			ofs << "f " << noBefore+(*ii)(0)+1 << " " << noBefore+(*ii)(1)+1 << " " << noBefore+(*ii)(2)+1 << endl;
		}
	}
	ofs.close();
}

void Kernel::exportToOBJ2Haptic(const char *filename)
{
	ofstream ofs(filename);
	//ofs << "# timestep " << time_step_index << "OBJ" << endl;
	ofs << "o " << "first" << endl;
	ofs << "g " << "object1" << endl;
	vector<Node>::iterator ni;
	for (ni = p_mesh->node_list.begin(); ni != p_mesh->node_list.end(); ++ni)
	{
		ofs << "v " << ni->coordinate(0) + ni->displacement(0) 
			<< " " << ni->coordinate(1) + ni->displacement(1) 
			<< " " << ni->coordinate(2) + ni->displacement(2) << endl;
	}

	vector<Face>::iterator fi;
	for (fi = p_mesh->face_list.begin(); fi != p_mesh->face_list.end(); ++fi)
	{
		ofs << "f " << fi->idx_node0+1 << " " << fi->idx_node1+1 << " " << fi->idx_node2+1 << endl;
	}
	ofs << "g " << "object2" << endl;
	ofs << "o " << "second" << endl;
	for (ni = sphere->node_list.begin(); ni != sphere->node_list.end(); ++ni)
	{
		ofs << "v " << ni->coordinate(0)/20 + surface_point_left(0) 
			<< " " << ni->coordinate(1)/20 + surface_point_left(1)
			<< " " << ni->coordinate(2)/20 + surface_point_left(2) << endl;
	}
	for (fi = sphere->face_list.begin(); fi != sphere->face_list.end(); ++fi)
	{
		ofs << "f " << p_mesh->number_node+fi->idx_node0+1 << " " << p_mesh->number_node+fi->idx_node1+1 << " " << p_mesh->number_node+fi->idx_node2+1 << endl;
	}

	ofs << "g " << "object3" << endl;
	ofs << "o " << "third" << endl;
	for (ni = sphere->node_list.begin(); ni != sphere->node_list.end(); ++ni)
	{
		ofs << "v " << ni->coordinate(0)/20 + surface_point_right(0) 
			<< " " << ni->coordinate(1)/20 + surface_point_right(1)
			<< " " << ni->coordinate(2)/20 + surface_point_right(2) << endl;
	}
	for (fi = sphere->face_list.begin(); fi != sphere->face_list.end(); ++fi)
	{
		ofs << "f " << p_mesh->number_node+sphere->number_node+ fi->idx_node0+1 << " " << p_mesh->number_node+sphere->number_node+fi->idx_node1+1 << " " << p_mesh->number_node+sphere->number_node+fi->idx_node2+1 << endl;
	}

}

void Kernel::removeConstraintOfAllLevel()
{
	for(int i = 0; i < level_list.size(); i++)
	{
		if(level_list[i]->voxmesh_level->constraint_node)
		{
			
			for(int j=0; j<level_list[i]->voxmesh_level->constraint_node->incident_cluster.size(); ++j)
			{
				level_list[i]->voxmesh_level->constraint_node->incident_cluster[j]->flag_constrained = false;
				level_list[i]->voxmesh_level->constraint_node->incident_cluster[j]->constraint_node = NULL;
			}

			for (int k=0; k<level_list[i]->voxmesh_level->constraint_node->duplicates.size(); ++k)
			{
				level_list[i]->voxmesh_level->constraint_node->duplicates[k]->flag_constraint_node = false;
			}
			level_list[i]->voxmesh_level->constraint_node = NULL;
		}
		//all targets set to be rest shape
		//vector<Cluster>::iterator ci;
		//for (ci=level_list[i]->voxmesh_level->cluster_list.begin(); ci!=level_list[i]->voxmesh_level->cluster_list.end(); ++ci)
		//{
		//	for (int j=0; j<ci->num_node; ++j)
		//	{
		//		ci->node_list[j].target_position = ci->node_list[j].mapped_node->coordinate;
		//	}
		//}
	}
}

void Kernel::setConstraint4AllLevel(Node *& ni)
{
	Node * temp = ni;
	for(int i = 0; i < level_list.size(); i++)
	{
		level_list[i]->voxmesh_level->constraint_node = temp;
		for(int j=0; j<level_list[i]->voxmesh_level->constraint_node->incident_cluster.size(); ++j)
		{
			level_list[i]->voxmesh_level->constraint_node->incident_cluster[j]->flag_constrained = true;
			level_list[i]->voxmesh_level->constraint_node->incident_cluster[j]->constraint_node = NULL;
		}

		for (int k=0; k<level_list[i]->voxmesh_level->constraint_node->duplicates.size(); ++k)
		{
			level_list[i]->voxmesh_level->constraint_node->duplicates[k]->flag_constraint_node = true;
			level_list[i]->voxmesh_level->constraint_node->prescribed_position = level_list[i]->voxmesh_level->constraint_node->target_position;
		}
			
		temp = temp->child_node;
	}
}

void Kernel::printVector3d(Vector3d & value)
{
	cout << value.x() << " " << value.y() << " " << value.z() << endl;
}

void Kernel::updateFromLeaf(Cluster * pcluster, Matrix3d & R, Vector3d & T, int &count)
{
	//if leaf, add to R & T
	if(pcluster->level_index == level_list.size()-1)
	{
		R += pcluster->angular_displacement;
		T += pcluster->linear_displacement;
		count ++;
	}
	else 
	{
		for(int i = 0; i < pcluster->cluster_list_children.size(); i ++)
		{
			updateFromLeaf(pcluster->cluster_list_children[i], R, T, count);
		}
	}
}

void Kernel::updateRTFromLeaf(Cluster * pcluster, vector<Vector3d> & T, vector<Matrix3d> & R, int &count)
{
	//for shape matching
	//if leaf, add to R
	if(pcluster->level_index == level_list.size()-1)
	{
		R.push_back(pcluster->r);
		Vector3d t = pcluster->current_center - pcluster->r * pcluster->original_center;
		//cout << "R" << pcluster->r << endl;
		//cout << "T" << t << endl;
		T.push_back(t);
		count ++;
	}
	else 
	{
		for(int i = 0; i < pcluster->cluster_list_children.size(); i ++)
		{
			updateRTFromLeaf(pcluster->cluster_list_children[i], T, R, count);
		}
	}
}

void Kernel::configNetwork()
{
	///////////////receiver end
	//pReceiver = new HSMReceiver(HNConnectionInfo("cs72283",20000), p_kernel->p_mesh);
	//pReceiver->init();
	//pReceiver->startGenerating();


	////test if client could receive data
	//HSMTransmitter * ptest = new HSMTransmitter(HNConnectionInfo(20000));
	//ptest->startStreaming();
	//ptest->HSMupdate(p_kernel->p_mesh);
	//pTransmitter.push_back(ptest);

	switch(network_role)
	{
		case NETWORK_ROLE_NONE:
			break;
		case NETWORK_ROLE_SERVER:
			{
				//cout << numOfClients << endl;
				for(int i = 0; i < numOfClients; i++)
				{
					HSMTransmitter * ptest = new HSMTransmitter(HNConnectionInfo(port+i), flag_compressData);
					ptest->index = i;
					pTransmitter.push_back(ptest);
					ptest->startStreaming();
					cout << i+1 << " client connected. " << endl;
				}

				//server = new Server(1,DEFAULT_PORT);
				//unsigned int serThreadID;
				//serThreadHandle = _beginthreadex(0, 0, ServerThread, (void*)server,0, &serThreadID);
				break;
			}
		case NETWORK_ROLE_CLIENT:
			{
				//both ip and domain could work!
				pReceiver = new HSMReceiver(HNConnectionInfo(ip, port), p_mesh, flag_compressData);
				cout << "connect to " << ip << " at port " << port << endl;
				//pReceiver = new HSMReceiver(HNConnectionInfo("cs72283", port), p_mesh);
				pReceiver->init();
				pReceiver->startGenerating();
				pReceiver->play();
				//cout << ip << " " << port << endl;
				break;
			}
	}
	flag_network_ready = true;
}

void Kernel::connectNewVox4VoxMesh(Vox * new_vox, Vox * parent_Vox, VoxMesh * v_mesh, int parent_index, int d, int d_total, int x, int y, int z)
{
	int index = 0;
	int v_idx = 0;
	//condition 1, node 4, check, check, check
	if (x-1>=0 && y-1>=0 && z-1>=0)
	{
		v_idx = parent_index *d*d*d + (z-1)*d*d + (y-1)*d + x-1;
		if (v_mesh->vox_locator[v_idx] != NULL)
		{
			new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_2;
		}
	}
	if(x == 0 && y == 0 && z == 0)//node 4 has potential 7 boundary neighbors
	{
		//bottom left back
		if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[1] >= 1 && parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2] - 1;
			v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_2;
			}
		}
		//bottom left
		if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[1] >= 1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2] - 1;
			v_idx = index *d*d*d + (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_2;
			}
		}
		//bottom back
		if(parent_Vox->coord_grid[0] >= 1 && parent_Vox->coord_grid[1] >= 1)
		{
			index = (parent_Vox->coord_grid[0]-1)*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d +(d-1)*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_2;
			}
		}
		//bottom
		if(parent_Vox->coord_grid[1] >= 1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_3;
			}
		}
		//left
		if(parent_Vox->coord_grid[2] >= 1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2] - 1;
			v_idx = index *d*d*d + (d-1);
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_1;
			}
		}
		//back
		if(parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0]-1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_7;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_3;
			}
		}
		//left back
		if(parent_Vox->coord_grid[0] >= 1 && parent_Vox->coord_grid[2] >= 1)
		{
			index = (parent_Vox->coord_grid[0]-1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + (d-1)*d*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_2;
			}
		}
	}
	//condition 2, line from node 4 to node 5, check, check, check
	if (y-1>=0 && z-1>=0)
	{
		v_idx = parent_index *d*d*d + (z-1)*d*d + (y-1)*d + x;
		if (v_mesh->vox_locator[v_idx] != NULL)
		{
			new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_3;
			new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_2;
		}
	}
	if(y == 0 && z == 0)//line 4-5 has potential 9 boundary neighbors
	{
		//left, right are not considered
		if(parent_Vox->coord_grid[1] >= 1 && parent_Vox->coord_grid[0] >= 1)
		{
			//bottom back
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d + x;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_2;			
			}
			//bottom left back
			if(x > 0)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_2;			
				}
			}
			//bottom right back
			if(x < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_3;			
				}
			}
		}
		if(parent_Vox->coord_grid[1] >= 1)
		{
			//bottom 
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d + x;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_3;				
			}
			//bottom left
			if(x > 0)
			{
				v_idx = index *d*d*d + (d-1)*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_1;
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_2;
				}
			}
			//bottom right
			if(x < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_0;
					new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_3;
				}
			}
		}
		//back
		if(parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d + x;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_7;			
			}
			//left back
			if(x > 0)
			{
				v_idx = index *d*d*d + (d-1)*d*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_6;
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_2;
				}
			}
			//right back
			if(x < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_3;
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_7;
				}
			}
		}
	}
	//condition 3, node 5, check, check, check
	if (x+1<d && y-1>=0 && z-1>=0)
	{
		v_idx = parent_index *d*d*d + (z-1)*d*d + (y-1)*d + x+1;
		if (v_mesh->vox_locator[v_idx] != NULL)
		{
			new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_3;
		}
	}
	if(x == d-1 && y == 0 && z ==0)//node 5 has potential 9 boundary neighbors
	{
		//right bottom back
		if(parent_Vox->coord_grid[2] < d_total-1 && parent_Vox->coord_grid[1] >= 1 && parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2] + 1;
			v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_3;				
			}
		}
		//right bottom
		if(parent_Vox->coord_grid[2] < d_total-1 && parent_Vox->coord_grid[1] >= 1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2] + 1;
			v_idx = index *d*d*d + (d-1)*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_3;
			}
		}
		//right
		if(parent_Vox->coord_grid[2] < d_total-1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2] + 1;
			v_idx = index *d*d*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_4;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_7;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_0;
			}
		}
		//bottom
		if(parent_Vox->coord_grid[1] >= 1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + z*d*d + (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_3;
			}
		}
		//bottom back
		if(parent_Vox->coord_grid[1] >= 1 && parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_3;
			}
		}
		//right back
		if(parent_Vox->coord_grid[2] < d_total-1 && parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2] + 1;
			v_idx = index *d*d*d + (d-1)*d*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_7;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_3;
			}
		}
		//back
		if(parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0] - 1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_7;
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_2;
			}
		}
	}
	//condition 4, line from node 0 to node 4, check,check, check
	if (x-1>=0 && z-1>=0)
	{
		v_idx = parent_index *d*d*d + (z-1)*d*d + y*d + x-1;
		if (v_mesh->vox_locator[v_idx] != NULL)
		{
			new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_2;
			new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_6;
		}
	}
	if (x == 0 && z == 0)//line 0-4 has potential 9 boundary neighbors
	{
		//top and bottom are not considered
		if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[0] >= 1)
		{
			//left back
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2] - 1;
			v_idx = index *d*d*d + (d-1)*d*d + y*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_6;	
			}
			//top left back
			if(y < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y+1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_6;	
				}
			}
			//bottom left back
			if(y > 0)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y-1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_2;	
				}
			}
		}
		if(parent_Vox->coord_grid[2] >= 1)
		{
			//left
			index = parent_Vox->coord_grid[0]*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2] - 1;
			v_idx = index *d*d*d + y*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_2;		
			}
			//top left
			if(y < d-1)
			{
				v_idx = index *d*d*d + (y+1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_5;
					new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_6;		
				}
			}
			//bottom left
			if(y > 0)
			{
				v_idx = index *d*d*d + (y-1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_1;
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_2;		
				}
			}
		}
		if(parent_Vox->coord_grid[0] >= 1)
		{
			//back
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d + y*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_7;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_2;				
			}
			//top back
			if(y < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y+1)*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_7;
					new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_6;				
				}
			}
			//bottom back
			if(y > 0)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y-1)*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_3;
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_2;				
				}
			}
		}
	}
	//condition 5, rectangle by node 0, 1, 5, 4, check, check, check
	if (z-1>=0)
	{
		v_idx = parent_index *d*d*d + (z-1)*d*d + y*d + x;
		if (v_mesh->vox_locator[v_idx] != NULL)
		{
			new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_3;
			new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_2;
			new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_6;
			new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_7;
		}
	}
	if (z == 0)
	{
		if(parent_Vox->coord_grid[0] >= 1)//face 0154 has potential 4 neighbors
		{
			//back
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d + y*d + x;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_7;		
			}
			//top back
			if(y < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y+1)*d + x;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_7;
					new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_6;		
				}
			}
			//bottom back
			if(y > 0)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y-1)*d + x;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_3;
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_2;		
				}
			}
			//left back
			if(x > 0)
			{
				v_idx = index *d*d*d + (d-1)*d*d + y*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_2;
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_6;		
				}
			}
			//right back
			if(x < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d*d + y*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_3;
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_7;		
				}
			}
			//top left back
			if(x > 0 && y < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y+1)*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_6;		
				}
			}
			//top right back
			if(x < d-1 && y < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y+1)*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_7;		
				}
			}
			//bottom left back
			if(x > 0 && y > 0)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y-1)*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_2;		
				}
			}
			//bottom right back
			if(x < d-1 && y > 0)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y-1)*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_3;		
				}
			}
		}
	}
	//condition 6, line from node 1 to node 5, check,check, check
	if (x+1<d && z-1>=0)
	{
		v_idx = parent_index *d*d*d + (z-1)*d*d + y*d + x+1;
		if (v_mesh->vox_locator[v_idx] != NULL)
		{
			new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_3;
			new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_7;
		}
	}
	if(x == d-1 && z == 0)//line 1-5 has potential 9 neighbors
	{
		//top, bottom are not considered
		if(parent_Vox->coord_grid[2] < d_total-1 &&  parent_Vox->coord_grid[0] >= 1)
		{
			//right back
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2] + 1;
			v_idx = index *d*d*d + (d-1)*d*d + y*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_7;		
			}
			//top right back
			if(y < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y+1)*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_7;		
				}
			}
			//bottom right back
			if(y > 0)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y-1)*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_3;		
				}
			}
		}
		if(parent_Vox->coord_grid[2] < d_total-1)
		{
			//right
			index = parent_Vox->coord_grid[0]*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2] + 1;
			v_idx = index *d*d*d + y*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_4;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_7;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_3;		
			}
			//top right
			if(y < d-1)
			{
				v_idx = index *d*d*d + (y+1)*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_4;
					new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_7;		
				}
			}
			//bottom right
			if(y > 0)
			{
				v_idx = index *d*d*d + (y-1)*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_0;
					new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_3;		
				}
			}
		}		
		if(parent_Vox->coord_grid[0] >= 1)
		{
			//back
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d + y*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_7;
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_3;			
			}
			//top back
			if(y < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y+1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_6;
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_7;			
				}
			}
			//bottom back
			if(y > 0)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y-1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_2;
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_3;			
				}
			}
		}

	}
	//condition 7, node 0, check, check, check
	if (x-1>=0 && y+1<d && z-1>=0)
	{
		v_idx = parent_index *d*d*d + (z-1)*d*d + (y+1)*d + x-1;
		if (v_mesh->vox_locator[v_idx] != NULL)
		{
			new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_6;
		}
	}
	if(x == 0 && y == d-1 && z == 0)//node 0 has 7 potential neighbors
	{
		//top left back
		if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[1] < d_total-1 &&  parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2] - 1;
			v_idx = index *d*d*d + (d-1)*d*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_6;		
			}
		}
		//left back
		if(parent_Vox->coord_grid[2] >= 1 &&  parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2] - 1;
			v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_6;		
			}
		}
		//left
		if(parent_Vox->coord_grid[2] >= 1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2] - 1;
			v_idx = index *d*d*d + (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_2;		
			}
		}
		//back
		if(parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_3;	
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_7;	
			}
		}
		//top left
		if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[1] < d_total-1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2] - 1;
			v_idx = index *d*d*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_6;		
			}
		}
		//top back
		if(parent_Vox->coord_grid[1] < d_total-1 &&  parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_7;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_6;
			}
		}
		//top
		if(parent_Vox->coord_grid[1] < d_total-1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_4;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_7;		
			}
		}
	}
	//condition 8, line from node 0 to node 1, check, check, check
	if (y+1<d && z-1>=0)
	{
		v_idx = parent_index *d*d*d + (z-1)*d*d + (y+1)*d + x;
		if (v_mesh->vox_locator[v_idx] != NULL)
		{
			new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_7;
			new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_6;
		}
	}
	if(y == d-1 && z == 0)//line 0-1 has 9 potential neighbors
	{
		//left, right are not considered
		if(parent_Vox->coord_grid[1] < d_total-1 &&  parent_Vox->coord_grid[0] >= 1)
		{
			//top back
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d + x;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_7;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_6;
			}
			//top left back
			if(x > 0)
			{
				v_idx = index *d*d*d + (d-1)*d*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_6;
				}
			}
			//top right back
			if(x < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_7;
				}
			}
		}
		if(parent_Vox->coord_grid[1] < d_total-1)
		{
			//top
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + x;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_4;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_5;	
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_7;	
			}
			//top left
			if(x > 0)
			{
				v_idx = index *d*d*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_5;
					new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_6;		
				}
			}
			//top right
			if(x < d-1)
			{
				v_idx = index *d*d*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_4;
					new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_7;		
				}
			}
		}
		if(parent_Vox->coord_grid[0] >= 1)
		{
			//back
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d + x;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_7;
			}
			//left back
			if(x > 0)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_2;
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_6;
				}
			}
			//right back
			if(x < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_3;
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_7;
				}
			}
		}
	}
	//condition 9, node 1, check, check, check
	if (x+1<d && y+1<d && z-1>=0)
	{
		v_idx = parent_index *d*d*d + (z-1)*d*d + (y+1)*d + x+1;
		if (v_mesh->vox_locator[v_idx] != NULL)
		{
			new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_7;
		}
	}
	if(x == d-1 && y == d-1 && z == 0)//node 1 has potential 7 neighbors
	{
		//top right back
		if(parent_Vox->coord_grid[2] < d_total-1 && parent_Vox->coord_grid[1] < d_total-1 &&  parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2]+1;
			v_idx = index *d*d*d + (d-1)*d*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_7;			
			}
		}
		//top right
		if(parent_Vox->coord_grid[2] < d_total-1 && parent_Vox->coord_grid[1] < d_total-1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2]+1;
			v_idx = index *d*d*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_4;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_7;
			}
		}
		//top back
		if(parent_Vox->coord_grid[1] < d_total-1 &&  parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_7;
			}
		}
		//top
		if(parent_Vox->coord_grid[1] < d_total-1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_4;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_7;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_6;
			}
		}
		//right back
		if(parent_Vox->coord_grid[2] < d_total-1 &&  parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + parent_Vox->coord_grid[1]*d_total + parent_Vox->coord_grid[2]+1;
			v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_7;
			}
		}
		//right
		if(parent_Vox->coord_grid[2] < d_total-1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]+1;
			v_idx = index *d*d*d + (d-1)*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_4;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_7;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_3;
			}
		}
		//back
		if(parent_Vox->coord_grid[0] >= 1)
		{
			index = (parent_Vox->coord_grid[0] - 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_7;
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_3;
			}
		}
	}
	//condition 10, line from node 4 to node 7, check, check, check
	if (x-1>=0 && y-1>=0)
	{
		v_idx = parent_index *d*d*d + z*d*d + (y-1)*d + x-1;
		if (v_mesh->vox_locator[v_idx] != NULL)
		{
			new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_1;
			new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_2;
		}
	}
	if(x == 0 && y == 0)//line 4-7 has potential 9 neighbors
	{
		//front, back are not considered
		if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[1] >= 1)
		{
			//bottom left
			index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + z*d*d + (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_2;	
			}
			//bottom left front
			if(z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d + (d-1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_1;	
				}
			}
			//bottom left back
			if(z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d + (d-1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_2;
				}
			}
		}
		if(parent_Vox->coord_grid[1] >= 1)
		{
			//bottom
			index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + z*d*d + (d-1)*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_1;	
			}
			//bottom front
			if(z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d + (d-1)*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_0;
					new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_1;	
				}
			}
			//bottom back
			if(z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d + (d-1)*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_3;
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_2;	
				}
			}
		}
		if(parent_Vox->coord_grid[2] >= 1)
		{
			//left
			index = parent_Vox->coord_grid[0] *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + z*d*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_1;	
			}
			//left front
			if(z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_5;	
				}
			}
			//left back
			if(z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_6;	
				}
			}
		}
	}
	//condition 11, line by node 6, 7, check, check, check
	if(y == 0 && z == d-1)// line 6-7 has 9 potential neighbors
	{
		//left, right are not considered
		if(parent_Vox->coord_grid[1] >= 1 && parent_Vox->coord_grid[0] < d_total-1)
		{
			//bottom front
			index = (parent_Vox->coord_grid[0]+1) *d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d + x;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_0;	
			}
			//bottom left front
			if(x > 0)
			{
				v_idx = index *d*d*d + (d-1)*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_1;	
				}
			}
			//bottom right front
			if(x < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_0;	
				}
			}
		}
		if(parent_Vox->coord_grid[1] >= 1)
		{
			//bottom
			index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d + x;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_1;		
			}
			//bottom left
			if(x > 0)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_1;
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_2;	
				}
			}
			//bottom right
			if(x < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_0;
					new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_3;	
				}
			}
		}
		if(parent_Vox->coord_grid[0] < d_total-1)
		{
			//front
			index = (parent_Vox->coord_grid[0]+1) *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + x;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_4;	
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_1;
			}
			//left front
			if(x > 0)
			{
				v_idx = index *d*d*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_5;
					new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_1;	
				}
			}
			//right front
			if(x < d-1)
			{
				v_idx = index *d*d*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_4;
					new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_0;	
				}
			}
		}
	}
	//condition 12, node 7, check, check, check
	if(x == 0 && y == 0 && z == d-1)//node 7 has 7 potential neighbors
	{
		//bottom left front
		if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[1] >= 1 &&  parent_Vox->coord_grid[0] < d_total-1)
		{
			index = (parent_Vox->coord_grid[0] + 1)*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_1;			
			}
		}
		//bottom left
		if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[1] >= 1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_2;	
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_1;		
			}
		}
		//bottom
		if(parent_Vox->coord_grid[1] >= 1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_2;			
			}
		}
		//left
		if(parent_Vox->coord_grid[2] >= 1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + (d-1)*d*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_5;			
			}
		}
		//left front
		if(parent_Vox->coord_grid[2] >= 1 &&  parent_Vox->coord_grid[0] < d_total-1)
		{
			index = (parent_Vox->coord_grid[0] + 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_1;			
			}
		}
		//bottom front
		if(parent_Vox->coord_grid[1] >= 1 && parent_Vox->coord_grid[0] < d_total-1)
		{
			index = (parent_Vox->coord_grid[0] + 1)*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_1;			
			}
		}
		//front
		if(parent_Vox->coord_grid[0] < d_total-1)
		{
			index = (parent_Vox->coord_grid[0]+1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_4;	
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_0;		
			}
		}
	}
	//condition 13, node 3, check, check, check
	if(x == 0 && y == d-1 && z == d-1)
	{
		//up left front
		if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[1] < d_total-1 &&  parent_Vox->coord_grid[0] < d_total-1)
		{
			index = (parent_Vox->coord_grid[0] + 1)*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_5;			
			}
		}
		//up left
		if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[1] < d_total-1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + (d-1)*d*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_5;			
			}
		}
		//up
		if(parent_Vox->coord_grid[1] < d_total-1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_7;	
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_4;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_6;		
			}
		}
		//left
		if(parent_Vox->coord_grid[2] >= 1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + (d-1)*d*d+ (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_6;			
			}
		}
		//up front
		if(parent_Vox->coord_grid[1] < d_total-1 &&  parent_Vox->coord_grid[0] < d_total-1)
		{
			index = (parent_Vox->coord_grid[0] + 1)*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_4;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_5;				
			}
		}
		//left front
		if(parent_Vox->coord_grid[2] >= 1 &&  parent_Vox->coord_grid[0] < d_total-1)
		{
			index = (parent_Vox->coord_grid[0] + 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_5;			
			}
		}
		//front
		if(parent_Vox->coord_grid[0] < d_total-1)
		{
			index = (parent_Vox->coord_grid[0] + 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_4;			
			}
		}
	}
	//condition 14, line by node 0, 3, check, check, check
	if(x == 0 && y == d-1)
	{
		//front, back are not considered.
		if(parent_Vox->coord_grid[2] >= 1 && parent_Vox->coord_grid[1] < d_total-1)
		{
			//up left
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + z*d*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_6;
			}
			//up left front
			if(z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{								
					new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_5;
				}
			}
			//up left back
			if(z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{								
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_6;
				}
			}
		}
		if(parent_Vox->coord_grid[1] < d_total-1)
		{
			//up
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] + 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + z*d*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_4;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_7;
			}
			//up front
			if(z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{								
					new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_4;
					new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_5;
				}
			}
			//up back
			if(z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{								
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_7;
					new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_6;
				}
			}
		}
		if(parent_Vox->coord_grid[2] >= 1)
		{
			//left
			index = parent_Vox->coord_grid[0]*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + z*d*d +(d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_5;
			}
			//left front
			if(z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d +(d-1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{								
					new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_1;
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_5;
				}
			}
			//left back
			if(z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d +(d-1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{								
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_2;
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_6;
				}
			}
		}
	}
	//condition 15, line by node 3, 7, check, check
	if(x == 0 && z == d-1)//line 3-7 may have potential 9 neighbors
	{
		if(parent_Vox->coord_grid[2] >= 1 &&  parent_Vox->coord_grid[0] < d_total-1)
		{
			//left front
			index = (parent_Vox->coord_grid[0] + 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + y*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_5;
			}
			//up left front
			if(y < d-1)
			{
				v_idx = index *d*d*d + (y+1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{								
					new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_5;
				}
			}
			//down left front
			if(y > 0)
			{
				v_idx = index *d*d*d + (y-1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{								
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_1;
				}
			}
		}
		if(parent_Vox->coord_grid[2] >= 1)
		{
			//left
			index = parent_Vox->coord_grid[0]*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d +(d-1)*d*d + y*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_1;
			}
			//up left
			if(y < d-1)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y+1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{								
					new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_6;
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_5;
				}
			}
			//down left
			if(y > 0)
			{
				v_idx = index *d*d*d + (d-1)*d*d + (y-1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{								
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_2;
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_1;
				}
			}
		}
		if(parent_Vox->coord_grid[0] < d_total-1)
		{
			//front
			index = (parent_Vox->coord_grid[0] + 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + y*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_4;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_1;
			}
			//top front
			if(y < d-1)
			{
				v_idx = index *d*d*d + (y+1)*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{								
					new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_4;
					new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_5;
				}
			}
			//down front
			if(y > 0)
			{
				v_idx = index *d*d*d + (y-1)*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{								
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_0;
					new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_1;
				}
			}
		}
		
	}
	//condition 16, line from node 5 to node 6, check, check, check
	if (x+1<d && y-1>=0)
	{
		v_idx = parent_index *d*d*d + z*d*d + (y-1)*d + x+1;
		if (v_mesh->vox_locator[v_idx] != NULL)
		{
			new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_0;
			new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_3;
		}
	}
	if(x == d-1 && y == 0)//line 5, 6 has 9 potential neighbors
	{
		if(parent_Vox->coord_grid[2] < d_total-1 && parent_Vox->coord_grid[1] >= 1)
		{
			//bottom right
			index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2] + 1;
			v_idx = index *d*d*d + z*d*d + (d-1)*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_3;		
			}
			//bottom right front
			if(z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d + (d-1)*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_0;		
				}
			}
			//bottom right back
			if(z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d + (d-1)*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_3;	
				}
			}
		}
		if(parent_Vox->coord_grid[1] >= 1)
		{
			//bottom
			index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + z*d*d + (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_0;	
			}
			//bottom front
			if(z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d + (d-1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_1;
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_0;	
				}
			}
			//bottom back
			if(z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d + (d-1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_2;
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_3;	
				}
			}
		}
		if(parent_Vox->coord_grid[2] < d_total-1)
		{
			//right
			index = parent_Vox->coord_grid[0] *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]+1;
			v_idx = index *d*d*d + z*d*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_4;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_7;	
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_0;
			}
			//right front
			if(z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_4;
					new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_0;	
				}
			}
			//right back
			if(z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_7;
					new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_3;	
				}
			}
		}
	}
	//condition 17, node 6, check, check, check
	if(x == d-1 && y == 0 && z == d-1)//node 6 has 7 potential neighbors
	{
		//bottom right front
		if(parent_Vox->coord_grid[2] < d_total-1 && parent_Vox->coord_grid[1] >= 1 &&  parent_Vox->coord_grid[0] < d_total-1)
		{
			index = (parent_Vox->coord_grid[0] + 1)*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2]+1;
			v_idx = index *d*d*d + (d-1)*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_0;			
			}
		}
		//bottom right
		if(parent_Vox->coord_grid[2] < d_total-1 && parent_Vox->coord_grid[1] >= 1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2]+1;
			v_idx = index *d*d*d + (d-1)*d*d + (d-1)*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_0;			
			}
		}
		//bottom front
		if(parent_Vox->coord_grid[1] >= 1 &&  parent_Vox->coord_grid[0] < d_total-1)
		{
			index = (parent_Vox->coord_grid[0]+1)*d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_1;	
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_0;		
			}
		}
		//bottom
		if(parent_Vox->coord_grid[1] >= 1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + (parent_Vox->coord_grid[1]-1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + (d-1)*d*d+ (d-1)*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_3;			
			}
		}
		//right front
		if(parent_Vox->coord_grid[2] < d_total-1 && parent_Vox->coord_grid[0] < d_total-1)
		{
			index = (parent_Vox->coord_grid[0] + 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]+1;
			v_idx = index *d*d*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_4;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_0;				
			}
		}
		//right
		if(parent_Vox->coord_grid[2] < d_total-1)
		{
			index = parent_Vox->coord_grid[0]*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]+1;
			v_idx = index *d*d*d + (d-1)*d*d;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_7;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_3;
				new_vox->node_1 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_4;			
			}
		}
		//front
		if(parent_Vox->coord_grid[0] < d_total-1)
		{
			index = (parent_Vox->coord_grid[0] + 1)*d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{								
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_4;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_2 = v_mesh->vox_locator[v_idx]->node_1;			
			}
		}
	}
	//condition 18, rectangle by node 4, 5, 6, 7, check, check, check
	if (y-1>=0)
	{
		v_idx = parent_index *d*d*d + z*d*d + (y-1)*d + x;
		if (v_mesh->vox_locator[v_idx] != NULL)
		{
			new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_0;
			new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_1;
			new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_2;
			new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_3;
		}
	}
	if(y == 0)//face 4567 has 5 potential neighbors
	{
		if(parent_Vox->coord_grid[1] >= 1)
		{
			//bottom
			index = parent_Vox->coord_grid[0] *d_total*d_total + (parent_Vox->coord_grid[1] - 1) *d_total + parent_Vox->coord_grid[2];
			v_idx = index *d*d*d + z*d*d + (d-1)*d + x;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_0;
				new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_2;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_3;
			}
			
			//bottom front
			if(z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d + (d-1)*d + x;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_1;
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_0;
				}
			}
			//bottom back
			if(z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d + (d-1)*d + x;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_3;
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_2;
				}
			}
			//bottom left 
			if(x > 0)
			{
				v_idx = index *d*d*d + z*d*d + (d-1)*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_1;
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_2;
				}
			}
			//bottom right
			if(x < d-1)
			{
				v_idx = index *d*d*d + z*d*d + (d-1)*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_0;
					new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_3;
				}
			}
			//bottom left front 
			if(x > 0 && z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d + (d-1)*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_1;
				}
			}
			//bottom right front 
			if(x < d-1 && z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d + (d-1)*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_6 = v_mesh->vox_locator[v_idx]->node_0;
				}
			}
			//bottom left back
			if(x > 0 && z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d + (d-1)*d + x-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_2;
				}
			}
			//bottom right back
			if(x < d-1 && z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d + (d-1)*d + x+1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_5 = v_mesh->vox_locator[v_idx]->node_3;
				}
			}
			
		}
	}
	//condition 19, rectangle by node 0, 4, 3, 7, check, check, check
	if (x-1>=0)
	{
		v_idx = parent_index *d*d*d + z*d*d + y*d + x-1;
		if (v_mesh->vox_locator[v_idx] != NULL)
		{
			new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_1;
			new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_5;
			new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_6;
			new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_2;
		}
	}
	if(x == 0)
	{
		if(parent_Vox->coord_grid[2] >= 1)
		{
			//left
			index = parent_Vox->coord_grid[0] *d_total*d_total + parent_Vox->coord_grid[1] *d_total + parent_Vox->coord_grid[2]-1;
			v_idx = index *d*d*d + z*d*d + y*d + d-1;
			if (v_mesh->vox_locator[v_idx] != NULL)
			{
				new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_1;
				new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_5;
				new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_6;
				new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_2;	
			}
			
			//left front
			if(z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d + y*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_5;
					new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_1;	
				}
			}
			//left back
			if(z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d + y*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_2;
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_6;	
				}
			}
			//top left
			if(y < d-1)
			{
				v_idx = index *d*d*d + z*d*d + (y+1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_5;
					new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_6;	
				}
			}
			//bottom left
			if(y > 0)
			{
				v_idx = index *d*d*d + z*d*d + (y-1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_1;
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_2;	
				}
			}
			//top left back
			if(y < d-1 && z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d + (y+1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_0 = v_mesh->vox_locator[v_idx]->node_6;	
				}
			}
			//bottom left back
			if(y > 0 && z > 0)
			{
				v_idx = index *d*d*d + (z-1)*d*d + (y-1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_4 = v_mesh->vox_locator[v_idx]->node_2;	
				}
			}
			//top left front
			if(y < d-1 && z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d + (y+1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_3 = v_mesh->vox_locator[v_idx]->node_5;	
				}
			}
			//bottom left front
			if(y > 0 && z < d-1)
			{
				v_idx = index *d*d*d + (z+1)*d*d + (y-1)*d + d-1;
				if (v_mesh->vox_locator[v_idx] != NULL)
				{
					new_vox->node_7 = v_mesh->vox_locator[v_idx]->node_1;	
				}
			}
			
		}
	}
}

void Kernel::resetMass4Level()
{
	for (int i = 0; i < level_list.size(); i++)
	{
		vector<Node>::iterator ni = level_list[i]->voxmesh_level->node_list.begin();
		for ( ;ni != level_list[i]->voxmesh_level->node_list.end(); ++ni)
		{
			ni->mass = 1.0;
			vector<DuplicatedNode *>::iterator di = ni->duplicates.begin();
			for (; di != ni->duplicates.end(); di++)
			{
				(*di)->mass = 1.0;
			}
		}

		vector<Cluster>::iterator ci = level_list[i]->voxmesh_level->cluster_list.begin();
		for ( ;ci != level_list[i]->voxmesh_level->cluster_list.end(); ++ci)
		{
			ci->computeRestMassCentroid();
			ci->computeAQQ();
		}
	}
}

void Kernel::test_findPointOutside()
{
	int count = 0;
	for (node_iterator nmi=p_mesh->node_list.begin(); nmi!=p_mesh->node_list.end(); ++nmi)
	{
		double maxX, maxY, maxZ, minX, minY, minZ;
		maxX = minX = nmi->list_interpolation_nodes[0][0]->displacement.x() + nmi->list_interpolation_nodes[0][0]->coordinate.x();
		maxY = minY = nmi->list_interpolation_nodes[0][0]->displacement.y() + nmi->list_interpolation_nodes[0][0]->coordinate.y();
		maxZ = minZ = nmi->list_interpolation_nodes[0][0]->displacement.z() + nmi->list_interpolation_nodes[0][0]->coordinate.z();
		for(int i = 1; i < 8; i++)
		{
			Vector3d temp;
			temp = nmi->list_interpolation_nodes[i][0]->displacement + nmi->list_interpolation_nodes[i][0]->coordinate;
			maxX = temp.x()>maxX?temp.x():maxX;
			maxY = temp.y()>maxY?temp.y():maxY;
			maxZ = temp.z()>maxZ?temp.z():maxZ;
			minX = temp.x()<minX?temp.x():minX;
			minY = temp.y()<minY?temp.y():minY;
			minZ = temp.z()<minZ?temp.z():minZ;
		}
		//Vector3d pos = nmi->coordinate + nmi->displacement;
		//if(pos.x() < (minX - 0.1) || pos.y() < (minY -0.1) || pos.z() < (minZ-0.1))
		//	cout << "count min: "<< count << endl;
		//if(pos.x() > (maxX + 0.1) || pos.y() > (maxY+0.1) || pos.z() > (maxZ+0.1))
		//	cout << "count max: "<< count << endl;
		count ++;
	}
}

void Kernel::test()
{
	/*
	Matrix3d r1 = Matrix3d::Identity();
	Matrix3d r2 = Matrix3d::Zero();
	r2(0, 0) = 1;
	r2(1, 1) = pow(3.0, 0.5)/2.0;
	r2(1, 2) = -0.5;
	r2(2, 1) = 0.5;
	r2(2, 2) = pow(3.0, 0.5)/2.0;
	Matrix3d r3 = Matrix3d::Zero();
	r3(0, 0) = 1;
	r3(1, 2) = -1;
	r3(2, 1) = 1;
	Matrix3d r4 = Matrix3d::Zero();
	r4(0, 0) = 1;
	r4(1, 1) = -0.5;
	r4(1, 2) = (-1.0) * pow(3.0, 0.5)/2.0;
	r4(2, 1) = pow(3.0, 0.5)/2;
	r4(2, 2) = 0.5;
	Quaterniond d1(r1);
	Quaterniond d2(r2);
	Quaterniond d3(r3);
	Quaterniond d4(r4);
	Quaterniond t1 = d1.slerp(0.5, d2);
	Quaterniond t2 = t1.slerp(1.0/3.0, d3);
	Quaterniond t3 = t2.slerp(0.25, d4);
	cout << t3.w() << endl;

	double q0 = exp(0.5 * log(d1.w()) + 0.5* log(d2.w()));
	double q1 = exp(0.5 * log(d2.x()));
	double q2 = exp(0.5 * log(d1.y()) + 0.5* log(d2.y()));
	double q3 = exp(0.5 * log(d1.z()) + 0.5* log(d2.z()));
	//double q0 = exp(0.25 * (log(d1.w()* d2.w()*d3.w()*d4.w())));
	//double q1 = exp(0.25 * (log(d1.x()* d2.x()*d3.x()*d4.x())));
	//double q2 = exp(0.25 * (log( d1.y()* d2.y()*d3.y()*d4.y())));
	//double q3 = exp(0.25 * (log( d1.z()* d2.z()*d3.z()*d4.z())));
	cout << r3 << endl;
	cout << log(0.0) << endl;
	cout << d1.w() << " " << d1.x() << " " << d1.y() << " " << d1.z() << endl;
	cout << d3.w() << " " << d3.x() << " " << d3.y() << " " << d3.z() << endl;
	cout << d2.w() << " " << d2.x() << " " << d2.y() << " " << d2.z() << endl;
	cout << endl;
	cout << q0 << endl;
	cout << q1 << endl;
	cout << q2 << endl;
	cout << q3 << endl;
	*/
	/*
	//test if leaf list is right
	vector<Cluster>::iterator ci;
	for(int i = 0; i < level_list.size(); i ++)
	{
		cout << "level " << i << endl;
		for (ci = level_list[i]->voxmesh_level->cluster_list.begin(); ci != level_list[i]->voxmesh_level->cluster_list.end(); ci++)
		{
			cout << ci->leaf_list.size() << endl;
		}
	}
	*/
	/*
	vector<Node>::iterator ni;
	for (ni = level_list[0]->voxmesh_level->node_list.begin(); ni!= level_list[0]->voxmesh_level->node_list.end(); ni++)
	{
		Vector3d temp = ni->coordinate + ni->displacement;
		if (temp == ni->target_position)
		{
			cout << "good!" << endl;
		}
	}
	*/
	//test if obj file could include 2 objects.
	//exportToOBJ("test.obj");
	//cout << "export OK! "<<endl;

	Vector3d v1, v2, axis;
	v1(0) = 1;
	v1(1) = 0;
	v1(2) = 0;
	v1.normalize();

	v2(0) = 0;
	v2(1) = 1;
	v2(2) = 0;

	//cout << v1 << endl;

	axis = v1.cross(v2);
	axis.normalize();
	double angle = (-1) * acos(0.0);

	Matrix3d _rotation;
	_rotation.setZero();
	_rotation(0, 0) = cos(angle) + axis(0) * axis(0) * (1 - cos(angle));
	_rotation(0, 1) = axis(0) * axis(1) * (1 - cos(angle)) - axis(2) * sin(angle);
	_rotation(0, 2) = axis(0) * axis(2) * (1 - cos(angle)) + axis(1) * sin(angle);
	_rotation(1, 0) = axis(1) * axis(0) * (1 - cos(angle)) + axis(2) * sin(angle);
	_rotation(1, 1) = cos(angle) + axis(1) * axis(1) * (1 - cos(angle));
	_rotation(1, 2) = axis(1) * axis(2) * (1 - cos(angle)) - axis(0) * sin(angle);
	_rotation(2, 0) = axis(2) * axis(0) * (1 - cos(angle)) - axis(1) * sin(angle);
	_rotation(2, 1) = axis(2) * axis(1) * (1 - cos(angle)) + axis(0) * sin(angle);
	_rotation(2, 2) = cos(angle) + axis(2) * axis(2) * (1 - cos(angle));
	cout << (_rotation * v2) << endl;
	cout << (_rotation * v1) << endl;
}

void Kernel::findSurfaceNode()
{
	//int l = level_list.size() - 1;
	//for (int i = 0; i < p_mesh->node_list.size(); i ++)
	//{
	//	for (int j = 0; j < 8; j++)
	//	{
	//		Node * temp = p_mesh->node_list[i].list_interpolation_nodes[j][l];
	//	}
	//}
}