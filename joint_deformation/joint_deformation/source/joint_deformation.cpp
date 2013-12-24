#include "joint_deformation.h"
//test
#include <iostream>

joint_deformation::joint_deformation(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	p_kernel = new Kernel;
	ui.renderWidget->p_kernel = p_kernel;
	ui.renderWidget->initArrow(p_kernel);
	
	flag_captureScreen = false;
	flag_captureSubScreen = false;
	flag_exportObj = false;

	//disable parts of UI
	ui.comboBox_level->setDisabled(true);
	ui.pushButton_addLevel->setDisabled(true);

	ui.radioButton_Server->setDisabled(true);
	ui.radioButton_Client->setDisabled(true);
	ui.pushButton_setNumOfClients->setDisabled(true);
	ui.pushButton_configNetwork->setDisabled(true);

}

joint_deformation::~joint_deformation()
{

}

void joint_deformation::loadMesh()
{
	QString path = QFileDialog::getOpenFileName(this, tr("load mesh"), ".", tr("obj file(*.obj)"));
	if (path.length())
	{
		p_kernel->p_mesh->read(path.toLatin1());
		
		p_kernel->flag_mesh_ready = true;
		ui.renderWidget->flag_show_mesh = true;
		ui.actionShowMesh->setChecked(true);

		if (p_kernel->p_mesh->flag_normalized)
		{
			p_kernel->mark_preprocess4Voxel(p_kernel->p_mesh, p_kernel->p_voxel, p_kernel->grid_density);
			ui.actionScaleMesh->setChecked(true);
		}
		else
		{
			scaleMesh();
		}

		setGridDensity();
		ui.actionShowVox->setChecked(true);
		ui.renderWidget->flag_show_vox = true;
		
		/*
		//for a low level mesh
		path.indexOf(".");
		QString path_low = path.leftRef(path.indexOf(".")).toString();
		path_low.append("_low.obj");
		p_kernel->p_mesh_low->read(path.toLatin1());
		if(!p_kernel->p_mesh->flag_normalized)
			p_kernel->p_mesh_low->scale(p_kernel->p_mesh->scalar);
		p_kernel->p_mesh->mesh_simplified = p_kernel->p_mesh_low;
		*/
		char msg[128];
		sprintf(msg, "read %d nodes and %d faces", p_kernel->p_mesh->number_node, p_kernel->p_mesh->number_face);
		QMessageBox::information(NULL, "done", msg);

		ui.comboBox_selectTreeDepth->setEnabled(true);
		ui.renderWidget->updateGL();
	}
}

void joint_deformation::scaleMesh()
{
	if (!p_kernel->flag_mesh_ready)
	{
		QMessageBox::warning(NULL, "warning", "load a mesh first");
		return;
	}

	if (!p_kernel->p_mesh->flag_normalized)
	{
		p_kernel->p_mesh->scale();
		p_kernel->mark_preprocess4Voxel(p_kernel->p_mesh, p_kernel->p_voxel, p_kernel->grid_density);
	}

	ui.actionScaleMesh->setChecked(true);
	ui.renderWidget->updateGL();
}

void joint_deformation::setGridDensity()
{
	if (!p_kernel->flag_mesh_ready)
	{
		QMessageBox::warning(NULL, "warning", "load a mesh first");
		return;
	}

	else if (!p_kernel->p_mesh->flag_normalized)
	{
		QMessageBox::warning(NULL, "warning", "scale the mesh first");
		return;
	}

	int d = ui.spinBox_gridDensity->value();
	int level = 0;
	if(ui.comboBox_level->isEnabled())
		level = ui.comboBox_level->currentIndex();
	
	if(level > 0)
	{
		int parent_index = level - 1;
		if(p_kernel->level_list[parent_index]->voxmesh_level == NULL)
		{
			QString message = "Error! The grid density of Level " + QString::number(parent_index) + " has not been set yet!" ;
			QMessageBox::warning(NULL, "warning", message);
			return;
		}
	}
	//if this level has a child level, then regenerate volmesh
	p_kernel->generateVoxMesh4Level(level++, d);
	while(level < p_kernel->level_list.size())
	{
		if(p_kernel->level_list[level]->voxmesh_level == NULL)
			break;
		d = p_kernel->level_list[level]->gridDensity;
		p_kernel->generateVoxMesh4Level(level, d);
		level ++;
	}
	
	ui.renderWidget->updateGL();
}

void joint_deformation::showGrid()
{
	ui.renderWidget->flag_show_grid = ui.actionShowGrid->isChecked();
	ui.actionShowGrid->setChecked(ui.renderWidget->flag_show_grid);
	ui.renderWidget->updateGL();
}

void joint_deformation::showMesh(bool a)
{
	ui.renderWidget->flag_show_mesh = a;
	ui.renderWidget->updateGL();
}

void joint_deformation::showGoalPosition(bool a)
{
	ui.renderWidget->flag_show_target = a;
	ui.renderWidget->updateGL();
}

void joint_deformation::showVox(bool a)
{
	ui.renderWidget->flag_show_vox = a;
	ui.renderWidget->updateGL();
}

void joint_deformation::showSelection(bool a)
{
	if (!p_kernel->flag_vox_ready)
	{
		QMessageBox::warning(NULL, "warning", "generate voxel mesh first");
		ui.actionShowSelection->setChecked(false);
		return;
	}

	ui.renderWidget->flag_show_selection = a;
	if(a && ui.renderWidget->flag_show_constraints)
	{
		ui.renderWidget->flag_show_constraints = false;
		ui.actionSetConstraint->setChecked(false);
	}
	if(!a)
	{
		ui.renderWidget->bottom_right_x = -1.0;
		ui.renderWidget->bottom_right_y = -1.0;
		ui.renderWidget->upper_left_x   = -1.0;
		ui.renderWidget->upper_left_y = -1.0;
	}
	ui.renderWidget->updateGL();
}

void joint_deformation::setRenderMode(bool a)
{
	if (!p_kernel->flag_vox_ready)
	{
		QMessageBox::warning(NULL, "warning", "generate voxel mesh first");
		ui.actionShowSelection->setChecked(false);
		return;
	}

	if (a)
	{
		ui.renderWidget->current_render_mode = Renderer::TREE_SETTING;
		ui.renderWidget->flag_show_selection = true;
	}
	else
	{
		ui.renderWidget->current_render_mode = Renderer::REGULAR;
		ui.renderWidget->flag_show_selection = false;
	}
		
	ui.renderWidget->updateGL();
}

void joint_deformation::startSimulation()
{
	if (!p_kernel->flag_simulator_ready)
	{
		QMessageBox::warning(NULL, "warning", "initialize simulator first");
		return;
	}

	if (!simulation_timer.isActive())
	{
		connect(&simulation_timer, SIGNAL(timeout()), this, SLOT(simulate()));
		ui.actionShowSelection->setChecked(false);
		ui.renderWidget->flag_show_selection = false;
		ui.renderWidget->flag_simulating = true;
		simulation_timer.start(0);

	}
}

void joint_deformation::pauseSimulation()
{
	if (!simulation_timer.isActive())
	{
		simulation_timer.start(0);
	}
	else
	{
		simulation_timer.stop();
	}
}

