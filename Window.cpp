#include "Window.h"

int Window::width;
int Window::height;
int Window::event;
int Window::flag_n = 1;
int Window::mode = 1;
const char* Window::windowTitle = "GLFW Starter Project";

// Objects to display.
Cube * Window::cube;
PointCloud * Window::cubePoints;
PointCloud * Window::bear;
PointCloud * Window::dragon;
PointCloud * Window::bunny;
PointCloud * Window::cat;


// The object currently displaying.
PointCloud * Window::currentObj;
PointCloud * Window::sphere;

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(0, 0, 20); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.
glm::vec3 Window::lastPoint;
glm::vec3 Window::curPos;
glm::vec3 Window::rotAxis;

glm::vec3 lightPos(1.0f, 1.0f, 8.0f);

GLfloat Window::angle;

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

GLuint Window::program; // The shader program id.

GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::modelLoc; // Location of model in shader.
GLuint Window::colorLoc; // Location of color in shader.
GLuint Window::light_position;
GLuint Window::light_linear;
GLuint Window::viewPos;
GLuint Window::light_color;
GLuint Window::obj_color;
GLuint Window::material_diffuse;
GLuint Window::material_shininess;
GLuint Window::material_specular;
GLuint Window::flag;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	colorLoc = glGetUniformLocation(program, "color");
    viewPos = glGetUniformLocation(program, "viewPos");
    light_position = glGetUniformLocation(program, "lightPos");
    light_linear = glGetUniformLocation(program, "light_linear");
    light_color = glGetUniformLocation(program, "lightColor");
    obj_color = glGetUniformLocation(program, "objectColor");
    material_diffuse = glGetUniformLocation(program, "material.diffuse");
    material_specular = glGetUniformLocation(program, "material.specular");
    material_shininess  = glGetUniformLocation(program, "material.shininess");
    flag = glGetUniformLocation(program, "flag");
    
    event = 0;
    
	return true;
}

bool Window::initializeObjects()
{
	// Create a cube of size 5.
	cube = new Cube(5.0f);
	// Create a point cloud consisting of cube vertices.
	cubePoints = new PointCloud("foo", 100);
    
    // BEAR CONFIG
    bear = new PointCloud("bear.obj", 0.25);
    bear->set_diffuse(glm::vec3(1.0f, 1.0f, 1.0f));
    bear->set_specular(glm::vec3(0.0f, 0.0f, 0.0f));
    bear->set_shininess(0.0f);
    bear->setColor(glm::vec3(0.8f, 0.5f, 0.0f));
    
    // DRAGON  CONFIG
    dragon = new PointCloud("dragon.obj", 0.25);
    dragon->set_diffuse(glm::vec3(0.0f, 0.0f,  0.0f));
    dragon->set_specular(glm::vec3(1.0f, 1.0f, 1.0f));
    dragon->set_shininess(128.0f);
    dragon->setColor(glm::vec3(0.3f, 0.4f, 1.0f));
    
    // BUNNY CONFIG
    bunny = new PointCloud("bunny.obj", 0.25);
    bunny->set_diffuse(glm::vec3(1.0f, 1.0f,  1.0f));
    bunny->set_specular(glm::vec3(1.0f, 1.0f,  1.0f));
    bunny->set_shininess(128.0f);
    bunny->setColor(glm::vec3(0.1f, 0.8f, 0.5f));
    
    //cat = new PointCloud("cat.obj", 10);
    
    sphere = new PointCloud("sphere.obj", 0.25);
    // sphere color = light color
    sphere->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    sphere->set_diffuse(glm::vec3(1.0f, 1.0f,  1.0f));
    sphere->set_specular(glm::vec3(1.0f, 1.0f,  1.0f));
    sphere->set_shininess(1.0f);
    
    sphere->scale(glm::scale(glm::vec3(0.05f, 0.05f, 0.05f)));
    sphere->translate(glm::translate(lightPos));
    
	// Set cube to be the first to display
	currentObj = bear;

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
	delete cubePoints;

	// Delete the shader program.
	glDeleteProgram(program);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);
    glfwSetScrollCallback(window, scroll_callback);
	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
		double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	// Perform any updates as necessary. 
	//currentObj->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Specify the values of the uniform variables we are going to use.
	glm::mat4 model = currentObj->getModel();
	glm::vec3 color = currentObj->getColor();
    glm::vec3 diffuse = currentObj->get_diffuse();
    glm::vec3 specular = currentObj->get_specular();
    float shininess = currentObj->get_shininess();
    
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    
    // light properties
    glUniform3fv(light_position, 1, glm::value_ptr(lightPos));
    glUniform3fv(viewPos, 1, glm::value_ptr(eye));
    glUniform1f(light_linear, 0.09f);
    glUniform3fv(light_color, 1, glm::value_ptr(glm::vec3(0.0f, 0.5f, 0.9f)));
    glUniform3fv(obj_color, 1, glm::value_ptr(color));
    
    // material properties
    glUniform3fv(material_diffuse, 1, glm::value_ptr(diffuse));
    glUniform3fv(material_specular, 1, glm::value_ptr(specular));
    glUniform1f(material_shininess, shininess);
    glUniform1i(flag, flag_n);

	// Render the object.
	currentObj->draw();
    
    glm::mat4 sphere_model = sphere->getModel();
    glm::vec3 sphere_color = sphere->getColor();
    glm::vec3 sphere_diffuse = sphere->get_diffuse();
    glm::vec3 sphere_specular = sphere->get_specular();
    float sphere_shininess = sphere->get_shininess();
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sphere_model));
    glUniform3fv(obj_color, 1, glm::value_ptr(sphere_color));
    glUniform3fv(material_diffuse, 1, glm::value_ptr(sphere_diffuse));
    glUniform3fv(material_specular, 1, glm::value_ptr(sphere_specular));
    glUniform1f(material_shininess, sphere_shininess);

    sphere->draw();
    
	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;
                
        // Mode 1
		case GLFW_KEY_1:
            mode = 1;
			break;
                
        // Mode 2
		case GLFW_KEY_2:
            mode = 2;
			break;
        
        // Mode 3
        case GLFW_KEY_3:
            mode = 3;
            break;
        
        case GLFW_KEY_N:
            if(flag_n){
                glUniform1i(flag, 0);
                flag_n = 0;
            }
            else{
                glUniform1i(flag, 1);
                flag_n = 1;
            }
            
            break;
        
        case GLFW_KEY_P:
            // Point size adjustment key 'P' pressed
            // Uppercase
            if(mods == GLFW_MOD_SHIFT){
                currentObj->updatePointSize(1);
            }
            // Lowercase
            else{
                currentObj->updatePointSize(-1);
            }
                
            break;
            
        case GLFW_KEY_F1:
                currentObj = bear;
                break;
                
        case GLFW_KEY_F2:
                currentObj = bunny;
                break;
                
        case GLFW_KEY_F3:
                currentObj = dragon;
                break;
                
        case GLFW_KEY_F4:
                currentObj = cat;
                break;
                
		default:
			break;
		}
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    switch (mode) {
        case 1:{
            // Scroll up
            if(yoffset > 0){
                currentObj->scale(1);
            }
            // Scroll down
            else{
                currentObj->scale(0);
            }
            break;
        }
        case 2:{
            glm::vec3 direction = glm::normalize(currentObj->calc_center() - lightPos);
            
            if(yoffset > 0){
                sphere->translate(glm::translate(-direction));
                lightPos = sphere->getModel() * glm::vec4(0, 0, 0, 1);
            }
            else{
                sphere->translate(glm::translate(direction));
                lightPos = sphere->getModel() * glm::vec4(0, 0, 0, 1);
            }
            
            break;
        }
        case 3:{
            glm::vec3 direction = glm::normalize(currentObj->calc_center() - lightPos);
            
            if(yoffset > 0){
                sphere->translate(glm::translate(-direction));
                lightPos = sphere->getModel() * glm::vec4(0, 0, 0, 1);
                currentObj->scale(1);
            }
            else{
                sphere->translate(glm::translate(direction));
                lightPos = sphere->getModel() * glm::vec4(0, 0, 0, 1);
                currentObj->scale(0);
            }
            
            break;
        }
        default:
            break;
    }
    
}

