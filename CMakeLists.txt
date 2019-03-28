# setup lab environment

cmake_minimum_required(VERSION 2.8)

get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include("${FIPS_ROOT_DIR}/cmake/fips.cmake")

# include python
find_package(PythonLibs 3.7 REQUIRED)

get_filename_component(ROOT_DIR "./" ABSOLUTE)
add_definitions(-DAI_URL=\"${ROOT_DIR}\")

fips_setup(PROJECT lab-env)
fips_include_directories(code)
fips_include_directories(${PYTHON_INCLUDE_DIRS})
fips_ide_group(Lab)
fips_add_subdirectory(code/ECS)
fips_add_subdirectory(code/Game)
fips_finish()