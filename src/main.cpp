#include "Graphics3DEngine.hpp"

int main() {

    cgel::Graphics3DEngine rw(941, 400);

    cgel::Mesh mesh = cgel::constructMeshFromObjectFile("ObjectFiles/sword.obj");
    rw.addMesh(mesh);

    while (1)
    {
        rw.update();
        rw.display();     
    }

    return 0;
}
