#include "Node.h"
#include "Geometry.h"
#include <vector>
class Group : public Node {
public:
	Group(char* name);
	Group();
	~Group();
	Geometry* sphere;
	char* groupName;
	std::vector<Node*> children;
	bool bounding;
	void draw(GLuint shaderProgram,glm::mat4 C) ;
	void addChild(Node* child);
	void removeChild(Node* child);
	void update() ;
};