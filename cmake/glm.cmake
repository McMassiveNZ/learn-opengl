include_guard()

CPMAddPackage(
  NAME GLM
  GITHUB_REPOSITORY g-truc/glm
  GIT_TAG 0.9.9.8
  VERSION 0.9.9.8
  DOWNLOAD_ONLY YES 
)

if(GLM_ADDED)
  add_library(GLM INTERFACE IMPORTED)
  target_include_directories(GLM INTERFACE ${GLM_SOURCE_DIR})
endif()