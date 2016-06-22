/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkDualQuaternion.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkDualQuaternion_h
#define __vtkDualQuaternion_h

#include "vtkQuaternion.h"

template<typename T> class vtkDualQuaternion
{
public:
  vtkDualQuaternion();
  vtkDualQuaternion(const T& rw, const T& rx, const T& ry, const T& rz,
                    const T& dw, const T& dx, const T& dy, const T& dz);
  vtkDualQuaternion(const T realDual[8]);
  vtkDualQuaternion(const vtkQuaternion<T>& real, const vtkQuaternion<T>& dual);
  vtkDualQuaternion(const vtkQuaternion<T>& rotation, const T translation[3]);

  // Description:
  // Real (rotation) part of the dual quaternion.
  // @sa GetDual()
  const vtkQuaternion<T>& GetReal() const;

  // Description:
  // Dual (translation) part of the dual quaternion.
  // @sa GetReal()
  const vtkQuaternion<T>& GetDual() const;

  // Description:
  // Invert the quaternion in place.
  // @sa Inverse()
  void Invert();

  // Description:
  // Conjugate / SquaredNorm
  // @sa Invert(), Inverse2()
  vtkDualQuaternion Inverse() const;

  // Description:
  // Inverse computed differently than Inverse()
  // @sa Invert(), Inverse()
  vtkDualQuaternion Inverse2() const;

  // Description:
  // Normalize the quaternion in place.
  // @sa Normalized()
  void Normalize();

  // Description:
  // Return a normalized quaternion.
  // @sa Normalize()
  vtkDualQuaternion<T> Normalized() const;

  // Description:
  // Conjugate the quaternion in place.
  // @sa Conjugated()
  void Conjugate();

  // Description:
  // Return a conjugated quaternion.
  // @sa Conjugate()
  vtkDualQuaternion<T> Conjugated() const;

  void SetRotationTranslation(const vtkQuaternion<T>& rotation, const T translation[3]);
  void SetRotation(const vtkQuaternion<T>& rotation);

  void SetTranslation(const T translation[3]);
  void GetTranslation(T translation[3]) const;

  void GetPosition(T position[3]) const;

  bool operator==(const vtkDualQuaternion<T>& dq)const;
  vtkDualQuaternion<T> operator+(const vtkDualQuaternion<T>& dq) const;
  vtkDualQuaternion<T> operator-(const vtkDualQuaternion<T>& dq) const;
  vtkDualQuaternion<T> operator-() const;
  vtkDualQuaternion<T> operator*(const T& scalar) const;
  vtkDualQuaternion<T> operator*(const vtkDualQuaternion<T>& dq) const;
  vtkDualQuaternion<T> operator/(const T& scalar) const;
  vtkDualQuaternion<T> operator/(const vtkDualQuaternion<T>& dq) const;

  vtkDualQuaternion<T> Lerp(const T& t, const vtkDualQuaternion<T>& dq) const;
  vtkDualQuaternion<T> ScLerp(const T& t, const vtkDualQuaternion<T>& _dq) const;
  vtkDualQuaternion<T> ScLerp2(const T& t, const vtkDualQuaternion<T>& dq) const;
  vtkDualQuaternion<T> ScLerp3(const T& t, const vtkDualQuaternion<T>& dq) const;

  vtkDualQuaternion<T> Dot(const vtkDualQuaternion<T>& dq) const;

  void LengthSquared(T& real, T& dual) const;
  void ReciprocalLengthSquared(T& real, T& dual) const;
  void ToScrew(T& angle, T& pitch, T dir[3], T moment[3]) const;
  void FromScrew(T angle, T pitch, T dir[3], T moment[3]);

  void ToMatrix4x4(vtkMatrix4x4* output) const;
  void FromMatrix4x4(const vtkMatrix4x4* input);

  void TransformPoint(const T point[3], T transformedPoint[3]) const;

protected:
  vtkQuaternion<T> Real;
  vtkQuaternion<T> Dual;
};