void Window::cursor_callback(GLFWwindow* window, double xpos, double ypos){ 
    // no action detected
    if(event == 0) return;
    // left press and hold
    if(event == 1){
        // Map the mouse position to a logical sphere location.
        // Keep it in the class variable lastPoint.
        glm::vec2 point = glm::vec2(xpos, ypos);
        
        // get world coord of first point click
        lastPoint = trackBallMapping(point); 
        // set event to handle cursor moving
        event = 2;
        return;
    }
    
    // Get the current point in world coord
    curPos = trackBallMapping(glm::vec2(xpos, ypos));
    /* Calculate the angle in radians, and clamp it between 0 and 90 degrees */
    angle = glm::acos(std::min(1.0f, glm::dot(lastPoint, curPos)));
    /* Cross product to get the rotation axis, but it's still in camera coordinate */
    rotAxis  = glm::cross(lastPoint, curPos);
    
    if(mode == 1){
        currentObj->rotate(glm::rotate(glm::degrees(angle) * 0.05f, rotAxis));
    }
    else if(mode == 2){
        sphere->rotate(glm::rotate(glm::degrees(angle) * 0.05f, rotAxis));
        glm::vec4 rotLight = glm::vec4(lightPos.x, lightPos.y, lightPos.z, 1.0f);
        rotLight = glm::rotate(glm::degrees(angle) * 0.05f, rotAxis) * rotLight;
        lightPos.x = rotLight.x;
        lightPos.y = rotLight.y;
        lightPos.z = rotLight.z;
    }
    else{
        currentObj->rotate(glm::rotate(glm::degrees(angle) * 0.05f, rotAxis));
        sphere->rotate(glm::rotate(glm::degrees(angle) * 0.05f, rotAxis));
        glm::vec4 rotLight = glm::vec4(lightPos.x, lightPos.y, lightPos.z, 1.0f);
        rotLight = glm::rotate(glm::degrees(angle) * 0.05f, rotAxis) * rotLight;
        lightPos.x = rotLight.x;
        lightPos.y = rotLight.y;
        lightPos.z = rotLight.z;
    }
    
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods){
    event = (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
}

glm::vec3 Window::trackBallMapping(glm::vec2 point){
    glm::vec3 v;
    float d;
    
    v.x = (2.0f * point.x - width) / width;
    v.y = (height - 2.0f * point.y) / height;
    v.z = 0.0f;
    
    d = glm::length(v);
    d = (d < 1.0f) ? d : 1.0f;
    v.z = sqrtf(1.001f - d * d);
    
    v = glm::normalize(v);
    return v;
}


