#include "Softbody.h"

#include "Sphere.h"
#include "Spring.h"

void Softbody::Build(
  PhysicsScene* scene,
  glm::vec2 position,
  float spacing,
  float springForce,
    float damping,
  std::vector<std::string>& strings)
{
  int numColumns = strings.size();
  int numRows = strings[0].length();

  // traverse across the array and add balls where the ascii art says they
  // should be
  Sphere** spheres = new Sphere*[numRows * numColumns];
  for (int i = 0; i < numRows; i++)
  {
    for (int j = 0; j < numColumns; j++)
    {
      if (strings[j][i] == '0')
      {
        spheres[i * numColumns + j] = new Sphere(
          position + glm::vec2(i, j) * spacing,
          glm::vec2(0),
          2.0f,
          1.0f,
          1.0f,
          glm::vec4(1, 0, 0, 1));
        scene->addActor(spheres[i * numColumns + j]);
      }
      else
      {
        spheres[i * numColumns + j] = nullptr;
      }
    }
  }

  // Build the springs
  for (int i = 1; i < numRows; i++)
  {
    for (int j = 1; j < numColumns; j++)
    {
      Sphere* s11 = spheres[i * numColumns + j];
      Sphere* s01 = spheres[(i - 1) * numColumns + j];
      Sphere* s10 = spheres[i * numColumns + j - 1];
      Sphere* s00 = spheres[(i - 1) * numColumns + j - 1];

      // make springs to cardinal neighbours
      if (s11 && s01)
      {
        scene->addActor(new Spring(s11, s01, springForce, damping));
      }
      if (s11 && s10)
      {
        scene->addActor(new Spring(s11, s10, springForce, damping));
      }

      if (s10 && s00)
      {
        scene->addActor(new Spring(s10, s00, springForce, damping));
      }

      if (s01 && s00)
      {
        scene->addActor(new Spring(s01, s00, springForce, damping));
      }

      // make springs to diagonals
      if (s11 && s00)
      {
        scene->addActor(new Spring(s11, s00, damping, springForce));
      }

      if (s01 && s10)
      {
        scene->addActor(new Spring(s01, s10, springForce, damping));
      }
      
    }
  }
}
