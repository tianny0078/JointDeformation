/************************************************************************/
/* the implementation of the mesh
   by Yin Yang @ UTD
*/
/************************************************************************/
#include "mesh.h"

Node::Node()
{
	
	flag_visited = false;
	flag_anchor_node = false;
	flag_surface_node = false;
	flag_trigger_node = false;
	flag_constraint_node = false;

	mass = 1.0;
	external_node_idx = -1;
	external_ele_idx = -1;

	index = -1;

	coordinate = Vector3d::Zero();
	displacement = Vector3d::Zero();
	normal = Vector3d::Zero();
	cur_normal = Vector3d::Zero();
	velocity = Vector3d::Zero();
	static_position = Vector3d::Zero();
	target_position = Vector3d::Zero();
	force = Vector3d::Zero();
	prescribed_position = Vector3d::Zero();
	prescribed_preposition = Vector3d::Zero();
	prescribed_rotation = Matrix3d::Identity();
	prescribed_prerotation = Matrix3d::Identity();
	linked_duplictae = NULL;

	prescribed_linear_velocity = Vector3d::Zero();
	prescribed_angular_velocity = Vector3d::Zero();

	child_node = NULL;

	//interpolation realted
	int i = 0;
	while(i < 8)
	{
		list_interpolation_nodes[i] = NULL;
		para_interpolate[i] = 0.0;
		i++;
	}
}


DuplicatedNode::DuplicatedNode()
{
	mapped_node = NULL;
	parent_dup = NULL;
	master_cluster = NULL;
	flag_visited = false;
	flag_constraint_node = false;
	
	mass = 1.0;
	external_node_idx = -1;
	external_ele_idx = -1;

	barycentric_coord[0] = barycentric_coord[1] = barycentric_coord[2] = barycentric_coord[3] = 0;

	index = -1;

	coordinate = Vector3d::Zero();
	displacement = Vector3d::Zero();
	normal = Vector3d::Zero();
	cur_normal = Vector3d::Zero();
	velocity = Vector3d::Zero();
	static_position = Vector3d::Zero();
	target_position = Vector3d::Zero();
	force = Vector3d::Zero();
	//
	rest_position = Vector3d::Zero();
}

DuplicatedNode::DuplicatedNode(Node* n)
{
	mass = n->mass;
	external_ele_idx = n->external_ele_idx;

	coordinate = n->coordinate;
	displacement = n->displacement;
	static_position = n->static_position;
	target_position = n->target_position;
	velocity = n->velocity;
	force = n->force;
	mapped_node = n;
	parent_dup = NULL;
	master_cluster = NULL;

	flag_visited = false;
	flag_constraint_node = false;
}


Face::Face()
{
	index = -1;
	area = 0;

	node0 = NULL;
	node1 = NULL;
	node2 = NULL;
}

void Face::updateArea()
{
	Vector3d p0 = node0->coordinate;
	Vector3d p1 = node1->coordinate;
	Vector3d p2 = node2->coordinate;

	p1 = p1 - p0;
	p2 = p2 - p0;

	p0 = p1.cross(p2);

	area = 0.5*p0.norm();
}

