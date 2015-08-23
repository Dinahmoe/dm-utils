//
//  MathUtilities.hpp
//  DMAF
//
//  Created by Alessandro Saccoia on 11/21/12.
//
//

#ifndef DMAF_MathUtilities_hpp
#define DMAF_MathUtilities_hpp

#include <cmath>

namespace dinahmoe {
namespace utils {

inline float DbToLinear (const float inDb_) {
  if (inDb_ < -80) {
    return 0;
  }
  return (float)pow(10., inDb_ / 20.);
}

inline float LinearToDb (const float linear_) {
  return 20.0 * log10(linear_);
}

inline bool EqualsFloatLiteral(float rhs, float literal) {
  return (rhs > literal - 0.0001) && (rhs < literal + 0.0001);
}

inline double degreesToRadians(const double& inDegrees) {
  return inDegrees/180.0 * M_PI;
}

inline bool isPowerOfTwo(unsigned int x)
{ 
	return (x & (x-1)) == 0;
}

// returns the geodetic distance in Km or Miles
inline double haversineDistance(const double& lat1
  ,const double& long1
  ,const double& lat2
  ,const double& long2
  ,bool inMiles = false) {
  const double dlat1 = degreesToRadians(lat1);
  const double dlong1 = degreesToRadians(long1);
  const double dlat2 = degreesToRadians(lat2);
  const double dlong2 = degreesToRadians(long2);
  const double dLong = dlong1-dlong2;
  const double dLat = dlat1-dlat2;
  double aHarv= pow(sin(dLat/2.0),2.0)+cos(dlat1)*cos(dlat2)*pow(sin(dLong/2),2);
  double cHarv = 2 * atan2(sqrt(aHarv), sqrt(1.0-aHarv));
  //earth's radius from wikipedia varies between 6,356.750 km — 6,378.135 km (˜3,949.901 — 3,963.189 miles)
  const double earth = inMiles ? 3963.19 : 6378.137;
  return earth * cHarv;
}

inline double bearingFromGpsCoordinatesInRadians(const double& lat1
  ,const double& long1
  ,const double& lat2
  ,const double& long2) {
 return atan2(
  sin(long2-long1)*cos(lat2),
  cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(long2-long1)
  );
}

}} // end namespace utils

#endif
