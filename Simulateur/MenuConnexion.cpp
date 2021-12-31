#include "MenuConnexion.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

#ifndef SIMULATEUR_MENUCONNEXION
void handleMenuConnexion(bool* aff) {
	if (*aff) {
		if (!ImGui::Begin("Connexion", aff)) {
			ImGui::End();
		}
		else {
			ImGui::Text(u8"Gestion du véhicule réel");
			//ImGui::Checkbox("Actif", aff);      // Edit bools storing our window open/close state
			char chaine[16] = "192.168.1.42";
			if (ImGui::InputText("IP", chaine, IM_ARRAYSIZE(chaine))) {
				//std::cout << chaine << std::endl;
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
	}
}
#endif // !SIMULATEUR_MENUCONNEXION


