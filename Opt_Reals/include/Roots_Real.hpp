// Copyright (C) 2005, 2006 International Business Machines and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
// If you have not obtained a copy of the EPL, you can find the license (version 2.0) at
// https://www.eclipse.org/legal/epl-2.0/
//
// Authors:  Carl Laird, Andreas Waechter     IBM                    2005-08-16
//           Daniel Doehring                  RWTH Aachen University 2022-11-20

#ifndef __ROOTS_REAL_HPP__
#define __ROOTS_REAL_HPP__

#include "IpTNLP.hpp"
#include "dco.hpp"
#include <vector>

using namespace Ipopt;

class Roots_Real: public TNLP
{
  const int NumStages, Degree, ConsOrder, NumStagesRef;
  const Number dtRef, dtExp;

  int NumEigVals, NumConstr, NumUnknowns, NumRoots;
  std::vector<Number> RealEigValsScaled, ImagEigValsScaled;
  Number RealMin, RealUB, RealMargin;
  std::vector<Number> x0;
  std::vector<Number> HullRealScaled, HullImagScaled;
  std::vector<Number> ImagDiff_over_RealDiff;

  bool OddDegree, UseHull;
  size_t i_min;

  Number *xMaxdt, Maxdt, InfPr;

public:
   /** Constructor */
   // NOTE: This is not the real application case
   Roots_Real(
      const int NumStages_,
      const int ConsOrder_,
      const int NumStagesRef_,
      const Number dtRef_,
      const std::string EigValFileName
   );

   Roots_Real(
      const int NumStages_,
      const int ConsOrder_,
      const int NumStagesRef_,
      const Number dtRef_,
      const std::string EigValFileName,
      const std::string HullPointPath
   );

   /** Destructor */
   virtual ~Roots_Real();

   /**@name Overloaded from TNLP */
   //@{
   /** Method to return some info about the NLP */
   virtual bool get_nlp_info(
      Index&          n,
      Index&          m,
      Index&          nnz_jac_g,
      Index&          nnz_h_lag,
      IndexStyleEnum& index_style
   );

   /** Method to return the bounds for my problem */
   virtual bool get_bounds_info(
      Index   n,
      Number* x_l,
      Number* x_u,
      Index   m,
      Number* g_l,
      Number* g_u
   );

   /** Method to return the starting point for the algorithm */
   virtual bool get_starting_point(
      Index   n,
      bool    init_x,
      Number* x,
      bool    init_z,
      Number* z_L,
      Number* z_U,
      Index   m,
      bool    init_lambda,
      Number* lambda
   );

   /** Method to return the objective value */
   virtual bool eval_f(
      Index         n,
      const Number* x,
      bool          new_x,
      Number&       obj_value
   );

   /** Method to return the gradient of the objective */
   virtual bool eval_grad_f(
      Index         n,
      const Number* x,
      bool          new_x,
      Number*       grad_f
   );

   /** Method to return the constraint residuals */
   virtual bool eval_g(
      Index         n,
      const Number* x,
      bool          new_x,
      Index         m,
      Number*       g
   );

   /** Method to return:
    *   1) The structure of the jacobian (if "values" is NULL)
    *   2) The values of the jacobian (if "values" is not NULL)
    */
   virtual bool eval_jac_g(
      Index         n,
      const Number* x,
      bool          new_x,
      Index         m,
      Index         nele_jac,
      Index*        iRow,
      Index*        jCol,
      Number*       values
   );

   /** Method to return:
    *   1) The structure of the hessian of the lagrangian (if "values" is NULL)
    *   2) The values of the hessian of the lagrangian (if "values" is not NULL)
    */
   virtual bool eval_h(
      Index         n,
      const Number* x,
      bool          new_x,
      Number        obj_factor,
      Index         m,
      const Number* lambda,
      bool          new_lambda,
      Index         nele_hess,
      Index*        iRow,
      Index*        jCol,
      Number*       values
   );

   /** This method is called when the algorithm is complete so the TNLP can store/write the solution */
   virtual void finalize_solution(
      SolverReturn               status,
      Index                      n,
      const Number*              x,
      const Number*              z_L,
      const Number*              z_U,
      Index                      m,
      const Number*              g,
      const Number*              lambda,
      Number                     obj_value,
      const IpoptData*           ip_data,
      IpoptCalculatedQuantities* ip_cq
   );
   //@}

   bool intermediate_callback(
      AlgorithmMode              mode,
      Index                      iter,
      Number                     obj_value,
      Number                     inf_pr,
      Number                     inf_du,
      Number                     mu,
      Number                     d_norm,
      Number                     regularization_size,
      Number                     alpha_du,
      Number                     alpha_pr,
      Index                      ls_trials,
      const IpoptData*           ip_data,
      IpoptCalculatedQuantities* ip_cq
   );

private:

   /**@name Methods to block default compiler methods.
    *
    * The compiler automatically generates the following three methods.
    *  Since the default compiler implementation is generally not what
    *  you want (for all but the most simple classes), we usually
    *  put the declarations of these methods in the private section
    *  and never implement them. This prevents the compiler from
    *  implementing an incorrect "default" behavior without us
    *  knowing. (See Scott Meyers book, "Effective C++")
    */
   //@{
   Roots_Real(
      const Roots_Real&
   );

   Roots_Real& operator=(
      const Roots_Real&
   );
   //@}
};

#endif
