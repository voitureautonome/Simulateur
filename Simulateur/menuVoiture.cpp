#include "menuVoiture.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
void handleMenuVoiture(bool* aff)
{
	if (*aff) {
		if (!ImGui::Begin(u8"V�hicule", aff)) {
			ImGui::End();
		}
		else {
			ImGui::Text(u8"Gestion du v�hicule simul�");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Actif", aff);      // Edit bools storing our window open/close state
			const char* items[] = { "Lidar", "Vision Camera","Les deux" };
			static int item_current = 0;
			ImGui::Combo("Mode", &item_current, items, IM_ARRAYSIZE(items));
			ImGui::End();
		}
		
	}
}
