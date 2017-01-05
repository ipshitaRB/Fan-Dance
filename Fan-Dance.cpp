// Fan-Dance.cpp : Defines the entry point for the console application.

/*
Author : Ipshita Roy Burman 



-This program uses hierarchical modelling to build a figure made of 
GLU and GLUT objects which are shaded.

-It allows interaction using keyboard/ mouse 

-It also allows to record and playback any animation created using 
interactive methods mentioned above

i - increment angle 
d - decrement angle 






*/

#include "stdafx.h"
#include <gl/glut.h>

#define NULL 0

#define TRACK_BALL_RADIUS 2.0
#define CONE_TORSO_RADIUS 2.5
#define CONE_TORSO_HEIGHT 7.0
#define UPPER_ARM_RADIUS  0.2
#define UPPER_ARM_HEIGHT 5.0
#define FAN_INNER_RADIUS 0.50
#define FAN_OUTER_RADIUS 4.0
#define FAN_START_ANGLE 0.0
#define FAN_SWEEP_ANGLE 130.0
#define LAMP_SHADE_UPPER_RADIUS 1.5
#define LAMP_SHADE_LOWER_RADIUS 3.0
#define LAMP_SHADE_HEIGHT 2.5
#define EYE_INNER_RADIUS 0.10
#define EYE_OUTER_RADIUS 0.35
#define SPHERE_HEAD_RADIUS 1.5

#define MAXEVENTS 300
#define RECORDSIZE 2

//methods to create the body parts of the character object 
void track_ball();
void cone_torso();
void sphere_head();
void left_arm();
void left_fan();
void right_arm();
void right_fan();
void lamp_shade();
void torus_left_eyes();
void torus_right_eyes();

//color arrays 
GLfloat const color_magenta [3] = {1.0, 0.0, 1.0};
GLfloat const color_cyan [3] = {0.0,1.0,1.0};
GLfloat const color_light_grey [3] = {0.75, 0.75, 0.75};
GLfloat const color_white [3] = {1.0 , 1.0, 1.0};
GLfloat const color_black [3] = {0.0, 0.0, 0.0};
GLfloat const color_red [3] = {1.0, 0.0, 0.0};
GLfloat const color_green [3] = {0.0, 1.0, 0.0};
GLfloat const color_blue [3] = {0.0, 0.0, 1.0};
GLfloat const color_brown [3] = { 0.7, 1.0 , 0.6 };
GLfloat const color_yellow [3] = {1.0,1.0,0.0};

// Buffer contains the saved user inputs. Each event is of RECORDSIZE.  
// E.g., if you only need to store the object_id and one transformation value (such as angle) per event, then RECORDSIZE is 2.  

int event_buffer[MAXEVENTS*RECORDSIZE];

// event_ptr is for recording into the event_buffer array.
int event_ptr = 0;

// playback_ptr is for reading/playing back from the event_buffer array.
int playback_ptr=0;

//recordMode and playbackMode are flags used for recording and playback.
int recordMode = 0;
int playbackMode = 0;

//Recorded events are saved to <first nameLastInitial>
FILE *jFile = NULL;
char *fileName = "IpshitaRB.txt";
char *fileName_special = "IpshitaRB_Fan_Dance.txt";

//window size
static int windowWidth = 600;
static int windowHeight = 600;


static GLint angle = 0;

//treenode for hierarchical modelling
typedef struct treenode
{
	GLfloat m[16];
	void (*f)();
	struct treenode *sibling;
	struct treenode *child;
}treenode; 

treenode cone_torso_node, track_ball_node, left_upper_arm_node,
	right_upper_arm_node, left_fan_node, right_fan_node, sphere_head_node, lamp_shade_node, left_torus_eye_node , right_torus_eye_node;

//to traverse the tree
void traverse(treenode* root)
{
	if(root==NULL) return;
	glPushMatrix();
	glMultMatrixf(root->m);
	root->f();
	if(root->child!=NULL) traverse(root->child);
	glPopMatrix();
	if(root->sibling!=NULL) traverse(root->sibling);
}

//objects for drawing style
GLUquadricObj *fill, *line ;

static GLfloat theta[7] = {0.0,0.0,90.0,0.0,-90.0,0.0,0.0}; /* initial joint angles */

//draws the ball used for locomotion by the robot

void track_ball()
{
	glColor3fv(color_magenta);
	glPushMatrix();

	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidSphere(TRACK_BALL_RADIUS,30,30);
	glPopMatrix();
}

