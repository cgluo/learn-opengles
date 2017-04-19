#include "SceneLesson5.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* SceneLesson5::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SceneLesson5::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SceneLesson5::init()
{
	this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
	if (!Layer::init())
	{
		return false;
	}

	return true;
}


void SceneLesson5::menuCloseCallback(Ref* pSender)
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

void SceneLesson5::visit(cocos2d::Renderer *renderer, const Mat4 &transform, uint32_t parentFlags)
{
	Layer::visit(renderer, transform, parentFlags);
	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(SceneLesson5::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void SceneLesson5::onDraw()
{
	//我们通过调用pushMatrix把当前矩阵压栈，这个操作会把原来栈顶上的元素拷贝一份并压入栈，这样我们后续对于此矩阵的操作可以通过调用popMatrix来撤销影响。
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

	static float rotation = 60;
	modelViewMatrix.rotate(Vec3(1, 1, 1), CC_DEGREES_TO_RADIANS(rotation));
	rotation++;
	if (rotation > 360) {
		rotation = 0;
	}
	Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, modelViewMatrix);

	//获得当前SceneLesson5的shader
	auto glProgram = getGLProgram();
	//使用此shader
	glProgram->use();
	//设置该shader的一些内置uniform,主要是MVP，即model-view-project矩阵
	glProgram->setUniformsForBuiltins();

	//set color to uniform
	GLuint uColorLocation = glGetUniformLocation(glProgram->getProgram(), "u_color");
	float uColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glUniform4fv(uColorLocation, 1, uColor);

	typedef struct {
		float Position[3];
		float Color[4];
	} Vertex;

	Vertex data[] =
	{
		// Front
		{ { 1, -1, 0 },{ 1, 0, 0, 1 } },
		{ { 1, 1, 0 },{ 0, 1, 0, 1 } },
		{ { -1, 1, 0 },{ 0, 0, 1, 1 } },
		{ { -1, -1, 0 },{ 0, 0, 0, 1 } },
		// Back
		{ { 1, 1, -2 },{ 1, 0, 0, 1 } },
		{ { -1, -1, -2 },{ 0, 1, 0, 1 } },
		{ { 1, -1, -2 },{ 0, 0, 1, 1 } },
		{ { -1, 1, -2 },{ 0, 0, 0, 1 } },
		// Left
		{ { -1, -1, 0 },{ 1, 0, 0, 1 } },
		{ { -1, 1, 0 },{ 0, 1, 0, 1 } },
		{ { -1, 1, -2 },{ 0, 0, 1, 1 } },
		{ { -1, -1, -2 },{ 0, 0, 0, 1 } },
		// Right
		{ { 1, -1, -2 },{ 1, 0, 0, 1 } },
		{ { 1, 1, -2 },{ 0, 1, 0, 1 } },
		{ { 1, 1, 0 },{ 0, 0, 1, 1 } },
		{ { 1, -1, 0 },{ 0, 0, 0, 1 } },
		// Top
		{ { 1, 1, 0 },{ 1, 0, 0, 1 } },
		{ { 1, 1, -2 },{ 0, 1, 0, 1 } },
		{ { -1, 1, -2 },{ 0, 0, 1, 1 } },
		{ { -1, 1, 0 },{ 0, 0, 0, 1 } },
		// Bottom
		{ { 1, -1, -2 },{ 1, 0, 0, 1 } },
		{ { 1, -1, 0 },{ 0, 1, 0, 1 } },
		{ { -1, -1, 0 },{ 0, 0, 1, 1 } },
		{ { -1, -1, -2 },{ 0, 0, 0, 1 } }
	};

	//set data
	GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	GLubyte indices[] = {
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
		22, 23, 20,
		// Front
		0, 1, 2,
		2, 3, 0
	};

	//创建索引缓冲区并绑定索引数据到缓冲区
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

	GLuint colorLocation = glGetAttribLocation(glProgram->getProgram(), "a_color");
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Color));

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);           // 使用完要解除VBO绑定
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);   // 使用完要解除IBO绑定

	glBindVertexArray(0);
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 36);

	//如果出错了，可以使用这个函数来获取出错信息
	CHECK_GL_ERROR_DEBUG();

	//对于此矩阵的操作可以通过调用popMatrix来撤销影响。
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}