#pragma once
#include "../../Includes/Imgui/imgui.h"
#include "../../Includes/Imgui/imgui_internal.h"

namespace bools
{
	static float boxsize = 1;
	static float boxheight = 1;
	static float boxwidth = 1;
	static float tracersize = 0.1f;
	static bool aimbot = true;
	static bool headesp = false;
	static int fovvalue = 150;
	static bool aimbotfov = true;
}

static float FovCircleColor[3] = { 0.f, 1.0f, 0.f };

auto ColorRNew = FovCircleColor[0] * 255;
auto ColorGNew = FovCircleColor[1] * 255;
auto ColorBNew = FovCircleColor[2] * 255;

auto drawmenu() -> void
{
	if (GetAsyncKeyState(VK_INSERT) & 1) { Settings::bMenu = !Settings::bMenu; }

	if (Settings::bMenu)
	{
		ImGui::SetNextWindowSize({ 350,350 });
		ImGui::Begin(" Open Source Version  ", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		{

			ImGui::Spacing();

			ImGui::Checkbox("2D Box", &Settings::Visuals::bBox);
			ImGui::Checkbox("Box V2", &Settings::Visuals::bboxv2);
			ImGui::Checkbox("Cornered Box", &Settings::Visuals::bBoxOutlined);
			ImGui::Checkbox("Snaplines", &Settings::Visuals::bSnaplines);
			ImGui::Checkbox("Distance", &Settings::Visuals::bDistance);
			ImGui::Checkbox("HealthBar", &Settings::Visuals::bHealth);
			ImGui::SliderFloat("Box Size", &bools::boxsize, 1, 10);
			ImGui::SliderFloat("Tracer Size", &bools::tracersize, 0, 10);
		}

		ImGui::End();
	}
}


