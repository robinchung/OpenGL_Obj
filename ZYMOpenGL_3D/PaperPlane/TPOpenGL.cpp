/********************************************************************************
*																				*
*	Description:	be used for OpenGL											*
*	Author	   :	Yingmou.Zhong												*
*	Date	   :    2016-06-20													*
*																				*
*********************************************************************************/
#include "StdAfx.h"
#include "TPOpenGL.h"
#include "ObjModelOpenGL.h"
#include "stdio.h"

#ifndef YMTEST
#ifndef YMTESTMOVESTAR
bool twinkle;//跟踪闪烁效果是否启用
const int StarNum = 50;//绘制的星星数
typedef struct//为星星构造一个结构体
{
	int r,g,b;
	GLfloat dist;//星星距离中心的距离
	GLfloat angle; //当前星星所处的角度
}stars;
stars star[StarNum];
GLfloat zoom = -6.0f;
GLfloat tilt = -90.0f;
GLfloat	spin;	
GLuint loop;
#endif
#endif

ObjModelOpenGL*	pobj = new ObjModelOpenGL();
int TPOpenGL::TextureCount=0;//初始化纹理个数记录
int TPOpenGL::InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glewInit();//必须要有，否则glew无法运行
#ifndef YMTEST//测试自己写的加载obj格式模型
	pobj->Import3DObjectFile(_3DMODELOBJPATH);
#endif
	glGenTextures(1,&m_ShadeTexture);////////////////////////////////////3d纹理
	LoadShadeTexture("3DModel/default.shd");
	if(!LoadGLTextures("PictureRes/testRes/Glass.bmp"))//存入一个纹理
		return false;
	if(!LoadGLTextures("PictureRes/testRes/Star.bmp"))//存入第二个纹理
		return false;
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glShadeModel(GL_SMOOTH);
	//glViewport(0,0,259,231);//指定绘制区域大小
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,1,0.1,100.0);
	//gluPerspective(60.0, 1,1, 2000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
///////////////////////////////////////////////////////////////////////////////3d 后面要设置一个标志位是否开启3d
	glPolygonMode(GL_FRONT,GL_LINE);//GL_FILL or GLLINE
	glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
	glPointSize(8.0);		// NICE BEEFY POINTS FOR THE VERTEX SELECTION
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
/////////////////////////////////////////////////////////////////////////////////
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// 设置深度测试的渲染方式，在做test_movestar时应该去掉
	glDepthFunc(GL_LEQUAL);								// 在做test_movestar时应该去掉
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	return true;										// Initialization Went OK
}

