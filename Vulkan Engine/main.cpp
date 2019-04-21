#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>

const int WIDTH = 800;
const int HEIGHT = 600;
class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow* window;
	VkInstance instance;

	void initWindow() {
		//Initialize glfw for window context
		glfwInit();

		//Disable for openGL
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		//Create window
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Engine", nullptr, nullptr);

	}
	//Check that all required extensions is avaible
	VkResult checkExtensions() {
		//Get avaible extensions
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		//Get required extensions
		uint32_t requiredExtensionCount = 0;
		const char **glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
		std::vector<const char*> requiredExtensions(glfwExtensions, glfwExtensions + requiredExtensionCount);

		//Check required against avaible
		int counter = 0;
		for (const auto& requiredExtension : requiredExtensions) {
			for (const auto& avaibleExtension : extensions) {
				if (!strcmp(avaibleExtension.extensionName, requiredExtension)) {
					counter++;
				}
			}
		}
		if (counter != requiredExtensionCount) {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
		return VK_SUCCESS;
	}

	//Create a vulkan instance
	void createInstance() {
		//Struct with application specific information
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		//Struct with info about global extansions and validation layers
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.pNext = NULL;

		//Check extensions	
		if (checkExtensions() != VK_SUCCESS) {
			throw std::runtime_error("Extensions is missing!");
		}

		//Get the extansion for vulkan
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		createInfo.enabledLayerCount = 0;

		//Create the instance
		if (vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}


	}

	void initVulkan() {
		createInstance();
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	void cleanup() {
		glfwDestroyWindow(window);

		glfwTerminate();
	}
};

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}