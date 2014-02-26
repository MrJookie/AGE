// for leak detection
#define _CRTDBG_MAP_ALLOC
#define GLM_FORCE_INLINE 
#define GLM_FORCE_SSE2 
#define GLM_FORCE_AVX

#include <stdlib.h>
#include <crtdbg.h>


#include <stdlib.h>
#include "Core/Engine.hh"
#include "DemoScene.hh"
#include "Utils/PubSub.hpp"
#include "Context/SdlContext.hh"
#include "Core/SceneManager.hh"
#include "Core/Renderer.hh"
#include "OpenGL/VertexManager.hh"
#include "OpenGL/Attribute.hh"
#include <MediaFiles/AssetsManager.hpp>
#include <Audio/AudioManager.hh>

int			main(int ac, char **av)
{
	Engine	e;

	// set Rendering context of the engine
	// you can also set any other dependencies
	e.setInstance<PubSub::Manager>();
	e.setInstance<SdlContext, IRenderContext>();
	e.setInstance<Input>();
	e.setInstance<Timer>();
	e.setInstance<AssetsManager>()->init(&e);
	e.setInstance<Renderer>(&e);
	e.setInstance<SceneManager>();
	if (!e.setInstance<AudioManager>()->init())
		return EXIT_FAILURE;

	// init engine
	if (e.init() == false)
		return (EXIT_FAILURE);


	// add scene
	e.getInstance<SceneManager>()->addScene(std::make_shared<DemoScene>(e), "demo");

	// bind scene
	if (!e.getInstance<SceneManager>()->initScene("demo"))
		return false;
	e.getInstance<SceneManager>()->enableScene("demo", 0);

	// launch engine
	if (e.start() == false)
		return (EXIT_FAILURE);
	while (e.update())
		;
	e.stop();
	return (EXIT_SUCCESS);
}