#ifndef __SceneTest1_SCENE_H__
#define __SceneTest1_SCENE_H__

#include "cocos2d.h"

class SceneTest1 : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(SceneTest1);

	virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;

	void onDraw();

private:
	cocos2d::CustomCommand _command;
};

#endif // __SceneTest1_SCENE_H__
