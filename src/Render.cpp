//
// Render.cpp
// GEL
//
// Created by Usama Alshughry 25.09.2022.
// Copyright © 2022 Usama Alshughry. All rights reserved
//

#include "Render.hpp"
#include "Log.hpp"
#include "ShaderProgram.hpp"

namespace ge
{

static uint32_t flushCounter;

static GLFWwindow* m_window;
static ivec2 m_gridSize;
static ivec2 m_size;
static ivec2 m_tileSize;
static ShaderProgram program;
extern bool pause;

static void framebufferSizeCallback(GLFWwindow* window, int w, int h)
{
  glViewport(0, 0, w, h);
}

struct DrawObject
{
  uint32_t vao, vboPos, vboTrans, vboCol;
};

static DrawObject quadObject;

static void initDrawObjects(float side, ivec2 gridSize);
static void loadProgram();
static void flush();

static void internalInit(int side, ivec2 gridSize)
{
  initDrawObjects((float)side, gridSize);
  loadProgram();
}

struct Vertex
{
  vec2 position;
  vec4 color;
};

std::vector<vec4> colors;
std::vector<vec2> translations;
static size_t totalQuads = 0;

static void initDrawObjects();

bool Render::Init(GLFWwindow** window, const std::string& title, ivec2 size, ivec2 tileSize)
{
  AssertReturn(glfwInit(), false, "Failed to init glfw");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
#endif

  int count;
  int choice;
  int monitorChoice;
  GLFWmonitor** monitors = glfwGetMonitors(&count);
  printf("%d monitors available\n", count);
  std::cout << "select monitor: ";
  std::cin >> monitorChoice;
  const GLFWvidmode* modes = glfwGetVideoModes(monitors[monitorChoice - 1], &count);
  printf("available modes:\n");

  for (int i = 0; i < count; ++i)
  {
    printf("%d) %dx%d:%d\n", i + 1, modes[i].width, modes[i].height, modes[i].refreshRate);
  }

  std::cout << "Enter prefered mode: ";
  std::cin >> choice;

  const GLFWvidmode* mode = &modes[choice - 1];
  GLFWmonitor* monitor = monitors[monitorChoice - 1];

  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  m_window = glfwCreateWindow(
      mode->width,
      mode->height,
      title.c_str(),
      monitor,
      NULL
  );
  Printf("%d %d", mode->width, mode->height);

  glfwSetWindowMonitor(m_window,
      monitors[monitorChoice - 1],
      0,
      0,
      mode->width,
      mode->height,
      mode->refreshRate);

  m_size = ivec2{mode->width, mode->height};
  m_tileSize = tileSize;
  m_gridSize = {
    m_size.x / m_tileSize.x,
    m_size.y / m_tileSize.y,
  };
  AssertReturn(m_window != nullptr, false, "Failed to create window");
  *window = m_window;
  glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

  glfwMakeContextCurrent(m_window);

  AssertReturn(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), false, "Failed to load GL");
  Print("OpenGL Loaded...");
  Print("Vendor:      ", glGetString(GL_VENDOR));
  Print("Renderer:    ", glGetString(GL_RENDERER));
  Print("Version:     ", glGetString(GL_VERSION));

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  internalInit(m_tileSize.x, m_gridSize);
  return true;
}

void Render::begin()
{
  // glClearColor(0.1, 0.08, 0.02, 1.0);
  glClearColor(0, 0, 0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::end()
{
  flush();
  glfwSwapBuffers(m_window);
  // Printf("Flushed %d times this frame", flushCounter);
  // flushCounter = 0;
}

void flush()
{
  program.use();
  glBindVertexArray(quadObject.vao);
  glBindBuffer(GL_ARRAY_BUFFER, quadObject.vboTrans);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * translations.size(),
      translations.data());
  glBindBuffer(GL_ARRAY_BUFFER, quadObject.vboCol);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * colors.size(),
      colors.data());
  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, colors.size());
  colors.clear();
  translations.clear();
}

void Render::Quit()
{
  glfwTerminate();
}

void Render::fillRect(vec2 pos, vec4 color)
{
  translations.push_back(pos);
  colors.push_back(color);
}

void initDrawObjects(float side, ivec2 gridSize)
{
  // float quadVertices[] = {
  //    0,        0,
  //    side - 1, 0,
  //    side - 1, side - 1,
  //    0,        0,
  //    side - 1, side - 1,
  //    0,        side - 1
  // };

  float s = side * 0.7;
  float quadVertices[] = {
     0, 0,
     s, 0,
     s, s,
     0, 0,
     s, s,
     0, s
  };

  glGenVertexArrays(1, &quadObject.vao);
  glGenBuffers(3, &quadObject.vboPos);

  glBindVertexArray(quadObject.vao);
  glBindBuffer(GL_ARRAY_BUFFER, quadObject.vboPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
      GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, quadObject.vboTrans);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * gridSize.w * gridSize.h, nullptr,
      GL_DYNAMIC_DRAW);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribDivisor(1, 1);

  glBindBuffer(GL_ARRAY_BUFFER, quadObject.vboCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * gridSize.w * gridSize.h, nullptr,
      GL_DYNAMIC_DRAW);

  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), 0);
  glEnableVertexAttribArray(2);
  glVertexAttribDivisor(2, 1);
}

void loadProgram()
{
  program.load("shaders/default.vert", "shaders/default.frag");
  program.use();
  auto location = glGetUniformLocation(program.getId(), "projection");
  float mat[16] = {0};
  // vec2 scale(1.f, 1.f);
  // glfwGetWindowContentScale(m_window, &scale.x, &scale.y);
  float l = 0.f;
  float r = m_size.w;// * m_tileSize.w;// * scale.x;
  float t = 0;
  float b = m_size.h;// * m_tileSize.h;// * scale.y;
  float n = -2.f;
  float f = 2.f;
  // Print(scale, l, r, t,b );
  mat[0]  = 2.f/(r-l);
  mat[5]  = 2.f/(t-b);
  mat[10] = 2.f/(f-n);
  mat[12] = -(r+l)/(r-l);
  mat[13] = -(t+b)/(t-b);
  mat[14] = -(f+n)/(f-n);
  mat[15] = 1;
  glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}

} // namespace ge
