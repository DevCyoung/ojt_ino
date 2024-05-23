#include "pch.h"
#include "Resource.h"
#include "EnumResource.h"

Resource::Resource()	
	: mRelativePathOrName(L"")
	, mResourceType(eResourceType::End)
{
}

Resource::Resource(eResourceType mResourceType)
	: mRelativePathOrName(L"")
	, mResourceType(mResourceType)

{
}

Resource::~Resource()
{
}