//draws the cone body of the robot
//using glut wire cone

void cone_torso()
{
	glColor3fv(color_magenta);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);//making parallel to y axis
	glutSolidCone(CONE_TORSO_RADIUS,CONE_TORSO_HEIGHT,36,36);
	glPopMatrix();
}

//to draw the left arm
void left_arm()
{
	glColor3fv(color_light_grey);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);//TODO also might have to add translate
	gluCylinder(fill,UPPER_ARM_RADIUS, UPPER_ARM_RADIUS, UPPER_ARM_HEIGHT,10,10);
	glPopMatrix();
}

//to draw the right arm
void right_arm()
{
	glColor3fv(color_light_grey);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);//TODO also might have to add translate
	gluCylinder(fill,UPPER_ARM_RADIUS, UPPER_ARM_RADIUS, UPPER_ARM_HEIGHT,10,10);
	glPopMatrix();
}


//draws the right or left  fan using partial Disk
void left_fan()
{

	glColor3fv(color_brown);
	glPushMatrix();



	gluPartialDisk(line, FAN_INNER_RADIUS, FAN_OUTER_RADIUS, 13 , 2, FAN_START_ANGLE, FAN_SWEEP_ANGLE); //obj,0.05,0.4,13,2,0.0,130.0);

	glPopMatrix();

}

//draws the right fan using partial Disk
void right_fan()
{
	glColor3fv(color_brown);
	glPushMatrix();




	gluPartialDisk(line, FAN_INNER_RADIUS, FAN_OUTER_RADIUS, 13 , 2, FAN_START_ANGLE, FAN_SWEEP_ANGLE); //obj,0.05,0.4,13,2,0.0,130.0);

	glPopMatrix();

}


//draws the lamp shade cum hat 
//using glu wire cylinder

void lamp_shade()
{

	glColor3fv(color_yellow);
	glPushMatrix();

	glRotatef(45.0,1.0,0.0,0.0);
	gluCylinder(line,LAMP_SHADE_UPPER_RADIUS, LAMP_SHADE_LOWER_RADIUS, LAMP_SHADE_HEIGHT, 10, 5);// 0.25, 0.4, 0.4, 10, 10);
	glPopMatrix();
}


//draws the left  eyes
//using glut wire torus

void left_eye()
{
	glColor3fv(color_cyan);
	glPushMatrix();

	glutSolidTorus(EYE_INNER_RADIUS, EYE_OUTER_RADIUS,18, 18); //glutWireTorus(0.01,0.035,18,18);

	glPopMatrix();
}

//draws the left  eyes
//using glut wire torus
void right_eye()
{
	glColor3fv(color_cyan);
	glPushMatrix();
	glutSolidTorus(EYE_INNER_RADIUS, EYE_OUTER_RADIUS,18, 18); //glutWireTorus(0.01,0.035,18,18);

	glPopMatrix();
}

//sphere head using glut sphere
void sphere_head()
{
	glColor3fv(color_white);
	glPushMatrix();

	glRotatef(90.0, 1.0, 0.0, 0.0);//making parallel to y axis
	glutSolidSphere(SPHERE_HEAD_RADIUS, 36, 36);//0.20,36,36);
	glPopMatrix();
}



