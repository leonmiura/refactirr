#include "CSampleSceneNode.h"
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;


	CSampleSceneNode::CSampleSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id)
		: ISceneNode(parent, mgr, id)
	{
		Material.Wireframe = false;
		Material.Lighting = false;

		//define scene node vertices

		//Vertices[0] = S3DVertex(0, 0, 10, 1, 1, 0, SColor(255, 0, 255, 255), 0, 1); //Study parameters to S3DVertex - (position), (normal), (color), (texture coordinates)
		//Vertices[1] = S3DVertex(10, 0, -10, 1, 0, 0, SColor(255, 255, 0, 255), 1, 1); 
		//Vertices[2] = S3DVertex(0,20,0,0,1,1, SColor(255, 255, 255, 0), 1, 0); 
		//Vertices[3] = S3DVertex(-10, 0, -10, 0, 0, 1, SColor(255, 0, 255, 255), 0, 0); 

		////if no texture is being used, then texture coordinates should be irrelevant. Are they? --> YES. They may have set the values because so that in a later tutorial we can apply textures
		//Vertices[0] = S3DVertex(0, 0, 10, 1, 1, 0, SColor(255, 0, 255, 255), 0, 0); //Study parameters to S3DVertex - (position), (normal), (color), (texture coordinates)
		//Vertices[1] = S3DVertex(10, 0, -10, 1, 0, 0, SColor(255, 255, 0, 255), 0, 0);
		//Vertices[2] = S3DVertex(0, 20, 0, 0, 1, 1, SColor(255, 255, 255, 0), 0, 0);
		//Vertices[3] = S3DVertex(-10, 0, -10, 0, 0, 1, SColor(255, 0, 255, 255), 0, 0);

		//if no lighting is being used, then normal coordinates should be irrelevant. Are they? --> YES. 
		Vertices[0] = S3DVertex(0, 0, 10, -1, 0, -1, SColor(255, 0, 255, 255), 0, 0); //Study parameters to S3DVertex - (position), (normal), (color), (texture coordinates)
		Vertices[1] = S3DVertex(10, 0, -10, 1, 1, 1 , SColor(255, 255, 0, 255), 0, 0);
		Vertices[2] = S3DVertex(0, 20, 0, 1, 0, 1, SColor(255, 255, 255, 0), 0, 0);
		Vertices[3] = S3DVertex(-10, 0, -10, 1, 1, 1, SColor(255, 0, 255, 255), 0, 0);


		//if no lighting is being used, then normal coordinates should be irrelevant. Are they? --> YES. 
		Vertices[0] = S3DVertex(0, 0, 10, -1, 0, -1, SColor(255, 0, 255, 255), 0, 0); //Study parameters to S3DVertex - (position), (normal), (color), (texture coordinates)
		Vertices[1] = S3DVertex(10, 0, -10, 1, 1, 1, SColor(255, 255, 0, 255), 0, 0);
		Vertices[2] = S3DVertex(0, 20, 0, 1, 0, 1, SColor(255, 255, 255, 0), 0, 0);
		Vertices[3] = S3DVertex(-10, 0, -10, 1, 1, 1, SColor(255, 0, 255, 255), 0, 0);



		//create bounding box for scene node
		//Box.reset(Vertices[0].Pos);
		for (s32 i = 1; i < 4; ++i) //any reason for ++i instead of i++?
		{
			Box.addInternalPoint(Vertices[i].Pos);
		}
	}

	void CSampleSceneNode::OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
	}


	void CSampleSceneNode::render()
	{
		u16 indices[] = { 0,2,3, 2,1,3, 1,0,3, 2,0,1 };
		IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setMaterial(Material);
		driver->setTransform(ETS_WORLD, AbsoluteTransformation);
		driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 4, EVT_STANDARD, EPT_TRIANGLES, EIT_16BIT);
	}

	const core::aabbox3d<f32>& CSampleSceneNode::getBoundingBox() const
	{
		return Box;
	}

	u32 CSampleSceneNode::getMaterialCount() const
	{
		return 1;
	}

	// Because we have only one material here, we can return the only one material, assuming that no one ever calls getMaterial() with an index greater than 0.
	SMaterial& CSampleSceneNode::getMaterial(u32 i)
	{
		return Material;
	}