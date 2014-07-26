// Renderer.cpp: implementation of the Renderer class.
//
//////////////////////////////////////////////////////////////////////

#include "renderer.h"

void _stdcall quadric_error_cb(int signal)
{
	const GLubyte *estring;
	estring = gluErrorString(signal);
	std::cerr << "Quadric Error: " << estring << endl;
	exit(EXIT_FAILURE);
}


Renderer::Renderer(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	// accept key input
	setFocusPolicy(Qt::StrongFocus);

	current_render_mode = REGULAR;

	// colors
	background_color = QColor::fromRgb(255, 255, 255);
	
	render_mesh_color[0] = 0.0; render_mesh_color[1] = 162.0/255.0; render_mesh_color[2] = 232.0/255.0;
	grid_color[0] = 0.0; grid_color[1] = 1.0; grid_color[2] = 0;
	vox_mesh_color[0] = 1.0; vox_mesh_color[1] = 174.0/255.0; vox_mesh_color[2] = 201.0/255.0;
	status_color[0] = 128.0/255.0; status_color[1] = 128.0/255.0; status_color[2] = 1.0;
	anchor_node_color[0] = 1.0; anchor_node_color[1] = 0.5; anchor_node_color[2] = 0.25;
	active_node_color[0] = 0.0; active_node_color[1] = 0.0; active_node_color[2] = 1.0;
	node_color[0] = 112.0/255.0; node_color[1] = 146.0/255.0; node_color[2] = 190.0/255.0;
	anchor_cube_color[0] = 1.0; anchor_cube_color[1] = 1.0; anchor_cube_color[2] = 0.0;
	active_cube_color[0] = 0.0; active_cube_color[1] = 1.0; active_cube_color[2] = 1.0;
	
	flag_initialized = false;
	flag_show_grid = false;
	flag_show_mesh = false;
	flag_show_vox = false;
	flag_show_target = false;
	flag_show_selection = false;
	flag_simulating = false;
	flag_show_test = false;
	flag_show_constraints = false;
	flag_axis = 1;
	flag_show_mass = false;
	flag_cube_operation = false;
	flag_show_cube_static_constraints = false;
	flag_show_cube_active_constraints = false;
	flag_rope_dragging = false;
		
	rotate_dist = 0;
	view_dist = 0;
		
	gl_version = NULL;
	gl_vendor = NULL;
	gl_renderer = NULL;
	gl_extensions = NULL;
	glu_extensions = NULL;

	win_width = 0;
	win_height = 0;
		
	// selection square
	bottom_right_x = -1.0;
	bottom_right_y = -1.0;
	upper_left_x   = -1.0;
	upper_left_y   = -1.0;
				
	initCamera();

	showNum1 = 0;
	showNum2 = 128;
	showNumNeighbor = 0;
	showNumParentNeighbor = 0;
	showVox4Vertex = 0;
	showVox4Idx[0] = 0;
	showVox4Idx[1] = 0;
	showVox4Idx[2] = 0;
	showVox4Idx[3] = 0;

	LCS_translation = Vector3d::Zero();
	LCS_rotation = Matrix3d::Identity();

	LCS_x = Vector3d::Zero();
	LCS_x(0) = 0.3;
	LCS_y = Vector3d::Zero();
	LCS_y(1) = 0.3;
	LCS_z = Vector3d::Zero();
	LCS_z(2) = 0.3;

	//tool = new cubeTool();
	force_arrow.setZero();

	flag_left_surface = false;
	flag_right_surface = false;

	myArrow = NULL;

	idx_FLRegion = 0;
}

Renderer::~Renderer()
{
	
}

void Renderer::initArrow(Kernel * p)
{
	myArrow = new arrow(p);
	myArrow->setScale(0.03, 0.03);
	myArrow->setLength(0.05);
}

QSize Renderer::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize Renderer::sizeHint() const

{
	return QSize(400, 400);
}

void Renderer::initializeGL()
{
	if (!flag_initialized)
	{
		gl_version = glGetString(GL_VERSION);
		gl_vendor = glGetString(GL_VENDOR);
		gl_renderer = glGetString(GL_RENDERER);
		gl_extensions = glGetString(GL_EXTENSIONS);
		glu_extensions = gluGetString(GLU_EXTENSIONS);

		glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0, -1.0);

		glShadeModel(GL_SMOOTH);

		glEnable(GL_DEPTH_TEST);

		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		glEnable(GL_COLOR_MATERIAL);
		
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable (GL_LINE_SMOOTH);
		// non-integer line width makes the line more smooth 
		glLineWidth(1.1);
		
		glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		// set pixel alignment
		glPixelStorei(GL_PACK_ALIGNMENT, 1);

		initLighting();
		initQuadrics();
		initColorTab();
		qglClearColor(background_color);
			
		flag_initialized = true;
	}
}
	

void Renderer::paintGL()
{
	resizeGL(width(), height());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// setup matrix from camera
	theCamera.SetGL(view_dist/DOF, view_dist*DOF, win_width, win_height);

	//get the model view, projection, and viewport matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, currentmodelview);
	glGetDoublev(GL_PROJECTION_MATRIX, currentprojection);
	glGetIntegerv(GL_VIEWPORT, currentviewport);

	switch (current_render_mode)
	{
	case REGULAR:
		if (p_kernel->p_rope)
		{
			p_kernel->p_rope->render();
		}

		if (flag_cube_operation)
		{
			//tool->render();
		}

		if (flag_show_mesh && p_kernel->flag_mesh_ready)
		{
			renderMesh(p_kernel->p_mesh);
			/*
			glColor3f(0.0, 1.0, 0.0);
			glPushMatrix();
			glTranslatef(p_kernel->surface_point_left[0],p_kernel->surface_point_left[1],p_kernel->surface_point_left[2]);
			glutSolidSphere(0.03, 16, 16);
			glPopMatrix();
			glColor3f(1.0, 0.0, 0.0);
			glPushMatrix();
			glTranslatef(p_kernel->surface_point_right[0],p_kernel->surface_point_right[1],p_kernel->surface_point_right[2]);
			glutSolidSphere(0.03, 16, 16);
			glPopMatrix();
			*/
		}

		if (flag_show_grid)
		{
			renderGrid();
			//renderClusterHierarchy(p_kernel->p_vox_mesh);
		}

		if(flag_show_vox || flag_show_target)
		{
			renderSelectSquare(upper_left_x, upper_left_y, bottom_right_x, bottom_right_y);
			if (flag_show_vox)
			{
				//renderVoxMesh(p_kernel->p_vox_mesh);
				
				//level detail for the vox mesh
				Level * plevel = p_kernel->level_list[p_kernel->level_display];
				renderLevelVoxMesh(plevel);
			}

			if (flag_show_target)
			{
				float target_color[3] = {1.0, 0.0, 0.0};

				////current level(0-)
				Level * plevel = p_kernel->level_list[p_kernel->level_display];
				//renderLevelStaticPosition(plevel, target_color, showNum1, showNum2);
				renderLevelStaticPosition(plevel, target_color);
				///////////////////////display neighbor
				//renderNeighborStaticPosition(plevel, showNumNeighbor);
				////////////////////// display parent neighbor
				//if(p_kernel->level_display > 0)
				//{
				//	Level * plevel_parent = p_kernel->level_list[p_kernel->level_display-1];
				//	renderParentNeighborStaticPosition(plevel, plevel_parent, showNumParentNeighbor);
				//}
			}
		}
		if (flag_simulating)
		{
			renderForce();
		}


		if(flag_show_test)
		{
			//////////////////////////////////////////////////////////////////////////
			///// test if flsm region is right
			//////////////////////////////////////////////////////////////////////////
			//float color[] = {1.0, 0.0, 0.0};
			//renderFLParticle4Region(idx_FLRegion,color);
			//////////////////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////////
			///////// Render last level clusters to show the multi-resolution
			//////////////////////////////////////////////////////////////////
			//float target_color[3] = {1.0, 0.0, 0.0};
			//
			//for (int i = 0; i<p_kernel->level_list.size(); i++)
			//{
			//	renderLevelVoxMesh4Static(p_kernel->level_list[i]);
			//}
			/////////////////////////////////////////////////

			/*
			//renderLevelStaticPosition(plevel, target_color, showNum1, showNum2);
			///////////////////////display neighbor
			//renderNeighborStaticPosition(plevel, showNumNeighbor);
			////////////////////// display parent neighbor
			if(p_kernel->level_display > 0)
			{
				Level * plevel_parent = p_kernel->level_list[p_kernel->level_display-1];
				//renderParentNeighborStaticPosition(plevel, plevel_parent, showNumParentNeighbor);

			}
			////test whether vertex belows to correct vox
			//renderLevelVertex(p_kernel->p_mesh, plevel, showVox4Vertex);
			*/
			///////////////////////////////////////////////////////////////
			//test vox according to index
			//renderLevelVox(plevel, showVox4Idx);
			//renderLevelVertex(p_kernel->p_mesh, plevel, showVox4Vertex);
			///////////////////////////////////////////////////////////////
		
		}



		break;


	case TREE_SETTING:

		//renderClusterHierarchy(p_kernel->p_vox_mesh);

		if (flag_show_mesh && p_kernel->flag_mesh_ready)
		{
			renderMesh(p_kernel->p_mesh);
		}

		if (flag_show_grid)
		{
			renderGrid();
		}
		
		float light_mesh_color[] = {1.0, 1.0, 1.0};
		if (flag_show_vox)
		{
			renderVoxMesh(p_kernel->p_vox_mesh, light_mesh_color);
			renderSelectSquare(upper_left_x, upper_left_y, bottom_right_x, bottom_right_y);
		}
	}
			
	renderInvertedCluster();
	// aux rendering
	renderStatus();	
	renderWCS(-2.25);
	//renderWCS(-1);
}

void Renderer::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);

	win_width = width;
	win_height = height;
}

void Renderer::initLighting()
{
	GLfloat	light0_Ka[] = {0.4f,0.4f,0.4f,1.0f};
	GLfloat	light0_Kd[] = {0.5f,0.5f,0.5f,1.0f};
	GLfloat light0_Ks[] = {0.5f,0.5f,0.5f,1.0f};
	GLfloat light0_Pos[] = {1.0f,1.0f,1.0f,0.0f};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_Ka);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_Kd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_Ks);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_Pos);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// separate specular component
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
		
	light_pos[0] = light0_Pos[0];
	light_pos[1] = light0_Pos[1];
	light_pos[2] = light0_Pos[2];
	light_pos[3] = light0_Pos[3];
	
	return;
}


void Renderer::initCamera()
{
	Vector3d homepos, homerotaxis;

	// Since the model is always normalized, constants suffice
	homepos(0) = 0;
	homepos(1) = 0;
	homepos(2) = CAMERA_VIEW_DISTANCE;

	homerotaxis(0) = 0;
	homerotaxis(1) = 0;
	homerotaxis(2) = 1;
	
	theCamera.SetHome(homepos, 0, homerotaxis, DEFAULT_FOV);
	wcsCamera.SetHome(homepos, 0, homerotaxis, DEFAULT_FOV);

	rotate_dist = CAMERA_ROT_DISTANCE;
	view_dist   = CAMERA_VIEW_DISTANCE;
	
	theCamera.GoHome();
	wcsCamera.GoHome();
}

void Renderer::initQuadrics()
{
	quadric_list = glGenLists(4);

	GLUquadricObj *qobj;
	qobj = gluNewQuadric();
	gluQuadricCallback(qobj, GLU_ERROR, (GluQuadricCallbackType)quadric_error_cb);

	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	// A sphere quadric
	glNewList(quadric_list, GL_COMPILE);
	gluSphere(qobj, 0.02, 30, 20);
	glEndList();

	// A cone quadric
	glNewList(quadric_list+1, GL_COMPILE);
	gluCylinder(qobj, 0.04, 0, 0.08, 30, 20);
	glEndList();

	// A disk quadric
	glNewList(quadric_list+2, GL_COMPILE);
	gluDisk(qobj, 0, 0.04, 30, 20);
	glEndList();

	// A cylinder quadric
	glNewList(quadric_list+3, GL_COMPILE);
	gluCylinder(qobj, 0.02, 0.02, 0.4, 30, 20);
	glEndList();

}

//void Renderer::InitTextureData()
//{
//	int i, j, c;
//	bool bitmap_ok[3] = { false, false, false };
//
//	BitMap m_bitmap[3];
//	m_bitmap[0].Load("..\\res\\envmap.bmp");
//	if (m_bitmap[0].IsLoaded())
//		bitmap_ok[0] = true;
//	else
//		cerr << "Warning: cannot load default environment map texture data!" << endl;
//
//	m_bitmap[1].Load("..\\res\\floor.bmp");
//	if (m_bitmap[1].IsLoaded())
//		bitmap_ok[1] = true;
//	else
//		cerr << "Warning: cannot load default floor texture data!" << endl;
//
//	m_bitmap[2].Load("..\\res\\colorbar.bmp");
//	if (m_bitmap[2].IsLoaded())
//		bitmap_ok[2] = true;
//	else
//		cerr << "Warning: cannot load default colorbar texture data!" << endl;
//
//
//	for (i = 0; i < TEX_HEIGHT; i++)
//		for (j = 0; j < TEX_WIDTH; j++)
//		{
//			c = 255 * (((i&0x4)==0)^((j&0x4)==0));
//			check_texture[i][j][0] = (GLubyte)c;
//			check_texture[i][j][1] = (GLubyte)c;
//			check_texture[i][j][2] = (GLubyte)c;
//			check_texture[i][j][3] = (GLubyte)255;
//		}
//
//		for (i = 0; i < TEX_HEIGHT; i++)
//			for (j = 0; j < TEX_WIDTH; j++)
//			{
//				if (bitmap_ok[0])
//				{
//					COLORREF rgb = m_bitmap[0].GetPixel(j, TEX_HEIGHT-i-1);
//					env_texture[i][j][0] = (GLubyte)GetRValue(rgb);
//					env_texture[i][j][1] = (GLubyte)GetGValue(rgb);
//					env_texture[i][j][2] = (GLubyte)GetBValue(rgb);
//					env_texture[i][j][3] = (GLubyte)255;
//				}
//				else
//				{
//					env_texture[i][j][0] = check_texture[i][j][0];
//					env_texture[i][j][1] = check_texture[i][j][1];
//					env_texture[i][j][2] = check_texture[i][j][2];
//					env_texture[i][j][3] = check_texture[i][j][3];
//				}
//			}
//
//			for (i = 0; i < TEX_HEIGHT; i++)
//				for (j = 0; j < TEX_WIDTH; j++)
//				{
//					if (bitmap_ok[0])
//					{
//						COLORREF rgb = m_bitmap[1].GetPixel(j, TEX_HEIGHT-i-1);
//						floor_texture[i][j][0] = (GLubyte)GetRValue(rgb);
//						floor_texture[i][j][1] = (GLubyte)GetGValue(rgb);
//						floor_texture[i][j][2] = (GLubyte)GetBValue(rgb);
//						floor_texture[i][j][3] = (GLubyte)255;
//					}
//					else
//					{
//						floor_texture[i][j][0] = check_texture[i][j][0];
//						floor_texture[i][j][1] = check_texture[i][j][1];
//						floor_texture[i][j][2] = check_texture[i][j][2];
//						floor_texture[i][j][3] = check_texture[i][j][3];
//					}
//				}
//
//				for (i = 0; i < TEX_WIDTH; i++)
//				{
//					c = 255 * ((i&0x4)==0);
//					horizbar_texture[i*4+0] = (GLubyte)255;
//					horizbar_texture[i*4+1] = (GLubyte)c;
//					horizbar_texture[i*4+2] = (GLubyte)c;
//					horizbar_texture[i*4+3] = (GLubyte)255;
//				}
//
//				for (i = 0; i < TEX_WIDTH; i++)
//				{
//					c = 255 * ((i&0x4)==0);
//					vertbar_texture[i*4+0] = (GLubyte)c;
//					vertbar_texture[i*4+1] = (GLubyte)c;
//					vertbar_texture[i*4+2] = (GLubyte)255;
//					vertbar_texture[i*4+3] = (GLubyte)255;
//				}
//
//				for (i = 0; i < TEX_WIDTH; i++)
//				{
//					c = 255 * ((i%8)==0);
//					nrwhorizbar_texture[i*4+0] = (GLubyte)255;
//					nrwhorizbar_texture[i*4+1] = (GLubyte)(255-c);
//					nrwhorizbar_texture[i*4+2] = (GLubyte)(255-c);
//					nrwhorizbar_texture[i*4+3] = (GLubyte)255;
//				}
//
//				for (i = 0; i < TEX_WIDTH; i++)
//				{
//					c = 255 * ((i%8)==0);
//					nrwvertbar_texture[i*4+0] = (GLubyte)(255-c);
//					nrwvertbar_texture[i*4+1] = (GLubyte)(255-c);
//					nrwvertbar_texture[i*4+2] = (GLubyte)255;
//					nrwvertbar_texture[i*4+3] = (GLubyte)255;
//				}
//
//				for (i = 0; i < TEX_HEIGHT; i++)
//				{
//					if (bitmap_ok[2])
//					{
//						COLORREF rgb = m_bitmap[2].GetPixel(1, TEX_HEIGHT-i-1);
//						colorbar_texture[i*4+0] = (GLubyte)GetRValue(rgb);
//						colorbar_texture[i*4+1] = (GLubyte)GetGValue(rgb);
//						colorbar_texture[i*4+2] = (GLubyte)GetBValue(rgb);
//						colorbar_texture[i*4+3] = (GLubyte)255;
//					}
//					else
//					{
//						colorbar_texture[i*4+0] = horizbar_texture[i*4+0];
//						colorbar_texture[i*4+1] = horizbar_texture[i*4+1];
//						colorbar_texture[i*4+2] = horizbar_texture[i*4+2];
//						colorbar_texture[i*4+3] = horizbar_texture[i*4+3];
//					}
//				}
//}
//
//
//
//void Renderer::InitTexture()
//{
//	glGenTextures(TEX_COUNT, texture_name);
//
//	// Checkerboard Texture
//	glBindTexture(GL_TEXTURE_2D, texture_name[0]);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_WIDTH, TEX_HEIGHT,
//		0, GL_RGBA, GL_UNSIGNED_BYTE, check_texture);
//
//	if (gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, TEX_WIDTH, TEX_HEIGHT,
//		GL_RGBA, GL_UNSIGNED_BYTE, check_texture) != 0)
//		cerr << "Automatic 2D mipmap construction failed!" << endl;
//
//	// Environment mapping Texture
//	glBindTexture(GL_TEXTURE_2D, texture_name[1]);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_WIDTH, TEX_HEIGHT,
//		0, GL_RGBA, GL_UNSIGNED_BYTE, env_texture);
//
//	if (gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, TEX_WIDTH, TEX_HEIGHT,
//		GL_RGBA, GL_UNSIGNED_BYTE, env_texture) != 0)
//		cerr << "Automatic 2D mipmap construction failed!" << endl;
//
//	// Floor Texture
//	glBindTexture(GL_TEXTURE_2D, texture_name[2]);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_WIDTH, TEX_HEIGHT,
//		0, GL_RGBA, GL_UNSIGNED_BYTE, floor_texture);
//
//	if (gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, TEX_WIDTH, TEX_HEIGHT,
//		GL_RGBA, GL_UNSIGNED_BYTE, floor_texture) != 0)
//		cerr << "Automatic 2D mipmap construction failed!" << endl;
//
//	// Horizontal Bar Texture
//	glBindTexture(GL_TEXTURE_1D, texture_name[3]);
//
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, TEX_WIDTH,
//		0, GL_RGBA, GL_UNSIGNED_BYTE, horizbar_texture);
//
//
//	if (gluBuild1DMipmaps(GL_TEXTURE_1D, GL_RGBA, TEX_WIDTH,
//		GL_RGBA, GL_UNSIGNED_BYTE, horizbar_texture) != 0)
//		cerr << "Automatic 1D mipmap construction failed!" << endl;
//
//
//
//	// Vertical Bar Texture
//	glBindTexture(GL_TEXTURE_1D, texture_name[4]);
//
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, TEX_WIDTH,
//		0, GL_RGBA, GL_UNSIGNED_BYTE, vertbar_texture);
//
//	if (gluBuild1DMipmaps(GL_TEXTURE_1D, GL_RGBA, TEX_WIDTH,
//		GL_RGBA, GL_UNSIGNED_BYTE, vertbar_texture) != 0)
//		cerr << "Automatic 1D mipmap construction failed!" << endl;
//
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//
//
//	// Narrow Horizontal Bar Texture
//	glBindTexture(GL_TEXTURE_1D, texture_name[5]);
//
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, TEX_WIDTH,
//		0, GL_RGBA, GL_UNSIGNED_BYTE, nrwhorizbar_texture);
//
//
//	if (gluBuild1DMipmaps(GL_TEXTURE_1D, GL_RGBA, TEX_WIDTH,
//		GL_RGBA, GL_UNSIGNED_BYTE, nrwhorizbar_texture) != 0)
//		cerr << "Automatic 1D mipmap construction failed!" << endl;
//
//
//	// Narrow Vertical Bar Texture
//	glBindTexture(GL_TEXTURE_1D, texture_name[6]);
//
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, TEX_WIDTH,
//		0, GL_RGBA, GL_UNSIGNED_BYTE, nrwvertbar_texture);
//
//
//	if (gluBuild1DMipmaps(GL_TEXTURE_1D, GL_RGBA, TEX_WIDTH,
//		GL_RGBA, GL_UNSIGNED_BYTE, nrwvertbar_texture) != 0)
//		cerr << "Automatic 1D mipmap construction failed!" << endl;
//
//
//	// Color Bar Texture
//	glBindTexture(GL_TEXTURE_1D, texture_name[7]);
//
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, TEX_HEIGHT,
//		0, GL_RGBA, GL_UNSIGNED_BYTE, colorbar_texture);
//
//
//	if (gluBuild1DMipmaps(GL_TEXTURE_1D, GL_RGBA, TEX_HEIGHT,
//		GL_RGBA, GL_UNSIGNED_BYTE, colorbar_texture) != 0)
//		cerr << "Automatic 1D mipmap construction failed!" << endl;
//
//
//}


