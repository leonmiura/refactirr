#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

class CSampleSceneNode : public ISceneNode
{
	aabbox3d<f32> Box; //bounding box for the scene node. Used by the engine for culling, etc. Can be skipped by calling scene::ISceneNode::setAutomaticCulling(scene::EAC_OFF) 
	S3DVertex Vertices[4];
	SMaterial Material;

public:
	CSampleSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id);

	virtual void OnRegisterSceneNode();
	virtual void render();
	virtual const aabbox3d<f32>& getBoundingBox() const;
	virtual u32 getMaterialCount() const;
	virtual video::SMaterial& getMaterial(u32 i);
};