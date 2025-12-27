// This header provides custom error types and functions to simplify exception handling
// Reserves no memory to prevent hard crashes in cases of memory failure
// Polymorphic -> should be caught by reference
// Uses namespaces to clearly categorize which errors belong to what
#include <cstdint>
#include <iostream>
#include <string>

namespace error
{
class Error
{
public:
	virtual std::string category() const
	{
		return "ERR";
	}
	virtual std::string what() const
	{
		return "Error";
	};
};

namespace effect
{
class EffectError : public Error
{
public:
	virtual std::string category() const
	{
		return "FX_ERR";
	}
};

class CreateFail : public EffectError
{
public:
	virtual std::string what() const override
	{
		return "CreateFail";
	};
};

class InvalidEffect : public EffectError
{
public:
	virtual std::string what() const override
	{
		return "InvalidEffect";
	};
};

class InvalidTechnique : public EffectError
{
public:
	virtual std::string what() const override
	{
		return "InvalidTechnique";
	};
};

class LayoutCreateFail : public EffectError
{
public:
	virtual std::string what() const override
	{
		return "LayoutCreateFail";
	};
};
} // namespace effect

namespace mesh
{
class MeshError : public Error
{
public:
	virtual std::string category() const override
	{
		return "MESH_ERR";
	};
};

class CreateFail : public MeshError
{
public:
	virtual std::string what() const override
	{
		return "CreateFail";
	};
};

class BufferCreateFail : public MeshError
{
public:
	virtual std::string what() const override
	{
		return "BufferCreateFail";
	};
};

class BufferIsEmpty : public MeshError
{
public:
	virtual std::string what() const override
	{
		return "BufferIsEmpty";
	};
};
} // namespace mesh

namespace rendering
{
}

namespace dx11
{
}

namespace utils
{
template <typename Function>
bool HandleThrowingFunction( Function f ) noexcept // All exceptions are contained within this function -> doesn't throw
{
	try
	{
		f();
	}
	catch ( const Error& e )
	{
		std::cout << "Caught exception with category [" << e.category() << "]: " << e.what() << "\n ";
		return true;
	}
	catch ( const std::exception& e )
	{
		std::cout << "Caught exception: " << e.what() << "\n";
		return true;
	}
	catch ( const std::string& eString )
	{
		std::cout << "Caught exception: " << eString << "\n";
		return true;
	}
	catch ( int eCode )
	{
		std::cout << "Caught exception: CODE=[0x" << std::hex << eCode << "]\n";
		return true;
	}
	catch ( uint32_t eCode )
	{
		std::cout << "Caught exception: CODE=[0x" << std::hex << eCode << "]\n";
		return true;
	}
	catch ( ... )
	{
		std::cout << "Caught unhandled exception\n";
		return true;
	}

	return false;
}
} // namespace utils
} // namespace error
