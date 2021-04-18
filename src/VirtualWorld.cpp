#include "VirtualWorld.h"
#include "Debug.h"

class WorldSystem
{
public:
	WorldSystem(float x, float y, Random* random)
	{
		this->random = random;

		//uint32_t nx = (uint32_t)(1000000 * x);
		//uint32_t ny = (uint32_t)(1000000 * y);
		uint32_t nx = (uint32_t)(1 * x);
		uint32_t ny = (uint32_t)(1 * y);

		//int nSeed = y << 16 | x;
		int nSeed = (nx & 0xFFFF) << 16 | (ny & 0xFFFF);

		random->seed = nSeed;
		//CactusExists = random->Rand() % 256 < 32;
		CactusExists = random->RandomInt(0, 20)  == 1;
	}
public:
	bool CactusExists = false;
	Random* random;
};

VirtualWorld::VirtualWorld()
{
	random = new Random();
}

VirtualWorld::~VirtualWorld()
{
	delete random;
	delete octreeRenderer;
}

void VirtualWorld::Initilise()
{
	floor = new Character();
	floor->LoadModel("Assets/floor.obj");
	floor->LoadTextures();

	lutTexture00 = Texture::LoadTexture("Assets/lut00.tga").Anisotropic()->ClampEdges()->Create();
	lutTexture01 = Texture::LoadTexture("Assets/lut01.tga").Anisotropic()->ClampEdges()->Create();
	lutTexture02 = Texture::LoadTexture("Assets/lut02.tga").Anisotropic()->ClampEdges()->Create();
	lutTexture03 = Texture::LoadTexture("Assets/lut03.tga").Anisotropic()->ClampEdges()->Create();
	lutTexture04 = Texture::LoadTexture("Assets/lut04.tga").Anisotropic()->ClampEdges()->Create();

	cactus = new Character();
	cactus->LoadModel("Assets/cactus_v1.obj");
	cactus->LoadTextures();

	character00 = new Character();
	//character00->LoadModel("Assets/boggie/body.obj");
	//character00->LoadModel("Assets/Harpy/Harpy.obj");
	//character00->LoadModel("Assets/Cars/Audi_R8.obj");
	character00->LoadModel("Assets/diablo3_pose/diablo3_pose.obj");
	//character00->LoadModel("Assets/Dragon.obj");
	//character00->LoadModel("Assets/cactus_v1.obj");
	//character00->LoadModel("Assets/african_head/african_head.obj");
	//character00->LoadModel("Assets/african_head/african_head_eye_inner.obj");
	//character00->LoadModel("Assets/african_head/african_head_eye_outer.obj");
	//character00->LoadModel("Assets/Character_A1016A475/aaa.obj");
	//character00->LoadModel("Assets/pistol.obj");
	//character00->LoadModel("Assets/deagle.obj");
	//character00->LoadModel("Assets/teapot.obj");
	character00->LoadTextures();

	character01 = new Character();
	//character01->LoadModel("Assets/Harpy/Harpy.obj");
	//character01->LoadModel("Assets/boggie/head.obj");
	character01->LoadModel("Assets/african_head/african_head_eye_inner.obj");
	character01->LoadTextures();

	//character02 = new Character();
	//character02->LoadModel("Assets/boggie/eyes.obj");
	//character02->LoadModel("Assets/african_head/african_head_eye_outer.obj");
	//character02->LoadTextures();

	//std::string model = "Assets/Audi_R8_3D_Model/coche_high_fin.dae";
	//animModel = AnimatedModelLoader::LoadModel(model);
	//animRenderer = new AnimatedModelRenderer();
	//animDiffuseTexture = Texture::LoadTexture("Assets/Animations/diffuse.tga").Anisotropic()->Create();
	//selectedAnimation = animModel->getAnimation();
	//animator = new Animator(animModel);
	//animator->doAnimation(selectedAnimation);

	//std::string model = "Assets/Animations/model2.dae";
	//animModel = AnimatedModelLoader::LoadModel(model);
	//animRenderer = new AnimatedModelRenderer();
	//animDiffuseTexture = Texture::LoadTexture("Assets/Animations/diffuse.tga").Anisotropic()->Create();
	//selectedAnimation = animModel->getAnimation();
	//animator = new Animator(animModel);
	//animator->doAnimation(selectedAnimation);

	//animDiffuseTexture = Texture::LoadTexture("Assets/african_head/african_head_diffuse.tga").Anisotropic()->Create();
	//animNormalTexture = Texture::LoadTexture("Assets/african_head/african_head_nm_tangent.tga").Anisotropic()->Create();
	//animSpecularTexture = Texture::LoadTexture("Assets/african_head/african_head_spec.tga").Anisotropic()->Create();
	//std::string model = "Assets/african_head/african_head.dae";
	//animModel = AnimatedModelLoader::LoadModel(model);
	//animRenderer = new AnimatedModelRenderer();
	//selectedAnimation = animModel->getAnimation();
	//animator = new Animator(animModel);
	//animator->doAnimation(selectedAnimation);

	//std::string model = "Assets/Animations/Mannequin_figure/free3DmodelDAE/free3Dmodel2.dae";
	//std::string anim = "Assets/Animations/Mannequin_figure/Male_Locomotion_PackDAE/walking.dae";
	//animModel = AnimatedModelLoader::LoadModel(model);
	//AnimatedModel* animModel2 = AnimatedModelLoader::LoadModel(anim);
	//animRenderer = new AnimatedModelRenderer();
	//animDiffuseTexture = Texture::LoadTexture("Assets/Animations/diffuse.tga").Anisotropic()->Create();
	//selectedAnimation = animModel2->getAnimation();
	//animator = new Animator(animModel);
	//animator->doAnimation(selectedAnimation);

	//skybox = new Skybox("Assets/mp_midnight/midnight-silence_rt.tga", "Assets/mp_midnight/midnight-silence_lf.tga", "Assets/mp_midnight/midnight-silence_up.tga", "Assets/mp_midnight/midnight-silence_dn.tga", "Assets/mp_midnight/midnight-silence_bk.tga", "Assets/mp_midnight/midnight-silence_ft.tga");

	//terrain = new Terrain();

	octreeRenderer = new OctreeRenderer();
	octreeRenderer->Initilise();
	octreeRenderer->AddOBJToOctree(character00->meshBuffer);
	octreeRenderer->lutTexture00 = lutTexture00;
	octreeRenderer->lutTexture01 = lutTexture01;
	octreeRenderer->lutTexture02 = lutTexture02;
	octreeRenderer->lutTexture03 = lutTexture03;
	octreeRenderer->lutTexture04 = lutTexture04;
}

