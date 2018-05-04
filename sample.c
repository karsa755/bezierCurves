// This is a template for assignment 2 of DM6122: 3D Modeling and Reconstruction, which has provided functions for extracting Bezier segments from a B-spline curve.
// NTU
// August 2016
//
// Open a new project "Win32 Console Application" and add sample.c to Source Files
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
//#include <GL/glut.h>
#include "glut.h"
#include <math.h>

#define DEG 50
typedef struct {
	double x, y;      // x, y coordinates of a 2D point
} Point2d;


typedef struct {
	int      degree;   // degree of the B-spline curve
	int      cntNum;   // number of the deBoor points of the B-spline curve
	double   *knots;   // knot vector of the B-spline curve
	Point2d  *cnt;     // control points of the B-spline curve
} Bspline;


// B-spline curve
Bspline  bcr;

// The parameters are used to define a visible window in this application's World Coordinate System.
double	winLLx = 0.0;
double  winLLy = 0.0;
double	winLen = 100.0;

//
int    displayCP = 1;    // flag for whether the control polygon is displayed or not
int    adaptivePlot = 1;    // flag for whether the plot is adaptive or uniform
int    samplingPnt = 0;    // flag for whether the sampling points are displayed or not
int    tessNum = 10;   // number of sampling points
double tessEps = 2.;     // approximation error for tessellation


						 //============================================================
static void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);         // set display-window color to white
	glMatrixMode(GL_PROJECTION);	           // set projection parameters
	gluOrtho2D(winLLx, winLLx + winLen, winLLy, winLLy + winLen);   // set an orthogonal projection
}
int idxOfKnot(double x)
{
	int k;
	for (int i = 0; i < bcr.degree + bcr.cntNum + 2; i++)
	{
		if ((x >= bcr.knots[i]) && (x <= bcr.knots[i + 1]))
		{

			if (x == bcr.knots[i]) return i;
			if (x == bcr.knots[i + 1]) return (i + 1);
			return i;
		}
	}

}


//this is not entirely correct, the knot vector needs to modified.
Point2d deBoor(double x)
{
	double paddedKnot[DEG];
	//int k = idxOfKnot(x);
	//this should be degree + 1.
	Point2d d[DEG];
	for (int i = 0; i < bcr.degree+bcr.cntNum+1; i++)
	{
		if (i == 0)
		{
			for (int j = 0; j < bcr.degree; j++)
			{
				paddedKnot[j] = bcr.knots[0];
			}
		}
		if (i > 0)
		{
			paddedKnot[i + bcr.degree-1] = bcr.knots[i];
		}
		if (i == bcr.degree + bcr.cntNum)
		{
			
			for (int j = i + bcr.degree; j < 3 * bcr.degree-2 + bcr.cntNum + 1; j++)
			{
				paddedKnot[j] = bcr.knots[i];
			}
		}
	}
	
	int k;
	for (int i = 0; i < bcr.degree + bcr.cntNum + 2; i++)
	{
		if ((x >= bcr.knots[i]) && (x <= bcr.knots[i + 1]))
		{

			if (x == bcr.knots[i]) k = i;
			if (x == bcr.knots[i + 1]) k = i;
			k = i;
		}
	}

	for (int i = 0; i < bcr.degree + 1; i++)
	{
		d[i] = bcr.cnt[i + k - bcr.degree];
	}

	for (int r = 1; r < bcr.degree + 1; r++)
	{
		for (int j = bcr.degree; j >= r; j--)
		{

			double a = (x - bcr.knots[j + k - bcr.degree]) / (bcr.knots[j + 1 + k - r] - bcr.knots[j + k - bcr.degree]);
			d[j].x = (1.0 - a) * d[j - 1].x + a * d[j].x;
			d[j].y = (1.0 - a) * d[j - 1].y + a * d[j].y;
		}

	}
	return d[bcr.degree];
}



