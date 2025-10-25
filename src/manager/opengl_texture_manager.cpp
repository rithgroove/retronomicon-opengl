#include "retronomicon/manager/opengl_texture_manager.h"
#include "retronomicon/graphics/opengl_texture.h"
namespace retronomicon::opengl::manager{
	using retronomicon::opengl::graphics::OpenGLTexture;
	OpenGLTextureManager::OpenGLTextureManager(){

	}

	OpenGLTextureManager::~OpenGLTextureManager(){

	}

	std::shared_ptr<Texture>  OpenGLTextureManager::createTexture(std::shared_ptr<ImageAsset> imageAsset){
		return std::make_shared<OpenGLTexture>(imageAsset);
	}
}