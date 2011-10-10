#pragma once
#include <string>
namespace GCL
{

	class Resource
	{
	public:
		Resource(): mRefCount(1) {}
		virtual ~Resource() {}
		size_t mRefCount;
		static void SetAppPath(const char *resourcePath) 
		{ 
			smResourcePath = resourcePath;
#if !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE
			smResourcePath += std::string("/Contents/Resources/");
#else
			smResourcePath += std::string("/");
#endif
		}
		static const std::string &GetResourcePath() { return smResourcePath; }
	private:
			static std::string smResourcePath; 
	};


}