void joint_deformation::simulate()
{
	if (!p_kernel->simulateNextStep())
	{
		pauseSimulation();
	}

	//interpolation surface points
	Vector3d a, b, c; 
	if (p_kernel->paraNode[0] != NULL && p_kernel->paraNode[1] != NULL && p_kernel->paraNode[2] != NULL )
	{
		a = p_kernel->paraNode[0]->coordinate + p_kernel->paraNode[0]->displacement;
		b =p_kernel->paraNode[1]->coordinate + p_kernel->paraNode[1]->displacement;
		c =p_kernel->paraNode[2]->coordinate + p_kernel->paraNode[2]->displacement;
		p_kernel->surface_point_left = p_kernel->para[0] * a + p_kernel->para[1] * b + p_kernel->para[2] * c;
	}
	if (p_kernel->paraNode2[0] != NULL && p_kernel->paraNode2[1] != NULL && p_kernel->paraNode2[2] != NULL)
	{
		a = p_kernel->paraNode2[0]->coordinate + p_kernel->paraNode2[0]->displacement;
		b =p_kernel->paraNode2[1]->coordinate + p_kernel->paraNode2[1]->displacement;
		c =p_kernel->paraNode2[2]->coordinate + p_kernel->paraNode2[2]->displacement;
		p_kernel->surface_point_right = p_kernel->para2[0] * a + p_kernel->para2[1] * b + p_kernel->para2[2] * c;
	}
	////////////////////

	if(p_kernel->pReceiver != NULL)
	{
		if(p_kernel->pReceiver->isMeshReady)
			p_kernel->flag_mesh_ready = true;
		else
			p_kernel->flag_mesh_ready = false;
	}
	ui.renderWidget->updateGL();

	if (flag_captureScreen)
	{
		//QPixmap Screenshot = ui.RenderWidget->renderPixmap();
		QPixmap Screenshot = QPixmap::grabWindow(QApplication::desktop()->winId(), 
			this->pos().x(), this->pos().y(), width()+15, height()+35);
		char filenames[32];
		sprintf(filenames, "..\\ScreenShots\\SIMG%0004d.bmp", p_kernel->time_step_index);
		Screenshot.save(filenames, "bmp", -1);
	}

	if (flag_captureSubScreen)
	{
		//ui.renderWidget->fini();
		QImage Screenshot = ui.renderWidget->grabFrameBuffer(false);
		char filenames[32];
		sprintf(filenames, "..\\ScreenShots\\IMG%0004d.png", p_kernel->time_step_index);
		Screenshot.save(filenames, "png", -1);
	}

	if (flag_exportObj || p_kernel->flag_exportObj)
	{
		char filenames[32];
		sprintf(filenames, "..\\OBJs\\OBJ%0004d.obj", p_kernel->num_obj);
		//p_kernel->exportToOBJ(filenames);
		p_kernel->exportToOBJ2Haptic(filenames);
		p_kernel->num_obj ++;
	}
}

void joint_deformation::setBeating(bool a)
{
	p_kernel->flag_beating = a;

	ui.renderWidget->updateGL();
}

void joint_deformation::simulateNextStep()
{
	if (!p_kernel->flag_simulator_ready)
	{
		QMessageBox::warning(NULL, "warning", "initialize simulator first");
		return;
	}
	if (!p_kernel->simulateNextStep()) 
	{
		pauseSimulation();
	}



	//if (flag_exportObj)
	//{
	//	char filenames[32];
	//	sprintf(filenames, "..\\OBJs\\OBJ%0004d.obj", p_kernel->time_step_index);
	//	p_kernel->exportToOBJ(filenames);
	//}
	ui.renderWidget->updateGL();
}

void joint_deformation::loadLevel()
{
	if (!p_kernel->flag_mesh_ready)
	{
		QMessageBox::warning(NULL, "warning", "load a mesh first");
		return;
	}

	else if (!p_kernel->p_mesh->flag_normalized)
	{
		QMessageBox::warning(NULL, "warning", "scale the mesh first");
		return;
	}
	p_kernel->clearAllLevel();
	ifstream ifs("level.txt");
	char line[1024];
	char * token;
	while (!ifs.eof())
	{
		ifs.getline(line, 1024);
		if (strlen(line) == 0)
			break;
		token = strtok(line, " ");
		int level = atoi(token);
		token = strtok(NULL, " ");
		int d = atoi(token);
		cout << level << " " << d << endl;
		if(level > 0)
		{
			p_kernel->addLevel();
			int index = ui.comboBox_level->count();
			QString	item = "Level " + QString::number(index);
			ui.comboBox_level->addItem(item);
			ui.comboBox_level->setCurrentIndex(index);
		}
		p_kernel->generateVoxMesh4Level(level++, d);
	}
	ui.renderWidget->updateGL();
}

void joint_deformation::saveLevel()
{
	ofstream ofs("level.txt");
	for(int i = 0; i < p_kernel->level_list.size(); i ++)
	{
		ofs << i << " " << p_kernel->level_list[i]->gridDensity << endl;
	}
	ofs.close();
}

void joint_deformation::initializeSimulator()
{
	if (p_kernel->used_simulator == Kernel::UNDEFINED)
	{
		QMessageBox::warning(NULL, "warning", "choose simulator first");
		return;
	}
	p_kernel->initializeSimulator();

	QMessageBox::information(NULL, "success", "simulator initialized");
}

void joint_deformation::updateTreeDepth()
{
	int currnet_index = ui.comboBox_selectTreeDepth->currentIndex();
	int current_cluster_hierarchy_depth = ui.comboBox_selectTreeDepth->count();
	int new_cluster_hierarchy_depth =  p_kernel->p_vox_mesh->cluster_hierarchy.max_depth() + 1;
	if (current_cluster_hierarchy_depth < new_cluster_hierarchy_depth)
	{
		char item[32];
		sprintf(item, "Lv %d", new_cluster_hierarchy_depth - 1);
		ui.comboBox_selectTreeDepth->addItem(item);
		
	}
	ui.comboBox_selectTreeDepth->setCurrentIndex(currnet_index);
}

void joint_deformation::nextSameLevel()
{
	tree<Cluster>::fixed_depth_iterator loc = p_kernel->p_vox_mesh->current_parent_cluster;
	tree<Cluster>::fixed_depth_iterator new_loc = loc;
	tree<Cluster>* p_tree = &(p_kernel->p_vox_mesh->cluster_hierarchy);
		
	if (loc == p_tree->begin()) // if current parent cluster is the root, click next sibling remains itself
	{
		return;
	}
	
	if (!p_tree->is_valid(++new_loc))
	{
		new_loc = p_tree->begin_fixed(p_tree->begin(), p_tree->depth(loc));
		new_loc->linkToMappedNode();
	}
	
	p_kernel->p_vox_mesh->current_parent_cluster = new_loc;
	p_kernel->p_vox_mesh->updateVoxVisit();
	ui.renderWidget->updateGL();
}


void joint_deformation::lastSameLevel()
{
	tree<Cluster>::fixed_depth_iterator loc = p_kernel->p_vox_mesh->current_parent_cluster;
	tree<Cluster>::fixed_depth_iterator new_loc = loc;
	tree<Cluster>* p_tree = &(p_kernel->p_vox_mesh->cluster_hierarchy);

	if (loc == p_tree->begin()) // if current parent cluster is the root, click next sibling remains itself
	{
		return;
	}

	if (!p_tree->is_valid(--new_loc))
	{
		// the end_fix is not working....
		tree<Cluster>::fixed_depth_iterator pre_iter = loc;
		while(p_tree->is_valid(pre_iter))
		{
			new_loc = pre_iter;
			++pre_iter; 
		}
		new_loc->linkToMappedNode();
	}

	p_kernel->p_vox_mesh->current_parent_cluster = new_loc;
	p_kernel->p_vox_mesh->updateVoxVisit();
	ui.renderWidget->updateGL();
}

void joint_deformation::updateCurrentParentCluster(int new_lv)
{
	tree<Cluster>* p_tree = &(p_kernel->p_vox_mesh->cluster_hierarchy);
	int current_lv = p_kernel->p_vox_mesh->current_cluster_level;

	if (new_lv != current_lv)
	{
		tree<Cluster>::iterator loc = p_tree->begin_fixed(p_tree->begin(), new_lv);
		p_kernel->p_vox_mesh->current_cluster_level = new_lv;
		loc->linkToMappedNode();
		p_kernel->p_vox_mesh->current_parent_cluster = loc;
		p_kernel->p_vox_mesh->updateVoxVisit();
	}

	ui.renderWidget->updateGL();
}