//============================================================
void uniformRender()
{
	double Uk = bcr.knots[bcr.degree];
	double Un1 = bcr.knots[bcr.cntNum + 1];
	double step = (Un1 - Uk) / (tessNum - 1);
	Point2d P;

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < tessNum; i++)
	{
		double t = Uk + i*step;
		P = deBoor(t);

		glVertex2f(P.x, P.y);
	}
	glEnd();
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < tessNum; i++)
	{
		if (samplingPnt)
		{
			double t = Uk + i*step;
			P = deBoor(t);
			glVertex2f(P.x, P.y);
		}
	}
	glEnd();
}




//============================================================
void extractBezier(Point2d* bez, int ind)
{
	int     i, j;
	int     k;
	double  knots[50];
	Point2d cnt[30];

	k = bcr.degree;

	// copy one segment
	for (i = ind - k, j = 0; i <= ind; i++) {
		cnt[j].x = bcr.cnt[i].x;
		cnt[j].y = bcr.cnt[i].y;
		j++;
	}
	for (i = ind - k, j = 0; i <= ind + k + 1; i++) {
		knots[j] = bcr.knots[i];
		j++;
	}

	// insert knots to make the left end be Bezier end
	while (1) {
		for (i = k - 1; i>0; i--) {
			if (knots[i] < knots[k]) {
				j = i;
				break;
			}
			j = 0;
		}

		if (j == 0) break;

		// update control points
		for (i = 0; i<j; i++) {
			cnt[i].x = ((knots[k + 1 + i] - knots[k]) / (knots[k + i + 1] - knots[i + 1]))*cnt[i].x
				+ ((knots[k] - knots[i + 1]) / (knots[k + i + 1] - knots[i + 1]))*cnt[i + 1].x;
			cnt[i].y = ((knots[k + 1 + i] - knots[k]) / (knots[k + i + 1] - knots[i + 1]))*cnt[i].y
				+ ((knots[k] - knots[i + 1]) / (knots[k + i + 1] - knots[i + 1]))*cnt[i + 1].y;
		}
		// update knots
		for (i = 0; i<j; i++)
			knots[i] = knots[i + 1];
		knots[j] = knots[k];
	}

	// insert knots to make the right end be Bezier end
	while (1) {
		for (i = k + 2; i< k + k + 1; i++) {
			if (knots[i] > knots[k + 1]) {
				j = i;
				break;
			}
			j = 0;
		}

		if (j == 0) break;

		// update control points
		for (i = k; i >= j - k; i--) {
			cnt[i].x = ((knots[k + i] - knots[k + 1]) / (knots[k + i] - knots[i]))*cnt[i - 1].x
				+ ((knots[k + 1] - knots[i]) / (knots[k + i] - knots[i]))*cnt[i].x;
			cnt[i].y = ((knots[k + i] - knots[k + 1]) / (knots[k + i] - knots[i]))*cnt[i - 1].y
				+ ((knots[k + 1] - knots[i]) / (knots[k + i] - knots[i]))*cnt[i].y;
		}
		// update knots
		for (i = k + k + 1; i>j; i--)
			knots[i] = knots[i - 1];
		knots[j] = knots[k + 1];
	}

	// return the Bezier control points
	for (i = 0; i< bcr.cntNum; i++) {
		bez[i].x = cnt[i].x;
		bez[i].y = cnt[i].y;
	}
}

double maxDistance(Point2d* bez, int deg)
{
	//calculate x and y coordinate for the baseline
	double baselineX = bez[deg + 1].x - bez[0].x;
	double baselineY = bez[deg + 1].y - bez[0].y;

	double maxH = -1;
	for (int i = 0; i < deg + 1; i++)
	{
		double seclineX = bez[i].x - bez[0].x;
		double seclineY = bez[i].y - bez[0].y;
		//cross product of baseline X secline
		double cross = ((baselineX * seclineY) - (baselineY * seclineX));
		double h = fabs(cross) / sqrt(baselineX*baselineX + baselineY*baselineY);
		if (maxH < h)
		{

			maxH = h;
		}
	}

	return maxH;
}

