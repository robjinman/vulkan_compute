#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <vulkan/vulkan.h>
#include "application.hpp"
#include "exception.hpp"

namespace {

const int VERSION_MAJOR = 0;
const int VERSION_MINOR = 1;

class ApplicationImpl : public Application {
  public:
    ApplicationImpl();

    void start() override;
    
  private:
    void createVulkanInstance();
    void setupDebugMessenger();
    
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;
};

const std::vector<const char*> ValidationLayers = {
  "VK_LAYER_KHRONOS_validation"
};

void checkValidationLayerSupport() {
  uint32_t layerCount;
  VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr),
    "Failed to enumerate instance layer properties");

  std::vector<VkLayerProperties> available(layerCount);
  VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, available.data()),
    "Failed to enumerate instance layer properties");

  for (auto layer : ValidationLayers) {
    auto fnMatches = [=](const VkLayerProperties& p) {
      return strcmp(layer, p.layerName) == 0;
    };
    if (std::find_if(available.begin(), available.end(), fnMatches) == available.end()) {
      EXCEPTION("Validation layer '" << layer << "' not supported");
    }
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT /*severity*/,
  VkDebugUtilsMessageTypeFlagsEXT /*type*/,
  const VkDebugUtilsMessengerCallbackDataEXT* data,
  void* /*userData*/) {

  std::cerr << "Validation layer: " << data->pMessage << std::endl;

  return VK_FALSE;
}

VkDebugUtilsMessengerCreateInfoEXT getDebugMessengerCreateInfo() {
  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                         | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                         | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData = nullptr;
  return createInfo;
}

std::vector<const char*> getRequiredExtensions() {
  std::vector<const char*> extensions;

#ifndef NDEBUG
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

  return extensions;
}

}

void ApplicationImpl::setupDebugMessenger() {
  auto createInfo = getDebugMessengerCreateInfo();

  auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
    vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));
  if (func == nullptr) {
    EXCEPTION("Error getting pointer to vkCreateDebugUtilsMessengerEXT()");
  }
  VK_CHECK(func(m_instance, &createInfo, nullptr, &m_debugMessenger),
           "Error setting up debug messenger");
}

void ApplicationImpl::createVulkanInstance() {
#ifndef NDEBUG
  checkValidationLayerSupport();
#endif

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Vulkan Compute Examples";
  appInfo.applicationVersion = VK_MAKE_VERSION(VERSION_MAJOR, VERSION_MINOR, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
#ifdef NDEBUG
  createInfo.enabledLayerCount = 0;
  createInfo.pNext = nullptr;
#else
  createInfo.enabledLayerCount = ValidationLayers.size();
  createInfo.ppEnabledLayerNames = ValidationLayers.data();

  auto debugMessengerInfo = getDebugMessengerCreateInfo();
  createInfo.pNext = &debugMessengerInfo;
#endif

  auto extensions = getRequiredExtensions();

  createInfo.enabledExtensionCount = extensions.size();
  createInfo.ppEnabledExtensionNames = extensions.data();

  VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_instance), "Failed to create instance");
}

ApplicationImpl::ApplicationImpl() {
  createVulkanInstance();
#ifndef NDEBUG
  setupDebugMessenger();
#endif
}

void ApplicationImpl::start() {
  // TODO
}

ApplicationPtr createApplication() {
  return std::make_unique<ApplicationImpl>();
}

