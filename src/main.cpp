#include "Graphics3DEngine.hpp"

int main() {

    // Change screen resolution here.
    cgel::Graphics3DEngine rw(941, 400);
    //                                                        Change object file here.
    //                                                                   |
    //                                                                   v
    cgel::Mesh mesh = cgel::constructMeshFromObjectFile("ObjectFiles/sword.obj"); 
    rw.addMesh(mesh);

    while (1)
    {
        rw.update();
        rw.display();     
    }

    return 0;
}
