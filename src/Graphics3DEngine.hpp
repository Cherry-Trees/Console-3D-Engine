#ifndef _GRAPHICS3DENGINE_HPP_
#define _GRAPHICS3DENGINE_HPP_

#include "ConsoleGameEngine.hpp"
#include "Mesh.hpp"
#include <list>

namespace cgel
{
    class Graphics3DEngine : public ConsoleGameEngine
    {
        private:
            std::vector<Mesh> m_meshCollection;
            const char m_asciiGradient[92] = "`.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
            const int m_asciiGradientSize = 92;

            // Math :3
            Matrix<float, 4, 4> m_projectionMatrix;
            Matrix<float, 4, 4> m_worldTransformationMatrix;
            Matrix<float, 4, 4> m_cameraMatrix;
            Matrix<float, 4, 4> m_viewMatrix;
            Matrix<float, 4, 4> m_cameraPitchRotationMatrix;
            Matrix<float, 4, 4> m_cameraYawRotationMatrix;
            Matrix<float, 4, 4> m_cameraFullRotationMatrix;

            Vec4f m_cameraLookFrom;
            Vec4f m_cameraLookDirection;
            Vec4f m_cameraTarget;
            Vec4f m_directionalLight;
            Vec4f m_up;
            Vec4f m_forward;
            Vec4f m_right;

            float m_horizontalFov;
            float m_yaw;
            float m_pitch;
            float m_zNear;
            float m_zFar;

            float m_forwardMovementSpeed;
            float m_backwardMovementSpeed;
            float m_leftMovementSpeed;
            float m_rightMovementSpeed;
            float m_upMovementSpeed;
            float m_downMovementSpeed;
            float m_yawRotationSpeed;
            float m_pitchRotationSpeed;

            void m_handleKeyboardEvents()
            {
                if (Keyboard::isKeyPressed(Keyboard::A)) 
                    m_cameraLookFrom += Vec4f{1, 0, 0, 1} * m_cameraYawRotationMatrix;

                if (Keyboard::isKeyPressed(Keyboard::D)) 
                    m_cameraLookFrom += Vec4f{-1, 0, 0, 1} * m_cameraYawRotationMatrix;             

                if (Keyboard::isKeyPressed(Keyboard::Space)) 
                    m_cameraLookFrom.Y() += m_upMovementSpeed;
                
                if (Keyboard::isKeyPressed(Keyboard::LShift)) 
                    m_cameraLookFrom.Y() -= m_downMovementSpeed;

                m_cameraTarget = {0, 0, 1, 1};
                m_cameraLookDirection = m_cameraTarget.multiply(m_cameraFullRotationMatrix);
                m_cameraTarget = m_cameraLookFrom.addH(m_cameraLookDirection);
                m_forward = m_cameraLookDirection.multiplyH(m_forwardMovementSpeed);

                if (Keyboard::isKeyPressed(Keyboard::W)) 
                    m_cameraLookFrom.addH_assign(m_forward);
                
                if (Keyboard::isKeyPressed(Keyboard::S)) 
                    m_cameraLookFrom.subtractH_assign(m_forward);

                if (Keyboard::isKeyPressed(Keyboard::Left)) 
                    m_yaw += m_yawRotationSpeed;
                
                if (Keyboard::isKeyPressed(Keyboard::Right)) 
                    m_yaw -= m_yawRotationSpeed;

                if (Keyboard::isKeyPressed(Keyboard::Up)) 
                    m_pitch += m_pitchRotationSpeed;
                
                if (Keyboard::isKeyPressed(Keyboard::Down)) 
                    m_pitch -= m_pitchRotationSpeed;
            }
        

        public:
            Graphics3DEngine(const unsigned width, const unsigned height, const float fov = HALF_PI, const float zNear = 0.01, const float zFar = 100) :
                ConsoleGameEngine(width, height), 
                m_horizontalFov(fov),
                m_zNear(zNear),
                m_zFar(zFar),
                m_forwardMovementSpeed(0.5),
                m_backwardMovementSpeed(0.5),
                m_leftMovementSpeed(0.5),
                m_rightMovementSpeed(0.5),
                m_upMovementSpeed(0.5),
                m_downMovementSpeed(0.5),
                m_yawRotationSpeed(0.1),
                m_pitchRotationSpeed(0.1),
                m_up({0, 1, 0, 1}),
                m_forward({0, 0, 1, 1}),
                m_right({1, 0, 0, 1}),
                m_cameraLookFrom({0, 0, 0, 1}),
                m_cameraLookDirection({0, 0, 1, 1}),
                m_cameraTarget({0, 0, 1, 1}),
                m_directionalLight({0, 0.45, -1, 1}),
                m_projectionMatrix(make_projection_4x4<float>(width, height, fov, zNear, zFar)),
                m_worldTransformationMatrix(make_identity<float, 4>()) {}