void joint_deformation::chooseSimulator(int t)
{
	//disable some parts of UI.
	ui.comboBox_level->setDisabled(true);
	ui.pushButton_addLevel->setDisabled(true);
	switch (t)
	{
	case 0:
		p_kernel->used_simulator = Kernel::UNDEFINED;
		break;
	case 1:
		//p_kernel->used_simulator = Kernel::HIERARCHY;
		p_kernel->used_simulator = Kernel::HSM_FORCE4ITERATION;
		// keep Level 0, dicard other Levels
		p_kernel->clearAllLevel();
		//enable some parts of UI.
		ui.comboBox_level->clear();
		ui.comboBox_level->addItem("Level 0");
		ui.comboBox_level->setDisabled(false);
		ui.pushButton_addLevel->setDisabled(false);
		break;
	case 2:
		//p_kernel->used_simulator = Kernel::HIERARCHY;
		p_kernel->used_simulator = Kernel::HSM_ORIGINAL;
		// keep Level 0, dicard other Levels
		p_kernel->clearAllLevel();
		//enable some parts of UI.
		ui.comboBox_level->clear();
		ui.comboBox_level->addItem("Level 0");
		ui.comboBox_level->setDisabled(false);
		ui.pushButton_addLevel->setDisabled(false);
		break;
	case 3:
		//p_kernel->used_simulator = Kernel::HIERARCHY;
		p_kernel->used_simulator = Kernel::HSM_ONE_STEP;
		// keep Level 0, dicard other Levels
		p_kernel->clearAllLevel();
		//enable some parts of UI.
		ui.comboBox_level->clear();
		ui.comboBox_level->addItem("Level 0");
		ui.comboBox_level->setDisabled(false);
		ui.pushButton_addLevel->setDisabled(false);
		break;
	case 4:
		//p_kernel->used_simulator = Kernel::HIERARCHY;
		p_kernel->used_simulator = Kernel::HSM_FORCE4STEP;
		// keep Level 0, dicard other Levels
		p_kernel->clearAllLevel();
		//enable some parts of UI.
		ui.comboBox_level->clear();
		ui.comboBox_level->addItem("Level 0");
		ui.comboBox_level->setDisabled(false);
		ui.pushButton_addLevel->setDisabled(false);
		break;
	case 5:
		//p_kernel->used_simulator = Kernel::HIERARCHY;
		p_kernel->used_simulator = Kernel::HSM_ADAPTIVE_STEP;
		// keep Level 0, dicard other Levels
		p_kernel->clearAllLevel();
		//enable some parts of UI.
		ui.comboBox_level->clear();
		ui.comboBox_level->addItem("Level 0");
		ui.comboBox_level->setDisabled(false);
		ui.pushButton_addLevel->setDisabled(false);
		break;
	case 6:
		p_kernel->used_simulator = Kernel::SHAPE_MATCHING;
		break;
	case 7:
		p_kernel->used_simulator = Kernel::VELOCITY_MATCHING;
		break;

	case 8:
		p_kernel->used_simulator = Kernel::SINGLE_GRID;
		break;
	case 9:
		p_kernel->used_simulator = Kernel::PAIR_MATCHING;
		break;
	case 10:
		p_kernel->used_simulator = Kernel::MULTIPLE_VELOCITY_MATCHING;
		// keep Level 0, dicard other Levels
		p_kernel->clearAllLevel();
		//enable some parts of UI.
		ui.comboBox_level->clear();
		ui.comboBox_level->addItem("Level 0");
		ui.comboBox_level->setDisabled(false);
		ui.pushButton_addLevel->setDisabled(false);
		break;
	case 11:
		p_kernel->used_simulator = Kernel::SIMULATION_NETWORKING;
		ui.renderWidget->flag_show_mesh = true;
		ui.actionShowMesh->setChecked(true);
		ui.actionScaleMesh->setChecked(true);
		ui.comboBox_selectTreeDepth->setEnabled(true);
		ui.renderWidget->updateGL();
		break;
	case 12:
		p_kernel->used_simulator = Kernel::SIMULATION_MOBILE;
		ui.renderWidget->flag_show_mesh = true;
		ui.actionShowMesh->setChecked(true);
		ui.actionScaleMesh->setChecked(true);
		ui.comboBox_selectTreeDepth->setEnabled(true);
		ui.renderWidget->updateGL();
		p_kernel->myMobile.open("output_2.txt");
		if(p_kernel->myMobile.fail())
			return;
		break;
	case 13:
		//p_kernel->used_simulator = Kernel::HIERARCHY;
		p_kernel->used_simulator = Kernel::HSM_FORCE4STEP_FIRST;
		// keep Level 0, dicard other Levels
		p_kernel->clearAllLevel();
		//enable some parts of UI.
		ui.comboBox_level->clear();
		ui.comboBox_level->addItem("Level 0");
		ui.comboBox_level->setDisabled(false);
		ui.pushButton_addLevel->setDisabled(false);
		break;
	case 14:
		//p_kernel->used_simulator = Kernel::HIERARCHY;
		p_kernel->used_simulator = Kernel::HSM_FORCE4STEP_FIRST1;
		// keep Level 0, dicard other Levels
		p_kernel->clearAllLevel();
		//enable some parts of UI.
		ui.comboBox_level->clear();
		ui.comboBox_level->addItem("Level 0");
		ui.comboBox_level->setDisabled(false);
		ui.pushButton_addLevel->setDisabled(false);
		break;
	case 15:
		//p_kernel->used_simulator = Kernel::HIERARCHY;
		p_kernel->used_simulator = Kernel::HSM_FORCE4STEP_FIRST2;
		// keep Level 0, dicard other Levels
		p_kernel->clearAllLevel();
		//enable some parts of UI.
		ui.comboBox_level->clear();
		ui.comboBox_level->addItem("Level 0");
		ui.comboBox_level->setDisabled(false);
		ui.pushButton_addLevel->setDisabled(false);
		break;
	default:
		break;
	}
	
}

void joint_deformation::addLevel()
{
	p_kernel->addLevel();
	//ui
	int index = ui.comboBox_level->count();
	QString	item = "Level " + QString::number(index);
	ui.comboBox_level->addItem(item);
	ui.comboBox_level->setCurrentIndex(index);

}

void joint_deformation::clearLevel()
{
	p_kernel->clearAllLevel();
	//ui
	ui.comboBox_level->setCurrentIndex(0);
	while(ui.comboBox_level->count() > 1)
		ui.comboBox_level->removeItem(1);
}

void joint_deformation::chooseLevelDisplay(int value)
{
	if (value < 0)
		value = 0;
	p_kernel->level_display = value;
	
	ui.renderWidget->updateGL();
}

void joint_deformation::setShowNum()
{
	int num = ui.spinBox_gridDensity_2->text().toInt();
	ui.renderWidget->showNum1 = num;
	num = ui.spinBox_gridDensity_3->text().toInt();
	ui.renderWidget->showNum2 = num;
	ui.renderWidget->updateGL();

	//test final result
	p_kernel->flag_constraint_fixed = !p_kernel->flag_constraint_fixed;
}

void joint_deformation::testNeighbor()
{
	int num = ui.spinBox_gridDensity_2->text().toInt();
	ui.renderWidget->showNumNeighbor = num;
	ui.renderWidget->updateGL();
}

void joint_deformation::testParentNeighbor()
{
	int num = ui.spinBox_gridDensity_2->text().toInt();
	ui.renderWidget->showNumParentNeighbor = num;
	ui.renderWidget->updateGL();
}

void joint_deformation::testVox4Vertex()
{
	int num = ui.spinBox_vox_vertex->text().toInt();
	ui.renderWidget->showVox4Vertex = num;
	ui.renderWidget->updateGL();
}

void joint_deformation::testVox4Idx()
{
	ui.renderWidget->showVox4Idx[0] = ui.spinBox_voxX->value();
	ui.renderWidget->showVox4Idx[1] = ui.spinBox_voxY->value();
	ui.renderWidget->showVox4Idx[2] = ui.spinBox_voxZ->value();
	ui.renderWidget->showVox4Idx[3] = ui.spinBox_gridDensity_vertex->value();
	ui.renderWidget->updateGL();
}

void joint_deformation::showTest(bool a)
{
	ui.renderWidget->flag_show_test = a;
	ui.renderWidget->updateGL();
}

