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
	void RenderScene(HDC &hrenderDC);//���ƺ���
#ifndef YMTEST
	void test_carve();//�ο�Ч�����Ժ���
	void test_triangle();//��������ͼ��
	void test_texture();//��������Ч��
	void test_movestar();//����3d�ռ����ƶ�ͼ��
//	float TPOpenGL::CalculateShadow(tVector *normal,tVector *light, tMatrix *mat);
	void LoadShadeTexture(const char *texfile);
#endif
	AUX_RGBImageRec *LoadBMP(char *Filename);//����λͼ
	int LoadGLTextures(char *Filename);//����λͼ���������溯������תΪ����,ע�⣺һ��ֻ����һ��ͼƬ
private:
	HGLRC hrenderRC;
	int PixelFormat;
	GLuint texture[TEXTURENUM];//��������
	static int TextureCount;//���ڼ�¼����洢����
	float		m_SilhouetteWidth;			// Width of Silhouette line
	unsigned int	m_ShadeTexture;				// Pointer to Shaded texture
	PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
	PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
	PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1fARB;
};

