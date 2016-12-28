/********************************************************************************
*																				*
*	Description:	be used for OpenGL											*
*	Author	   :	Yingmou.Zhong												*
*	Date	   :    2016-06-20													*
*																				*
*********************************************************************************/
#pragma once
#include "gl/glew.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glaux.h"
#include "gl/glext.h"
#define TEXTURENUM 1
#define _3DMODELOBJPATH "3DModel/pig.obj"
#define _3DMODELMTLPATH "3DModel/pig.mtl"
class TPOpenGL 
{
public:
	TPOpenGL(int p):PixelFormat(p){};
	~TPOpenGL(void){};
	int InitGL(GLvoid);
	bool SetWindowPixelFormat(HDC hDC);
	bool CreateViewGLContext(HDC hDC);
	void RenderScene(HDC &hrenderDC);//绘制函数
#ifndef YMTEST
	void test_carve();//镂空效果测试函数
	void test_triangle();//测试立体图形
	void test_texture();//测试纹理效果
	void test_movestar();//测试3d空间中移动图像
//	float TPOpenGL::CalculateShadow(tVector *normal,tVector *light, tMatrix *mat);
	void LoadShadeTexture(const char *texfile);
#endif
	AUX_RGBImageRec *LoadBMP(char *Filename);//加载位图
	int LoadGLTextures(char *Filename);//载入位图（调用上面函数）并转为纹理,注意：一次只加载一张图片
private:
	HGLRC hrenderRC;
	int PixelFormat;
	GLuint texture[TEXTURENUM];//保存纹理
	static int TextureCount;//用于记录纹理存储数量
	float		m_SilhouetteWidth;			// Width of Silhouette line
	unsigned int	m_ShadeTexture;				// Pointer to Shaded texture
	PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
	PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
	PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1fARB;
};

