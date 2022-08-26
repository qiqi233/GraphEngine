#include "RenderingInterface.h"


FGuid IRenderingIntface::GetGuid()
{
	return RenderGuid;
}

IRenderingIntface::IRenderingIntface()
{
	RenderGuid=FGuid::NewGuid();
}

IRenderingIntface::~IRenderingIntface()
{

}

bool IRenderingIntface::operator==(const IRenderingIntface& InOther)
{
	return RenderGuid==InOther.RenderGuid;
}