void midSubdivide(Point2d* bez, Point2d *leftBez, Point2d* rightBez, int deg)
{
	double t = 0.5;
	leftBez[0].x = bez[0].x;
	leftBez[0].y = bez[0].y;
	rightBez[deg].x = bez[deg].x;
	rightBez[deg].y = bez[deg].y;
	for (int k = 1; k < deg + 1; k++)
	{
		for (int i = 0; i < deg - k + 1; i++)
		{
			//basically de Casteljau algorithm
			bez[i].x = (1 - t) * bez[i].x + t*bez[i + 1].x;
			bez[i].y = (1 - t) * bez[i].y + t*bez[i + 1].y;

		}
		//subdivision of the curve
		leftBez[k].x = bez[0].x;
		leftBez[k].y = bez[0].y;
		rightBez[deg - k].x = bez[deg - k].x;
		rightBez[deg - k].y = bez[deg - k].y;
	}

}
//============================================================
void plotBezier(Point2d* bez, int deg)
{

	double height = maxDistance(bez, deg);
	Point2d leftBez[DEG];
	Point2d rightBez[DEG];

	if (height < tessEps)
	{
		glBegin(GL_LINE_STRIP);
		glVertex2f(bez[0].x, bez[0].y);
		glVertex2f(bez[deg].x, bez[deg].y);
		glEnd();

		if (samplingPnt)
		{
			glPointSize(3.0f);
			glBegin(GL_POINTS);
			glVertex2f(bez[0].x, bez[0].y);
			glVertex2f(bez[deg].x, bez[deg].y);
			glEnd();
		}
		return;
	}
	else
	{
		midSubdivide(bez, leftBez, rightBez, deg);
		plotBezier(leftBez, deg);
		plotBezier(rightBez, deg);


	}

}


//============================================================
void adaptiveRender()
{
	Point2d  bez[30];  // assume the degree is not greater than 29.
	int      i;

	for (i = bcr.degree; i< bcr.cntNum; i++) {
		if (fabs(bcr.knots[i] - bcr.knots[i + 1]) < 0.00001) continue;  // no segment, skip over
		extractBezier(bez, i);        // extract the i-th Bezier curve
		plotBezier(bez, bcr.degree);   // adaptively plot a Bezier curve
	}
}



//============================================================
static void drawCurve(void)
{
	int i;

	glClear(GL_COLOR_BUFFER_BIT);	// clear display window
	glColor3f(1.0, 0.0, 0.0);   // set line segment color to red


								// Draw the control polygon
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(3.0);
	if (displayCP != 0) {
		glBegin(GL_LINE_STRIP);      // display the control polygon
		for (i = 0; i<bcr.cntNum; i++)
			glVertex2f(bcr.cnt[i].x, bcr.cnt[i].y);
		glEnd();

		glPointSize(6.0);            // display the control points
		glBegin(GL_POINTS);
		for (i = 0; i<bcr.cntNum; i++)
			glVertex2f(bcr.cnt[i].x, bcr.cnt[i].y);
		glEnd();
	}

	// Draw the curve
	glLineWidth(2.0);
	if (adaptivePlot) {  // plot adaptively
		glColor3f(0.0, 1.0, 0.0);
		adaptiveRender();

	}
	else {  // plot uniformly
		glColor3f(0.0, 0.0, 1.0);
		uniformRender();
	}


	glFlush();		    // process all openGL routines as quickly as possible
	glutSwapBuffers();  // swap buffers to display the current frame
}

//============================================================
static void idle(void)
{
	drawCurve();
}