            void addMesh(Mesh mesh)
            {
                m_meshCollection.push_back(mesh);
            }

            void update()
            {       
                clear();

                // Update camera rotation.
                m_cameraPitchRotationMatrix = make_rotationX_4x4<float>(m_pitch);
                m_cameraYawRotationMatrix = make_rotationY_4x4<float>(m_yaw);
                m_cameraFullRotationMatrix = m_cameraPitchRotationMatrix * m_cameraYawRotationMatrix;
                
                // Handle keyboard input.
                m_handleKeyboardEvents();

                // Construct the camera and view matrices.
                m_cameraMatrix = make_pointat_4x4<float>(m_cameraLookFrom, m_cameraTarget, m_up);
                m_viewMatrix = make_quick_inverse_4x4<float>(m_cameraMatrix);

                for (Mesh &mesh : m_meshCollection)
                {
                    // Construct a vector containing the triangles that will be rasterized.
                    std::vector<Triangle> rasterTriangles;
                    for (Triangle &tri : mesh.getTriangleCollection()) 
                    {
                        Vertex vertex0 = tri.vertex0;
                        Vertex vertex1 = tri.vertex1;
                        Vertex vertex2 = tri.vertex2;
                      
                        // vertex0
                        vertex0.position.subtract_assign({0.5, 0.5, 0.5, 0});
                        vertex0.position.multiply_assign(m_worldTransformationMatrix);
                        vertex0.position.Z() += 5.75;

                        // vertex1
                        vertex1.position.subtract_assign({0.5, 0.5, 0.5, 0});
                        vertex1.position.multiply_assign(m_worldTransformationMatrix);
                        vertex1.position.Z() += 5.75;

                        // vertex2
                        vertex2.position.subtract_assign({0.5, 0.5, 0.5, 0});
                        vertex2.position.multiply_assign(m_worldTransformationMatrix);
                        vertex2.position.Z() += 5.75;

                        // Get triangle face normal.
                        Vec4f edge0 = vertex0.position.subtractH(vertex1.position);
                        Vec4f edge1 = vertex2.position.subtractH(vertex1.position);
                        Vec4f faceNormal = edge1.crossH(edge0);
                        faceNormal.normalizeH();
                        
                        if (faceNormal.dotH(vertex0.position.subtractH(m_cameraLookFrom)) < 0) 
                        {

                            const float lightDP = faceNormal.dotH(m_directionalLight);

                            unsigned short triangleAsciiGradientIndex = std::max(std::min((int)roundf(lightDP * m_asciiGradientSize), m_asciiGradientSize - 2), 0);
                            char triangleAsciiChar = m_asciiGradient[triangleAsciiGradientIndex];

                            // Convert to view space.
                            vertex0.position.multiply_assign(m_viewMatrix);
                            vertex1.position.multiply_assign(m_viewMatrix);
                            vertex2.position.multiply_assign(m_viewMatrix);
                            vertex0.position.mapW();
                            vertex1.position.mapW();
                            vertex2.position.mapW();

                            short clippedTriangleCount = 0;
                            Triangle clippedTriangle[2];
                            clippedTriangleCount = triangle_clip_against_plane<float>(Vec4f{ 0, 0, 0.1 },
                                                                                      Vec4f{ 0, 0, 1 },
                                                                                      Triangle{ vertex0, vertex1, vertex2, faceNormal, triangleAsciiChar }, 
                                                                                      clippedTriangle[0], clippedTriangle[1]);
                      
                            for (uint8_t i = 0; i < clippedTriangleCount; i++) 
                            {

                                Vertex clippedVertex0 = clippedTriangle[i].vertex0;
                                Vertex clippedVertex1 = clippedTriangle[i].vertex1;
                                Vertex clippedVertex2 = clippedTriangle[i].vertex2;
                                const char clippedAsciiChar = clippedTriangle[i].asciiChar;

                                clippedVertex0.position.X() *= -1;
                                clippedVertex0.position.Y() *= -1;
                                clippedVertex1.position.X() *= -1;
                                clippedVertex1.position.Y() *= -1;
                                clippedVertex2.position.X() *= -1;
                                clippedVertex2.position.Y() *= -1;

                                // Project to 2D space.
                                clippedVertex0.position.multiply_assign(m_projectionMatrix);
                                clippedVertex0.position.mapW();
                                clippedVertex0.position.X()++;
                                clippedVertex0.position.Y()++;
                                clippedVertex0.position.X() *= 0.5 * this->m_screen_width;
                                clippedVertex0.position.Y() *= 0.5 * this->m_screen_height;

                                clippedVertex1.position.multiply_assign(m_projectionMatrix);
                                clippedVertex1.position.mapW();
                                clippedVertex1.position.X()++;
                                clippedVertex1.position.Y()++;
                                clippedVertex1.position.X() *= 0.5 * this->m_screen_width;
                                clippedVertex1.position.Y() *= 0.5 * this->m_screen_height;

                                clippedVertex2.position.multiply_assign(m_projectionMatrix);
                                clippedVertex2.position.mapW();
                                clippedVertex2.position.X()++;
                                clippedVertex2.position.Y()++;
                                clippedVertex2.position.X() *= 0.5 * this->m_screen_width;
                                clippedVertex2.position.Y() *= 0.5 * this->m_screen_height;

                                rasterTriangles.push_back(Triangle{clippedVertex0, clippedVertex1, clippedVertex2, faceNormal, clippedAsciiChar});
                            }

                        }

                    }



                    std::sort(rasterTriangles.begin(), rasterTriangles.end(), [](Triangle &t1, Triangle &t2)
                    {
                        float z1 = (t1.vertex0.position.Z() + t1.vertex1.position.Z() + t1.vertex2.position.Z()) / 3.0f;
                        float z2 = (t2.vertex0.position.Z() + t2.vertex1.position.Z() + t2.vertex2.position.Z()) / 3.0f;
                        return z1 > z2;
                    });


                    for (auto &&tri : rasterTriangles) 
                    {
                        Triangle clippedTriangle[2];
                        std::list<Triangle> triangleList;

                        triangleList.push_back(tri);
                        uint8_t newTriangles = 1;

                        for (uint8_t i = 0; i < 4; i++) {

                            uint8_t trianglesToAdd = 0;
                            while(newTriangles > 0) {

                                Triangle testTriangle = triangleList.front();
                                triangleList.pop_front();
                                newTriangles--;

                                switch(i) {
                                    case 0:	trianglesToAdd = triangle_clip_against_plane<float>({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, testTriangle, clippedTriangle[0], clippedTriangle[1]); break;
                                    case 1:	trianglesToAdd = triangle_clip_against_plane<float>({ 0.0f, (float)this->m_screen_height - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, testTriangle, clippedTriangle[0], clippedTriangle[1]); break;
                                    case 2:	trianglesToAdd = triangle_clip_against_plane<float>({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, testTriangle, clippedTriangle[0], clippedTriangle[1]); break;
                                    case 3:	trianglesToAdd = triangle_clip_against_plane<float>({ (float)this->m_screen_width - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, testTriangle, clippedTriangle[0], clippedTriangle[1]); break;
                                }

                                for (uint8_t j = 0; j < trianglesToAdd; j++) {
                                    triangleList.push_back(clippedTriangle[j]);
                                }

                            }
                            newTriangles = triangleList.size();
                        }

                        for (auto &&tri : triangleList) 
                        {
                            const Vec2f p0{tri.vertex0.position.X(), tri.vertex0.position.Y()};
                            const Vec2f p1{tri.vertex1.position.X(), tri.vertex1.position.Y()};
                            const Vec2f p2{tri.vertex2.position.X(), tri.vertex2.position.Y()};
                            drawTriangle(p0, p1, p2, tri.asciiChar);
                        }


                    }
                    

                }
            }
    };
}

#endif
