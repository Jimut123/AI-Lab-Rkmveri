// Include standard headers
#include <cstdlib>
#include <iostream>

// Include GLEW
// OpenGL Extension Wrangler
#include <GL/glew.h>



#include <GL/glut.h>



// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow *window;


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

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
        SCR_WIDTH, SCR_HEIGHT, "OpenGL Code Snippets : Triangles", NULL, NULL);
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

    // Bad code: position and color are different types
    
    GLfloat vertices[] = {

        // first square = extreme top left

        -0.5f, 0.5f, 0.0f,   // top left
        1.0f, 0.0f, 0.0f,    // dark red
        -0.5f, 0.0f, 0.0f,   // extreme left
        1.0f, 0.0f, 0.0f,    // dark red
        0.0f, 0.5f, 0.0f,    // middle top
        1.0f, 0.0f, 0.0f,    // dark red
        
        -0.5f, 0.0f, 0.0f,   // extreme left
        1.0f, 0.0f, 0.0f,    // dark red
        0.0f, 0.5f, 0.0f,    // middle top
        1.0f, 0.0f, 0.0f,    // dark red
        0.0f, 0.0f, 0.0f,    // middle 
        1.0f, 0.0f, 0.0f,    // dark red
        


        // second square = extreme top right

        0.0f, 0.0f, 0.0f,    // middle
        0.0f, 1.0f, 0.0f,    // dark green
        0.0f, 0.5f, 0.0f,    // middle top
        0.0f, 1.0f, 0.0f,    // dark green
        0.5f, 0.0f, 0.0f,    // extreme left middle
        0.0f, 1.0f, 0.0f,    // dark green
        
        0.0f, 0.5f, 0.0f,    // middle top
        0.0f, 1.0f, 0.0f,    // dark green
        0.5f, 0.5f, 0.0f,    // extreme right top
        0.0f, 1.0f, 0.0f,    // dark green
        0.5f, 0.0f, 0.0f,    // extreme right
        0.0f, 1.0f, 0.0f,    // dark green


        // third square = extreme bottom right

        
        0.0f, 0.0f, 0.0f,   // middle
        1.0f, 1.0f, 0.0f,    // dark yellow
        0.5f, 0.0f, 0.0f,    // middle right
        1.0f, 1.0f, 0.0f,    // dark yellow
        0.0f, -0.5f, 0.0f,    // bottom
        1.0f, 1.0f, 0.0f,    // dark yellow
        

        0.5f, 0.0f, 0.0f,   // middle right
        1.0f, 1.0f, 0.0f,    // dark yellow
        0.5f, -0.5f, 0.0f,    // bottom right
        1.0f, 1.0f, 0.0f,    // dark yellow
        0.0f, -0.5f, 0.0f,    // bottom middle
        1.0f, 1.0f, 0.0f,    // dark yellow



        // fourth square
        
        0.0f, 0.0f, 0.0f,    // middle
        0.0f, 0.0f, 1.0f,    // dark blue
        -0.5f, 0.0f, 0.0f,    // middle left
        0.0f, 0.0f, 1.0f,    // dark blue
        0.0f, -0.5f, 0.0f,    // middle bottom
        0.0f, 0.0f, 1.0f,    // dark blue
        
        -0.5f, 0.0f, 0.0f,    // middle left
        0.0f, 0.0f, 1.0f,    // dark blue
        -0.5f, -0.5f, 0.0f,    // extreme left bottom
        0.0f, 0.0f, 1.0f,    // dark blue
        0.0f, -0.5f, 0.0f,    // extreme bottom
        0.0f, 0.0f, 1.0f,    // dark blue

        
    };


    // Add text to the screen
   



    // ************************************


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

    // white background
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

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

        glDrawArrays(GL_TRIANGLES, 0, 24);
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




