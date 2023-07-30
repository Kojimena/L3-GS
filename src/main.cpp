#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include <ctime>
#include "gl.h"
#include "load.h"

// Suponemos que esta es la firma de tu función drawTriangle
void drawTriangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
    // Transform model coordinates to screen coordinates
    int x1 = (v1.x + 1) * SCREEN_WIDTH / 2;
    int y1 = (v1.y + 1) * SCREEN_HEIGHT / 2;
    int x2 = (v2.x + 1) * SCREEN_WIDTH / 2;
    int y2 = (v2.y + 1) * SCREEN_HEIGHT / 2;
    int x3 = (v3.x + 1) * SCREEN_WIDTH / 2;
    int y3 = (v3.y + 1) * SCREEN_HEIGHT / 2;

    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);

    // Draw the first side of the triangle
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    // Draw the second side of the triangle
    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
    // Draw the third side of the triangle
    SDL_RenderDrawLine(renderer, x3, y3, x1, y1);
}


std::vector<glm::vec3> modelVertices;

void render() {
    for (size_t i = 0; i < modelVertices.size(); i += 3) {
        drawTriangle(modelVertices[i], modelVertices[i+1], modelVertices[i+2]);
    }
}

std::vector<glm::vec3> setupVertexArray(const std::vector<glm::vec3>& vertices, const std::vector<Face>& faces)
{
    std::vector<glm::vec3> vertexArray;

    // For each face
    for (const auto& face : faces)
    {
        // For each vertex index in the face
        for (const auto& vertexIndex : face.vertexIndices)
        {
            // Get the vertex position from the input arrays using the index from the face
            glm::vec3 vertexPosition = vertices[vertexIndex];

            // Add the vertex position to the vertex array
            vertexArray.push_back(vertexPosition);
        }
    }

    return vertexArray;
}



int main(int argc, char** argv) {
    init();

    std::vector<glm::vec3> vertices;
    std::vector<Face> faces;
    if (!loadOBJ("/Users/jime/Downloads/cubee.obj", vertices, faces)) {
        std::cerr << "Failed to load model" << std::endl;
        return 1;
    }

    modelVertices = setupVertexArray(vertices, faces);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        clear();
        render();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    SDL_PumpEvents();

    return 0;
}


