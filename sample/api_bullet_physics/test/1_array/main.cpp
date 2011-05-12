/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2011 Advanced Micro Devices, Inc.  http://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

///Contributors:
///2011-may-1: This file was created by Erwin Coumans

#include "bullet_physics/util/bt_inplace_array.h"
#include <stdio.h>

struct btBla
{
	PfxInplaceArray<int,30> m_ints;
};

int main(int argc, char* argv[])
{

	btBla bla;
	int sz = sizeof(btBla);
	bla.m_ints.resize(4);
	for (int i=0;i<30;i++)//bla.m_ints.size();i++)
	{
		bla.m_ints.at(i)=10;
	}

	return 0;
}