Vox::Vox()
{
	node_0 = NULL;
	node_1 = NULL;
	node_2 = NULL;
	node_3 = NULL;
	node_4 = NULL;
	node_5 = NULL;
	node_6 = NULL;
	node_7 = NULL;
	
	//node_0_child = NULL;
	//node_1_child = NULL;
	//node_2_child = NULL;
	//node_3_child = NULL;
	//node_4_child = NULL;
	//node_5_child = NULL;
	//node_6_child = NULL;
	//node_7_child = NULL;

	flag_surface_vox = false;
	flag_top_face = false; 
	flag_bottom_face = false;
	flag_left_face = false;
	flag_right_face = false;
	flag_front_face = false;
	flag_back_face = false;

	top_neighbor = NULL;
	bottom_neighbor = NULL;
	
	left_neighbor = NULL;
	right_neighbor = NULL;
	front_neighbor = NULL;
	back_neighbor = NULL;
	left_back_neighbor = NULL;
	right_back_neighbor = NULL;
	left_front_neighbor = NULL;
	right_front_neighbor = NULL;

	top_neighbor = NULL;
	top_left_neighbor = NULL;
	top_right_neighbor = NULL;
	top_front_neighbor = NULL;
	top_back_neighbor = NULL;
	top_left_back_neighbor = NULL;
	top_right_back_neighbor = NULL;
	top_left_front_neighbor = NULL;
	top_right_front_neighbor = NULL;

	bottom_neighbor = NULL;
	bottom_left_neighbor = NULL;
	bottom_right_neighbor = NULL;
	bottom_front_neighbor = NULL;
	bottom_back_neighbor = NULL;
	bottom_left_back_neighbor = NULL;
	bottom_right_back_neighbor = NULL;
	bottom_left_front_neighbor = NULL;
	bottom_right_front_neighbor = NULL;

	flag_selected = false;
	flag_visited = false;

	//x, y, z for grid coordinates
	coord_grid.setZero();
	parent_vox = NULL;
	clusterid = 0;
	//pcluster = NULL;

	//interpolation realted
	int i = 0;
	while(i < 8)
	{
		list_near_parentVox[i] = NULL;
		para_interpolate[i] = 0.0;
		i++;
	}
}

Cluster::Cluster()
{
	num_node = 0;
	original_center = Vector3d::Zero();
	current_center = Vector3d::Zero();

	a_qq = Matrix3d::Identity();
	a_pq = Matrix3d::Identity();
	a = Matrix3d::Identity();
	r = Matrix3d::Identity();
	
	a_pq_tilde.setZero();
	a_qq_tilde.setZero();
	a_tilde.setZero();
	r_tilde.setZero();

	alpha = 1.0;
	beta = 0.0;
	kappa = 0.01;

	max_x = -1e10;
	max_y = -1e10;
	max_z = -1e10;

	min_x = 1e10;
	min_y = 1e10;
	min_z = 1e10;

	location_in_hierarchy = NULL;
	flag_inverted = false;
	flag_constrained = false;

	//penalty
	inertial_penalty = 1.0;
	
	level_index = 0;

	constraint_node = NULL;
	parent_cluster = NULL;

	linear_displacement = Vector3d::Zero();
	angular_displacement = Matrix3d::Identity();

	super_parent_cluster = NULL;
	term_normlize = 1.0;

}

Cluster::~Cluster()
{
	clear();
}

void Cluster::clear()
{
	node_list.clear();
	vox_list.clear();
	trigger_node_list.clear();
	surface_quads.clear();

	num_node = 0;
	num_surface_quads = 0;

	//set pointer to be NULL
	constraint_node = NULL;
	parent_cluster = NULL;
	//for(int i = 0; i < cluster_list_children.size(); i++)
	//{
	//	Cluster * temp = cluster_list_children[i];
	//	cluster_list_children[i] = NULL;
	//	delete temp;
	//}
	if(!cluster_list_children.empty())
		cluster_list_children.clear();
}