void joint_deformation::showConstraint(bool a)
{
	ui.renderWidget->flag_show_constraints = a;
	if(a && ui.renderWidget->flag_show_selection)
	{
		ui.renderWidget->flag_show_selection = false;
		ui.actionShowSelection->setChecked(false);
	}
	// remove the single constraints
	p_kernel->removeConstraintOfAllLevel();
	if(!a)
	{
		ui.renderWidget->bottom_right_x = -1.0;
		ui.renderWidget->bottom_right_y = -1.0;
		ui.renderWidget->upper_left_x   = -1.0;
		ui.renderWidget->upper_left_y = -1.0;
	}
	ui.renderWidget->updateGL();
}
void joint_deformation::setCubeOperation(bool a)
{
	ui.renderWidget->flag_cube_operation = a;
	ui.renderWidget->updateGL();
}
void joint_deformation::setCubeStaticConstraint(bool a)
{
	ui.renderWidget->flag_show_cube_static_constraints = a;
	if(a && ui.renderWidget->flag_show_cube_active_constraints)
	{
		ui.renderWidget->flag_show_cube_active_constraints = false;
		ui.actionSetCubeActiveConstraint->setChecked(false);
	}
	if(!a)
	{
		ui.renderWidget->bottom_right_x = -1.0;
		ui.renderWidget->bottom_right_y = -1.0;
		ui.renderWidget->upper_left_x   = -1.0;
		ui.renderWidget->upper_left_y = -1.0;
	}
	ui.renderWidget->updateGL();
}
void joint_deformation::setCubeActiveConstraint(bool a)
{
	ui.renderWidget->flag_show_cube_active_constraints = a;
	if(a && ui.renderWidget->flag_show_cube_static_constraints)
	{
		ui.renderWidget->flag_show_cube_static_constraints = false;
		ui.actionSetCubeStaticConstraint->setChecked(false);
	}
	if(!a)
	{
		ui.renderWidget->bottom_right_x = -1.0;
		ui.renderWidget->bottom_right_y = -1.0;
		ui.renderWidget->upper_left_x   = -1.0;
		ui.renderWidget->upper_left_y = -1.0;
	}
	ui.renderWidget->updateGL();
}
void joint_deformation::setPositionConstraint(bool a)
{
	p_kernel->constraintType = Kernel::POSITION_CONSTRAINT;
	ui.renderWidget->updateGL();
}

void joint_deformation::setOrientationConstraint(bool a)
{
	p_kernel->constraintType = Kernel::ORIENTATION_CONSTRAINT;
	ui.renderWidget->updateGL();
}

void joint_deformation::setDampParameter()
{
	double kappa= ui.lineEdit_damping->text().toDouble();
	for(int i = 0; i < p_kernel->level_list.size(); i ++)
	{
		vector<Cluster>::iterator ci;
		for( ci = p_kernel->level_list[i]->voxmesh_level->cluster_list.begin(); ci != p_kernel->level_list[i]->voxmesh_level->cluster_list.end(); ci++)
		{
			ci->kappa = kappa;
		}
	}
}

void joint_deformation::setMultigrid(bool a)
{
	p_kernel->flag_multigrid = a;
}

void joint_deformation::setDynamics(bool a)
{
	p_kernel->flag_dynamics = a;
}

void joint_deformation::setGravity(bool a)
{
	if(a && (!p_kernel->flag_dynamics))
	{
		QMessageBox::warning(NULL, "warning", "Please enable dynamics!");
		ui.checkBox_gravity->setChecked(false);
		return;
	}
	p_kernel->flag_gravity = a;

}

void joint_deformation::setGravityMagnitude()
{
	p_kernel->gravity_magnitude = (-1) * ui.lineEdit_gravityMagnitude->text().toDouble();
}

void joint_deformation::setWindMagnitude()
{
	p_kernel->wind_magnitude = ui.lineEdit_windMagnitude->text().toDouble();
}

void joint_deformation::setTimesShapeMatching()
{
	int d = ui.spinBox_TimesShapeMatching->value();
	int index = p_kernel->level_display;
	p_kernel->level_list[index]->times_ShapeMatching = d;
	ui.renderWidget->updateGL();
}

void joint_deformation::setForce()
{
	p_kernel->const_force = Vector3d(ui.lineEdit_forceX->text().toDouble(), ui.lineEdit_forceY->text().toDouble(), ui.lineEdit_forceZ->text().toDouble());
}

void joint_deformation::enableSetForce(bool a)

{
	p_kernel->flag_setForce = a;
	p_kernel->timestep_begin = p_kernel->time_step_index;
}

void joint_deformation::enableSetWindForce(bool a)
{
	p_kernel->flag_setWindForce = a;
}

void joint_deformation::setForceScalar()
{
	p_kernel->force_scalar = ui.lineEdit_forceScalar->text().toDouble();
}

void joint_deformation::setExportObj(bool a)
{
	flag_exportObj = a;
}

void joint_deformation::setExportTxt(bool a)
{
	p_kernel->flag_exportTxt = a;
}


void joint_deformation::setCaptureScreen(bool a)
{
	flag_captureScreen = a;
}

void joint_deformation::setCaptureSubScreen(bool a)
{
	flag_captureSubScreen = a;
}


void joint_deformation::setForceConstraint(bool a)
{
	if(a)
	{
		p_kernel->constraintType = Kernel::FORCE_CONSTRAINT;
		//ui.radioButton_setPositionConstraint->setChecked(false);
	}
}

void joint_deformation::setNetwork(bool a)
{
	if(a)
	{
		if(!p_kernel->flag_simulator_ready)
		{
			QMessageBox::warning(NULL, "warning", "initialize simulator first");
			ui.checkBox_Network->setChecked(false);
			return;
		}
	}
	ui.radioButton_Server->setEnabled(a);
	ui.radioButton_Client->setEnabled(a);
	ui.pushButton_setNumOfClients->setEnabled(a);
	ui.pushButton_configNetwork->setEnabled(a);
	
}

void joint_deformation::setServer(bool a)
{
	if(a)
		p_kernel->setServerRole();
	else
		p_kernel->setClientRole();
}

void joint_deformation::setClient(bool a)
{
	if(a)
		p_kernel->setClientRole();
	else
		p_kernel->setServerRole();
}

void joint_deformation::setNumOfClients()
{
	p_kernel->numOfClients = ui.spinBox_NumClients->value();
}
void joint_deformation::configNetwork()
{
	if(ui.lineEdit_IP->text().size() == 0)
	{
		QMessageBox::warning(NULL, "warning", "Please input ip first");
		return;
	}
	if(ui.lineEdit_Port->text().size() == 0)
	{
		QMessageBox::warning(NULL, "warning", "Please input ip first");
		return;
	}
	string value = ui.lineEdit_IP->text().toStdString();
	p_kernel->ip = new char[value.size() + 1];
	std::copy(value.begin(), value.end(), p_kernel->ip);
	p_kernel->ip[value.size()] = '\0';
	p_kernel->port = ui.lineEdit_Port->text().toInt();
	if(p_kernel->network_role == 0)
	{
		QMessageBox::warning(NULL, "warning", "Please choose network role first");
		return;
	}

	p_kernel->configNetwork();
	
	QString message2(p_kernel->ip);
	message2.append(" connected at ");
	message2.append(QString::number(p_kernel->port));
	ui.listWidget_Status->addItem(message2);

	if(p_kernel->network_role == Kernel::NETWORK_ROLE_CLIENT)
	{
		ui.renderWidget->flag_show_mesh = true;
		ui.actionScaleMesh->setChecked(true);
		ui.actionShowMesh->setChecked(true);
		p_kernel->flag_mesh_ready = true;
		startSimulation();
	}
}

