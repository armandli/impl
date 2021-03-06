#include <cassert>
#include <cstddef>
#include <cmath>
#include <limits>
#include <vector>
#include <string>
#include <random>
#include <armadillo>

//TODO: there is an issue with STL random generator unable to produce the upper
//bound value; need to double check
//TODO: add optional output layer only activation function
//TODO: add optional softmax option

#ifndef NN_FORMAT
#define NN_FORMAT "arma_binary"
#endif

/* Feed Forward Neural Network */

namespace NN {

  /* Activation Function Trait */
  template <template <typename> class ActFun, typename T = double>
  struct ActFunTrait {
    static void normalize(arma::Mat<T>& m){
      ActFun<T> f;
      f.normalize(m);
      }
    static arma::Mat<T> function(const arma::Mat<T>& m){
      ActFun<T> f;
      return f.function(m);
      }
    static arma::Mat<T> deriviative(const arma::Mat<T>& m){
      ActFun<T> f;
      return f.deriviative(m);
      }
    };

  /* Cost Function Trait */
  template <template <template <typename> class, typename> class CostFun,
            template <typename> class ActFun,
            typename T = double>
  struct CostFunTrait {
    static T cost(const arma::Mat<T>& a, const arma::Mat<T>& e){
      CostFun<ActFun, T> f;
      return f.cost(a, e);
      }
    static arma::Mat<T> deriviative(const arma::Mat<T>& z, const arma::Mat<T>& a, const arma::Mat<T>& e){
      CostFun<ActFun, T> f;
      return f.deriviative(z, a, e);
      }
    };

  /* Regularization Function Trait for weight matrix */
  template <template <typename> class RegFun, typename T = double>
  struct RegFunTrait {
    static void regularize(arma::Mat<T>& w, const arma::Mat<T>& dw, T learn, T lambda){
      RegFun<T> f;
      f.regularize(w, dw, learn, lambda);
      }
    };
  
  /* Matrix Initialization Function Trait */
  template <template <typename> class InitFun, typename T = double>
  struct InitFunTrait {
    static void initialize(arma::Mat<T>& m, bool isFirstLayer){
      InitFun<T> f;
      f.initialize(m, isFirstLayer);
      }
    };
  
  /* Linear Activation Function */
  template <typename T = double>
  struct LinearFun {
    void normalize(arma::Mat<T>&){ /*do nothing*/ }
    arma::Mat<T> function(const arma::Mat<T>& m){
      arma::Mat<T> r = m;
      return r;
      }
    arma::Mat<T> deriviative(const arma::Mat<T>& m){
      arma::Mat<T> r(m.n_rows, m.n_cols);
      r.fill(1.);
      return r;
      }
    };

  /* Sigmoid Activation Function */
  template <typename T = double>
  struct SigmoidFun {
    static T sigmoid(T v){
      return 1. / (1. + exp(-1. * v));
      }
    void normalize(arma::Mat<T>& m){
      T maxVal = arma::max(arma::max(m));
      T minVal = arma::min(arma::min(m));

      m.transform( [=](T v){ return (v - minVal) / (maxVal - minVal); } );
      }
    arma::Mat<T> function(const arma::Mat<T>& m){
      arma::Mat<T> r = m;
      r.transform( [](T v){ return SigmoidFun<T>::sigmoid(v); } );
      return r;
      }
    arma::Mat<T> deriviative(const arma::Mat<T>& m){
      arma::Mat<T> r = m;
      r.transform( [](T v){ return SigmoidFun<T>::sigmoid(v) * (1. - SigmoidFun<T>::sigmoid(v)); } );
      return r;
      }
    };

  /* Hyperbolic Tangent Activation Function */
  template <typename T = double>
  struct HTanFun {
    void normalize(arma::Mat<T>& m){
      T maxVal = arma::max(arma::max(m));
      T minVal = arma::min(arma::min(m));

      m.transform( [=](T v){ return ((v - minVal) / (maxVal - minVal) - 0.5L) * 2.L ; } );
      }
    arma::Mat<T> function(const arma::Mat<T>& m){
      arma::Mat<T> r = m;
      r.transform( [](T v){ return std::tanh(v); } );
      return r;
      }
    arma::Mat<T> deriviative(const arma::Mat<T>& m){
      arma::Mat<T> r = m;
      r.transform( [](T v){ auto k = std::tanh(v); return 1 - k * k; } );
      return r;
      }
    };

