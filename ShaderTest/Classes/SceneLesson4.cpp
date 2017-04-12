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
	//����ͨ������pushMatrix�ѵ�ǰ����ѹջ������������ԭ��ջ���ϵ�Ԫ�ؿ���һ�ݲ�ѹ��ջ���������Ǻ������ڴ˾���Ĳ�������ͨ������popMatrix������Ӱ�졣
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);

	//��õ�ǰSceneLesson4��shader
	auto glProgram = GLProgram::createWithFilenames("myVertextShader.vert", "myFragmentShader.frag");
	//ʹ�ô�shader
	glProgram->use();
	//���ø�shader��һЩ����uniform,��Ҫ��MVP����model-view-project����
	glProgram->setUniformsForBuiltins();

	//set color to uniform
	GLuint uColorLocation = glGetUniformLocation(glProgram->getProgram(), "u_color");
	float uColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glUniform4fv(uColorLocation, 1, uColor);

	//auto size = Director::getInstance()->getWinSize();
	//ָ����Ҫ���Ƶ������ε��������㣬�ֱ�λ����Ļ���½ǣ����½Ǻ����м�Ķ���
	//float vertercies[] = { 
	//	0,0,   //��һ���������
	//	size.width, 0,   //�ڶ����������
	//	size.width / 2, size.height //�������������
	//};  

	////normalized device space�ռ��ȡֵ��Χ��-1~+1
	//float vertercies[] =
	//{ -0.5,-0.5,
	//	0.5, -0.5,
	//	-0.5, 0.5,
	//	0.5,0.5 };

	////ָ��2�������ε�����
	//GLubyte indices[] = { 0,1,2,  //��һ������������
	//	2,3,1 }; //�ڶ�������������

	////�������������������������ݵ�������
	//GLuint indexVBO;
	//glGenBuffers(1, &indexVBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	////ָ��ÿһ���������ɫ����ɫֵ��RGBA��ʽ�ģ�ȡֵ��Χ��0-1
	//float color[] = { 0, 1,0, 1,
	//	0,1,0, 1,
	//	0, 1, 0, 1,
	//	0, 1,0, 1,
	//	0,1,0, 1,
	//	0,1, 0, 1 };

	////��������Ϊposition��color��vertex attribute
	//GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
	////�ֱ��position��colorָ������Դ
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

	//ָ��2�������ε�����
	GLubyte indices[] = { 0,1,2,  //��һ������������
		2,3,1 }; //�ڶ�������������

	//�������������������������ݵ�������
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);           // ʹ����Ҫ���VBO��
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);   // ʹ����Ҫ���IBO��

	glBindVertexArray(0);
	//�����6�ǿ�ѡ�ģ��ĳ�6���Ը��õ���cocos2d-x�����ں�
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);

	//��������ˣ�����ʹ�������������ȡ������Ϣ
	CHECK_GL_ERROR_DEBUG();

	//���ڴ˾���Ĳ�������ͨ������popMatrix������Ӱ�졣
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}