//initializing all variables and tree nodes
void init(){

	fill = gluNewQuadric();
	gluQuadricDrawStyle(fill, GLU_FILL);

	line = gluNewQuadric();
	gluQuadricDrawStyle(line, GLU_LINE);

	//tree set up
	glLoadIdentity();

	glGetFloatv(GL_MODELVIEW_MATRIX,cone_torso_node.m);
	cone_torso_node.f = cone_torso;
	cone_torso_node.sibling = NULL;
	cone_torso_node.child =  &sphere_head_node;


	glLoadIdentity();
	glTranslatef(0.0, CONE_TORSO_HEIGHT+SPHERE_HEAD_RADIUS, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX,sphere_head_node.m);
	sphere_head_node.f = sphere_head;
	sphere_head_node.sibling = &left_upper_arm_node;
	sphere_head_node.child =  &lamp_shade_node;


	glLoadIdentity();
	glTranslatef(0.0,SPHERE_HEAD_RADIUS,-SPHERE_HEAD_RADIUS);
	glGetFloatv(GL_MODELVIEW_MATRIX,lamp_shade_node.m);
	lamp_shade_node.f = lamp_shade;
	lamp_shade_node.sibling = &left_torus_eye_node;
	lamp_shade_node.child =  NULL;


	glLoadIdentity();
	glTranslatef(-0.8,0.0,SPHERE_HEAD_RADIUS+0.1);
	glGetFloatv(GL_MODELVIEW_MATRIX,left_torus_eye_node.m);
	left_torus_eye_node.f = left_eye;
	left_torus_eye_node.sibling = &right_torus_eye_node;
	left_torus_eye_node.child =  NULL;


	glLoadIdentity();
	glTranslatef(0.8,0.0,SPHERE_HEAD_RADIUS+0.1);
	glGetFloatv(GL_MODELVIEW_MATRIX,right_torus_eye_node.m);
	right_torus_eye_node.f = right_eye;
	right_torus_eye_node.sibling = NULL;
	right_torus_eye_node.child =  NULL;


	glLoadIdentity();
	glTranslatef(-0.33*CONE_TORSO_RADIUS,0.66*CONE_TORSO_HEIGHT,0.0);
	glRotatef(theta[2], 0.0,0.0,1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX,left_upper_arm_node.m);
	left_upper_arm_node.f = left_arm;
	left_upper_arm_node.sibling = &right_upper_arm_node;
	left_upper_arm_node.child =  &left_fan_node;

	glLoadIdentity();
	glTranslatef(+0.33*CONE_TORSO_RADIUS,0.66*CONE_TORSO_HEIGHT,0.0);
	glRotatef(theta[4], 0.0,0.0,1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX,right_upper_arm_node.m);
	right_upper_arm_node.f = right_arm;
	right_upper_arm_node.sibling = &track_ball_node;
	right_upper_arm_node.child =  &right_fan_node;


	glLoadIdentity();
	glTranslatef(0.0,UPPER_ARM_HEIGHT,0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX,left_fan_node.m);
	left_fan_node.f = left_fan;
	left_fan_node.sibling = NULL;
	left_fan_node.child =  NULL;

	glLoadIdentity();
	glTranslatef(0.0,UPPER_ARM_HEIGHT,0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX,right_fan_node.m);
	right_fan_node.f = right_fan;
	right_fan_node.sibling = NULL;
	right_fan_node.child =  NULL;


	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX,track_ball_node.m);
	track_ball_node.f = track_ball;
	track_ball_node.sibling = NULL;
	track_ball_node.child =  NULL;

	GLfloat mat_specular[] = {1.0, 1.0, 0.5 , 1.0 };
	GLfloat mat_diffuse[] = {0.0 , 0.0, 0.5 ,1.0};
	GLfloat mat_ambient[] = {0.0, 0.0, 0.5, 1.0 };
	GLfloat mat_shininess = 80.0;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialf(GL_FRONT, GL_AMBIENT, mat_shininess);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);


}


int first_time = 0;

GLfloat z_position = 0.0;
//display method
void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//init();
	glLoadIdentity();
	glTranslatef(0.0,-3.0,z_position);


	//start with torso cone as it has maximum connected parts
	traverse(&cone_torso_node);

	if(first_time == 0){
		printf("Right Click for menu options . Thank you \n");
		first_time = 1;
	}

	glutSwapBuffers();
}

int obj_id;
//plays the saved records
void animate(int obj_id){
	switch(obj_id)
	{

	case 1 :
		glLoadIdentity();
		glRotatef(theta[obj_id], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX,cone_torso_node.m);
		break;

	case 2 :
		glLoadIdentity();
		glTranslatef(0.0,(CONE_TORSO_HEIGHT+SPHERE_HEAD_RADIUS),0.0);
		glRotatef(-90.0,1.0,0.0,0.0);
		glRotatef(theta[obj_id], 1.0, 0.0, 0.0);

		glGetFloatv(GL_MODELVIEW_MATRIX,sphere_head_node.m);
		break;

	case 3:
		glLoadIdentity();
		glTranslatef(+0.33*CONE_TORSO_RADIUS,0.66*CONE_TORSO_HEIGHT,0.0);
		glRotatef(theta[4], 0.0,0.0,1.0);
		glRotatef(theta[obj_id], 0.0, 0.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX,right_upper_arm_node.m);
		break;

	case 4:
		glLoadIdentity();

		glLoadIdentity();
		glTranslatef(0.0,UPPER_ARM_HEIGHT,0.0);
		glRotatef(90.0, 0.0,1.0,0.0);
		glRotatef(theta[obj_id], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX,right_fan_node.m);
		break;

	case 5:
		glLoadIdentity();
		glTranslatef(-0.33*CONE_TORSO_RADIUS,0.66*CONE_TORSO_HEIGHT,0.0);
		glRotatef(theta[2], 0.0,0.0,1.0);
		glRotatef(theta[obj_id], 0.0, 0.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX,left_upper_arm_node.m);
		break;

	case 6:
		glLoadIdentity();

		glLoadIdentity();
		glTranslatef(0.0,UPPER_ARM_HEIGHT,0.0);

		glRotatef(theta[obj_id], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX,left_fan_node.m);

		break;




	}

	glutPostRedisplay();
}