  /* Quadradic Cost Function */
  template <template <typename> class ActFun, typename T = double>
  struct QCost {
    T cost(const arma::Mat<T>& a, const arma::Mat<T>& e){
      T n = a - e;
      arma::Mat<T> tmp = arma::sum(n % n, 1) % (T)a.n_rows;
      return 0.5 * arma::sum(arma::sum(tmp));
      }
    arma::Mat<T> deriviative(const arma::Mat<T>& z, const arma::Mat<T>& a, const arma::Mat<T>& e){
      return (a - e) % ActFunTrait<ActFun,T>::deriviative(z);
      }
    };

  /* Cross Entropy Cost Function */
  template <template <typename> class ActFun, typename T = double>
  struct CECost {
    T cost(const arma::Mat<T>& a, const arma::Mat<T>& e){
      arma::Mat<T> tmp = arma::sum(-1 * e % arma::log(a) - (1 - e) % arma::log(1 - a), 1) / (T)a.n_rows;
      return arma::sum(arma::sum(tmp));
      }
    arma::Mat<T> deriviative(const arma::Mat<T>& z, const arma::Mat<T>& a, const arma::Mat<T>& e){
      return a - e;
      }
    };

  /* No-Regularization Regularization Function */
  template <typename T = double>
  struct NRRegFun {
    void regularize(arma::Mat<T>& w, const arma::Mat<T>& dw, T, T){
      w -= dw;
      }
    };

  /* L2 Regularization Function */
  template <typename T = double>
  struct L2RegFun {
    void regularize(arma::Mat<T>& w, const arma::Mat<T>& dw, T learn, T lambda){
      w = (1. - learn * lambda) * w - dw;
      }
    };

  /* L1 Regularization Function */
  template <typename T = double>
  struct L1RegFun {
    void regularize(arma::Mat<T>& w, const arma::Mat<T>& dw, T learn, T lambda){
      w -= (learn * lambda) * signFunc(w) + dw;
      }
  private:
    arma::Mat<T> signFunc(const arma::Mat<T>& w){
      arma::Mat<T> sw = w;
      w.transform( [](T v){ return v < std::numeric_limits<T>::epsilon() &&
                                  -1. * v < std::numeric_limits<T>::epsilon() ? 0. :
                                  (v > 0.                                     ? 1. :
                                                                               -1.);
                          }
                 );
      return sw;
      }
    };
  
  /* Random Postive Initialization Function; value [0.,1.] with uniform distribution */
  template <typename T = double>
  struct RndPInit {
    void initialize(arma::Mat<T>& m, bool){
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::default_random_engine generator(seed);
      std::uniform_real_distribution<double> distribution(0., 1.);
  
      m.imbue( [&distribution, &generator](){ return distribution(generator); } );
      }
    };
  
  /* Random Initialization Function; value [-1.,1.] with uniform distribution */
  template <typename T = double>
  struct RndNInit {
    void initialize(arma::Mat<T>& m, bool){
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::default_random_engine generator(seed);
      std::uniform_real_distribution<T> distribution(-1., 1.);
  
      m.imbue( [&distribution, &generator](){ return distribution(generator); } );
      }
    };

  /* Random Initialization Function; value [-1.,1.] with Gaussian Distribution of mean 0 */
  template <typename T = double>
  struct GauNInit {
    void initialize(arma::Mat<T>& m, bool){
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::default_random_engine generator(seed);
      std::normal_distribution<T> distribution(-1., 1.);

      m.imbue( [&distribution, &generator](){ return distribution(generator); } );
      }
    };