void Cluster::initializeNodeList()
{
	if (vox_list.empty())
	{
		return;
	}

	node_list.clear();
	node_list.reserve(8*vox_list.size());

	for (vector<Vox*>::iterator vox_iter=vox_list.begin(); vox_iter!=vox_list.end(); ++vox_iter)
	{
		(*vox_iter)->node_0->flag_visited = false;
		(*vox_iter)->node_1->flag_visited = false;
		(*vox_iter)->node_2->flag_visited = false;
		(*vox_iter)->node_3->flag_visited = false;
		(*vox_iter)->node_4->flag_visited = false;
		(*vox_iter)->node_5->flag_visited = false;
		(*vox_iter)->node_6->flag_visited = false;
		(*vox_iter)->node_7->flag_visited = false;
	}

	for (vector<Vox*>::iterator vox_iter=vox_list.begin(); vox_iter!=vox_list.end(); ++vox_iter)
	{
		if (!(*vox_iter)->node_0->flag_visited)
		{
			DuplicatedNode new_node((*vox_iter)->node_0);
			(*vox_iter)->node_0->flag_visited = true;
			node_list.push_back(new_node);
		}

		if (!(*vox_iter)->node_1->flag_visited)
		{
			DuplicatedNode new_node((*vox_iter)->node_1);
			(*vox_iter)->node_1->flag_visited = true;
			node_list.push_back(new_node);
		}

		if (!(*vox_iter)->node_2->flag_visited)
		{
			DuplicatedNode new_node((*vox_iter)->node_2);
			(*vox_iter)->node_2->flag_visited = true;
			node_list.push_back(new_node);
		}

		if (!(*vox_iter)->node_3->flag_visited)
		{
			DuplicatedNode new_node((*vox_iter)->node_3);
			(*vox_iter)->node_3->flag_visited = true;
			node_list.push_back(new_node);
		}

		if (!(*vox_iter)->node_4->flag_visited)
		{
			DuplicatedNode new_node((*vox_iter)->node_4);
			(*vox_iter)->node_4->flag_visited = true;
			node_list.push_back(new_node);
		}

		if (!(*vox_iter)->node_5->flag_visited)
		{
			DuplicatedNode new_node((*vox_iter)->node_5);
			(*vox_iter)->node_5->flag_visited = true;
			node_list.push_back(new_node);
		}

		if (!(*vox_iter)->node_6->flag_visited)
		{
			DuplicatedNode new_node((*vox_iter)->node_6);
			(*vox_iter)->node_6->flag_visited = true;
			node_list.push_back(new_node);
		}

		if (!(*vox_iter)->node_7->flag_visited)
		{
			DuplicatedNode new_node((*vox_iter)->node_7);
			(*vox_iter)->node_7->flag_visited = true;
			node_list.push_back(new_node);
		}
	}

	num_node = node_list.size();

	for (int i=0; i<num_node; ++i)
	{
		node_list[i].master_cluster = this;
	}
}

void Cluster::linkToMappedNode()
{
	if (node_list.empty())
	{
		return;
	}

	vector<DuplicatedNode>::iterator n_iter = node_list.begin();
	for (; n_iter!=node_list.end(); ++n_iter)
	{
		n_iter->mapped_node->linked_duplictae = &(*n_iter);
	}
}

void Cluster::clearVoxVisit()
{
	for (int i=0; i<vox_list.size(); ++i)
	{
		vox_list[i]->flag_visited = false;
	}
}

void Cluster::clearNodeVisit()
{
	for (int i=0; i<node_list.size(); ++i)
	{
		node_list[i].flag_visited = false;
	}
}

void Cluster::clearMappedNodeVisit()
{
	for (int i=0; i<node_list.size(); ++i)
	{
		node_list[i].mapped_node->flag_visited = false;
	}
}

void Cluster::markVoxVisit()
{
	for (int i=0; i<vox_list.size(); ++i)
	{
		vox_list[i]->flag_visited = true;
	}
}

void Cluster::markMappedNodeVisit()
{
	for (int i=0; i<node_list.size(); ++i)
	{
		node_list[i].mapped_node->flag_visited = true;
	}
}

void Cluster::markNodeVisit()
{
	for (int i=0; i<node_list.size(); ++i)
	{
		node_list[i].flag_visited = true;
	}
}

void Cluster::inheritParentMotion()
{
	for (int i=0; i<node_list.size(); ++i)
	{
		if (node_list[i].mapped_node->flag_trigger_node)
		{
			node_list[i].displacement = node_list[i].mapped_node->displacement;
			node_list[i].velocity = node_list[i].mapped_node->velocity;
		}
	}
}

void Cluster::uploadToParentMotion()
{
	for (int i=0; i<node_list.size(); ++i)
	{
		node_list[i].parent_dup->displacement = node_list[i].displacement;
		node_list[i].parent_dup->velocity = node_list[i].velocity;
	}
}

void Cluster::computeRestMassCentroid()
{
	original_center.setZero();
	vector<DuplicatedNode>::const_iterator ni;
	double mass_sum = 0;
	for(ni=node_list.begin(); ni!=node_list.end(); ++ni)
	{
		original_center += ni->mass * ni->coordinate;
		mass_sum += ni->mass;
	}
	original_center /= mass_sum;
}

void Cluster::computeCurrentMassCentroid()
{
	current_center.setZero();
	vector<DuplicatedNode>::const_iterator const_ni;
	double mass_sum = 0;
	for(const_ni=node_list.begin(); const_ni!=node_list.end(); ++const_ni)
	{
		current_center += const_ni->mass * (const_ni->coordinate + const_ni->displacement);
		mass_sum += const_ni->mass;
	}

	current_center /= mass_sum;
}

