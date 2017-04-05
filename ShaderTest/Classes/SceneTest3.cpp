#include "SceneTest3.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* SceneTest3::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SceneTest3::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SceneTest3::init()
{
	this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
    if ( !Layer::init() )
    {
        return false;
    }
        
    return true;
}


void SceneTest3::menuCloseCallback(Ref* pSender)
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

void SceneTest3::visit(cocos2d::Renderer *renderer, const Mat4 &transform, uint32_t parentFlags)
{
	Layer::visit(renderer, transform, parentFlags);
	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(SceneTest3::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void SceneTest3::onDraw()
{
	//����ͨ������pushMatrix�ѵ�ǰ����ѹջ������������ԭ��ջ���ϵ�Ԫ�ؿ���һ�ݲ�ѹ��ջ���������Ǻ������ڴ˾���Ĳ�������ͨ������popMatrix������Ӱ�졣
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);

	//��õ�ǰSceneTest3��shader
	auto glProgram = GLProgram::createWithFilenames("myVertextShader.vert", "myFragmentShader.frag");
	//ʹ�ô�shader
	glProgram->use();
	//���ø�shader��һЩ����uniform,��Ҫ��MVP����model-view-project����
	glProgram->setUniformsForBuiltins();

	//set color to uniform
	GLuint uColorLocation = glGetUniformLocation(glProgram->getProgram(), "u_color");
	float uColor[] = { 1.0, 0.0, 0.0, 1.0 };
	glUniform4fv(uColorLocation, 1, uColor);

	auto size = Director::getInstance()->getWinSize();
	//ָ����Ҫ���Ƶ������ε��������㣬�ֱ�λ����Ļ���½ǣ����½Ǻ����м�Ķ���
	//float vertercies[] = { 
	//	0,0,   //��һ���������
	//	size.width, 0,   //�ڶ����������
	//	size.width / 2, size.height //�������������
	//};  

	//normalized device space�ռ��ȡֵ��Χ��-1~+1
	float vertercies[] = {
		-1,-1,   //��һ���������
		1, -1,   //�ڶ����������
		0, 1 //�������������
	};  
	
	//ָ��ÿһ���������ɫ����ɫֵ��RGBA��ʽ�ģ�ȡֵ��Χ��0-1
	float color[] = { 0, 1,0, 1,    //��һ�������ɫ����ɫ
		1,0,0, 1,  //�ڶ��������ɫ, ��ɫ
		0, 0, 1, 1 };  //�����������ɫ�� ��ɫ
					   //��������Ϊposition��color��vertex attribute
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
	//�ֱ��position��colorָ������Դ
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertercies);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);
	//���������Σ���ν��draw call����ָ�����������
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//֪ͨcocos2d-x ��renderer�������ں��ʵ�ʱ�������ЩOpenGL����
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
	//��������ˣ�����ʹ�������������ȡ������Ϣ
	CHECK_GL_ERROR_DEBUG();

	//���ڴ˾���Ĳ�������ͨ������popMatrix������Ӱ�졣
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}