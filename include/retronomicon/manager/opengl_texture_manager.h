#pragma once

#include <memory>
#include "retronomicon/graphics/texture.h"
#include "retronomicon/manager/texture_manager.h"
#include "retronomicon/asset/image_asset.h"  

namespace retronomicon::opengl::manager{
	using retronomicon::manager::TextureManager;
    using retronomicon::graphics::Texture;
    using retronomicon::asset::ImageAsset;
	class OpenGLTextureManager:public TextureManager{
		public:
			OpenGLTextureManager();
			~OpenGLTextureManager();
			std::shared_ptr<Texture>  createTexture(std::shared_ptr<ImageAsset> ImageAsset) override;
	};
}