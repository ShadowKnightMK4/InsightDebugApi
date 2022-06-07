#include "ObjectAttributesHelper.h"

/*
* x64
test sizeof() == 48
test.length offset=0 size= 4
test.rootdirectory offset=8 size= 8
test.objectname offset=16 size= 8
test.attributes offset=24 size= 4
test.securitydesc offset=32 size= 8
test.qos offset=40 size= 8*/

/*
* x86
test sizeof() == 24
test.length offset=0 size= 4
test.rootdirectory offset=4 size= 4
test.objectname offset=8 size= 4
test.attributes offset=12 size= 4
test.securitydesc offset=16 size= 4
test.qos offset=20 size= 4

	*/




ULONG ObjectAttributesHelper::Length()
{
	// offsets are both the same regardless if 64-bit or 32-bit plus same size.
	return this->Data.Length;
}

VOID ObjectAttributesHelper::Length(ULONG val)
{
	this->Data.Length = val;
}

UINT64 ObjectAttributesHelper::RootDirectory()
{
	if (this->Data.Length == 24)
	{
		return this->Data.Machine86.RootDirectory;
	}
	else
	{
		return this->Data.Machine64.RootDirectory;
	}
}

VOID ObjectAttributesHelper::RootDirectory(UINT64 val)
{
	if (this->Data.Length == 24)
	{
		this->Data.Machine86.RootDirectory = (UINT)val;
	}
	else
	{
		this->Data.Machine64.RootDirectory = val;
	}

}

UINT64 ObjectAttributesHelper::ObjectName()
{
	if (this->Data.Length == 24)
	{
		return this->Data.Machine86.ObjectName;
	}
	else
	{
		return this->Data.Machine64.ObjectName;
	}
}

VOID ObjectAttributesHelper::ObjectName(UINT64 val)
{
	if (this->Data.Length == 24)
	{
		this->Data.Machine86.ObjectName = (ULONG)val;
	}
	else
	{
		this->Data.Machine64.ObjectName = val;
	}
}

ULONG ObjectAttributesHelper::Attributes()
{
	if (this->Data.Length == 24)
	{
		return this->Data.Machine86.Attributes;
	}
	else
	{
		return this->Data.Machine64.Attributes;
	}
}

VOID ObjectAttributesHelper::Attributes(ULONG val)
{
	if (this->Data.Length == 24) 
	{
		this->Data.Machine86.Attributes = val;
	}
	else
	{
		this->Data.Machine64.Attributes = val;
	}
}

UINT64 ObjectAttributesHelper::SecurityDescriptor()
{
	if (this->Data.Length == 24)
	{
		return this->Data.Machine86.SecurityDesc;
	}
	else
	{
		return this->Data.Machine64.SecurityDescriptor;
	}
}

VOID ObjectAttributesHelper::SecurityDescriptor(UINT64 val)
{
	if (this->Data.Length == 24) 
	{
		this->Data.Machine86.SecurityDesc = (UINT)val;
	}
	else
	{
		this->Data.Machine64.SecurityDescriptor = val;
	}
}

UINT64 ObjectAttributesHelper::SecurityQoS()
{
	if (this->Data.Length == 24)
	{
		return this->Data.Machine86.QoS;
	}
	else
	{
		return this->Data.Machine64.SecurityQOS;
	}
}

VOID ObjectAttributesHelper::SecurityQoS(UINT64 val)
{
	if (this->Data.Length == 24)
	{
		this->Data.Machine86.QoS = (UINT)val;
	}
	else
	{
		this->Data.Machine64.SecurityQOS = val;
	}
}

OBJECT_ATTRIBUTES* ObjectAttributesHelper::GetPointer()
{
	return (OBJECT_ATTRIBUTES*)&this->Data;
}