//============================================================
static void hotkey(unsigned char k, int x, int y)
{
	// Here we are processing keyboard events.
	switch (k)
	{
	case 27:
		free(bcr.cnt);
		free(bcr.knots);
		exit(0);
		break;

		// Toggle plotting the control polygon
	case 'C':
	case 'c':
		displayCP = !displayCP;
		break;

		// Toggle sampling points
	case 'P':
	case 'p':
		samplingPnt = !samplingPnt;
		break;

		// Toggle adaptive/uniform plotting
	case 'A':
	case 'a':
		adaptivePlot = !adaptivePlot;
		break;

		// Increase tessellation
	case '+':
	case '=':
		if (adaptivePlot) {
			tessEps *= 0.7;
			if (tessEps < 0.8)  tessEps = 0.8;
		}
		else {
			tessNum += 1;
			if (tessNum > 100) tessNum = 100;
		}
		break;

		// Decrease tessellation
	case '-':
	case '_':
		if (adaptivePlot) {
			tessEps *= 1.4;
			if (tessEps > 50)  tessEps = 100;
		}
		else {
			tessNum -= 1;
			if (tessNum < 2) tessNum = 2;
		}
		break;
	}
}

//============================================================
void chooseWindow()
{
	int    i;
	double left, right, bottom, top;


	left = right = bcr.cnt[0].x;
	for (i = 1; i< bcr.cntNum; i++) {
		if (left > bcr.cnt[i].x)  left = bcr.cnt[i].x;
		if (right < bcr.cnt[i].x) right = bcr.cnt[i].x;
	}

	bottom = top = bcr.cnt[0].y;
	for (i = 1; i< bcr.cntNum; i++) {
		if (bottom > bcr.cnt[i].y)  bottom = bcr.cnt[i].y;
		if (top < bcr.cnt[i].y) top = bcr.cnt[i].y;
	}

	winLen = top - bottom;
	if (winLen < right - left) winLen = right - left;

	winLen += 100;
	winLLy = bottom - 50;
	winLLx = left - 50;
}



//============================================================
int readFile(char* filename)
{
	FILE *fp;
	int  i;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		return 0;  // fail to open the file
	}

	fscanf(fp, "%d%d", &(bcr.degree), &(bcr.cntNum));
	bcr.knots = (double *)malloc((bcr.cntNum + bcr.degree + 1) * sizeof(double));
	bcr.cnt = (Point2d *)malloc(bcr.cntNum * sizeof(Point2d));

	for (i = 0; i <= bcr.cntNum + bcr.degree; i++)
		fscanf(fp, "%lf", &(bcr.knots[i]));

	for (i = 0; i< bcr.cntNum; i++)
		fscanf(fp, "%lf%lf", &(bcr.cnt[i].x), &(bcr.cnt[i].y));
	fclose(fp);

	chooseWindow();

	return 1;
}



//============================================================
void main(int argc, char *argv[])
{


	// load the curve from a file
	char filename[20];

	printf("\n Please enter a filename: ");
	scanf("%s", filename);
	if (readFile(filename) == 0)
	{
		printf("not read correctly");
		return;
	}

	// help information
	printf("\n\nB-spline curve plotting\n");
	printf("NTU, September 2006\n");
	printf("\n");
	printf(" ESC      - Quit program\n");
	printf("\n");
	printf(" A/a : Toggle adaptive/uniform plotting (Default adaptive)\n");
	printf(" C/c : Toggle plotting the control polygon (Default On)\n");
	printf(" P/p : Toggle sampling points (Default Off)\n");
	printf(" +   : Increase tessellation\n");
	printf(" -   : Decrease tessellation\n");
	printf("\n");



	// set up graphics window
	glutInit(&argc, argv);                         // initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  // set display mode
	glutInitWindowSize(650, 650);                 // set display window width and height
	glutInitWindowPosition(100, 100);             // set top-left display window position
	glutCreateWindow("Your name: Karim Samim Karim, use +, -, c, a, p, and Esc keys.");

	Init();                        // execute initialization procedure
	glutIdleFunc(idle);            // enables us to make interaction.
	glutDisplayFunc(drawCurve);    // send graphics to display window
	glutKeyboardFunc(hotkey);

	glutMainLoop();                // display everything and wait

}
