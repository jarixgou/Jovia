#include "Common.h"

#define IMNODES_IMPLEMENTATION
#include "ImGui/imnodes.h"

struct ShaderNode
{
	int id;
	std::string name;
	
	
};

struct ShaderGraph
{
	std::vector<ShaderNode> nodes;
	std::vector<std::pair<int, int>> links;
	std::string defaultTemplate;

	ShaderGraph()
	{
		defaultTemplate =
			"precision mediump float;\n"
			"void main()\n"
			"{\n"
			"	float r = %r;\n"
			"	float g = %g;\n"
			"	float b = %b;\n"
			"	float a = %a;\n"
			"	gl_FragColor = vec4(r, g, b, a);\n"
			"}\n";
	}

	std::string GenerateShader()
	{
		std::string shader = defaultTemplate;

		// Remplacer les placeholders par les valeurs des nodes
		for (const auto& node : nodes)
		{
			size_t pos = shader.find(node.name);
			if (pos != std::string::npos)
			{
				shader.replace(pos, node.name.length(), std::to_string(node.value));
			}
		}

		return shader;
	}
};

void main()
{
	sf::Vector2f screenSize = { 1920 * 0.5f, 1080 * 0.5f };
	sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "Shader Graph", sf::Style::None);
	ImGui::SFML::Init(window);
	ImNodes::CreateContext();

	sf::Clock deltaClock;
	bool isOpen = true;
	sf::Vector2i dragOffset;
	bool isDragging = false;

	int nodeIdCounter = 0;
	ShaderGraph shaderGraph;

	while (window.isOpen())
	{
		sf::Time deltaTime = deltaClock.restart();
		float dt = deltaTime.asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event);
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaTime);
		ImGui::SetNextWindowPos({ 0,0 });
		ImGui::SetNextWindowSize({ screenSize.x, screenSize.y });
		ImGui::Begin("Shader Graph", &isOpen, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

		if (!isOpen)
		{
			window.close();
		}

		ImVec2 currentWindowSize = ImGui::GetWindowSize();
		if (currentWindowSize.x != screenSize.x || currentWindowSize.y != screenSize.y)
		{
			screenSize.x = currentWindowSize.x;
			screenSize.y = currentWindowSize.y;
			window.setSize({ static_cast<unsigned>(screenSize.x), static_cast<unsigned>(screenSize.y) });
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			if (!isDragging)
			{
				dragOffset = window.getPosition() - sf::Mouse::getPosition();
				isDragging = true;
			}
			window.setPosition(sf::Mouse::getPosition() + dragOffset);
		}
		else
		{
			isDragging = false;
		}

		ImNodes::BeginNodeEditor();

		// Node de sortie
		ImNodes::BeginNode(nodeIdCounter);
		ImNodes::BeginOutputAttribute(nodeIdCounter + 1);
		ImGui::TextUnformatted("Output (RGBA)");
		ImNodes::EndOutputAttribute();
		ImNodes::EndNode();

		ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
		ImNodes::EndNodeEditor();

		// Afficher le shader généré dans une section séparée
		ImGui::Separator();
		ImGui::Text("Shader generate :");
		ImGui::NewLine();
		ImGui::TextWrapped("%s", shaderGraph.GenerateShader().c_str());

		ImGui::End();

		window.clear(sf::Color::Black);

		ImGui::SFML::Render(window);
		window.display();
	}

	ImNodes::DestroyContext();
	ImGui::SFML::Shutdown();
}