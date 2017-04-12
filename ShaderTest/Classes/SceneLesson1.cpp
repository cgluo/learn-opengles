#include "SceneLesson1.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* SceneLesson1::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SceneLesson1::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SceneLesson1::init()
{
	this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
    if ( !Layer::init() )
    {
        return false;
    }
        
    return true;
}


void SceneLesson1::menuCloseCallback(Ref* pSender)
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

void SceneLesson1::visit(cocos2d::Renderer *renderer, const Mat4 &transform, uint32_t parentFlags)
{
	Layer::visit(renderer, transform, parentFlags);
	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(SceneLesson1::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void SceneLesson1::onDraw()
{
	//��õ�ǰSceneLesson1��shader
	auto glProgram = getGLProgram();
	//ʹ�ô�shader
	glProgram->use();
	//���ø�shader��һЩ����uniform,��Ҫ��MVP����model-view-project����
	glProgram->setUniformsForBuiltins();

	auto size = Director::getInstance()->getWinSize();
	//ָ����Ҫ���Ƶ������ε��������㣬�ֱ�λ����Ļ���½ǣ����½Ǻ����м�Ķ���
	float vertercies[] = { 0,0,   //��һ���������
		size.width, 0,   //�ڶ����������
		size.width / 2, size.height };  //�������������
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
}