// Description:
// Several macros to define the various operator overloads for the quaternions.
// These are necessary for the derived classes that are commonly used.
#define vtkDualQuaternionNormalized(quaternionType, type) \
quaternionType Normalized() const \
{ \
  return quaternionType(vtkDualQuaternion<type>::Normalized()); \
}
#define vtkDualQuaternionConjugated(quaternionType, type) \
quaternionType Conjugated() const \
{ \
  return quaternionType(vtkDualQuaternion<type>::Conjugated()); \
}
#define vtkDualQuaternionInverse(quaternionType, type) \
quaternionType Inverse() const \
{ \
  return quaternionType(vtkDualQuaternion<type>::Inverse()); \
}
#define vtkDualQuaternionScLerp(quaternionType, type) \
quaternionType ScLerp(type t, const quaternionType& q) const \
{ \
  return quaternionType(vtkDualQuaternion<type>::ScLerp(t, q)); \
}
#define vtkDualQuaternionOperatorPlus(quaternionType, type) \
inline quaternionType operator+(const quaternionType& q) const \
{ \
  return quaternionType(static_cast< vtkDualQuaternion<type> > (*this) + \
    static_cast< vtkDualQuaternion<type> > (q)); \
}
#define vtkDualQuaternionOperatorMinus(quaternionType, type) \
inline quaternionType operator-(const quaternionType& q) const \
{ \
  return quaternionType(static_cast< vtkDualQuaternion<type> > (*this) - \
    static_cast< vtkDualQuaternion<type> > (q)); \
}
#define vtkDualQuaternionOperatorMultiply(quaternionType, type) \
inline quaternionType operator*(const quaternionType& q) const \
{ \
  return quaternionType(static_cast< vtkDualQuaternion<type> > (*this) * \
    static_cast< vtkDualQuaternion<type> > (q)); \
}
#define vtkDualQuaternionOperatorMultiplyScalar(quaternionType, type) \
inline quaternionType operator*(const type& scalar) const \
{ \
  return quaternionType(static_cast< vtkDualQuaternion<type> > (*this) * scalar); \
}
#define vtkDualQuaternionOperatorDivide(quaternionType, type) \
inline quaternionType operator/(const quaternionType& q) const \
{ \
  return quaternionType(static_cast< vtkDualQuaternion<type> > (*this) / \
    static_cast< vtkDualQuaternion<type> > (q)); \
}
#define vtkDualQuaternionOperatorDivideScalar(quaternionType, type) \
inline quaternionType operator/(const type& scalar) const \
{ \
  return quaternionType(static_cast< vtkDualQuaternion<type> > (*this) / scalar); \
}

#define vtkDualQuaternionOperatorMacro(quaternionType, type) \
vtkDualQuaternionNormalized(quaternionType, type) \
vtkDualQuaternionConjugated(quaternionType, type) \
vtkDualQuaternionInverse(quaternionType, type) \
vtkDualQuaternionScLerp(quaternionType, type) \
vtkDualQuaternionOperatorPlus(quaternionType, type) \
vtkDualQuaternionOperatorMinus(quaternionType, type) \
vtkDualQuaternionOperatorMultiply(quaternionType, type) \
vtkDualQuaternionOperatorMultiplyScalar(quaternionType, type) \
vtkDualQuaternionOperatorDivide(quaternionType, type) \
vtkDualQuaternionOperatorDivideScalar(quaternionType, type)


// .NAME vtkDualQuaternionf - Float dual quaternion type.
//
// .SECTION Description
// This class is uses vtkDualQuaternion with float type data.
// For futher description, see the templated class vtkDualQuaternion.
// @sa vtkDualQuaterniond vtkDualQuaternion vtkQuaternionf
class vtkDualQuaternionf : public vtkDualQuaternion<float>
{
public:
  vtkDualQuaternionf() {}
  explicit vtkDualQuaternionf(float rw, float rx, float ry, float rz,
    float dw, float dx, float dy, float dz)
    : vtkDualQuaternion<float>(rw, rx, ry, rz, dw, dx, dy, dz) {}
  explicit vtkDualQuaternionf(const float *init) : vtkDualQuaternion<float>(init) {}
  vtkDualQuaternionOperatorMacro(vtkDualQuaternionf, float)
protected:
  explicit vtkDualQuaternionf(const vtkDualQuaternion<float>& init) : vtkDualQuaternion<float>(init.GetReal(), init.GetDual()) {}
};

// .NAME vtkDualQuaterniond - Double dual quaternion type.
//
// .SECTION Description
// This class is uses vtkDualQuaternion with double type data.
// For futher description, seethe templated class vtkDualQuaternion.
// @sa vtkDualQuaternionf vtkDualQuaternion vtkQuaterniond
class vtkDualQuaterniond : public vtkDualQuaternion<double>
{
public:
  vtkDualQuaterniond() {}
  explicit vtkDualQuaterniond(double rw, double rx, double ry, double rz,
    double dw, double dx, double dy, double dz)
    : vtkDualQuaternion<double>(rw, rx, ry, rz, dw, dx, dy, dz) {}
  explicit vtkDualQuaterniond(const double *init) : vtkDualQuaternion<double>(init) {}
  vtkDualQuaternionOperatorMacro(vtkDualQuaterniond, double);
protected:
  explicit vtkDualQuaterniond(const vtkDualQuaternion<double>& init) : vtkDualQuaternion<double>(init.GetReal(), init.GetDual()) {}
};

#include "vtkDualQuaternion.txx"

#endif
