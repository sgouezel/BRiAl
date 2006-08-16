// -*- c++ -*-
//*****************************************************************************
/** @file pbori_algo.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-07
 *
 * This file includes some templates of simple transformations and similar
 * procedures. 
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
 * Revision 1.2  2006/08/16 16:11:17  dreyer
 * + first lowlevel variant running
 *
 * Revision 1.1  2006/08/15 16:03:58  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

// include polybori's definitions
#include "pbori_defs.h"

// get polybori's functionals
#include "pbori_func.h"

#ifndef pbori_algo_int_h_
#define pbori_algo_int_h_

BEGIN_NAMESPACE_PBORI


/// A first version
/// Function templates extracting minimal elements of dd wrt. inclusion
/// Assumption, navi is navigator of dd
template <class NaviType, class DDType, class DDType2>
//DDType
NaviType
dd_minimal_elements(NaviType navi, DDType dd, DDType2& multiples, int) {


  if (!navi.isConstant()) {     // Not at end of path

    DdManager* man = dd.manager().getManager();
    int nlen = Cudd_zddCount(man, navi);


    if(nlen == 2) {
      multiples = navi;
      //Cudd_Ref(navi);

      std::vector<int> indices (Cudd_SupportSize(man,navi));
      std::vector<int>::iterator iter(indices.begin()), iend(indices.end());
      bool is_reducible = false;

      int red_idx; int used = 0;

      while(!multiples.isConstant()) {
        *iter = *multiples;
        if((multiples.thenBranch() == multiples.elseBranch())){
          red_idx = *multiples;
          multiples.incrementElse();
          is_reducible = true;
        }
        else
          multiples.incrementThen();
        used++;
        ++iter;
      }
      indices.resize(used);

      if (is_reducible) {
        std::vector<int>::const_reverse_iterator start(indices.rbegin()),
          finish(indices.rend());
        
        int nmax = dd.nVariables();
        
      Cudd_Ref(multiples);
      NaviType emptyset = Cudd_ReadZero(man);


      NaviType tmp, tmpnavi = multiples;

      Cudd_Ref(tmpnavi);

      int i = nmax-1;
      
      while(start != finish){
        
        while(i > *start) {
          
          tmp = cuddZddGetNode(man, i, multiples, multiples);
          Cudd_Ref(tmp);
          Cudd_Deref(multiples);
          multiples = tmp;
          --i;
        }
        tmp = cuddZddGetNode(man, i, multiples, emptyset);

        Cudd_Ref(tmp);
        Cudd_Deref(multiples);
        multiples = tmp;

        tmp =  cuddZddGetNode(man, i, tmpnavi, emptyset);
        Cudd_Ref(tmp);
        Cudd_Deref(tmpnavi);
        tmpnavi = tmp;

        --i;
        ++start;
      }

      //  tmp = Cudd_zddSubset0(man, navi, red_idx);
      //Cudd_Ref(tmp);

      navi = tmpnavi;
      return navi;
      }
    }



    if(nlen == 1) {
      multiples = navi;
      Cudd_Ref(navi);

      std::vector<int> indices (Cudd_SupportSize(man,navi));
      std::vector<int>::iterator iter(indices.begin());

      while(!multiples.isConstant()) {
        *iter = *multiples;
        multiples.incrementThen();
        ++iter;
      }

      std::vector<int>::const_reverse_iterator start(indices.rbegin()),
        finish(indices.rend());

      int nmax = dd.nVariables();

      Cudd_Ref(multiples);
      NaviType emptyset = navi.elseBranch();

      NaviType tmp;
      int i = nmax-1;

      while(start != finish){

        while(i > *start) {

          tmp = cuddZddGetNode(man, i, multiples, multiples);
          Cudd_Ref(tmp);
          Cudd_Deref(multiples);
          multiples = tmp;
          --i;
        }
        tmp = cuddZddGetNode(man, i, multiples, emptyset);
        Cudd_Ref(tmp);
        Cudd_Deref(multiples);
        multiples = tmp;
        --i;
        ++start;
      }

      return navi;
    }

    NaviType elsedd = navi.elseBranch();


    NaviType tmp1, elseMultiples;
    tmp1 = dd_minimal_elements(elsedd, /*else*/dd, elseMultiples,
    int());

    elsedd = tmp1;

    // Cudd_Ref(elseMultiples);
    //Cudd_Deref(elseMultiples);
    // short cut if else and then branches equal or else contains 1
 
    if((navi.elseBranch() == navi.thenBranch()) ||
       (elsedd.isConstant() && elsedd.terminalValue()) ){
      multiples = elseMultiples;
      // Cudd_Ref(elsedd); Cudd_Ref(elseMultiples);/////
      return elsedd;
    }
 
    //    NaviType elseMult = elseMultiples.navigation();

    NaviType elseMult = elseMultiples;
    int nmax;
    if (elseMult.isConstant()){
      if (elseMult.terminalValue())
        nmax = dd.nVariables();
      else
        nmax = *navi;
    }
    else
      nmax = *elseMult;


    NaviType tmp;
    for(int i = nmax-1; i > *navi; --i){
      tmp = cuddZddGetNode(man, i, elseMult, elseMult);
      Cudd_Ref(tmp);
      Cudd_Deref(elseMult);
      elseMult = tmp;
    }


    NaviType thenNavi = navi.thenBranch();
    Cudd_Ref(thenNavi);


    tmp = Cudd_zddDiff(man, thenNavi, elseMult);
    Cudd_Ref(tmp);
    //    Cudd_Deref(thenNavi);
    Cudd_RecursiveDerefZdd(man, thenNavi);
    thenNavi = tmp;

    //  DDType thendd = DDType(ZDD(&dd.manager(), thenNavi));
    //Cudd_Deref(thenNavi);
    
    NaviType tmp2, thenMultiples;
    tmp2 = dd_minimal_elements(thenNavi, dd, thenMultiples, int()); 
    //Cudd_Ref(tmp2);
    //Cudd_Ref(thenNavi);
    ////Cudd_Deref(thenNavi);///!
    Cudd_RecursiveDerefZdd(man, thenNavi);

