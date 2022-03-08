#include <kdt1.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

struct p2 {
  float x, y;
  p2() = default;
  p2(float x, float y): x(x), y(y) {}
};

template <>
s::array<float,2UL> coordinate_of(const p2& obj){
  s::array<float,2UL> a = {obj.x, obj.y};
  return a;
}

struct p3 {
  float x, y, z;
  p3() = default;
  p3(float x, float y, float z): x(x), y(y), z(z) {}
};

template <>
s::array<float,3UL> coordinate_of(const p3& obj){
  s::array<float,3UL> a = {obj.x, obj.y, obj.z};
  return a;
}

TEST(Construct2D, Construct2D1){
  s::vector<p2> points = {{70., 721.}, {343., 858.}, {207., 313.}, {615., 40.}, {751., 177.}, {888., 585.}, {479., 449.}};
  kdtree_v1<p2,2,float> kdt(points);

  s::cout << kdt.print() << s::endl;
}

TEST(Construct2D, Construct2D2){
  s::vector<p2> points = {{272.,50.},{259.,189.},{481.,144.},{915.,157.},{139.,310.},{913.,276.},{821.,386.},{622.,410.},{281.,467.},{43.,480.},{136.,615.},{445.,585.},{260.,685.},{592.,715.},{749.,683.},{163.,826.},{438.,828.},{571.,839.},{662.,798.},{879.,810.}};
  kdtree_v1<p2,2,float> kdt(points);

  s::cout << kdt.print() << s::endl;
}

TEST(NearestNeighbour2D, NearestNeighbour2D1){
  s::vector<p2> points = {{70., 721.}, {343., 858.}, {207., 313.}, {615., 40.}, {751., 177.}, {888., 585.}, {479., 449.}};
  kdtree_v1<p2,2,float> kdt(points);

  s::array<float,2> query = {438., 681.};
  p2* ret = kdt.neighbour<float(const s::array<float,2>&,const s::array<float,2>&)>(query, distance<float,2>);
  assert(ret != nullptr);

  s::array<float,2> cord = coordinate_of<float,2>(*ret);
  s::cout << cord[0] << " " << cord[1] << s::endl;
}

TEST(NearestNeighbour2D, NearestNeighbour2D2){
  s::vector<p2> points = {{272.,50.},{259.,189.},{481.,144.},{915.,157.},{139.,310.},{913.,276.},{821.,386.},{622.,410.},{281.,467.},{43.,480.},{136.,615.},{445.,585.},{260.,685.},{592.,715.},{749.,683.},{163.,826.},{438.,828.},{571.,839.},{662.,798.},{879.,810.}};
  kdtree_v1<p2,2,float> kdt(points);

  s::array<float,2> query = {298.,825.};
  p2* ret = kdt.neighbour<float(const s::array<float,2>&,const s::array<float,2>&)>(query, distance<float,2>);
  assert(ret != nullptr);

  s::array<float,2> cord = coordinate_of<float,2>(*ret);
  s::cout << cord[0] << " " << cord[1] << s::endl;
}
