/*
 *  slimgb.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 02.05.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "slimgb_wrapper.h"
#include <iostream>
#include <vector>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <nf.h>

#include <groebner_alg.h>
using namespace boost::python;
USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB

void export_slimgb(){
  boost::python::class_<slimgb_reduction_type>("SlimgbReduction")
  .def(init<slimgb_reduction_type>())
  .def(init<GroebnerStrategy&>())
  .def("addPolynomial", &slimgb_reduction_type::addPolynomial)
  .def("nextResult", &slimgb_reduction_type::nextResult)
  .def_readonly("result",&slimgb_reduction_type::result)
  
  .def("reduce", &slimgb_reduction_type::reduce);
  class_ <std::vector<BoolePolynomial> >("BoolePolynomialVector")
    .def(vector_indexing_suite<std::vector<BoolePolynomial> >());
  def("parallel_reduce", parallel_reduce);
  def("redTail", redTail);
}