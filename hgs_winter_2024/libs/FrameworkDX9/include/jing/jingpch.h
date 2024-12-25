/**********************************************************//**
 * @file jingpch.h
 * @brief プリコンパイル済みヘッダー
 *
 * @author Taito Jin
 * @date since 2024/12/17
 **************************************************************/
#pragma once

/* インクルード */

#pragma region Includes

// Windows
#define _WIN32_DCOM
#define NOMINMAX
#include <Windows.h>

// DirectX SDK
#include <d3dx9.h>
#define DIRECTINPUT_VERSION	(0x0800)
#include <dinput.h>
#include <xaudio2.h>
#include <XInput.h>
#include <d3dcompiler.h>
#include <wrl.h>

// Others
#include <algorithm>
#include <array>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <atomic>
#include <future>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <memory>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>
#include <cassert>
#include <chrono>
#define _USE_MATH_DEFINES
#include <cmath>
#include <codecvt>
#include <comutil.h>
#include <cstdarg>
#include <functional>
#include <random>
#include <string>
#include <utility>
#include <cstdint>
#include <type_traits>
#include <bitset>
#include <regex>
#include <optional>
#include <variant>
#include <any>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <locale>
#include <ctime>

// JSON
#include "nlohmann/json.hpp"
#include "nlohmann/json-schema.hpp"
//#include "custom_json.h"

// ECS
#include <entt/entt.hpp>

// magic_enum
#include <magic_enum.hpp>

// utf
#include "utf8.h"

// imgui-docking
#define IMGUI_DEFINE_MATH_OPERATORS
#include "jing/imgui/imgui.h"
#include "jing/imgui/imgui_impl_dx9.h"
#include "jing/imgui/imgui_impl_win32.h"
#include "jing/imgui/imgui_internal.h"

// imgui-addons
#include "jing/imgui/imgui_addons.h"

// bullet-physics
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

#pragma endregion

/* ライブラリのリンク */

#pragma region Libraries

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")

#pragma comment(lib, "Bullet2FileLoader.lib")
#pragma comment(lib, "Bullet3Collision.lib")
#pragma comment(lib, "Bullet3Common.lib")
#pragma comment(lib, "Bullet3Dynamics.lib")
#pragma comment(lib, "Bullet3Geometry.lib")
#pragma comment(lib, "Bullet3OpenCL_clew.lib")
#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "BulletExampleBrowserLib.lib")
#pragma comment(lib, "BulletFileLoader.lib")
#pragma comment(lib, "BulletInverseDynamics.lib")
#pragma comment(lib, "BulletInverseDynamicsUtils.lib")
#pragma comment(lib, "BulletRobotics.lib")
#pragma comment(lib, "BulletRoboticsGUI.lib")
#pragma comment(lib, "BulletSoftBody.lib")
#pragma comment(lib, "BulletWorldImporter.lib")
#pragma comment(lib, "BulletXmlWorldImporter.lib")

#pragma comment(lib, "BussIK.lib")
#pragma comment(lib, "clsocket.lib")
#pragma comment(lib, "ConvexDecomposition.lib")
#pragma comment(lib, "GIMPACTUtils.lib")
#pragma comment(lib, "gwen.lib")
#pragma comment(lib, "HACD.lib")
#pragma comment(lib, "LinearMath.lib")

#pragma endregion
