#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <GL/glut.h>
#include <raytrace.h>

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <omp.h>

#define WIDTH 640
#define HEIGHT 480

float angX = 0;
float angY = 0;
float angZ = 0;
float moonAngle = 0;
int numReflection = 0;

RayTracer raytracer;
ImageRGBf img(WIDTH, HEIGHT);

void display(void)
{
  raytracer.rayTrace(img, numReflection);
  glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, img.data);
  glutSwapBuffers();
};

//inicializa o universo
void init(int win_width, int win_height)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  Material material;
  Vec pos, normal;
  float radius;

  //Configurando a matriz de projecao
  raytracer.viewer.setWindowSize(win_width, win_height); //atualiza dimensoes da tela
  raytracer.viewer.moveCamera(Vec(0.0, 7.0, -10.0));

  raytracer.world.bgColor = Vec(0, 0, 0);
  raytracer.world.lightEnv = 1.0;
  raytracer.world.ka = 0.0;

  //Moon
  material.kr = 0.0;
  material.n_shiny = 10.0;
  material.color = Vec(1.0, 1.0, 1.0);
  material.setKs(0.1, raytracer.world.ka);
  pos = Vec(0, 0, 10.0);
  radius = 0.5;
  raytracer.world.objs.push_back(new Sphere(material, pos, radius));

  //Earth
  material.kr = 0.0;
  material.n_shiny = 10.0;
  material.color = Vec(0.17, 0.46, 1.0);
  material.setKs(0.1, raytracer.world.ka);
  pos = Vec(0.0, 0.0, 0.0);
  radius = 1.83;
  raytracer.world.objs.push_back(new Sphere(material, pos, radius));

  //Wall normal axis Z
  material.kr = 0.0;
  material.n_shiny = 30.0;
  material.color = Vec(0.3, 0.3, 0.3);
  material.setKs(0.1, raytracer.world.ka);
  pos = Vec(0.0, 0.0, 90.0);
  normal = Vec(0.0, 0.0, -1.0);
  raytracer.world.objs.push_back(new Plane(material, pos, normal));

  //Wall normal axis Y
  material.kr = 0.0;
  material.n_shiny = 30.0;
  material.color = Vec(0.3, 0.3, 0.3);
  material.setKs(0.3, raytracer.world.ka);
  pos = Vec(0.0, -90.0, 0.0);
  normal = Vec(0.0, 1.0, 0.0);
  raytracer.world.objs.push_back(new Plane(material, pos, normal));

  //Wall normal axis X
  material.kr = 0.0;
  material.n_shiny = 30.0;
  material.color = Vec(0.3, 0.3, 0.3);
  material.setKs(0.1, raytracer.world.ka);
  pos = Vec(60.0, 0.0, 0.0);
  normal = Vec(-1.0, 0.0, 0.0);
  raytracer.world.objs.push_back(new Plane(material, pos, normal));

  //Sunlight
  raytracer.world.lights.push_back(LightSource(Vec(1.5, 5.0, -5.0), Vec(1.0, 1.0, 1.0)));
};

void spinMoon(int value)
{
  moonAngle += 0.1;

  glm::vec4 posLight(raytracer.world.lights.begin()->pos, 1.0);
  glm::mat4 model = glm::mat4(0.4f);

  glm::vec3 tvec = glm::vec3(25.0f, 0.0f, 0.0f);
  glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::mat4 translate = glm::translate(model, tvec);
  glm::mat4 rotate = glm::rotate(model, (GLfloat) moonAngle, axis);

  posLight = translate * posLight * rotate;
  Vec aux(posLight.x, posLight.y, posLight.z);

  raytracer.world.objs.front()->pos = aux;

  display();
  glutTimerFunc(100, spinMoon, 0);
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(0, 150);
  glutCreateWindow("Projeto Final - Computacao Grafica 2020.5");
  init(WIDTH, HEIGHT);

  glutDisplayFunc(display);
  glutTimerFunc(1, spinMoon, 0);
  glutMainLoop();

  return 0;
}
