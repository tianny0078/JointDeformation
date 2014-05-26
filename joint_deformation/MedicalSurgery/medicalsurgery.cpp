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
	default:
		break;
	}

}