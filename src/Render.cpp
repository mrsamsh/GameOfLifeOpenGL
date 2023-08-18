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

#define OFFSET(a, b) (void*)(offsetof(a, b))

namespace ge
{

static uint32_t flushCounter;

static constexpr size_t MaxQuads = 10000;
static GLFWwindow* m_window;
static ivec2 m_size;
static ivec2 m_tileSize;
static ShaderProgram program;

static void framebufferSizeCallback(GLFWwindow* window, int w, int h)
{
  glViewport(0, 0, w, h);
}

struct DrawObject
{
  uint32_t vao, vbo, ebo;
};

static DrawObject quadObject;

static void initDrawObjects();
static void initColorTexture();
static void loadProgram();
static void flush();

static void internalInit()
{
  initDrawObjects();
  initColorTexture();
  loadProgram();
}

struct Vertex
{
  vec3 position;
  vec2 texCoord;
  vec4 color;
  float textureIndex;
};

static Vertex quadVertices[MaxQuads * 4];
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
  m_tileSize = {1, 1};
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

  internalInit();
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
  Printf("Flushed %d times this frame", flushCounter);
  flushCounter = 0;
}

void flush()
{
  flushCounter++;
  program.use();
  glBindVertexArray(quadObject.vao);
  glBindBuffer(GL_ARRAY_BUFFER, quadObject.vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
  glDrawElements(GL_TRIANGLES, totalQuads * 6, GL_UNSIGNED_INT, NULL);
  totalQuads = 0;
}

void Render::Quit()
{
  glfwTerminate();
}

void Render::fillRect(vec2 pos, vec2 size, vec4 color)
{
  if (totalQuads == MaxQuads)
  {
    flush();
    totalQuads = 0;
  }
  size /= 2.f;
  quadVertices[totalQuads * 4 + 0].position = vec3(
      pos.x - size.w, pos.y - size.h, 0.f);
  quadVertices[totalQuads * 4 + 0].color = color;
  quadVertices[totalQuads * 4 + 0].textureIndex = 0;
  quadVertices[totalQuads * 4 + 0].texCoord = {0, 0};

  quadVertices[totalQuads * 4 + 1].position = vec3(
      pos.x + size.w, pos.y - size.h, 0.f);
  quadVertices[totalQuads * 4 + 1].color = color;
  quadVertices[totalQuads * 4 + 1].textureIndex = 0;
  quadVertices[totalQuads * 4 + 1].texCoord = {1, 0};

  quadVertices[totalQuads * 4 + 2].position = vec3(
      pos.x + size.w, pos.y + size.h, 0.f);
  quadVertices[totalQuads * 4 + 2].color = color;
  quadVertices[totalQuads * 4 + 2].textureIndex = 0;
  quadVertices[totalQuads * 4 + 2].texCoord = {1, 1};

  quadVertices[totalQuads * 4 + 3].position = vec3(
      pos.x - size.w, pos.y + size.h, 0.f);
  quadVertices[totalQuads * 4 + 3].color = color;
  quadVertices[totalQuads * 4 + 3].textureIndex = 0;
  quadVertices[totalQuads * 4 + 3].texCoord = {0, 1};

  totalQuads++;
}

void initDrawObjects()
{
  // populate quadIndices
  uint32_t quadIndices[MaxQuads * 6];
  for (size_t i = 0; i < MaxQuads; ++i)
  {
    size_t index = i * 6;
    size_t vertIndex = i * 4;
    quadIndices[index + 0] = vertIndex + 0;
    quadIndices[index + 1] = vertIndex + 1;
    quadIndices[index + 2] = vertIndex + 2;
    quadIndices[index + 3] = vertIndex + 0;
    quadIndices[index + 4] = vertIndex + 2;
    quadIndices[index + 5] = vertIndex + 3;
  }

  glGenVertexArrays(1, &quadObject.vao);
  glGenBuffers(2, &quadObject.vbo);

  glBindVertexArray(quadObject.vao);
  glBindBuffer(GL_ARRAY_BUFFER, quadObject.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), NULL, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadObject.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices,
      GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      OFFSET(Vertex, position));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      OFFSET(Vertex, texCoord));
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      OFFSET(Vertex, color));
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      OFFSET(Vertex, textureIndex));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
}

void initColorTexture()
{
  uint32_t texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  uint8_t solid_white[] = { 255, 255, 255, 255 };
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
      solid_white);
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
  float r = m_size.w * m_tileSize.w;// * scale.x;
  float t = 0;
  float b = m_size.h * m_tileSize.h;// * scale.y;
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
