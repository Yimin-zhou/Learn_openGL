//#pragma once
//
//#include "model/model.h"
//#include "camera/camera.h"
//
//class Scene
//{
//public:
//	virtual void init() = 0;
//	virtual void update(std::shared_ptr<Window> window, float deltaTime) = 0;
//	virtual void draw(std::shared_ptr<Window> window) = 0;
//	virtual void resize(std::shared_ptr<Window> window) = 0;
//	virtual void setCameraAspectRatio(float aspectRatio) = 0;
//	virtual uint32_t getFinalTexture() = 0;
//	virtual ~Scene() = default;
//
//public:	
//	std::vector<Model> m_models;
//	std::vector<Shader> m_shaders;
//	Camera m_camera;
//};