#include "Entity.h"
#include "3dMath.h"
#include "Camera.h"
#include "Input.h"
void Entity::Update() {
	if (getComponent<Camera>() == NULL && getComponent<Light>() == NULL) {
		
		transform.rotation.y = count * 8;
		//Camera::activeCamera->entity->transform.rotation.z = count * 8;
		//Camera::activeCamera->entity->transform.rotation.x = count * 8;
		//Camera::activeCamera->entity->transform.position.z = 10;
		//Camera::activeCamera->entity->transform.position.y = abs(sinf(count));
		//transform.rotation.z = count * 10;
		//transform.scale.y = abs(sinf(count));
		//transform.position.z = -10;
		transform.position.z = -6.0f;
		transform.position.y = -4.0f;
		transform.rotation.y = 180.0f;
		count += .1f;
	}

	
	else if (getComponent<Camera>() != NULL) {

		float speed = .3f;
		if (Input::getKey(GLFW_KEY_LEFT_SHIFT)) {
			speed = .5f;
		}

		if (Input::getKey(GLFW_KEY_W)) {
			transform.position = transform.position+ transform.getForward()*speed;
		}
		else if (Input::getKey(GLFW_KEY_S)) {
			transform.position = transform.position - transform.getForward()*speed;
		}

		if (Input::getKey(GLFW_KEY_A)) {
			transform.position = transform.position - transform.getRight()*speed;
		}
		else if (Input::getKey(GLFW_KEY_D)) {
			transform.position = transform.position + transform.getRight()*speed;
		}
		if (Input::getKey(GLFW_KEY_LEFT_CONTROL)) {
			transform.rotation.y += Input::mouseDeltax * .04f;
			transform.rotation.x -= Input::mouseDeltay * .04f;
		}

	}
	else if (getComponent<Light>() != NULL) {
		transform.rotation.y = 180;
		transform.rotation.x = 0;
		transform.rotation.z = 0;
		getComponent<Light>()->type = Light::Spot;
	}



	if (Input::getKeyUp(GLFW_KEY_SPACE)) {
		if (getComponent<Mesh>() != NULL) {
			getComponent<Mesh>()->material->shader->Reload();
		}
	}






}

 Entity::Entity() {

}

 Entity::~Entity()
 {

	 for (int i = 0; i < Components.size(); i++)
	 {
		 Component* temp = Components[Components.size() - 1];
		 Components.pop_back();
		 delete(temp);
		 temp = NULL;
	 }
 }
