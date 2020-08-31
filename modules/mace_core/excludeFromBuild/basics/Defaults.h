#pragma once

// default window settings
const float PHI = 1.618f;
const float DEFAULT_DESKTOP_WINDOW_HEIGHT = 720.0f;
const float DEFAULT_DESKTOP_WINDOW_WIDTH = DEFAULT_DESKTOP_WINDOW_HEIGHT * PHI;
const int DEFAULT_DESKTOP_WINDOW_CHANNELS = 3;
const std::string DEFAULT_DESKTOP_WINDOW_NAME = "DesktopWindow";
const int DEFAULT_DESKTOP_WINDOW_REFRESH_RATE = 16;
const bool DEFAULT_DESKTOP_WINDOW_RESIZABLE = true;
const float DEFAULT_ZOOM_FACTOR = 0.05f;
const Eigen::Vector4f DEFAULT_DESKTOP_WINDOW_BACKGROUND_COLOR = Eigen::Vector4f (0.2f, 0.22f, 0.24f, 1.0f);
const Eigen::Vector4f DEFAULT_MESH_COLOR = Eigen::Vector4f (1.0f, 0.5f, 0.0f, 1.0f);
const float DEFAULT_RENDER_GAMMA = 1.0f;

const Eigen::Vector3f DEFAULT_CAMERA_POSIIION = Eigen::Vector3f (2.0f, 3.0f, -4.0f);
const Eigen::Vector3f DEFAULT_CAMERA_TARGET = Eigen::Vector3f::Zero();
const float DEFAULT_FOV_DEGREES = 45.0f;
const float DEFAULT_ASPECT = (float)DEFAULT_DESKTOP_WINDOW_WIDTH / (float)DEFAULT_DESKTOP_WINDOW_HEIGHT;
const float DEFAULT_NEAR_PLANE = 0.01f;
const float DEFAULT_FAR_PLANE = 1000.0f;

using ItemID = int64_t;
using ImageID = int64_t;
using TextureID = int64_t;
using MaterialID = int64_t;

const int64_t INVALID_ID = -1;
const int64_t INVALID_INDEX = -1;
const std::string INVALID_PATH = "invalid path";
const std::string INVALID_NAME = "invalid name";
const std::string DEFAULT_ERROR_MESSAGE = "AOK";
const std::string DEFAULT_CAMERA_NAME = "Default_Camera";

const std::string BASE_COLOR_TEXTURE_NAME = "baseColorTexture";
const std::string METALLIC_ROUGHNESS_TEXTURE_NAME = "metallicRoughness";
const std::string NORMAL_MAP_TEXTURE_NAME = "normalMap";
const std::string DEFAULT_AREA_LIGHT_NAME = "AreaLight";

using BodyID = ItemID;
using PolyID = ItemID;
using RenderedPixels = std::vector<uint8_t>;
using RenderedFloatPixels = std::vector<float>;
using ImagePixels = RenderedPixels;
using ImageFloatPixels = RenderedFloatPixels;

const int DEFAULT_MIN_WINDOW = 10;
const int DEFAULT_CHANNELS = 3;
const int INVALID_IMAGE_SIZE = -1;

// mapped from GLFW  
#define MOUSE_BUTTON_1 0
#define MOUSE_BUTTON_2 1
#define MOUSE_BUTTON_3 2
#define MOUSE_BUTTON_4 3
#define MOUSE_BUTTON_5 4
#define MOUSE_BUTTON_6 5
#define MOUSE_BUTTON_7 6
#define MOUSE_BUTTON_8 7
#define MOUSE_BUTTON_LAST MOUSE_BUTTON_8
#define MOUSE_BUTTON_LEFT MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE MOUSE_BUTTON_3

#define MOUSE_RELEASE 0
#define MOUSE_PRESS 1
#define MOUSE_REPEAT 2