  /* Nguyen-Widrow Initialization Function */
  template <typename T = double>
  struct NWInit {
    void initialize(arma::Mat<T>& m, bool isFirstLayer){
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::default_random_engine generator(seed);
      std::normal_distribution<T> distribution(-1., 1.);

      m.imbue( [&distribution, &generator](){ return distribution(generator); } );

      if (!isFirstLayer) return;

      // Nguyen-Widrow initialization does special operations to first layer only
      double beta = 0.7 * std::pow((T)m.n_cols, 1. / m.n_rows);
      arma::Mat<T> N(arma::sqrt(arma::sum(arma::pow(m, 2))));
      assert(N.n_rows == 1);
      for (size_t ic = 0; ic < m.n_cols; ++ic)
        for (size_t ir = 0; ir < m.n_rows; ++ir)
          m(ir, ic) = m(ir, ic) * beta / N(0, ic);
      }
    };

  /* Used to bypass overload resolution for FFNet constructor to specify constant hidden layers */
  template <size_t ... Hs> class HiddenLayer{};

  /* Feed Forward Neural Network of fixed number of hidden layers */
  template <class Derived,
            template <typename> class ActFunc,
            template <typename> class InitFunc,
            typename T>
  class FFNet {
  protected:
    /* weight matrix between each neuron layer,
     * Row: size of previous neuron layer
     * Col: size of neuron in next layer
     */
    std::vector<arma::Mat<T>> mWMtx;
    /* biase matrix for all neuron layers except input
     * Row: 1
     * Col: number of neuron in the layer
     */
    std::vector<arma::Mat<T>> mBMtx;
    /* non-activated output of neurons in each layer, but not include input layer.
     * Row: # of input
     * Col: # of neuron in this layer
     */
    std::vector<arma::Mat<T>> mZMtx;
    /* activated output of neurons in each layer, not include input layer.
     * Row: # of input
     * Col: # of neuron in this layer
     */
    std::vector<arma::Mat<T>> mAMtx;
    size_t mInSize;
    size_t mOutSize;
    size_t mEpoch;
  
    void feedForward(const arma::Mat<T>& im){
      assert(mWMtx.size() >= 1);
      mZMtx.clear();
      mAMtx.clear();
  
      arma::Mat<T> d = im * mWMtx[0] + arma::repmat(mBMtx[0], im.n_rows, 1);
      arma::Mat<T> e = ActFunTrait<ActFunc,T>::function(d);
      mZMtx.push_back(d);
      mAMtx.push_back(e);
      for (size_t i = 1; i < mWMtx.size(); ++i){
        d = e * mWMtx[i] + arma::repmat(mBMtx[i], im.n_rows, 1);
        e = ActFunTrait<ActFunc,T>::function(d);
        mZMtx.push_back(d);
        mAMtx.push_back(e);
        }
      }
  
  public:
    template <size_t ... Hs>
    FFNet(HiddenLayer<Hs...>, size_t in, size_t out, size_t epoch) :
      mInSize(in), mOutSize(out), mEpoch(epoch) {
      size_t HiddenLayerSizes[] = { Hs ... };

      //we only need to initialize the weight matrix and bias matrix
      size_t mRows = mInSize;
      for (size_t i = 0; i < sizeof...(Hs); ++i){
        arma::Mat<T> wm(mRows, HiddenLayerSizes[i]);
        InitFunTrait<InitFunc,T>::initialize(wm, i == 0);
        mWMtx.emplace_back(wm);

        mRows = HiddenLayerSizes[i];

        arma::Mat<T> bm(1, mRows);
        InitFunTrait<InitFunc,T>::initialize(bm, false);
        mBMtx.emplace_back(bm);
        }
      arma::Mat<T> wm(mRows, mOutSize);
      InitFunTrait<InitFunc,T>::initialize(wm, sizeof...(Hs) == 0);
      mWMtx.emplace_back(wm);

      arma::Mat<T> bm(1, mOutSize);
      InitFunTrait<InitFunc,T>::initialize(bm, false);
      mBMtx.emplace_back(bm);
      }
  
    virtual ~FFNet(){}

    /* save the neural network to file*/
    void save(const std::string& filename){
      size_t cnt = 0;
      for (auto& mtx : mWMtx){
        std::string name = filename;
        name += std::to_string(cnt++);
        mtx.save(name.c_str(), NN_FORMAT);
        }
      for (auto& mtx : mBMtx){
        std::string name = filename;
        name += std::to_string(cnt++);
        mtx.save(name.c_str(), NN_FORMAT);
        }
      }

