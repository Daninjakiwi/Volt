#include <render/Mesh.hpp>

namespace volt {
	Vertex::Vertex(Vec3 _pos, Vec3 _normal, Vec2 _uv) : position(_pos), normal(_normal), uv(_uv) {}

	Mesh Mesh::createPrimative(PrimativeTypes type) {
		Mesh mesh;
		switch (type) {
		case PrimativeTypes::CUBE:
			mesh.vertices.emplace_back(Vec3(0.0f,0.0f,0.0f), Vec3(0.0f,0.0f,1.0f), Vec2(0.0f,0.0f));
			mesh.vertices.emplace_back(Vec3(1.0f,0.0f,0.0f), Vec3(0.0f,0.0f,1.0f), Vec2(1.0f,0.0f));
			mesh.vertices.emplace_back(Vec3(1.0f,1.0f,0.0f), Vec3(0.0f,0.0f,1.0f), Vec2(1.0f,1.0f));
			mesh.vertices.emplace_back(Vec3(0.0f,1.0f,0.0f), Vec3(0.0f,0.0f,1.0f), Vec2(0.0f,1.0f));

			mesh.vertices.emplace_back(Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec2(0.0f, 0.0f));
			mesh.vertices.emplace_back(Vec3(1.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec2(1.0f, 0.0f));
			mesh.vertices.emplace_back(Vec3(1.0f, 1.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec2(1.0f, 1.0f));
			mesh.vertices.emplace_back(Vec3(1.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec2(0.0f, 1.0f));

			mesh.vertices.emplace_back(Vec3(1.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec2(0.0f, 0.0f));
			mesh.vertices.emplace_back(Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec2(1.0f, 0.0f));
			mesh.vertices.emplace_back(Vec3(0.0f, 1.0f, -1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec2(1.0f, 1.0f));
			mesh.vertices.emplace_back(Vec3(1.0f, 1.0f, -1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec2(0.0f, 1.0f));

			mesh.vertices.emplace_back(Vec3(0.0f, 0.0f, -1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec2(0.0f, 0.0f));
			mesh.vertices.emplace_back(Vec3(0.0f, 0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec2(1.0f, 0.0f));
			mesh.vertices.emplace_back(Vec3(0.0f, 1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec2(1.0f, 1.0f));
			mesh.vertices.emplace_back(Vec3(0.0f, 1.0f, -1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec2(0.0f, 1.0f));

			mesh.vertices.emplace_back(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec2(0.0f, 0.0f));
			mesh.vertices.emplace_back(Vec3(1.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec2(1.0f, 0.0f));
			mesh.vertices.emplace_back(Vec3(1.0f, 1.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec2(1.0f, 1.0f));
			mesh.vertices.emplace_back(Vec3(0.0f, 1.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec2(0.0f, 1.0f));

			mesh.vertices.emplace_back(Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.0f, 0.0f));
			mesh.vertices.emplace_back(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec2(1.0f, 0.0f));
			mesh.vertices.emplace_back(Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f,-1.0f, 0.0f), Vec2(1.0f, 1.0f));
			mesh.vertices.emplace_back(Vec3(1.0f, 0.0f, -1.0f), Vec3(0.0f,-1.0f, 0.0f), Vec2(0.0f, 1.0f));

			for (int i = 0; i < 6; i++) {
				int offset = i * 4;
				mesh.indices.push_back(0 + offset);
				mesh.indices.push_back(1 + offset);
				mesh.indices.push_back(2 + offset);
				mesh.indices.push_back(2 + offset);
				mesh.indices.push_back(3 + offset);
				mesh.indices.push_back(0 + offset);
			}

			break;
		}

		return mesh;
	}
}