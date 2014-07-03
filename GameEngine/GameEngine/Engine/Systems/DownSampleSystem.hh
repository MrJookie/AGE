#pragma once

#include <memory>
#include <Systems/System.h>
#include <Core/EntityFilter.hpp>
#include <Utils/DrawQuad.hh>

class AScene;

class DownSampleSystem : public System
{
public:
	DownSampleSystem(std::weak_ptr<AScene> &&scene);
	virtual ~DownSampleSystem();

	virtual void updateBegin(double time) { }
	virtual void updateEnd(double time) { }
	virtual void mainUpdate(double time);
	virtual bool initialize();

private:
	EntityFilter						_cameraFilter;

	// DownSample quad
	DrawQuad						_quad;
};