void VirtualWorld::Render(sf::RenderWindow* window, Transform cameraTransform, Matrix4 projectionMatrix, Matrix4 modelview, Vector3 light_dir, Matrix4 lightModelView)
{
	character00->lutTexture00 = lutTexture00;
	character00->lutTexture01 = lutTexture01;
	character00->lutTexture02 = lutTexture02;
	character00->lutTexture03 = lutTexture03;
	character00->lutTexture04 = lutTexture04;

	character01->lutTexture00 = lutTexture00;
	character01->lutTexture01 = lutTexture01;
	character01->lutTexture02 = lutTexture02;
	character01->lutTexture03 = lutTexture03;
	character01->lutTexture04 = lutTexture04;

	//character02->lutTexture00 = lutTexture00;
	//character02->lutTexture01 = lutTexture01;
	//character02->lutTexture02 = lutTexture02;
	//character02->lutTexture03 = lutTexture03;
	//character02->lutTexture04 = lutTexture04;
	Matrix4 model_floor = Matrix4::GetIdentity();
	model_floor.Scale(10.0f);

	floor->Render(window, cameraTransform, projectionMatrix, model_floor, light_dir, lightModelView);


	Matrix4 model = Matrix4::GetIdentity();
	//model.Scale(0.1f);
	model.Translate(modelview.GetTranslation());
	model.Translate(Vector3(1.5f, 0, 0));


	character00->Render(window, cameraTransform, projectionMatrix, model * modelview, light_dir, lightModelView);
	//character01->Render(window, cameraTransform, projectionMatrix, model * modelview, light_dir, lightModelView);
	//character02->Render(window, cameraTransform, projectionMatrix, model * modelview, light_dir, lightModelView);

	//Debug::DrawLines_RenderDispatch(window, cameraTransform, projectionMatrix, model * modelview);

	//octreeRenderer->Render(window, cameraTransform, projectionMatrix, modelview, light_dir, lightModelView);
	//octreeRenderer->RenderAABB();

	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			float cx = worldPosition.x / 16;
			float cy = worldPosition.z / 16;

			WorldSystem worldSystem(x + cx, y + cy, random);


			if (worldSystem.CactusExists)
			{
				model = Matrix4::GetIdentity();
				model.Scale(0.1f);
				//model.Translate(modelview.GetTranslation());
				//model.Translate(Vector3(1.5f, 0, 0));
				model.Translate(Vector3(x * 10.0f, 0, y * 10.0f));
				//model.Translate(cameraTransform.GetPosition());
				//model.Translate(worldPosition);

				cactus->Render(window, cameraTransform, projectionMatrix, model, light_dir, lightModelView);
				Debug::DrawLines_RenderDispatch(window, cameraTransform, projectionMatrix, model);
			}
		}
	}

	// Show Actor
	//animator->update();
	//if(animDiffuseTexture != nullptr) animDiffuseTexture->bindToUnit(0);
	//if(animNormalTexture != nullptr) animNormalTexture->bindToUnit(1);
	//if(animSpecularTexture != nullptr) animSpecularTexture->bindToUnit(2);
	//if (lutTexture00 != nullptr) lutTexture00->bindToUnit(4);
	//if (lutTexture01 != nullptr) lutTexture01->bindToUnit(5);
	//if (lutTexture02 != nullptr) lutTexture02->bindToUnit(6);
	//if (lutTexture03 != nullptr) lutTexture03->bindToUnit(7);
	//if (lutTexture04 != nullptr) lutTexture04->bindToUnit(8);
	//animRenderer->render(animModel, cameraTransform, projectionMatrix, modelview, lightDir);

	//terrain->lutTexture00 = lutTexture00;
	//terrain->lutTexture01 = lutTexture01;
	//terrain->lutTexture02 = lutTexture02;
	//terrain->lutTexture03 = lutTexture03;
	//terrain->lutTexture04 = lutTexture04;

	//terrain->Render(cameraTransform, projectionMatrix, lightDir);
	//skybox->Render(cameraTransform, projectionMatrix);
}