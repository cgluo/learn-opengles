#include "SceneLesson6.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* SceneLesson6::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SceneLesson6::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SceneLesson6::init()
{
	this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
	if (!Layer::init())
	{
		return false;
	}

	return true;
}


void SceneLesson6::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}

void SceneLesson6::visit(cocos2d::Renderer *renderer, const Mat4 &transform, uint32_t parentFlags)
{
	Layer::visit(renderer, transform, parentFlags);
	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(SceneLesson6::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void SceneLesson6::onDraw()
{
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);

	Mat4 projectionMatrix;
	auto size = Director::getInstance()->getWinSize();
	Mat4::createPerspective(60, size.width / size.height, 1.0, 50, &projectionMatrix);
	Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, projectionMatrix);

	Mat4 modelViewMatrix;
	Mat4::createLookAt(Vec3(0, 0, 1), Vec3(0, 0, 0), Vec3(0, 1, 0), &modelViewMatrix);
	modelViewMatrix.translate(0, 0, -5);

	static float rotation = 0;
	modelViewMatrix.rotate(Vec3(1, 1, 1), CC_DEGREES_TO_RADIANS(rotation));
	rotation++;
	if (rotation > 360) {
		rotation = 0;
	}
	Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, modelViewMatrix);

	auto glProgram = getGLProgram();
	glProgram->use();
	glProgram->setUniformsForBuiltins();

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//set color to uniform
	GLuint uColorLocation = glGetUniformLocation(glProgram->getProgram(), "u_color");
	float uColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glUniform4fv(uColorLocation, 1, uColor);

	typedef struct {
		float Position[3];
		float TexCoord[2];
	} Vertex;

	Vertex data[] = {
		// Front
		{ { 1, -1, 0 }, { 1, 0 } },
		{ { 1, 1, 0 }, { 1, 1 } },
		{ { -1, 1, 0 }, { 0, 1 } },
		{ { -1, -1, 0 }, { 0, 0 } },
		// Back
		{ { 1, 1, -2 }, { 1, 0 } },
		{ { -1, -1, -2 }, { 0, 1 } },
		{ { 1, -1, -2 }, { 1, 1 } },
		{ { -1, 1, -2 }, { 0, 0 } },
		// Left
		{ { -1, -1, 0 }, { 1, 0 } },
		{ { -1, 1, 0 }, { 1, 1 } },
		{ { -1, 1, -2 }, { 0, 1 } },
		{ { -1, -1, -2 }, { 0, 0 } },
		// Right
		{ { 1, -1, -2 }, { 1, 0 } },
		{ { 1, 1, -2 }, { 1, 1 } },
		{ { 1, 1, 0 }, { 0, 1 } },
		{ { 1, -1, 0 }, { 0, 0 } },
		// Top
		{ { 1, 1, 0 }, { 1, 0 } },
		{ { 1, 1, -2 }, { 1, 1 } },
		{ { -1, 1, -2 }, { 0, 1 } },
		{ { -1, 1, 0 }, { 0, 0 } },
		// Bottom
		{ { 1, -1, -2 }, { 1, 0 } },
		{ { 1, -1, 0 }, { 1, 1 } },
		{ { -1, -1, 0 }, { 0, 1 } },
		{ { -1, -1, -2 }, { 0, 0 } }
	};

	//set data
	GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	GLubyte indices[] = {
		// Front
		0, 1, 2,
		2, 3, 0,
		// Back
		4, 6, 5,
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

	//set indices
	GLuint indexVBO;
	glGenBuffers(1, &indexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint positionLocation = glGetAttribLocation(glProgram->getProgram(), "a_position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Position));

	GLuint TexCoordLocation = glGetAttribLocation(glProgram->getProgram(), "a_texCoord");
	glEnableVertexAttribArray(TexCoordLocation);
	glVertexAttribPointer(TexCoordLocation,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, TexCoord));

	GLuint textureId;
	textureId = Director::getInstance()->getTextureCache()->addImage("1.png")->getName();
	GL::bindTexture2D(textureId);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 36);

	CHECK_GL_ERROR_DEBUG();

	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}