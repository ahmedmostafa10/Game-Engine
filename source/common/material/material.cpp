#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        pipelineState.setup();

        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        Material::setup();
        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        TintedMaterial::setup(); 
        
        shader->set("alphaThreshold", alphaThreshold); 

        glActiveTexture(GL_TEXTURE0);
        texture->bind();
        sampler->bind(0); 
        shader->set("tex", 0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    void LitMaterial::setup() const{
        TexturedMaterial::setup();
        shader->attach("assets/shaders/light-test.vert", GL_VERTEX_SHADER);
        shader->attach("assets/shaders/light-test.frag", GL_FRAGMENT_SHADER);
        shader->link();
        if(sampler) sampler->bind(0);
        if(specular) specular->bind();
        if(roughness) roughness->bind();
        if(ao)       ao->bind();
        if(emission) emission->bind();

        shader->set("albedoMap", 0);
        shader->set("specularMap", 1);
        shader->set("roughnessMap", 2);
        shader->set("aoMap", 3);
        shader->set("emissionMap", 4);
    }

    void LitMaterial::deserialize(const nlohmann::json& data){
        TexturedMaterial::deserialize(data);
        if(!data.is_object()) return;
            // Load the remaining textures if they exist
        if(data.contains("specular")) {
            std::string specularPath = data["specular"];
            specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        }

        if(data.contains("roughness")) {
            std::string roughnessPath = data["roughness"];
            roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        }

        if(data.contains("ao")) {
            std::string aoPath = data["ao"];
            ao = AssetLoader<Texture2D>::get(data.value("ao", ""));
        }

        if(data.contains("emission")) {
            std::string emissionPath = data["emission"];
            AssetLoader<Texture2D>::get(data.value("emission", ""));
        }

        // Setup sampler (with default filtering/wrapping)
        sampler = new Sampler();
        sampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        sampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        sampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
        sampler->set(GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

}