bool TPOpenGL::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;

	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | 
	PFD_SUPPORT_OPENGL |
	PFD_DOUBLEBUFFER |
	PFD_TYPE_RGBA;

	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 0;
	pixelDesc.cRedShift = 0;
	pixelDesc.cGreenBits = 0;
	pixelDesc.cGreenShift = 0;
	pixelDesc.cBlueBits = 0;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 0;
	pixelDesc.cAccumRedBits = 0;
	pixelDesc.cAccumGreenBits = 0;
	pixelDesc.cAccumBlueBits = 0;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 0;
	pixelDesc.cStencilBits = 1;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	PixelFormat = ChoosePixelFormat(hDC,&pixelDesc);
	if(PixelFormat==0) // Choose default
	{
	 PixelFormat = 1;
	 if(DescribePixelFormat(hDC,PixelFormat,
	  sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
	 {
	  return FALSE;
	 }
	}

	if(SetPixelFormat(hDC,PixelFormat,&pixelDesc)==FALSE)

	{ 
	 return FALSE;
	}

	return TRUE;
}

bool TPOpenGL::CreateViewGLContext(HDC hDC)
{
	hrenderRC = wglCreateContext(hDC);
	if(hrenderRC==NULL)
	return FALSE;
	if(wglMakeCurrent(hDC,hrenderRC)==FALSE)
	return FALSE;
	return TRUE;
}

AUX_RGBImageRec* TPOpenGL::LoadBMP(char *Filename)//加载位图
{
	FILE *pFile = NULL;

	if(!Filename) return NULL;
	pFile = fopen(Filename,"r");
	if(pFile)
		fclose(pFile);
	return auxDIBImageLoad(Filename);
}

int TPOpenGL::LoadGLTextures(char *Filename)
{
	int Status = false;
	AUX_RGBImageRec *TextureImage[1];//创建纹理的存储空间
	memset(TextureImage,0,sizeof(void*)*1);
	if(TextureImage[0] = LoadBMP(Filename))
	{
		if(TextureCount<=TEXTURENUM)
		{
			glGenTextures(1,&texture[TextureCount]);
			glBindTexture(GL_TEXTURE_2D,texture[TextureCount]);
			glTexImage2D(GL_TEXTURE_2D,0,3,TextureImage[0]->sizeX,TextureImage[0]->sizeY,0,GL_RGB,GL_UNSIGNED_BYTE,TextureImage[0]->data);//生成纹理
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			TextureCount+=1;
		}
		if(TextureImage[0])
		{
			if(TextureImage[0]->data)
				free(TextureImage[0]->data);
			free(TextureImage[0]);
		}
		Status = true;
	}
	return Status;
}

void TPOpenGL::RenderScene(HDC &hrenderDC)   
{	static GLfloat	xrot=0,yrot=0,zrot=0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//glPolygonMode(GL_FRONT,GL_LINE);
	//glPolygonMode(GL_BACK,GL_LINE);
	//glTranslatef(-60.0f,-50.0f,-70.0f);      // Move Left 1.5 Units And Into The Screen 6.
	glTranslatef(0.0f,-2.0f,-11.0f);
	//glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	//glRotatef(zrot,0.0f,0.0f,1.0f);	
	//xrot+=1.0f;
	yrot+=0.5f;
	//zrot+=1.0f;
/********************测试函数在此调用***********************/
	//test_triangle();
	test_texture();
	//test_movestar();
	pobj->DrawModel();
/***********************************************************/
	SwapBuffers(hrenderDC);
}
#ifndef YMTEST
void TPOpenGL::test_triangle()
{
	glBegin(GL_TRIANGLES);								// 画立体锥形，注意所有面都要按一个方向绘制，要么都是顺时针要么都是逆时针
			glColor3f(1.0f,0.0f,0.0f);						// Set The Color To red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// 顶点(前侧)
			glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
		glVertex3f(-1.0f,-1.0f, 1.0f);					// 左下
			glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);					// 右下

			glColor3f(1.0f,0.0f,0.0f);					//右侧
		glVertex3f( 0.0f,1.0f, 0.0f);
			glColor3f(0.0f,0.0f,1.0f);
		glVertex3f( 1.0f,-1.0f, 1.0f);
			glColor3f(0.0f,1.0f,0.0f);
		glVertex3f( 1.0f,-1.0f, -1.0f);

			glColor3f(1.0f,0.0f,0.0f);					//后侧
		glVertex3f( 0.0f,1.0f, 0.0f);
			glColor3f(0.0f,1.0f,0.0f);
		glVertex3f( 1.0f,-1.0f, -1.0f);
			glColor3f(0.0f,0.0f,1.0f);
		glVertex3f( -1.0f,-1.0f, -1.0f);

			glColor3f(1.0f,0.0f,0.0f);					//左侧
		glVertex3f( 0.0f,1.0f, 0.0f);
			glColor3f(0.0f,0.0f,1.0f);
		glVertex3f( -1.0f,-1.0f, -1.0f);
			glColor3f(0.0f,1.0f,0.0f);
		glVertex3f( -1.0f,-1.0f, 1.0f);
	glEnd();
	glBegin(GL_QUADS);
			glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);					// 右下
			glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
		glVertex3f(-1.0f,-1.0f, 1.0f);
			glColor3f(1.0f,0.0f,0.0f);
		glVertex3f( -1.0f,-1.0f, -1.0f);
			glColor3f(0.0f,1.0f,0.0f);
		glVertex3f( 1.0f,-1.0f, -1.0f);
	glEnd();
}

void TPOpenGL::test_texture()
{
	glEnable(GL_TEXTURE_2D);//启用纹理映射
	glBindTexture(GL_TEXTURE_2D,texture[0]);/*
	glBegin(GL_TRIANGLES);							
	glTexCoord2f(0.5f,1.0f);glVertex3f( 0.0f, 1.0f, 0.0f);					// 顶点(前侧)
	glTexCoord2f(0.0f,0.0f);glVertex3f(-1.0f,-1.0f, 1.0f);					// 左下
	glTexCoord2f(1.0f,0.0f);glVertex3f( 1.0f,-1.0f, 1.0f);					// 右下

	//右侧
	glTexCoord2f(0.5f,1.0f);glVertex3f( 0.0f,1.0f, 0.0f);
	glTexCoord2f(0.0f,0.0f);glVertex3f( 1.0f,-1.0f, 1.0f);
	glTexCoord2f(1.0f,0.0f);glVertex3f( 1.0f,-1.0f, -1.0f);

	//后侧
	glTexCoord2f(0.5f,1.0f);glVertex3f( 0.0f,1.0f, 0.0f);
	glTexCoord2f(0.0f,0.0f);glVertex3f( 1.0f,-1.0f, -1.0f);
	glTexCoord2f(1.0f,0.0f);glVertex3f( -1.0f,-1.0f, -1.0f);
	//左侧
	glTexCoord2f(0.5f,1.0f);glVertex3f( 0.0f,1.0f, 0.0f);
	glTexCoord2f(0.0f,0.0f);glVertex3f( -1.0f,-1.0f, -1.0f);
	glTexCoord2f(1.0f,0.0f);glVertex3f( -1.0f,-1.0f, 1.0f);
glEnd();*/
}

