#include "CubeTexture.h"
using namespace GL;

cocos2d::Scene* CubeTexture::createScene()
{
	auto scene = Scene::create();
	auto layer = CubeTexture::create();
	scene->addChild(layer);
	return scene;
}

bool CubeTexture::init()
{
	if (Layer::init())
	{
		mShaderProgram = new GLProgram;
		mShaderProgram->initWithFilenames("myshader.vert", "myshader.frag");
		mShaderProgram->link();
		mShaderProgram->updateUniforms();

		_textureID = Director::getInstance()->getTextureCache()->addImage("1.png")->getName();
		_textureID2 = Director::getInstance()->getTextureCache()->addImage("2.png")->getName();
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		return true;
	}
	return false;
}

void CubeTexture::draw(Renderer *renderer, const Mat4 &transform, uint32_t transformUpdated)
{
	Layer::draw(renderer, transform, transformUpdated);

	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(CubeTexture::onDraw, this);
	renderer->addCommand(&_customCommand);
}

void CubeTexture::onDraw()
{
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);

	Mat4 modelViewMatrix;
	Mat4::createLookAt(Vec3(0, 0, 5), Vec3(0, 0, 0), Vec3(0, -1, 0), &modelViewMatrix);
	modelViewMatrix.translate(0, 0, 0);

	static float rotation = 0;
	modelViewMatrix.rotate(Vec3(0, 1, 0), CC_DEGREES_TO_RADIANS(rotation));
	rotation++;
	if (rotation > 360) {
		rotation = 0;
	}

	Mat4 projectionMatrix;
	Mat4::createPerspective(60, 480 / 320, 1.0, 42, &projectionMatrix);
	Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, projectionMatrix);
	Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, modelViewMatrix);

	typedef struct {
		float Position[3];
		float Color[4];
		float TexCoord[2];
	} Vertex;
