#ifndef STAN_MATH_TORSTEN_PKMODEL_HPP
#define STAN_MATH_TORSTEN_PKMODEL_HPP

#include <stan/math/prim/arr/meta/get.hpp>
#include <stan/math/prim/mat/meta/get.hpp>

#include <torsten/PKModel/pmetricsCheck.hpp>
#include <torsten/PKModel/functions.hpp>
#include <torsten/PKModel/SearchReal.hpp>
#include <torsten/PKModel/ExtractVector.hpp>
#include <torsten/PKModel/pmxModel.hpp>
#include <torsten/PKModel/Event.hpp>
#include <torsten/PKModel/Rate.hpp>
#include <torsten/PKModel/ModelParameters.hpp>
#include <torsten/PKModel/integrator.hpp>
#include <torsten/PKModel/Pred/PolyExp.hpp>
// #include <torsten/PKModel/Pred1.hpp>
// #include <torsten/PKModel/PredSS.hpp>
#include <torsten/PKModel/Pred.hpp>

extern int marker_count;  // For testing purposes

#endif
