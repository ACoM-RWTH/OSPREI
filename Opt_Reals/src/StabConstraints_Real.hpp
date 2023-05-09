// This code is published under the Eclipse Public License.
// If you have not obtained a copy of the EPL, you can find the license (version 2.0) at
// https://www.eclipse.org/legal/epl-2.0/
//
// Authors:  Daniel Doehring                  RWTH Aachen University 2022-11-20

#ifndef __STABCONSTRAINTSREAL_HPP__
#define __STABCONSTRAINTSREAL_HPP__

#include "Interpolation.hpp"

#include <complex>
#include <vector>

// For Odd Base Polynom => Even Lower Degree Polynomial
template <typename T>
void StabConstr_Real(const T* x, T* g, const int NumRoots, const int NumEigVals,
                     const std::vector<T>& RealEigValsScaled, const std::vector<T>& ImagEigValsScaled,
                     const T dtExp)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigValsScaled[i] / dtExp * x[NumRoots];
    ImagEV = ImagEigValsScaled[i] / dtExp * x[NumRoots];

    b = Lin_IntPol(x[0], RealEigValsScaled, ImagEigValsScaled);
    Radius = x[0]*x[0] + b*b;
    
    Real = (x[0] * (x[0] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[0]*ImagEV) / Radius;
    Prod = std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV / Radius, Imag - 2.*b*RealEV/Radius);

    for(size_t j = 1; j < NumRoots; j++) {
      b = Lin_IntPol(x[j], RealEigValsScaled, ImagEigValsScaled);
      Radius = x[j]*x[j] + b*b;

      Real  = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag  = (b*RealEV - x[j]*ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}

// For Odd Base Polynom => Even Lower Degree Polynomial
template <typename T>
void StabConstr_Real(const T* x, T* g, const int NumRoots, const int NumEigVals,
                     const std::vector<T>& RealEigVals, const std::vector<T>& ImagEigVals)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigVals[i] * x[NumRoots];
    ImagEV = ImagEigVals[i] * x[NumRoots];

    b = x[NumRoots] * Lin_IntPol(x[0], RealEigVals, ImagEigVals);
    Radius = x[0]*x[NumRoots] * x[0]*x[NumRoots] + b*b;
    
    Real = (x[0]*x[NumRoots] * (x[0]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[0]*x[NumRoots] * ImagEV) / Radius;
    Prod = std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV / Radius, Imag - 2.*b*RealEV/Radius);

    for(size_t j = 1; j < NumRoots; j++) {
      b = x[NumRoots] * Lin_IntPol(x[j], RealEigVals, ImagEigVals);
      Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

      Real  = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag  = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}


// For Even Base Polynomial => Odd Lower degree polynomial
template <typename T>
void StabConstr_Real(const T* x, T* g, const int NumRoots, const int NumEigVals,
                     const std::vector<T>& RealEigValsScaled, const std::vector<T>& ImagEigValsScaled,
                     const std::vector<T>& ImagDiff_over_RealDiff,
                     const T dtExp, const size_t i_min)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigValsScaled[i] / dtExp * x[NumRoots];
    ImagEV = ImagEigValsScaled[i] / dtExp * x[NumRoots];

    Prod = std::complex<T>(1. - RealEV/x[i_min], -ImagEV/x[i_min]);

    for(size_t j = 0; j < i_min; j++) {
      b = Lin_IntPol(x[j], RealEigValsScaled, ImagEigValsScaled, ImagDiff_over_RealDiff);
      Radius = x[j]*x[j] + b*b;

      Real  = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag  = (b*RealEV - x[j]*ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    for(size_t j = i_min + 1; j < NumRoots; j++) {
      b = Lin_IntPol(x[j], RealEigValsScaled, ImagEigValsScaled, ImagDiff_over_RealDiff);
      Radius = x[j]*x[j] + b*b;

      Real  = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag  = (b*RealEV - x[j]*ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}

// For Even Base Polynomial => Odd Lower degree polynomial
template <typename T>
void StabConstr_Real(const T* x, T* g, const int NumRoots, const int NumEigVals,
                     const std::vector<T>& RealEigVals, const std::vector<T>& ImagEigVals,
                     const size_t i_min)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigVals[i] * x[NumRoots];
    ImagEV = ImagEigVals[i] * x[NumRoots];

    Prod = std::complex<T>(1. - RealEV/(x[i_min]*x[NumRoots]), -ImagEV/(x[i_min]*x[NumRoots]));

    for(size_t j = 0; j < i_min; j++) {
      b = x[NumRoots] * Lin_IntPol(x[j], RealEigVals, ImagEigVals);
      Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

      Real  = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag  = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    for(size_t j = i_min + 1; j < NumRoots; j++) {
      b = x[NumRoots] * Lin_IntPol(x[j], RealEigVals, ImagEigVals);
      Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

      Real  = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag  = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}


// For Odd Base Polynom => Even Lower Degree Polynomial
template <typename T>
void StabConstr_Real(const T* x, T* g, const int NumRoots, const int NumEigVals,
                     const std::vector<T>& RealEigValsScaled, const std::vector<T>& ImagEigValsScaled,
                     const std::vector<T>& HullRealScaled, const std::vector<T>& HullImagScaled,
                     const T dtExp)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigValsScaled[i] / dtExp * x[NumRoots];
    ImagEV = ImagEigValsScaled[i] / dtExp * x[NumRoots];

    b = Lin_IntPol(x[0], HullRealScaled, HullImagScaled);
    Radius = x[0]*x[0] + b*b;
    
    Real = (x[0] * (x[0] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[0]*ImagEV) / Radius;
    Prod = std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);

    for(size_t j = 1; j < NumRoots; j++) {
      b = Lin_IntPol(x[j], HullRealScaled, HullImagScaled);
      Radius = x[j]*x[j] + b*b;

      Real  = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag  = (b*RealEV - x[j]*ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}

// For Odd Base Polynom => Even Lower Degree Polynomial
template <typename T>
void StabConstr_Real(const T* x, T* g, const int NumRoots, const int NumEigVals,
                     const std::vector<T>& RealEigVals, const std::vector<T>& ImagEigVals,
                     const std::vector<T>& HullReal, const std::vector<T>& HullImag)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigVals[i] * x[NumRoots];
    ImagEV = ImagEigVals[i] * x[NumRoots];

    b = x[NumRoots] * Lin_IntPol(x[0], HullReal, HullImag);
    Radius = x[0]*x[NumRoots] * x[0]*x[NumRoots] + b*b;
    
    Real = (x[0]*x[NumRoots] * (x[0]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[0]*x[NumRoots] * ImagEV) / Radius;
    Prod = std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);

    for(size_t j = 1; j < NumRoots; j++) {
      b = x[NumRoots] * Lin_IntPol(x[j], HullReal, HullImag);
      Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

      Real  = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag  = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}


// For Even Base Polynomial => Odd Lower degree polynomial
template <typename T>
void StabConstr_Real(const T* x, T* g, const int NumRoots, const int NumEigVals,
                     const std::vector<T>& RealEigValsScaled, const std::vector<T>& ImagEigValsScaled,
                     const std::vector<T>& HullRealScaled, const std::vector<T>& HullImagScaled,
                     const std::vector<T>& ImagDiff_over_RealDiff,
                     const T dtExp, const size_t i_min)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigValsScaled[i] / dtExp * x[NumRoots];
    ImagEV = ImagEigValsScaled[i] / dtExp * x[NumRoots];

    Prod = std::complex<T>(1. - RealEV / x[i_min], -ImagEV / x[i_min]);

    for(size_t j = 0; j < i_min; j++) {
      b = Lin_IntPol(x[j], HullRealScaled, HullImagScaled, ImagDiff_over_RealDiff);
      Radius = x[j]*x[j] + b*b;

      Real  = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag  = (b*RealEV - x[j]*ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    for(size_t j = i_min+1; j < NumRoots; j++) {
      b = Lin_IntPol(x[j], HullRealScaled, HullImagScaled, ImagDiff_over_RealDiff);
      Radius = x[j]*x[j] + b*b;

      Real  = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag  = (b*RealEV - x[j]*ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}

// For Even Base Polynomial => Odd Lower degree polynomial
template <typename T>
void StabConstr_Real(const T* x, T* g, const int NumRoots, const int NumEigVals,
                     const std::vector<T>& RealEigVals, const std::vector<T>& ImagEigVals,
                     const std::vector<T>& HullReal, const std::vector<T>& HullImag,
                     const size_t i_min)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigVals[i] * x[NumRoots];
    ImagEV = ImagEigVals[i] * x[NumRoots];

    Prod = std::complex<T>(1. - RealEV / (x[i_min]*x[NumRoots]), -ImagEV / (x[i_min]*x[NumRoots]) );

    for(size_t j = 0; j < i_min; j++) {
      b = x[NumRoots] * Lin_IntPol(x[j], HullReal, HullImag);
      Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

      Real  = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag  = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    for(size_t j = i_min+1; j < NumRoots; j++) {
      b = x[NumRoots] * Lin_IntPol(x[j], HullReal, HullImag);
      Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

      Real  = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag  = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}


// For Odd Base Polynom => Even Lower Degree Polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
void StabConstr_Real(const std::vector<T>& x, std::vector<T>& g, const int NumRoots, const int NumEigVals,
                     const std::vector<PT>& RealEigValsScaled, const std::vector<PT>& ImagEigValsScaled, 
                     const PT dtExp)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigValsScaled[i] / dtExp * x[NumRoots];
    ImagEV = ImagEigValsScaled[i] / dtExp * x[NumRoots];

    b = Lin_IntPol(x[0], RealEigValsScaled, ImagEigValsScaled);
    Radius = x[0]*x[0] + b*b;
    
    Real = (x[0] * (x[0] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[0]*ImagEV) / Radius;
    Prod = std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);

    for(size_t j = 1; j < NumRoots; j++) {
      b = Lin_IntPol(x[j], RealEigValsScaled, ImagEigValsScaled);
      Radius = x[j]*x[j] + b*b;

      Real = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag = (b*RealEV - x[j]*ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}

// For Odd Base Polynom => Even Lower Degree Polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
void StabConstr_Real(const std::vector<T>& x, std::vector<T>& g, const int NumRoots, const int NumEigVals,
                     const std::vector<PT>& RealEigVals, const std::vector<PT>& ImagEigVals)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigVals[i] * x[NumRoots];
    ImagEV = ImagEigVals[i] * x[NumRoots];

    b = x[NumRoots] * Lin_IntPol(x[0], RealEigVals, ImagEigVals);
    Radius = x[0]*x[NumRoots] * x[0]*x[NumRoots] + b*b;
    
    Real = (x[0]*x[NumRoots] * (x[0]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[0]*x[NumRoots] * ImagEV) / Radius;
    Prod = std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);

    for(size_t j = 1; j < NumRoots; j++) {
      b = x[NumRoots] * Lin_IntPol(x[j], RealEigVals, ImagEigVals);
      Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

      Real = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}


// For Even Base Polynomial => Odd Lower degree polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
void StabConstr_Real(const std::vector<T>& x, std::vector<T>& g, const int NumRoots, 
                     const int NumEigVals, 
                     const std::vector<PT>& RealEigValsScaled, const std::vector<PT>& ImagEigValsScaled,
                     const std::vector<PT>& ImagDiff_over_RealDiff,
                     const PT dtExp, const size_t i_min)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigValsScaled[i] / dtExp * x[NumRoots];
    ImagEV = ImagEigValsScaled[i] / dtExp * x[NumRoots];

    Prod = std::complex<T>(1. - RealEV / x[i_min], -ImagEV / x[i_min]);

    for(size_t j = 0; j < i_min; j++) {
      b = Lin_IntPol(x[j], RealEigValsScaled, ImagEigValsScaled, ImagDiff_over_RealDiff);
      Radius = x[j]*x[j] + b*b;

      Real = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag = (b*RealEV - x[j]*ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    for(size_t j = i_min + 1; j < NumRoots; j++) {
      b = Lin_IntPol(x[j], RealEigValsScaled, ImagEigValsScaled, ImagDiff_over_RealDiff);
      Radius = x[j]*x[j] + b*b;

      Real = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag = (b*RealEV - x[j]*ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}

// For Even Base Polynom => Odd Lower Degree Polynom
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
void StabConstr_Real(const std::vector<T>& x, std::vector<T>& g, const int NumRoots, 
                     const int NumEigVals, const std::vector<PT>& RealEigVals, const std::vector<PT>& ImagEigVals,
                     const size_t i_min)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigVals[i] * x[NumRoots];
    ImagEV = ImagEigVals[i] * x[NumRoots];

    Prod = std::complex<T>(1. - RealEV / (x[i_min]*x[NumRoots]), -ImagEV / (x[i_min]*x[NumRoots]) );

    for(size_t j = 0; j < i_min; j++) {
      b = x[NumRoots] * Lin_IntPol(x[j], RealEigVals, ImagEigVals);
      Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

      Real = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    for(size_t j = i_min + 1; j < NumRoots; j++) {
      b = x[NumRoots] * Lin_IntPol(x[j], RealEigVals, ImagEigVals);
      Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

      Real = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}


// For Odd Base Polynom => Even Lower Degree Polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
void StabConstr_Real(const std::vector<T>& x, std::vector<T>& g, const int NumRoots, const int NumEigVals,
                     const std::vector<PT>& RealEigValsScaled, const std::vector<PT>& ImagEigValsScaled,
                     const std::vector<PT>& HullRealScaled, const std::vector<PT>& HullImagScaled, 
                     const PT dtExp)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigValsScaled[i] / dtExp * x[NumRoots];
    ImagEV = ImagEigValsScaled[i] / dtExp * x[NumRoots];

    b = Lin_IntPol(x[0], HullRealScaled, HullImagScaled);
    Radius = x[0]*x[0] + b*b;
    
    Real = (x[0] * (x[0] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[0]*ImagEV) / Radius;
    Prod = std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);

    for(size_t j = 1; j < NumRoots; j++) {
      b = Lin_IntPol(x[j], HullRealScaled, HullImagScaled);
      Radius = x[j]*x[j] + b*b;

      Real = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag = (b*RealEV - x[j]*ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}

// For Odd Base Polynom => Even Lower Degree Polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
void StabConstr_Real(const std::vector<T>& x, std::vector<T>& g, const int NumRoots, const int NumEigVals,
                     const std::vector<PT>& RealEigVals, const std::vector<PT>& ImagEigVals,
                     const std::vector<PT>& HullReal, const std::vector<PT>& HullImag)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigVals[i] * x[NumRoots];
    ImagEV = ImagEigVals[i] * x[NumRoots];

    b = x[NumRoots] * Lin_IntPol(x[0], HullReal, HullImag);
    Radius = x[0]*x[NumRoots] * x[0]*x[NumRoots] + b*b;
    
    Real = (x[0]*x[NumRoots] * (x[0]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[0]*x[NumRoots] * ImagEV) / Radius;
    Prod = std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);

    for(size_t j = 1; j < NumRoots; j++) {
      b = x[NumRoots] * Lin_IntPol(x[j], HullReal, HullImag);
      Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

      Real = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}


// For Even Base Polynomial => Odd Lower degree polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
void StabConstr_Real(const std::vector<T>& x, std::vector<T>& g, const int NumRoots, const int NumEigVals,
                     const std::vector<PT>& RealEigValsScaled, const std::vector<PT>& ImagEigValsScaled,
                     const std::vector<PT>& HullRealScaled, const std::vector<PT>& HullImagScaled,
                     const std::vector<PT>& ImagDiff_over_RealDiff,
                     const PT dtExp, const size_t i_min)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigValsScaled[i] / dtExp * x[NumRoots];
    ImagEV = ImagEigValsScaled[i] / dtExp * x[NumRoots];

    Prod = std::complex<T>(1. - RealEV / x[i_min], -ImagEV / x[i_min]);

    for(size_t j = 0; j < i_min; j++) {
      b = Lin_IntPol(x[j], HullRealScaled, HullImagScaled, ImagDiff_over_RealDiff);
      Radius = x[j]*x[j] + b*b;

      Real = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag = (b*RealEV - x[j]*ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    for(size_t j = i_min + 1; j < NumRoots; j++) {
      b = Lin_IntPol(x[j], HullRealScaled, HullImagScaled, ImagDiff_over_RealDiff);
      Radius = x[j]*x[j] + b*b;

      Real = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag = (b*RealEV - x[j]*ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}

// For Even Base Polynomial => Odd Lower degree polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
void StabConstr_Real(const std::vector<T>& x, std::vector<T>& g, const int NumRoots, const int NumEigVals,
                     const std::vector<PT>& RealEigVals, const std::vector<PT>& ImagEigVals,
                     const std::vector<PT>& HullReal, const std::vector<PT>& HullImag,
                     const size_t i_min)
{
  T RealEV, ImagEV, b, Radius, Real, Imag;
  std::complex<T> Prod;

  for(size_t i = 0; i < NumEigVals; i++) {
    RealEV = RealEigVals[i] * x[NumRoots];
    ImagEV = ImagEigVals[i] * x[NumRoots];

    Prod = std::complex<T>(1. - RealEV / (x[i_min]*x[NumRoots]), -ImagEV / (x[i_min]*x[NumRoots]));

    for(size_t j = 0; j < i_min; j++) {
      b = x[NumRoots] * Lin_IntPol(x[j], HullReal, HullImag);
      Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

      Real = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    for(size_t j = i_min + 1; j < NumRoots; j++) {
      b = x[NumRoots] * Lin_IntPol(x[j], HullReal, HullImag);
      Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

      Real = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
      Imag = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
      Prod *= std::complex<T>(Real, Imag);

      Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
    }

    // From lower-degree to actual stability polynomial
    Prod *= std::complex<T>(RealEV, ImagEV);
    Prod += 1.;

    g[i] = std::abs(Prod);
  }
}

// Constraint for Hessian: Compute one constraint at a time
// For Odd Base Polynom => Even Lower Degree Polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
T StabConstr_Real_i(const std::vector<T>& x, const int NumRoots,
                    const std::vector<PT>& RealEigValsScaled, const std::vector<PT>& ImagEigValsScaled,
                    const int EigValInd, const PT dtExp)
{
  T b, Radius, Real, Imag;
  std::complex<T> Prod;

  const T RealEV = RealEigValsScaled[EigValInd] / dtExp * x[NumRoots];
  const T ImagEV = ImagEigValsScaled[EigValInd] / dtExp * x[NumRoots];

  b = Lin_IntPol(x[0], RealEigValsScaled, ImagEigValsScaled);
  Radius = x[0]*x[0] + b*b ;
  
  Real = (x[0] * (x[0] - RealEV) + b * (b - ImagEV)) / Radius;
  Imag = (b*RealEV - x[0]*ImagEV) / Radius;
  Prod = std::complex<T>(Real, Imag);

  Prod *= std::complex<T>(Real + 2.*b*ImagEV / Radius, Imag - 2.*b*RealEV/Radius);

  for(size_t j = 1; j < NumRoots; j++) {
    b = Lin_IntPol(x[j], RealEigValsScaled, ImagEigValsScaled);
    Radius = x[j]*x[j] + b*b;

    Real = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[j]*ImagEV) / Radius;
    Prod *= std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
  }

  // From lower-degree to actual stability polynomial
  Prod *= std::complex<T>(RealEV, ImagEV);
  Prod += 1.;

  return std::abs(Prod);
}

// Constraint for Hessian: Compute one constraint at a time
// For Odd Base Polynom => Even Lower Degree Polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
T StabConstr_Real_i(const std::vector<T>& x, const int NumRoots,
                    const std::vector<PT>& RealEigVals, const std::vector<PT>& ImagEigVals,
                    const int EigValInd)
{
  T b, Radius, Real, Imag;
  std::complex<T> Prod;

  const T RealEV = RealEigVals[EigValInd] * x[NumRoots];
  const T ImagEV = ImagEigVals[EigValInd] * x[NumRoots];

  b = x[NumRoots] * Lin_IntPol(x[0], RealEigVals, ImagEigVals);
  Radius = x[0]*x[NumRoots] * x[0]*x[NumRoots] + b*b ;
  
  Real = (x[0]*x[NumRoots] * (x[0]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
  Imag = (b*RealEV - x[0]*x[NumRoots] * ImagEV) / Radius;
  Prod = std::complex<T>(Real, Imag);

  Prod *= std::complex<T>(Real + 2.*b*ImagEV / Radius, Imag - 2.*b*RealEV/Radius);

  for(size_t j = 1; j < NumRoots; j++) {
    b = x[NumRoots] * Lin_IntPol(x[j], RealEigVals, ImagEigVals);
    Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

    Real = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
    Prod *= std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
  }

  // From lower-degree to actual stability polynomial
  Prod *= std::complex<T>(RealEV, ImagEV);
  Prod += 1.;

  return std::abs(Prod);
}


// Constraint for Hessian: Compute one constraint at a time
// For Even Base Polynomial => Odd Lower degree polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
T StabConstr_Real_i(const std::vector<T>& x, const int NumRoots,
                    const std::vector<PT>& RealEigValsScaled, const std::vector<PT>& ImagEigValsScaled,
                    const int EigValInd, 
                    const std::vector<PT>& ImagDiff_over_RealDiff,
                    const PT dtExp, const size_t i_min)
{
  T b, Radius, Real, Imag;
  std::complex<T> Prod;

  const T RealEV = RealEigValsScaled[EigValInd] / dtExp * x[NumRoots];
  const T ImagEV = ImagEigValsScaled[EigValInd] / dtExp * x[NumRoots];

  Prod = std::complex<T>(1. - RealEV / x[i_min], -ImagEV / x[i_min]);

  for(size_t j = 0; j < i_min; j++) {
    b = Lin_IntPol(x[j], RealEigValsScaled, ImagEigValsScaled, ImagDiff_over_RealDiff);
    Radius = x[j]*x[j] + b*b;

    Real = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[j]*ImagEV) / Radius;
    Prod *= std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
  }

  for(size_t j = i_min + 1; j < NumRoots; j++) {
    b = Lin_IntPol(x[j], RealEigValsScaled, ImagEigValsScaled, ImagDiff_over_RealDiff);
    Radius = x[j]*x[j] + b*b;

    Real = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[j]*ImagEV) / Radius;
    Prod *= std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
  }

  // From lower-degree to actual stability polynomial
  Prod *= std::complex<T>(RealEV, ImagEV);
  Prod += 1.;

  return std::abs(Prod);
}

// Constraint for Hessian: Compute one constraint at a time
// For Even Base Polynomial => Odd Lower degree polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
T StabConstr_Real_i(const std::vector<T>& x, const int NumRoots,
                    const std::vector<PT>& RealEigVals, const std::vector<PT>& ImagEigVals,
                    const int EigValInd, const size_t i_min)
{
  T b, Radius, Real, Imag;
  std::complex<T> Prod;

  const T RealEV = RealEigVals[EigValInd] * x[NumRoots];
  const T ImagEV = ImagEigVals[EigValInd] * x[NumRoots];

  Prod = std::complex<T>(1. - RealEV / (x[i_min]*x[NumRoots]), -ImagEV / (x[i_min]*x[NumRoots]));

  for(size_t j = 0; j < i_min; j++) {
    b = x[NumRoots] * Lin_IntPol(x[j], RealEigVals, ImagEigVals);
    Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

    Real = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
    Prod *= std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
  }

  for(size_t j = i_min + 1; j < NumRoots; j++) {
    b = x[NumRoots] * Lin_IntPol(x[j], RealEigVals, ImagEigVals);
    Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

    Real = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
    Prod *= std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
  }

  // From lower-degree to actual stability polynomial
  Prod *= std::complex<T>(RealEV, ImagEV);
  Prod += 1.;

  return std::abs(Prod);
}


// For Even Base Polynomial => Odd Lower degree polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
T StabConstr_Real_i(const std::vector<T>& x, const int NumRoots,
                    const std::vector<PT>& RealEigValsScaled, const std::vector<PT>& ImagEigValsScaled,
                    const int EigValInd,
                    const std::vector<PT>& HullRealScaled, const std::vector<PT>& HullImagScaled,
                    const std::vector<PT>& ImagDiff_over_RealDiff,
                    const PT dtExp)
{
  T b, Radius, Real, Imag;
  std::complex<T> Prod;

  const T RealEV = RealEigValsScaled[EigValInd] / dtExp * x[NumRoots];
  const T ImagEV = ImagEigValsScaled[EigValInd] / dtExp * x[NumRoots];

  b = Lin_IntPol(x[0], HullRealScaled, HullImagScaled, ImagDiff_over_RealDiff);
  Radius = x[0]*x[0] + b*b ;
  
  Real = (x[0] * (x[0] - RealEV) + b * (b - ImagEV)) / Radius;
  Imag = (b*RealEV - x[0]*ImagEV) / Radius;
  Prod = std::complex<T>(Real, Imag);

  Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);

  for(size_t j = 1; j < NumRoots; j++) {
    b = Lin_IntPol(x[j], HullRealScaled, HullImagScaled, ImagDiff_over_RealDiff);
    Radius = x[j]*x[j] + b*b;

    Real = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[j]*ImagEV) / Radius;
    Prod *= std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
  }

  // From lower-degree to actual stability polynomial
  Prod *= std::complex<T>(RealEV, ImagEV);
  Prod += 1.;

  return std::abs(Prod);
}

// For Even Base Polynomial => Odd Lower degree polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
T StabConstr_Real_i(const std::vector<T>& x, const int NumRoots,
                    const std::vector<PT>& RealEigVals, const std::vector<PT>& ImagEigVals,
                    const int EigValInd,
                    const std::vector<PT>& HullReal, const std::vector<PT>& HullImag)
{
  T b, Radius, Real, Imag;
  std::complex<T> Prod;

  const T RealEV = RealEigVals[EigValInd] * x[NumRoots];
  const T ImagEV = ImagEigVals[EigValInd] * x[NumRoots];

  b = x[NumRoots] * Lin_IntPol(x[0], HullReal, HullImag);
  Radius = x[0]*x[NumRoots] * x[0]*x[NumRoots] + b*b ;
  
  Real = (x[0]*x[NumRoots] * (x[0]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
  Imag = (b*RealEV - x[0]*x[NumRoots] * ImagEV) / Radius;
  Prod = std::complex<T>(Real, Imag);

  Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);

  for(size_t j = 1; j < NumRoots; j++) {
    b = x[NumRoots] * Lin_IntPol(x[j], HullReal, HullImag);
    Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

    Real = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
    Prod *= std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
  }

  // From lower-degree to actual stability polynomial
  Prod *= std::complex<T>(RealEV, ImagEV);
  Prod += 1.;

  return std::abs(Prod);
}


// For Even Base Polynomial => Odd Lower degree polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
T StabConstr_Real_i(const std::vector<T>& x, const int NumRoots,
                    const std::vector<PT>& RealEigValsScaled, const std::vector<PT>& ImagEigValsScaled,
                    const int EigValInd,
                    const std::vector<PT>& HullRealScaled, const std::vector<PT>& HullImagScaled,
                    const std::vector<PT>& ImagDiff_over_RealDiff,
                    const PT dtExp, const size_t i_min)
{
  T b, Radius, Real, Imag;
  std::complex<T> Prod;

  const T RealEV = RealEigValsScaled[EigValInd] / dtExp * x[NumRoots];
  const T ImagEV = ImagEigValsScaled[EigValInd] / dtExp * x[NumRoots];

  Prod = std::complex<T>(1. - RealEV / x[i_min], -ImagEV / x[i_min]);

  for(size_t j = 0; j < i_min; j++) {
    b = Lin_IntPol(x[j], HullRealScaled, HullImagScaled, ImagDiff_over_RealDiff);
    Radius = x[j]*x[j] + b*b;

    Real = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[j]*ImagEV) / Radius;
    Prod *= std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
  }

  for(size_t j = i_min + 1; j < NumRoots; j++) {
    b = Lin_IntPol(x[j], HullRealScaled, HullImagScaled, ImagDiff_over_RealDiff);
    Radius = x[j]*x[j] + b*b;

    Real = (x[j]*(x[j] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[j]*ImagEV) / Radius;
    Prod *= std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
  }

  // From lower-degree to actual stability polynomial
  Prod *= std::complex<T>(RealEV, ImagEV);
  Prod += 1.;

  return std::abs(Prod);
}

// For Even Base Polynomial => Odd Lower degree polynomial
template<typename T, typename PT> // T: for dco types PT: Passive Type: For usual real types
T StabConstr_Real_i(const std::vector<T>& x, const int NumRoots,
                    const std::vector<PT>& RealEigVals, const std::vector<PT>& ImagEigVals,
                    const int EigValInd,
                    const std::vector<PT>& HullReal, const std::vector<PT>& HullImag,
                    const size_t i_min)
{
  T b, Radius, Real, Imag;
  std::complex<T> Prod;

  const T RealEV = RealEigVals[EigValInd] * x[NumRoots];
  const T ImagEV = ImagEigVals[EigValInd] * x[NumRoots];

  Prod = std::complex<T>(1. - RealEV / (x[i_min]*x[NumRoots]), -ImagEV / (x[i_min]*x[NumRoots]));

  for(size_t j = 0; j < i_min; j++) {
    b = x[NumRoots] * Lin_IntPol(x[j], HullReal, HullImag);
    Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

    Real = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
    Prod *= std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
  }

  for(size_t j = i_min + 1; j < NumRoots; j++) {
    b = x[NumRoots] * Lin_IntPol(x[j], HullReal, HullImag);
    Radius = x[j]*x[NumRoots] * x[j]*x[NumRoots] + b*b;

    Real = (x[j]*x[NumRoots] * (x[j]*x[NumRoots] - RealEV) + b * (b - ImagEV)) / Radius;
    Imag = (b*RealEV - x[j]*x[NumRoots] * ImagEV) / Radius;
    Prod *= std::complex<T>(Real, Imag);

    Prod *= std::complex<T>(Real + 2.*b*ImagEV/Radius, Imag - 2.*b*RealEV/Radius);
  }

  // From lower-degree to actual stability polynomial
  Prod *= std::complex<T>(RealEV, ImagEV);
  Prod += 1.;

  return std::abs(Prod);
}

#endif