void joint_deformation::loadCamConfig()
{
	ifstream ifs;
	ifs.open("camConfig.txt", ifstream::in);
	double angle, pos0, pos1, pos2;
	ifs >> pos0;
	ifs >> pos1;
	ifs >> pos2;

	ui.renderWidget->theCamera.pos(0) = pos0;
	ui.renderWidget->theCamera.pos(1) = pos1;
	ui.renderWidget->theCamera.pos(2) = pos2;

	ifs >> angle;
	ifs >> pos0;
	ifs >> pos1;
	ifs >> pos2;
	ui.renderWidget->theCamera.rotation.angle() = angle;
	ui.renderWidget->theCamera.rotation.axis()(0) = pos0;
	ui.renderWidget->theCamera.rotation.axis()(1) = pos1;
	ui.renderWidget->theCamera.rotation.axis()(2) = pos2;


	ifs >> ui.renderWidget->theCamera.fov;

	ifs.close();
	//ui.renderWidget->theCamera.RestoreSave();
	ui.renderWidget->updateGL();
}

void joint_deformation::saveCamConfig()
{
	ofstream ofs;
	ofs.open("camConfig.txt", ofstream::out);
	ofs << ui.renderWidget->theCamera.pos(0) << " " 
		<< ui.renderWidget->theCamera.pos(1) << " "
		<< ui.renderWidget->theCamera.pos(2) << endl;

	ofs << ui.renderWidget->theCamera.rotation.angle() << endl;
	ofs << ui.renderWidget->theCamera.rotation.axis()(0) << " "
		<< ui.renderWidget->theCamera.rotation.axis()(1) << " "
		<< ui.renderWidget->theCamera.rotation.axis()(2) << endl;

	ofs << ui.renderWidget->theCamera.fov << endl;
	ofs.close();

	char * msg = "Save Camera Confg Success!";
	QMessageBox::information(NULL, "Done", msg);

}

void joint_deformation::sendInitData()
{
	if(p_kernel->flag_network_ready && p_kernel->network_role == Kernel::NETWORK_ROLE_SERVER)
	{
		for(int i = 0; i < p_kernel->pTransmitter.size(); i++)
		{
			if(p_kernel->pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_PC)
				p_kernel->pTransmitter[i]->HSMupdate(p_kernel->p_mesh);
			//else if(p_kernel->pTransmitter[i]->_type == HN_TRANSMITTER_TYPE_MOBILE)
			//	p_kernel->pTransmitter[i]->HSMinit(p_kernel->data4Mobile);
		}
		
	}
}

void joint_deformation::setCompressedData(bool a)
{
	p_kernel->flag_compressData = a;
}

void joint_deformation::testSleepTime()
{
	p_kernel->timeSleep = ui.lineEdit_sleep->text().toInt();
}

void joint_deformation::testFunction()
{
	//if(!p_kernel->p_voxel)
	//{
	//	delete []p_kernel->p_voxel;
	//	p_kernel->p_voxel = NULL;
	//}

	//p_kernel->p_voxel = new unsigned int[729];

	//memset(p_kernel->p_voxel, 0, sizeof(unsigned int)*729);
	//p_kernel->mark_fillInterior4VoxelLevel(p_kernel->p_voxel, 3, 3);
	//for(int i = 0; i < 729; i++)
	//	cout << i << ": " << p_kernel->p_voxel[i] << " ";
	//cout << endl;
	p_kernel->test();
}

void joint_deformation::setMass()
{
	ui.renderWidget->setMass(ui.lineEdit_mass->text().toDouble());
}

void joint_deformation::resetMass()
{
	p_kernel->resetMass4Level();
}

void joint_deformation::setEnableMass(bool value)
{
	if (!p_kernel->flag_vox_ready)
	{
		QMessageBox::warning(NULL, "warning", "generate voxel mesh first");
		ui.actionSetMass->setChecked(false);
		return;
	}

	ui.renderWidget->flag_show_mass = value;
	if(value && ui.renderWidget->flag_show_constraints)
	{
		ui.renderWidget->flag_show_constraints = false;
		ui.actionSetConstraint->setChecked(false);
	}
	if(value && ui.renderWidget->flag_show_selection)
	{
		ui.renderWidget->flag_show_selection = false;
		ui.actionShowSelection->setChecked(false);
	}
	ui.renderWidget->updateGL();
}