//void Renderer::RenderT4FBace(Component* theComponent, float* color)
//{
//	if (!theComponent->pT4mesh) 
//		return;
//
//	if (!theComponent->pT4mesh->size_tetrahedra())
//		return;
//
//	int mat_mode = material_mode;
//	
//	glPolygonMode(GL_FRONT, GL_FILL);
//	glEnable(GL_POLYGON_OFFSET_LINE);
//	glPolygonOffset(1.0, 1.0);
//	glEnable(GL_COLOR_MATERIAL);
//	glColor3fv(color);
//
//	Kernel::T4BFace::face_iterator bf_iter = theComponent->pT4bface->begin();
//	for (; bf_iter!=theComponent->pT4bface->end(); ++bf_iter)
//	{
//		Kernel::Vector3 n0, n1, n2, normal;
//		n0 = theComponent->pT4mesh->node(bf_iter->idx0())->coord()+
//			theComponent->pT4mesh->node(bf_iter->idx0())->m_disp;
//		n1 = theComponent->pT4mesh->node(bf_iter->idx1())->coord()+
//			theComponent->pT4mesh->node(bf_iter->idx1())->m_disp;
//		n2 = theComponent->pT4mesh->node(bf_iter->idx2())->coord()+
//			theComponent->pT4mesh->node(bf_iter->idx2())->m_disp;
//
//		int idx0 = theComponent->pT4mesh->node(bf_iter->idx0())->m_parent_node_index;
//		int idx1 = theComponent->pT4mesh->node(bf_iter->idx1())->m_parent_node_index;
//		int idx2 = theComponent->pT4mesh->node(bf_iter->idx2())->m_parent_node_index;
//
//		if (bAvgNormal)
//		{
//			glBegin(GL_TRIANGLES);
//			glNormal3d(p_KNL->pT4mesh->node(idx0)->m_cur_normal[0],
//				p_KNL->pT4mesh->node(idx0)->m_cur_normal[1],
//				p_KNL->pT4mesh->node(idx0)->m_cur_normal[2]);
//			glVertex3d(n0[0], n0[1], n0[2]);
//
//			glNormal3d(p_KNL->pT4mesh->node(idx1)->m_cur_normal[0],
//				p_KNL->pT4mesh->node(idx1)->m_cur_normal[1],
//				p_KNL->pT4mesh->node(idx1)->m_cur_normal[2]);
//			glVertex3d(n1[0], n1[1], n1[2]);
//
//			glNormal3d(p_KNL->pT4mesh->node(idx2)->m_cur_normal[0],
//				p_KNL->pT4mesh->node(idx2)->m_cur_normal[1],
//				p_KNL->pT4mesh->node(idx2)->m_cur_normal[2]);
//			glVertex3d(n2[0], n2[1], n2[2]);
//			glEnd();
//		}
//
//		else
//		{
//			Kernel::Vector3 p1, p2;
//			p1 = n0 - n1;
//			p2 = n0 - n2;
//			normal = cross(p1, p2);
//			normal = normalize(normal);
//			bf_iter->m_cur_face_normal = normal;
//
//			glBegin(GL_TRIANGLES);
//			glNormal3d(normal[0], normal[1], normal[2]);
//			glVertex3d(n0[0], n0[1], n0[2]);
//			glVertex3d(n1[0], n1[1], n1[2]);
//			glVertex3d(n2[0], n2[1], n2[2]);
//			glEnd();
//		}
//	}
//	
//	if (glIsEnabled(GL_TEXTURE_2D))
//		glDisable(GL_TEXTURE_2D);
//	if (glIsEnabled(GL_TEXTURE_1D))
//		glDisable(GL_TEXTURE_1D);
//
//	glDisable(GL_COLOR_MATERIAL);
//	glDisable(GL_POLYGON_OFFSET_FILL);
//
//}
//
//
//void Renderer::RenderT4FBaceAlpha(Component* theComponent, float* color, float a)
//{
//	if (!theComponent->pT4mesh) 
//		return;
//
//	if (!theComponent->pT4mesh->size_tetrahedra())
//		return;
//
//	int mat_mode = material_mode;
//
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
//
//	glPolygonMode(GL_FRONT, GL_FILL);
//	//glEnable(GL_POLYGON_OFFSET_FILL);
//	//glPolygonOffset(1.0, 1.0);
//
//	glEnable(GL_COLOR_MATERIAL);
//	glColor4f(color[0], color[1], color[2], a);
//
//	glEnable (GL_BLEND);
//	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	Kernel::T4BFace::face_iterator bf_iter = theComponent->pT4bface->begin();
//	for (; bf_iter!=theComponent->pT4bface->end(); ++bf_iter)
//	{
//		Kernel::Vector3 n0, n1, n2, normal;
//		
//		n0 = theComponent->pT4mesh->node(bf_iter->idx0())->coord()+
//			theComponent->pT4mesh->node(bf_iter->idx0())->m_disp;
//		n1 = theComponent->pT4mesh->node(bf_iter->idx1())->coord()+
//			theComponent->pT4mesh->node(bf_iter->idx1())->m_disp;
//		n2 = theComponent->pT4mesh->node(bf_iter->idx2())->coord()+
//			theComponent->pT4mesh->node(bf_iter->idx2())->m_disp;
//
//		Kernel::Vector3 p1, p2;
//		p1 = n0 - n1;
//		p2 = n0 - n2;
//		normal = cross(p1, p2);
//		normal = normalize(normal);
//		bf_iter->m_cur_face_normal = normal;
//	
//		glBegin(GL_TRIANGLES);
//		glNormal3d(normal[0], normal[1], normal[2]);
//		glVertex3d(n0[0], n0[1], n0[2]);
//		glVertex3d(n1[0], n1[1], n1[2]);
//		glVertex3d(n2[0], n2[1], n2[2]);
//		glEnd();
//
//	}
//
//	if (glIsEnabled(GL_TEXTURE_2D))
//		glDisable(GL_TEXTURE_2D);
//	if (glIsEnabled(GL_TEXTURE_1D))
//		glDisable(GL_TEXTURE_1D);
//
//	glDisable(GL_COLOR_MATERIAL);
//	glDisable(GL_BLEND);
//
//}
//
//void Renderer::RenderInterface()
//{
//	if (p_KNL->Interfaces.empty())
//		return;
//
//	int mat_mode = material_mode;
//
//	glDisable(GL_LIGHTING);
//	glDisable(GL_LIGHT0);
//
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	glEnable(GL_POLYGON_OFFSET_FILL);
//	glPolygonOffset(1.0, 1.0);
//
//	glEnable(GL_COLOR_MATERIAL);
//	
//	for (int i=0; i<p_KNL->Interfaces.size(); i++)
//	{
//		if (i == currentInterface)
//		{
//			glColor3f(1.0, 0.0, 0.0);
//		}
//		else
//		{
//			glColor3f(1.0, 0.5, 0.0);
//		}
//
//		Kernel::Vector3 n0, n1, n2, normal;
//
//		for (int j=0; j<p_KNL->Interfaces[i]->numFaces; j++)
//		{
//			int idx0 = p_KNL->Interfaces[i]->gFaces[j*3+0];
//			int idx1 = p_KNL->Interfaces[i]->gFaces[j*3+1];
//			int idx2 = p_KNL->Interfaces[i]->gFaces[j*3+2];
//			
//			n0 = p_KNL->pT4mesh->node(idx0)->coord() + p_KNL->pT4mesh->node(idx0)->m_disp;
//			n1 = p_KNL->pT4mesh->node(idx1)->coord() + p_KNL->pT4mesh->node(idx1)->m_disp;
//			n2 = p_KNL->pT4mesh->node(idx2)->coord() + p_KNL->pT4mesh->node(idx2)->m_disp;
//
//			Kernel::Vector3 p1, p2;
//			p1 = n0 - n1;
//			p2 = n0 - n2;
//			normal = cross(p1, p2);
//			normal = normalize(normal);
//
//			// draw double face
//			glBegin(GL_TRIANGLES);
//
//			glNormal3d(normal[0], normal[1], normal[2]);
//			glVertex3d(n0[0], n0[1], n0[2]);
//			glVertex3d(n1[0], n1[1], n1[2]);
//			glVertex3d(n2[0], n2[1], n2[2]);
//
//			glNormal3d(normal[0], normal[1], normal[2]);
//			glVertex3d(n0[0], n0[1], n0[2]);
//			glVertex3d(n2[0], n2[1], n2[2]);
//			glVertex3d(n1[0], n1[1], n1[2]);
//			
//			glEnd();
//
//		}
//	}
//
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	
//	glColor3f(1.0, 1.0, 1.0);
//	glEnable(GL_COLOR_MATERIAL);
//
//	for (int i=0; i<p_KNL->Interfaces.size(); i++)
//	{
//		Kernel::Vector3 n0, n1, n2, normal;
//		for (int j=0; j<p_KNL->Interfaces[i]->numFaces; j++)
//		{
//			int idx0 = p_KNL->Interfaces[i]->gFaces[j*3+0];
//			int idx1 = p_KNL->Interfaces[i]->gFaces[j*3+1];
//			int idx2 = p_KNL->Interfaces[i]->gFaces[j*3+2];
//
//			n0 = p_KNL->pT4mesh->node(idx0)->coord() + p_KNL->pT4mesh->node(idx0)->m_disp;
//			n1 = p_KNL->pT4mesh->node(idx1)->coord() + p_KNL->pT4mesh->node(idx1)->m_disp;
//			n2 = p_KNL->pT4mesh->node(idx2)->coord() + p_KNL->pT4mesh->node(idx2)->m_disp;
//
//			Kernel::Vector3 p1, p2;
//			p1 = n0 - n1;
//			p2 = n0 - n2;
//			normal = cross(p1, p2);
//			normal = normalize(normal);
//
//			glBegin(GL_TRIANGLES);
//
//			glNormal3d(normal[0], normal[1], normal[2]);
//
//			glVertex3d(n0[0], n0[1], n0[2]);
//			glVertex3d(n1[0], n1[1], n1[2]);
//			glVertex3d(n2[0], n2[1], n2[2]);
//
//			glVertex3d(n0[0], n0[1], n0[2]);
//			glVertex3d(n2[0], n2[1], n2[2]);
//			glVertex3d(n1[0], n1[1], n1[2]);
//			
//			glEnd();
//
//		}
//
//	}
//
//	if (glIsEnabled(GL_TEXTURE_2D))
//		glDisable(GL_TEXTURE_2D);
//	if (glIsEnabled(GL_TEXTURE_1D))
//		glDisable(GL_TEXTURE_1D);
//
//	glDisable(GL_COLOR_MATERIAL);
//	//glDisable(GL_POLYGON_OFFSET_FILL);
//
//}
//
//void Renderer::RenderT4FBaceLine(Component* theComponent, float* color)
//{
//	if (!theComponent->pT4mesh) 
//		return;
//
//	if (!theComponent->pT4mesh->size_tetrahedra())
//		return;
//
//	int mat_mode = material_mode;
//
//	glPolygonMode(GL_FRONT, GL_LINE);
//	glEnable(GL_COLOR_MATERIAL);
//	glColor3fv(color);
//	Kernel::T4BFace::face_iterator bf_iter = theComponent->pT4bface->begin();
//	for (; bf_iter!=theComponent->pT4bface->end(); ++bf_iter)
//	{
//		Kernel::Vector3 n0, n1, n2;
//		
//		n0 = theComponent->pT4mesh->node(bf_iter->idx0())->coord()+
//			theComponent->pT4mesh->node(bf_iter->idx0())->m_disp;
//		n1 = theComponent->pT4mesh->node(bf_iter->idx1())->coord()+
//			theComponent->pT4mesh->node(bf_iter->idx1())->m_disp;
//		n2 = theComponent->pT4mesh->node(bf_iter->idx2())->coord()+
//			theComponent->pT4mesh->node(bf_iter->idx2())->m_disp;
//				
//		glBegin(GL_TRIANGLES);
//		glVertex3d(n0[0], n0[1], n0[2]);
//		glVertex3d(n1[0], n1[1], n1[2]);
//		glVertex3d(n2[0], n2[1], n2[2]);
//		glEnd();
//	}
//
//	if (glIsEnabled(GL_TEXTURE_2D))
//		glDisable(GL_TEXTURE_2D);
//	if (glIsEnabled(GL_TEXTURE_1D))
//		glDisable(GL_TEXTURE_1D);
//
//	glDisable(GL_COLOR_MATERIAL);
//	glPolygonMode(GL_FRONT, GL_FILL);
//	SetMaterial(mat_mode);
//
//}
//void Renderer::RenderT4FBace()
//{
//	if (!p_KNL->pT4mesh->size_nodes())
//	{
//		return;
//	}
//
//	glPolygonMode(GL_FRONT, GL_FILL);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
//	
//	Kernel::T4BFace::face_iterator bf_iter = p_KNL->pT4bface->begin();
//	
//	bf_iter = p_KNL->pT4bface->begin();
//	for (; bf_iter!=p_KNL->pT4bface->end(); ++bf_iter)
//	{
//		Kernel::Vector3 n0, n1, n2, normal;
//		n0 = p_KNL->pT4mesh->node(bf_iter->idx0())->coord()+
//			p_KNL->pT4mesh->node(bf_iter->idx0())->m_disp;
//		n1 = p_KNL->pT4mesh->node(bf_iter->idx1())->coord()+
//			p_KNL->pT4mesh->node(bf_iter->idx1())->m_disp;
//		n2 = p_KNL->pT4mesh->node(bf_iter->idx2())->coord()+
//			p_KNL->pT4mesh->node(bf_iter->idx2())->m_disp;
//
//		if (bAvgNormal)
//		{
//			glBegin(GL_TRIANGLES);
//			glNormal3d(p_KNL->pT4mesh->node(bf_iter->idx0())->m_cur_normal[0],
//			p_KNL->pT4mesh->node(bf_iter->idx0())->m_cur_normal[1],
//			p_KNL->pT4mesh->node(bf_iter->idx0())->m_cur_normal[2]);
//			glVertex3d(n0[0], n0[1], n0[2]);
//
//			glNormal3d(p_KNL->pT4mesh->node(bf_iter->idx1())->m_cur_normal[0],
//				p_KNL->pT4mesh->node(bf_iter->idx1())->m_cur_normal[1],
//				p_KNL->pT4mesh->node(bf_iter->idx1())->m_cur_normal[2]);
//			glVertex3d(n1[0], n1[1], n1[2]);
//
//			glNormal3d(p_KNL->pT4mesh->node(bf_iter->idx2())->m_cur_normal[0],
//				p_KNL->pT4mesh->node(bf_iter->idx2())->m_cur_normal[1],
//				p_KNL->pT4mesh->node(bf_iter->idx2())->m_cur_normal[2]);
//			glVertex3d(n2[0], n2[1], n2[2]);
//			glEnd();
//		}
//
//		else
//		{
//			Kernel::Vector3 p1, p2;
//			p1 = n0 - n1;
//			p2 = n0 - n2;
//			normal = cross(p1, p2);
//			normal = normalize(normal);
//			bf_iter->m_cur_face_normal = normal;
//			
//			glBegin(GL_TRIANGLES);
//			glNormal3d(normal[0], normal[1], normal[2]);
//			glVertex3d(n0[0], n0[1], n0[2]);
//			glVertex3d(n1[0], n1[1], n1[2]);
//			glVertex3d(n2[0], n2[1], n2[2]);
//			glEnd();
//		}
//	}
//	
//	if (glIsEnabled(GL_TEXTURE_2D))
//		glDisable(GL_TEXTURE_2D);
//	if (glIsEnabled(GL_TEXTURE_1D))
//		glDisable(GL_TEXTURE_1D);
//}
//
//void Renderer::RenderT4FBace(float* color)
//{
//	int mat_mode = material_mode;
//
//	if (!p_KNL->pT4mesh->size_nodes())
//	{
//		return;
//	}
//
//	glPolygonMode(GL_FRONT, GL_FILL);
//	glEnable(GL_POLYGON_OFFSET_FILL);
//	glPolygonOffset(1.0, 1.0);
//
//	//int num_vertices = pSolid->numVertices();
//	glEnable(GL_COLOR_MATERIAL);
//	glColor3fv(color);
//	Kernel::T4BFace::face_iterator bf_iter = p_KNL->pT4bface->begin();
//	for (; bf_iter!=p_KNL->pT4bface->end(); ++bf_iter)
//	{
//		Kernel::Vector3 n0, n1, n2, normal;
//		n0 = p_KNL->pT4mesh->node(bf_iter->idx0())->coord()+
//			 p_KNL->pT4mesh->node(bf_iter->idx0())->m_disp;
//		n1 = p_KNL->pT4mesh->node(bf_iter->idx1())->coord()+
//			 p_KNL->pT4mesh->node(bf_iter->idx1())->m_disp;
//		n2 = p_KNL->pT4mesh->node(bf_iter->idx2())->coord()+
//			 p_KNL->pT4mesh->node(bf_iter->idx2())->m_disp;
//		normal = bf_iter->m_cur_face_normal;
//
//		glBegin(GL_TRIANGLES);
//		glNormal3d(normal[0], normal[1], normal[2]);
//		glVertex3d(n0[0], n0[1], n0[2]);
//		glVertex3d(n1[0], n1[1], n1[2]);
//		glVertex3d(n2[0], n2[1], n2[2]);
//		glEnd();
//
//	}
//
//	if (glIsEnabled(GL_TEXTURE_2D))
//		glDisable(GL_TEXTURE_2D);
//	if (glIsEnabled(GL_TEXTURE_1D))
//		glDisable(GL_TEXTURE_1D);
//
//	glDisable(GL_COLOR_MATERIAL);
//	SetMaterial(material_mode);
//}



