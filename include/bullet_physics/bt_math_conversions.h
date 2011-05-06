
#ifndef BT_MATH_CONVERSIONS_H
#define BT_MATH_CONVERSIONS_H

#include <float.h>

#include "physics_effects.h"
#include <assert.h>

#define MAXSIZE 256
#define MAXINDICESPERFACE 64

typedef sce::PhysicsEffects::PfxVector3 btVector3;

template <class T>
inline const T& VMMIN(const T& a, const T& b) 
{
  return a < b ? a : b ;
}

template <class T>
inline const T& VMMAX(const T& a, const T& b) 
{
  return  a > b ? a : b;
}

#define VM_FLT_MAX FLT_MAX
///for vector3
#define VMDOT(a,b) dot(a,b)
#define VMLERP(v0,v1,factor) lerp(factor,v0,v1)
#define VMGETX(v) v.getX()
#define VMGETY(v) v.getY()
#define VMGETZ(v) v.getZ()
#define VMCROSS(a,b) cross(a,b)
#define VMLENGTH(v) length(v)
#define VMNORMALIZE(a) a = normalize(a)
#define VMNORMALIZED(a) normalize(a)
#define VMSET(v,x,y,z) v=btVector3(x,y,z)
#define VM_SETMIN(a,b) a = minPerElem(a,b)
#define VM_SETMAX(a,b) a = maxPerElem(a,b)
#define VMMUL(a,b) mulPerElem(a,b)
#define VMMINAXIS(v) VMGETX(v) < VMGETY(v) ? (VMGETX(v) <VMGETZ(v) ? 0 : 2) : (VMGETY(v) <VMGETZ(v) ? 1 : 2)
#define VMMAXAXIS(v) VMGETX(v) < VMGETY(v) ? (VMGETY(v) <VMGETZ(v) ? 2 : 1) : (VMGETX(v) <VMGETZ(v) ? 2 : 0)


//for transform
#define VMGETBASIS(t) t.getUpper3x3()
#define VMGETTRANSLATION(t) t.getTranslation()
#define VMTRANS(t,v) t*v+t.getTranslation()


#define VMASSERT(a) assert(a)

template<typename T> inline void VMSWAP(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}


typedef sce::PhysicsEffects::PfxTransform3 btTransform;
typedef float btScalar;



#endif// BT_MATH_CONVERSIONS_H