void Cluster::computeCurrentMassCentroid4Target()
{
	current_center.setZero();
	vector<DuplicatedNode>::const_iterator const_ni;
	double mass_sum = 0;
	for(const_ni=node_list.begin(); const_ni!=node_list.end(); ++const_ni)
	{
		current_center += const_ni->mass * const_ni->target_position;
		mass_sum += const_ni->mass;
	}

	current_center /= mass_sum;
}

void Cluster::computeCurrentMassCentroid4Static()
{
	current_center.setZero();
	vector<DuplicatedNode>::const_iterator const_ni;
	double mass_sum = 0;
	for(const_ni=node_list.begin(); const_ni!=node_list.end(); ++const_ni)
	{
		current_center += const_ni->mass * const_ni->static_position;
		mass_sum += const_ni->mass;
	}

	current_center /= mass_sum;
}
void Cluster::computeAQQ()
{
	Vector3d q;
	Matrix3d a_qq_ = Matrix3d::Zero();

	for(vector<DuplicatedNode>::const_iterator ni = node_list.begin(); ni != node_list.end(); ++ni)
	{
		q  = ni->coordinate - original_center;
		a_qq_ += ni->mass*q*q.transpose();
	}
	a_qq = a_qq_.inverse();
}

void Cluster::goRest()
{
	vector<DuplicatedNode>::iterator ni;
	for(ni=node_list.begin(); ni!=node_list.end(); ++ni)
	{
		ni->displacement.setZero();
		ni->velocity.setZero();
	}
	current_center = original_center;
}

void Cluster::buildVelocityMatrix()
{
	velocity_matrix.resize(3*num_node, 6);
	velocity_matrix.setZero();
	for (int i=0; i<num_node; ++i)
	{
		Matrix3d m33;
		m33.setZero();
		Vector3d a = node_list[i].static_position;
		m33(0, 1) = -a(2); m33(0, 2) = a(1);
		m33(1, 0) = a(2);  m33(1, 2) = -a(0);
		m33(2, 0) = -a(1); m33(2, 1) = a(0);
				
		velocity_matrix.block<3, 3>(i*3, 0) = m33.transpose();
		velocity_matrix.block<3, 3>(i*3, 3) = Matrix3d::Identity();
	}
}

Mesh::Mesh()
{
	number_node = 0;
	number_face = 0;
	mesh_simplified = NULL;
	flag_normalized = false;
}

void Mesh::updateArea()
{
	for (int i=0; i<number_face; ++i)
	{
		face_list[i].updateArea();
	}
}

void Mesh::clearNodeVisit()
{
	for (int i=0; i<node_list.size(); i++)
	{
		node_list[i].flag_visited = false;
	}
}

void Mesh::markNodeVisit()
{
	for (int i=0; i<node_list.size(); i++)
	{
		node_list[i].flag_visited = true;
	}
}

void Mesh::clear()
{
	node_list.clear();
	face_list.clear();
}

void Mesh::scale()
{
	cout << "scaling the mesh to a unit cube..."  << endl;
	double  sx = max_x - min_x;
	double  sy = max_y - min_y;
	double  sz = max_z - min_z;

	double mesh_max_dimension = 0.0;
	mesh_max_dimension = sx > sy? sx : sy;
	mesh_max_dimension = mesh_max_dimension > sz? mesh_max_dimension : sz;


	vector<Node>::iterator node_iter = node_list.begin();
	for (; node_iter!=node_list.end(); ++node_iter)
	{
		node_iter->coordinate = (node_iter->coordinate - mesh_center) / mesh_max_dimension;
	}

	max_x = (max_x - mesh_center(0)) / mesh_max_dimension;
	max_y = (max_y - mesh_center(1)) / mesh_max_dimension;
	max_z = (max_z - mesh_center(2)) / mesh_max_dimension;

	min_x = (min_x - mesh_center(0)) / mesh_max_dimension;
	min_y = (min_y - mesh_center(1)) / mesh_max_dimension;
	min_z = (min_z - mesh_center(2)) / mesh_max_dimension;

	cout << "shift :" << mesh_center(0) << " " << mesh_center(1) << " " << mesh_center(2) << "  scaling: "  << mesh_max_dimension << endl;

	mesh_center.setZero();
	flag_normalized = true;
	scalar = mesh_max_dimension;
}