//The timer function
void timerFunc(int val)
{
	// Check if playback_ptr has reached the last event
	if(playback_ptr<event_ptr)
	{	
		obj_id = event_buffer[playback_ptr++];
		angle = event_buffer[playback_ptr++];


		// Update the object's transformation value
		switch(obj_id)
		{
		case 0:
				z_position = angle;
		case 1:
			theta[1] = angle;
			break;
		case 2:
			theta[2] = angle;
			break;
		case 3:
			theta[3]= angle;
			break;
		case 4:
			theta[4] = angle;
			break;
		case 5:
			theta[5] = angle;
			break;
		case 6:
			theta[6]= angle;
			break;

		}

		

		animate(obj_id);


		// Call TimerFunc to read another event
		glutTimerFunc(150, timerFunc, 1);		
	}
	else
	{
		playback_ptr=0;
	}
}





//Reset resets all the transformation values and update the screen.
void reset()
{
	z_position = 0.0;
	theta[0] = 0.0;
	theta[1] = 0.0;
	theta[2] = 90.0;
	theta[3] = 0.0;
	theta[4] = -90.0;
	theta[5] = 0.0; 
	theta[6] = 0.0;
	init();
	display();
}

int option;

//popup menu function for recording and playing 
void menu(int id){
	option = 0;
	if(id <14 ) angle=id;
	if(id ==14 ) exit(0);

	if(id>= 1 && id <= 6){
		obj_id = id;
		printf("press i or d to increase or decrease the angles\n");
	}
	else 

		if(id>= 7 && id <= 13){
			option = id;

		}
		else if(id ==14 ) 
			exit(0);

	glPushMatrix();
	switch(option)
	{

	case 7 : //begin recording. Set recordMode
		//reset();
		recordMode = 1;
		playbackMode = 0;
		printf("Recording... \n");
		event_ptr=0;
		break;

	case 8 : //stop recording. Reset recordMode.
		if(recordMode == 1)
		{
			recordMode = 0;
			printf("Recording stopped. \n");
		}			
		break;

	case 10: //Load file. Reset everything and load the contents of the file into the buffer.
		recordMode = 0;
		playbackMode = 0;

		event_ptr=0;
		playback_ptr=0;
		reset();

		printf("Loading file %s\n", fileName);

		jFile = fopen(fileName, "r");
		if ( jFile == NULL ) {
			printf("Warning: Could not open %s\n", fileName);		
			playbackMode = 0;
		}
		else {
			// Store the events to event_buffer
			while((fscanf(jFile, "%d ", &event_buffer[event_ptr])) != EOF)
			{
				event_ptr++;
			}
			fclose(jFile);
			playbackMode = 1;
		}
		break;



	case 11: //Playback 
		if(playbackMode==1)
		{
			//reset();
			glutTimerFunc(4,timerFunc,1);		
		}
		break;

	case 9: //Save file.
		recordMode = 0;
		playbackMode = 0;

		jFile = fopen(fileName, "w");
		if (jFile == NULL) 
		{
			printf("Warning: Could not open %s\n", fileName);
		}
		else {
			for(int j=0;j<event_ptr;j++)
				fprintf(jFile, "%d ", event_buffer[j]);
			fclose(jFile);
			printf("\nEvents saved in %s\n", fileName);
		}
		playback_ptr=0;
		break;

	case 12:
		recordMode = 0;
		playbackMode = 0;

		event_ptr=0;
		playback_ptr=0;
		reset();

		printf("Loading file %s\n", fileName_special);

		jFile = fopen(fileName_special, "r");
		if ( jFile == NULL ) {
			printf("Warning: Could not open %s\n", fileName_special);		
			playbackMode = 0;
		}
		else {
			// Store the events to event_buffer
			while((fscanf(jFile, "%d ", &event_buffer[event_ptr])) != EOF)
			{
				event_ptr++;
			}
			fclose(jFile);
			playbackMode = 1;
		}

		printf("Presenting ... The Fan Dance \n");
		if(playbackMode==1)
		{
			//reset();
			glutTimerFunc(4,timerFunc,1);		
		}
		break;

	case 13: //Reset everything.

		reset();
		init();
		glutPostRedisplay();
		break;
	}
	glPopMatrix();
	glutPostRedisplay();

}



