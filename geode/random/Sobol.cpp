//#####################################################################
// Class Sobol
//#####################################################################
// See Bratley and Fox. 1988. Algorithm 659: Implementing Sobol's quasirandom sequence generator. Acm Trans. Math. Softw. 14, 88-100.
//#####################################################################
#include <geode/random/Sobol.h>
#include <geode/geometry/Box.h>
#include <geode/math/integer_log.h>
#include <geode/python/Class.h>
namespace geode {

typedef real T;

namespace {
template<class T> struct Helper {
  typedef typename Sobol<Vector<T,1>>::TI TI;
  static const TI vs[4][numeric_limits<T>::digits];
};

// Generated by sobol-helper: DO NOT EDIT (especially since it isn't used)
/*
template<> const Helper<float>::TI Helper<float>::vs[4][24] = {
  {8388608,4194304,2097152,1048576,524288,262144,131072,65536,32768,16384,8192,4096,2048,1024,512,256,128,64,32,16,8,4,2,1},
  {8388608,12582912,10485760,15728640,8912896,13369344,11141120,16711680,8421376,12632064,10526720,15790080,8947712,13421568,11184640,16776960,8388736,12583104,10485920,15728880,8913032,13369548,11141290,16711935},
  {8388608,4194304,14680064,11534336,6815744,15990784,8781824,5177344,15237120,11812864,6742016,16756736,8448000,4240384,14706176,11599616,6848640,16007232,8839392,5222576,15263848,11842804,6710918,16777039},
  {8388608,12582912,14680064,5242880,3670016,11272192,6422528,9633792,14385152,16596992,5955584,4042752,12060672,7080960,8523264,12780800,14910336,5327552,3728928,11450672,6522296,9520084,14216110,16692955}};
*/
template<> const Helper<double>::TI Helper<double>::vs[4][53] = {
  {4503599627370496,2251799813685248,1125899906842624,562949953421312,281474976710656,140737488355328,70368744177664,35184372088832,17592186044416,8796093022208,4398046511104,2199023255552,1099511627776,549755813888,274877906944,137438953472,68719476736,34359738368,17179869184,8589934592,4294967296,2147483648,1073741824,536870912,268435456,134217728,67108864,33554432,16777216,8388608,4194304,2097152,1048576,524288,262144,131072,65536,32768,16384,8192,4096,2048,1024,512,256,128,64,32,16,8,4,2,1},
  {4503599627370496,6755399441055744,5629499534213120,8444249301319680,4785074604081152,7177611906121728,5981343255101440,8972014882652160,4521191813414912,6781787720122368,5651489766768640,8477234650152960,4803766301753344,7205649452630016,6004707877191680,9007061815787520,4503668346847232,6755502520270848,5629585433559040,8444378150338560,4785147618525184,7177721427787776,5981434523156480,8972151784734720,4521260801327104,6781891201990656,5651576001658880,8477364002488320,4803839601410048,7205759402115072,6004799501762560,9007199252643840,4503599628419072,6755399442628608,5629499535523840,8444249303285760,4785074605195264,7177611907792896,5981343256494080,8972014884741120,4521191814467584,6781787721701376,5651489768084480,8477234652126720,4803766302871808,7205649454307712,6004707878589760,9007061817884640,4503668347895824,6755502521843736,5629585434869780,8444378152304670,4785147619639313},
  {4503599627370496,2251799813685248,7881299347898368,6192449487634432,3659174697238528,8584986789675008,4714705859903488,2779565395017728,8180366510653440,6341983069011968,3619592278638592,8996204138463232,4535485464576000,2276538825310208,7895318121152512,6227496420769792,3676835602759680,8593817242435584,4745612444565504,2803849140109312,8194715996389376,6358056984117248,3602896667017216,9007104228589568,4503655730380800,2251930944405504,7881371758362624,6192492269535232,3659299737829376,8585083803926528,4714760834646016,2779702831874048,8180435231178752,6342017429274624,3619712539557888,8996298629185536,4535541300002816,2276669823811584,7895390062952448,6227538834219008,3676822988197888,8593914014894080,4745564596788224,2803797434824192,8194785203461376,6358023002396288,3602879702895808,9007199254093792,4503599629275152,2251799815161864,7881299348741148,6192449489729046,3659174698294541},
  {4503599627370496,6755399441055744,7881299347898368,2814749767106560,1970324836974592,6051711999279104,3448068464705536,5172102697058304,7722969673498624,8910442231496704,3197379813572608,2170435953229824,6475023975972864,3801561453035520,4575892516896768,6861639752089600,8004925686546432,2860207700967424,2001952976142336,6147532719652864,3501631001853952,5111056179396608,7632215940792320,8961961974824960,3219370314563584,2155043193094144,6575080003862528,3859285981265920,4504699256438784,6757049069207552,7883223698767872,2815437270155264,1970806333636608,6053189999132672,3448910468874240,5173365546811392,7724855550083072,8912617859022848,3198160696623104,2170966253805568,6474405709377536,3801389959043072,4574810640008192,6862215811900928,8006330332788480,2860905761634944,2001892164859328,6146800120945504,3501326059175952,5112007514652696,7631452510355484,8962282486759434,3220156025143303}};
}

template<class TV> Sobol<TV>::Sobol(const Box<TV>& box)
  : offset(box.min)
  , scales(box.sizes()/(T)((TI)1<<max_bits))
  , n(0) {}

template<class TV> Sobol<TV>::~Sobol() {}

template<class TV> TV Sobol<TV>::vector() {
  const int bit = integer_log_exact(min_bit(~n++));
  GEODE_ASSERT(bit<max_bits,"Ran out of bits (floating point precision has been exhausted)");
  for (int i=0;i<d;i++)
    x[i] ^= Helper<T>::vs[i][bit];
  return offset+scales*TV(x);
}

#define INSTANTIATE(d) \
  template<> GEODE_DEFINE_TYPE(Sobol<Vector<T,d>>) \
  template class Sobol<Vector<T,d>>;
INSTANTIATE(1)
INSTANTIATE(2)
INSTANTIATE(3)
INSTANTIATE(4)

}
using namespace geode;

template<int d> static void wrap_helper() {
  typedef Sobol<Vector<T,d>> Self;
  static char name[8] = "Sobol?d";
  name[5] = '0'+d;
  Class<Self>(name)
    .GEODE_INIT(Box<Vector<T,d>>)
    .GEODE_METHOD(vector)
    ;
}

void wrap_sobol() {
  wrap_helper<1>();
  wrap_helper<2>();
  wrap_helper<3>();
}
