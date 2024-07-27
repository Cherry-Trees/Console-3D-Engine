#ifndef _CONSOLE_GAME_ENGINE_HPP_
#define _CONSOLE_GAME_ENGINE_HPP_

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <array>

#include <stdio.h>
#include <Windows.h>
#include <WinCon.h>

#include "MathUtil.hpp"
#include "MiscUtil.hpp"

namespace cgel 
{
    class ConsoleGameEngine 
    {
        private:
            std::array<long, 2> *m_contour_x;
            LPSTR m_screen_buffer;
            HANDLE m_console_handle;
            DWORD m_dw_bytes_written;
            SMALL_RECT m_rect_window;
        
            void m_scan_line(long x1, long y1, long x2, long y2) {

                long sx, sy, dx1, dy1, dx2, dy2, x, y, m, n, k, cnt;

                sx = x2 - x1;
                sy = y2 - y1;

                if (sx > 0) dx1 = 1;
                else if (sx < 0) dx1 = -1;
                else dx1 = 0;

                if (sy > 0) dy1 = 1;
                else if (sy < 0) dy1 = -1;
                else dy1 = 0;

                m = std::labs(sx);
                n = std::labs(sy);
                dx2 = dx1;
                dy2 = 0;

                if (m < n)
                {
                    m = std::labs(sy);
                    n = std::labs(sx);
                    dx2 = 0;
                    dy2 = dy1;
                }

                x = x1; y = y1;
                cnt = m + 1;
                k = n / 2;

                while (cnt--)
                {
                    if ((y >= 0) && (y < m_screen_height))
                    {
                        if (x < m_contour_x[y][0]) m_contour_x[y][0] = x;
                        if (x > m_contour_x[y][1]) m_contour_x[y][1] = x;
                    }

                    k += n;
                    if (k < m)
                    {
                        x += dx2;
                        y += dy2;
                    }
                    else
                    {
                        k -= m;
                        x += dx1;
                        y += dy1;
                    }
                }
                
            }


        protected:
            int m_screen_width;
            int m_screen_height;


        public:
            ConsoleGameEngine(const unsigned width, const unsigned height) :
                m_screen_width(width),
                m_screen_height(height),
                m_contour_x(new std::array<long, 2>[height]),
                m_screen_buffer(new char[width * height]),
                m_console_handle(GetStdHandle(STD_OUTPUT_HANDLE)),//CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL)),
                m_dw_bytes_written(0),
                m_rect_window{0, 0, 1, 1} 
                {SetConsoleActiveScreenBuffer(m_console_handle);}   
            
            ~ConsoleGameEngine() 
            { 
                delete[] m_contour_x;
                delete[] m_screen_buffer;
            }

            char &at(const unsigned i) {return m_screen_buffer[i];}
            void setAsciiChar(const unsigned i, const unsigned j, const char asciiChar) 
            {
                if ((j < 0) || (j >= m_screen_width) ||
                    (i < 0) || (i >= m_screen_height))
                {
                    return;
                }
                m_screen_buffer[i * m_screen_width + j] = asciiChar;
            }

            void display() 
            {
                m_screen_buffer[m_screen_width * m_screen_height - 1] = '\0';
                WriteConsoleOutputCharacter(m_console_handle, (LPSTR)m_screen_buffer, m_screen_width * m_screen_height, {0, 0}, &m_dw_bytes_written);
            }

            void clear() {
                for (unsigned i = 0; i < m_screen_width * m_screen_height; i++) {
                    m_screen_buffer[i] = ' ';
                }
            }
            
            void drawTriangle(const Vec2f &p1, const Vec2f &p2, const Vec2f &p3, const char asciiChar) 
            {
                int y;
                for (y = 0; y < m_screen_height; y++)
                {
                    m_contour_x[y][0] = LONG_MAX; // min X
                    m_contour_x[y][1] = LONG_MIN; // max X
                }

                m_scan_line(p1.X(), p1.Y(), p2.X(), p2.Y());
                m_scan_line(p2.X(), p2.Y(), p3.X(), p3.Y());
                m_scan_line(p3.X(), p3.Y(), p1.X(), p1.Y());

                for (y = 0; y < m_screen_height; y++)
                {
                    if (m_contour_x[y][1] >= m_contour_x[y][0])
                    {
                        long x = m_contour_x[y][0];
                        long len = 1 + m_contour_x[y][1] - m_contour_x[y][0];

                        while (len--)
                        {
                            setAsciiChar(y, x++, asciiChar);

                        }
                    }
                }
          }
    };
}



#endif
