static const char* boundSearchKernelsDX11= \
"/*\n"
"Bullet Continuous Collision Detection and Physics Library\n"
"Copyright (c) 2011 Advanced Micro Devices, Inc.  http://bulletphysics.org\n"
"\n"
"This software is provided 'as-is', without any express or implied warranty.\n"
"In no event will the authors be held liable for any damages arising from the use of this software.\n"
"Permission is granted to anyone to use this software for any purpose, \n"
"including commercial applications, and to alter it and redistribute it freely, \n"
"subject to the following restrictions:\n"
"\n"
"1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.\n"
"2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.\n"
"3. This notice may not be removed or altered from any source distribution.\n"
"*/\n"
"//Author Takahiro Harada\n"
"\n"
"typedef uint u32;\n"
"\n"
"#define GET_GROUP_IDX groupIdx.x\n"
"#define GET_LOCAL_IDX localIdx.x\n"
"#define GET_GLOBAL_IDX globalIdx.x\n"
"#define GROUP_LDS_BARRIER GroupMemoryBarrierWithGroupSync()\n"
"#define DEFAULT_ARGS uint3 globalIdx : SV_DispatchThreadID, uint3 localIdx : SV_GroupThreadID, uint3 groupIdx : SV_GroupID\n"
"#define AtomInc(x) InterlockedAdd(x, 1)\n"
"#define AtomInc1(x, out) InterlockedAdd(x, 1, out)\n"
"\n"
"\n"
"\n"
"typedef struct\n"
"{\n"
"	u32 m_key; \n"
"	u32 m_value;\n"
"}SortData;\n"
"\n"
"\n"
"\n"
"cbuffer SortCB : register( b0 )\n"
"{\n"
"	u32 m_nSrc;\n"
"	u32 m_nDst;\n"
"	u32 m_padding[2];\n"
"};\n"
"\n"
"\n"
"StructuredBuffer<SortData> src : register( t0 );\n"
"RWStructuredBuffer<u32> dst : register( u0 );\n"
"\n"
"\n"
"[numthreads(64, 1, 1)]\n"
"void SearchSortDataLowerKernel( DEFAULT_ARGS )\n"
"{\n"
"	int gIdx = GET_GLOBAL_IDX;\n"
"	u32 nSrc = m_nSrc;\n"
"	u32 nDst = m_nDst;\n"
"\n"
"	if( gIdx < nSrc )\n"
"	{\n"
"		SortData iData;\n"
"		SortData jData;\n"
"		if( gIdx==0 ) iData.m_key = iData.m_value = (u32)-1;\n"
"		else iData = src[gIdx-1];\n"
"\n"
"		if( gIdx==nSrc ) jData.m_key = jData.m_value = nDst;\n"
"		else jData = src[gIdx];\n"
"\n"
"		if( iData.m_key != jData.m_key )\n"
"		{\n"
"//			for(u32 k=iData.m_key+1; k<=min(jData.m_key, nDst-1); k++)\n"
"			u32 k = jData.m_key;\n"
"			{\n"
"				dst[k] = gIdx;\n"
"			}\n"
"		}\n"
"	}\n"
"}\n"
"\n"
"[numthreads(64, 1, 1)]\n"
"void SearchSortDataUpperKernel( DEFAULT_ARGS )\n"
"{\n"
"	int gIdx = GET_GLOBAL_IDX;\n"
"	u32 nSrc = m_nSrc;\n"
"	u32 nDst = m_nDst;\n"
"\n"
"	if( gIdx < nSrc+1 )\n"
"	{\n"
"		SortData iData;\n"
"		SortData jData;\n"
"		if( gIdx==0 ) iData.m_key = iData.m_value = 0;\n"
"		else iData = src[gIdx-1];\n"
"\n"
"		if( gIdx==nSrc ) jData.m_key = jData.m_value = nDst;\n"
"		else jData = src[gIdx];\n"
"\n"
"		if( iData.m_key != jData.m_key )\n"
"		{\n"
"//			for(u32 k=iData.m_key; k<min(jData.m_key, nDst); k++)\n"
"			u32 k = iData.m_key;\n"
"			{\n"
"				dst[k] = gIdx;\n"
"			}\n"
"		}\n"
"	}\n"
"}\n"
"\n"
;
