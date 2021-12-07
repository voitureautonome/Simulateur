#include "MenuConnexion.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#ifndef SIMULATEUR_MENUCONNEXION
void handleMenuConnexion(bool* aff) {
	if (*aff) {
		ImGui::Begin("Connexion");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("Gestion du véhicule réel");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Actif", aff);      // Edit bools storing our window open/close state
		char chaine[16] = "192.168.1.42";
		ImGui::InputText("IP", chaine, IM_ARRAYSIZE(chaine));

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}
#endif // !SIMULATEUR_MENUCONNEXION