void Renderer::mousePressEvent(QMouseEvent *e)
{
	current_mouse_x = e->x();
	current_mouse_y = e->y();
	current_mouse_button = e->button();
	
	mousei2d(current_mouse_x, current_mouse_y, &current_mouse_xd, &current_mouse_yd);
		
	if (current_mouse_button == Qt::LeftButton)
	{
		if (flag_show_selection && last_mouse_button == Qt::RightButton && current_render_mode == REGULAR)
		{
			char msg[1024];
			int cx = 0;
			for (int i = 0; i < p_kernel->level_list.size(); i++)
			{
				vector<Node>::iterator n_iter = p_kernel->level_list[i]->voxmesh_level->node_list.begin();

				for (; n_iter!=p_kernel->level_list[i]->voxmesh_level->node_list.end(); ++n_iter)
				{
					double wx, wy, wz;
					Vector3d p = n_iter->coordinate + n_iter->displacement;

					gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

					if (isSelected(wx, wy))
					{
						if (!n_iter->flag_anchor_node)
						{
							n_iter->flag_anchor_node = true;
							p_kernel->level_list[i]->voxmesh_level->anchor_node_list.push_back(&*n_iter);
						}
					}
				}
				cx += sprintf(msg+cx, "Level %d:  %d anchor nodes have been chosen\n", i, p_kernel->level_list[i]->voxmesh_level->anchor_node_list.size());
			}
			QMessageBox::information(NULL, tr("success"), tr(msg));
		}
		if (flag_show_constraints && last_mouse_button == Qt::RightButton && current_render_mode == REGULAR)
		{
			char msg[1024];
			int cx = 0;				
			Vector3d sum;
			sum.setZero();
			int l = p_kernel->level_list.size() - 1;
			int i = 0;
			for (i = 0; i < p_kernel->level_list.size(); i++)
			{
				vector<Node>::iterator n_iter = p_kernel->level_list[i]->voxmesh_level->node_list.begin();


				for (; n_iter!=p_kernel->level_list[i]->voxmesh_level->node_list.end(); ++n_iter)
				{
					double wx, wy, wz;
					Vector3d p = n_iter->coordinate + n_iter->displacement;

					
					gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

					if (isSelected(wx, wy))
					{
						/*
						if (p_kernel->used_simulator == Kernel::HSM_FORCE4STEP)
						{
							if(i > 0 && n_iter->parent_node != NULL)
							{
								if (!n_iter->flag_constraint_node && n_iter->parent_node->flag_constraint_node)
								{
									n_iter->flag_constraint_node = true;
									p_kernel->level_list[i]->voxmesh_level->constraint_node_list.push_back(&*n_iter);

									for(int j=0; j < n_iter->incident_cluster.size(); ++j)
									{
										n_iter->incident_cluster[j]->flag_constrained = true;
										n_iter->incident_cluster[j]->constraint_node = NULL;
									}

									for (int k=0; k < n_iter->duplicates.size(); ++k)
									{
										n_iter->duplicates[k]->flag_constraint_node = true;
										n_iter->prescribed_position = n_iter->target_position;
										n_iter->prescribed_preposition = n_iter->target_position;
									}
								}
							}
							else if(i == 0)
							{
								if (!n_iter->flag_constraint_node )
								{
									n_iter->flag_constraint_node = true;
									p_kernel->level_list[i]->voxmesh_level->constraint_node_list.push_back(&*n_iter);

									for(int j=0; j < n_iter->incident_cluster.size(); ++j)
									{
										n_iter->incident_cluster[j]->flag_constrained = true;
										n_iter->incident_cluster[j]->constraint_node = NULL;
									}

									for (int k=0; k < n_iter->duplicates.size(); ++k)
									{
										n_iter->duplicates[k]->flag_constraint_node = true;
										n_iter->prescribed_position = n_iter->target_position;
										n_iter->prescribed_preposition = n_iter->target_position;
									}
								}
							}
						}
						else{
						*/
							if (!n_iter->flag_constraint_node)
							{
								n_iter->flag_constraint_node = true;
								if(p_kernel->idx_constraint == 1)
									p_kernel->level_list[i]->voxmesh_level->constraint_node_list.push_back(&*n_iter);
								else
									p_kernel->level_list[i]->voxmesh_level->another_constraint_node_list.push_back(&*n_iter);

								for(int j=0; j < n_iter->incident_cluster.size(); ++j)
								{
									n_iter->incident_cluster[j]->flag_constrained = true;
									n_iter->incident_cluster[j]->constraint_node = NULL;
									p_kernel->level_list[i]->voxmesh_level->constraint_cluster_list.push_back(n_iter->incident_cluster[j]);
								}

								for (int k=0; k < n_iter->duplicates.size(); ++k)
								{
									n_iter->duplicates[k]->flag_constraint_node = true;
									n_iter->prescribed_position = n_iter->target_position;
									n_iter->prescribed_preposition = n_iter->target_position;
								}
							}
						/*}*/

					}
				}

	
				
			}
			for(i = 0; i < p_kernel->level_list.size(); i ++)
			{
				if(p_kernel->idx_constraint == 1)
				{
					int size_k = p_kernel->level_list[l]->voxmesh_level->constraint_node_list.size();
					Vector3d sum = Vector3d::Zero();
					for(int k = 0; k < size_k; k ++)
					{
						sum += p_kernel->level_list[l]->voxmesh_level->constraint_node_list[k]->prescribed_position;
					}
					p_kernel->level_list[i]->voxmesh_level->constraint_center = sum / size_k;
				}
				else
				{
					int size_k = p_kernel->level_list[l]->voxmesh_level->another_constraint_node_list.size();
					Vector3d sum = Vector3d::Zero();
					for(int k = 0; k < size_k; k ++)
					{
						sum += p_kernel->level_list[l]->voxmesh_level->another_constraint_node_list[k]->prescribed_position;
					}
					p_kernel->level_list[i]->voxmesh_level->another_constraint_center = sum / size_k;
				}
				//cout << i << "constraint center: ";
				//p_kernel->printVector3d(p_kernel->level_list[i]->voxmesh_level->constraint_center);
				if(p_kernel->idx_constraint == 1)
				{
					p_kernel->level_list[i]->voxmesh_level->constraint_displacement.clear();
					for(int j = 0; j < p_kernel->level_list[i]->voxmesh_level->constraint_node_list.size(); j++)
					{
						Vector3d displacement = p_kernel->level_list[i]->voxmesh_level->constraint_node_list[j]->coordinate 
							+ p_kernel->level_list[i]->voxmesh_level->constraint_node_list[j]->displacement - p_kernel->level_list[i]->voxmesh_level->constraint_center;
						p_kernel->level_list[i]->voxmesh_level->constraint_displacement.push_back(displacement);
					}
					cx += sprintf(msg+cx, "Level %d:  %d constraint nodes have been chosen\n", i, p_kernel->level_list[i]->voxmesh_level->constraint_node_list.size());
				}
				else
				{
					p_kernel->level_list[i]->voxmesh_level->another_constraint_displacement.clear();
					for(int j = 0; j < p_kernel->level_list[i]->voxmesh_level->another_constraint_node_list.size(); j++)
					{
						Vector3d displacement = p_kernel->level_list[i]->voxmesh_level->another_constraint_node_list[j]->coordinate 
							+ p_kernel->level_list[i]->voxmesh_level->another_constraint_node_list[j]->displacement - p_kernel->level_list[i]->voxmesh_level->another_constraint_center;
						p_kernel->level_list[i]->voxmesh_level->another_constraint_displacement.push_back(displacement);
					}
					cx += sprintf(msg+cx, "Level %d:  %d constraint nodes have been chosen\n", i, p_kernel->level_list[i]->voxmesh_level->another_constraint_node_list.size());
				}
			}
			if(p_kernel->idx_constraint == 1)
				p_kernel->constraint_first = p_kernel->level_list[l]->voxmesh_level->constraint_center;
			else
				p_kernel->constraint_second = p_kernel->level_list[l]->voxmesh_level->another_constraint_center;
			QMessageBox::information(NULL, tr("success"), tr(msg));
		}
		if (flag_show_cube_static_constraints && last_mouse_button == Qt::RightButton && current_render_mode == REGULAR)
		{
			char msg[1024];
			int cx = 0;		

			for (int i = 0; i < p_kernel->level_list.size(); i++)
			{
				vector<Cluster>::iterator c_iter = p_kernel->level_list[i]->voxmesh_level->cluster_list.begin();


				for (; c_iter!=p_kernel->level_list[i]->voxmesh_level->cluster_list.end(); ++c_iter)
				{
					if (i == 0)
					{
						double wx, wy, wz;
						Vector3d p = c_iter->current_center;

						gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

						if (isSelected(wx, wy))
						{
							if (!c_iter->flag_cube_anchored)
							{
								c_iter->flag_cube_anchored = true;
								p_kernel->level_list[i]->voxmesh_level->static_cube_list.push_back(&(*c_iter));
								vector<DuplicatedNode>::iterator d_iter = c_iter->node_list.begin();
								for (; d_iter != c_iter->node_list.end(); ++ d_iter)
								{
									d_iter->mapped_node->flag_anchor_node = true;
								}
							}
						}
					}
					else
					{
						if (c_iter->parent_cluster->flag_cube_anchored)
						{
							c_iter->flag_cube_anchored = true;
							p_kernel->level_list[i]->voxmesh_level->static_cube_list.push_back(&(*c_iter));
							vector<DuplicatedNode>::iterator d_iter = c_iter->node_list.begin();
							for (; d_iter != c_iter->node_list.end(); ++ d_iter)
							{
								d_iter->mapped_node->flag_anchor_node = true;
							}
						}
					}
				}
				cx += sprintf(msg+cx, "Level %d:  %d cube clusters have been chosen\n", i, p_kernel->level_list[i]->voxmesh_level->static_cube_list.size());
			}

			QMessageBox::information(NULL, tr("success"), tr(msg));
		}
		if (flag_show_cube_active_constraints && last_mouse_button == Qt::RightButton && current_render_mode == REGULAR)
		{
			char msg[1024];
			int cx = 0;		

			for (int i = 0; i < p_kernel->level_list.size(); i++)
			{
				vector<Cluster>::iterator c_iter = p_kernel->level_list[i]->voxmesh_level->cluster_list.begin();


				for (; c_iter!=p_kernel->level_list[i]->voxmesh_level->cluster_list.end(); ++c_iter)
				{
					if(i == 0)
					{
						double wx, wy, wz;
						Vector3d p = c_iter->current_center;


						gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

						if (isSelected(wx, wy))
						{
							if (!c_iter->flag_cube_constrained)
							{
								c_iter->flag_cube_constrained = true;
								p_kernel->level_list[i]->voxmesh_level->active_cube_list.push_back(&(*c_iter));
								vector<DuplicatedNode>::iterator d_iter = c_iter->node_list.begin();
								for (; d_iter != c_iter->node_list.end(); ++ d_iter)
								{
									d_iter->mapped_node->flag_constraint_node = true;
								}
							}
						}

					}
					else
					{
						if (c_iter->parent_cluster->flag_cube_constrained)
						{
							c_iter->flag_cube_constrained = true;
							p_kernel->level_list[i]->voxmesh_level->active_cube_list.push_back(&(*c_iter));
							vector<DuplicatedNode>::iterator d_iter = c_iter->node_list.begin();
							for (; d_iter != c_iter->node_list.end(); ++ d_iter)
							{
								d_iter->mapped_node->flag_constraint_node = true;
							}
						}
					}
				}
				cx += sprintf(msg+cx, "Level %d:  %d cube clusters have been chosen\n", i, p_kernel->level_list[i]->voxmesh_level->active_cube_list.size());
			}

			QMessageBox::information(NULL, tr("success"), tr(msg));
		}

		/*
		if (current_render_mode == TREE_SETTING && last_mouse_button == Qt::RightButton)
		{
			 vector <Vox*> selected_vox_list;
			 selected_vox_list.reserve(p_kernel->p_vox_mesh->current_parent_cluster->num_vox);

			 vector<Vox*>::iterator v_iter = p_kernel->p_vox_mesh->current_parent_cluster->vox_list.begin();
			 for (; v_iter != p_kernel->p_vox_mesh->current_parent_cluster->vox_list.end(); ++v_iter)
			 {
				 if ((*v_iter)->flag_visited) // if the voxel is already selected
				 {
					 continue;
				 }

				 double wx, wy, wz;
				 Vector3d p = (*v_iter)->vox_center;

				 gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

				 if (isSelected(wx, wy))
				 {
					 selected_vox_list.push_back(*v_iter);
				 }
			 }

			 if (!selected_vox_list.empty())
			 {
				  p_kernel->p_vox_mesh->appendMaxConnectedComponent(selected_vox_list);
				  emit appendCluster();
			 }
		}
		*/
		bottom_right_x = -1.0;
		bottom_right_y = -1.0;
		upper_left_x   = -1.0;
		upper_left_y   = -1.0;
	}

	if (current_mouse_button == Qt::RightButton)
	{
		if (flag_show_selection || flag_show_constraints || flag_show_mass || flag_show_cube_static_constraints || flag_show_cube_active_constraints)
		{
			bottom_right_x = -1.0;
			bottom_right_y = -1.0;
			upper_left_x = current_mouse_x;
			upper_left_y = current_mouse_y;
		}

		/*
		//rope dragging
		if (flag_rope_dragging)
		{
			if (!p_kernel->p_rope->activeJoint != -1)
			{
				Vector3d mouse_pos_before = p_kernel->p_rope->joint_list[p_kernel->p_rope->activeJoint]._x;

				double wx, wy, wz;

				gluProject(mouse_pos_before[0], mouse_pos_before[1], mouse_pos_before[2], 
					currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

				wx = current_mouse_x;
				wy = win_height - current_mouse_y;

				Vector3d mouse_pos;

				gluUnProject(wx, wy, wz, currentmodelview, currentprojection, currentviewport, 
					&mouse_pos[0], &mouse_pos[1], &mouse_pos[2]);

				p_kernel->p_rope->joint_list[p_kernel->p_rope->activeJoint]._x = mouse_pos;
			}
		}
		*/
	}
		
	last_mouse_x = current_mouse_x;
	last_mouse_y = current_mouse_y;
	last_mouse_xd = current_mouse_xd;
	last_mouse_yd = current_mouse_yd;
	last_mouse_button = current_mouse_button;

	updateGL();
}

void Renderer::mouseDoubleClickEvent(QMouseEvent *e)
{
	current_mouse_button = e->button();

	current_mouse_x = e->x();
	current_mouse_y = e->y();

	// truncate if over win boundary
	current_mouse_x = current_mouse_x > int(win_width) ? int(win_width) : current_mouse_x;
	current_mouse_x = current_mouse_x < 1 ? 1 : current_mouse_x;

	current_mouse_y = current_mouse_y > int(win_height) ? int(win_height) : current_mouse_y;
	current_mouse_y = current_mouse_y < 1 ? 1 : current_mouse_y;	
	
	if (current_mouse_button == Qt::LeftButton)
	{
		if (flag_show_selection)
		{
			///////////////////////////needs to be revised for all levels
			//vector<Node*>::iterator ni = p_kernel->p_vox_mesh->anchor_node_list.begin();
			//for (; ni!=p_kernel->p_vox_mesh->anchor_node_list.end(); ++ni)
			//{
			//	(*ni)->flag_anchor_node = false;
			//}
			//p_kernel->p_vox_mesh->anchor_node_list.clear();

			// clear anchor nodes for all level
			for (int i = 0; i < p_kernel->level_list.size(); i++)
			{
				vector<Node *>::iterator n_iter = p_kernel->level_list[i]->voxmesh_level->anchor_node_list.begin();

				for (; n_iter!=p_kernel->level_list[i]->voxmesh_level->anchor_node_list.end(); ++n_iter)
				{
					(*n_iter)->flag_anchor_node = false;
				}
				p_kernel->level_list[i]->voxmesh_level->anchor_node_list.clear();
				/*
				//all targets set to be rest shape
				vector<Cluster>::iterator ci;
				for (ci = p_kernel->level_list[i]->voxmesh_level->cluster_list.begin(); ci!=p_kernel->level_list[i]->voxmesh_level->cluster_list.end(); ++ci)
				{
					for (int j=0; j<ci->num_node; ++j)
					{
						ci->node_list[j].target_position = ci->node_list[j].mapped_node->coordinate;
					}
				}
				*/
			}

			QMessageBox::information(NULL, "success", "all anchor nodes are removed");
		}
		else if (flag_show_cube_static_constraints)
		{

			// clear anchor nodes for all level
			for (int i = 0; i < p_kernel->level_list.size(); i++)
			{
				vector<Cluster *>::iterator c_iter = p_kernel->level_list[i]->voxmesh_level->static_cube_list.begin();
				for (; c_iter!=p_kernel->level_list[i]->voxmesh_level->static_cube_list.end(); ++c_iter)
				{
					(*c_iter)->flag_cube_anchored = false;
					vector<DuplicatedNode>::iterator d_iter = (*c_iter)->node_list.begin();
					for (; d_iter != (*c_iter)->node_list.end(); ++d_iter)
					{
						d_iter->mapped_node->flag_anchor_node = false;
					}
				}
				p_kernel->level_list[i]->voxmesh_level->static_cube_list.clear();
			}

			QMessageBox::information(NULL, "success", "all anchor cubes are removed");
		}
		else if (flag_show_constraints)
		{
			// clear constraint nodes for all level
			for (int i = 0; i < p_kernel->level_list.size(); i++)
			{
				vector<Node *>::iterator n_iter;
				
				n_iter = p_kernel->level_list[i]->voxmesh_level->constraint_node_list.begin();
				for (; n_iter!=p_kernel->level_list[i]->voxmesh_level->constraint_node_list.end(); ++n_iter)
				{
					(*n_iter)->flag_constraint_node = false;
				}
				n_iter = p_kernel->level_list[i]->voxmesh_level->another_constraint_node_list.begin();
				for (; n_iter!=p_kernel->level_list[i]->voxmesh_level->another_constraint_node_list.end(); ++n_iter)
				{
					(*n_iter)->flag_constraint_node = false;
				}
				p_kernel->level_list[i]->voxmesh_level->constraint_node_list.clear();
				p_kernel->level_list[i]->voxmesh_level->another_constraint_node_list.clear();
				p_kernel->level_list[i]->voxmesh_level->constraint_cluster_list.clear();
				p_kernel->level_list[i]->voxmesh_level->constraint_center.setZero();
				p_kernel->level_list[i]->voxmesh_level->another_constraint_center.setZero();

				
				//all targets set to be rest shape
				vector<Cluster>::iterator ci;
				for (ci = p_kernel->level_list[i]->voxmesh_level->cluster_list.begin(); ci!=p_kernel->level_list[i]->voxmesh_level->cluster_list.end(); ++ci)
				{
					for (int j=0; j<ci->num_node; ++j)
					{
						ci->node_list[j].target_position = ci->node_list[j].mapped_node->coordinate;
						if(ci->flag_constrained)
						{
							ci->flag_constrained = false;
							ci->constraint_node_list.clear();	
						}
						if(ci->node_list[j].flag_constraint_node)
						{
							ci->node_list[j].flag_constraint_node = false;
						}
					}
				}
				
			}
			LCS_x.setZero();
			LCS_y.setZero();
			LCS_z.setZero();
			QMessageBox::information(NULL, "success", "all constraint nodes are removed");
		}
		else if (flag_show_cube_active_constraints)
		{
			// clear anchor nodes for all level
			for (int i = 0; i < p_kernel->level_list.size(); i++)
			{
				vector<Cluster *>::iterator c_iter = p_kernel->level_list[i]->voxmesh_level->active_cube_list.begin();
				for (; c_iter!=p_kernel->level_list[i]->voxmesh_level->active_cube_list.end(); ++c_iter)
				{
					(*c_iter)->flag_cube_constrained = false;
					vector<DuplicatedNode>::iterator d_iter = (*c_iter)->node_list.begin();
					for (; d_iter != (*c_iter)->node_list.end(); ++d_iter)
					{
						d_iter->mapped_node->flag_constraint_node = false;
					}
				}
				p_kernel->level_list[i]->voxmesh_level->active_cube_list.clear();
			}
			QMessageBox::information(NULL, "success", "all constraint cubes are removed");
		}
		else if (flag_cube_operation)
		{
			double x, y, z;
			Vector3d p, q, l, m;
			x = current_mouse_x;
			y = win_height - current_mouse_y;
			z = 0;
			gluUnProject(x, y, z, currentmodelview, currentprojection, currentviewport, 
				&p[0], &p[1], &p[2]);
			z = 1;
			gluUnProject(x, y, z, currentmodelview, currentprojection, currentviewport, 
				&q[0], &q[1], &q[2]);
			l = q - p;
			vector<Face>::iterator fi = p_kernel->p_mesh->face_list.begin();
			for (; fi!=p_kernel->p_mesh->face_list.end(); ++fi)
			{
				Vector3d n0, n1, n2, n01, n02, n03, n;

				n0 = fi->node0->coordinate + fi->node0->displacement;
				n1 = fi->node1->coordinate + fi->node1->displacement;
				n2 = fi->node2->coordinate + fi->node2->displacement;

				n = (-1)*fi->normal;
				float nDotL = n.dot(l);

				if (nDotL <= 0.0)
					continue;

				float d = n.dot(n0 - p) / nDotL;

				if (d < 0.0f || d > 1.0f) // plane is beyond the ray we consider
					continue;

				m = p + d * l;

				n01 = (n0 - n1).cross(m - n0);
				n02 = (n1 - n2).cross(m - n1);
				n03 = (n2 - n0).cross(m - n2);
				/*
				if (n.dot(n01) >= 0.0f &&
					n.dot(n02) >= 0.0f &&
					n.dot(n03) >= 0.0f)
				{
					tool->setVisible(true);
					tool->setPosBegin(m[0], m[1], m[2]);
					tool->setPosEnd(m[0]+fi->normal[0], m[1]+fi->normal[1], m[2]+fi->normal[2]);
					//break; // to break loop for nodes
				}
				*/

				//else
					//tool->setVisible(false);
				/*
				if (isPicked4Cube(x, y))
				{
					tool->setVisible(true);
					tool->setPosBegin(p[0], p[1], p[2]);
					tool->setPosEnd(p[0]+fi->normal[0], p[1]+fi->normal[1], p[2]+fi->normal[2]);
					break; // to break loop for nodes
				}
				else
					tool->setVisible(false);
					*/
			}
		}
		else if (flag_left_surface || flag_right_surface)
		{
			double x, y, z;
			Vector3d p, q, l, m;
			x = current_mouse_x;
			y = win_height - current_mouse_y;
			z = 0;
			gluUnProject(x, y, z, currentmodelview, currentprojection, currentviewport, 
				&p[0], &p[1], &p[2]);
			z = 1;
			gluUnProject(x, y, z, currentmodelview, currentprojection, currentviewport, 
				&q[0], &q[1], &q[2]);
			l = q - p;
			vector<Face>::iterator fi = p_kernel->p_mesh->face_list.begin();
			int count = 0;
			for (; fi!=p_kernel->p_mesh->face_list.end(); ++fi)
			{
				Vector3d n0, n1, n2, n01, n02, n03, n;

				n0 = fi->node0->coordinate + fi->node0->displacement;
				n1 = fi->node1->coordinate + fi->node1->displacement;
				n2 = fi->node2->coordinate + fi->node2->displacement;

				n = (-1)*fi->normal;
				float nDotL = n.dot(l);

				if (nDotL <= 0.0)
					continue;

				float d = n.dot(n0 - p) / nDotL;

				if (d < 0.0f || d > 1.0f) // plane is beyond the ray we consider
					continue;

				m = p + d * l;

				n01 = (n0 - n1).cross(m - n0);
				n02 = (n1 - n2).cross(m - n1);
				n03 = (n2 - n0).cross(m - n2);

				if (n.dot(n01) >= 0.0f &&
					n.dot(n02) >= 0.0f &&
					n.dot(n03) >= 0.0f)
				{
					if (flag_left_surface)
					{
						p_kernel->para[0] = ((n1[1]-n2[1])*(m[0]-n2[0]) + (n2[0]-n1[0])*(m[1]-n2[1]))/((n1[1]-n2[1])*(n0[0]-n2[0]) + (n2[0]-n1[0])*(n0[1]-n2[1]));
						p_kernel->para[1] = ((n2[1]-n0[1])*(m[0]-n2[0]) + (n0[0]-n2[0])*(m[1]-n2[1]))/((n1[1]-n2[1])*(n0[0]-n2[0]) + (n2[0]-n1[0])*(n0[1]-n2[1]));
						p_kernel->para[2] = 1 - p_kernel->para[0] - p_kernel->para[1];
						p_kernel->paraNode[0] = fi->node0;
						p_kernel->paraNode[1] = fi->node1;
						p_kernel->paraNode[2] = fi->node2;
						p_kernel->surface_point_left[0] = m[0];
						p_kernel->surface_point_left[1] = m[1];
						p_kernel->surface_point_left[2] = m[2];
						
						//cout << m << endl;
						//cout << n0 * p_kernel->para[0] + n1*p_kernel->para[1] + n2*p_kernel->para[2] << endl;
					}
					else
					{
						p_kernel->para2[0] = ((n1[1]-n2[1])*(m[0]-n2[0]) + (n2[0]-n1[0])*(m[1]-n2[1]))/((n1[1]-n2[1])*(n0[0]-n2[0]) + (n2[0]-n1[0])*(n0[1]-n2[1]));
						p_kernel->para2[1] = ((n2[1]-n0[1])*(m[0]-n2[0]) + (n0[0]-n2[0])*(m[1]-n2[1]))/((n1[1]-n2[1])*(n0[0]-n2[0]) + (n2[0]-n1[0])*(n0[1]-n2[1]));
						p_kernel->para2[2] = 1 - p_kernel->para2[0] - p_kernel->para2[1];
						p_kernel->paraNode2[0] = fi->node0;
						p_kernel->paraNode2[1] = fi->node1;
						p_kernel->paraNode2[2] = fi->node2;
						p_kernel->surface_point_right[0] = m[0];
						p_kernel->surface_point_right[1] = m[1];
						p_kernel->surface_point_right[2] = m[2];
						
					}
				}
				count ++;
			}
		}
		if (flag_simulating)
		{
			vector<Node*>::iterator ni = p_kernel->p_vox_mesh->surface_node_list.begin();
			double x, y, z;
			Vector3d p;
			bool ispicked = false;

			force_arrow = Vector3d::Zero();

			switch (p_kernel->used_simulator)
			{
			case Kernel::VELOCITY_MATCHING:
			case Kernel::MULTIPLE_VELOCITY_MATCHING:
			case Kernel::HSM_ORIGINAL:
			case Kernel::HSM_FORCE4ITERATION:
			case Kernel::HSM_FORCE4STEP:
			case Kernel::HSM_ONE_STEP:
			case Kernel::HSM_ADAPTIVE_STEP:
			case Kernel::HSM_FORCE4STEP_FIRST:
			case Kernel::HSM_FORCE4STEP_FIRST1:
			case Kernel::HSM_FORCE4STEP_FIRST2:

				if(p_kernel->p_vox_mesh->constraint_node_list.empty())
				{
					for (; ni!=p_kernel->p_vox_mesh->surface_node_list.end(); ++ni)
					{
						p = (*ni)->coordinate + (*ni)->displacement;
						gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &x, &y, &z);

						if (isPicked(x, y))
						{
							p_kernel->removeConstraintOfAllLevel();
							p_kernel->setConstraint4AllLevel((*ni));
							ispicked = true;
							break; // to break loop for nodes
						}
					} // end loop node
					if(!ispicked)
					{
						p_kernel->removeConstraintOfAllLevel();
					}
				}
				break;
			case Kernel::SINGLE_GRID:
			case Kernel::SHAPE_MATCHING:
				for (; ni!=p_kernel->p_vox_mesh->surface_node_list.end(); ++ni)
				{
					p = (*ni)->coordinate + (*ni)->displacement;
					gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &x, &y, &z);

					if (isPicked(x, y))
					{
						p_kernel->p_vox_mesh->active_node = (*ni);
						break;
					}
				}
				break;
			}
		}
		//rope dragging
		if (flag_rope_dragging)
		{
			vector<joint>::iterator ji = p_kernel->p_rope->joint_list.begin();
			int i = 0;
			for (; ji != p_kernel->p_rope->joint_list.end(); ji++)
			{
				double x, y, z;
				Vector3d p = ji->_x;
				gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &x, &y, &z);

				if (isPicked(x, y))
				{
					p_kernel->p_rope->activeJoint = i;
					p_kernel->p_rope->constraint_list[0] = i;
					break;
				}
				i++;
			}
		}
	}


	if (current_mouse_button == Qt::RightButton)
	{
		if(flag_simulating)
		{
			if (p_kernel->p_vox_mesh->active_node)
			{
				p_kernel->p_vox_mesh->active_node->force.setZero();
				for(int i=0; i<p_kernel->p_vox_mesh->active_node->duplicates.size(); ++i)
				{
					p_kernel->p_vox_mesh->active_node->duplicates[i]->force.setZero();
				}
				p_kernel->p_vox_mesh->active_node = NULL;
				force_arrow.setZero();
			}
		}

		//rope dragging
		if (flag_rope_dragging)
		{
			vector<joint>::iterator ji = p_kernel->p_rope->joint_list.begin();
			int i = 0;
			for (; ji != p_kernel->p_rope->joint_list.end(); ji++)
			{
				double x, y, z;
				Vector3d p = ji->_x;
				gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &x, &y, &z);

				if (isPicked(x, y))
				{
					p_kernel->p_rope->staticJoint = i;
					if (p_kernel->p_rope->constraint_list.size() > 1)
						p_kernel->p_rope->constraint_list[1] = i;
					else{
						p_kernel->p_rope->constraint_list.push_back(i);
					}


					break;
				}
				i++;
			}
		}
	}
		
	last_mouse_button = current_mouse_button;
	
	updateGL();
}

