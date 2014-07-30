#pragma once

# include <Utils/OpenGL.hh>
# include <glm/glm.hpp>
# include <memory>
# include <Render/Data.hh>

namespace gl
{
	class Shader;
}
class DependenciesInjector;
class Renderer;
#if !TEST_NEW_VERTEXMANAGER
template <uint8_t SIZE_ATTRIBUTE> class VertexManager;
#endif

class DrawQuad
{
public:
	DrawQuad();
	~DrawQuad();

	void	init(std::weak_ptr<DependenciesInjector> &&engine);
	void	draw(GLuint texture, int sampleNbr, glm::uvec2 const &textureSize);

private:
	void						initShaders();
	gl::GeometryManager			*_m;
	gl::Key<gl::Vertices>		_quadvertices;
	gl::Key<gl::Indices>		_quadindices;
	gl::Key<gl::VertexPool>		_pool;
	std::shared_ptr<gl::Shader>			_fboToScreen;
	std::shared_ptr<gl::Shader>			_fboToScreenMultisampled;
	Renderer*	_renderer;
};

