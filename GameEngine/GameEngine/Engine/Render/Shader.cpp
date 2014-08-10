#include <Render/Shader.hh>
#include <string>
#include <fstream>
#include <cassert>

#include <Render/OpenGLTask.hh>

# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	assert(0 && std::string(std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "].").c_str()); return return_type; }


namespace gl
{
	Shader::Shader()
		: _progId(0),
		_vertexId(0),
		_fragId(0),
		_geometryId(0),
		_computeId(0),
		_vertexName(""),
		_fragName(""),
		_geometryName(""),
		_computeName("")
	{
	}

	Shader::Shader(std::string const &compute)
		: Shader()
	{
		_computeName = compute;
		_computeId = addShader(compute, GL_COMPUTE_SHADER);
		_progId = glCreateProgram();
		glAttachShader(_progId, _computeId);
		if (linkProgram() == false)
			_vertexId = _fragId = _computeId = _geometryId = -1;
	}

	Shader::Shader(std::string const &vertex, std::string const &fragment)
		: Shader()
	{
		_vertexName = vertex;
		_fragName = fragment;
		_vertexId = addShader(vertex, GL_VERTEX_SHADER);
		_fragId = addShader(fragment, GL_FRAGMENT_SHADER);
		_progId = glCreateProgram();
		glAttachShader(_progId, _vertexId);
		glAttachShader(_progId, _fragId);
		if (linkProgram() == false)
			_vertexId = _fragId = _computeId = _geometryId = -1;
	}

	Shader::Shader(std::string const &vertex, std::string const &fragment, std::string const &geometry)
		: Shader()
	{
		_vertexName = vertex;
		_fragName = fragment;
		_geometryName = geometry;
		_vertexId = addShader(vertex, GL_VERTEX_SHADER);
		_fragId = addShader(fragment, GL_FRAGMENT_SHADER);
		_geometryId = addShader(geometry, GL_GEOMETRY_SHADER);
		_progId = glCreateProgram();
		glAttachShader(_progId, _vertexId);
		glAttachShader(_progId, _fragId);
		glAttachShader(_progId, _geometryId);
		if (linkProgram() == false)
			_vertexId = _fragId = _computeId = _geometryId = -1;
	}

	Shader::Shader(Shader const &shader)
		: Shader()
	{
		_uniforms = shader._uniforms;
		_samplers = shader._samplers;
		_vertexName = shader._vertexName;
		_fragName = shader._fragName;
		_geometryName = shader._geometryName;
		_computeName = shader._computeName;

		_progId = glCreateProgram();

		if (_vertexName != "") { _vertexId = addShader(_vertexName, GL_VERTEX_SHADER); glAttachShader(_progId, _vertexId); }
		if (_fragName != "") { _fragId = addShader(_fragName, GL_FRAGMENT_SHADER); glAttachShader(_progId, _fragId); }
		if (_geometryName != "") { _geometryId = addShader(_geometryName, GL_GEOMETRY_SHADER); glAttachShader(_progId, _geometryId); }
		if (_computeName != "") { _computeId = addShader(_computeName, GL_COMPUTE_SHADER); glAttachShader(_progId, _computeId); }
		if (linkProgram() == false)
			_vertexId = _fragId = _computeId = _geometryId = -1;
	}

