/*
 * FloatDetails.h
 *
 *  Created on: Mar 22, 2014
 *      Author: sam
 */

#ifndef FLOATDETAILS_H_
#define FLOATDETAILS_H_

#include <math.h>
#include <limits>
#include <stdint.h>

class FloatDetails
{
    /*!
     WHAT THIS OBJECT REPRESENTS
     This object is a tool for converting floating point numbers into an
     explicit integer representation and then also converting back.  In
     particular, a float_details object represents a floating point number with
     a 64 bit mantissa and 16 bit exponent.  These are stored in the public
     fields of the same names.

     The main use of this object is to convert floating point values into a
     known uniform representation so they can be serialized to an output stream.
     This allows dlib serialization code to work on any system, regardless of
     the floating point representation used by the hardware.  It also means
     that, for example, a double can be serialized and then deserialized into a
     float and it will perform the appropriate conversion.


     In more detail, this object represents a floating point value equal to
     mantissa*pow(2,exponent), except when exponent takes on any of the
     following special values:
     - is_inf
     - is_ninf
     - is_nan
     These values are used to indicate that the floating point value should be
     either infinity, negative infinity, or not-a-number respectively.
     !*/

  public:
    FloatDetails(const int32_t& man, const int16_t& exp) :
        mantissa(man), exponent(exp)
    {
    }
    /*!
     ensures
     - #mantissa == man
     - #exponent == exp
     !*/

    FloatDetails() :
        mantissa(0), exponent(0)
    {
    }
    /*!
     ensures
     - this object represents a floating point value of 0
     !*/

    FloatDetails(const float& val)
    {
      *this = val;
    }
    /*!
     ensures
     - converts the given value into a float_details representation.  This
     means that converting #*this back into a floating point number should
     recover the input val.
     !*/

    FloatDetails& operator=(const float& val)
    {
      convert_from(val);
      return *this;
    }

    /*!
     ensures
     - converts the given value into a float_details representation.  This
     means that converting #*this back into a floating point number should
     recover the input val.
     !*/

    const int32_t& getMantissa() const
    {
      return mantissa;
    }

    const int16_t& getExponent() const
    {
      return exponent;
    }

    operator float() const
    {
      return convert_to();
    }

    /*!
     ensures
     - converts the contents of this float_details object into a floating point number.
     !*/

    const static int16_t is_inf = 32000;
    const static int16_t is_ninf = 32001;
    const static int16_t is_nan = 32002;

    int32_t mantissa;
    int16_t exponent;

  private:

// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------
//                                  IMPLEMENTATION DETAILS
// ----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------

    void convert_from(const float& val)
    {
      mantissa = 0;

      const int digits = std::numeric_limits<float>::digits;

      if (val == std::numeric_limits<float>::infinity())
      {
        exponent = is_inf;
      }
      else if (val == -std::numeric_limits<float>::infinity())
      {
        exponent = is_ninf;
      }
      else if (val < std::numeric_limits<float>::infinity())
      {
        int exp;
        mantissa = static_cast<int32_t>(frexp(val, &exp) * (((uint32_t) 1) << digits));
        exponent = exp - digits;

        // Compact the representation a bit by shifting off any low order bytes
        // which are zero in the mantissa.  This makes the numbers in mantissa and
        // exponent generally smaller which can make serialization and other things
        // more efficient in some cases.
        for (int i = 0; i < 8 && ((mantissa & 0xFF) == 0); ++i)
        {
          mantissa >>= 8;
          exponent += 8;
        }
      }
      else
      {
        exponent = is_nan;
      }
    }

    float convert_to() const
    {
      if (exponent < is_inf)
        return ldexp(mantissa, exponent);
      else if (exponent == is_inf)
        return std::numeric_limits<float>::infinity();
      else if (exponent == is_ninf)
        return -std::numeric_limits<float>::infinity();
      else
        return std::numeric_limits<float>::quiet_NaN();
    }

};

#endif /* FLOATDETAILS_H_ */
