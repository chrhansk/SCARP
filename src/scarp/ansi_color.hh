/*
 * Licensed under the MIT License: http://www.opensource.org/licenses/mit-license.php
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef SCARP_ANSI_COLOR_HH
#define SCARP_ANSI_COLOR_HH

#include <iostream>
#include <string>

namespace scarp
{

/**
 * Definition of the public functions. It is a "define" in which for every
 * color it implements a pair of inline functions like:
 *
 *  Example function                                Implements
 *  std::ostream & color(std::ostream &__out)   ==> cout << ansi::color;
 *  std::string color(STRING_TYPE __str)        ==> cout << ansi::color("string")
 *
 */
#define ANSI_COLOR_IMPLEMENT(COLOR, SIZE)                               \
  inline std::ostream & (COLOR)(std::ostream &__os)                     \
  { if (can_colorize) __os.write(raw::_##COLOR, SIZE); return __os; }   \
                                                                        \
  template <typename T>                                                 \
  inline std::string (COLOR)(const T __str)                             \
  { return __wrap__(raw::_##COLOR, __str); }

#define ANSI_COLOR_IMPLEMENT_WITH_BG(COLOR, SIZE)                          \
  ANSI_COLOR_IMPLEMENT(COLOR, SIZE)                                        \
                                                                           \
  inline std::ostream & (COLOR##_bg)(std::ostream &__os)                   \
  { if (can_colorize) __os.write(raw::_##COLOR##_bg, SIZE); return __os; } \

namespace ansi {

  extern const bool can_colorize;

  /**
   * Constant strings for ANSI color codes representation.
   * Internal use only. :-)
   */
  namespace raw {
    const char _reset[]   = "\x1b[0m";
    const char _bright[]  = "\x1b[1m";
    const char _underline[] = "\x1b[4m";
    const char _blink[]   = "\x1b[5m";
    const char _inverse[] = "\x1b[7m";

    const char _black[]   = "\x1b[30m";
    const char _red[]     = "\x1b[31m";
    const char _green[]   = "\x1b[32m";
    const char _yellow[]  = "\x1b[33m";
    const char _blue[]    = "\x1b[34m";
    const char _magenta[] = "\x1b[35m";
    const char _cyan[]    = "\x1b[36m";
    const char _white[]   = "\x1b[37m";

    const char _black_bg[]   = "\x1b[40m";
    const char _red_bg[]     = "\x1b[41m";
    const char _green_bg[]   = "\x1b[42m";
    const char _yellow_bg[]  = "\x1b[43m";
    const char _blue_bg[]    = "\x1b[44m";
    const char _magenta_bg[] = "\x1b[45m";
    const char _cyan_bg[]    = "\x1b[46m";
    const char _white_bg[]   = "\x1b[47m";
  }

  /**
   * Templated inline function to wrap a string with ansi colors.
   * Used internally for the ansi::color(string) stream helpers.
   */
  template <typename _StrT>
  inline std::string __wrap__(const char *__col, const _StrT __in)
  {
    std::string __ostr(__col);
    return __ostr.append(__in).append(raw::_reset);
  }

  /**
   * Implementation of the various colors and control codes
   */
  ANSI_COLOR_IMPLEMENT(reset, 4)
  ANSI_COLOR_IMPLEMENT(bright, 4)
  ANSI_COLOR_IMPLEMENT(underline, 4)
  ANSI_COLOR_IMPLEMENT(blink, 4)
  ANSI_COLOR_IMPLEMENT(inverse, 4)

  ANSI_COLOR_IMPLEMENT_WITH_BG(black, 5)
  ANSI_COLOR_IMPLEMENT_WITH_BG(red, 5)
  ANSI_COLOR_IMPLEMENT_WITH_BG(green, 5)
  ANSI_COLOR_IMPLEMENT_WITH_BG(yellow, 5)
  ANSI_COLOR_IMPLEMENT_WITH_BG(blue, 5)
  ANSI_COLOR_IMPLEMENT_WITH_BG(magenta, 5)
  ANSI_COLOR_IMPLEMENT_WITH_BG(cyan, 5)
  ANSI_COLOR_IMPLEMENT_WITH_BG(white, 5)
}

} // namespace scarp

/**
 * Helper functions injected into the std namespace to help with resetting
 * ansi colors. Example usage:
 *
 *      std::cout << endc;      ==>     std::cout << ansi::reset;
 *      std::cout << endlc;     ==>     std::cout << ansi::reset << std:endl;
 *
 */
namespace std {

  template<typename _CharT, typename _Traits>
  inline basic_ostream<_CharT, _Traits>&
  endc(basic_ostream<_CharT, _Traits>& __os)
  { return __os << scarp::ansi::reset; }

  template<typename _CharT, typename _Traits>
  inline basic_ostream<_CharT, _Traits>&
  endlc(basic_ostream<_CharT, _Traits>& __os)
  { return __os << scarp::ansi::reset << std::endl; }
}

#endif // define SCARP_ANSI_COLOR_HH
