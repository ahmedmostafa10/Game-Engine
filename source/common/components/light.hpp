#include "../ecs/component.hpp"
#include<string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <json/json.hpp>

namespace our {
    enum class LightType {
        Directional,
        Point,
        Spot
    };
    
    class lightComponent : public Component {
        public:
            glm::vec4 color;
            glm::vec4 diffuseColor;
            glm::vec4 specularColor;
            LightType type; // Directional Light, Point Light, Spot Light, these are all supported types
            float umbraAngle; // outer cone angle
            float penumbraAngle; // inner cone angle
            float intensity; // Brightness of the light
            
            // The ID of this component type is "light"
            static std::string getID() { return "Light"; }

            void lightComponent::deserialize(const nlohmann::json& data){
                if(!data.is_object()) return;
                color = data.value("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                diffuseColor = data.value("diffuseColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                specularColor = data.value("specularColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                std::string lightTypeStr = data.value("type", "directional");
                if(lightTypeStr == "point") type = our::LightType::POINT;
                else if(lightTypeStr == "spot") type = our::LightType::SPOT;
                else type = our::LightType::DIRECTIONAL;
                umbraAngle = data.value("umbraAngle",0.0f);
                penumbraAngle = data.value("penumbraAngle",0.0f);
                intensity = data.value("intensity",0.0f);
                
            }
    }; 
}