void Renderer::mouseMoveEvent(QMouseEvent *e)
{	
	// mouseMoveEvent is not able to catch what mouse button is down
	double delta_x, delta_y;

	current_mouse_x = e->x();
	current_mouse_y = e->y();

	// truncate if over win boundary
	current_mouse_x = current_mouse_x > int(win_width) ? int(win_width) : current_mouse_x;
	current_mouse_x = current_mouse_x < 1 ? 1 : current_mouse_x;

	current_mouse_y = current_mouse_y > int(win_height) ? int(win_height) : current_mouse_y;
	current_mouse_y = current_mouse_y < 1 ? 1 : current_mouse_y;	
	
	mousei2d(current_mouse_x, current_mouse_y, &current_mouse_xd, &current_mouse_yd);

	delta_x = current_mouse_xd - last_mouse_xd;
	delta_y = current_mouse_yd - last_mouse_yd;
	
	if (last_mouse_button == Qt::LeftButton)
	{
		switch (theCamera.status)
		{
		case Camera::PANNING:
			translateCamera(delta_x, delta_y, 0);
			break;

		case Camera::ROTATING:
			rotateCamera(last_mouse_xd, last_mouse_yd, current_mouse_xd, current_mouse_yd);
			break;

		case Camera::ZOOMING:
			translateCamera(0, 0, -delta_y);
			break;

		case Camera::IDLING:
			/*
			if (flag_cube_operation)
			{
				Vector3d p = tool->getPosEnd();

				double wx, wy, wz;

				gluProject(p[0], p[1], p[2], 
					currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

				wx = current_mouse_x;
				wy = win_height - current_mouse_y;

				Vector3d np;
				gluUnProject(wx, wy, wz, currentmodelview, currentprojection, currentviewport, 
					&np[0], &np[1], &np[2]);

				//tool->setPosEnd(np[0], np[1],np[2]);
			}
			*/
			break;

		default:
			break;
		}
	}
		
	if (last_mouse_button == Qt::RightButton)
	{
		// defining a selection square with right button movement
		if (flag_show_selection || flag_show_constraints || flag_show_mass || flag_show_cube_static_constraints || flag_show_cube_active_constraints)
		{
			// before simulation
			if (last_mouse_button == Qt::RightButton)
			{
				bottom_right_x = current_mouse_x;
				bottom_right_y = current_mouse_y;
			}
			//after simulation
		}

		if (flag_simulating && !flag_show_selection && !flag_show_constraints && !flag_show_mass && !flag_show_cube_static_constraints && !flag_show_cube_active_constraints)
		{
			if(!flag_cube_operation)
			{
				switch (p_kernel->used_simulator)
				{
				case Kernel::SHAPE_MATCHING:
				case Kernel::SINGLE_GRID:
				case Kernel::HSM_ADAPTIVE_STEP:
				case Kernel::HSM_FORCE4ITERATION:
				case Kernel::HSM_FORCE4STEP:
				case Kernel::HSM_ONE_STEP:
				case Kernel::HSM_ORIGINAL:
				case Kernel::HSM_FORCE4STEP_FIRST:
				case Kernel::HSM_FORCE4STEP_FIRST1:
				case Kernel::HSM_FORCE4STEP_FIRST2:
				case Kernel::FLSM_ORIGINAL:
					//single point
					if (p_kernel->p_vox_mesh->active_node)
					{
						Vector3d force_arrow_begin = p_kernel->p_vox_mesh->active_node->coordinate + p_kernel->p_vox_mesh->active_node->displacement;

						double wx, wy, wz;

						gluProject(force_arrow_begin[0], force_arrow_begin[1], force_arrow_begin[2], 
							currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

						wx = current_mouse_x;
						wy = win_height - current_mouse_y;

						gluUnProject(wx, wy, wz, currentmodelview, currentprojection, currentviewport, 
							&force_arrow[0], &force_arrow[1], &force_arrow[2]);

						p_kernel->p_vox_mesh->active_node->force = force_arrow - force_arrow_begin;
						for(int i=0; i<p_kernel->p_vox_mesh->active_node->duplicates.size(); ++i)
						{
							p_kernel->p_vox_mesh->active_node->duplicates[i]->force = p_kernel->p_vox_mesh->active_node->force;
						}
					}
					//many constraints
					for(int j = 0; j < p_kernel->level_list.size(); j ++)
					{
						if ((!p_kernel->level_list[j]->voxmesh_level->constraint_node_list.empty() && p_kernel->idx_constraint == 1) || (!p_kernel->level_list[j]->voxmesh_level->another_constraint_node_list.empty() && p_kernel->idx_constraint == 2))
						{	
							if(p_kernel->constraintType == Kernel::FORCE_CONSTRAINT)
							{
								int size_k = p_kernel->level_list[j]->voxmesh_level->constraint_node_list.size();
								//put center to be the same, using the same force, set bottom level center.
								Vector3d force_arrow_begin = p_kernel->level_list[p_kernel->level_list.size()-1]->voxmesh_level->constraint_center;


								double wx, wy, wz;

								gluProject(force_arrow_begin[0], force_arrow_begin[1], force_arrow_begin[2], 
									currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

								wx = current_mouse_x;
								wy = win_height - current_mouse_y;

								gluUnProject(wx, wy, wz, currentmodelview, currentprojection, currentviewport, 
									&force_arrow[0], &force_arrow[1], &force_arrow[2]);

								//for recording
								p_kernel->current_force = force_arrow - force_arrow_begin;

								for(int k = 0; k < size_k; k ++)
								{
									p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->force =  force_arrow - force_arrow_begin;

									//revised here 0204
									for(int i=0; i<p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->duplicates.size(); ++i)
									{
										p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->duplicates[i]->force = force_arrow - force_arrow_begin;
									}
								}
							}
							else if(p_kernel->constraintType == Kernel::ORIENTATION_CONSTRAINT)
							{
								double degree = ((current_mouse_xd - last_mouse_xd) +  (current_mouse_yd - last_mouse_yd)) *90;

								//rotate(degree, axis_x, p_kernel->level_list[0]->voxmesh_level->constraint_center);
								//rotate(degree, axis_x, LCS_x);

								Matrix3d r;
								Vector3d angular_velocity;
								if(flag_axis == 1)
								{
									r = rotate(degree, LCS_x);
									angular_velocity = getAngularVelocity(degree, LCS_x);
								}
								else if(flag_axis == 2)
								{
									r = rotate(degree, LCS_y);
									angular_velocity = getAngularVelocity(degree, LCS_y);
								}
								else
								{
									r = rotate(degree, LCS_z);
									angular_velocity = getAngularVelocity(degree, LCS_z);
								}

								LCS_rotation = r * LCS_rotation;
								LCS_x = r * LCS_x;
								LCS_y = r * LCS_y;
								LCS_z = r * LCS_z;

								//rotation
								for(int j = 0; j < p_kernel->level_list.size(); j ++)
								{
									//p_kernel->level_list[j]->voxmesh_level->constraint_center = r * p_kernel->level_list[j]->voxmesh_level->constraint_center;
									if (!p_kernel->level_list[j]->voxmesh_level->constraint_node_list.empty())
									{
										int size_k = p_kernel->level_list[j]->voxmesh_level->constraint_node_list.size();
										for(int k = 0; k < size_k; k ++)
										{
											p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_position = p_kernel->level_list[j]->voxmesh_level->constraint_center + LCS_translation + LCS_rotation * p_kernel->level_list[j]->voxmesh_level->constraint_displacement[k];
											p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_rotation = r * p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_rotation;
										}
									}
								}

							}
							else if (p_kernel->constraintType == Kernel::POSITION_CONSTRAINT)
							{
								if(p_kernel->idx_constraint == 1)
								{
									if (!p_kernel->level_list[j]->voxmesh_level->constraint_node_list.empty())
									{
										int size_k = p_kernel->level_list[j]->voxmesh_level->constraint_node_list.size();
										//Vector3d sum = Vector3d::Zero();
										//for(int k = 0; k < size_k; k ++)
										//{
										//	 sum += p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_position;
										//}
										//p_kernel->level_list[j]->voxmesh_level->constraint_center = sum / size_k;

										Vector3d disp_center_before = p_kernel->level_list[j]->voxmesh_level->constraint_center;

										double wx, wy, wz;

										gluProject(disp_center_before[0], disp_center_before[1], disp_center_before[2], 
											currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

										wx = current_mouse_x;
										wy = win_height - current_mouse_y;

										Vector3d disp_center_after;
										gluUnProject(wx, wy, wz, currentmodelview, currentprojection, currentviewport, 
											&disp_center_after[0], &disp_center_after[1], &disp_center_after[2]);

										p_kernel->constraint_first = disp_center_after;
										p_kernel->level_list[j]->voxmesh_level->constraint_center = disp_center_after;
										LCS_translation = disp_center_after - disp_center_before;

										for(int k = 0; k < size_k; k ++)
										{
											Vector3d preposition = p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_position;
											p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_position = LCS_rotation * p_kernel->level_list[j]->voxmesh_level->constraint_displacement[k] + disp_center_after;
										}
									}
								}
								else
								{
									if (!p_kernel->level_list[j]->voxmesh_level->another_constraint_node_list.empty())
									{
										int size_k = p_kernel->level_list[j]->voxmesh_level->another_constraint_node_list.size();
										//Vector3d sum = Vector3d::Zero();
										//for(int k = 0; k < size_k; k ++)
										//{
										//	sum += p_kernel->level_list[j]->voxmesh_level->another_constraint_node_list[k]->prescribed_position;
										//}
										//p_kernel->level_list[j]->voxmesh_level->another_constraint_center =  sum / size_k;

										Vector3d disp_center_before = p_kernel->level_list[j]->voxmesh_level->another_constraint_center;

										double wx, wy, wz;

										gluProject(disp_center_before[0], disp_center_before[1], disp_center_before[2], 
											currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

										wx = current_mouse_x;
										wy = win_height - current_mouse_y;

										Vector3d disp_center_after;
										gluUnProject(wx, wy, wz, currentmodelview, currentprojection, currentviewport, 
											&disp_center_after[0], &disp_center_after[1], &disp_center_after[2]);

										p_kernel->constraint_second = disp_center_after;
										LCS_translation = disp_center_after - disp_center_before;

										for(int k = 0; k < size_k; k ++)
										{
											Vector3d preposition = p_kernel->level_list[j]->voxmesh_level->another_constraint_node_list[k]->prescribed_position;
											p_kernel->level_list[j]->voxmesh_level->another_constraint_node_list[k]->prescribed_position = LCS_rotation * p_kernel->level_list[j]->voxmesh_level->another_constraint_displacement[k] + disp_center_after;
										}
									}
								}
							}
						}
					}
					break;
				case Kernel::VELOCITY_MATCHING:
				case Kernel::MULTIPLE_VELOCITY_MATCHING:
					//single point
					for(int j = 0; j < p_kernel->level_list.size(); j ++)
					{
						if (p_kernel->level_list[j]->voxmesh_level->constraint_node)
						{
							Node* p_node = p_kernel->level_list[j]->voxmesh_level->constraint_node;

							Vector3d disp_arrow_begin = p_node->coordinate + p_node->displacement;

							double wx, wy, wz;

							gluProject(disp_arrow_begin[0], disp_arrow_begin[1], disp_arrow_begin[2], 
								currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

							wx = current_mouse_x;
							wy = win_height - current_mouse_y;

							gluUnProject(wx, wy, wz, currentmodelview, currentprojection, currentviewport, 
								&p_node->prescribed_position[0], &p_node->prescribed_position[1], &p_node->prescribed_position[2]);

							//for(int i=0; i<p_node->duplicates.size(); ++i)
							//{
							//	p_node->duplicates[i]->target_position = p_node->prescribed_position;
							//}
							if(p_node)
								force_arrow = p_node->prescribed_position;
							else
								force_arrow = Vector3d::Zero();
						}
					}
					//many constraints
					if(p_kernel->constraintType == Kernel::POSITION_CONSTRAINT)
					{
						for(int j = 0; j < p_kernel->level_list.size(); j ++)
						{
							if (!p_kernel->level_list[j]->voxmesh_level->constraint_node_list.empty())
							{
								Vector3d disp_center_before = p_kernel->level_list[j]->voxmesh_level->constraint_center;

								double wx, wy, wz;

								gluProject(disp_center_before[0], disp_center_before[1], disp_center_before[2], 
									currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

								wx = current_mouse_x;
								wy = win_height - current_mouse_y;

								Vector3d disp_center_after;
								gluUnProject(wx, wy, wz, currentmodelview, currentprojection, currentviewport, 
									&disp_center_after[0], &disp_center_after[1], &disp_center_after[2]);

							
								LCS_translation = disp_center_after - disp_center_before;

								int size_k = p_kernel->level_list[j]->voxmesh_level->constraint_node_list.size();
								for(int k = 0; k < size_k; k ++)
								{
									Vector3d preposition = p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_position;
									p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_position = LCS_rotation * p_kernel->level_list[j]->voxmesh_level->constraint_displacement[k] + disp_center_after;
									Vector3d linear_velocity = p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_position - preposition;
									//cout << linear_velocity << endl;
									//Vector3d linear_velocity = p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_position - p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->target_position;
									p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_linear_velocity = linear_velocity / p_kernel->time_step_size;
									/*
									for(int n = 0; n < p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->duplicates.size(); n ++)
										p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->duplicates[n]->prescribed_linear_velocity = 
										p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_position - p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->duplicates[n]->static_position;
									*/
									p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->flag_constraint_type = 1;
								}
							}
						}
					}
					else
					{
						double degree = ((current_mouse_xd - last_mouse_xd) +  (current_mouse_yd - last_mouse_yd)) *90;

						//rotate(degree, axis_x, p_kernel->level_list[0]->voxmesh_level->constraint_center);
						//rotate(degree, axis_x, LCS_x);

						Matrix3d r;
						Vector3d angular_velocity;
						if(flag_axis == 1)
						{
							r = rotate(degree, LCS_x);
							angular_velocity = getAngularVelocity(degree, LCS_x);
						}
						else if(flag_axis == 2)
						{
							r = rotate(degree, LCS_y);
							angular_velocity = getAngularVelocity(degree, LCS_y);
						}
						else
						{
							r = rotate(degree, LCS_z);
							angular_velocity = getAngularVelocity(degree, LCS_z);
						}

						LCS_rotation = r * LCS_rotation;
						LCS_x = r * LCS_x;
						LCS_y = r * LCS_y;
						LCS_z = r * LCS_z;

						//rotation
						for(int j = 0; j < p_kernel->level_list.size(); j ++)
						{
							//p_kernel->level_list[j]->voxmesh_level->constraint_center = r * p_kernel->level_list[j]->voxmesh_level->constraint_center;
							if (!p_kernel->level_list[j]->voxmesh_level->constraint_node_list.empty())
							{
								int size_k = p_kernel->level_list[j]->voxmesh_level->constraint_node_list.size();
								for(int k = 0; k < size_k; k ++)
								{
									p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_position = p_kernel->level_list[j]->voxmesh_level->constraint_center + LCS_translation + LCS_rotation * p_kernel->level_list[j]->voxmesh_level->constraint_displacement[k];
									p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_rotation = r * p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_rotation;
									p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_angular_velocity = angular_velocity / p_kernel->time_step_size;
									p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->flag_constraint_type = 2;
								}
							}
						}
					}
					break;
				} // end switch
			}
			else
			{
				//cube operation
			}

			//rope dragging
			if (flag_rope_dragging)
			{
				if (p_kernel->p_rope->activeJoint != -1)
				{
					Vector3d mouse_pos_before = p_kernel->p_rope->joint_list[p_kernel->p_rope->activeJoint]._x;

					double wx, wy, wz;

					gluProject(mouse_pos_before[0], mouse_pos_before[1], mouse_pos_before[2], 
						currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

					wx = current_mouse_x;
					wy = win_height - current_mouse_y;
					
					Vector3d mouse_pos;

					gluUnProject(wx, wy, wz, currentmodelview, currentprojection, currentviewport, 
						&mouse_pos[0], &mouse_pos[1], &mouse_pos[2]);

					p_kernel->p_rope->joint_list[p_kernel->p_rope->activeJoint]._x = mouse_pos;
				}
			}
		} // end if simulation
	}


	last_mouse_xd = current_mouse_xd;
	last_mouse_yd = current_mouse_yd;

	last_mouse_x = current_mouse_x;
	last_mouse_y = current_mouse_y;
	if (!flag_simulating)
	{
		updateGL();
	}
}

void Renderer::mouseReleaseEvent(QMouseEvent *e)
{
	/*double delta_x, delta_y;

	current_mouse_x = e->x();
	current_mouse_y = e->y();

	mousei2d(current_mouse_x, current_mouse_y, &current_mouse_xd, &current_mouse_yd);

	delta_x = current_mouse_xd - last_mouse_xd;
	delta_y = current_mouse_yd - last_mouse_yd;*/

	current_mouse_button = e->button();
	
	if (current_mouse_button == Qt::RightButton)
	{
		if(flag_simulating)
		{
			//if one active node, delete
			if (p_kernel->p_vox_mesh->active_node)
			{
				p_kernel->p_vox_mesh->active_node->force.setZero();
				for(int i=0; i<p_kernel->p_vox_mesh->active_node->duplicates.size(); ++i)
				{
					p_kernel->p_vox_mesh->active_node->duplicates[i]->force.setZero();
				}
				p_kernel->p_vox_mesh->active_node = NULL;
				force_arrow.setZero();
			}
			//if many active nodes, delete all
			if(p_kernel->constraintType == Kernel::FORCE_CONSTRAINT)
			{
				for(int j = 0; j < p_kernel->level_list.size(); j ++)
				{
					if (!p_kernel->level_list[j]->voxmesh_level->constraint_node_list.empty())
					{	
						int size_k = p_kernel->level_list[j]->voxmesh_level->constraint_node_list.size();
						//put center to be the same, using the same force, set bottom level center.
						for(int k = 0; k < size_k; k ++)
						{
							p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->force.setZero();
							p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->flag_constraint_node = false;

							for(int i=0; i<p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->duplicates.size(); ++i)
							{
								p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->duplicates[i]->force.setZero();
								p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->duplicates[i]->flag_constraint_node = false;
							}
						}
						p_kernel->level_list[j]->voxmesh_level->constraint_node_list.clear();
						p_kernel->level_list[j]->voxmesh_level->constraint_cluster_list.clear();
					}
					force_arrow.setZero();
					p_kernel->current_force.setZero();
				}
			}
			//if one constraint
		}
			/*
		if(flag_simulating)
		{
			//many constraints
			if(flag_position_constraint)
			{
				for(int j = 0; j < p_kernel->level_list.size(); j ++)
				{
					if (!p_kernel->level_list[j]->voxmesh_level->constraint_node_list.empty())
					{
						int size_k = p_kernel->level_list[j]->voxmesh_level->constraint_node_list.size();
						for(int k = 0; k < size_k; k ++)
						{
							//p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_linear_velocity = Vector3d::Zero();
						}
					}
				}
			}
			else
			{
				//rotation
				for(int j = 0; j < p_kernel->level_list.size(); j ++)
				{
					//p_kernel->level_list[j]->voxmesh_level->constraint_center = r * p_kernel->level_list[j]->voxmesh_level->constraint_center;
					if (!p_kernel->level_list[j]->voxmesh_level->constraint_node_list.empty())
					{
						int size_k = p_kernel->level_list[j]->voxmesh_level->constraint_node_list.size();
						for(int k = 0; k < size_k; k ++)
						{
							p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->prescribed_angular_velocity = Vector3d::Zero();
						}
					}
				}
			}
		}
			*/
	}
	
	last_mouse_button = current_mouse_button;
	updateGL();
}

void Renderer::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{

	case Qt::Key_Control: 
		theCamera.status = Camera::PANNING;
		break;

	case Qt::Key_Shift:
		theCamera.status = Camera::ZOOMING;
		break;

	case Qt::Key_Alt:
		theCamera.status = Camera::ROTATING;
		break;

	case Qt::Key_Escape:
		initCamera();
		break;

	case Qt::Key_Backspace:
		theCamera.RestoreSave();
		break;

	case Qt::Key_1:
		flag_axis = 1;
		break;
	
	case Qt::Key_2:
		flag_axis = 2;
		break;

	case Qt::Key_3:
		flag_axis = 3;
		break;
	case Qt::Key_A:
		p_kernel->flag_converge = true;
		break;
	case Qt::Key_B:
		p_kernel->flag_exportObj = !p_kernel->flag_exportObj;
		if (p_kernel->flag_exportObj)
		{
			cout << "start exporting obj!" << endl;
		}	
		else
		{
			cout << "stop exporting obj!" << endl;
		}
		break;
	case Qt::Key_C:
		if(p_kernel->idx_constraint == 2)
		{
			p_kernel->idx_constraint = 1;
			cout << "1" << endl;
		}
		else
		{
			p_kernel->idx_constraint = 2;
			cout << "2" << endl;
		}
		break;
	case Qt::Key_D:
		p_kernel->flag_exportForce = !p_kernel->flag_exportForce;
		if (p_kernel->flag_exportForce)
		{
			p_kernel->myForce.open("record.txt", ios::out);
			cout << "start exporting force!" << endl;
		}	
		else
		{
			p_kernel->myForce.close();
			cout << "stop exporting force!" << endl;
		}
		break;
	case Qt::Key_E:
		p_kernel->flag_exportPosConstraint = !p_kernel->flag_exportPosConstraint;
		if (p_kernel->flag_exportPosConstraint)
		{
			p_kernel->myForce.open("record_first.txt", ios::out);
			p_kernel->myAnotherForce.open("record_second.txt", ios::out);
			cout << "start exporting position!" << endl;
		}	
		else
		{
			p_kernel->myAnotherForce.close();
			p_kernel->myForce.close();
			cout << "stop exporting position!" << endl;
		}
		break;
	case Qt::Key_P:
		p_kernel->p_rope->addPath();
		break;
	case Qt::Key_Q:
		p_kernel->p_rope->addPos2Path(p_kernel->p_rope->joint_list[0]._x,0);
		break;
	default:
		theCamera.status = Camera::IDLING;
		break;

	}
	updateGL();
	return;
}

void Renderer::keyReleaseEvent(QKeyEvent *e)
{
	switch (e->key())
	{

	case Qt::Key_Control: 
		theCamera.status = Camera::IDLING;
		break;

	case Qt::Key_Shift:
		theCamera.status = Camera::IDLING;
		break;

	case Qt::Key_Alt:
		theCamera.status = Camera::IDLING;
		break;

	case Qt::Key_C:
				
		break;

	default:

		//theCamera.status = Camera::IDLING;
		break;

	}
	updateGL();
	return;
}

void Renderer::rotateCamera(double ox, double oy, double nx, double ny)
{
	Quaternion<double> q;
	Camera::Arc2Quaternion(ox, oy, nx, ny, q);

	if (rotate_dist == 0) 
	{
		q.x() = -q.x();
		q.y() = -q.y();
	}

	theCamera.Rotate(q, rotate_dist);
	wcsCamera.Rotate(q, CAMERA_ROT_DISTANCE);
}

void Renderer::translateCamera(double dx, double dy, double dz)
{
	// some auto param adjustment to be refined later
	double scalefactor = 0.5*view_dist;
	dx *= scalefactor*theCamera.fov;
	dy *= scalefactor*theCamera.fov;
	dz = view_dist * (exp(-0.5 * dz) - 1.0);

	// moving the camera in the opposite direction to
	// the direction we want the object to move
	theCamera.Move(-dx, -dy, -dz);
	updateDist(-dx, -dy, -dz);
}

// convert integer coordinates of mouse position to normalized floats
void Renderer::mousei2d(int x, int y, double *xf, double *yf)
{
	int screenwidth = width();
	int screenheight = height();

	double r = (double)screenwidth/screenheight;
	if (r > 1.0) {
		*xf = r * (2.0 * x / screenwidth - 1.0);
		*yf = 1.0 - 2.0 * y / screenheight;
	} else {
		*xf = 2.0 * x / screenwidth - 1.0;
		*yf = (1.0 - 2.0 * y / screenheight) / r;
	}

}

void Renderer::updateDist(float dx, float dy, float dz)
{
	Vector3d viewdir(0, 0, -view_dist);
	Vector3d delta(-dx, -dy, -dz);

	Vector3d newviewdir = viewdir + delta;

	view_dist = -newviewdir(2);
	// Always rotate around the center of the object
	// which is assumed to be place at the origin of the world coordinate
	rotate_dist = view_dist;

}

void Renderer::renderSelectSquare(int x1, int y1, int x2, int y2)
{
	/***
		Need to re specify both model view and projection matrix
	***/

	if (x1 <0 || x2 < 0 || y1 < 0 || y2 <0)
	return;

	glDisable(GL_LIGHTING);
	
	glColor4f(0.1, 0.4, 0.2, 0.1);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, win_width, 0, win_height, 0, 1);

	glDisable(GL_DEPTH_TEST);

	y1 = win_height-y1;
	y2 = win_height-y2;
	/*
	glBegin(GL_QUADS);
	glVertex2i(x1, y1);
	glVertex2i(x1, y2);
	glVertex2i(x2, y2);
	glVertex2i(x2, y1);
	glEnd();
	*/

	glDisable(GL_BLEND);
	glColor3f(0.1, 0.9, 0.2);

	glBegin(GL_LINE_LOOP);
	x1--; y1--; x2++; y2++;
	glVertex2i(x1, y1);
	glVertex2i(x1, y2);
	glVertex2i(x2, y2);
	glVertex2i(x2, y1);
	glEnd();
		
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
}

void Renderer::renderStatus()
{
	char text[20][256];

	// raster fonts
	glDisable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, win_width, 0.0, win_height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3fv(status_color);
	int num_stats_line = 0;
	
	// mesh information
	if (p_kernel->flag_mesh_ready)
	{
		sprintf(text[num_stats_line++], "Mesh: %d vertex, %d face", p_kernel->p_mesh->number_node, p_kernel->p_mesh->number_face);
		if(p_kernel->network_role == Kernel::NETWORK_ROLE_CLIENT)
		{
			sprintf(text[num_stats_line++], "Data from host: %s  port: %d", p_kernel->ip, p_kernel->port);
			//sprintf(text[num_stats_line++], "FPS: %f", p_kernel->fps);
		}
		else
		{
			//sprintf(text[num_stats_line++], "FPS: %f", p_kernel->fps);
			int cx = 0;

			cx = sprintf(text[num_stats_line], "Grid Resolution: ");
			for(int i = 0; i < p_kernel->level_list.size(); i++)
			{
				cx += sprintf(text[num_stats_line]+cx, "L%d %dX%dX%d ", i,  p_kernel->level_list[i]->gridDensity,  p_kernel->level_list[i]->gridDensity,  p_kernel->level_list[i]->gridDensity);
			}
			num_stats_line ++;
			cx = 0;

			cx = sprintf(text[num_stats_line], "Node #: ", p_kernel->num_sim_node);
			for(int i = 0; i < p_kernel->level_list.size(); i++)
			{
				cx += sprintf(text[num_stats_line]+cx, "L%d %d ", i, p_kernel->level_list[i]->num_sim_node);
			}
			num_stats_line ++;
			cx = 0;

			cx = sprintf(text[num_stats_line], "Cluster #: ");
			for(int i = 0; i < p_kernel->level_list.size(); i++)
			{
				if(p_kernel->level_list[i]->voxmesh_level)
					cx += sprintf(text[num_stats_line]+cx, "L%d %d ", i, p_kernel->level_list[i]->voxmesh_level->num_cluster);
			}
			num_stats_line ++;
			cx = 0;
			
			cx = sprintf(text[num_stats_line], "SM #: ");
			for(int i = 0; i < p_kernel->level_list.size(); i++)
			{
				cx += sprintf(text[num_stats_line]+cx, "L%d %d ", i, p_kernel->level_list[i]->times_ShapeMatching);
			}
			num_stats_line ++;
			cx = 0;

			sprintf(text[num_stats_line++], "Display Level: %d", p_kernel->level_display);

			if(p_kernel->flag_setForce)
				sprintf(text[num_stats_line++], "Constant Force: %f %f %f", p_kernel->const_force.x(), p_kernel->const_force.y(), p_kernel->const_force.z());
		}
	}
	
	// others...

	// render string
	for (int i=0; i<num_stats_line; ++i)
	{
		glPushMatrix();
		glTranslatef(20.0, win_height - 20.0 - 15.0*i, 0.0f);
		//glScalef(0.1, 0.1, 0.1);
		renderChar(text[i], GLUT_BITMAP_HELVETICA_12);
		glPopMatrix();
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_LIGHTING);
}

void Renderer::renderArrow(double x0, double y0, double z0, double x1, double y1, double z1, double size, float R, float G, float B, float A)
{
	double arraw_len = sqrt( (x0-x1)*(x0-x1)+(y0-y1)*(y0-y1)+(z0-z1)*(z0-z1) );
	
	Vector3d v1, v2, axis;
	v1(0) = x0 - x1;
	v1(1) = y0 - y1;
	v1(2) = z0 - z1;
	v1.normalize();

	v2(0) = 0;
	v2(1) = 0;
	v2(2) = 1;

	axis = v1.cross(v2);
	double angle = acos(v1(2))/3.1415926535*180.0;

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glColor4f(R, G, B, A);

	glPushMatrix();
	glTranslated(x1, y1, z1);
	glRotated(-angle, axis[0], axis[1], axis[2]);
	glutSolidCylinder(size, arraw_len*0.8, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x1+(x0-x1)*0.8, y1+(y0-y1)*0.8, z1+0.8*(z0-z1));
	glRotated(-angle, axis(0), axis(1), axis(2));
	glutSolidCone(size*1.5, arraw_len*0.2, 10, 10);
	glPopMatrix();
}

void Renderer::renderSphere(double r, int lats, int longs)
{
	int i, j;
	for(i = 0; i <= lats; i++) {
	   double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
	   double z0  = sin(lat0);
	   double zr0 =  cos(lat0);

	   double lat1 = M_PI * (-0.5 + (double) i / lats);
	   double z1 = sin(lat1);
	   double zr1 = cos(lat1);

	   glBegin(GL_QUAD_STRIP);
	   for(j = 0; j <= longs; j++) {
	        double lng = 2 * M_PI * (double) (j - 1) / longs;
	        double x = cos(lng);
		    double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
	   }
	   glEnd();
	}
}

void Renderer::renderCylinder(double x0, double y0, double z0, double x1, double y1, double z1, double size, float R, float G, float B, float A)
{
	/*int mat = material_mode;

	double arraw_len = sqrt( (x0-x1)*(x0-x1)+(y0-y1)*(y0-y1)+(z0-z1)*(z0-z1) );
	
	Kernel::Vector3 v1, v2, axis;
	v1[0] = x0 - x1;
	v1[1] = y0 - y1;
	v1[2] = z0 - z1;
	v1 = normalize(v1);

	v2[0] = 0;
	v2[1] = 0;
	v2[2] = 1;

	axis = cross(v1, v2);
	double angle = acos(v1[2])/3.1415926535*180.0;

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glColor4f(R, G, B, A);

	glPushMatrix();
	glTranslated(x1, y1, z1);
	glRotated(-angle, axis[0], axis[1], axis[2]);
	glutSolidCylinder(size, arraw_len, 10, 10);
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glDisable(GL_COLOR_MATERIAL);
	SetMaterial(mat);*/
}	

void Renderer::renderCircle(double x, double y, double z, double n1, double n2, double n3, double r, 
							double color_r, double color_g, double color_b, double color_a)
{
	//glDisable(GL_LIGHT0);
	//glDisable(GL_LIGHTING);

	//glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Kernel::Vector3 p0, p1, n, o;
	//
	//o[0] = x; o[1] = y; o[2] = z;
	//n[0] = n1; n[1] = n2; n[2] = n3;

	//p0[0] = 0; p0[1] = 0; p0[2] = 1;

	//p0 = cross(p0, n);

	//if ( length(p0) < 1e-5 ) // if n is very parallel to z axis--> change anthor axis
	//{
	//	p0[0] = 0; p0[1] = 1; p0[2] = 0;
	//	p0 = cross(p0, n);
	//}

	//p0 = normalize(p0);
	//
	//p0 = p0*r;
	//
	//for (int i=0; i<100; i++)
	//{
	//	Kernel::RotateVector(p0, n, 2.0*3.1416/100.0, p1);
	//	glColor4f(color_r, color_g, color_b, color_a);
	//	glPushMatrix();
	//	glTranslated(x, y, z);
	//	glBegin(GL_LINES);
	//	glVertex3d(p0[0], p0[1], p0[2]);
	//	glVertex3d(p1[0], p1[1], p1[2]);
	//	glEnd();
	//	glPopMatrix();
	//	p0 = p1;
	//}
	//		
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);
	//glDisable(GL_BLEND);
}

void Renderer::renderCircle(Vector3d c, Vector3d n, double r, double color_r, double color_g, double color_b, double color_a)
{
	//glDisable(GL_LIGHT0);
	//glDisable(GL_LIGHTING);

	//glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//double pi = 3.1415926;
	//Vector3d W = n;
	//W.normalize();
	//Vector3d U;
	//Vector3d V;
	//glBegin(GL_LINES); 
 //   //glLineWidth(3); 

	//W.normalize();
	//for(int i = 0;i <= 360; i++) //no of short lines to be drawn 
 //   { 
	//	int j = (i+1) % 360;
	//	double t1 = i * pi/180;
	//	double t2 = j * pi/180;

	//	double factor;
	//	if (abs(W(0)) >= abs(W(1)))
	//	{
	//	factor = 1/sqrt(W(0)*W(0) + W(2)*W(2));
	//	U(0) = -w2*factor;
	//	u1 = 0;
	//	u2 = +w0*factor;
	//	}
	//	else
	//	{
	//	factor = 1/sqrt(w1*w1+w2*w2);
	//	u0 = 0;
	//	u1 = +w2*factor;
	//	u2 = -w1*factor;
	//	}

 //
 //   } 
 //   glEnd();

	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);
	//glDisable(GL_BLEND);
}

void Renderer::renderMesh(const Mesh* m)
{
	glColor3fv(render_mesh_color);
	vector<Face>::iterator fi = p_kernel->p_mesh->face_list.begin();
	for (; fi!=p_kernel->p_mesh->face_list.end(); ++fi)
	{
		Vector3d n0, n1, n2, n01, n02, n;

		n0 = fi->node0->coordinate + fi->node0->displacement;
		n1 = fi->node1->coordinate + fi->node1->displacement;
		n2 = fi->node2->coordinate + fi->node2->displacement;

		n01 = n0 - n1;
		n02 = n0 - n2;

		n = n01.cross(n02);
		n.normalize();
		fi->normal = n;
		
		//display a line mesh
		//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glBegin(GL_TRIANGLES);
		glNormal3d(n[0], n[1], n[2]);
		glVertex3d(n0[0], n0[1], n0[2]);
		glVertex3d(n1[0], n1[1], n1[2]);
		glVertex3d(n2[0], n2[1], n2[2]);
		glEnd();
	}
}

void Renderer::renderWCS(float shift)
{
	// fixed coordinate always on top
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(shift, shift*float(win_height)/float(win_width), 0);

	glPushMatrix();
	wcsCamera.SetCoordGL(CAMERA_VIEW_DISTANCE/DOF, CAMERA_VIEW_DISTANCE*DOF, win_width, win_height);
	
	glDisable(GL_LIGHTING);

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.3, 0, 0);
	glEnd();
	renderChar(0.3, -0.05, 0.0, "x", GLUT_BITMAP_HELVETICA_10);

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0.3, 0);
	glEnd();
	renderChar(0.05, 0.3, 0.0, "y", GLUT_BITMAP_HELVETICA_10);

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 0.3);
	glEnd();
	renderChar(0.05, 0.0, 0.3, "z", GLUT_BITMAP_HELVETICA_10);

	glPopMatrix();
	glPopMatrix();

	glEnable(GL_LIGHTING);
	
}

