// Include standard headers
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>


// Include GLEW
// OpenGL Extension Wrangler
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow *window;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);
GLfloat rng_float(void);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// GLSL language

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 vPos;\n"
    "layout (location = 1) in vec3 vCol;\n"
    "out vec4 fCol;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(vPos.x, vPos.y, vPos.z, 1.0);\n"
    "   fCol = vec4(vCol.r, vCol.g, vCol.b, 1.0);\n"
    "}\0";

const char *fragmentShaderSource =
    "#version 330 core\n"
    "in vec4 fCol;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(fCol);\n"
    "}\n\0";



int main(void) 
{
    // glfw stuff

    // initialize and configure glfw
    // ------------------------------
    if (!glfwInit()) {
        std::cerr << "Failed to initialize glfw.\n";
        exit(EXIT_FAILURE);
    }
    // multisampling anti-aliasing (MSAA) 4x
    glfwWindowHint(GLFW_SAMPLES, 4);

    // opengl version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // opengl core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(
        SCR_WIDTH, SCR_HEIGHT, "Icosa - Jimut Assignment - 1", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create glfw window." << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glew stuff

    // initialize glew
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize glew.\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // build and compile our shader program
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed.\n"
                  << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "fragment shader compilation failed.\n"
                  << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shade program linking failed.\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes

    // add a new set of vertices to form a second triangle (a total of 6
    // vertices); the vertex attribute configuration remains the same (still one
    // 3-float position vector per vertex)


    // https://github.com/sgonzalez/OpenGL-Platonic-Solids/blob/master/Classes/Icosahedron.h

    // const int num_icosahedron_indices = 60;
    // const int num_icosahedron_vertices = 36;
    // const int num_icosahedron_colors = 48;

    static const GLfloat icosahedronVertices[]= {
        0, -0.525731, 0.850651,             // vertices[0]
        0.850651, 0, 0.525731,              // vertices[1]
        0.850651, 0, -0.525731,             // vertices[2]
        -0.850651, 0, -0.525731,            // vertices[3]
        -0.850651, 0, 0.525731,             // vertices[4]
        -0.525731, 0.850651, 0,             // vertices[5]
        0.525731, 0.850651, 0,              // vertices[6]
        0.525731, -0.850651, 0,             // vertices[7]
        -0.525731, -0.850651, 0,            // vertices[8]
        0, -0.525731, -0.850651,            // vertices[9]
        0, 0.525731, -0.850651,             // vertices[10]
        0, 0.525731, 0.850651              // vertices[11]
    };

    /*
    static const GLfloat icosahedronColors[] = {
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.5, 0.0, 1.0,
        1.0, 1.0, 0.0, 1.0,
        0.5, 1.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 0.5, 1.0,
        0.0, 1.0, 1.0, 1.0,
        0.0, 0.5, 1.0, 1.0,
        0.0, 0.0, 1.0, 1.0,
        0.5, 0.0, 1.0, 1.0,
        1.0, 0.0, 1.0, 1.0,
        1.0, 0.0, 0.5, 1.0
    };
    */

    static const GLubyte icosahedronIndicies[] = {
        1, 2, 6,
        1, 7, 2,
        3, 4, 5,
        4, 3, 8,
        6, 5, 11,
        5, 6, 10,
        9, 10, 2,
        10, 9, 3,
        7, 8, 9,
        8, 7, 0,
        11, 0, 1,
        0, 11, 4,
        6, 2, 10,
        1, 6, 11,
        3, 5, 10,
        5, 4, 11,
        2, 7, 9,
        7, 1, 0,
        3, 9, 8,
        4, 8, 0,
    };

    GLfloat vertices[360];
    int counter = 0;
    //int color_counter = 0;
    for(int i=0;i<60;i++)
    {
        vertices[counter++] = icosahedronVertices[3*icosahedronIndicies[i]];
        vertices[counter++] = icosahedronVertices[3*icosahedronIndicies[i]+1];
        vertices[counter++] = icosahedronVertices[3*icosahedronIndicies[i]+2];
        /*
        if(i%3==0)
        {
            vertices[counter++] = icosahedronColors[color_counter++];
            vertices[counter++] = icosahedronColors[color_counter++];
            vertices[counter++] = icosahedronColors[color_counter++];
            color_counter++;
        }
        */
        vertices[counter++] = rng_float();
        vertices[counter++] = rng_float();
        vertices[counter++] = rng_float();
    }

    /*
        static const GLfloat icosahedronNormals[] = {
            0.000000, -0.417775, 0.675974,
            0.675973, 0.000000, 0.417775,
            0.675973, -0.000000, -0.417775,
            -0.675973, 0.000000, -0.417775,
            -0.675973, -0.000000, 0.417775,
            -0.417775, 0.675974, 0.000000,
            0.417775, 0.675973, -0.000000,
            0.417775, -0.675974, 0.000000,
            -0.417775, -0.675974, 0.000000,
            0.000000, -0.417775, -0.675973,
            0.000000, 0.417775, -0.675974,
            0.000000, 0.417775, 0.675973,
        };

    */








    /*
    const GLfloat X = 0.525731112119133606f;
    const GLfloat Z = 0.850650808352039932f;

    static GLfloat vdata[12][3] = {    
    {-X, 0.0f, Z}, {X, 0.0f, Z}, {-X, 0.0f, -Z}, {X, 0.0f, -Z},    
    {0.0f, Z, X}, {0.0f, Z, -X}, {0.0f, -Z, X}, {0.0f, -Z, -X},    
    {Z, X, 0.0f}, {-Z, X, 0.0f}, {Z, -X, 0.0f}, {-Z, -X, 0.0f},
    };

    static GLint tindices[20][3] = { 
    {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},    
    {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},    
    {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, 
    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

    GLfloat vertices[360]; 
    int counter = 0;

    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j< 3; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                vertices[counter++] = vdata[tindices[i][j]][k];
            }
            vertices[counter++] = rng_float();
            vertices[counter++] = rng_float();
            vertices[counter++] = rng_float();
        }
    }

    */

    GLuint VBO[1], VAO[1];

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                          (void *)(0 * sizeof(GLfloat)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                          (void *)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered
    // VBO as the vertex attribute's bound vertex buffer object so afterwards we
    // can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally
    // modify this VAO, but this rarely happens. Modifying other

    // VAOs requires a call to glBindVertexArray anyways so we generally don't
    // unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // sap black background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        process_input(window);

        // render
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our triangles
        glUseProgram(shaderProgram);
        // seeing as we only have a single VAO there's no need to bind it every
        // time, but we'll do so to keep things a bit more organized

        glBindVertexArray(VAO[0]);
        // set the count to 6 since we're drawing 9 vertices now (3 triangles);
        // not 3!

        glDrawArrays(GL_TRIANGLES, 0, 500);
        // no need to unbind it every time

        // glBindVertexArray(0);

        // swap buffers and poll IO events (keys pressed/released, mouse moved
        // etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);
    glDeleteProgram(shaderProgram);

    // terminate glfw, clearing all previously allocated GLFW resources.s
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

// process all input: query GLFW whether relevant keys are pressed/released
// this frame and react accordingly
void process_input(GLFWwindow *window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// whenever the window size changed (by OS or user resize) this callback
// function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) 
{
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);
}


static std::random_device rd;
static std::mt19937 gen(rd());

GLfloat rng_float(void)
{
  std::uniform_real_distribution<GLfloat> dis(0.0f,1.0f);
  return dis(gen);
}




