/**
 * Non-metric Space Library
 *
 * Authors: Bilegsaikhan Naidan (https://github.com/bileg), Leonid Boytsov (http://boytsov.info).
 * With contributions from Lawrence Cayton (http://lcayton.com/) and others.
 *
 * For the complete list of contributors and further details see:
 * https://github.com/searchivarius/NonMetricSpaceLib 
 * 
 * Copyright (c) 2014
 *
 * This code is released under the
 * Apache License Version 2.0 http://www.apache.org/licenses/.
 *
 */

#ifndef _SPACE_JS_H_
#define _SPACE_JS_H_

#include <string>
#include <map>
#include <stdexcept>

#include <string.h>
#include "global.h"
#include "object.h"
#include "utils.h"
#include "space.h"
#include "space_vector.h"

#define SPACE_JS_DIV_SLOW         "jsdivslow"
#define SPACE_JS_DIV_FAST         "jsdivfast"
#define SPACE_JS_DIV_FAST_APPROX  "jsdivfastapprox"

#define SPACE_JS_METR_SLOW         "jsmetrslow"
#define SPACE_JS_METR_FAST         "jsmetrfast"
#define SPACE_JS_METR_FAST_APPROX  "jsmetrfastapprox"

namespace similarity {

template <typename dist_t>
class SpaceJSBase : public VectorSpace<dist_t> {
 public:
  enum JSType {kJSSlow, kJSFastPrecomp, kJSFastPrecompApprox};

  explicit SpaceJSBase(JSType type) : type_(type) {}
  virtual ~SpaceJSBase() {}

  virtual std::string ToString() const = 0;
  virtual Object* CreateObjFromVect(IdType id, LabelType label, const std::vector<dist_t>& InpVect) const;

 protected:
  dist_t JensenShannonFunc(const Object* obj1, const Object* obj2) const;
  JSType  GetType() const { return type_; }
 private:
  JSType   type_;
};

template <typename dist_t>
class SpaceJSDiv : public SpaceJSBase<dist_t> {
 public:
  explicit SpaceJSDiv(typename SpaceJSBase<dist_t>::JSType type) : SpaceJSBase<dist_t>(type) {}
  virtual ~SpaceJSDiv() {}

  virtual std::string ToString() const {
    std::stringstream stream;
    stream << "Jensen-Shannon divergence: type code = " << SpaceJSBase<dist_t>::GetType();
    return stream.str();
  }

 protected:
  virtual dist_t HiddenDistance(const Object* obj1, const Object* obj2) const {
    return SpaceJSBase<dist_t>::JensenShannonFunc(obj1, obj2);
  }
 private:
};

template <typename dist_t>
class SpaceJSMetric : public SpaceJSBase<dist_t> {
 public:
  explicit SpaceJSMetric(typename SpaceJSBase<dist_t>::JSType type) : SpaceJSBase<dist_t>(type) {}
  virtual ~SpaceJSMetric() {}

  virtual std::string ToString() const {
    std::stringstream stream;
    stream << "Jensen-Shannon metric: type code = " << SpaceJSBase<dist_t>::GetType();
    return stream.str();
  }
 protected:
  virtual dist_t HiddenDistance(const Object* obj1, const Object* obj2) const {
    return sqrt(SpaceJSBase<dist_t>::JensenShannonFunc(obj1, obj2));
  }
 private:
};


}  // namespace similarity

#endif 