void Renderer::renderLCS(Vector3d p)
{
	//// fixed coordinate always on top
	//glClear(GL_DEPTH_BUFFER_BIT);
	//
	//glMatrixMode(GL_MODELVIEW_MATRIX);
	//glPushMatrix();
	//glLoadIdentity();
	//glTranslatef(p(0), p(1), p(2));

	//glPushMatrix();
	//wcsCamera.SetCoordGL(CAMERA_VIEW_DISTANCE/DOF, CAMERA_VIEW_DISTANCE*DOF, win_width, win_height);

	glDisable(GL_LIGHTING);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
	glColor4f(1.0, 0.0, 0.0, 0.4);

	glBegin(GL_LINES);
	//glVertex3f(0, 0, 0);
	//glVertex3f(LCS_x(0)-p(0), LCS_x(1)-p(1), LCS_x(2)-p(2));
	glVertex3f(p(0), p(1), p(2));
	glVertex3f(LCS_x(0)+p(0), LCS_x(1)+p(1), LCS_x(2)+p(2));
	glVertex3f(0.3, 0, 0);
	glEnd();
	//renderChar(0.3, -0.05, 0.0, "x", GLUT_BITMAP_HELVETICA_10);

	glColor4f(0.0, 1.0, 0.0, 0.4);
	glBegin(GL_LINES);
	//glVertex3f(0, 0, 0);
	//glVertex3f(0, 0.3, 0);
	glVertex3f(p(0), p(1), p(2));
	glVertex3f(LCS_y(0)+p(0), LCS_y(1)+p(1), LCS_y(2)+p(2));
	glEnd();
	//renderChar(0.05, 0.3, 0.0, "y", GLUT_BITMAP_HELVETICA_10);

	glColor4f(0.0, 0.0, 1.0, 0.4);
	glBegin(GL_LINES);
	glVertex3f(p(0), p(1), p(2));
	glVertex3f(LCS_z(0)+p(0), LCS_z(1)+p(1), LCS_z(2)+p(2));
	//glVertex3f(0, 0, 0);
	//glVertex3f(0, 0, 0.3);
	glEnd();
	//renderChar(0.05, 0.0, 0.3, "z", GLUT_BITMAP_HELVETICA_10);

	//glPopMatrix();
	//glPopMatrix();

	glEnable(GL_LIGHTING);
}
bool Renderer::isSelected(double x, double y)
{
	y = win_height - y;

	if (x >= upper_left_x && x <= bottom_right_x && y >= upper_left_y && y <= bottom_right_y)
		return true;
	else 
		return false;
}

