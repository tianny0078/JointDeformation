// Renderer.h: interface for the Renderer class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __RENDERER_H
#define __RENDERER_H

// QT includes
#include <QtGui>
#include <QtOpenGL>
#include <QGLWidget>
#include <QElapsedTimer>

#include <stack>

#include <QMouseEvent>
#include <QMetaEnum> // in order to get the value of key pressed
#include <sstream>

#include "Camera.h"
#include "Kernel.h"
#include "GL/freeglut.h"
#include <vector>
#include <stdio.h>
#include "cubeTool.h"


#define MIN_CUBE_SIZE		0.005

#define DOF					5.0f
#define CAMERA_VIEW_DISTANCE 6.0
#define CAMERA_ROT_DISTANCE 6.0


#ifndef GL_SEPARATE_SPECULAR_COLOR
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#endif

#ifndef GL_LIGHT_MODEL_COLOR_CONTROL
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#endif


typedef void (__stdcall *GluQuadricCallbackType)();

using namespace std;

class Renderer : public QGLWidget
{
	Q_OBJECT
	
public:
	signals:
	void	appendCluster();

	
public:
	typedef enum {REGULAR, TREE_SETTING} Render_mode;

	Renderer(QWidget *parent = 0);
	~Renderer();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;
					
	// Initialization functions
	void initLighting();
	void initQuadrics();
	void initCamera();
	void initColorTab();
	
	// rendering 
	void renderMesh(const Mesh* m);
	void renderGrid();
	void renderVoxMesh(const VoxMesh* vm);
	void renderVoxMesh(const VoxMesh* vm, float* specified_color);
	void renderClusterStaticPosition(const Cluster* c, float* cluster_color);
	void renderClusterNode(const Cluster* c, double cube_size);												
	void renderClusterHierarchy(const VoxMesh* vm);
	void renderInvertedCluster();

	//rendering for level
	void renderLevelVoxMesh(const Level * plevel);
	void renderLevelStaticPosition(const Level * plevel, float * level_color, int showNum1, int showNum2);
	void renderLevelStaticPosition(const Level * plevel, float * level_color);
	void renderNeighborStaticPosition(const Level * plevel, int showNumNeighbor);
	void renderParentNeighborStaticPosition(const Level * plevel, const Level * plevel_parent, int showNumNeighbor);
	void renderLevelVertex(const Mesh * m, const Level * plevel, int showVertexIndex);
	void renderLevelVox(const Level * plevel, int showVoxIdx[]);
	void renderVoxSurfaceBoundary(Vox * pVox, float * pColor);
	void renderVoxSurface(Vox * pVox, float * pColor);
	
	
	//void renderMeshForSelectAnchorNode(const Mesh* m);
	void renderWCS(float shift);
	void renderLCS(Vector3d p);
	void renderStatus();
	void renderCube(double size, double x, double y, double z);
	// the qt renderText clear stencil buffer which disables line smoothing
	void renderChar(const char *text, void *font);											
	void renderChar(double x, double y, double z, const char *text, void *font);
	
	void renderSelectSquare(int x1, int y1, int x2, int y2);
	void renderForce();


	// aux render function
		
	//void RenderGrid(float* color);
	//void RenderFixedCoord(float shift);
	
	void renderArrow(float R, float G, float B, float size);
	void renderArrow(double x0, double y0, double z0, double x1, double y1, double z1, double size, float R, float G, float B, float A);
	void renderCylinder(double x0, double y0, double z0, double x1, double y1, double z1, double size, float R, float G, float B, float A);

	//void RenderComponentBoundaryBox();
	//void RenderComponentBoundaryBox(int c_idx, float R, float G, float B);
	//void RenderConstrainedPosition();
	
	void renderCircle(double x, double y, double z, double n1, double n2, double n3, double r, double color_r, double color_g, double color_b, double color_a);
	void renderCircle(Vector3d c, Vector3d n, double r, double color_r, double color_g, double color_b, double color_a);
	

	// return whether a windows position is within the selection quad
	bool isSelected(double x, double y);				

	// return whether a position is picked
	bool isPicked(double x, double y);
	bool isPicked4Cube(double x, double y);
	
	// get current selected nodes
	//bool GetCurrentSelectedNodes(vector<int> &onodeList, 
	//	vector<int> &otetlist, int ComponentIdx, int& numInternalNode, int& numBoundaryNode);

