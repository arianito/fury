#include "zeus/core.h"

using namespace zeus;


struct Game1 : game {
	shader sh;

	glu vao, vbo, ebo;

	void create() {
		sh.create(path::local("src", "z02_point", "s1"));


		glGenVertexArrays(1, &vao);
	}
	void dispose() {
		sh.dispose();

		glDeleteVertexArrays(1, &vao);
	}
	void render(f32 t){
		const glf color[] = {0.2f, 0.25f, 0.3f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, color);

		sh.begin();
		glPointSize(40);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);

		sh.end();


	}
};


int main(){
	return Game1().run();
}