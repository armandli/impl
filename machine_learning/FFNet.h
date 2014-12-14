#include <cstddef>
#include <cmath>
#include <cassert>
#include <vector>
#include <random>
#include <armadillo>

//TODO: there is an issue with STL random generator unable to produce the upper
//bound value; need to double check

/* Feed Forward Neural Network */

namespace NN {

  /* Normalization Function Trait */
  template <template <typename> class NormFun, typename T = double>
  struct NormFunTrait {
    static void normalize(arma::Mat<T>& m){
      NormFun<T> f;
      f.normalize(m);
      }
    };

  /* Activation Function Trait */
  template <template <typename> class ActFun, typename T = double>
  struct ActFunTrait {
    static arma::Mat<T> function(const arma::Mat<T>& m){
      ActFun<T> f;
      return f.function(m);
      }
    static arma::Mat<T> deriviative(const arma::Mat<T>& m){
      ActFun<T> f;
      return f.deriviative(m);
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

  /* Sigmoid Normalization Function */
  template <typename T = double>
  struct SigmoidNormFun {
    void normalize(arma::Mat<T>& m){
      T maxVal = arma::max(arma::max(m));
      T minVal = arma::min(arma::min(m));

      m.transform( [=](T v){ return (v - minVal) / (maxVal - minVal); } );
      }
    };

  /* Hyperbolic Tangent Normalization Function */
  template <typename T = double>
  struct HTanNormFun {
    void normalize(arma::Mat<T>& m){
      T maxVal = arma::max(arma::max(m));
      T minVal = arma::min(arma::min(m));

      m.transform( [=](T v){ return ((v - minVal) / (maxVal - minVal) - 0.5L) * 2.L ; } );
      }
    };
  
  /* Sigmoid Activation Function */
  template <typename T = double>
  struct SigmoidFun {
    static T sigmoid(T v){
      return 1. / (1. + exp(-1. * v));
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
  class HTanFun {
  public:
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
  
  /* Random Postive Initialization Function; random value between 0. to 1. */
  template <typename T = double>
  struct RndPInit {
    void initialize(arma::Mat<T>& m, bool){
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::default_random_engine generator(seed);
      std::uniform_real_distribution<double> distribution(0., 1.);
  
      m.imbue( [&distribution, &generator](){ return distribution(generator); } );
      }
    };
  
  /* Random Initialization Function; random value between -1. to 1. */
  template <typename T = double>
  struct RndNInit {
    void initialize(arma::Mat<T>& m, bool){
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::default_random_engine generator(seed);
      std::uniform_real_distribution<T> distribution(-1., 1.);
  
      m.imbue( [&distribution, &generator](){ return distribution(generator); } );
      }
    };

  /* Nguyen-Widrow Initialization Function */
  template <typename T = double>
  struct NWInit {
    void initialize(arma::Mat<T>& m, bool isFirstLayer){
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
      std::default_random_engine generator(seed);
      std::uniform_real_distribution<T> distribution(-0.5, 0.5);

      m.imbue( [&distribution, &generator](){ return distribution(generator); } );

      if (!isFirstLayer) return;

      // Nguyen-Widrow initialization does special operations to first layer only
      double beta = 0.7 * std::pow(m.n_cols, 1. / m.n_rows);
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
  template <class Derived, template <typename> class ActFunc, template <typename> class InitFunc, typename T>
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
    /* output of neurons in each layer, but not include input layer.
     * Row: # of input
     * Col: # of neuron in this layer
     */
    std::vector<arma::Mat<T>> mLMtx;
    size_t mInSize;
    size_t mOutSize;
    size_t mEpoch;
  
    void feedForward(const arma::Mat<T>& im){
      assert(mWMtx.size() >= 1);
      mLMtx.clear();
  
      arma::Mat<T> d = im * mWMtx[0] + arma::repmat(mBMtx[0], im.n_rows, 1);
      arma::Mat<T> e = ActFunTrait<ActFunc>::function(d);
      mLMtx.push_back(d);
      for (size_t i = 1; i < mWMtx.size(); ++i){
        d = e * mWMtx[i] + arma::repmat(mBMtx[i], im.n_rows, 1);
        e = ActFunTrait<ActFunc>::function(d);
        mLMtx.push_back(d);
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
      //TODO
      }

    /* load neural network from file */
    void load(const std::string& filename){
      //TODO
      }
  
    /* Train the neural network; return training result matrix for error calculation*/
    arma::Mat<T> train(const arma::Mat<T>& data){
      assert(data.n_cols == mInSize + mOutSize);
      assert(mWMtx.size() >= 1 && mBMtx.size() >= 1);
  
      static_cast<Derived&>(*this).backPropagate(data);

      arma::Mat<T> in = data.cols(0, mInSize - 1);
      arma::Mat<T> out = data.cols(mInSize, data.n_cols - 1);

      feedForward(in);
      return ActFunTrait<ActFunc>::function(mLMtx.back()) - out;
      }
  
    /* Test the neural network; return test result matrix for error calculation*/
    arma::Mat<T> test(const arma::Mat<T>& data){
      assert(data.n_cols == mInSize + mOutSize);

      arma::Mat<T> in = data.cols(0, mInSize - 1);
      arma::Mat<T> out = data.cols(mInSize, data.n_cols - 1);

      feedForward(in);
      return ActFunTrait<ActFunc>::function(mLMtx.back()) - out;
      }
  
    arma::Mat<T> predict(const arma::Mat<T>& in){
      feedForward(in);
      return ActFunTrait<ActFunc>::function(mLMtx.back());
      }
    };
  
  /* Feed Forward Neural Network; paired with Back Propagation using stochastic gradient descent */
  template <template <typename> class ActFunc, template <typename> class InitFunc, typename T = double>
  class BPFFNet : public FFNet<BPFFNet<ActFunc, InitFunc, T>, ActFunc, InitFunc, T> {
    /* previous change to weight matrix for momentum calculation */
    std::vector<arma::Mat<T>> mDWMtx;
    double mLearn;
    double mMomentum;
    size_t mBatchSize;
  public:
    template <size_t ... Hs>
    BPFFNet(HiddenLayer<Hs...> hidden, size_t in, size_t out, double learning_rate, double momentum, size_t batch_size, size_t epoch) :
      FFNet<BPFFNet<ActFunc, InitFunc, T>, ActFunc, InitFunc, T>::FFNet(hidden, in, out, epoch),
      mLearn(learning_rate), mMomentum(momentum), mBatchSize(batch_size) {
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
  
    void backPropagate(const arma::Mat<T>& data){
      std::vector<arma::Mat<T>>& mWMtx = this->mWMtx;
      std::vector<arma::Mat<T>>& mLMtx = this->mLMtx;
      std::vector<arma::Mat<T>>& mBMtx = this->mBMtx;
      size_t mInSize = this->mInSize;
      size_t mOutSize = this->mOutSize;
      size_t mEpoch = this->mEpoch;

      arma::Mat<T> sdata = arma::shuffle(data, 0);

      for (size_t i = 0; i < mEpoch; ++i){
        mLMtx.clear();
        size_t start = i * sdata.n_rows / mEpoch % sdata.n_rows;
        size_t end = start + mBatchSize >= sdata.n_rows ? sdata.n_rows - 1 : start + mBatchSize - 1;
        arma::Mat<T> in = sdata.submat(start, 0, end, mInSize - 1);
        arma::Mat<T> out = sdata.submat(start, mInSize, end, sdata.n_cols - 1);

        // feed forward step for back propagation
        this->feedForward(in);

        // back propagation
        /* Output layer Gradient Descent
         *   Del[N*O] = (E[N*O] - R[N*O]) % f'(L[N*O])
         *   G[I*O]   = P[N*I]^t * Del[N*O] 
         *   DW@t+1   = l * G + m * DW@t
         *   DB@t+1   = Del
         *     where
         *     N   = number of input case
         *     O   = output layer neuron size
         *     I   = previous layer neuron size, this could be input player if no hidden layer is present
         *     Del = delta matrix
         *     L   = output layer before applying activation function
         *     G   = gradient matrix
         *     P   = previous layer output
         *     DW  = change to weight matrix before output layer before the previous layer
         *     l   = learning rate
         *     m   = momentum
         *
         * Inner Layer (non-output layer) Gradient Descent
         *   Del[N*O] = Del1[N*O1] * W1[O*O1]^t % f'(L[N*O])
         *   G[I*O]   = P[N*I]^t * Del[N*O]
         *   DW@t+1   = l * G + m * DW@t
         *   DB@t+1   = Del
         *     where
         *     N    = number of input case
         *     O1   = number of output in next layer
         *     O    = number of output in this layer, or input in next layer
         *     I    = number of input in this layer
         *     Del  = delta matrix
         *     Del1 = delta matrix of next layer
         *     W1   = weight matrix of next layer (before update)
         *     L    = output before applying activation function for this layer
         *     G    = gradient matrix
         *     P    = input from previous layer
         *     DW   = change to weight matrix before this layer and previous layer
         *     l    = learning rate
         *     m    = momentum
         */
        arma::Mat<T> d = (out - ActFunTrait<ActFunc>::function(mLMtx.back())) % ActFunTrait<ActFunc>::deriviative(mLMtx.back());
        const arma::Mat<T>& p = mLMtx.size() == 1 ? in : ActFunTrait<ActFunc>::function(mLMtx[mLMtx.size() - 2]);
        arma::Mat<T> g = arma::strans(p) * d;
        arma::Mat<T> w = mLearn * g + mMomentum * mDWMtx.back();
        // if there is no hidden layer, we can update the weight matrix now
        if (mWMtx.size() == 1){
          mBMtx[0] += arma::sum(d);
          mWMtx[0] += w;
          mDWMtx[0] = w;
          }
    
        // do the hidden layers in reverse iterative order,
        // this loop only happens if there is >1 hidden layers
        for (size_t i = mLMtx.size() - 2; mLMtx.size() > 2 && i > 0; --i){
          mBMtx[i+1] += arma::sum(d);
          d = ActFunTrait<ActFunc>::deriviative(mLMtx[i]) % (d * arma::strans(mWMtx[i+1]));
          g = arma::strans(ActFunTrait<ActFunc>::function(mLMtx[i - 1])) * d;
          mWMtx[i+1] += w;
          mDWMtx[i+1] = w;
          w = mLearn * g + mMomentum * mDWMtx[i];
          }
        // do the first layer if # hidden layer > 0
        if (mLMtx.size() >= 2){
          mBMtx[1] += arma::sum(d);
          d = ActFunTrait<ActFunc>::deriviative(mLMtx[0]) % (d * arma::strans(mWMtx[1]));
          g = arma::strans(in) * d;
          mWMtx[1] += w;
          mDWMtx[1] = w;
          w = mLearn * g + mMomentum * mDWMtx[0];
          mBMtx[0] += arma::sum(d);
          mWMtx[0] += w;
          mDWMtx[0] = w;
          }
        }
      }
    };

  template <template <typename> class ActFunc, template <typename> class InitFunc, typename T = double>
  class RPROPFFNet : public FFNet<RPROPFFNet<ActFunc, InitFunc, T>, ActFunc, InitFunc, T> {
    //TODO
    };
  }
