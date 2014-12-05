#include <Render/Program.hh>
#include <assert.h>
#include <Utils/OpenGL.hh>

Program::Program(std::vector<std::shared_ptr<UnitProg>> const &u) :
_unitsProg(u)
{
	create();
}

Program::Program(Program &&move):
_resourcesProgram(std::move(move._resourcesProgram)),
_unitsProg(std::move(move._unitsProg)),
_id(std::move(move._id))
{
	move._id = 0;
}

/**
* Method:    operator=
* FullName:  Program::operator=
* Access:    public 
* Returns:   Program &
* Qualifier:
* Parameter: Program & & u
* Goal:		 Assignment operation
*/
Program & Program::operator=(Program &&u)
{
	_resourcesProgram = std::move(u._resourcesProgram);
	_unitsProg = std::move(u._unitsProg);
	_id = std::move(u)._id;
	u._id = 0;
	return (*this);
}

/**
* Method:    setAttribute
* FullName:  Program::setAttribute
* Access:    public 
* Returns:   Program &
* Qualifier:
* Parameter: std::vector<Attribute> const & attibutes
* Goal:		 Alloc a new vertexPool in function attributes
*/
Program & Program::setAttribute(std::vector<Attribute> const &attributes)
{
	_graphicalMemory.load(attributes);
	return (*this);
}

/**
* Method:    setAttribute
* FullName:  Program::setAttribute
* Access:    public 
* Returns:   Program &
* Qualifier:
* Parameter: std::vector<Attribute> & & attibutes
* Goal:		 Alloc a new vertexPool in function attributes
*/
Program & Program::setAttribute(std::vector<Attribute> &&attibutes)
{
	return (*this);
}

/**
* Method:    getId
* FullName:  Program::getId
* Access:    public 
* Returns:   GLuint
* Qualifier: const
* Goal:		 accessors
*/
GLuint Program::getId() const
{
	return (_id);
}

Program & Program::update()
{
	return (*this);
}

Program const & Program::use() const
{
	static GLint currentProgram = 0;
	if (currentProgram == _id)
		return (*this);
	currentProgram = _id;
	glUseProgram(_id);
	return (*this);
}

/**
* Method:    create
* FullName:  Program::create
* Access:    private 
* Returns:   void
* Qualifier: 
* Goal:      Create the id program, attach unit, and link, No verification
*/
void Program::create()
{
	_id = glCreateProgram();
	for (auto &element : _unitsProg)
	{
		glAttachShader(_id, element->getId());
	}
	glLinkProgram(_id);
}

/**
* Method:    destroy
* FullName:  Program::destroy
* Access:    private 
* Returns:   void
* Qualifier: 
* Goal:      Delete the _id if it is superior than 0
*/
void Program::destroy()
{
	if (_id > 0)
		glDeleteProgram(_id);
}