void joint_deformation::testVoxConnection()
{
	vector<Vox>::iterator v_iter;
	int count = 0;
	for(v_iter = p_kernel->level_list[1]->voxmesh_level->vox_list.begin(); v_iter != p_kernel->level_list[1]->voxmesh_level->vox_list.end(); v_iter++)
	{
		if(count == 141 || count == 425)
			count = count + 0;
		//1
		if(v_iter->back_neighbor != NULL)
		{
			if(v_iter->back_neighbor->node_3 != v_iter->node_0)
				cout << "duplicate:" << count << endl;
			if(v_iter->back_neighbor->node_2 != v_iter->node_1)
				cout << "duplicate:" << count << endl;
			if(v_iter->back_neighbor->node_6 != v_iter->node_5)
				cout << "duplicate:" << count << endl;
			if(v_iter->back_neighbor->node_7 != v_iter->node_4)
				cout << "duplicate:" << count << endl;
		}
		//2 
		if(v_iter->bottom_back_neighbor != NULL)
		{
			if(v_iter->node_4 != v_iter->bottom_back_neighbor->node_3)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_5 != v_iter->bottom_back_neighbor->node_2)
				cout << "duplicate:" << count << endl;
		}
		//3
		if(v_iter->bottom_front_neighbor != NULL)
		{
			if(v_iter->node_7 != v_iter->bottom_front_neighbor->node_0)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_6 != v_iter->bottom_front_neighbor->node_1)
				cout << "duplicate:" << count << endl;
		}
		//4
		if(v_iter->bottom_left_back_neighbor != NULL)
		{
			if(v_iter->node_4 != v_iter->bottom_left_back_neighbor->node_2)
				cout << "duplicate:" << count << endl;
		}
		//5
		if(v_iter->bottom_left_front_neighbor != NULL)
		{
			if(v_iter->node_7 != v_iter->bottom_left_front_neighbor->node_1)
				cout << "duplicate:" << count << endl;
		}
		//6
		if(v_iter->bottom_left_neighbor != NULL)
		{
			if(v_iter->node_4 != v_iter->bottom_left_neighbor->node_1)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_7 != v_iter->bottom_left_neighbor->node_2)
				cout << "duplicate:" << count << endl;
		}
		//7
		if(v_iter->bottom_neighbor != NULL)
		{
			if(v_iter->node_4 != v_iter->bottom_neighbor->node_0)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_5 != v_iter->bottom_neighbor->node_1)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_6 != v_iter->bottom_neighbor->node_2)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_7 != v_iter->bottom_neighbor->node_3)
				cout << "duplicate:" << count << endl;
		}
		//8
		if(v_iter->bottom_right_back_neighbor != NULL)
		{
			if(v_iter->node_5 != v_iter->bottom_right_back_neighbor->node_3)
				cout << "duplicate:" << count << endl;
		}
		//9
		if(v_iter->bottom_right_front_neighbor != NULL)
		{
			if(v_iter->node_6 != v_iter->bottom_right_front_neighbor->node_0)
				cout << "duplicate:" << count << endl;
		}
		//10
		if(v_iter->bottom_right_neighbor != NULL)
		{
			if(v_iter->node_5 != v_iter->bottom_right_neighbor->node_0)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_6 != v_iter->bottom_right_neighbor->node_3)
				cout << "duplicate:" << count << endl;
		}
		//11
		if(v_iter->front_neighbor != NULL)
		{
			if(v_iter->node_2 != v_iter->front_neighbor->node_1)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_3 != v_iter->front_neighbor->node_0)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_7 != v_iter->front_neighbor->node_4)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_6 != v_iter->front_neighbor->node_5)
				cout << "duplicate:" << count << endl;
		}
		//12
		if(v_iter->left_back_neighbor != NULL)
		{
			if(v_iter->node_0 != v_iter->left_back_neighbor->node_2)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_4 != v_iter->left_back_neighbor->node_6)
				cout << "duplicate:" << count << endl;
		}
		//13
		if(v_iter->left_front_neighbor != NULL)
		{
			if(v_iter->node_3 != v_iter->left_front_neighbor->node_1)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_7 != v_iter->left_front_neighbor->node_5)
				cout << "duplicate:" << count << endl;
		}
		//14
		if(v_iter->left_neighbor != NULL)
		{
			if(v_iter->node_0 != v_iter->left_neighbor->node_1)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_4 != v_iter->left_neighbor->node_5)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_7 != v_iter->left_neighbor->node_6)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_3 != v_iter->left_neighbor->node_2)
				cout << "duplicate:" << count << endl;
		}
		//15
		if(v_iter->right_back_neighbor != NULL)
		{
			if(v_iter->node_1 != v_iter->right_back_neighbor->node_3)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_5 != v_iter->right_back_neighbor->node_7)
				cout << "duplicate:" << count << endl;
		}
		//16
		if(v_iter->right_front_neighbor != NULL)
		{
			if(v_iter->node_2 != v_iter->right_front_neighbor->node_0)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_6 != v_iter->right_front_neighbor->node_4)
				cout << "duplicate:" << count << endl;
		}
		//17
		if(v_iter->right_neighbor != NULL)
		{
			if(v_iter->node_1 != v_iter->right_neighbor->node_0)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_2 != v_iter->right_neighbor->node_3)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_6 != v_iter->right_neighbor->node_7)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_5 != v_iter->right_neighbor->node_4)
				cout << "duplicate:" << count << endl;
		}
		//18
		if(v_iter->top_back_neighbor != NULL)
		{
			if(v_iter->node_1 != v_iter->top_back_neighbor->node_6)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_0 != v_iter->top_back_neighbor->node_7)
				cout << "duplicate:" << count << endl;
		}
		//19
		if(v_iter->top_front_neighbor != NULL)
		{
			if(v_iter->node_2 != v_iter->top_front_neighbor->node_5)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_3 != v_iter->top_front_neighbor->node_4)
				cout << "duplicate:" << count << endl;
		}
		//20
		if(v_iter->top_left_back_neighbor != NULL)
		{
			if(v_iter->node_0 != v_iter->top_left_back_neighbor->node_6)
				cout << "duplicate:" << count << endl;
		}
		//21
		if(v_iter->top_left_front_neighbor != NULL)
		{
			if(v_iter->node_3 != v_iter->top_left_front_neighbor->node_5)
				cout << "duplicate:" << count << endl;
		}
		//22
		if(v_iter->top_left_neighbor != NULL)
		{
			if(v_iter->node_0 != v_iter->top_left_neighbor->node_5)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_3 != v_iter->top_left_neighbor->node_6)
				cout << "duplicate:" << count << endl;
		}
		//23
		if(v_iter->top_neighbor != NULL)
		{
			if(v_iter->node_0 != v_iter->top_neighbor->node_4)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_1 != v_iter->top_neighbor->node_5)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_2 != v_iter->top_neighbor->node_6)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_3 != v_iter->top_neighbor->node_7)
				cout << "duplicate:" << count << endl;
		}
		//24
		if(v_iter->top_right_back_neighbor != NULL)
		{
			if(v_iter->node_1 != v_iter->top_right_back_neighbor->node_7)
				cout << "duplicate:" << count << endl;
		}
		//25
		if(v_iter->top_right_front_neighbor != NULL)
		{
			if(v_iter->node_2 != v_iter->top_right_front_neighbor->node_4)
				cout << "duplicate:" << count << endl;
		}
		//26
		if(v_iter->top_right_neighbor != NULL)
		{
			if(v_iter->node_1 != v_iter->top_right_neighbor->node_4)
				cout << "duplicate:" << count << endl;
			if(v_iter->node_2 != v_iter->top_right_neighbor->node_7)
				cout << "duplicate:" << count << endl;
		}
		/*
		Vector3d c = v_iter->node_0->coordinate - v_iter->node_1->coordinate;
		if(c.norm() > 0.0416667 || c.norm() == 0)
			cout << "abnormal:" << count << endl;
		c = v_iter->node_0->coordinate - v_iter->node_3->coordinate;
		if(c.norm() > 0.0416667 || c.norm() == 0)
			cout << "abnormal:" << count << endl;
		c = v_iter->node_0->coordinate - v_iter->node_4->coordinate;
		if(c.norm() > 0.0416667 || c.norm() == 0)
			cout << "abnormal:" << count << endl;
		c = v_iter->node_1->coordinate - v_iter->node_2->coordinate;
		if(c.norm() > 0.0416667 || c.norm() == 0)
			cout << "abnormal:" << count << endl;		
		c = v_iter->node_1->coordinate - v_iter->node_5->coordinate;
		if(c.norm() > 0.0416667 || c.norm() == 0)
			cout << "abnormal:" << count << endl;
		c = v_iter->node_2->coordinate - v_iter->node_6->coordinate;
		if(c.norm() > 0.0416667 || c.norm() == 0)
			cout << "abnormal:" << count << endl;		
		c = v_iter->node_3->coordinate - v_iter->node_7->coordinate;
		if(c.norm() > 0.0416667 || c.norm() == 0)
			cout << "abnormal:" << count << endl;		
		c = v_iter->node_4->coordinate - v_iter->node_7->coordinate;
		if(c.norm() > 0.0416667 || c.norm() == 0)
			cout << "abnormal:" << count << endl;		
		c = v_iter->node_4->coordinate - v_iter->node_5->coordinate;
		if(c.norm() > 0.0416667 || c.norm() == 0)
			cout << "abnormal:" << count << endl;		
		c = v_iter->node_5->coordinate - v_iter->node_6->coordinate;
		if(c.norm() > 0.0416667 || c.norm() == 0)
			cout << "abnormal:" << count << endl;		
		c = v_iter->node_6->coordinate - v_iter->node_7->coordinate;
		if(c.norm() > 0.0416667 || c.norm() == 0)
			cout << "abnormal:" << count << endl;
		
		cout << c.norm() << endl;
		*/
		count ++;
	}

}

void joint_deformation::setEnergyThreshold()
{
	p_kernel->energyThreshold = ui.lineEdit_energyThreshold->text().toDouble();
}

void joint_deformation::loadConstraints()
{
	string filename;
	if (p_kernel->level_list.size() == 1)
		filename = "constraints4naive.txt";
	else
		filename = "constraints.txt";
	ifstream ifs(filename);
	char line[1024];
	char * token;
	while (!ifs.eof())
	{
		ifs.getline(line, 1024);
		if (strlen(line) == 0)
			break;
		token = strtok(line, " ");
		int level = atoi(token);
		token = strtok(NULL, " ");
		int z = atoi(token);
		token = strtok(NULL, " ");
		int y = atoi(token);
		token = strtok(NULL, " ");
		int x = atoi(token);
		token = strtok(NULL, " ");
		int nodei = atoi(token);
		//traverse level vox, find the vox, and find corresponding node
		vector<Vox>::iterator vi = p_kernel->level_list[level]->voxmesh_level->vox_list.begin();
		for (; vi != p_kernel->level_list[level]->voxmesh_level->vox_list.end(); vi++)
		{
			if (z == vi->coord_grid(0) && y == vi->coord_grid(1) && x == vi->coord_grid(2))
			{
				Node * temp = NULL;
				if (nodei == 0)
					temp = vi->node_0;
				else if (nodei == 1)
					temp = vi->node_1;
				else if (nodei == 2)
					temp = vi->node_2;
				else if (nodei == 3)
					temp = vi->node_3;
				else if (nodei == 4)
					temp = vi->node_4;
				else if (nodei == 5)
					temp = vi->node_5;
				else if (nodei == 6)
					temp = vi->node_6;
				else if (nodei == 7)
					temp = vi->node_7;
				temp->flag_constraint_node = true;
				p_kernel->level_list[level]->voxmesh_level->constraint_node_list.push_back(temp);
				for (int k=0; k < temp->duplicates.size(); ++k)
				{
					temp->duplicates[k]->flag_constraint_node = true;
					temp->prescribed_position = temp->target_position;
					temp->prescribed_preposition = temp->target_position;
				}
				for(int j=0; j < temp->incident_cluster.size(); ++j)
				{
					temp->incident_cluster[j]->flag_constrained = true;
					temp->incident_cluster[j]->constraint_node = NULL;
					p_kernel->level_list[level]->voxmesh_level->constraint_cluster_list.push_back(temp->incident_cluster[j]);
				}
				break;
			}
		}
	}
	int l = p_kernel->level_list.size() - 1;
	for(int i = 0; i < p_kernel->level_list.size(); i ++)
	{
		int size_k = p_kernel->level_list[l]->voxmesh_level->constraint_node_list.size();
		Vector3d sum = Vector3d::Zero();
		for(int k = 0; k < size_k; k ++)
		{
			sum += p_kernel->level_list[l]->voxmesh_level->constraint_node_list[k]->prescribed_position;
		}
		p_kernel->level_list[i]->voxmesh_level->constraint_center = sum / size_k;
		p_kernel->level_list[i]->voxmesh_level->constraint_displacement.clear();
		for(int j = 0; j < p_kernel->level_list[i]->voxmesh_level->constraint_node_list.size(); j++)
		{
			Vector3d displacement = p_kernel->level_list[i]->voxmesh_level->constraint_node_list[j]->coordinate 
				+ p_kernel->level_list[i]->voxmesh_level->constraint_node_list[j]->displacement - p_kernel->level_list[i]->voxmesh_level->constraint_center;
			p_kernel->level_list[i]->voxmesh_level->constraint_displacement.push_back(displacement);
		}
	}
	p_kernel->constraint_first = p_kernel->level_list[l]->voxmesh_level->constraint_center;
	char msg[1024];
	int cx = 0;
	for (int i = 0; i < p_kernel->level_list.size(); i++)
	{
		cx += sprintf(msg+cx, "Level %d:  %d constraint nodes have been chosen\n", i, p_kernel->level_list[i]->voxmesh_level->constraint_node_list.size());
	}
	QMessageBox::information(NULL, tr("success"), tr(msg));
	cout << "load constraints" << endl;
}

