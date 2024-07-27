#ifndef _MISCUTIL_HPP_
#define _MISCUTIL_HPP_

#include <Windows.h>
#include "Mesh.hpp"

namespace cgel
{
    class Keyboard
    {
        public:

            enum
            {
                W           = (unsigned short)'W',
                A           = (unsigned short)'A',
                S           = (unsigned short)'S',
                D           = (unsigned short)'D',

                Q           = (unsigned short)'Q',
                E           = (unsigned short)'E',
                R           = (unsigned short)'R',
                Z           = (unsigned short)'Z',
                X           = (unsigned short)'X',
                C           = (unsigned short)'C',
                V           = (unsigned short)'V',
                B           = (unsigned short)'B',
                I           = (unsigned short)'I',           

                Left        = VK_LEFT,
                Right       = VK_RIGHT,
                Up          = VK_UP,
                Down        = VK_DOWN,

                Space       = VK_SPACE,
                Tab         = VK_TAB,

                LShift      = VK_LSHIFT,
                LControl    = VK_LCONTROL,
                RShift      = VK_RSHIFT,
                RControl    = VK_RCONTROL,
            };

            static BOOL isKeyPressed(const unsigned short key)
            {
                return GetAsyncKeyState(key) & 0x8000; 
            }

    };


    template<typename Type>
    Vec4<Type> vector4_intersect_plane(const Vec4<Type> &plane_p, 
                                          const Vec4<Type> &plane_n, 
                                          const Vec4<Type> &line_start,
                                          const Vec4<Type> &line_end) {
        const float plane_d = -plane_n.dotH(plane_p);
        const float ad = line_start.dotH(plane_n);
        const float bd = line_end.dotH(plane_n);
        const float t = (-plane_d - ad) / (bd - ad);
        const Vec4<Type> line_start_to_end = line_end.subtractH(line_start);
        const Vec4<Type> line_to_intersect = line_start_to_end.multiplyH(t);
        return line_start.addH(line_to_intersect);
    }

    // Return number of triangles needed to clip.
    template<typename Type>
    short triangle_clip_against_plane(Vec4<Type> plane_p,
                                      Vec4<Type> plane_n,
                                      Triangle in_tri,
                                      Triangle &out_tri1,
                                      Triangle &out_tri2) 
    {

        auto dist = [&](const Vec4<Type> &p) 
        {
            const Vec4<Type> unit_p = p.unitH();
            return (plane_n.X() * p.X() + plane_n.Y() * p.Y() + plane_n.Z() * p.Z() - plane_n.dotH(plane_p));
        };

        Vec4<Type> *inside_points[3];
        Vec4<Type> *outside_points[3];
        short inside_point_count = 0;
        short outside_point_count = 0;

        const float d1 = dist(in_tri.vertex0.position);
        const float d2 = dist(in_tri.vertex1.position);
        const float d3 = dist(in_tri.vertex2.position);

        if (d1 >= 0) { inside_points[inside_point_count++] = &in_tri.vertex0.position; }
        else         { outside_points[outside_point_count++] = &in_tri.vertex0.position; }
        if (d2 >= 0) { inside_points[inside_point_count++] = &in_tri.vertex1.position; }
        else         { outside_points[outside_point_count++] = &in_tri.vertex1.position; }
        if (d3 >= 0) { inside_points[inside_point_count++] = &in_tri.vertex2.position; }
        else         { outside_points[outside_point_count++] = &in_tri.vertex2.position; }

        if (inside_point_count == 0) 
        {
            return 0;
        }

        if (inside_point_count == 3) 
        {
            out_tri1 = in_tri;
            return 1;
        }

        if (inside_point_count == 1 && outside_point_count == 2) 
        {
            out_tri1.asciiChar = in_tri.asciiChar;
            out_tri1.vertex0.position = *inside_points[0];

            out_tri1.vertex1.position = vector4_intersect_plane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
            out_tri1.vertex2.position = vector4_intersect_plane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

            return 1;
        }

        if (inside_point_count == 2 && outside_point_count == 1) 
        {
            out_tri1.asciiChar = in_tri.asciiChar;
            out_tri2.asciiChar = in_tri.asciiChar;

            out_tri1.vertex0.position = *inside_points[0];
            out_tri1.vertex1.position = *inside_points[1];
            out_tri1.vertex2.position = vector4_intersect_plane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

            out_tri2.vertex0.position = *inside_points[1];
            out_tri2.vertex1.position = out_tri1.vertex2.position;
            out_tri2.vertex2.position = vector4_intersect_plane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

            return 2;
        }

        return 0;
    }



}

#endif
