#include "Rendering/Renderer.h"

int main(int, char**)
{
    auto renderer = new Renderer();
    return renderer->Run(1200, 800, 1.15f);
}