void Renderer::renderChar(const char *text, void *font)
{
	if (!text)
		return;

	int nchar = strlen(text);
	
	glRasterPos2f(0, 0);

	for (int i = 0; i < nchar; ++i)
	{
		glutBitmapCharacter(font, text[i]);
	}
}

void Renderer::renderChar(double x, double y, double z, const char *text, void *font)
{
	// raster fonts
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslated(x, y, z);
	renderChar(text, font);
	glPopMatrix();
}

//void Renderer::getSelectedNode(const T4Mesh* tet_mesh, vector<int> &node_list)
//{
//	node_list.clear();
//	node_list.reserve(tet_mesh->size_nodes()/10);
//
//	double wx, wy, wz;
//	T4Mesh::node_iterator n_iter = tet_mesh->node_begin();
//	for (; n_iter != tet_mesh->node_end(); ++n_iter)
//	{
//		vec3 p = n_iter->world_coordinate;
//
//		gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);
//
//		if (isSelected(wx, wy) && !n_iter->is_anchor_node)
//		{
//			node_list.push_back(n_iter->idx());
//		}
//	}
//};

void Renderer::renderCube(double v_dim, double vol_center_x, double vol_center_y, double vol_center_z)
{
	// render six quads
	glBegin(GL_QUADS);

	// face up
	glNormal3d(0.0, 1.0, 0.0);
	glVertex3d(vol_center_x - v_dim, vol_center_y + v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x - v_dim, vol_center_y + v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y + v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y + v_dim, vol_center_z - v_dim);

	// face down
	glNormal3d(0.0, -1.0, 0.0);
	glVertex3d(vol_center_x - v_dim, vol_center_y - v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y - v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y - v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x - v_dim, vol_center_y - v_dim, vol_center_z + v_dim);

	// face left
	glNormal3d(-1.0, 0.0, 0.0);
	glVertex3d(vol_center_x - v_dim, vol_center_y + v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x - v_dim, vol_center_y - v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x - v_dim, vol_center_y - v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x - v_dim, vol_center_y + v_dim, vol_center_z + v_dim);

	// face right
	glNormal3d(1.0, 0.0, 0.0);
	glVertex3d(vol_center_x + v_dim, vol_center_y + v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y + v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y - v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y - v_dim, vol_center_z - v_dim);

	// face front
	glNormal3d(0.0, 0.0, 1.0);
	glVertex3d(vol_center_x - v_dim, vol_center_y + v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x - v_dim, vol_center_y - v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y - v_dim, vol_center_z + v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y + v_dim, vol_center_z + v_dim);

	// face back
	glNormal3d(0.0, 0.0, -1.0);
	glVertex3d(vol_center_x - v_dim, vol_center_y + v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y + v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x + v_dim, vol_center_y - v_dim, vol_center_z - v_dim);
	glVertex3d(vol_center_x - v_dim, vol_center_y - v_dim, vol_center_z - v_dim);

	glEnd();
}

void Renderer::renderGrid()
{
	glColor4f(grid_color[0], grid_color[1], grid_color[2], 0.1);
	int dim = p_kernel->grid_density;
	int dim2 = dim*dim;
	double v_dim = 1.0 / double(dim);

	// grid line
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	
	for (int i=0; i<dim+1; ++i)
	{
		float x = float(i)*v_dim - 0.5;
		float y = float(i)*v_dim - 0.5;
		float z = float(i)*v_dim - 0.5;

		glBegin(GL_QUADS);
		glNormal3f(1.0 , 0.0, 0.0);
		glVertex3f(x, -0.5, 0.5);
		glVertex3f(x, -0.5, -0.5);
		glVertex3f(x, 0.5, -0.5);
		glVertex3f(x, 0.5, 0.5);

		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(-0.5, y, 0.5);
		glVertex3f(0.5, y, 0.5);
		glVertex3f(0.5, y, -0.5);
		glVertex3f(-0.5, y, -0.5);

		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-0.5, -0.5, z);
		glVertex3f(0.5, -0.5, z);
		glVertex3f(0.5, 0.5, z);
		glVertex3f(-0.5, 0.5, z);
		glEnd();
	}

	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_CULL_FACE);
}

void Renderer::renderVoxMesh(const VoxMesh* vm)
{
	if (!vm->num_surface_vox)
	{
		return;
	}

	double v_dim = vm->vox_size * 0.5;

	
	glColor3fv(vox_mesh_color);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	vector<Vox*>::const_iterator vox_iter = vm->surface_vox_list.begin();
	
	for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
	{
		Vector3d p0, p1, p2, p3, n1, n2, n;
		glBegin(GL_QUADS);

		if ((*vox_iter)->flag_top_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_bottom_face)
		{
			p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}


		if ((*vox_iter)->flag_left_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_right_face)
		{
			p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_front_face)
		{
			p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_back_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		glEnd();
	}
	
	glColor4f(vox_mesh_color[0], vox_mesh_color[1], vox_mesh_color[2], 0.2);

	glPolygonMode(GL_FRONT, GL_FILL);
	vox_iter = vm->surface_vox_list.begin();
	
	for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
	{
		Vector3d p0, p1, p2, p3, n1, n2, n;
		glBegin(GL_QUADS);

		if ((*vox_iter)->flag_top_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_bottom_face)
		{
			p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}


		if ((*vox_iter)->flag_left_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_right_face)
		{
			p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_front_face)
		{
			p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_back_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		glEnd();
	}
		
	double cube_size = v_dim*0.2;
	// need to differentiate selected and none selected (surface) voxel

	// render selected nodes for anchor nodes
	glColor3fv(anchor_node_color);
	if (flag_show_selection) 
	{
		vector<Node*>::const_iterator n_iter = vm->surface_node_list.begin();

		for (; n_iter!=vm->surface_node_list.end(); ++n_iter)
		{
			double wx, wy, wz;
			Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;

			gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

			if (isSelected(wx, wy))
			{
				renderCube(cube_size, p(0), p(1), p(2));
			}
		}
	}
	// render anchor nodes
	if (!vm->anchor_node_list.empty())
	{
		vector<Node*>::const_iterator n_iter = vm->anchor_node_list.begin();
		for(; n_iter!=vm->anchor_node_list.end(); ++n_iter)
		{
			Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;
			renderCube(cube_size, p(0), p(1), p(2));
		}
	}

	// render selected nodes for constraint nodes
	glColor3fv(active_node_color);
	if (flag_show_constraints) 
	{
		vector<Node*>::const_iterator n_iter = vm->surface_node_list.begin();

		for (; n_iter!=vm->surface_node_list.end(); ++n_iter)
		{
			double wx, wy, wz;
			Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;

			gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

			if (isSelected(wx, wy))
			{
				renderCube(cube_size, p(0), p(1), p(2));
			}
		}
	}
	
	// render constraint nodes
	if (!vm->constraint_node_list.empty())
	{
		vector<Node*>::const_iterator n_iter = vm->constraint_node_list.begin();
		for(; n_iter!=vm->constraint_node_list.end(); ++n_iter)
		{
			Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;
			renderCube(cube_size, p(0), p(1), p(2));
		}
	}

	// render active node
	if (vm->active_node)
	{
		glColor3fv(active_node_color);
		Vector3d p = vm->active_node->coordinate + vm->active_node->displacement;
		renderCube(cube_size, p(0), p(1), p(2));
	}

	//render constraint_node
	if (vm->constraint_node)
	{
		glColor3fv(active_node_color);
		Vector3d p = vm->constraint_node->coordinate + vm->constraint_node->displacement;
		renderCube(cube_size, p(0), p(1), p(2));
	}
}

void Renderer::renderVoxMesh(const VoxMesh* vm, float* specified_color)
{
	if (!vm->num_surface_vox)
	{
		return;
	}

	double v_dim = vm->vox_size * 0.5;
	
	glColor4f(specified_color[0], specified_color[1], specified_color[2], 0.3);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	vector<Vox*>::const_iterator vox_iter = vm->surface_vox_list.begin();

	for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
	{
		Vector3d p0, p1, p2, p3, n1, n2, n;
		glBegin(GL_QUADS);

		if ((*vox_iter)->flag_top_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;

			Vector3d p10 = p1 - p0;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_bottom_face)
		{
			p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;

			Vector3d p10 = p1 - p0;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}


		if ((*vox_iter)->flag_left_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;

			Vector3d p10 = p1 - p0;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_right_face)
		{
			p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;

			Vector3d p10 = p1 - p0;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_front_face)
		{
			p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;

			Vector3d p10 = p1 - p0;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_back_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;

			Vector3d p10 = p1 - p0;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		glEnd();
	}

	glColor4f(specified_color[0], specified_color[1], specified_color[2], 0.1);

	glPolygonMode(GL_FRONT, GL_FILL);
	vox_iter = vm->surface_vox_list.begin();

	for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
	{
		Vector3d p0, p1, p2, p3, n1, n2, n;
		glBegin(GL_QUADS);

		if ((*vox_iter)->flag_top_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;

			Vector3d p10 = p1 - p0;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_bottom_face)
		{
			p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;

			Vector3d p10 = p1 - p0;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}


		if ((*vox_iter)->flag_left_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;

			Vector3d p10 = p1 - p0;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_right_face)
		{
			p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;

			Vector3d p10 = p1 - p0;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_front_face)
		{
			p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;

			Vector3d p10 = p1 - p0;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if ((*vox_iter)->flag_back_face)
		{
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;

			Vector3d p10 = p1 - p0;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		glEnd();
	}
}


void Renderer::renderClusterStaticPosition(const Cluster* c, float* cluster_color)
{
	if (c->surface_quads.empty())
	{
		return;
	}
	
	glColor4f(cluster_color[0], cluster_color[1], cluster_color[2], 0.3);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);

	for (int i=0; i<c->num_surface_quads; ++i)
	{
		Vector3d p0, p1, p2, p3, n1, n2, n;
				
		p0 = c->surface_quads[i*4 + 0]->static_position;
		p1 = c->surface_quads[i*4 + 1]->static_position;
		p2 = c->surface_quads[i*4 + 2]->static_position;
		p3 = c->surface_quads[i*4 + 3]->static_position;

		Vector3d p10 = p1 - p0;
		Vector3d p30 = p3 - p0;

		n1 = p10.cross(p30);
		n1.normalize();

		Vector3d p12 = p1 - p2;
		Vector3d p32 = p3 - p2;

		n2 = p12.cross(p32);
		n2.normalize();

		n = n1 + n2;
		n.normalize();

		glNormal3d(n[0], n[1], n[2]);

		glVertex3d(p0[0], p0[1], p0[2]);
		glVertex3d(p1[0], p1[1], p1[2]);
		glVertex3d(p2[0], p2[1], p2[2]);
		glVertex3d(p3[0], p3[1], p3[2]);
	}
			
	glEnd();
	/*
	glColor4f(cluster_color[0], cluster_color[1], cluster_color[2], 0.1);
	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_QUADS);
	for (int i=0; i<c->num_surface_quads; ++i)
	{
		Vector3d p0, p1, p2, p3, n1, n2, n;

		p0 = c->surface_quads[i*4 + 0]->static_position;
		p1 = c->surface_quads[i*4 + 1]->static_position;
		p2 = c->surface_quads[i*4 + 2]->static_position;
		p3 = c->surface_quads[i*4 + 3]->static_position;

		Vector3d p10 = p1 - p0;
		Vector3d p30 = p3 - p0;

		n1 = p10.cross(p30);
		n1.normalize();

		Vector3d p12 = p1 - p2;
		Vector3d p32 = p3 - p2;

		n2 = p12.cross(p32);
		n2.normalize();

		n = n1 + n2;
		n.normalize();

		glNormal3d(n[0], n[1], n[2]);

		glVertex3d(p0[0], p0[1], p0[2]);
		glVertex3d(p1[0], p1[1], p1[2]);
		glVertex3d(p2[0], p2[1], p2[2]);
		glVertex3d(p3[0], p3[1], p3[2]);
	}

	glEnd();
	*/

	
}

bool Renderer::isPicked(double x, double y)
{
	y = win_height - y;
	if ((current_mouse_x-x)*(current_mouse_x-x) + (current_mouse_y-y)*(current_mouse_y-y) <= 25.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Renderer::isPicked4Cube(double x, double y)
{
	y = win_height - y;
	if ((current_mouse_x-x)*(current_mouse_x-x) + (current_mouse_y-y)*(current_mouse_y-y) <= 50.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Renderer::renderForce()
{
	//one constraint
	if (p_kernel->p_vox_mesh->active_node)
	{
		if (force_arrow.norm() > 1e-3)
		{
			Vector3d force_arrow_begin = p_kernel->p_vox_mesh->active_node->coordinate
				+ p_kernel->p_vox_mesh->active_node->displacement;
			
			renderArrow(force_arrow[0], force_arrow[1], force_arrow[2],
				force_arrow_begin[0], force_arrow_begin[1], force_arrow_begin[2],
				0.01, 0.0, 0.0, 1.0, 0.6);
		}
	}
	//many constraints
	if(p_kernel->constraintType == Kernel::FORCE_CONSTRAINT)
	{
		int j = p_kernel->level_list.size()-1;
		if (p_kernel->flag_redo)
		{
			if (p_kernel->no_record < p_kernel->force_list.size())
			{
				p_kernel->current_force = p_kernel->force_list[p_kernel->no_record];
				force_arrow = p_kernel->level_list[j]->voxmesh_level->constraint_center + p_kernel->current_force;
			}
		}
		if(p_kernel->current_force.norm() > 1e-3)
		{
			Vector3d force_arrow_begin;
			if (!p_kernel->level_list[j]->voxmesh_level->constraint_node_list.empty())
			{	
				force_arrow_begin = Vector3d::Zero();
				//compute constraint center: force_arrow_begin
				int size_k = p_kernel->level_list[j]->voxmesh_level->constraint_node_list.size();
				for(int k = 0; k < size_k; k ++)
				{
					force_arrow_begin += p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->coordinate;
					force_arrow_begin += p_kernel->level_list[j]->voxmesh_level->constraint_node_list[k]->displacement;
				} 

				force_arrow_begin /= size_k; 
				p_kernel->level_list[j]->voxmesh_level->constraint_center = force_arrow_begin;

				force_arrow_begin = p_kernel->level_list[j]->voxmesh_level->constraint_center;
				//renderArrow(force_arrow[0], force_arrow[1], force_arrow[2],
				//	force_arrow_begin[0], force_arrow_begin[1], force_arrow_begin[2],
				//	0.01, 0.0, 0.0, 0.1, 0.6);
				myArrow->setPosBeginEnd(force_arrow_begin[0], force_arrow_begin[1], force_arrow_begin[2], force_arrow[0], force_arrow[1], force_arrow[2]);
				myArrow->render();
				p_kernel->flag_exportObj4Arrow = true;
			}
			else
				p_kernel->flag_exportObj4Arrow = false;
		}
		else
			p_kernel->flag_exportObj4Arrow = false;

	}
	//one constraint(only for HSM)
	if (p_kernel->p_vox_mesh->constraint_node)
	{
		if (force_arrow.norm() > 1e-3)
		{
			Vector3d force_arrow_begin = p_kernel->p_vox_mesh->constraint_node->coordinate
				+ p_kernel->p_vox_mesh->constraint_node->displacement;

			renderArrow(force_arrow[0], force_arrow[1], force_arrow[2],
				force_arrow_begin[0], force_arrow_begin[1], force_arrow_begin[2],
				0.01, 0.0, 0.0, 1.0, 0.6);
		}
	}
}

void Renderer::renderClusterNode(const Cluster* c, double cube_size)
{
	vector<DuplicatedNode>::const_iterator n_iter = c->node_list.begin();
	for (; n_iter != c->node_list.end(); ++n_iter)
	{
		renderCube(cube_size, n_iter->coordinate(0)+n_iter->displacement(0), 
			n_iter->coordinate(1)+n_iter->displacement(1), 
			n_iter->coordinate(2)+n_iter->displacement(2));
	}
}

void Renderer::initColorTab()
{
	color_tab.clear();
	color_tab.resize(8);
		
	color_tab[0] = new float[3];
	color_tab[0][0] = 200.0/255.0; color_tab[0][1] = 191.0/255.0; color_tab[0][2] = 231.0/255.0;
	
	color_tab[1] = new float[3];
	color_tab[1][0] = 112.0/255.0; color_tab[1][1] = 146.0/255.0; color_tab[1][2] = 190.0/255.0;
	
	color_tab[2] = new float[3];
	color_tab[2][0] = 181.0/255.0; color_tab[2][1] = 230.0/255.0; color_tab[2][2] = 29.0/255.0;

	color_tab[3] = new float[3];
	color_tab[3][0] = 153.0/255.0; color_tab[3][1] = 217.0/255.0; color_tab[3][2] = 234.0/255.0;

	color_tab[4] = new float[3];
	color_tab[4][0] = 239.0/255.0; color_tab[4][1] = 228.0/255.0; color_tab[4][2] = 176.0/255.0;

	color_tab[5] = new float[3];
	color_tab[5][0] = 255.0/255.0; color_tab[5][1] = 201.0/255.0; color_tab[5][2] = 14.0/255.0;

	color_tab[6] = new float[3];
	color_tab[6][0] = 255.0/255.0; color_tab[6][1] = 174.0/255.0; color_tab[6][2] = 201.0/255.0;

	color_tab[7] = new float[3];
	color_tab[7][0] = 185.0/255.0; color_tab[7][1] = 122.0/255.0; color_tab[7][2] = 87.0/255.0;
}

/*
void Renderer::renderClusterHierarchy(const VoxMesh* vm)
{
	if (vm->cluster_hierarchy.empty())
	{
		return;
	}

	// render current parent cluster
	tree<Cluster>::iterator tree_loc = vm->current_parent_cluster;
	if (!vm->cluster_hierarchy.is_valid(tree_loc))
	{
		return;
	}

	double node_size = vm->vox_size * 0.2;
	glColor4f(1.0, 1.0, 1.0, 0.4);
	renderClusterNode(&(*tree_loc), node_size);

	tree<Cluster>::sibling_iterator sib_iter(vm->cluster_hierarchy.child(tree_loc, 0));

	for (; vm->cluster_hierarchy.is_valid(sib_iter); ++ sib_iter)
	{
		glColor3fv(color_tab[vm->cluster_hierarchy.index(sib_iter) % 8]);
		renderClusterNode(&(*sib_iter), vm->vox_size * 0.3);
	}

	glColor4f(0.0, 1.0, 0.0, 0.4);
	vector<Vox*>::iterator vox_iter;
	for (vox_iter=tree_loc->vox_list.begin(); vox_iter!=tree_loc->vox_list.end(); ++vox_iter)
	{
		if ((*vox_iter)->flag_visited)
		{
			continue;
		}
		double x, y, z;
		Vector3d p = (*vox_iter)->vox_center;
		gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &x, &y, &z);

		if (isSelected(x, y))
		{
			renderCube(vm->vox_size * 0.48, p(0), p(1), p(2));
		}
	}
}
*/

void Renderer::renderInvertedCluster()
{
	if (p_kernel->p_vox_mesh->inverted_cluster)
	{
		Cluster* c = p_kernel->p_vox_mesh->inverted_cluster;
		glColor3f(1.0, 0.0, 0.0);
		vector<DuplicatedNode>::const_iterator n_iter = c->node_list.begin();
		for (; n_iter != c->node_list.end(); ++n_iter)
		{
			renderCube(1.0/double(p_kernel->grid_density) * 0.2, n_iter->coordinate(0)+n_iter->displacement(0), 
				n_iter->coordinate(1)+n_iter->displacement(1), 
				n_iter->coordinate(2)+n_iter->displacement(2));
		}

		glColor3f(0.8, 0.4, 0.2);
		n_iter = c->node_list.begin();
		for (; n_iter != c->node_list.end(); ++n_iter)
		{
			renderCube(1.0/double(p_kernel->grid_density) * 0.2, n_iter->coordinate(0), 
				n_iter->coordinate(1), 
				n_iter->coordinate(2));
		}
		
	}
}


void Renderer::renderLevelStaticPosition(const Level * plevel, float * level_color, int showNum1, int showNum2)
{
	if(plevel->voxmesh_level != NULL)
	{
		for(int j = showNum1; j < plevel->voxmesh_level->num_cluster; j ++)
		{
			renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[j], level_color);
			
			if (plevel->voxmesh_level->constraint_node)
			{
				glColor3fv(active_node_color);
				Vector3d p = plevel->voxmesh_level->constraint_node->coordinate + plevel->voxmesh_level->constraint_node->displacement;
				renderCube(cube_size, p(0), p(1), p(2));
			}
			if(j == showNum2)
				break;
		}
	}
}

void Renderer::renderLevelStaticPosition(const Level * plevel, float * level_color)
{
	if(plevel->voxmesh_level != NULL)
	{
		double cube_size = plevel->voxmesh_level->vox_size*0.5*0.2;
		vector<Cluster>::const_iterator c_iter = plevel->voxmesh_level->cluster_list.begin();
		for(; c_iter != plevel->voxmesh_level->cluster_list.end(); ++c_iter)
		{
			if (c_iter->flag_cube_constrained)
				renderClusterStaticPosition(&(*c_iter), active_cube_color);
			else if (c_iter->flag_cube_anchored)
				renderClusterStaticPosition(&(*c_iter), anchor_cube_color);
			else
			{
				if(flag_show_cube_static_constraints || flag_show_cube_active_constraints)
				{
					double wx, wy, wz;
					Vector3d p = c_iter->current_center;

					gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

					if (isSelected(wx, wy))
					{
						if (flag_show_cube_static_constraints)
							renderClusterStaticPosition(&(*c_iter), anchor_cube_color);
						else
							renderClusterStaticPosition(&(*c_iter), active_cube_color);
					}
					else
						renderClusterStaticPosition(&(*c_iter), level_color);
				}
				else
					renderClusterStaticPosition(&(*c_iter), level_color);

			}
			
			// show selection while user draging selecting square
			if (flag_show_constraints )
			{
				glColor3fv(active_node_color);
				vector<Node>::iterator n_i = plevel->voxmesh_level->node_list.begin();
				for (; n_i != plevel->voxmesh_level->node_list.end(); ++n_i)
				{
					double wx, wy, wz;
					Vector3d p = n_i->coordinate + n_i->displacement;

					gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

					if (isSelected(wx, wy))
					{
						renderCube(cube_size, p(0), p(1), p(2));
					}
				}
			}

			//show constraint nodes //revised not showing while doing shape matching
			//if(!plevel->voxmesh_level->constraint_node_list.empty() && (p_kernel->used_simulator == Kernel::VELOCITY_MATCHING || p_kernel->used_simulator == Kernel::MULTIPLE_VELOCITY_MATCHING))
			if((!plevel->voxmesh_level->constraint_node_list.empty()) && (p_kernel->constraintType != Kernel::FORCE_CONSTRAINT))
			{
				vector<Node*>::const_iterator n_iter = plevel->voxmesh_level->constraint_node_list.begin();
				for(; n_iter!=plevel->voxmesh_level->constraint_node_list.end(); ++n_iter)
				{
					glColor4f(0.0, 0.0, 1.0, 0.4);
					Vector3d p = (*n_iter)->prescribed_position;
					renderCube(cube_size, p(0), p(1), p(2));
				}
			}

			//show local wcs
			if(p_kernel->constraintType == Kernel::ORIENTATION_CONSTRAINT && !plevel->voxmesh_level->constraint_node_list.empty())
			{	
				renderLCS(plevel->voxmesh_level->constraint_center + LCS_translation);
			}

			// show selection while user draging selecting square
			if (flag_show_selection)
			{
				glColor3fv(anchor_node_color);
				vector<Node>::iterator n_i = plevel->voxmesh_level->node_list.begin();
				for (; n_i != plevel->voxmesh_level->node_list.end(); ++n_i)
				{
					double wx, wy, wz;
					Vector3d p = n_i->coordinate + n_i->displacement;

					gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

					if (isSelected(wx, wy))
					{
						renderCube(cube_size, p(0), p(1), p(2));
					}
				}
			}
			/*
			// show anchor node
			if (!plevel->voxmesh_level->anchor_node_list.empty())
			{
				vector<Node*>::const_iterator n_iter = plevel->voxmesh_level->anchor_node_list.begin();
				for(; n_iter!=plevel->voxmesh_level->anchor_node_list.end(); ++n_iter)
				{
					glColor3fv(anchor_node_color);
					Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;
					renderCube(cube_size, p(0), p(1), p(2));
				}
			}
			*/
		}
	}
}


void Renderer::renderNeighborStaticPosition(const Level * plevel, int showNumNeighbor)
{
	if(plevel->voxmesh_level != NULL)
	{
		if(showNumNeighbor < plevel->voxmesh_level->num_cluster)
		{
			float color[3] = {0.0, 0.0, 1.0};
			
			renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[showNumNeighbor], color);
			
			color[1] = 1.0;
			color[2] = 0.0;
			Vox * temp = plevel->voxmesh_level->cluster_list[showNumNeighbor].vox_list[0];
			if(temp->back_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->back_neighbor->clusterid], color);
			else
				cout << "back_neighbor" << endl;
			if(temp->bottom_back_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->bottom_back_neighbor->clusterid], color);
			else
				cout << "bottom_back_neighbor" << endl;
			if(temp->bottom_front_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->bottom_front_neighbor->clusterid], color);
			else
				cout << "bottom_front_neighbor" << endl;
			if(temp->bottom_left_back_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->bottom_left_back_neighbor->clusterid], color);
			else
				cout << "bottom_left_back_neighbor" << endl;
			if(temp->bottom_left_front_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->bottom_left_front_neighbor->clusterid], color);
			else
				cout << "bottom_left_front_neighbor" << endl;
			if(temp->bottom_left_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->bottom_left_neighbor->clusterid], color);
			else
				cout << "bottom_left_neighbor" << endl;
			if(temp->bottom_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->bottom_neighbor->clusterid], color);
			else
				cout << "bottom_neighbor" << endl;
			if(temp->bottom_right_back_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->bottom_right_back_neighbor->clusterid], color);
			else
				cout << "bottom_right_back_neighbor" << endl;
			if(temp->bottom_right_front_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->bottom_right_front_neighbor->clusterid], color);
			else
				cout << "bottom_right_front_neighbor" << endl;

			if(temp->bottom_right_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->bottom_right_neighbor->clusterid], color);
			else
				cout << "bottom_right_neighbor" << endl;
			if(temp->left_back_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->left_back_neighbor->clusterid], color);
			else
				cout << "left_back_neighbor" << endl;
			if(temp->left_front_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->left_front_neighbor->clusterid], color);
			else
				cout << "left_front_neighbor" << endl;
			if(temp->left_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->left_neighbor->clusterid], color);	
			else
				cout << "left_neighbor" << endl;
			if(temp->front_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->front_neighbor->clusterid], color);
			else
				cout << "front_neighbor" << endl;
			if(temp->right_back_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->right_back_neighbor->clusterid], color);
			else
				cout << "right_back_neighbor" << endl;
			if(temp->right_front_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->right_front_neighbor->clusterid], color);
			else
				cout << "right_front_neighbor" << endl;
			if(temp->right_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->right_neighbor->clusterid], color);
			else
				cout << "right_neighbor" << endl;

			if(temp->top_back_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->top_back_neighbor->clusterid], color);
			else
				cout << "top_back_neighbor" << endl;
			if(temp->top_front_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->top_front_neighbor->clusterid], color);
			else
				cout << "top_front_neighbor" << endl;
			if(temp->top_left_back_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->top_left_back_neighbor->clusterid], color);
			else
				cout << "top_left_back_neighbor" << endl;
			if(temp->top_left_front_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->top_left_front_neighbor->clusterid], color);
			else
				cout << "top_left_front_neighbor" << endl;
			if(temp->top_left_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->top_left_neighbor->clusterid], color);
			else
				cout << "top_left_neighbor" << endl;
			if(temp->top_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->top_neighbor->clusterid], color);
			else
				cout << "top_neighbor" << endl;
			if(temp->top_right_back_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->top_right_back_neighbor->clusterid], color);
			else
				cout << "top_right_back_neighbor" << endl;
			if(temp->top_right_front_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->top_right_front_neighbor->clusterid], color);
			else
				cout << "top_right_front_neighbor" << endl;
			if(temp->top_right_neighbor)
				renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[temp->top_right_neighbor->clusterid], color);
			else
				cout << "top_right_neighbor" << endl;

			cout << endl;
		}
	}
}

