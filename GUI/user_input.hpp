#ifndef USER_INPUT_HPP_
#define USER_INPUT_HPP_

/* ----------------------------------------------------------------------
 * Copyright (C) 2016 Daniel Beale and Luca Benedetti. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * ---------------------------------------------------------------------- */

#include "common.hpp"

class UserInput
{
//  UserInput();
public:
  enum Type{
    BEGIN,
    CONTINUE,
    END
  };

  UserInput();
  UserInput(int timestamp, const QPoint &point, UserInput::Type type,
            QString tool_type, double tablet_pressure=1.0,
            double x_tilt=0.0, double y_tilt=0.0,
            double rotation=0.0);
  UserInput(const UserInput &u);
  ~UserInput();
  UserInput &operator =(const UserInput &u);
  QString device_str() const;

  UserInput::Type input_type;
  int timestamp;
  QPoint point;
  QString tool_type;
  double tablet_pressure;
  double x_tilt;
  double y_tilt;
  double rotation;
};
Q_DECLARE_METATYPE(UserInput)

#endif // USER_INPUT_HPP_
