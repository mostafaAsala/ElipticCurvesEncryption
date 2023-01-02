#pragma once

#include"bigInt.h"
#include <cmath>
#include <iostream>

using namespace std;
class EllipticPoint
{
    BigInt::Rossi M_X, M_Y;
    double m_x, m_y;
    static constexpr double ZeroThreshold = 1e20;
    static constexpr double B = 7; // the 'b' in y^2 = x^3 + a * x + b
                                  //  'a' is 0

    void Double() noexcept
    {
        if (IsZero())
        {
            // doubling zero is still zero
            return;
        }
        // based on the property of the curve, the line going through the
        // current point and the negative doubled point is tangent to the
        // curve at the current point.  wikipedia has a nice diagram.
        if (m_y == 0)
        {
            // at this point the tangent to the curve is vertical.
            // this point doubled is 0
            *this = EllipticPoint();
        }
        else
        {
            double L = (3 * m_x * m_x) / (2 * m_y);
            double newX = L * L - 2 * m_x;
            m_y = L * (m_x - newX) - m_y;
            m_x = newX;
        }
    }

public:
    friend std::ostream& operator<<(std::ostream&, const EllipticPoint&);

    // Create a point that is initialized to Zero
     EllipticPoint() noexcept : m_x(0), m_y(ZeroThreshold * 1.01) {}

    // Create a point based on the yCoordiante.  For a curve with a = 0 and b = 7
    // there is only one x for each y
    explicit EllipticPoint(double yCoordinate) noexcept
    {
        m_y = yCoordinate;
        m_x = cbrt(m_y * m_y - B);
        M_X = BigInt::Rossi(m_x);
        M_Y = BigInt::Rossi(m_y);
    }
    
    // Check if the point is 0
    bool IsZero() const noexcept
    {
        // when the elliptic point is at 0, y = +/- infinity
        bool isNotZero = abs(m_y) < ZeroThreshold;
        return !isNotZero;
        
    }

    // make a negative version of the point  (p = -q)
    EllipticPoint operator-() const noexcept
    {
        EllipticPoint negPt;
        negPt.m_x = m_x;
        negPt.m_y = -m_y;
        negPt.M_X = BigInt::Rossi(m_x);
        negPt.M_Y = BigInt::Rossi(m_y);

        return negPt;
    }

    // add a point to this one  ( p+=q )
    EllipticPoint& operator+=(const EllipticPoint& rhs) noexcept
    {
        if (IsZero())
        {
            *this = rhs;
        }
        else if (rhs.IsZero())
        {
            // since rhs is zero this point does not need to be
            // modified
        }
        else
        {
            double L = (rhs.m_y - m_y) / (rhs.m_x - m_x);
            if (isfinite(L))
            {
                double newX = L * L - m_x - rhs.m_x;
                m_y = L * (m_x - newX) - m_y;
                m_x = newX;
            }
            else
            {
                if (signbit(m_y) != signbit(rhs.m_y))
                {
                    // in this case rhs == -lhs, the result should be 0
                    *this = EllipticPoint();
                }
                else
                {
                    // in this case rhs == lhs.
                    Double();
                }
            }
        }

        return *this;
    }

    // subtract a point from this one  (p -= q)
    EllipticPoint& operator-=(const EllipticPoint& rhs) noexcept
    {
        *this += -rhs;
        return *this;
    }

    // multiply the point by an integer (p *= 3)
    EllipticPoint& operator*=(int rhs) noexcept
    {
        EllipticPoint r;
        EllipticPoint p = *this;

        if (rhs < 0)
        {
            // change p * -rhs to -p * rhs
            rhs = -rhs;
            p = -p;
        }

        for (int i = 1; i <= rhs; i <<= 1)
        {
            if (i & rhs) r += p;
            p.Double();
        }

        *this = r;
        return *this;
    }
};



