#ifndef __SceneTest2_SCENE_H__
#define __SceneTest2_SCENE_H__

#include "cocos2d.h"

class SceneTest2 : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(SceneTest2);

	virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;

	void onDraw();

private:
	cocos2d::CustomCommand _command;

	uint32_t _vao;
};

#endif // __SceneTest2_SCENE_H__