void Renderer::renderParentNeighborStaticPosition(const Level * plevel,  const Level * plevel_parent, int showNumNeighbor)
{
	if(plevel->voxmesh_level != NULL)
	{
		if(showNumNeighbor < plevel->voxmesh_level->num_cluster)
		{
			float color[3] = {0.0, 0.0, 1.0};
			
			renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[showNumNeighbor], color);

			Vox * voxtemp = plevel->voxmesh_level->cluster_list[showNumNeighbor].vox_list[0];

			color[1] = 1.0;
			color[2] = 0.0;
			for(int i = 0; i < 8; i++)
			{
				if(voxtemp->list_near_parentVox[i])
				{
					renderClusterStaticPosition(&plevel_parent->voxmesh_level->cluster_list[voxtemp->list_near_parentVox[i]->clusterid], color);
					cout << i << "th parent neighbor exists" << endl;
				}
				cout << i << "th parent neighbor parameter:" << voxtemp->para_interpolate[i] << endl;
			}
			cout << endl;
		}
	}
}

void Renderer::renderLevelVertex(const Mesh * m, const Level * plevel, int showVertexIndex)
{
	float color[3] = {1.0, 0.0, 0.0};
	if(showVertexIndex < m->node_list.size())
	{
		Vector3d p = m->node_list[showVertexIndex].coordinate;
		glColor3f(color[0], color[1], color[2]);
		renderCube(0.01, p(0), p(1), p(2));

		renderClusterStaticPosition(m->node_list[showVertexIndex].incident_cluster[plevel->level_index], color);
	}
	else
		cout << "Test Error! index of vertex is out of range" << endl;
}

void Renderer::renderNode(float x, float y, float z, float * pColor)
{
	glColor4f(pColor[0], pColor[1], pColor[2], 0.3);
	glVertex3f(x, y, z);
}
/*
void Renderer::renderFLParticle4Region(int idx, float * pColor)
{
	glColor4f(pColor[0], pColor[1], pColor[2], 0.8);
	if (idx >= p_kernel->p_body->latticeLocationsWithExistentRegions.size())
	{
		printf("Error, index of lattice is beyond the range\n");
	}
	else
	{
		LatticeLocation * pl = p_kernel->p_body->latticeLocationsWithExistentRegions[idx];
		GLfloat vx = pl->_node->static_position.x();
		GLfloat vy = pl->_node->static_position.y();
		GLfloat vz = pl->_node->static_position.z();
		renderCube(0.01, vx, vy, vz);
		
		//neighbors
		glColor4f(0.0, 1.0, 0.0, 0.8);
		vector<LatticeLocation *>::const_iterator nb = pl->neighborhood.begin();
		for (; nb != pl->neighborhood.end(); nb++)
		{
			GLfloat nx = (*nb)->_node->static_position.x();
			GLfloat ny = (*nb)->_node->static_position.y();
			GLfloat nz = (*nb)->_node->static_position.z();
			renderCube(0.01, nx, ny, nz);
		}
	}
}
*/

void Renderer::renderLevelVox(const Level * plevel, int showVoxIdx[])
{
	float color[3] = {1.0, 1.0, 0.0};
	int idx = plevel->gridDensity * plevel->gridDensity * plevel->gridDensity * showVoxIdx[3]
		+ plevel->gridDensity * plevel->gridDensity * showVoxIdx[2] + plevel->gridDensity * showVoxIdx[1] + showVoxIdx[0];
	cout << showVoxIdx[3] << showVoxIdx[2] << showVoxIdx[1] << showVoxIdx[0]<< endl;
	if(plevel->voxmesh_level->vox_locator[idx] != NULL)
	{
		int cid = plevel->voxmesh_level->vox_locator[idx]->clusterid;
		renderClusterStaticPosition(&plevel->voxmesh_level->cluster_list[cid], color);
	}
	else
		cout << "No vox for this index!!" << endl;
}

Matrix3d Renderer::rotate( double degrees, const Vector3d& axe)
 {
	 double PI = 3.14159265359;
     Vector3d dir = axe;
     dir.normalize();
     degrees *= (-1.0*PI/180);
     double dCos=cos( degrees );
     double dSin=sin( degrees );
 
     Matrix3d m;
 
     m(0,0) = (float)(( dir.x() * dir.x()) * ( 1.0f - dCos ) + dCos);
     m(0,1) = (float)(( dir.x() * dir.y() ) * ( 1.0f - dCos ) - (dir.z() * dSin));
     m(0,2) = (float)(( dir.x() * dir.z() ) * ( 1.0f - dCos ) + (dir.y() * dSin));
 
     m(1,0) = (float)(( dir.y() * dir.x()) * ( 1.0f - dCos ) + (dir.z() * dSin));
     m(1,1) = (float)(( dir.y() * dir.y() ) * ( 1.0f - dCos ) + dCos);
     m(1,2) = (float)(( dir.y() * dir.z() ) * ( 1.0f - dCos ) - (dir.x()* dSin));
 
     m(2,0) = (float)(( dir.z() * dir.x()) * ( 1.0f - dCos ) - (dir.y() * dSin));
     m(2,1) = (float)(( dir.z() * dir.y() ) * ( 1.0f - dCos ) + (dir.x()* dSin));
     m(2,2) = (float)(( dir.z() * dir.z() ) * ( 1.0f - dCos ) + dCos);
 
     //m(0,3) = m(1,3) = m(2,3) = 0.0f;
     //m(3,0) = m(3,1) = m(3,2) = 0.0f;
     //m(3,3) = 1.0f;
 
  //   Vector4d nc;
	 //nc(0) = n(0);
	 //nc(1) = n(1);
	 //nc(2) = n(2);
	 //nc(3) = 1;

	 //Vector4d nr = nc.transpose() * m;
	 //n(0) = nr(0);
	 //n(1) = nr(1);
	 //n(2) = nr(2);

	// n = m * n;

	 return m;
 }

Vector3d Renderer::getAngularVelocity( double degrees, const Vector3d& axe)
{
	double PI = 3.14159265359;
    Vector3d dir = axe;
    dir.normalize();
    degrees *= (-1.0*PI/180);
	Vector3d angular_velocity = degrees * dir;
	return angular_velocity;
}

