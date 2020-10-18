#include "TestGLM.h"
#include "vendor/glm/vec3.hpp" // glm::vec3
#include "vendor/glm/vec4.hpp" // glm::vec4
#include "vendor/glm/mat4x4.hpp" // glm::mat4
#include "vendor/glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale
#include "vendor/glm/ext/matrix_clip_space.hpp" // glm::perspective
#include "vendor/glm/ext/scalar_constants.hpp" // glm::pi
#include "vendor/glm/gtx/string_cast.hpp"
#include "vendor/glm/ext/matrix_transform.hpp"
#include <stdio.h>

void TestGLM::setUp() {
    printf("TestGLM::setUp \n");
}

void TestGLM::run() {

  glm::vec4 vec(5.0f, 6.0f, 7.0f, 1.0f);//创建一个点
  printf("ori: %s\n", glm::to_string(vec).c_str());
  glm::mat4 Identity(1.0f);

  /******************
  1 0 0 2     5     7

  0 1 0 2     6     8
           x     =
  0 0 1 2     7     9

  0 0 0 1     1     1
  ******************/
	glm::mat4 trans = glm::translate(Identity, glm::vec3(2.0, 2.0, 2.0));//设置平移矩阵
  printf("%s\n", glm::to_string(trans).c_str());
	glm::vec4 vecTrans = trans * vec;//变换矩阵左乘点向量，获得变换后的点
  printf("trans: %s\n", glm::to_string(vecTrans).c_str());


  /******************
  2 0 0 0     5     10

  0 2 0 0     6     12
           x     =
  0 0 2 0     7     14

  0 0 0 1     1     1
  ******************/
  glm::mat4 ScalingMatrix = glm::scale(Identity, glm::vec3(2.0f, 2.0f, 2.0f));
  glm::vec4 vecScale = ScalingMatrix * vec;
  printf("scale %s\n", glm::to_string(vecScale).c_str());

  glm::mat4 ortho = glm::ortho(0.0f, 800.0f, 0.0f, 480.0f, 1.0f, -1.0f);
  printf("ortho %s\n", glm::to_string(ortho).c_str());
  glm::vec4 v1(500.0f, 100.0f, 0.0f, 1.0f);
  glm::vec4 v1Ortho = ortho * v1;
  printf("v1Ortho %s\n", glm::to_string(v1Ortho).c_str());

}

void TestGLM::tearDown() {
    printf("TestGLM::tearDown \n\n");
}