	Shader &Shader::operator=(Shader const &s)
	{
		if (this != &s)
		{
			_uniforms = s._uniforms;
			_samplers = s._samplers;
			if (_progId > 0)
			{
				if (_vertexId > 0) { glDetachShader(_progId, _vertexId); glDeleteShader(_vertexId); }
				if (_geometryId > 0) { glDetachShader(_progId, _geometryId); glDeleteShader(_geometryId); }
				if (_computeId > 0) { glDetachShader(_progId, _computeId); glDeleteShader(_computeId); }
				if (_fragId > 0) { glDetachShader(_progId, _fragId); glDeleteShader(_fragId); }
				glDeleteProgram(_progId);
			}
			_vertexName = s._vertexName;
			_fragName = s._fragName;
			_geometryName = s._geometryName;
			_computeName = s._computeName;

			_progId = glCreateProgram();

			if (_vertexName != "") { _vertexId = addShader(_vertexName, GL_VERTEX_SHADER); glAttachShader(_progId, _vertexId); }
			if (_fragName != "") { _fragId = addShader(_fragName, GL_FRAGMENT_SHADER); glAttachShader(_progId, _fragId); }
			if (_geometryName != "") { _geometryId = addShader(_geometryName, GL_GEOMETRY_SHADER); glAttachShader(_progId, _geometryId); }
			if (_computeName != "") { _computeId = addShader(_computeName, GL_COMPUTE_SHADER); glAttachShader(_progId, _computeId); }

			if (linkProgram() == false)
				_vertexId = _fragId = _computeId = _geometryId = -1;
		}
		return (*this);
	}

	Shader::~Shader()
	{
		if (_progId > 0)
		{
			if (_vertexId > 0) { glDetachShader(_progId, _vertexId); glDeleteShader(_vertexId); }
			if (_geometryId > 0) { glDetachShader(_progId, _geometryId); glDeleteShader(_geometryId); }
			if (_computeId > 0) { glDetachShader(_progId, _computeId); glDeleteShader(_computeId); }
			if (_fragId > 0) { glDetachShader(_progId, _fragId); glDeleteShader(_fragId); }
			glDeleteProgram(_progId);
			for (size_t index = 0; index < _tasks.size(); ++index)
				_tasks.clear();
		}
	}

	GLuint Shader::addShader(std::string const &path, GLenum type)
	{
		GLuint shaderId;
		std::ifstream file(path.c_str(), std::ios_base::binary);
		GLchar *content;
		GLint fileSize;

		if (file.fail())
			DEBUG_MESSAGE("Error", "Shader.cpp-Shader(path, type)", "File doesn't exist", -1);
		file.seekg(0, file.end);
		fileSize = static_cast<GLint>(file.tellg()) + 1;
		file.seekg(0, file.beg);
		content = new GLchar[fileSize];
		file.read(content, fileSize - 1);
		content[fileSize - 1] = 0;
		shaderId = glCreateShader(type);
		glShaderSource(shaderId, 1, const_cast<const GLchar**>(&content), const_cast<const GLint*>(&fileSize));
		if (compileShader(shaderId, path) == false)
			DEBUG_MESSAGE("Error", "Shader.cpp-Shader(path, type)", "File doesn't compile", -1);
		return (shaderId);
	}

