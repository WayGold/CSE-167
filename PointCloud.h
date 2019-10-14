#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <math.h>

#include "Object.h"

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
    std::vector<glm::ivec3> triangles;
    std::vector<glm::vec3> normals;
	GLuint vao, ebo;
    GLuint vbos[2];
	GLfloat pointSize;
public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();

	void draw();
	void update();

	void updatePointSize(GLfloat size);
	void spin(float deg);
    void scale(bool direc);
    void rotate(glm::mat4 matrix);
    
    GLfloat get_pointsize(){
        return pointSize;
    };
};

#endif

