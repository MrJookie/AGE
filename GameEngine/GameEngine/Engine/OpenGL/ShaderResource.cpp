#include <OpenGL/ShaderResource.hh>
#include <stdint.h>
#include <OpenGL/Shader.hh>
#include <glm/glm.hpp>
#include <OpenGL/MemoryGPU.hh>
#include <OpenGL/UniformBlock.hh>

namespace gl
{
	ShaderResource::ShaderResource(std::string const &flag, Shader const *attach)
		: _flag(flag),
		_data(NULL),
		_sizeData(0),
		_attach(attach),
		_location(-1)
	{
	}

	ShaderResource::ShaderResource()
		: ShaderResource("", NULL)
	{

	}

	ShaderResource::ShaderResource(ShaderResource const &ressource)
		: ShaderResource(ressource._flag, ressource._attach)
	{
		_sizeData = ressource._sizeData;
		_data = new uint8_t[_sizeData];
		memcpy(_data, ressource._data, _sizeData);
	}

	ShaderResource &ShaderResource::operator=(ShaderResource const &u)
	{
		if (this != &u)
		{
			_flag = u._flag;
			_location = u._location;
			if (u._sizeData != _sizeData)
			{
				_sizeData = u._sizeData;
				if (_data)
					delete _data;
				_data = new uint8_t[_sizeData];
			}
			memcpy(_data, u._data, _sizeData);
			_attach = u._attach;
		}
		return (*this);
	}

	ShaderResource::~ShaderResource()
	{
		if (_data)
			delete _data;
	}

	bool ShaderResource::getUniformLocation()
	{
		if (_attach == NULL)
			DEBUG_MESSAGE("Error", "ShaderResource.cpp - getUniformLocation()", "No attach on this uniform", false);
		_attach->use();
		if ((_location = glGetUniformLocation(_attach->getId(), _flag.c_str())) == -1)
			DEBUG_MESSAGE("Error", "ShaderResource.cpp - getUniformLocation()", "the location [" + _flag + "] doesn't exist on the shader", false)
		return (true);
	}

	bool ShaderResource::getUniformBlockLocation()
	{
		if (_attach == NULL)
			DEBUG_MESSAGE("Error", "Uniform.cpp - getUniformBlockLocation()", "No attach on this uniform", false);
		_attach->use();
		if ((_location = glGetUniformBlockIndex(_attach->getId(), _flag.c_str())) == -1)
			DEBUG_MESSAGE("Error", "Uniform.cpp - getUniformBlockLocation()", "the location [" + _flag + "] doesn't exist on the shader", false)
		return (true);
	}

	template <typename TYPE>
	static void *setAllocation(size_t &sizeData, void *data)
	{
		if (sizeof(TYPE) != sizeData)
		{
			sizeData = sizeof(TYPE);
			if (data)
				delete data;
			data = new TYPE;
		}
		return (data);
	}

	// code all set syncronisation func you need
	ShaderResource &ShaderResource::set(glm::mat4 const &value)
	{
		_data = setAllocation<glm::mat4>(_sizeData, _data);
		memcpy(_data, &value, _sizeData);
		if (_location == -1 && getUniformLocation() == false)
			return (*this);
		glUniformMatrix4fv(_location, 1, GL_FALSE, &(*(glm::mat4 *)_data)[0].x);
		return (*this);
	}

	ShaderResource &ShaderResource::set(glm::mat3 const &value)
	{
		_data = setAllocation<glm::mat3>(_sizeData, _data);
		memcpy(_data, &value, _sizeData);
		if (_location == -1 && getUniformLocation() == false)
			return (*this);
		glUniformMatrix3fv(_location, 1, GL_FALSE, &(*(glm::mat3 *)_data)[0].x);
		return (*this);
	}

	ShaderResource &ShaderResource::set(int value)
	{
		_data = setAllocation<int>(_sizeData, _data);
		memcpy(_data, &value, _sizeData);
		if (_location == -1 && getUniformLocation() == false)
			return (*this);
		glUniform1i(_location, value);
		return (*this);
	}

	ShaderResource &ShaderResource::set(UniformBlock const &value)
	{
		_data = setAllocation<UniformBlock>(_sizeData, _data);
		memcpy(_data, &value, _sizeData);
		if (getUniformBlockLocation() == false)
			return (*this);
		glUniformBlockBinding(_attach->getId(), _location, value.getBindingPoint());
		value.bind();
		return (*this);
	}

	ShaderResource &ShaderResource::set(float value)
	{
		_data = setAllocation<float>(_sizeData, _data);
		memcpy(_data, &value, _sizeData);
		if (_location == -1 && getUniformLocation() == false)
			return (*this);
		glUniform1f(_location, value);
		return (*this);
	}

	ShaderResource &ShaderResource::set(glm::vec4 const &value)
	{
		_data = setAllocation<glm::vec4>(_sizeData, _data);
		memcpy(_data, &value, _sizeData);
		if (_location == -1 && getUniformLocation() == false)
			return (*this);
		glUniform4f(_location, value[0], value[1], value[2], value[3]);
		return (*this);
	}

}