void joint_deformation::saveConstraints()
{
	ofstream ofs("constraints.txt");
	for(int i = 0; i < p_kernel->level_list.size(); i ++)
	{
		vector<Node *>::iterator ni = p_kernel->level_list[i]->voxmesh_level->constraint_node_list.begin();
		for(; ni != p_kernel->level_list[i]->voxmesh_level->constraint_node_list.end(); ni++)
		{
			int nodei;
			if ((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_0)
				nodei = 0;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_1))
				nodei = 1;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_2))
				nodei = 2;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_3))
				nodei = 3;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_4))
				nodei = 4;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_5))
				nodei = 5;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_6))
				nodei = 6;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_7))
				nodei = 7;
			ofs << i << " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(0)
				<< " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(1)
				<< " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(2) 
				<< " " << nodei << endl;
		}
	}
	ofs.close();
	ofs.open("constraints4naive.txt");
	int l = p_kernel->level_list.size() - 1;
	vector<Node *>::iterator ni = p_kernel->level_list[l]->voxmesh_level->constraint_node_list.begin();
	for(; ni != p_kernel->level_list[l]->voxmesh_level->constraint_node_list.end(); ni++)
	{
		int nodei;
		if ((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_0)
			nodei = 0;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_1))
			nodei = 1;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_2))
			nodei = 2;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_3))
			nodei = 3;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_4))
			nodei = 4;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_5))
			nodei = 5;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_6))
			nodei = 6;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_7))
			nodei = 7;
		ofs << "0" << " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(0)
			<< " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(1)
			<< " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(2) 
			<< " " << nodei << endl;
	}
	ofs.close();
	cout << "save constraints" << endl;
}

void joint_deformation::loadConstraints2()
{
	string filename;
	if (p_kernel->level_list.size() == 1)
		filename = "constraints24naive.txt";
	else
		filename = "constraints2.txt";
	ifstream ifs(filename);
	char line[1024];
	char * token;
	while (!ifs.eof())
	{
		ifs.getline(line, 1024);
		if (strlen(line) == 0)
			break;
		token = strtok(line, " ");
		int level = atoi(token);
		token = strtok(NULL, " ");
		int z = atoi(token);
		token = strtok(NULL, " ");
		int y = atoi(token);
		token = strtok(NULL, " ");
		int x = atoi(token);
		token = strtok(NULL, " ");
		int nodei = atoi(token);
		//traverse level vox, find the vox, and find corresponding node
		vector<Vox>::iterator vi = p_kernel->level_list[level]->voxmesh_level->vox_list.begin();
		for (; vi != p_kernel->level_list[level]->voxmesh_level->vox_list.end(); vi++)
		{
			if (z == vi->coord_grid(0) && y == vi->coord_grid(1) && x == vi->coord_grid(2))
			{
				Node * temp = NULL;
				if (nodei == 0)
					temp = vi->node_0;
				else if (nodei == 1)
					temp = vi->node_1;
				else if (nodei == 2)
					temp = vi->node_2;
				else if (nodei == 3)
					temp = vi->node_3;
				else if (nodei == 4)
					temp = vi->node_4;
				else if (nodei == 5)
					temp = vi->node_5;
				else if (nodei == 6)
					temp = vi->node_6;
				else if (nodei == 7)
					temp = vi->node_7;
				temp->flag_constraint_node = true;
				p_kernel->level_list[level]->voxmesh_level->another_constraint_node_list.push_back(temp);
				for (int k=0; k < temp->duplicates.size(); ++k)
				{
					temp->duplicates[k]->flag_constraint_node = true;
					temp->prescribed_position = temp->target_position;
					temp->prescribed_preposition = temp->target_position;
				}
				for(int j=0; j < temp->incident_cluster.size(); ++j)
				{
					temp->incident_cluster[j]->flag_constrained = true;
					temp->incident_cluster[j]->constraint_node = NULL;
					p_kernel->level_list[level]->voxmesh_level->constraint_cluster_list.push_back(temp->incident_cluster[j]);
				}
				break;
			}
		}
	}
	int l = p_kernel->level_list.size() - 1;
	for(int i = 0; i < p_kernel->level_list.size(); i ++)
	{
		int size_k = p_kernel->level_list[l]->voxmesh_level->another_constraint_node_list.size();
		Vector3d sum = Vector3d::Zero();
		for(int k = 0; k < size_k; k ++)
		{
			sum += p_kernel->level_list[l]->voxmesh_level->another_constraint_node_list[k]->prescribed_position;
		}
		p_kernel->level_list[i]->voxmesh_level->another_constraint_center = sum / size_k;
		p_kernel->level_list[i]->voxmesh_level->another_constraint_displacement.clear();
		for(int j = 0; j < p_kernel->level_list[i]->voxmesh_level->another_constraint_node_list.size(); j++)
		{
			Vector3d displacement = p_kernel->level_list[i]->voxmesh_level->another_constraint_node_list[j]->coordinate 
				+ p_kernel->level_list[i]->voxmesh_level->another_constraint_node_list[j]->displacement - p_kernel->level_list[i]->voxmesh_level->another_constraint_center;
			p_kernel->level_list[i]->voxmesh_level->another_constraint_displacement.push_back(displacement);
		}
	}
	p_kernel->constraint_second = p_kernel->level_list[l]->voxmesh_level->another_constraint_center;
	char msg[1024];
	int cx = 0;
	for (int i = 0; i < p_kernel->level_list.size(); i++)
	{
		cx += sprintf(msg+cx, "Level %d:  %d constraint nodes have been chosen\n", i, p_kernel->level_list[i]->voxmesh_level->another_constraint_node_list.size());
	}
	QMessageBox::information(NULL, tr("success"), tr(msg));
	cout << "load another constraints" << endl;
}

