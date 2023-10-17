-- A solution contains projects, and defines the available configurations
workspace "opengl_reference"
	configurations { "Debug", "Release" }
	location "build"
	targetdir "build"
	includedirs { "inc", "src/glcommon" }

	platforms { "linux", "windows" }

	filter "platforms:linux"
		links { "SDL2", "GLEW", "GL", "m" }
	
	filter "platforms:windows"
		links { "mingw32", "SDL2main", "SDL2", "glew32", "opengl32" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
	

	project "ex1"
		kind "ConsoleApp"
		language "C"
		files {
			"src/ex1.c"
		}
		
		configuration { "gmake" }
			buildoptions { "-std=c99", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-Wimplicit" }


	project "ex2"
		kind "ConsoleApp"
		language "C"
		files {
			"src/ex2.c"
		}
		
		configuration { "gmake" }
			buildoptions { "-std=c99", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-Wimplicit" }

	project "lesson1"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/lesson1.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/rsw_math.cpp",
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "lesson2"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/lesson2.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/rsw_math.cpp",
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "lesson3"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/lesson3.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/rsw_math.cpp",
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "lesson4"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/lesson4.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/rsw_math.cpp",
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "lesson5"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/lesson5.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/rsw_math.cpp",
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "lesson6"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/lesson6.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/rsw_math.cpp",
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "lesson7"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/lesson7.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/rsw_math.cpp",
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "multidraw"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/multidraw.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/rsw_math.cpp",
		}

	project "texturing"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/texturing.cpp",
			"src/glcommon/rsw_math.cpp",
			"src/glcommon/gltools.cpp"
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "modelviewer"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/modelviewer.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/rsw_math.cpp",
			"src/glcommon/rsw_primitives.cpp",
			"src/glcommon/rsw_halfedge.cpp"
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "testprimitives"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/testprimitives.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/rsw_math.cpp",
			"src/glcommon/rsw_primitives.cpp",
			"src/glcommon/rsw_halfedge.cpp"
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "grass"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/grass.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/rsw_math.cpp",
			"src/glcommon/rsw_glframe.cpp",
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }


	project "sphereworld_color"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/sphereworld_color.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/rsw_math.cpp",
			"src/glcommon/rsw_glframe.cpp",
			"src/glcommon/rsw_primitives.cpp",
			"src/glcommon/rsw_halfedge.cpp",
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "flying"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/flying.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/rsw_math.cpp",
			"src/glcommon/rsw_glframe.cpp",
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	
	project "glm_modelviewer"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/glm_modelviewer.cpp",
			"src/glcommon/c_utils.cpp",
			"src/glcommon/glm_primitives.cpp",
			"src/glcommon/glm_halfedge.cpp"
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }


	project "glm_texturing"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/glm_texturing.cpp",
			"src/glcommon/c_utils.cpp",
			"src/glcommon/gltools.cpp"
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "point_sprites"
		kind "ConsoleApp"
		language "C"
		files {
			"src/point_sprites.c",
			"src/glcommon/gltools.c"
		}
		
		configuration { "gmake" }
			buildoptions { "-std=c99", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-Wimplicit" }

	project "glm_sphereworld_color"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/glm_sphereworld_color.cpp",
			"src/glcommon/glm_glframe.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/glm_primitives.cpp",
			"src/glcommon/glm_halfedge.cpp",
			"src/glcommon/c_utils.cpp",
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }

	project "glm_grass"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/glm_grass.cpp",
			"src/glcommon/gltools.cpp",
			"src/glcommon/glm_glframe.cpp",
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }


	project "glm_flying"
		kind "ConsoleApp"
		language "C++"
		files {
			"src/glm_flying.cpp",
			"src/glcommon/glm_glframe.cpp",
			"src/glcommon/gltools.cpp",
		}
		
		configuration { "gmake" }
			buildoptions { "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type" }


	project "left_handed"
		kind "ConsoleApp"
		language "C"
		files {
			"src/left_handed.c"
		}
		
		configuration { "gmake" }
			buildoptions { "-std=c99", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-Wimplicit" }


	project "gears"
		kind "ConsoleApp"
		language "C"
		files {
			"src/gears.c"
		}
		
		configuration { "gmake" }
			buildoptions { "-std=c99", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-Wimplicit" }

-- programs just to test things
	project "test_poly_modes"
		kind "ConsoleApp"
		language "C"
		files {
			"src/tests/poly_modes.c"
		}
		
		configuration { "gmake" }
			buildoptions { "-std=c99", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-Wimplicit" }

	project "test_scissoring"
		kind "ConsoleApp"
		language "C"
		files {
			"src/tests/scissoring.c"
		}
		
		configuration { "gmake" }
			buildoptions { "-std=c99", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-Wimplicit" }

	project "test_viewport"
		kind "ConsoleApp"
		language "C"
		files {
			"src/tests/viewport.c"
		}
		
		configuration { "gmake" }
			buildoptions { "-std=c99", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-Wimplicit" }

	project "test_clipping"
		kind "ConsoleApp"
		language "C"
		files {
			"src/tests/clipping.c"
		}
		
		configuration { "gmake" }
			buildoptions { "-std=c99", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-Wimplicit" }

	project "test_depthclamp"
		kind "ConsoleApp"
		language "C"
		files {
			"src/tests/depthclamp.c"
		}
		
		configuration { "gmake" }
			buildoptions { "-std=c99", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-Wimplicit" }

	project "test_line_depth"
		kind "ConsoleApp"
		language "C"
		files {
			"src/tests/thick_line_depth.c"
		}
		
		configuration { "gmake" }
			buildoptions { "-std=c99", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-Wimplicit" }

	project "client_arrays"
		kind "ConsoleApp"
		language "C"
		files {
			"src/tests/client_arrays.c"
		}
		
		configuration { "gmake" }
			buildoptions { "-std=c99", "-fno-strict-aliasing", "-Wunused-variable", "-Wreturn-type", "-Wimplicit" }

-- GLES programs have their own premake5.lua in src/tests/ and a build directory
-- in build/gles

