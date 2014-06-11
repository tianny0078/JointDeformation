#include "medicalsurgery.h"

MedicalSurgery::MedicalSurgery(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	p_kernel = new Kernel;
	ui.renderWidget->p_kernel = p_kernel;
	ui.renderWidget->initArrow(p_kernel);

	flag_captureScreen = false;
	flag_captureSubScreen = false;
	flag_exportObj = false;

}

MedicalSurgery::~MedicalSurgery()
{

}

void MedicalSurgery::loadMesh()
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

		//ui.comboBox_selectTreeDepth->setEnabled(true);
		ui.renderWidget->updateGL();
	}
}

void MedicalSurgery::scaleMesh()
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

void MedicalSurgery::setGridDensity()
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
	//if(ui.comboBox_level->isEnabled())
		//level = ui.comboBox_level->currentIndex();

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

void MedicalSurgery::setAnchor(bool a)
{
	if (!p_kernel->flag_vox_ready)
	{
		QMessageBox::warning(NULL, "warning", "generate voxel mesh first");
		ui.actionSetAnchor->setChecked(false);
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

void MedicalSurgery::setConstraint(bool a)
{
	ui.renderWidget->flag_show_constraints = a;
	if(a && ui.renderWidget->flag_show_selection)
	{
		ui.renderWidget->flag_show_selection = false;
		ui.actionSetAnchor->setChecked(false);
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

void MedicalSurgery::showMesh(bool a)
{
	ui.renderWidget->flag_show_mesh = a;
	ui.renderWidget->updateGL();
}

void MedicalSurgery::showGoalPosition(bool a)
{
	ui.renderWidget->flag_show_target = a;
	ui.renderWidget->updateGL();
}

void MedicalSurgery::showVox(bool a)
{
	ui.renderWidget->flag_show_vox = a;
	ui.renderWidget->updateGL();
}

void MedicalSurgery::chooseSimulator(int t)
{
	//disable some parts of UI.
	//ui.comboBox_level->setDisabled(true);
	//ui.pushButton_addLevel->setDisabled(true);
	switch (t)
	{
	case 0:
		p_kernel->used_simulator = Kernel::UNDEFINED;
		break;
	case 1:
		p_kernel->used_simulator = Kernel::SHAPE_MATCHING;
		break;
	case 2:
		p_kernel->used_simulator = Kernel::FTL_ROPE;
		break;
	default:
		break;
	}

}

void MedicalSurgery::simulateNextStep()
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

void MedicalSurgery::startSimulation()
{
	if (!p_kernel->flag_simulator_ready)
	{
		QMessageBox::warning(NULL, "warning", "initialize simulator first");
		return;
	}

	if (!simulation_timer.isActive())
	{
		connect(&simulation_timer, SIGNAL(timeout()), this, SLOT(simulate()));
		ui.actionSetAnchor->setChecked(false);
		ui.renderWidget->flag_show_selection = false;
		ui.renderWidget->flag_simulating = true;
		simulation_timer.start(0);

	}
}

void MedicalSurgery::pauseSimulation()
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

void MedicalSurgery::simulate()
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
	ui.renderWidget->updateGL();
	/*
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
	*/
}

void MedicalSurgery::initializeSimulator()
{
	if (p_kernel->used_simulator == Kernel::UNDEFINED)
	{
		QMessageBox::warning(NULL, "warning", "choose simulator first");
		return;
	}
	p_kernel->initializeSimulator();

	QMessageBox::information(NULL, "success", "simulator initialized");
}

void MedicalSurgery::setDampParameter()
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

void MedicalSurgery::setDynamics(bool a)
{
	p_kernel->flag_dynamics = a;
}

void MedicalSurgery::setGravity(bool a)
{
	if(a && (!p_kernel->flag_dynamics))
	{
		QMessageBox::warning(NULL, "warning", "Please enable dynamics!");
		ui.checkBox_gravity->setChecked(false);
		return;
	}
	p_kernel->flag_gravity = a;
}

void MedicalSurgery::setGravityMagnitude()
{
	p_kernel->gravity_magnitude = (-1) * ui.lineEdit_gravityMagnitude->text().toDouble();
}

void MedicalSurgery::setMass()
{
	ui.renderWidget->setMass(ui.lineEdit_mass->text().toDouble());
}

void MedicalSurgery::setForceConstraint(bool a)
{
	if(a)
	{
		p_kernel->constraintType = Kernel::FORCE_CONSTRAINT;
	}
}

void MedicalSurgery::setPositionConstraint(bool a)
{
	p_kernel->constraintType = Kernel::POSITION_CONSTRAINT;
	ui.renderWidget->updateGL();
}

void MedicalSurgery::setOrientationConstraint(bool a)
{
	p_kernel->constraintType = Kernel::ORIENTATION_CONSTRAINT;
	ui.renderWidget->updateGL();
}

void MedicalSurgery::setRopeDragging(bool a)
{
	ui.renderWidget->flag_rope_dragging = a;
	ui.renderWidget->updateGL();
}

void MedicalSurgery::setJointNumber()
{
	p_kernel->p_rope->setNum(ui.lineEdit_JointNum->text().toInt());
	ui.renderWidget->updateGL();
}

void MedicalSurgery::setLength()
{
	p_kernel->p_rope->setLength(ui.lineEdit_Length->text().toDouble());
	ui.renderWidget->updateGL();
}