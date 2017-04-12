#include "SceneLesson4.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* SceneLesson4::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SceneLesson4::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SceneLesson4::init()
{
	this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
	if (!Layer::init())
	{
		return false;
	}

	return true;
}


void SceneLesson4::menuCloseCallback(Ref* pSender)
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

void SceneLesson4::visit(cocos2d::Renderer *renderer, const Mat4 &transform, uint32_t parentFlags)
{
	Layer::visit(renderer, transform, parentFlags);
	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(SceneLesson4::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void SceneLesson4::onDraw()
{
	//我们通过调用pushMatrix把当前矩阵压栈，这个操作会把原来栈顶上的元素拷贝一份并压入栈，这样我们后续对于此矩阵的操作可以通过调用popMatrix来撤销影响。
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);

	//获得当前SceneLesson4的shader
	auto glProgram = GLProgram::createWithFilenames("myVertextShader.vert", "myFragmentShader.frag");
	//使用此shader
	glProgram->use();
	//设置该shader的一些内置uniform,主要是MVP，即model-view-project矩阵
	glProgram->setUniformsForBuiltins();

	//set color to uniform
	GLuint uColorLocation = glGetUniformLocation(glProgram->getProgram(), "u_color");
	float uColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glUniform4fv(uColorLocation, 1, uColor);

	//auto size = Director::getInstance()->getWinSize();
	//指定将要绘制的三角形的三个顶点，分别位到屏幕左下角，右下角和正中间的顶端
	//float vertercies[] = { 
	//	0,0,   //第一个点的坐标
	//	size.width, 0,   //第二个点的坐标
	//	size.width / 2, size.height //第三个点的坐标
	//};  

	////normalized device space空间的取值范围是-1~+1
	//float vertercies[] =
	//{ -0.5,-0.5,
	//	0.5, -0.5,
	//	-0.5, 0.5,
	//	0.5,0.5 };

	////指定2个三角形的索引
	//GLubyte indices[] = { 0,1,2,  //第一个三角形索引
	//	2,3,1 }; //第二个三角形索引

	////创建索引缓冲区并绑定索引数据到缓冲区
	//GLuint indexVBO;
	//glGenBuffers(1, &indexVBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	////指定每一个顶点的颜色，颜色值是RGBA格式的，取值范围是0-1
	//float color[] = { 0, 1,0, 1,
	//	0,1,0, 1,
	//	0, 1, 0, 1,
	//	0, 1,0, 1,
	//	0,1,0, 1,
	//	0,1, 0, 1 };

	////激活名字为position和color的vertex attribute
	//GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
	////分别给position和color指定数据源
	//glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertercies);
	//glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);


	typedef struct {
		float Position[2];
		float Color[4];
	} Vertex;

	Vertex data[] =
	{
		{ { -0.5,-0.5 },{ 0,1,0,1 } },
		{ { 0.5,-0.5 },{ 0,1,0,1 } },
		{ { -0.5,0.5 },{ 0,1,0,1 } },
		{ { 0.5,0.5 },{ 0,1,0,1 } }
	};

	//set data
	GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	//指定2个三角形的索引
	GLubyte indices[] = { 0,1,2,  //第一个三角形索引
		2,3,1 }; //第二个三角形索引

	//创建索引缓冲区并绑定索引数据到缓冲区
	GLuint indexVBO;
	glGenBuffers(1, &indexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint positionLocation = glGetAttribLocation(glProgram->getProgram(), "a_position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation,
		2,
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

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);           // 使用完要解除VBO绑定
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);   // 使用完要解除IBO绑定

	glBindVertexArray(0);
	//这里的6是可选的，改成6可以更好地与cocos2d-x引擎融合
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);

	//如果出错了，可以使用这个函数来获取出错信息
	CHECK_GL_ERROR_DEBUG();

	//对于此矩阵的操作可以通过调用popMatrix来撤销影响。
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}