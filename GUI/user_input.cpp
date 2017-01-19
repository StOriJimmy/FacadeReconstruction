#include "user_input.hpp"

UserInput::UserInput()
{
  qFatal("UserInput has no implicit constructor, by design.");
}

UserInput::UserInput(int t, const QPoint &p, UserInput::Type type, QString tooype, double tp,
                     double xt, double yt, double rot)
{
  timestamp = t;
  point = p;
  input_type = type;
  tool_type = tooype;
  tablet_pressure = tp;
  x_tilt = xt;
  y_tilt = yt;
  rotation = rot;
}

UserInput::UserInput(const UserInput &u)
{
  timestamp = u.timestamp;
  point = u.point;
  input_type = u.input_type;
  tool_type = u.tool_type;
  tablet_pressure = u.tablet_pressure;
  x_tilt = u.x_tilt;
  y_tilt = u.y_tilt;
  rotation = u.rotation;
}

UserInput::~UserInput()
{
}

UserInput &UserInput::operator =(const UserInput &u)
{
  timestamp = u.timestamp;
  point = u.point;
  input_type = u.input_type;
  tool_type = u.tool_type;
  tablet_pressure = u.tablet_pressure;
  x_tilt = u.x_tilt;
  y_tilt = u.y_tilt;
  rotation = u.rotation;
  return *this;
}

QString UserInput::device_str() const
{
  QString str;

  if((tablet_pressure == 1.0) &&
     (x_tilt == 0.0) &&
     (y_tilt == 0.0) &&
     (rotation == 0.0)) {
    str = QString("device=\"mouse\" pressure=\"100.0\" "
                  "tilt_x=\"0.0\" tilt_y=\"0.0\" rotation=\"0.0\"");
  } else {
    str = QString("device=\"pen\" pressure=\"%1\" "
                  "tilt_x=\"%2\" tilt_y=\"%3\" rotation=\"%4\"")
          .arg(100.0 * tablet_pressure)
          .arg(x_tilt)
          .arg(y_tilt)
          .arg(rotation);
  }

  return str;
}