void Mesh::scale(double scalar)
{
	vector<Node>::iterator node_iter = node_list.begin();
	for (; node_iter!=node_list.end(); ++node_iter)
	{
		node_iter->coordinate = (node_iter->coordinate - mesh_center) / scalar;
	}

	max_x = (max_x - mesh_center(0)) / scalar;
	max_y = (max_y - mesh_center(1)) / scalar;
	max_z = (max_z - mesh_center(2)) / scalar;

	min_x = (min_x - mesh_center(0)) / scalar;
	min_y = (min_y - mesh_center(1)) / scalar;
	min_z = (min_z - mesh_center(2)) / scalar;

	mesh_center.setZero();
	flag_normalized = true;
}

void Mesh::read(const char* filename)
{
	clear();

	cout << "loading obj mesh...";

	ifstream ifs;
	ifs.open(filename);
	if(ifs.fail())
		return;

	node_list.reserve(10000);
	face_list.reserve(30000);

	max_x = max_y = max_z = -1e10;
	min_x = min_y = min_z = 1e10;

	char line[1024];

	char* token;
	////test
	//int *idx = new int[100000];
	//memset(idx, 0, sizeof(int)*100000);
	////
	while (!ifs.eof())
	{
		ifs.getline(line, 1024);
		token = strtok(line, " ");
		char vertex[] = "v";
		char face[] = "f";
		char commt[] = "#";

		if (strlen(line) == 0)
		{
			continue;
		}

		else if (strcmp(token, commt) == 0)
		{
			continue;
		}

		// a line for node index
		else if (strcmp(token, vertex) == 0 )
		{
			Node new_node;

			token = strtok(NULL, " ");
			new_node.coordinate(0) = atof(token);

			token = strtok(NULL, " ");
			new_node.coordinate(1) = atof(token);

			token = strtok(NULL, " ");
			new_node.coordinate(2) = atof(token);

			max_x = new_node.coordinate(0) > max_x ? new_node.coordinate(0) : max_x;
			max_y = new_node.coordinate(1) > max_y ? new_node.coordinate(1) : max_y;
			max_z = new_node.coordinate(2) > max_z ? new_node.coordinate(2) : max_z;

			min_x = new_node.coordinate(0) < min_x ? new_node.coordinate(0) : min_x;
			min_y = new_node.coordinate(1) < min_y ? new_node.coordinate(1) : min_y;
			min_z = new_node.coordinate(2) < min_z ? new_node.coordinate(2) : min_z;

			node_list.push_back(new_node);
			new_node.index = node_list.size() - 1;
		}

		// a line for face
		else if (strcmp (token, face) == 0)
		{
			Face new_face;

			token = strtok(NULL, " ");
			int node_idx_0 = atoi(token) - 1;
			new_face.node0 = &node_list[node_idx_0];
			//
			//idx[node_idx_0] = 1;

			token = strtok(NULL, " ");
			int node_idx_1 = atoi(token) - 1;
			new_face.node1 = &node_list[node_idx_1];
			//
			//idx[node_idx_1] = 1;

			token = strtok(NULL, " ");
			int node_idx_2 = atoi(token) - 1;
			new_face.node2 = &node_list[node_idx_2];
			//
			//idx[node_idx_2] = 1;
			
			//add idx for networking
			new_face.idx_node0 = node_idx_0;
			new_face.idx_node1 = node_idx_1;
			new_face.idx_node2 = node_idx_2;

			face_list.push_back(new_face);
		}
	}

	ifs.close();

	number_node = node_list.size();
	number_face = face_list.size();


	////////////////////////test whether face_list uses all the nodes//////////////
	//int count = 0;
	//for(int i = 0; i < node_list.size(); i++)
	//{
	//	if(idx[i] == 0)
	//		count ++;
	//}
	//cout << "number of nodes which is not used:" << count << endl;
	////
	double sx = max_x - min_x;
	double sy = max_y - min_y;
	double sz = max_z - min_z;

	dimension = 0.0;
	dimension = sx > sy? sx : sy;
	dimension = dimension > sz? dimension : sz;

	mesh_center(0) = 0.5* (max_x + min_x);
	mesh_center(1) = 0.5* (max_y + min_y);
	mesh_center(2) = 0.5* (max_z + min_z);

	cout << number_node <<" nodes and " << number_face << " faces" << "read" << endl;
	if (mesh_center.norm() < 1e-6 && abs(dimension - 1.0)<1e-6)
	{
		flag_normalized = true;

		cout << "mesh is already normalized" << endl;
	}
}

