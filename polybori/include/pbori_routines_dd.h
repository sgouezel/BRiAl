// -*- c++ -*-
//*****************************************************************************
/** @file pbori_routines_cuddext.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-23
 *
 * This file includes files, which define function templates related to
 * decision diagrams. It should be loaded from pbori_routines.h only
 *
 * @par Copyright:
 *   (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and @n
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.3  2006/08/29 14:36:08  dreyer
 * CHANGE: moved functionality to CDDOperations
 *
 * Revision 1.2  2006/08/29 10:37:56  dreyer
 * CHANGE: non-const version of diagram() now internalDiagram()
 *
 * Revision 1.1  2006/08/23 14:24:54  dreyer
 * ADD: BooleSet::usedVariables and infrastructure
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// get addition definitions
#include "CTermIter.h"
#include "PBoRiOutIter.h"
#include <set>
#include <vector>


BEGIN_NAMESPACE_PBORI

/// @func dd_last_lexicographical_term
/// @brief Get last term (wrt. lexicographical order).
template<class DDType, class OutputType>
OutputType
dd_last_lexicographical_term(const DDType& dd, type_tag<OutputType>) {

  typedef typename DDType::idx_type idx_type;
  typedef typename DDType::size_type size_type;
  typedef OutputType term_type;

  term_type result(true);

  if (dd.emptiness())
    result = false;
  else {

    size_type nlen = std::distance(dd.lastBegin(), dd.lastEnd());

    // store indices in list
    std::vector<idx_type> indices(nlen);

    // iterator, which uses changeAssign to insert variable
    // wrt. given indices to a monomial
    PBoRiOutIter<term_type, idx_type, change_assign<term_type> >  
      outiter(result);
    
    // insert backward (for efficiency reasons)
    reversed_inter_copy(dd.lastBegin(), dd.lastEnd(), indices, outiter);
  }

  return result;
}


END_NAMESPACE_PBORI