    /* load neural network from file */
    void load(const std::string& filename){
      size_t cnt = 0;
      for (auto& mtx : mWMtx){
        std::string name = filename;
        name += std::to_string(cnt++);
        mtx.load(name.c_str(), NN_FORMAT);
        }
      for (auto& mtx : mBMtx){
        std::string name = filename;
        name += std::to_string(cnt++);
        mtx.load(name.c_str(), NN_FORMAT);
        }
      }
  
    /* Train the neural network; return training cost and validation cost after each epoch,
     * if training or validation cost was not specified to generate, return vector contains nothing*/
    std::vector<T> train(const arma::Mat<T>& td,                  //training data
                         const arma::Mat<T>& vd = arma::Mat<T>(), //validation data
                         bool computeTrainingCost = false,
                         bool computeValidationCost = false){
      assert(td.n_cols == mInSize + mOutSize);
      assert(mWMtx.size() >= 1 && mBMtx.size() >= 1);
  
      return static_cast<Derived&>(*this).backPropagate(td, vd, computeTrainingCost, computeValidationCost);
      }
  
    /* Test the neural network; return test result matrix for error calculation*/
    arma::Mat<T> test(const arma::Mat<T>& data){
      assert(data.n_cols == mInSize + mOutSize);

      arma::Mat<T> in = data.cols(0, mInSize - 1);
      arma::Mat<T> out = data.cols(mInSize, data.n_cols - 1);

      feedForward(in);
      return out - mAMtx.back();
      }
  
    arma::Mat<T> predict(const arma::Mat<T>& in){
      feedForward(in);
      return mAMtx.back();
      }
    };
  
  /* Feed Forward Neural Network; paired with Back Propagation using stochastic gradient descent */
  template <template <template <typename> class, typename> class CostFunc,
            template <typename> class ActFunc,
            template <typename> class InitFunc,
            template <typename> class RegFunc = NRRegFun,
            typename T = double>
  class BPFFNet : public FFNet<BPFFNet<CostFunc, ActFunc, InitFunc, RegFunc, T>, ActFunc, InitFunc, T> {
    /* previous change to weight matrix for momentum calculation */
    std::vector<arma::Mat<T>> mDWMtx;
    T mLearn;    //learning rate
    T mRegu;     //regularization rate lambda
    T mMomentum; //momentum
    size_t mBatchSize;
  public:
    template <size_t ... Hs>
    BPFFNet(HiddenLayer<Hs...> hidden, size_t in, size_t out, T learning_rate, T lambda, T momentum, size_t batch_size, size_t epoch) :
      FFNet<BPFFNet<CostFunc, ActFunc, InitFunc, RegFunc, T>, ActFunc, InitFunc, T>::FFNet(hidden, in, out, epoch),
      mLearn(learning_rate), mRegu(lambda), mMomentum(momentum), mBatchSize(batch_size) {
      size_t HiddenLayersSizes[] = { Hs ... };

      size_t mRows = in;
      for (size_t i = 0; i < sizeof...(Hs); ++i){
        arma::Mat<T> wm(mRows, HiddenLayersSizes[i], arma::fill::zeros);
        mDWMtx.emplace_back(wm);
        mRows = HiddenLayersSizes[i];
        }
      arma::Mat<T> wm(mRows, out, arma::fill::zeros);
      mDWMtx.emplace_back(wm);
      }
  
    virtual ~BPFFNet(){}
  