#define TEX_COORD_MAX   1

	Vertex Vertices[] = {
		// Front
		{ { 1, -1, 0 },{ 1, 0, 0, 1 },{ TEX_COORD_MAX, 0 } },
		{ { 1, 1, 0 },{ 0, 1, 0, 1 },{ TEX_COORD_MAX, TEX_COORD_MAX } },
		{ { -1, 1, 0 },{ 0, 0, 1, 1 },{ 0, TEX_COORD_MAX } },
		{ { -1, -1, 0 },{ 0, 0, 0, 1 },{ 0, 0 } },
		// Back
		{ { 1, 1, -2 },{ 1, 0, 0, 1 },{ TEX_COORD_MAX, 0 } },
		{ { -1, -1, -2 },{ 0, 1, 0, 1 },{ TEX_COORD_MAX, TEX_COORD_MAX } },
		{ { 1, -1, -2 },{ 0, 0, 1, 1 },{ 0, TEX_COORD_MAX } },
		{ { -1, 1, -2 },{ 0, 0, 0, 1 },{ 0, 0 } },
		// Left
		{ { -1, -1, 0 },{ 1, 0, 0, 1 },{ TEX_COORD_MAX, 0 } },
		{ { -1, 1, 0 },{ 0, 1, 0, 1 },{ TEX_COORD_MAX, TEX_COORD_MAX } },
		{ { -1, 1, -2 },{ 0, 0, 1, 1 },{ 0, TEX_COORD_MAX } },
		{ { -1, -1, -2 },{ 0, 0, 0, 1 },{ 0, 0 } },
		// Right
		{ { 1, -1, -2 },{ 1, 0, 0, 1 },{ TEX_COORD_MAX, 0 } },
		{ { 1, 1, -2 },{ 0, 1, 0, 1 },{ TEX_COORD_MAX, TEX_COORD_MAX } },
		{ { 1, 1, 0 },{ 0, 0, 1, 1 },{ 0, TEX_COORD_MAX } },
		{ { 1, -1, 0 },{ 0, 0, 0, 1 },{ 0, 0 } },
		// Top
		{ { 1, 1, 0 },{ 1, 0, 0, 1 },{ TEX_COORD_MAX, 0 } },
		{ { 1, 1, -2 },{ 0, 1, 0, 1 },{ TEX_COORD_MAX, TEX_COORD_MAX } },
		{ { -1, 1, -2 },{ 0, 0, 1, 1 },{ 0, TEX_COORD_MAX } },
		{ { -1, 1, 0 },{ 0, 0, 0, 1 },{ 0, 0 } },
		// Bottom
		{ { 1, -1, -2 },{ 1, 0, 0, 1 },{ TEX_COORD_MAX, 0 } },
		{ { 1, -1, 0 },{ 0, 1, 0, 1 },{ TEX_COORD_MAX, TEX_COORD_MAX } },
		{ { -1, -1, 0 },{ 0, 0, 1, 1 },{ 0, TEX_COORD_MAX } },
		{ { -1, -1, -2 },{ 0, 0, 0, 1 },{ 0, 0 } }
	};
	int vertexCount = sizeof(Vertices) / sizeof(Vertices[0]);

	GLubyte Indices[] = {
		// Front
		0, 1, 2,
		2, 3, 0,
		// Back
		4, 5, 6,
		4, 5, 7,
		// Left
		8, 9, 10,
		10, 11, 8,
		// Right
		12, 13, 14,
		14, 15, 12,
		// Top
		16, 17, 18,
		18, 19, 16,
		// Bottom
		20, 21, 22,
		22, 23, 20
	};

	// 1) Add to top of file
	const Vertex Vertices2[] = {
		{ { 0.5, -0.5, 0.01 },{ 1, 1, 1, 1 },{ 1, 1 } },
		{ { 0.5, 0.5, 0.01 },{ 1, 1, 1, 1 },{ 1, 0 } },
		{ { -0.5, 0.5, 0.01 },{ 1, 1, 1, 1 },{ 0, 0 } },
		{ { -0.5, -0.5, 0.01 },{ 1, 1, 1, 1 },{ 0, 1 } },
	};

	const GLubyte Indices2[] = {
		1, 0, 2, 3
	};

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);


	_positionLocation = glGetAttribLocation(mShaderProgram->getProgram(), "a_position");
	_colorLocation = glGetAttribLocation(mShaderProgram->getProgram(), "a_color");

	_textureLocation = glGetAttribLocation(mShaderProgram->getProgram(), "TextureCoord");
	_textureUniform = glGetUniformLocation(mShaderProgram->getProgram(), "CC_Texture0");

	mShaderProgram->use();
	mShaderProgram->setUniformsForBuiltins();

	glEnableVertexAttribArray(_positionLocation);
	glEnableVertexAttribArray(_colorLocation);
	glEnableVertexAttribArray(_textureLocation);

	glVertexAttribPointer(_positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));

	glVertexAttribPointer(_colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));

	glVertexAttribPointer(_textureLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, TexCoord));
	//
	////set sampler
	GL::bindTexture2DN(0, _textureID);
	//glActiveTexture( GL_TEXTURE0 );
	//glBindTexture(GL_TEXTURE_2D, _textureID);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
	glUniform1i(_textureUniform, 0); // unnecc in practice

	glGenBuffers(1, &_vertexBuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);

	glGenBuffers(1, &_indexBuffer2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer2);

	GL::bindTexture2DN(0, _textureID2);
	glUniform1i(_textureUniform, 0); // unnecc in practice

	glVertexAttribPointer(_positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(_colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(_textureLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 7));

	glDrawElements(GL_TRIANGLE_STRIP, sizeof(Indices2) / sizeof(Indices2[0]), GL_UNSIGNED_BYTE, 0);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, vertexCount);

	CHECK_GL_ERROR_DEBUG();
	glDisable(GL_DEPTH_TEST);

	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
