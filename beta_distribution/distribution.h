// there is no beta distribution in <random> so we make our own

#include <string>
#include <iostream>
#include <random>

namespace s = std;

template <typename RealType>
struct beta_distribution {
  using ty = RealType;
  class param_type {
    ty ma, mb;
  public:
    using distribution_type = beta_distribution;
    explicit param_type(ty a = 1., ty b = 1.): ma(a), mb(b) {}
    ty a() const { return ma; }
    ty b() const { return mb; }
    bool operator==(const param_type& o) const {
      return ma == o.ma && mb == o.mb;
    }
    bool operator!=(const param_type& o) const {
      return !(*this == o);
    }
  };

  beta_distribution(): mGammaA(DFV, DFV), mGammaB(DFV, DFV) {}
  beta_distribution(ty a, ty b): mGammaA(a, DFV), mGammaB(b, DFV) {}
  explicit beta_distribution(const param_type& p): mGammaA(p.a(), DFV), mGammaB(p.b(), DFV) {}

  void reset(){ /*DO NOTHING*/ }

  param_type param() const {
    return param_type(DFV, DFV);
  }
  void param(const param_type& p){
    mGammaA = s::gamma_distribution(p.a(), 1.);
    mGammaB = s::gamma_distribution(p.b(), 1.);
  }

  RealType alpha() const { return mGammaA.alpha(); }
  RealType beta() const { return mGammaB.alpha(); }

  template <class Generator>
  ty operator()(Generator& g){ return generate(g); }
  template <class Generator>
  ty operator()(Generator& g, const param_type& params){
    mGammaA = s::gamma_distribution<ty>(params.a(), DFV);
    mGammaB = s::gamma_distribution<ty>(params.b(), DFV);
    return generate(g);
  }

  ty min() const { return 0.; }
  ty max() const { return 1.; }

  bool operator==(const beta_distribution<ty>& o) const {
    return (mGammaA == o.mGammaA && mGammaB == o.mGammaB);
  }
  bool operator!=(const beta_distribution<ty>& o) const {
    return !(*this == o);
  }
private:
  //beta distribution can be implemented as double gamma distribution
  s::gamma_distribution<ty> mGammaA;
  s::gamma_distribution<ty> mGammaB;
  static constexpr RealType DFV = 1.;

  template <typename G>
  ty generate(G& g){
    ty a = mGammaA(g);
    return a / (a + mGammaB(g));
  }
};

template <typename CharT, class Traits, class ResultType>
s::basic_ostream<CharT, Traits>& operator<<(s::basic_ostream<CharT, Traits>& ost, const beta_distribution<ResultType>& d){
  ost << "~Beta(" << d.alpha() << "," << d.beta() << ")";
  return ost;
}

template <typename CharT, class Traits, class ResultType>
s::basic_istream<CharT, Traits>& operator>>(s::basic_istream<CharT, Traits>& ist, beta_distribution<ResultType>& d){
  s::string str;
  ResultType a, b;
  if (s::getline(ist, str, '(') && str == "~Beta" && ist >> a >> ist.get() == ',' && ist >> b && ist.get() == ')')
    d = beta_distribution<ResultType>(a, b);
  else
    ist.setstate(s::ios::failbit);
  return ist;
}
