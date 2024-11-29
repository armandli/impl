#include <algorithm>

namespace s = std;

using ll = long long;

ll gcd1(ll a, ll b){
  return b ? gcd1(b, a % b) : a;
}

ll gcd2(ll a, ll b){
  while (b) b ^= a ^= b ^= a %= b;
  return a;
}

// binary gcd
// 1. gcd(0, b) = b, gcd(a, 0) = a
// 2. gcd(2a, 2b) = 2 * gcd(a, b)
// 3. gcd(2a, b) = gcd(a, b) if b is odd, gcd(a, 2b) = gcd(a, b)
//    if a is odd
// 4. gcd(a, b) = gcd(|a - b|, min(a, b)) if both a and b are odd

ll bgcd1(ll a, ll b){
  if (a == 0) return b;
  if (b == 0) return a;
  if (a == b) return a;

  ll mul = 1;
  while (a % 2 == 0 and b % 2 == 0){
    a >>= 1;
    b >>= 1;
    mul <<= 1;
  }

  while (a % 2 == 0 and b % 2 == 1){
    a >>= 1;
  }
  while (a % 2 == 1 and b % 2 == 0){
    b >>= 1;
  }
  return bgcd1(s::abs(a - b), s::min(a, b)) * mul;
}

ll bgcd2(ll a, ll b){
  if (a == 0) return b;
  if (b == 0) return a;

  ll az = __builtin_ctz(a);
  ll bz = __builtin_ctz(b);
  ll shift = s::min(az, bz);

  a >>= az;
  b >>= bz;

  while (a != 0){
    int diff = a - b;
    b = s::min(a, b);
    a = s::abs(diff);
    a >>= __builtin_ctz(a);
  }

  return b << shift;
}

ll bgcd3(ll a, ll b){
  if (a == 0) return b;
  if (b == 0) return a;

  ll az = __builtin_ctz(a);
  ll bz = __builtin_ctz(b);
  ll shift = s::min(az, bz);

  b >>= bz;

  while (a != 0){
    a >>= az;
    int diff = b - a;
    az = __builtin_ctz(diff);
    b = s::min(a, b);
    a = s::abs(diff);
  }
  return b << shift;
}