void joint_deformation::saveConstraints2()
{
	ofstream ofs("constraints2.txt");
	for(int i = 0; i < p_kernel->level_list.size(); i ++)
	{
		vector<Node *>::iterator ni = p_kernel->level_list[i]->voxmesh_level->another_constraint_node_list.begin();
		for(; ni != p_kernel->level_list[i]->voxmesh_level->another_constraint_node_list.end(); ni++)
		{
			int nodei;
			if ((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_0)
				nodei = 0;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_1))
				nodei = 1;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_2))
				nodei = 2;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_3))
				nodei = 3;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_4))
				nodei = 4;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_5))
				nodei = 5;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_6))
				nodei = 6;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_7))
				nodei = 7;
			ofs << i << " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(0)
				<< " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(1)
				<< " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(2) 
				<< " " << nodei << endl;
		}
	}
	ofs.close();
	ofs.open("constraints24naive.txt");
	int l = p_kernel->level_list.size() - 1;
	vector<Node *>::iterator ni = p_kernel->level_list[l]->voxmesh_level->another_constraint_node_list.begin();
	for(; ni != p_kernel->level_list[l]->voxmesh_level->another_constraint_node_list.end(); ni++)
	{
		int nodei;
		if ((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_0)
			nodei = 0;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_1))
			nodei = 1;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_2))
			nodei = 2;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_3))
			nodei = 3;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_4))
			nodei = 4;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_5))
			nodei = 5;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_6))
			nodei = 6;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_7))
			nodei = 7;
		ofs << "0" << " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(0)
			<< " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(1)
			<< " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(2) 
			<< " " << nodei << endl;
	}
	ofs.close();
	cout << "save constraints" << endl;
}

void joint_deformation::loadAnchor()
{
	string filename;
	if (p_kernel->level_list.size() == 1)
		filename = "anchor4naive.txt";
	else
		filename = "anchor.txt";
	ifstream ifs(filename);
	char line[1024];
	char * token;
	while (!ifs.eof())
	{
		ifs.getline(line, 1024);
		if (strlen(line) == 0)
			break;
		token = strtok(line, " ");
		int level = atoi(token);
		token = strtok(NULL, " ");
		int z = atoi(token);
		token = strtok(NULL, " ");
		int y = atoi(token);
		token = strtok(NULL, " ");
		int x = atoi(token);
		token = strtok(NULL, " ");
		int nodei = atoi(token);
		//traverse level vox, find the vox, and find corresponding node
		vector<Vox>::iterator vi = p_kernel->level_list[level]->voxmesh_level->vox_list.begin();
		for (; vi != p_kernel->level_list[level]->voxmesh_level->vox_list.end(); vi++)
		{
			if (z == vi->coord_grid(0) && y == vi->coord_grid(1) && x == vi->coord_grid(2))
			{
				Node * temp = NULL;
				if (nodei == 0)
					temp = vi->node_0;
				else if (nodei == 1)
					temp = vi->node_1;
				else if (nodei == 2)
					temp = vi->node_2;
				else if (nodei == 3)
					temp = vi->node_3;
				else if (nodei == 4)
					temp = vi->node_4;
				else if (nodei == 5)
					temp = vi->node_5;
				else if (nodei == 6)
					temp = vi->node_6;
				else if (nodei == 7)
					temp = vi->node_7;
				temp->flag_anchor_node = true;
				p_kernel->level_list[level]->voxmesh_level->anchor_node_list.push_back(temp);
				
				break;
			}
		}
	}
	char msg[1024];
	int cx = 0;
	for (int i = 0; i < p_kernel->level_list.size(); i++)
	{
		cx += sprintf(msg+cx, "Level %d:  %d anchor nodes have been chosen\n", i, p_kernel->level_list[i]->voxmesh_level->anchor_node_list.size());
	}
	QMessageBox::information(NULL, tr("success"), tr(msg));
	cout << "load anchor nodes" << endl;
}

void joint_deformation::saveAnchor()
{
	ofstream ofs("anchor.txt");
	for(int i = 0; i < p_kernel->level_list.size(); i ++)
	{
		vector<Node *>::iterator ni = p_kernel->level_list[i]->voxmesh_level->anchor_node_list.begin();
		for(; ni != p_kernel->level_list[i]->voxmesh_level->anchor_node_list.end(); ni++)
		{
			int nodei;
			if ((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_0)
				nodei = 0;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_1))
				nodei = 1;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_2))
				nodei = 2;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_3))
				nodei = 3;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_4))
				nodei = 4;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_5))
				nodei = 5;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_6))
				nodei = 6;
			else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_7))
				nodei = 7;
			ofs << i << " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(0)
			<< " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(1)
			<< " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(2) 
			<< " " << nodei << endl;
		}
	}
	ofs.close();
	ofs.open("anchor4naive.txt");
	int l = p_kernel->level_list.size() - 1;
	vector<Node *>::iterator ni = p_kernel->level_list[l]->voxmesh_level->anchor_node_list.begin();
	for(; ni != p_kernel->level_list[l]->voxmesh_level->anchor_node_list.end(); ni++)
	{
		int nodei;
		if ((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_0)
			nodei = 0;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_1))
			nodei = 1;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_2))
			nodei = 2;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_3))
			nodei = 3;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_4))
			nodei = 4;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_5))
			nodei = 5;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_6))
			nodei = 6;
		else if(((*ni) == (*ni)->incident_cluster[0]->vox_list[0]->node_7))
			nodei = 7;
		ofs << "0" << " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(0)
			<< " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(1)
			<< " " << (*ni)->incident_cluster[0]->vox_list[0]->coord_grid(2) 
			<< " " << nodei << endl;
	}
	ofs.close();
	cout << "save anchor nodes" << endl;
}

void joint_deformation::loadForce()
{
	ifstream ifs("record.txt");
	char line[1024];
	char * token;
	while (!ifs.eof())
	{
		ifs.getline(line, 1024);
		if (strlen(line) == 0)
			break;
		token = strtok(line, " ");
		double x = atof(token);
		token = strtok(NULL, " ");
		double y = atof(token);
		token = strtok(NULL, " ");
		double z = atof(token);
		p_kernel->force_list.push_back(Vector3d(x, y, z));
	}
	p_kernel->flag_redo = true;
	cout << "load force " << p_kernel->force_list.size() << endl;
}

void joint_deformation::loadPosConstraint()
{
	ifstream ifs("record_first.txt");
	char line[1024];
	char * token;
	while (!ifs.eof())
	{
		ifs.getline(line, 1024);
		if (strlen(line) == 0)
			break;
		token = strtok(line, " ");
		double x = atof(token);
		token = strtok(NULL, " ");
		double y = atof(token);
		token = strtok(NULL, " ");
		double z = atof(token);
		p_kernel->force_list.push_back(Vector3d(x, y, z));
	}
	ifs.close();
	token = NULL;
	ifs.open("record_second.txt");	
	while (!ifs.eof())
	{
		ifs.getline(line, 1024);
		if (strlen(line) == 0)
			break;
		token = strtok(line, " ");
		double x = atof(token);
		token = strtok(NULL, " ");
		double y = atof(token);
		token = strtok(NULL, " ");
		double z = atof(token);
		p_kernel->another_force_list.push_back(Vector3d(x, y, z));
	}
	p_kernel->flag_redo = true;
	cout << "load Position  " << p_kernel->force_list.size() << endl;
}
void joint_deformation::loadStep()
{
	ifstream ifs("rt.txt");
	char line[1024];
	char * token;
	while (!ifs.eof())
	{
		ifs.getline(line, 1024);
		if (strlen(line) == 0)
			break;
		token = strtok(line, " ");
		int x = atoi(token);
		p_kernel->step_list.push_back(x);
	}
	ifs.close();
	cout << "load step  " << p_kernel->step_list.size() << endl;
}

void joint_deformation::setLeftSurface(bool a)
{
	ui.renderWidget->flag_left_surface = a;
	if(a)
	{
		ui.actionRightSurface->setChecked(false);
		ui.renderWidget->flag_right_surface = false;
	}
	ui.renderWidget->updateGL();
}

void joint_deformation::setRightSurface(bool a)
{
	ui.renderWidget->flag_right_surface = a;
	if(a)
	{

		ui.actionLeftSurface->setChecked(false);
		ui.renderWidget->flag_left_surface = false;
	}
	ui.renderWidget->updateGL();
}