	bool Shader::compileShader(GLuint shaderId, std::string const &file) const
	{
		GLint         compileRet = 0;
		GLsizei       msgLenght;
		GLchar        *errorMsg;

		glCompileShader(shaderId);
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileRet);
		// write error shader message
		if (compileRet == GL_FALSE)
		{
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &msgLenght);
			errorMsg = new GLchar[msgLenght];
			glGetShaderInfoLog(shaderId, msgLenght, &msgLenght, errorMsg);
			std::cerr << "Compile error on " << file.data() << ": " << std::endl;
			std::cerr << std::endl << errorMsg << std::endl << std::endl;
			delete[] errorMsg;
			DEBUG_MESSAGE("Error", "Shader.cpp-compileShader(shaderId, file)", "File doesn't compile", false);
		}
		return (true);
	}

	bool Shader::linkProgram() const
	{
		GLint         linkRet = 0;
		GLsizei       msgLenght;
		GLchar        *errorMsg;

		glLinkProgram(_progId);
		glGetProgramiv(_progId, GL_LINK_STATUS, &linkRet);
		if (linkRet == GL_FALSE)
		{
			glGetProgramiv(_progId, GL_INFO_LOG_LENGTH, &msgLenght);
			errorMsg = new GLchar[msgLenght];
			glGetProgramInfoLog(_progId, msgLenght,
				&msgLenght, errorMsg);
			std::cerr << "Link error on program : " << std::endl;
			std::cerr << std::endl << errorMsg << std::endl << std::endl;
			delete[] errorMsg;
			DEBUG_MESSAGE("Error", "Shader.cpp-linkProgram()", "File doesn't link", false);
		}
		return (true);
	}

	GLuint Shader::getUniformLocation(char const *flag)
	{
		GLuint location;

		use();
		if ((location = glGetUniformLocation(_progId, flag)) == -1)
			DEBUG_MESSAGE("Error", "Shader - getUniformLocation", "the location [" + std::string(flag) + "] doesn't exist on the shader", -1);
		return (location);
	}

	GLuint Shader::getUniformBlockLocation(char const *flag)
	{
		GLuint location;
		
		use();
		if ((location = glGetUniformBlockIndex(_progId, flag)) == -1)
			DEBUG_MESSAGE("Error", "Shader - getUniformBlockLocation", "the location [" + std::string(flag) + "] doesn't exist on the shader", -1);
		return (location);
	}

	void Shader::use() const
	{
		static GLint idbind = 0;
		
		if (idbind != _progId)
		{
			glUseProgram(_progId);
			idbind = _progId;
		}
	}

	GLuint Shader::getId() const
	{
		return (_progId);
	}

	bool Shader::isValid() const
	{
		if (_vertexId == -1 || _fragId == -1 || _geometryId == -1 || _computeId == -1)
			return (false);
		if ((_vertexId + _fragId + _geometryId + _computeId) == 0)
			return (false);
		return (true);
	}

	std::string const &Shader::getVertexName() const
	{
		return (_vertexName);
	}

	std::string const &Shader::getFragName() const
	{
		return (_fragName);
	}

	std::string const &Shader::getGeoName() const
	{
		return (_geometryName);
	}

	std::string const &Shader::getComputeName() const
	{
		return (_computeName);
	}

	Key<Uniform> Shader::getUniform(size_t target) const
	{
		if (target >= _uniforms.size())
			DEBUG_MESSAGE("Warning", "Shader.cpp - getUniform(size_t target)", "the target is out of range", Key<Uniform>(KEY_DESTROY))
		auto &element = _uniforms.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	void Shader::createUniformTask(Task &task, std::string const &flag)
	{
		task.func = NULL;
		task.nbrParams = 2;
		task.indexToTarget = 1;
		task.sizeParams = new size_t[task.nbrParams];
		task.params = new void *[task.nbrParams];
		
		task.params[0] = new GLuint;
		task.sizeParams[0] = sizeof(GLuint);
		GLuint location = getUniformLocation(flag.c_str());
		*(GLuint *)task.params[0] = location;
		
		task.params[1] = NULL;
		task.sizeParams[1] = 0;
	}

	void Shader::createSamplerTask(Task &task, std::string const &flag)
	{
		task.func = setUniformSampler;
		task.indexToTarget = 1;
		task.nbrParams = 3;
		task.sizeParams = new size_t[task.nbrParams];
		task.params = new void *[task.nbrParams];
		task.params[0] = new GLuint;
		task.sizeParams[0] = sizeof(GLuint);
		GLuint location = getUniformLocation(flag.c_str());
		*(GLuint *)task.params[0] = location;
		task.params[1] = new GLint;
		*(GLenum *)task.params[1] = GL_TEXTURE_2D;
		task.sizeParams[1] = sizeof(GLenum);
		task.params[2] = new GLint;
		*(GLint *)task.params[2] = 0;
		task.sizeParams[2] = sizeof(GLint);
	}

	void Shader::createUniformBlockTask(Task &task, std::string const &flag, UniformBlock const &ubo)
	{
		task.func = setBlockPointerUBO;
		task.indexToTarget = 4;
		task.nbrParams = 5;
		task.sizeParams = new size_t[task.nbrParams];
		task.params = new void *[task.nbrParams];
		task.params[0] = new GLuint;
		*(GLuint *)task.params[0] = _progId;
		task.sizeParams[0] = sizeof(GLuint);
		task.params[1] = new GLuint;
		GLuint location = getUniformBlockLocation(flag.c_str());
		*(GLuint *)task.params[1] = location;
		task.sizeParams[1] = sizeof(GLuint);
		task.params[2] = new GLuint;
		*(GLuint *)task.params[2] = ubo.getBindingPoint();
		task.sizeParams[2] = sizeof(GLuint);
		task.params[3] = new GLuint;
		*(GLuint *)task.params[3] = ubo.getBufferId();
		task.sizeParams[3] = sizeof(GLuint);
		task.params[4] = new UniformBlock const *;
		*(UniformBlock const **)task.params[4] = &ubo;
		task.sizeParams[4] = sizeof(UniformBlock *);
	}

	Key<Uniform> Shader::addUniform(std::string const &flag)
	{
		Key<Uniform> key;
		_tasks.push_back(Task());
		Task *task = &_tasks.back();
		_uniforms[key] = _tasks.size() - 1;
		createUniformTask(*task, flag);
		return (key);
	}

	Key<Uniform> Shader::addUniform(std::string const &flag, glm::mat4 const &value)
	{
		Key<Uniform> key;
		_tasks.push_back(Task());
		Task *task = &_tasks[_tasks.size() - 1];
		_uniforms[key] = _tasks.size() - 1;
		createUniformTask(*task, flag);
		setUniformTask<glm::mat4>(*task, setUniformMat4, (void *)&value);
		return (key);
	}
	
	Key<Uniform> Shader::addUniform(std::string const &flag, glm::mat3 const &value)
	{
		Key<Uniform> key;
		_tasks.push_back(Task());
		Task *task = &_tasks.back();
		_uniforms[key] = _tasks.size() - 1;
		createUniformTask(*task, flag);
		setUniformTask<glm::mat3>(*task, setUniformMat3, (void *)&value);
		return (key);
	}
	
	Key<Uniform> Shader::addUniform(std::string const &flag, glm::vec4 const &value)
	{
		Key<Uniform> key;
		_tasks.push_back(Task());
		Task *task = &_tasks.back();
		_uniforms[key] = _tasks.size() - 1;
		createUniformTask(*task, flag);
		setUniformTask<glm::vec4>(*task, setUniformVec4, (void *)&value);		return (key);
		return (key);
	}

	Key<Uniform> Shader::addUniform(std::string const &flag, float value)
	{
		Key<Uniform> key;
		_tasks.push_back(Task());
		Task *task = &_tasks.back();
		_uniforms[key] = _tasks.size() - 1;
		createUniformTask(*task, flag);
		setUniformTask<float>(*task, setUniformFloat, (void *)&value);
		return (key);
	}

	Shader &Shader::setUniform(Key<Uniform> const &key, glm::mat4 const &value)
	{
		Task *task;

		if ((task = getUniform(key, "setUniform")) == NULL)
			return (*this);
		setUniformTask<glm::mat4>(*task, setUniformMat4, (void *)&value);
		return (*this);
	}

	Shader &Shader::setUniform(Key<Uniform> const &key, glm::mat3 const &value)
	{
		Task *task;

		if ((task = getUniform(key, "setUniform")) == NULL)
			return (*this);
		setUniformTask<glm::mat3>(*task, setUniformMat3, (void *)&value);
		return (*this);
	}

	Shader &Shader::setUniform(Key<Uniform> const &key, glm::vec4 const &value)
	{
		Task *task;

		if ((task = getUniform(key, "setUniform")) == NULL)
			return (*this);
		setUniformTask<glm::vec4>(*task, setUniformVec4, (void *)&value);
		return (*this);
	}

	Shader &Shader::setUniform(Key<Uniform> const &key, float value)
	{
		Task *task;

		if ((task = getUniform(key, "setUniform")) == NULL)
			return (*this);
		setUniformTask<float>(*task, setUniformFloat, (void *)&value);
		return (*this);
	}

	Key<Sampler> Shader::addSampler(std::string const &flag)
	{
		Key<Sampler> key;

		_tasks.push_back(Task());
		Task *task = &_tasks.back();
		_samplers[key] = _tasks.size() - 1;
		createSamplerTask(*task, flag);
		return (key);
	}

	Key<InputSampler> Shader::addInputSampler(std::string const &flag)
	{
		Key<InputSampler> key;

		_tasks.push_back(Task());
		Task *task = &_tasks.back();
		_inputSampler[key] = _tasks.size() - 1;
		createSamplerTask(*task, flag);
		return (key);
	}

	Key<Sampler> Shader::getSampler(size_t target) const
	{
		if (target >= _samplers.size())
			DEBUG_MESSAGE("Warning", "Shader.cpp - getSampler(size_t target)", "the target is out of range", Key<Sampler>(KEY_DESTROY));
		auto &element = _samplers.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Key<InputSampler> Shader::getInputSampler(size_t target) const
	{
		if (target >= _inputSampler.size())
			DEBUG_MESSAGE("Warning", "Shader.cpp - getInputSampler(size_t target)", "the target is out of range", Key<InputSampler>(KEY_DESTROY));
		auto &element = _inputSampler.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	size_t Shader::getNbrInputSampler() const
	{
		return (_inputSampler.size());
	}

	Shader &Shader::setInputSampler(Key<InputSampler> const &key, Texture2D const &texture)
	{
		Task *task;

		if ((task = getInputSampler(key, "setInputSampler")) == NULL)
			return (*this);
		setSamplerTask(*task, texture);
		return (*this);
	}

	Shader &Shader::setSampler(Key<Sampler> const &key, Texture const &texture)
	{
		Task *task;

		if ((task = getSampler(key, "setSampler")) == NULL)
			return (*this);
		setSamplerTask(*task, texture);
		return (*this);
	}

	Key<InterfaceBlock> Shader::addInterfaceBlock(std::string const &flag, UniformBlock const &uniformBlock)
	{
		Key<InterfaceBlock> key;

		_tasks.push_back(Task());
		_interfaceBlock[key] = _tasks.size() - 1;
		Task *task = &_tasks.back();
		createUniformBlockTask(*task, flag, uniformBlock);
		return (key);
	}

	Key<InterfaceBlock> Shader::getInterfaceBlock(size_t target) const
	{
		if (target >= _interfaceBlock.size())
			DEBUG_MESSAGE("Warning", "Shader.cpp - getInterfaceBlock(size_t target)", "the target is out of range", Key<InterfaceBlock>(KEY_DESTROY));
		auto &element = _interfaceBlock.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	size_t Shader::getIndexUniform(Key<Uniform> const &key, std::string const &msg)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Shader.hh - " + msg, "key destroy use", -1);
		auto &element = _uniforms.find(key);
		if (element == _uniforms.end())
			DEBUG_MESSAGE("Warning", "Shader.cpp - " + msg, "the key correspond of any element in list", -1);
		return (element->second);
	}

	Task *Shader::getUniform(Key<Uniform> const &key, std::string const &msg)
	{
		size_t index = getIndexUniform(key, msg);
		if (index != -1)
			return (&_tasks[index]);
		return (NULL);
	}

	size_t Shader::getIndexSampler(Key<Sampler> const &key, std::string const &msg)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Shader.hh - " + msg, "key destroy use", -1);
		auto &element = _samplers.find(key);
		if (element == _samplers.end())
			DEBUG_MESSAGE("Warning", "Shader.cpp - " + msg, "the key correspond of any element in list", -1);
		return (element->second);
	}

	size_t Shader::getIndexInputSampler(Key<InputSampler> const &key, std::string const &msg)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Shader.hh - " + msg, "key destroy use", -1);
		auto &element = _inputSampler.find(key);
		if (element == _inputSampler.end())
			DEBUG_MESSAGE("Warning", "Shader.cpp - " + msg, "the key correspond of any element in list", -1);
		return (element->second);
	}

	Task *Shader::getSampler(Key<Sampler> const &key, std::string const &msg)
	{
		size_t index = getIndexSampler(key, msg);
		if (index != -1)
			return (&_tasks[index]);
		return (NULL);
	}

	Task *Shader::getInputSampler(Key<InputSampler> const &key, std::string const &msg)
	{
		size_t index = getIndexInputSampler(key, msg);
		if (index != -1)
			return (&_tasks[index]);
		return (NULL);
	}

	size_t Shader::getIndexInterfaceBlock(Key<InterfaceBlock> const &key, std::string const &msg)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Shader.hh - " + msg, "key destroy use", -1);
		auto &element = _interfaceBlock.find(key);
		if (element == _interfaceBlock.end())
			DEBUG_MESSAGE("Warning", "Shader.cpp - " + msg, "the key correspond of any element in list", -1);
		return (element->second);
	}

	Task *Shader::getInterfaceBlock(Key<InterfaceBlock> const &key, std::string const &msg)
	{
		size_t index = getIndexInterfaceBlock(key, msg);
		if (index != -1)
			return (&_tasks[index]);
		return (NULL);
	}

	size_t Shader::getUniformBindMaterial(Key<Uniform> const &key, std::string const &msg)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Shader.hh - " + msg, "key destroy use", -1);
		auto &element = _bindUniform.find(key);
		if (element == _bindUniform.end())
			DEBUG_MESSAGE("Warning", "Shader.cpp - " + msg, "the key correspond of any element in list", -1);
		return (element->second);
	}

	Shader Shader::setInterfaceBlock(Key<InterfaceBlock> const &key, UniformBlock const &uniformBlock)
	{
		Task *task;

		if ((task = getInterfaceBlock(key, "setUniform")) == NULL)
			return (*this);
		setUniformBlockTask(*task, uniformBlock);
		return (*this);
	}

	void Shader::setTransformationTask(glm::mat4 const &mat)
	{
		Task *task;
		
		if ((task = getUniform(_bindTransformation, "setTransformationTask")) == NULL)
			return;
		setUniformTask<glm::mat4>(*task, setUniformMat4, (void *)&mat);
	}

	void Shader::preDraw(Material const &material, glm::mat4 const &transform)
	{
		use();
		setTransformationTask(transform);
		for (size_t index = 0; index < _bind.size(); ++index)
			if (_bind[index].isUse)
				setTaskWithMaterial(_bind[index], material);
		for (size_t index = 0; index < _tasks.size(); ++index)
		{
			if (!_tasks[index].isExec())
				DEBUG_MESSAGE("Warning", "Shader - updateMemory", "function pointer not set",);
			if (_tasks[index].update)
			{
				_tasks[index].func(_tasks[index].params);
				_tasks[index].update = false;
			}
		}
	}

	size_t Shader::createMaterialBind(size_t offset, size_t indexTask)
	{
		for (size_t index = 0; index < _bind.size(); ++index)
		{
			if (_bind[index].isUse == false)
			{
				setMaterialBinding(_bind[index], indexTask, offset);
				return (index);
			}
		}
		MaterialBind mb;
		setMaterialBinding(mb, indexTask, offset);
		_bind.push_back(mb);
		return (_bind.size() - 1);
	}

	Shader &Shader::unbindMaterial(Key<Uniform> const &key)
	{
		size_t binding;
		if ((binding = getUniformBindMaterial(key, "unbindMaterial")) == -1)
			return (*this);
		_bind[binding].isUse = false;
		return (*this);
	}

	Shader &Shader::bindingTransformation(Key<Uniform> const &key)
	{
		_bindTransformation = key;
		return (*this);
	}
}