//     if(/*(tmp2==thenNavi)&&*/ ((DdNode*)tmp2)->ref <=2)
//       std::cout << ((DdNode*)tmp2)->ref<<" ";
//     else
//       Cudd_Deref(thenNavi);
//     std::cout.flush();


    thenNavi=tmp2;

    NaviType thenMult = thenMultiples;

    if (thenMult.isConstant()){
      if (thenMult.terminalValue())
        nmax =  dd.nVariables();
      else
        nmax = *navi;
    }
    else
      nmax = *thenMult;


    for(int i = nmax-1; i > *navi; --i){
      tmp = cuddZddGetNode(man, i, thenMult, thenMult);
      Cudd_Ref(tmp);
      Cudd_Deref(thenMult);
      thenMult = tmp;
    }



    tmp =  Cudd_zddUnion(man, thenMult, elseMult );

    Cudd_Ref(tmp);
    //Cudd_Deref(thenMult);///
      Cudd_RecursiveDerefZdd(man, thenMult);///
    thenMult = tmp;


    tmp = cuddZddGetNode(man, *navi, thenMult, elseMult);

    Cudd_Ref(tmp);
//     Cudd_Deref(thenMult);///
//     Cudd_Deref(elseMult);///
    Cudd_RecursiveDerefZdd(man, thenMult);
    Cudd_RecursiveDerefZdd(man, elseMult);

    thenMult = tmp;

    thenMultiples = thenMult;

    multiples = thenMultiples;

//     thendd = thendd.change(*navi);

//     DDType result =  thendd.unite(elsedd);

    NaviType result = cuddZddGetNode(man, *navi, thenNavi, elsedd);

    Cudd_Ref(result);

    ///Cudd_Deref(thenNavi);///
    ///Cudd_Deref(elsedd);///

    Cudd_RecursiveDerefZdd(man, thenNavi);
    Cudd_RecursiveDerefZdd(man, elsedd);
//       std::cout << "e ";
//      std::cout.flush();

    return result;

  }

  multiples = navi;//dd;
  Cudd_Ref(multiples);
  Cudd_Ref(navi);
  return navi;
  //  return dd;
}

END_NAMESPACE_PBORI

#endif