//key input function
void key(unsigned char key, int x, int y)
{
	/* Process keyboard inputs */

	int w, h;

	switch (key) {
	case 'i':
		if(angle == 0){
			z_position += 1.5;
			if(recordMode == 1){
				event_buffer[event_ptr++] = obj_id;
				event_buffer[event_ptr++] = z_position;
			}
		}else{
			theta[angle] += 5.0;
			if( theta[angle] > 360.0 ) theta[angle] -= 360.0;
			if(recordMode == 1)
			{		
				event_buffer[event_ptr++] = obj_id;
				event_buffer[event_ptr++] = theta[angle];
			}}


		break;

	case 'd':
		if(angle == 0){
			z_position -= 1.5;
			if(recordMode == 1){
				event_buffer[event_ptr++] = obj_id;
				event_buffer[event_ptr++] = z_position;
			}
		}else{
			theta[angle] -= 5.0;
			if( theta[angle] < 360.0 ) theta[angle] += 360.0;
			if(recordMode == 1)
			{		
				event_buffer[event_ptr++] = obj_id;
				event_buffer[event_ptr++] = theta[angle];
			}}
		break;

	default:
		exit(0);
		break;
	}

	switch(angle)
	{

	case 1 :
		glLoadIdentity();
		glRotatef(theta[angle], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX,cone_torso_node.m);
		break;

	case 2 :
		glLoadIdentity();
		glTranslatef(0.0,(CONE_TORSO_HEIGHT+SPHERE_HEAD_RADIUS),0.0);
		glRotatef(-90.0,1.0,0.0,0.0);
		glRotatef(theta[angle], 1.0, 0.0, 0.0);

		glGetFloatv(GL_MODELVIEW_MATRIX,sphere_head_node.m);
		break;

	case 3:
		glLoadIdentity();
		glTranslatef(+0.33*CONE_TORSO_RADIUS,0.66*CONE_TORSO_HEIGHT,0.0);
		glRotatef(theta[4], 0.0,0.0,1.0);
		glRotatef(theta[angle], 0.0, 0.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX,right_upper_arm_node.m);
		break;

	case 4:
		glLoadIdentity();

		glLoadIdentity();
		glTranslatef(0.0,UPPER_ARM_HEIGHT,0.0);
		glRotatef(90.0, 0.0,1.0,0.0);
		glRotatef(theta[angle], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX,right_fan_node.m);
		break;

	case 5:
		glLoadIdentity();
		glTranslatef(-0.33*CONE_TORSO_RADIUS,0.66*CONE_TORSO_HEIGHT,0.0);
		glRotatef(theta[2], 0.0,0.0,1.0);
		glRotatef(theta[angle], 0.0, 0.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX,left_upper_arm_node.m);
		break;

	case 6:
		glLoadIdentity();

		glLoadIdentity();
		glTranslatef(0.0,UPPER_ARM_HEIGHT,0.0);

		glRotatef(theta[angle], 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX,left_fan_node.m);

		break;




	}

	glutPostRedisplay();
}



//resize function
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-10.0, 10.0, -10.0 * (GLfloat) h / (GLfloat) w,
		10.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
	else
		glOrtho(-10.0 * (GLfloat) w / (GLfloat) h,
		10.0 * (GLfloat) w / (GLfloat) h, 0.0, 10.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//first method to be called
void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize(windowWidth	, windowHeight);
	glutCreateWindow("Animated robot");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);

	glutCreateMenu(menu);
	glutAddMenuEntry("move forward/backward",0);
	glutAddMenuEntry("body", 1);
	glutAddMenuEntry("head", 2);
	glutAddMenuEntry("right upper arm", 3);
	glutAddMenuEntry("right fan", 4);
	glutAddMenuEntry("left upper arm", 5);
	glutAddMenuEntry("left fan", 6);
	glutAddMenuEntry("begin recording", 7);
	glutAddMenuEntry("stop recording", 8);
	glutAddMenuEntry("save recording", 9);
	glutAddMenuEntry("load recording", 10);
	glutAddMenuEntry("play recording", 11);
	glutAddMenuEntry("play Fan Dance animation", 12);
	glutAddMenuEntry("reset to original position", 13);
	glutAddMenuEntry("quit", 14);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}