VoxMesh::VoxMesh()
{
	num_node = 0;
	num_surface_vox = 0;
	num_cluster = 0;
	num_vox = 0;
	vox_size = 0;

	active_node = NULL;
	constraint_node = NULL;
	inverted_cluster = NULL;
	neighbor_flag = NULL;

	old_energy = 0.0;
	new_energy = 0.0;
}

void VoxMesh::clear()
{
	node_list.clear();
	vox_locator.clear();
	vox_list.clear();
	surface_vox_list.clear();
	surface_node_list.clear();
	anchor_node_list.clear();
	cluster_list.clear();
	root_to_leaf_cluster_list.clear();
	leaf_to_root_cluster_list.clear();

	num_node = 0;
	num_vox = 0;
	num_surface_vox = 0;
	num_cluster = 0;

	current_parent_cluster = NULL;
	current_sibling_index = -1;
	current_cluster_level = -1;
}

void VoxMesh::clearVoxSelect()
{
	for (int i=0; i<vox_list.size(); ++i)
	{
		vox_list[i].flag_selected = false;
	}
}

void VoxMesh::clearNodeVisit()
{
	for (int i=0; i<node_list.size(); ++i)
	{
		node_list[i].flag_visited = false;
	}
}

void VoxMesh::clearNodeTrigger()
{
	for (int i=0; i<node_list.size(); ++i)
	{
		node_list[i].flag_trigger_node = false;
	}
}

void VoxMesh::markNodeVisit()
{
	for (int i=0; i<node_list.size(); ++i)
	{
		node_list[i].flag_visited = true;
	}
}

void VoxMesh::initializeClusterHierarchy()
{
	cluster_hierarchy.clear();

	Cluster root_cluster;
	root_cluster.node_list.reserve(num_node);
	root_cluster.vox_list.reserve(num_vox);

	vector<Node>::iterator node_iter = node_list.begin();
	for (; node_iter != node_list.end(); ++node_iter)
	{
		DuplicatedNode new_dup_node(&(*node_iter));
		root_cluster.node_list.push_back(new_dup_node);
	}

	vector<Vox>::iterator vox_iter = vox_list.begin();
	for (; vox_iter != vox_list.end(); ++vox_iter)
	{
		root_cluster.vox_list.push_back(&(*vox_iter));
	}

	root_cluster.num_node = root_cluster.node_list.size();
	root_cluster.num_vox = root_cluster.vox_list.size();

	current_parent_cluster = cluster_hierarchy.insert(cluster_hierarchy.begin(), root_cluster);
	current_sibling_index = 0;
	current_cluster_level = 0;

	current_parent_cluster->linkToMappedNode();
}

