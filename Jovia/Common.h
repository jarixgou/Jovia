#ifndef COMMON__H
#define COMMON__H

#include <iostream>
#include <time.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui-SFML.h"

#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/freeglut.h>
#include <Engine/Logger/Logger.hpp>
#include "SFML/OpenGL.hpp"

#include <windows.h>

#ifdef _DEBUG
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-audio-d.lib")
#pragma comment(lib, "sfml-network-d.lib")
#else
#pragma comment(linker, "/NODEFAULTLIB:MSVCRTD")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd")

#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-audio.lib")
#pragma comment(lib, "sfml-network.lib")
#endif

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

struct WindowState
{
	bool vsync = false;

	bool requestFullscreen = false;
	bool fullscreen = false;
};

sf::Sprite LoadSprite(const std::string _path , bool);
void BlitSprite(sf::Sprite _sprite, sf::Vector2f _pos, float _angle, sf::RenderWindow& _window);
void DrawPixel(sf::Image& _image, sf::Vector2i _pos, sf::Color _color);

#endif