    std::vector<T> backPropagate(const arma::Mat<T>& data, const arma::Mat<T>& vd, bool computeTrainingCost, bool computeValidationCost){
      std::vector<arma::Mat<T>>& mWMtx = this->mWMtx;
      std::vector<arma::Mat<T>>& mBMtx = this->mBMtx;
      std::vector<arma::Mat<T>>& mZMtx = this->mZMtx;
      std::vector<arma::Mat<T>>& mAMtx = this->mAMtx;
      size_t mInSize = this->mInSize;
      size_t mOutSize = this->mOutSize;
      size_t mEpoch = this->mEpoch;
      T regLearn = mLearn * mBatchSize / data.n_rows;
      std::vector<T> costs;

      for (size_t i = 0; i < mEpoch; ++i){
        arma::Mat<T> sdata = arma::shuffle(data, 0);
        for (size_t start = 0, end = mBatchSize >= sdata.n_rows ? sdata.n_rows : mBatchSize;
             start < sdata.n_rows;
             start = end, end = start + mBatchSize >= sdata.n_rows ? sdata.n_rows : start + mBatchSize){
          arma::Mat<T> in = sdata.submat(start, 0, end - 1, mInSize - 1);
          arma::Mat<T> out = sdata.submat(start, mInSize, end - 1, sdata.n_cols - 1);

          // feed forward step for back propagation
          this->feedForward(in);
  
          arma::Mat<T> d = CostFunTrait<CostFunc,ActFunc,T>::deriviative(mZMtx.back(), mAMtx.back(), out);
          const arma::Mat<T>& p = mAMtx.size() == 1 ? in : mAMtx[mAMtx.size() - 2];
          arma::Mat<T> g = arma::strans(p) * d;
          arma::Mat<T> w = mLearn * g + mMomentum * mDWMtx.back();
          // if there is no hidden layer, we can update the weight matrix now
          if (mWMtx.size() == 1){
            mBMtx[0] -= mLearn * arma::sum(d);
            RegFunTrait<RegFunc>::regularize(mWMtx[0], w, regLearn, mRegu);
            mDWMtx[0] = w;
            }
      
          // do the hidden layers in reverse iterative order,
          // this loop only happens if there is >1 hidden layers
          for (size_t i = mAMtx.size() - 2; mAMtx.size() > 2 && i > 0; --i){
            mBMtx[i+1] -= mLearn * arma::sum(d);
            d = ActFunTrait<ActFunc,T>::deriviative(mZMtx[i]) % (d * arma::strans(mWMtx[i+1]));
            g = arma::strans(mAMtx[i - 1]) * d;
            RegFunTrait<RegFunc>::regularize(mWMtx[i+1], w, regLearn, mRegu);
            mDWMtx[i+1] = w;
            w = mLearn * g + mMomentum * mDWMtx[i];
            }
          // do the first layer if # hidden layer > 0
          if (mZMtx.size() >= 2){
            mBMtx[1] -= mLearn * arma::sum(d);
            d = ActFunTrait<ActFunc,T>::deriviative(mZMtx[0]) % (d * arma::strans(mWMtx[1]));
            g = arma::strans(in) * d;
            RegFunTrait<RegFunc>::regularize(mWMtx[1], w, regLearn, mRegu);
            mDWMtx[1] = w;
            w = mLearn * g + mMomentum * mDWMtx[0];
            mBMtx[0] -= mLearn * arma::sum(d);
            RegFunTrait<RegFunc>::regularize(mWMtx[0], w, regLearn, mRegu);
            mDWMtx[0] = w;
            }
          }
        if (computeTrainingCost){
          arma::Mat<T> in = sdata.cols(0, sdata.n_cols - mOutSize - 1);
          arma::Mat<T> out = sdata.cols(sdata.n_cols - mOutSize, sdata.n_cols - 1);
          this->feedForward(in);
          costs.push_back(computeCost(mAMtx.back(), out));
          }
        if (computeValidationCost && vd.n_rows > 0 && vd.n_cols == mInSize + mOutSize){
          arma::Mat<T> in = vd.cols(0, vd.n_cols - mOutSize - 1);
          arma::Mat<T> out = vd.cols(vd.n_cols - mOutSize, vd.n_cols - 1);
          this->feedForward(in);
          costs.push_back(computeCost(mAMtx.back(), out));
          }
        }
      return costs;
      }
  private:
    T computeCost(const arma::Mat<T>& a, const arma::Mat<T>& e){
      //compute the cost of result
      T cost = CostFunTrait<CostFunc, ActFunc, T>::cost(a, e);
      //add cost of weight of the neural network
      std::vector<arma::Mat<T>>& mWMtx = this->mWMtx;
      if (mRegu != 0.){
        T wcost = 0.;
        for (const auto& w : mWMtx)
          wcost += arma::sum(arma::sum(w % w));
        wcost *= 0.5 * mRegu / (T)a.n_rows;
        cost += wcost;
      }
      return cost;
      }
    };
  }