	// camera
	/*void SaveCamera(const char* filename);
	void LoadCamera(const char* filename);
	void UndoCamera();*/

	// mouse manipulation function
	void rotateCamera(double ox, double oy, double nx, double ny);

	void translateCamera(double dx, double dy, double dz);

	void initializeGL();

	void paintGL();

	void resizeGL(int width, int height);

	void updateDist(float dx, float dy, float dz);

	void mousei2d(int x, int y, double *xf, double *yf);

	void mousePressEvent(QMouseEvent *event);

	void mouseMoveEvent(QMouseEvent *event);

	void mouseDoubleClickEvent(QMouseEvent *event);

	void mouseReleaseEvent(QMouseEvent *event);

	void keyPressEvent(QKeyEvent *event);

	void keyReleaseEvent(QKeyEvent *event);

	//void getSelectedNode(const T4Mesh* tet_mesh, vector <int>& node_list);

	Matrix3d rotate( double degrees, const Vector3d& axe);
	Vector3d getAngularVelocity( double degrees, const Vector3d& axe);

	void setMass(double massValue);


public:


	Render_mode current_render_mode;

	bool flag_initialized;
	bool flag_show_grid;
	bool flag_show_mesh;
	bool flag_show_vox;
	bool flag_show_target;
	bool flag_show_selection;
	bool flag_simulating;
	//for test
	bool flag_show_test;
	//for point / orientation constraint
	bool flag_show_constraints;
	//bool flag_position_constraint;
	int flag_axis;// 1 is x, 2 is y, 3 is z
	bool flag_show_mass;
	bool flag_cube_operation;
	bool flag_show_cube_static_constraints;
	bool flag_show_cube_active_constraints;
	
	// colors
	QColor background_color;

	float anchor_node_color[3];
	float active_node_color[3];
	float node_color[3];
	float vox_mesh_color[3];
	float grid_color[3];
	float status_color[3];
	float render_mesh_color[3];
	float anchor_cube_color[3];
	float active_cube_color[3];
	
	vector<float*> color_tab;
		
	// GL info
	const GLubyte *gl_version;					// OpenGL Version
	const GLubyte *gl_vendor;					// OpenGL Vendor
	const GLubyte *gl_renderer;					// OpenGL Renderer
	const GLubyte *gl_extensions;				// OpenGL Extensions
	const GLubyte *glu_extensions;				// GLU Extensions
	//bool initialized;

	// Camera info
	Camera theCamera;							// The camera used for rendering mesh
	Camera wcsCamera;							// The camera used for rendering WCS

	//Camera coordCamera;						// The fixed coordinate camera
	double rotate_dist;							// How far the center of the trackball is from the camera
	double view_dist;							// How far the center of the world is from the camera

	// Light info
	GLfloat light_pos[4];						// Position of light source in world coordinate
	//GLfloat shadow_mat[4][4];					// Shadow matrix

	// Input device info
	int	current_mouse_x, current_mouse_y;		// current mouse windows coordinate
	int	last_mouse_x, last_mouse_y;				// last most windows coordinate

	double current_mouse_xd, current_mouse_yd;	// current normalized mouse coordinate
	double last_mouse_xd, last_mouse_yd;		// last normalized mouse coordinate

	int current_mouse_button;
	int	last_mouse_button;						// 0 --- left, 1 --- right
			
	unsigned int win_width, win_height;			// Width and Height of current window

	// for selection square
	double upper_left_x, upper_left_y;
	double bottom_right_x, bottom_right_y;
	
	// Rendering options
	GLuint quadric_list;						// Call-list for quadric
			
	// matrix info. (currently only when the rendering is under selection mode will get the matrix)
	// getting matrix could be slow
	GLint    currentviewport[4];
	GLdouble currentmodelview[16];
	GLdouble currentprojection[16];
	
	Kernel* p_kernel;

	float cube_size;							// cube is used for highlight some position (for its rendering efficiency)
		
	vector<int> current_selected_node;	

	Vector3d force_arrow;

	int showNum1;
	int showNum2;
	int showNumNeighbor;
	int showNumParentNeighbor;
	int showVox4Vertex;
	int showVox4Idx[4];

	Vector3d LCS_x;
	Vector3d LCS_y;
	Vector3d LCS_z;

	Matrix3d LCS_rotation;
	Vector3d LCS_translation;
	cubeTool * tool;
};

#endif
