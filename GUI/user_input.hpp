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

/**
 * @brief The UserInput class.
 * The user input class is for dealing with more than one input device, such as a graphics tablet.
 * It has proerties such as the pen tilt, pressure, rotation and position.
 */
class UserInput
{
//  UserInput();
public:
    /**
     * @brief The type of input
     */
    enum Type{
        BEGIN,
        CONTINUE,
        END
    };

    /**
     * @brief Construct an empty UserInput
     */
    UserInput();

    /**
     * @brief Construct a UserInput given current device output
     * @param timestamp The time at which the measurement was taken
     * @param point The position that was measured
     * @param type The type of input that has been triggered
     * @param tool_type The tool type
     * @param tablet_pressure The pressure
     * @param x_tilt The x tilt
     * @param y_tilt The y tilt
     * @param rotation The rotation
     */
    UserInput(int timestamp, const QPoint &point, UserInput::Type type,
            QString tool_type, double tablet_pressure=1.0,
            double x_tilt=0.0, double y_tilt=0.0,
            double rotation=0.0);

    /**
     * @brief A copy constructor for UserInput
     */
    UserInput(const UserInput &u);
    ~UserInput();

    /**
     * @brief An assigment operator for UserInput
     */
    UserInput &operator =(const UserInput &u);

    QString device_str() const; ///< \brief Return a string containing all of the device properties

    UserInput::Type input_type; ///< The input type
    int timestamp;              ///< The time that the input was registered
    QPoint point;               ///< The position
    QString tool_type;          ///< The type of input
    double tablet_pressure;     ///< The pressure
    double x_tilt;              ///< The x tilt
    double y_tilt;              ///< The y tilt
    double rotation;            ///< The rotation.
};
Q_DECLARE_METATYPE(UserInput)

#endif // USER_INPUT_HPP_