void Renderer::renderVoxSurfaceBoundary(Vox * pVox, float * pColor)
{
	glColor3fv(pColor);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Vector3d p0, p1, p2, p3, n1, n2, n;
		glBegin(GL_QUADS);

		if (pVox->flag_top_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			*/
			p0 = pVox->node_0->target_position;
			p1 = pVox->node_3->target_position;
			p2 = pVox->node_2->target_position;
			p3 = pVox->node_1->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if (pVox->flag_bottom_face)
		{
			/*
			p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			*/

			p0 = pVox->node_4->target_position;
			p1 = pVox->node_5->target_position;
			p2 = pVox->node_6->target_position;
			p3 = pVox->node_7->target_position;
			
			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}


		if (pVox->flag_left_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			*/

			p0 = pVox->node_0->target_position;
			p1 = pVox->node_4->target_position;
			p2 = pVox->node_7->target_position;
			p3 = pVox->node_3->target_position;


			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if (pVox->flag_right_face)
		{
			/*
			p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			*/

			p0 = pVox->node_1->target_position;
			p1 = pVox->node_2->target_position;
			p2 = pVox->node_6->target_position;
			p3 = pVox->node_5->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if (pVox->flag_front_face)
		{
			/*
			p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			*/
			
			p0 = pVox->node_2->target_position;
			p1 = pVox->node_3->target_position;
			p2 = pVox->node_7->target_position;
			p3 = pVox->node_6->target_position;
			
			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if (pVox->flag_back_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			*/

			p0 = pVox->node_0->target_position;
			p1 = pVox->node_1->target_position;
			p2 = pVox->node_5->target_position;
			p3 = pVox->node_4->target_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		glEnd();
}
void Renderer::renderVoxSurface(Vox * pVox, float * pColor)
{
	glColor4f(pColor[0], pColor[1], pColor[2], 0.2);
	glPolygonMode(GL_FRONT, GL_FILL);
	Vector3d p0, p1, p2, p3, n1, n2, n;
	glBegin(GL_QUADS);

	if (pVox->flag_top_face)
	{
		/*
		p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
		p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
		p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
		p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
		*/
		p0 = pVox->node_0->target_position;
		p1 = pVox->node_3->target_position;
		p2 = pVox->node_2->target_position;
		p3 = pVox->node_1->target_position;

		Vector3d p10 = p1 - p1;
		Vector3d p30 = p3 - p0;

		n1 = p10.cross(p30);
		n1.normalize();

		Vector3d p12 = p1 - p2;
		Vector3d p32 = p3 - p2;

		n2 = p12.cross(p32);
		n2.normalize();

		n = n1 + n2;
		n.normalize();

		glNormal3d(n[0], n[1], n[2]);

		glVertex3d(p0[0], p0[1], p0[2]);
		glVertex3d(p1[0], p1[1], p1[2]);
		glVertex3d(p2[0], p2[1], p2[2]);
		glVertex3d(p3[0], p3[1], p3[2]);
	}

	if (pVox->flag_bottom_face)
	{
		/*
		p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
		p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
		p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
		p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
		*/

		p0 = pVox->node_4->target_position;
		p1 = pVox->node_5->target_position;
		p2 = pVox->node_6->target_position;
		p3 = pVox->node_7->target_position;

		Vector3d p10 = p1 - p1;
		Vector3d p30 = p3 - p0;

		n1 = p10.cross(p30);
		n1.normalize();

		Vector3d p12 = p1 - p2;
		Vector3d p32 = p3 - p2;

		n2 = p12.cross(p32);
		n2.normalize();

		n = n1 + n2;
		n.normalize();

		glNormal3d(n[0], n[1], n[2]);

		glVertex3d(p0[0], p0[1], p0[2]);
		glVertex3d(p1[0], p1[1], p1[2]);
		glVertex3d(p2[0], p2[1], p2[2]);
		glVertex3d(p3[0], p3[1], p3[2]);
	}


	if (pVox->flag_left_face)
	{
		/*
		p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
		p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
		p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
		p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
		*/

		p0 = pVox->node_0->target_position;
		p1 = pVox->node_4->target_position;
		p2 = pVox->node_7->target_position;
		p3 = pVox->node_3->target_position;

		Vector3d p10 = p1 - p1;
		Vector3d p30 = p3 - p0;

		n1 = p10.cross(p30);
		n1.normalize();

		Vector3d p12 = p1 - p2;
		Vector3d p32 = p3 - p2;

		n2 = p12.cross(p32);
		n2.normalize();

		n = n1 + n2;
		n.normalize();

		glNormal3d(n[0], n[1], n[2]);

		glVertex3d(p0[0], p0[1], p0[2]);
		glVertex3d(p1[0], p1[1], p1[2]);
		glVertex3d(p2[0], p2[1], p2[2]);
		glVertex3d(p3[0], p3[1], p3[2]);
	}

	if (pVox->flag_right_face)
	{
		/*
		p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
		p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
		p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
		p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
		*/
		p0 = pVox->node_1->target_position;
		p1 = pVox->node_2->target_position;
		p2 = pVox->node_6->target_position;
		p3 = pVox->node_5->target_position;

		Vector3d p10 = p1 - p1;
		Vector3d p30 = p3 - p0;

		n1 = p10.cross(p30);
		n1.normalize();

		Vector3d p12 = p1 - p2;
		Vector3d p32 = p3 - p2;

		n2 = p12.cross(p32);
		n2.normalize();

		n = n1 + n2;
		n.normalize();

		glNormal3d(n[0], n[1], n[2]);

		glVertex3d(p0[0], p0[1], p0[2]);
		glVertex3d(p1[0], p1[1], p1[2]);
		glVertex3d(p2[0], p2[1], p2[2]);
		glVertex3d(p3[0], p3[1], p3[2]);
	}

	if (pVox->flag_front_face)
	{
		/*
		p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
		p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
		p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
		p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
		*/

		p0 = pVox->node_2->target_position;
		p1 = pVox->node_3->target_position;
		p2 = pVox->node_7->target_position;
		p3 = pVox->node_6->target_position;

		Vector3d p10 = p1 - p1;
		Vector3d p30 = p3 - p0;

		n1 = p10.cross(p30);
		n1.normalize();

		Vector3d p12 = p1 - p2;
		Vector3d p32 = p3 - p2;

		n2 = p12.cross(p32);
		n2.normalize();

		n = n1 + n2;
		n.normalize();

		glNormal3d(n[0], n[1], n[2]);

		glVertex3d(p0[0], p0[1], p0[2]);
		glVertex3d(p1[0], p1[1], p1[2]);
		glVertex3d(p2[0], p2[1], p2[2]);
		glVertex3d(p3[0], p3[1], p3[2]);
	}

	if (pVox->flag_back_face)
	{
		/*
		p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
		p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
		p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
		p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
		*/
		p0 = pVox->node_0->target_position;
		p1 = pVox->node_1->target_position;
		p2 = pVox->node_5->target_position;
		p3 = pVox->node_4->target_position;

		Vector3d p10 = p1 - p1;
		Vector3d p30 = p3 - p0;

		n1 = p10.cross(p30);
		n1.normalize();

		Vector3d p12 = p1 - p2;
		Vector3d p32 = p3 - p2;

		n2 = p12.cross(p32);
		n2.normalize();

		n = n1 + n2;
		n.normalize();

		glNormal3d(n[0], n[1], n[2]);

		glVertex3d(p0[0], p0[1], p0[2]);
		glVertex3d(p1[0], p1[1], p1[2]);
		glVertex3d(p2[0], p2[1], p2[2]);
		glVertex3d(p3[0], p3[1], p3[2]);
	}

	glEnd();
}
void Renderer::renderVoxSurfaceBoundary4Static(Vox * pVox, float * pColor)
{
	glColor3fv(pColor);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Vector3d p0, p1, p2, p3, n1, n2, n;
		glBegin(GL_QUADS);

		if (pVox->flag_top_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			*/
			p0 = pVox->node_0->static_position;
			p1 = pVox->node_3->static_position;
			p2 = pVox->node_2->static_position;
			p3 = pVox->node_1->static_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if (pVox->flag_bottom_face)
		{
			/*
			p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			*/

			p0 = pVox->node_4->static_position;
			p1 = pVox->node_5->static_position;
			p2 = pVox->node_6->static_position;
			p3 = pVox->node_7->static_position;
			
			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}


		if (pVox->flag_left_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			*/

			p0 = pVox->node_0->static_position;
			p1 = pVox->node_4->static_position;
			p2 = pVox->node_7->static_position;
			p3 = pVox->node_3->static_position;


			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if (pVox->flag_right_face)
		{
			/*
			p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			*/

			p0 = pVox->node_1->static_position;
			p1 = pVox->node_2->static_position;
			p2 = pVox->node_6->static_position;
			p3 = pVox->node_5->static_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if (pVox->flag_front_face)
		{
			/*
			p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
			p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
			p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
			p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
			*/
			
			p0 = pVox->node_2->static_position;
			p1 = pVox->node_3->static_position;
			p2 = pVox->node_7->static_position;
			p3 = pVox->node_6->static_position;
			
			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		if (pVox->flag_back_face)
		{
			/*
			p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
			p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
			p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
			p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
			*/

			p0 = pVox->node_0->static_position;
			p1 = pVox->node_1->static_position;
			p2 = pVox->node_5->static_position;
			p3 = pVox->node_4->static_position;

			Vector3d p10 = p1 - p1;
			Vector3d p30 = p3 - p0;

			n1 = p10.cross(p30);
			n1.normalize();

			Vector3d p12 = p1 - p2;
			Vector3d p32 = p3 - p2;

			n2 = p12.cross(p32);
			n2.normalize();

			n = n1 + n2;
			n.normalize();

			glNormal3d(n[0], n[1], n[2]);

			glVertex3d(p0[0], p0[1], p0[2]);
			glVertex3d(p1[0], p1[1], p1[2]);
			glVertex3d(p2[0], p2[1], p2[2]);
			glVertex3d(p3[0], p3[1], p3[2]);
		}

		glEnd();
}
void Renderer::renderVoxSurface4Static(Vox * pVox, float * pColor)
{
	glColor4f(pColor[0], pColor[1], pColor[2], 0.2);
	glPolygonMode(GL_FRONT, GL_FILL);
	Vector3d p0, p1, p2, p3, n1, n2, n;
	glBegin(GL_QUADS);

	if (pVox->flag_top_face)
	{
		/*
		p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
		p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
		p2 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
		p3 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
		*/
		p0 = pVox->node_0->static_position;
		p1 = pVox->node_3->static_position;
		p2 = pVox->node_2->static_position;
		p3 = pVox->node_1->static_position;

		Vector3d p10 = p1 - p1;
		Vector3d p30 = p3 - p0;

		n1 = p10.cross(p30);
		n1.normalize();

		Vector3d p12 = p1 - p2;
		Vector3d p32 = p3 - p2;

		n2 = p12.cross(p32);
		n2.normalize();

		n = n1 + n2;
		n.normalize();

		glNormal3d(n[0], n[1], n[2]);

		glVertex3d(p0[0], p0[1], p0[2]);
		glVertex3d(p1[0], p1[1], p1[2]);
		glVertex3d(p2[0], p2[1], p2[2]);
		glVertex3d(p3[0], p3[1], p3[2]);
	}

	if (pVox->flag_bottom_face)
	{
		/*
		p0 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
		p1 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
		p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
		p3 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
		*/

		p0 = pVox->node_4->static_position;
		p1 = pVox->node_5->static_position;
		p2 = pVox->node_6->static_position;
		p3 = pVox->node_7->static_position;

		Vector3d p10 = p1 - p1;
		Vector3d p30 = p3 - p0;

		n1 = p10.cross(p30);
		n1.normalize();

		Vector3d p12 = p1 - p2;
		Vector3d p32 = p3 - p2;

		n2 = p12.cross(p32);
		n2.normalize();

		n = n1 + n2;
		n.normalize();

		glNormal3d(n[0], n[1], n[2]);

		glVertex3d(p0[0], p0[1], p0[2]);
		glVertex3d(p1[0], p1[1], p1[2]);
		glVertex3d(p2[0], p2[1], p2[2]);
		glVertex3d(p3[0], p3[1], p3[2]);
	}


	if (pVox->flag_left_face)
	{
		/*
		p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
		p1 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
		p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
		p3 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
		*/

		p0 = pVox->node_0->static_position;
		p1 = pVox->node_4->static_position;
		p2 = pVox->node_7->static_position;
		p3 = pVox->node_3->static_position;

		Vector3d p10 = p1 - p1;
		Vector3d p30 = p3 - p0;

		n1 = p10.cross(p30);
		n1.normalize();

		Vector3d p12 = p1 - p2;
		Vector3d p32 = p3 - p2;

		n2 = p12.cross(p32);
		n2.normalize();

		n = n1 + n2;
		n.normalize();

		glNormal3d(n[0], n[1], n[2]);

		glVertex3d(p0[0], p0[1], p0[2]);
		glVertex3d(p1[0], p1[1], p1[2]);
		glVertex3d(p2[0], p2[1], p2[2]);
		glVertex3d(p3[0], p3[1], p3[2]);
	}

	if (pVox->flag_right_face)
	{
		/*
		p0 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
		p1 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
		p2 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
		p3 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
		*/
		p0 = pVox->node_1->static_position;
		p1 = pVox->node_2->static_position;
		p2 = pVox->node_6->static_position;
		p3 = pVox->node_5->static_position;

		Vector3d p10 = p1 - p1;
		Vector3d p30 = p3 - p0;

		n1 = p10.cross(p30);
		n1.normalize();

		Vector3d p12 = p1 - p2;
		Vector3d p32 = p3 - p2;

		n2 = p12.cross(p32);
		n2.normalize();

		n = n1 + n2;
		n.normalize();

		glNormal3d(n[0], n[1], n[2]);

		glVertex3d(p0[0], p0[1], p0[2]);
		glVertex3d(p1[0], p1[1], p1[2]);
		glVertex3d(p2[0], p2[1], p2[2]);
		glVertex3d(p3[0], p3[1], p3[2]);
	}

	if (pVox->flag_front_face)
	{
		/*
		p0 = (*vox_iter)->node_2->coordinate + (*vox_iter)->node_2->displacement;
		p1 = (*vox_iter)->node_3->coordinate + (*vox_iter)->node_3->displacement;
		p2 = (*vox_iter)->node_7->coordinate + (*vox_iter)->node_7->displacement;
		p3 = (*vox_iter)->node_6->coordinate + (*vox_iter)->node_6->displacement;
		*/

		p0 = pVox->node_2->static_position;
		p1 = pVox->node_3->static_position;
		p2 = pVox->node_7->static_position;
		p3 = pVox->node_6->static_position;

		Vector3d p10 = p1 - p1;
		Vector3d p30 = p3 - p0;

		n1 = p10.cross(p30);
		n1.normalize();

		Vector3d p12 = p1 - p2;
		Vector3d p32 = p3 - p2;

		n2 = p12.cross(p32);
		n2.normalize();

		n = n1 + n2;
		n.normalize();

		glNormal3d(n[0], n[1], n[2]);

		glVertex3d(p0[0], p0[1], p0[2]);
		glVertex3d(p1[0], p1[1], p1[2]);
		glVertex3d(p2[0], p2[1], p2[2]);
		glVertex3d(p3[0], p3[1], p3[2]);
	}

	if (pVox->flag_back_face)
	{
		/*
		p0 = (*vox_iter)->node_0->coordinate + (*vox_iter)->node_0->displacement;
		p1 = (*vox_iter)->node_1->coordinate + (*vox_iter)->node_1->displacement;
		p2 = (*vox_iter)->node_5->coordinate + (*vox_iter)->node_5->displacement;
		p3 = (*vox_iter)->node_4->coordinate + (*vox_iter)->node_4->displacement;
		*/
		p0 = pVox->node_0->static_position;
		p1 = pVox->node_1->static_position;
		p2 = pVox->node_5->static_position;
		p3 = pVox->node_4->static_position;

		Vector3d p10 = p1 - p1;
		Vector3d p30 = p3 - p0;

		n1 = p10.cross(p30);
		n1.normalize();

		Vector3d p12 = p1 - p2;
		Vector3d p32 = p3 - p2;

		n2 = p12.cross(p32);
		n2.normalize();

		n = n1 + n2;
		n.normalize();

		glNormal3d(n[0], n[1], n[2]);

		glVertex3d(p0[0], p0[1], p0[2]);
		glVertex3d(p1[0], p1[1], p1[2]);
		glVertex3d(p2[0], p2[1], p2[2]);
		glVertex3d(p3[0], p3[1], p3[2]);
	}

	glEnd();
}
void Renderer::renderLevelVoxMesh(const Level * plevel)
{
	if(plevel->voxmesh_level == NULL)
		return;

	VoxMesh* vm = plevel->voxmesh_level;
	if (!vm->num_surface_vox)
	{
		return;
	}
	double v_dim = vm->vox_size * 0.5;
	double cube_size = v_dim*0.2;
	//double cube_size = (1.0/20.0)*0.1;



	vector<Vox*>::const_iterator vox_iter = vm->surface_vox_list.begin();
	//cout << "surface vox number " << vm->surface_vox_list.size() << endl;

	for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
	{
		renderVoxSurfaceBoundary((*vox_iter), vox_mesh_color);
	}


	vox_iter = vm->surface_vox_list.begin();
	for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
	{
		if (vm->cluster_list[(*vox_iter)->clusterid].flag_cube_constrained)
			renderVoxSurface((*vox_iter), active_cube_color);
		else if (vm->cluster_list[(*vox_iter)->clusterid].flag_cube_anchored)
			renderVoxSurface((*vox_iter), anchor_cube_color);
		else
		{
			if(flag_show_cube_static_constraints || flag_show_cube_active_constraints)
			{
				double wx, wy, wz;
				Vector3d p = (*vox_iter)->vox_center;

				gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

				if (isSelected(wx, wy))
				{
					if (flag_show_cube_static_constraints)
						renderVoxSurface((*vox_iter), anchor_cube_color);
					else
						renderVoxSurface((*vox_iter), active_cube_color);
				}
				else
					renderVoxSurface((*vox_iter), vox_mesh_color);
			}
			else
				renderVoxSurface((*vox_iter), vox_mesh_color);
		}
	}
		
	// need to differentiate selected and none selected (surface) voxel



	// render selected nodes for anchor nodes
	glColor3fv(anchor_node_color);
	if (flag_show_selection) 
	{
		vector<Node*>::const_iterator n_iter = vm->surface_node_list.begin();

		for (; n_iter!=vm->surface_node_list.end(); ++n_iter)
		{
			double wx, wy, wz;
			/*Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;*/
			Vector3d p = (*n_iter)->target_position;

			gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

			if (isSelected(wx, wy))
			{
				renderCube(cube_size, p(0), p(1), p(2));
			}
		}
	}
	
	// render anchor nodes
	if (!vm->anchor_node_list.empty())
	{
		vector<Node*>::const_iterator n_iter = vm->anchor_node_list.begin();
		for(; n_iter!=vm->anchor_node_list.end(); ++n_iter)
		{
			/*Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;*/
			Vector3d p = (*n_iter)->target_position;
			renderCube(cube_size, p(0), p(1), p(2));
		}
	}
	
	// render selected nodes for constraint nodes
	glColor3fv(active_node_color);
	if (flag_show_constraints) 
	{
		vector<Node*>::const_iterator n_iter = vm->surface_node_list.begin();

		for (; n_iter!=vm->surface_node_list.end(); ++n_iter)
		{
			double wx, wy, wz;
			/*Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;*/
			Vector3d p = (*n_iter)->target_position;

			gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

			if (isSelected(wx, wy))
			{
				renderCube(cube_size, p(0), p(1), p(2));
			}
		}
	}
	
	// render constraint nodes
	if (!vm->constraint_node_list.empty())
	{
		vector<Node*>::const_iterator n_iter = vm->constraint_node_list.begin();
		for(; n_iter!=vm->constraint_node_list.end(); ++n_iter)
		{
			/*Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;*/
			Vector3d p = (*n_iter)->target_position;
			renderCube(cube_size, p(0), p(1), p(2));
		}
	}

	glColor3f(1.0, 0.0, 0.0);
	// render constraint nodes
	if (!vm->another_constraint_node_list.empty())
	{
		vector<Node*>::const_iterator n_iter = vm->another_constraint_node_list.begin();
		for(; n_iter!=vm->another_constraint_node_list.end(); ++n_iter)
		{
			/*Vector3d p = (*n_iter)->coordinate + (*n_iter)->displacement;*/
			Vector3d p = (*n_iter)->target_position;
			renderCube(cube_size, p(0), p(1), p(2));
		}
	}

	// render selected nodes for mass nodes
	glColor3fv(anchor_node_color);
	if (flag_show_mass) 
	{
		vector<Node>::const_iterator n_iter = vm->node_list.begin();

		for (; n_iter != vm->node_list.end(); ++n_iter)
		{
			double wx, wy, wz;
			/*Vector3d p = n_iter->coordinate + n_iter->displacement;*/
			Vector3d p = n_iter->target_position;

			gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

			if (isSelected(wx, wy))
			{
				renderCube(cube_size, p(0), p(1), p(2));
			}
		}
	}

	// render active node
	if (vm->active_node)
	{
		glColor3fv(active_node_color);
		/*Vector3d p = vm->active_node->coordinate + vm->active_node->displacement;*/
		Vector3d p = vm->active_node->target_position;
		renderCube(cube_size, p(0), p(1), p(2));
	}

	//render constraint_node
	if (vm->constraint_node)
	{
		glColor3fv(active_node_color);
		/*Vector3d p = vm->constraint_node->coordinate + vm->constraint_node->displacement;*/
		Vector3d p = vm->constraint_node->target_position;
		renderCube(cube_size, p(0), p(1), p(2));
	}

	//if((!plevel->voxmesh_level->constraint_node_list.empty()) && (!p_kernel->flag_forceNode))
	//{
	//	vector<Node*>::const_iterator n_iter = plevel->voxmesh_level->constraint_node_list.begin();
	//	for(; n_iter!=plevel->voxmesh_level->constraint_node_list.end(); ++n_iter)
	//	{
	//		glColor4f(0.0, 0.0, 1.0, 0.4);
	//		Vector3d p = (*n_iter)->prescribed_position;
	//		renderCube(cube_size, p(0), p(1), p(2));
	//	}
	//}
}

void Renderer::renderLevelVoxMesh4Static(const Level * plevel)
{
	if(plevel->voxmesh_level == NULL)
		return;

	VoxMesh* vm = plevel->voxmesh_level;
	if (!vm->num_surface_vox)
	{
		return;
	}
	double v_dim = vm->vox_size * 0.5;
	double cube_size = v_dim*0.2;
	//double cube_size = (1.0/20.0)*0.1;



	vector<Vox*>::const_iterator vox_iter = vm->surface_vox_list.begin();
	//cout << "surface vox number " << vm->surface_vox_list.size() << endl;

	for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
	{
		if(plevel->voxmesh_level->cluster_list[(*vox_iter)->clusterid].flag_isRendering)
			renderVoxSurfaceBoundary((*vox_iter), vox_mesh_color);
	}


	vox_iter = vm->surface_vox_list.begin();
	for (; vox_iter!=vm->surface_vox_list.end(); ++vox_iter)
	{
		if(plevel->voxmesh_level->cluster_list[(*vox_iter)->clusterid].flag_isRendering)
			renderVoxSurface((*vox_iter), vox_mesh_color);
	}
}

void Renderer::renderSurfaceNode()
{
	// render selected nodes for anchor nodes
	glColor3fv(anchor_node_color);
	int l = p_kernel->level_list.size() - 1;
	vector<Node*>::const_iterator n_iter = p_kernel->level_list[l]->voxmesh_level->surface_node_list.begin();
	double cube_size = p_kernel->level_list[l]->voxmesh_level->vox_size *0.2;

	for (; n_iter != p_kernel->level_list[l]->voxmesh_level->surface_node_list.end(); ++n_iter)
	{
		Vector3d p = (*n_iter)->target_position;
		renderCube(cube_size, p(0), p(1), p(2));
	}
}

void Renderer::setMass(double massValue)
{
	if (flag_show_mass && last_mouse_button == Qt::RightButton)
	{
		char msg[1024];
		int cx = 0;
		for (int i = 0; i < p_kernel->level_list.size(); i++)
		{
			vector<Node>::iterator n_iter = p_kernel->level_list[i]->voxmesh_level->node_list.begin();
			int count = 0;

			for (; n_iter!=p_kernel->level_list[i]->voxmesh_level->node_list.end(); ++n_iter)
			{
				double wx, wy, wz;
				Vector3d p = n_iter->coordinate + n_iter->displacement;

				gluProject(p[0], p[1], p[2], currentmodelview, currentprojection, currentviewport, &wx, &wy, &wz);

				if (isSelected(wx, wy))
				{
					n_iter->mass = massValue;
					vector<DuplicatedNode *>::iterator d_iter = n_iter->duplicates.begin();
					for(; d_iter != n_iter->duplicates.end(); ++d_iter)
					{
						(*d_iter)->mass = massValue;
					}
					count ++;
				}
			}

			vector<Cluster>::iterator c_iter = p_kernel->level_list[i]->voxmesh_level->cluster_list.begin();
			for (; c_iter!=p_kernel->level_list[i]->voxmesh_level->cluster_list.end(); ++c_iter)
			{
				c_iter->computeRestMassCentroid();
				c_iter->computeAQQ();
			}

			cx += sprintf(msg+cx, "Level %d:  %d nodes mass have been set\n", i, count);
			if(count > 0)
				p_kernel->initializeSimulator();
		}
		bottom_right_x = -1.0;
		bottom_right_y = -1.0;
		upper_left_x   = -1.0;
		upper_left_y   = -1.0;
	
		QMessageBox::information(NULL, tr("success"), tr(msg));
	}
}