void VoxMesh::appendMaxConnectedComponent(vector<Vox*> &v_list)
{
	for (vector<Vox*>::iterator v_iter = v_list.begin(); v_iter != v_list.end(); ++v_iter)
	{
		(*v_iter)->flag_selected = true;
	}

	stack<Vox*> vox_stack;

	int num_visited_vox = 0;
	int num_input_vox = v_list.size();

	// find all the connected components
	vector <vector <Vox*>> connected_components;

	while (num_visited_vox != num_input_vox)
	{
		vector<Vox*> a_connected_component;
		a_connected_component.reserve(num_input_vox);

		// find a un-visited vox first
		for (int i = 0; i<num_input_vox; ++i)
		{
			if (!v_list[i]->flag_visited)
			{
				v_list[i]->flag_visited = true;
				vox_stack.push(v_list[i]);
				break;
			}
		}

		while(!vox_stack.empty())
		{
			Vox* top_vox = vox_stack.top();
			vox_stack.pop();
			a_connected_component.push_back(top_vox);
			//++num_visited_vox;

			if (top_vox->top_neighbor)
			{
				if (!top_vox->top_neighbor->flag_visited && top_vox->top_neighbor->flag_selected)
				{
					vox_stack.push(top_vox->top_neighbor);
					top_vox->top_neighbor->flag_visited = true;
				}
			}

			if (top_vox->bottom_neighbor)
			{
				if (!top_vox->bottom_neighbor->flag_visited && top_vox->bottom_neighbor->flag_selected)
				{
					vox_stack.push(top_vox->bottom_neighbor);
					top_vox->bottom_neighbor->flag_visited = true;
				}
			}

			if (top_vox->front_neighbor)
			{
				if (!top_vox->front_neighbor->flag_visited && top_vox->front_neighbor->flag_selected)
				{
					vox_stack.push(top_vox->front_neighbor);
					top_vox->front_neighbor->flag_visited = true;
				}
			}

			if (top_vox->back_neighbor)
			{
				if (!top_vox->back_neighbor->flag_visited && top_vox->back_neighbor->flag_selected)
				{
					vox_stack.push(top_vox->back_neighbor);
					top_vox->back_neighbor->flag_visited = true;
				}
			}

			if (top_vox->left_neighbor)
			{
				if (!top_vox->left_neighbor->flag_visited && top_vox->left_neighbor->flag_selected)
				{
					vox_stack.push(top_vox->left_neighbor);
					top_vox->left_neighbor->flag_visited = true;
				}
			}

			if (top_vox->right_neighbor)
			{
				if (!top_vox->right_neighbor->flag_visited  && top_vox->right_neighbor->flag_selected)
				{
					vox_stack.push(top_vox->right_neighbor);
					top_vox->right_neighbor->flag_visited = true;
				}
			}
		}

		if (!a_connected_component.empty())
		{
			connected_components.push_back(a_connected_component);
			num_visited_vox += a_connected_component.size();
		}
	}

	// find the max connected component
	int max_connected_component_size = 0;
	vector<Vox*>* max_connected_component = NULL;
	for (int i=0; i<connected_components.size(); ++i)
	{
		if (connected_components[i].size() > max_connected_component_size)
		{
			max_connected_component = &connected_components[i];
			max_connected_component_size = max_connected_component->size();
		}
	}

	// generate out put
	Cluster new_cluster;
	new_cluster.clear();
	new_cluster.vox_list = *max_connected_component;

	// generate duplicated note
	new_cluster.num_vox = max_connected_component_size;
	new_cluster.initializeNodeList();

	// new cluster is a child of current parent cluster, link the parent dup
	for (int i=0; i<new_cluster.num_node; ++i)
	{
		new_cluster.node_list[i].parent_dup = new_cluster.node_list[i].mapped_node->linked_duplictae;
	}

	// house keeping: clear selected
	for (vector<Vox*>::iterator v_iter = v_list.begin(); v_iter != v_list.end(); ++v_iter)
	{
		(*v_iter)->flag_selected = false;
		(*v_iter)->flag_visited = false;
	}

	// all the selected voxel are visited -> so they are not selected by its siblings
	for (vector<Vox*>::iterator v_iter = new_cluster.vox_list.begin(); v_iter != new_cluster.vox_list.end(); ++v_iter)
	{
		(*v_iter)->flag_visited = true;
	}

	// find trigger node list
	clearNodeVisit();
	if (cluster_hierarchy.number_of_children(current_parent_cluster) > 0)
	{
		tree<Cluster>::iterator p_loc = cluster_hierarchy.child(current_parent_cluster, 0);
		p_loc->markMappedNodeVisit();

		while(cluster_hierarchy.is_valid(cluster_hierarchy.next_sibling(p_loc)))
		{
			p_loc = cluster_hierarchy.next_sibling(p_loc);
			p_loc->markMappedNodeVisit();
		}
	}
	
	for (int i=0; i<new_cluster.num_node; ++i)
	{
		if (new_cluster.node_list[i].mapped_node->flag_visited)
		{
			new_cluster.node_list[i].mapped_node->flag_trigger_node = true;
		}
	}
	
	if (cluster_hierarchy.is_valid(current_parent_cluster))
	{
		cluster_hierarchy.append_child(current_parent_cluster, new_cluster);
	}
	clearNodeVisit();
}

