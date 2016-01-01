#pragma once
#include <GL\glew.h>
#include <cstddef>

namespace SerraEngine {

	struct Position {
		GLfloat x;
		GLfloat y;
		Position() = default;
	};

	struct Color {
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
		Color() = default;
		Color(GLubyte r_, GLubyte g_, GLubyte b_, GLubyte a_) :
			r(r_),
			g(g_),
			b(b_),
			a(a_) {};
	};

	struct UV {
		GLfloat u;
		GLfloat v;
		UV() = default;
	};

	struct Vertex {
		Position position;
		Color color;
		UV uv;
		void setPosition(GLfloat x, GLfloat y) {
			position.x = x;
			position.y = y;
		};
		void setPosition(Position position_) {
			position = position_;
		};
		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		};
		void setColor(Color color_) {
			color = color_;
		};
		void setUV(GLfloat u, GLfloat v) {
			uv.u = u;
			uv.v = v;
		};
		void setUV(UV uv_) {
			uv = uv_;
		};
		Vertex() = default;
	};
}