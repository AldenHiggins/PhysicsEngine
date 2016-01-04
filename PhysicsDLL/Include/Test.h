#ifdef PHYSICSDLL_EXPORTS
#define PHYSICSDLL_API __declspec(dllexport) 
#else
#define PHYSICSDLL_API __declspec(dllimport) 
#endif

namespace Test
{
	class Testing
	{
	public:
		static PHYSICSDLL_API int testFunction();

	};

}