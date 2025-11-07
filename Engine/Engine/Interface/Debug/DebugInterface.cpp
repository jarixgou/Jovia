#include "DebugInterface.hpp"

#include <numeric>
#include <vector>
#include <algorithm>
#include <fstream>
#include <thread>
#include <Windows.h>
#include <ImGui/imgui.h>
#include <psapi.h>

float Engine::DebugInterface::m_dt = 0.0f;
float Engine::DebugInterface::m_gpuTime = 0.0f;
static float timer = 0.0f;
static std::vector<float> fpsHistory(100, 0.0f);
static std::vector<float> frameTimeHistory(100, 0.0f);
static std::vector<float> cpuHistory(100, 0.0f);
static std::vector<float> gpuHistory(100, 0.0f);
static float minFps = FLT_MAX;
static float maxFps = 0.0f;
static int targetFps = 60;

static std::string GetCPUName()
{
	HKEY hKey;
	char cpuName[256] = "Unknown CPU";
	DWORD bufferSize = sizeof(cpuName);

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
		0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		RegQueryValueExA(hKey, "ProcessorNameString", NULL, NULL,
			(LPBYTE)cpuName, &bufferSize);
		RegCloseKey(hKey);
	}

	return std::string(cpuName);
}

void Engine::DebugInterface::Update(sf::RenderWindow& _window, float _dt, float _gpuTime)
{
	timer += _dt;

	if (timer >= 0.08f)
	{
		m_dt = _dt;
		m_gpuTime = _gpuTime;

		// Update historic fps
		float currentFps = 1.0f / m_dt;
		fpsHistory.erase(fpsHistory.begin());
		fpsHistory.push_back(currentFps);

		// Update frame time history
		frameTimeHistory.erase(frameTimeHistory.begin());
		frameTimeHistory.push_back(m_dt * 1000.0f);

		// Update CPU/GPU history
		float cpuTime = (m_dt * 1000.0f) - m_gpuTime;
		cpuHistory.erase(cpuHistory.begin());
		cpuHistory.push_back(cpuTime);
		gpuHistory.erase(gpuHistory.begin());
		gpuHistory.push_back(m_gpuTime);

		// Calculate min/max
		minFps = std::min(minFps, currentFps);
		maxFps = std::max(maxFps, currentFps);

		timer = 0.0f;
	}

	ImGui::Begin("Info");

	// System Info Section
	if (ImGui::CollapsingHeader("System Info"))
	{
		ImGui::Text("OS: Windows");
		ImGui::Text("OpenGL: %s", glGetString(GL_VERSION));
		ImGui::Text("OpenGL: %s", glGetString(GL_VENDOR));
		ImGui::Text("CPU Cores: %d", std::thread::hardware_concurrency());
		ImGui::Text("CPU Name: %s", GetCPUName().c_str());
		ImGui::Text("GPU Name: %s", glGetString(GL_RENDERER));
	}

	// FPS Section
	if (ImGui::CollapsingHeader("Performance", ImGuiTreeNodeFlags_DefaultOpen))
	{
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		float memoryMB = pmc.WorkingSetSize / (1024.0f * 1024.0f);

		ImGui::Text("Memory Usage: %.2f MB", memoryMB);

		float currentFps = 1.0f / m_dt;
		float avgFps = std::accumulate(fpsHistory.begin(), fpsHistory.end(), 0.0f) / fpsHistory.size();

		// Calculate 1% and 0.1% lows
		std::vector<float> sortedFps = fpsHistory;
		std::sort(sortedFps.begin(), sortedFps.end());
		float fps1PercentLow = sortedFps[std::max(0, (int)(sortedFps.size() * 0.01f))];

		ImGui::Text("FPS :\t\t%.1f (avg: %.1f)", currentFps, avgFps);
		ImGui::Text("Min/Max :\t%.1f / %.1f", minFps, maxFps);
		ImGui::Text("1%% Low :\t%.1f", fps1PercentLow);

		ImGui::PlotLines("##FPS", fpsHistory.data(), (int)fpsHistory.size(), 0, nullptr,
			0.0f, maxFps * 1.2f, ImVec2(0, 80));

		// Timings
		ImGui::Separator();
		float cpuTime = (m_dt * 1000.0f) - m_gpuTime;
		ImGui::Text("Frame Time :\t%.2f ms", m_dt * 1000.0f);
		ImGui::Text("CPU Time :\t%.2f ms", cpuTime);
		ImGui::Text("GPU Time :\t%.2f ms", m_gpuTime);

		// CPU/GPU Graph
		ImGui::PlotLines("##CPU", cpuHistory.data(), (int)cpuHistory.size(),
			0, "CPU (ms)", 0.0f, 50.0f, ImVec2(0, 50));
		ImGui::PlotLines("##GPU", gpuHistory.data(), (int)gpuHistory.size(),
			0, "GPU (ms)", 0.0f, 50.0f, ImVec2(0, 50));

		// Indicate CPU/GPU bound
		float cpuGpuRatio = cpuTime / m_gpuTime;
		if (cpuGpuRatio > 1.2f)
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Status: CPU Bound");
		else if (cpuGpuRatio < 0.8f)
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Status: GPU Bound");
		else
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Status: Balanced");

		// Frame Budget
		ImGui::Separator();
		if (ImGui::SliderInt("Target FPS", &targetFps, 30, 240))
		{
			_window.setFramerateLimit(targetFps);
		}
		float targetFrameTime = 1000.0f / targetFps;
		float frameTimeBudget = targetFrameTime - (m_dt * 1000.0f);

		if (frameTimeBudget >= 0)
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Frame Budget: +%.2f ms", frameTimeBudget);
		else
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Frame Budget: %.2f ms (OVER)", frameTimeBudget);
	}

	// Advanced Section
	if (ImGui::CollapsingHeader("Advanced"))
	{
		float avgFps = std::accumulate(fpsHistory.begin(), fpsHistory.end(), 0.0f) / fpsHistory.size();

		// Frame time variance
		float variance = 0.0f;
		if (fpsHistory.size() > 1)
		{
			float mean = avgFps;
			for (float fps : fpsHistory)
			{
				variance += (fps - mean) * (fps - mean);
			}
			variance /= fpsHistory.size();
		}
		ImGui::Text("Frame Variance :\t%.2f", variance);

		// Standard deviation
		float stdDev = std::sqrt(variance);
		ImGui::Text("Std Deviation :\t\t%.2f", stdDev);

		ImGui::Separator();

		// Export button
		if (ImGui::Button("Export Stats to CSV"))
		{
			std::ofstream file("perf_stats.csv");
			file << "OS: Windows, CPU Core: " << std::thread::hardware_concurrency() << ", CPU Name: " << GetCPUName()
				<< ", GPU Name: " << glGetString(GL_RENDERER) << "\n" << "\n"
				 << "Frame,FPS,FrameTime(ms),CPU(ms),GPU(ms)\n";
			for (size_t i = 0; i < fpsHistory.size(); ++i)
			{
				file << i << "," << fpsHistory[i] << ","
					<< frameTimeHistory[i] << ","
					<< cpuHistory[i] << "," << gpuHistory[i] << "\n";
			}
			file.close();
		}

		ImGui::SameLine();

		// Reset button
		if (ImGui::Button("Reset Stats"))
		{
			minFps = FLT_MAX;
			maxFps = 0.0f;
			std::fill(fpsHistory.begin(), fpsHistory.end(), 0.0f);
			std::fill(frameTimeHistory.begin(), frameTimeHistory.end(), 0.0f);
			std::fill(cpuHistory.begin(), cpuHistory.end(), 0.0f);
			std::fill(gpuHistory.begin(), gpuHistory.end(), 0.0f);
		}
	}

	ImGui::End();
}