void TPOpenGL::test_carve()
{
	static GLubyte Mask[128] =

{
     0x00, 0x00, 0x00, 0x00,    //   这是最下面的一行
     0x00, 0x00, 0x00, 0x00,
     0x03, 0x80, 0x01, 0xC0,   
     0x06, 0xC0, 0x03, 0x60,   
     0x04, 0x60, 0x06, 0x20,   
     0x04, 0x30, 0x0C, 0x20,   
     0x04, 0x18, 0x18, 0x20,   
     0x04, 0x0C, 0x30, 0x20,    
     0x04, 0x06, 0x60, 0x20,   
     0x44, 0x03, 0xC0, 0x22,   
     0x44, 0x01, 0x80, 0x22,   
     0x44, 0x01, 0x80, 0x22,   
     0x44, 0x01, 0x80, 0x22,
     0x44, 0x01, 0x80, 0x22,   
     0x44, 0x01, 0x80, 0x22,
     0x44, 0x01, 0x80, 0x22,
     0x66, 0x01, 0x80, 0x66,
     0x33, 0x01, 0x80, 0xCC,
     0x19, 0x81, 0x81, 0x98,
     0x0C, 0xC1, 0x83, 0x30,
     0x07, 0xE1, 0x87, 0xE0,
     0x03, 0x3F, 0xFC, 0xC0,
     0x03, 0x31, 0x8C, 0xC0,
     0x03, 0x3F, 0xFC, 0xC0,
     0x06, 0x64, 0x26, 0x60,
     0x0C, 0xCC, 0x33, 0x30,
     0x18, 0xCC, 0x33, 0x18,
     0x10, 0xC4, 0x23, 0x08,
     0x10, 0x63, 0xC6, 0x08,
     0x10, 0x30, 0x0C, 0x08,
     0x10, 0x18, 0x18, 0x08,
     0x10, 0x00, 0x00, 0x08    // 这是最上面的一行
};
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_POLYGON_STIPPLE);
	glPolygonStipple(Mask);
}

void TPOpenGL::test_movestar()
{
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);//设置混色函数取得半透明效果
	glEnable(GL_BLEND);//启用混色
	glEnable(GL_TEXTURE_2D);//启用纹理映射
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	for (loop=0; loop<StarNum; loop++)
	{
		star[loop].angle=0.0f;
		star[loop].dist=(float(loop)/StarNum)*2.0f;
		star[loop].r=rand()%256;
		star[loop].g=rand()%256;
		star[loop].b=rand()%256;
	}
	for (loop=0; loop<StarNum; loop++)
	{
		glLoadIdentity();
		glTranslatef(0.0f,0.0f,zoom);
		
		glRotatef(tilt,1.0f,0.0f,0.0f);
		
		
		glRotatef(star[loop].angle,0.0f,0.0f,1.0f);
		
		glTranslatef(star[loop].dist,0.0f,0.0f);
	
		glRotatef(-star[loop].angle,0.0f,0.0f,1.0f);
	
		glRotatef(-tilt,1.0f,0.0f,0.0f);
	
		
		if (twinkle)
		{
//			glColor4ub(star[(num-loop)-1].r,star[(num-loop)-1].g,star[(num-loop)-1].b,255);
		glColor4ub(star[loop].r,star[loop].g,star[loop].b,255);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
			glEnd();
		}
	
		glRotatef(spin,0.0f,0.0f,1.0f);
	
		glColor4ub(star[loop].r,star[loop].g,star[loop].b,255);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();

		spin+=0.01f;
		star[loop].angle+=float(loop)/StarNum;
		star[loop].dist-=0.01f;
		if (star[loop].dist<0.0f)
		{
			star[loop].dist+=2.0f;
			star[loop].r=rand()%256;
			star[loop].g=rand()%256;
			star[loop].b=rand()%256;
		}
	}
}

void TPOpenGL::LoadShadeTexture(const char *texfile)
{
//// Local Variables ////////////////////////////////////////////////////////////////
	int loop;
	FILE *fp;
	char line[255];
	float value1, value2;
/////////////////////////////////////////////////////////////////////////////////////

	// Make a Default one One shade with highlight
	for (loop = 0; loop < 32; loop++)
	{

		if (loop < 8)
		{
//			MAKEVECTOR4(m_ShadeSrc[loop], 0.4f, 0.4f, 0.4f, 0.0f)
		}
		else if (loop < 28)
		{
	//		MAKEVECTOR4(m_ShadeSrc[loop], 0.9f, 0.9f, 0.9f, 1.0f)
		}
		else
		{
	//		MAKEVECTOR4(m_ShadeSrc[loop], 1.0f, 1.0f, 1.0f, 1.0f)
		}
	}

	// Totally simple file format to load a 1D shade table
	// just a list of floats in a text file
	fp = fopen(texfile,"r");
	if (fp)
	{
		for (loop = 0; loop < 32; loop++)
		{
			if (feof(fp))
				break;
			// Get a line from the file
			fgets(line,255,fp);
			// Convert it to a shade value
			sscanf(line,"%f %f",&value1,&value2);
//			m_ShadeSrc[loop].x = m_ShadeSrc[loop].y = m_ShadeSrc[loop].z = value1;
	//		m_ShadeSrc[loop].w = value2;
		}
		fclose(fp);
	}
	glBindTexture(GL_TEXTURE_1D, m_ShadeTexture);

	// Do not allow bilinear filtering - not for cartoon rendering
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

//	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 32, 0,
//			 GL_RGBA , GL_FLOAT, (float *)m_ShadeSrc); //visual->texData);
}
#endif