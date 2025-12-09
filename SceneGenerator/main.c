#include "stdio.h"

int main()
{
	while (1)
	{
		char nom[256] = { 0 };
		scanf("%s", nom);

		char chaine[256] = { 0 };
		sprintf(chaine, "%s.hpp", nom);

		// HPP
		FILE* f = fopen(chaine, "w");

		fprintf(f, "#ifndef %s\n", nom);
		fprintf(f, "#define %s\n\n", nom);

		fprintf(f, "#include \"../../common.h\"\n");
		fprintf(f, "#include <Engine/Scene/Scene.hpp>\n\n");

		fprintf(f, "namespace Engine\n");
		fprintf(f, "{\n");
		fprintf(f, "\tclass Camera;\n");
		fprintf(f, "\tclass Light;\n");
		fprintf(f, "\tclass GameObject;\n");
		fprintf(f, "{\n\n");

		fprintf(f, "class %s : public Engine::Scene\n", nom);
		fprintf(f, "{\n");
		fprintf(f, "public:\n");
		fprintf(f, "\t%s();\n", nom);
		fprintf(f, "\t~%s() override;\n", nom);

		fprintf(f, "\tvoid Init() override;\n");
		fprintf(f, "\tvoid PollEvents(sf::Event& _event) override;\n");
		fprintf(f, "\tvoid Update(float _dt) override;\n");
		fprintf(f, "\tvoid Display() override;\n");
		fprintf(f, "\tvoid Cleanup() override;\n");
		fprintf(f, "};\n\n");

		fprintf(f, "#endif");
		fclose(f);

		// CPP
		sprintf(chaine, "%s.cpp", nom);
		f = fopen(chaine, "w");

		fprintf(f, "#include \"%s.hpp\"\n\n", nom);

		fprintf(f, "// Engine include\n");
		fprintf(f, "#include <Engine/GameObject/GameObject.hpp>\n");
		fprintf(f, "#include <Engine/Light/Light.hpp>\n");
		fprintf(f, "#include <Engine/Camera/Camera.hpp>\n");
		fprintf(f, "#include <Engine/Interface/CameraInterface.hpp>\n");
		fprintf(f, "#include <Engine/System/System.hpp>\n");
		fprintf(f, "#include <Engine/Asset/AssetsManager.hpp>\n");
		fprintf(f, "#include <Engine/Scene/ScenesManager.hpp>\n");
		fprintf(f, "#include <Engine/Light/LightManager.hpp>\n");
		fprintf(f, "#include <Engine/Layer/LayerManager.hpp>\n");

		fprintf(f, "%s::%s()\n{\n\n}\n\n", nom, nom);
		fprintf(f, "%s::~%s()\n{\n\n}\n\n", nom, nom);
		fprintf(f, "%s::Init()\n{\n\t\n}\n\n", nom);
		fprintf(f, "%s::PollEvents(sf::Event& _event)\n{\n\t\n}\n\n", nom);
		fprintf(f, "%s::Update(float _dt)\n{\n\t\n}\n\n", nom);
		fprintf(f, "%s::Display()\n{\n\t\n}\n\n", nom);
		fprintf(f, "void %s::Cleanup()\n{\n\tScene::Cleanup()\n}\n\n", nom);

		fclose(f);

		printf("ok\n");
	}

}