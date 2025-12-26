// This header provides custom error types
// Reserves no memory to prevent hard crashes in cases of memory failure
// Polymorphic -> should be caught by reference
// Uses namespaces to clearly categorize which errors belong to what
#include <string>

namespace error
{
class Error
{
public:
	virtual std::string what()
	{
		return "Error";
	};
};

namespace effect
{
class CreateFail : public Error
{
public:
	virtual std::string what() override
	{
		return "CreateFail";
	};
};

class InvalidEffect : public Error
{
public:
	virtual std::string what() override
	{
		return "InvalidEffect";
	};
};

class InvalidTechnique : public Error
{
public:
	virtual std::string what() override
	{
		return "InvalidTechnique";
	};
};
} // namespace effect

namespace rendering
{
}

namespace dx11
{
}
} // namespace error
