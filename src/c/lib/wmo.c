#include "wmo.h"

WxIconBucket wmo_bucket(uint8_t code) {
  switch (code) {
    case 0:
      return WX_ICON_CLEAR;
    case 1:
    case 2:
      return WX_ICON_PARTLY_CLOUDY;
    case 3:
      return WX_ICON_OVERCAST;
    case 45:
    case 48:
      return WX_ICON_FOG;
    case 51:
    case 53:
    case 55:
    case 56:
    case 57:
      return WX_ICON_DRIZZLE;
    case 61:
    case 63:
    case 65:
    case 66:
    case 67:
    case 80:
    case 81:
    case 82:
      return WX_ICON_RAIN;
    case 71:
    case 73:
    case 75:
    case 77:
    case 85:
    case 86:
      return WX_ICON_SNOW;
    case 95:
      return WX_ICON_THUNDERSTORM;
    case 96:
    case 99:
      return WX_ICON_THUNDERSTORM_HAIL;
    default:
      return WX_ICON_OVERCAST;
  }
}
