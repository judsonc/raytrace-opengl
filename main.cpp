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

#define STEP_LIGHT_R 0.1f //5 graus
#define STEP_R 0.1f       //5 graus
#define STEP_D 2.0f

float angX = 0;
float angY = 0;
float angZ = 0;
int moonAngle = 0;
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
/***** Informacoes para calculo de projecao ****/
/*
* Informacoes ficam em RayTracer.viewerData, essas informacoes possuem um default
* caso nao seja setado apos a construcao de um objeto RayTracer, sera:
* znear(default = 0.01), zfar (default = 10000.0)
* Camera: from(0.0 ,0.0, -0.5), lookAt(0.0, 0.0, 0.0), vUp(0.0, 1.0, 0.0)
* angulo de abertura do pespective (default = 60.0)
* dimensoes da tela: 640x480;
* posicionamento e orientacao da Camera;
*/
//Cria e posiciona os Objetos
//Cria e posiciona os pontos de luz
void init(int win_width, int win_height)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  Material material;
  Vec pos, normal;
  float radius = 1.0;

  //Configurando a matriz de projecao
  raytracer.viewer.setWindowSize(win_width, win_height); //atualiza dimensoes da tela
  raytracer.viewer.moveCamera(Vec(0.0, 7.0, -10.0));

  raytracer.world.bgColor = Vec(0, 0, 0);
  raytracer.world.lightEnv = 1.0;
  raytracer.world.ka = 0.0;

  // Moon
  material.n_shiny = 10.0;
  material.kr = 0.0;
  material.color = Vec(1.0, 1.0, 1.0);
  material.setKs(0.1, raytracer.world.ka);
  pos = Vec(0, 0, 10.0);
  radius = 0.5;
  raytracer.world.objs.push_back(new Sphere(material, pos, radius));

  // Earth
  material.n_shiny = 200.0;
  material.color = Vec(0.17, 0.46, 1.0);
  material.setKs(0.5, raytracer.world.ka);
  material.kr = 0.6;
  pos = Vec(0, 0, 0);
  radius = 1.83;
  raytracer.world.objs.push_back(new Sphere(material, pos, radius));

  // Wall
  material.kr = 0.1;
  material.n_shiny = 30.0;
  material.color = Vec(0.6, 0.6, 0.6);
  material.setKs(0.7, raytracer.world.ka);
  pos = Vec(0, 0, 90.0);
  normal = Vec(0, 0, -1.0);
  raytracer.world.objs.push_back(new Plane(material, pos, normal));

  // Wall
  material.kr = 0.0;
  material.n_shiny = 30.0;
  material.color = Vec(0.6, 0.6, 0.6);
  material.setKs(0.1, raytracer.world.ka);
  pos = Vec(90, 0, 0.0);
  normal = Vec(-1, 0, 0);
  raytracer.world.objs.push_back(new Plane(material, pos, normal));

  // Wall
  material.kr = 0.0;
  material.n_shiny = 30.0;
  material.color = Vec(0.6, 0.6, 0.6);
  material.setKs(0.1, raytracer.world.ka);
  pos = Vec(-90, 0, 0.0);
  normal = Vec(1, 0, 0);
  raytracer.world.objs.push_back(new Plane(material, pos, normal));

  // Wall
  material.kr = 0.0;
  material.n_shiny = 30.0;
  material.color = Vec(0.6, 0.6, 0.6);
  material.setKs(0.6, raytracer.world.ka);
  pos = Vec(0, 0, -90.0);
  normal = Vec(0, 0, 1);
  raytracer.world.objs.push_back(new Plane(material, pos, normal));

  // Wall
  material.kr = 0.0;
  material.n_shiny = 30.0;
  material.color = Vec(0.6, 0.6, 0.6);
  material.setKs(0.1, raytracer.world.ka);
  pos = Vec(0, 90, 0.0);
  normal = Vec(0, -1, 0);
  raytracer.world.objs.push_back(new Plane(material, pos, normal));

  // Wall
  material.kr = 0.0;
  material.n_shiny = 30.0;
  material.color = Vec(0.6, 0.6, 0.6);
  material.setKs(0.3, raytracer.world.ka);
  pos = Vec(0, -90, 0.0);
  normal = Vec(0, 1, 0);
  raytracer.world.objs.push_back(new Plane(material, pos, normal));

  raytracer.world.lights.push_back(LightSource(Vec(1.5, 5.0, -5.0), Vec(1.0, 1.0, 1.0)));
};

void spinMoon(int value)
{
  moonAngle = (moonAngle + 1) % 360;

  glm::vec4 posLight(raytracer.world.lights.begin()->pos, 1.0);
  glm::mat4 model = glm::mat4(0.4f);

  glm::vec3 tvec = glm::vec3(25.0f, 0.0f, 0.0f);
  glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::mat4 translate = glm::translate(model, tvec);
  glm::mat4 rotate = glm::rotate(model, (GLfloat)moonAngle, axis);

  posLight = translate * posLight * rotate;
  Vec aux(posLight.x, posLight.y, posLight.z);

  raytracer.world.objs.front()->pos = aux;

  display();
  glutTimerFunc(500, spinMoon, 0);
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
