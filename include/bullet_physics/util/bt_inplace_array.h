/*
In-place array for the Bullet Physics Library, http://bulletphysics.org
Copyright (c) 2003-2011 Erwin Coumans

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PFX_INPLACE_ARRAY_H
#define PFX_INPLACE_ARRAY_H

#include <new>
//#include "../../include/conversions.h"
#ifndef VMASSERT
#define LOCALASSERT
#include <assert.h>
#define VMASSERT(a) assert(a);
#endif

template <typename T, int SIZE>
class PfxInplaceArray
{
	int					m_size;
	int					m_capacity;
	T					m_data[SIZE];
public:

	PfxInplaceArray()
	{
		m_size=0;
		m_capacity = SIZE;
	}
	inline int size() const
	{	
		return m_size;
	}
	inline const T& at(int n) const
	{
		return m_data[n];
	}

	inline T& at(int n)
	{
		VMASSERT(n<size());
		return m_data[n];
	}

	inline const T& operator[](int n) const
	{
		VMASSERT(n<size());
		return m_data[n];
	}

	inline T& operator[](int n)
	{
		VMASSERT(n<size());
		return m_data[n];
	}

	inline void push_back(const T& _Val)
	{	
		int sz = size();
		assert(sz<capacity());
		new ( &m_data[m_size] ) T(_Val);
//		m_data[size()] = _Val;			
		m_size++;
	}
	inline int capacity() const
	{	
		return m_capacity;
	}

	inline void	resize(int newsize, const T& fillData=T())
	{
		VMASSERT(newsize<=m_capacity);
		int curSize = size();

		if (newsize < curSize)
		{
			for(int i = newsize; i < curSize; i++)
			{
				m_data[i].~T();
			}
		} else
		{
			
			for (int i=curSize;i<newsize;i++)
			{
				new ( &m_data[i]) T(fillData);
			}

		}

		m_size = newsize;
	}

		void	swap(int index0,int index1)
		{
#ifdef BT_USE_MEMCPY
			char	temp[sizeof(T)];
			memcpy(temp,&m_data[index0],sizeof(T));
			memcpy(&m_data[index0],&m_data[index1],sizeof(T));
			memcpy(&m_data[index1],temp,sizeof(T));
#else
			T temp = m_data[index0];
			m_data[index0] = m_data[index1];
			m_data[index1] = temp;
#endif //BT_USE_PLACEMENT_NEW

		}

	
		class less
		{
			public:

				bool operator() ( const T& a, const T& b )
				{
					return ( a < b );
				}
		};
	
		template <typename L>
		void quickSortInternal(L CompareFunc,int lo, int hi)
		{
		//  lo is the lower index, hi is the upper index
		//  of the region of array a that is to be sorted
			int i=lo, j=hi;
			T x=m_data[(lo+hi)/2];

			//  partition
			do
			{    
				while (CompareFunc(m_data[i],x)) 
					i++; 
				while (CompareFunc(x,m_data[j])) 
					j--;
				if (i<=j)
				{
					swap(i,j);
					i++; j--;
				}
			} while (i<=j);

			//  recursion
			if (lo<j) 
				quickSortInternal( CompareFunc, lo, j);
			if (i<hi) 
				quickSortInternal( CompareFunc, i, hi);
		}


		template <typename L>
		void quickSort(L CompareFunc)
		{
			//don't sort 0 or 1 elements
			if (size()>1)
			{
				quickSortInternal(CompareFunc,0,size()-1);
			}
		}


};
#ifdef LOCALASSERT
#undef VMASSERT
#undef LOCALASSERT
#endif //LOCALASSERT

#endif //PFX_INPLACE_ARRAY_H
