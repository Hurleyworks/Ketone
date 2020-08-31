
// This header file was auto-generated by ClassMate++
// Created: 17 Oct 2019 2:51:45 pm
// Copyright (c) 2019, HurleyWorks

#pragma  once

// from tinyGLTF
#define TEXTURE_FILTER_NEAREST (9728)
#define TEXTURE_FILTER_LINEAR (9729)
#define TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST (9984)
#define TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST (9985)
#define TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR (9986)
#define TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR (9987)

#define TEXTURE_WRAP_REPEAT (10497)
#define TEXTURE_WRAP_CLAMP_TO_EDGE (33071)
#define TEXTURE_WRAP_MIRRORED_REPEAT (33648)

enum TextureMode
{
	wrap = 0,    /**< Wrapping address mode */
	clamp = 1,    /**< Clamp to edge address mode */
	mirror = 2,    /**< Mirror address mode */
	border = 3     /**< Border address mode */
};

enum TextureFilterMode
{
	point = 0,     /**< Point filter mode */
	linear = 1      /**< Linear filter mode */
};

struct TextureSampler
{
	std::string name;
	
	ImageID imageID = INVALID_ID;

	TextureMode wrapS = TextureMode::wrap;
	TextureMode wrapT = TextureMode::wrap;
	TextureFilterMode filter = TextureFilterMode::point;
};

using Samplers = std::vector<TextureSampler>;

using TextureHandle = std::shared_ptr<class Texture>;

class Texture : public HasId
{

 public:
	static TextureHandle create() { return std::make_shared<Texture>(); }
 
 public:
	Texture() {}
	~Texture() { LOG(DBUG) << _FN_; }

	const TextureID getID() const { return id(); }

	void addTextureImage(PixelBufferHandle& image) { textureImages.push_back(image); }
	void addTextureSampler(const TextureSampler& sampler) { samplers.push_back(sampler);}
	size_t imageCount() const { return textureImages.size(); }

	const Samplers& getSamplers() const { return samplers; }
	const Images& getImages() const { return textureImages; }
	Images& getImages()  { return textureImages; }

 private:
	Images textureImages;
	Samplers samplers;

}; // end class Texture

using Textures = std::vector<TextureHandle>;
using TextureDB = std::unordered_map<TextureID, TextureHandle>;