void VoxMesh::updateVoxVisit()
{
	current_parent_cluster->clearVoxVisit();
	tree<Cluster>::sibling_iterator sib_iter = cluster_hierarchy.child(current_parent_cluster, 0);
	for (; cluster_hierarchy.is_valid(sib_iter); ++sib_iter)
	{
		sib_iter->markVoxVisit();
	}
}

void VoxMesh::createBFSOrder()
{
	root_to_leaf_cluster_list.clear();

	tree<Cluster>::breadth_first_iterator bfs_iter = cluster_hierarchy.begin_breadth_first();
	for (; bfs_iter!=cluster_hierarchy.end_breadth_first(); ++bfs_iter)
	{
		root_to_leaf_cluster_list.push_back(&(*bfs_iter));
		bfs_iter->location_in_hierarchy = bfs_iter;
	}

	leaf_to_root_cluster_list.clear();
	int n_cluster = root_to_leaf_cluster_list.size();
	for (int i=0; i<n_cluster-1; ++i)
	{
		leaf_to_root_cluster_list.push_back(root_to_leaf_cluster_list[n_cluster - i - 1]);
	}
}

void VoxMesh::initializeNeighborFlag()
{
	// check neighboring relation 
	int num_row = 0;
	if (neighbor_flag != NULL)
	{
		delete [] neighbor_flag ;
	}

	neighbor_flag = new bool[num_cluster * num_cluster];
	memset(neighbor_flag, false, sizeof(bool) * num_cluster * num_cluster);

	clearNodeVisit();

	for (int i=0; i<num_cluster-1; ++i)
	{
		int ni = cluster_list[i].num_node;

		// mark node mapped with cluster i 
		for (int k=0; k<ni; ++k)
		{
			cluster_list[i].node_list[k].mapped_node->flag_visited = true;
		}

		for (int j=i+1; j<num_cluster; ++j)
		{
			int nj = cluster_list[j].num_node;
			
			// check for cluster j and if any node is already marked as visited, i and j share nodes and thus neighbor
			for (int k=0; k<nj; ++k)
			{
				if (cluster_list[j].node_list[k].mapped_node->flag_visited)
				{
					neighbor_flag[i*num_cluster * j] = true;
					num_row += 3*(ni+nj);
					break;
				}
			}
		}

		for (int k=0; k<ni; ++k)
		{
			cluster_list[i].node_list[k].mapped_node->flag_visited = false;
		}
	}

	system_matrix.resize(num_row, 6*num_cluster);
}

void VoxMesh::buildSystemMatrix()
{
	system_matrix.setZero();

	int row_index = 0;
	for (int i=0; i<num_cluster-1; ++i)
	{
		int ni = cluster_list[i].num_node;

		for (int j=i+1; j<num_cluster-1; ++j)
		{
			if (neighbor_flag[i*num_cluster + j])
			{
				int nj = cluster_list[j].num_node;

				system_matrix.block(row_index, 0, 3*ni, 6) = cluster_list[i].velocity_matrix;
				system_matrix.block(row_index + 3*ni, 0, 3*nj, 6) = cluster_list[j].velocity_matrix;
				system_matrix.block(row_index, j*6, 3*ni, 6) = -cluster_list[i].velocity_matrix;
				system_matrix.block(row_index + 3*ni, j*6, 3*nj, 6) = -cluster_list[j].velocity_matrix;

				row_index += 3*(ni + nj);
			}
		}
	}
}

Level::Level()
{
	gridDensity_total = 1;
	gridDensity = 1;
	level_index = 0;
	times_ShapeMatching = 1;
	voxmesh_level = NULL;

	num_sim_vox = 0;
	num_surface_vox = 0;
	num_sim_node = 0;
}

Level::~Level()
{
	clear();
}

void Level::clear()
{
	VoxMesh * temp = voxmesh_level;
	voxmesh_level = NULL;
	delete temp;

	gridDensity = 1;
	level_index = 0;
}