
#include <LowRenderer/ParticleSystem.hpp>

#include <Resources/Shader.hpp>
#include <Resources/Material.hpp>
#include <Resources/Scene.hpp>
#include <Resources/ResourcesManager.hpp>

#include <Engine/GameObject.hpp>

#include <Core/Log.hpp>

#include <Maths/Random.hpp>

#include <Utils/StringExtractor.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace Resources;

void ParticleSystem::showImGUI()
{
	Resources::ResourcesManager* resources = Resources::ResourcesManager::instance();
	ImGui::Checkbox("Active", &m_active);
	ImGui::Checkbox("Use Lights", &m_useLights);
	ImGui::Checkbox("Loop", &m_loop);
	
	if (!m_loop)
		ImGui::DragFloat("PlayTime", &m_playTime.m_endTime);

	if (ImGui::TreeNode("Render Options"))
	{
		const char* materialPreview = m_material ? m_materialName.c_str() : "none";
		if (ImGui::BeginCombo("Material", materialPreview))
		{
			for (auto& material : resources->m_materialName_material)
			{
				if (ImGui::Selectable(material.first.c_str(), material.first == materialPreview))
				{
					m_material = &material.second;
					m_materialName = material.first;
				}
			}
			ImGui::EndCombo();
		}

		const char* shaderPreview = m_shader ? m_shaderName.c_str() : "none";
		if (ImGui::BeginCombo("Shader", shaderPreview))
		{
			for (auto& shader : resources->m_shaderName_shader)
			{
				if (ImGui::Selectable(shader.first.c_str(), shader.first == shaderPreview))
				{
					m_shader = &shader.second;
					m_shaderName = shader.first;
				}
			}
			ImGui::EndCombo();
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Transform position"))
	{
		pos.showImGUI();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Spawning Options"))
	{
		ImGui::DragFloat2("Lifetime Range", &m_lifetimeRange.c[0]);
		ImGui::DragFloat2("SpawnRate Range", &m_spawnrateRange.c[0]);
		ImGui::DragFloat2("Start Size Range", &m_startSize.c[0]);
		ImGui::DragFloat2("Start Speed Range", &m_startSpeed.c[0]);
		ImGui::DragFloat4("Start Color Min Range", &m_startColorMin.e[0]);
		ImGui::DragFloat4("Start Color Max Range", &m_startColorMax.e[0]);
		ImGui::DragInt("Max Particles", &m_maxParticles);
		ImGui::DragFloat3("Start Velocity", &m_StartVelocity.c[0]);
		ImGui::DragFloat("Gravity Multiplier", &m_gravityMultiplier);

		ImGui::TreePop();
	}
	
	if (ImGui::TreeNode("Cone Options"))
	{
		ImGui::Checkbox("Use Cone Dispersion", &isCone);
		ImGui::DragFloat("Length", &Conesize);
		ImGui::DragFloat("Inner Radius", &spawnCircleRadius);
		ImGui::DragFloat("Outer Radius", &endCircleRadius);
		ImGui::DragFloat3("Cone Direction", &m_coneDir.c[0]);
		ImGui::TreePop();
	}

	const char* items[] = { "HIT", "DEATH", "VENT"};

	if (ImGui::BeginCombo("Preset", "Select"))
	{	
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			if (ImGui::Selectable(items[n], false))
			{
				ApplyPreset((ParticleSystemPresets) n);
			}
		}
		ImGui::EndCombo();
	}
}


void ParticleSystem::destroy()
{
	//  get Renderer Manager of the scene
	Core::RendererManager* _renderer = &m_gameObject->m_sceneReference->m_rendererManager;

	//  Get last index (trash index)
	int lastIndex = (int)_renderer->m_particleSystemList.size() - 1;

	//  Parsing list of the current renderer component type
	for (auto comp : _renderer->m_particleSystemList)
	{
		//  Once we find "this", let's procees to the retirement of the component from the list
		if (comp.second == this)
		{
			//  Swap current with the last one if it's not already the last one
			if (comp.first != lastIndex)
			{
				_renderer->m_particleSystemList[comp.first] = _renderer->m_particleSystemList[lastIndex];
			}

			//  Set the pointer to null
			_renderer->m_particleSystemList[lastIndex] = nullptr;

			//  Then erase it from the list
			_renderer->m_particleSystemList.erase(lastIndex);

			//  And leave the loop
			//  To avoid errors and optimize
			break;
		}
	}

	//  Then call the common component destoy function
	Component::destroy();
}


void ParticleSystem::play() 
{
	m_playTime.restart();
	m_playTime.on();
	setActive(true);
}

void ParticleSystem::ApplyPreset(ParticleSystemPresets preset) 
{
	switch (preset) 
	{
		case ParticleSystemPresets::HIT: 
		{
			m_loop = false; m_playTime.m_endTime = 0.08f; m_lifetimeRange.x = 0.15f; m_lifetimeRange.y = 0.28f; m_spawnrateRange = 0.05f;
			m_startSize.x = 0.5f; m_startSize.y = 0.75f; m_startSpeed.x = 4.f; m_startSpeed.y = 9.f; m_maxParticles = 100;
			m_gravityMultiplier = 2.f; Conesize = 2.f; spawnCircleRadius = 1.f; endCircleRadius = 3.f; isCone = true;
			return;
		}
	}
}

void ParticleSystem::update() 
{
	timer.setEndTime(Maths::randRange(m_spawnrateRange.x, m_spawnrateRange.y));

	for (int i = 0; i < particles.size(); i++)
		particles[i].timer.ended() ? destroyParticle(i) : particles[i].update();

	if (!m_active) return;
	
	if (!m_loop) 
		m_playTime.update();
	
	timer.update();
	if (timer.ended())
	{
		if (particles.size() < m_maxParticles)
		{
			timer.restart();
			emit();
		}
	}
	if (!m_loop && m_playTime.ended())
		m_active = false;
}

void ParticleSystem::draw() 
{
	if (!m_shader || !m_material) return;
	m_shader->use(); 
	m_shader->setMaterial(*m_material);
	for (const Particle& particle : particles) 
	{
		m_shader->setBool("is_affected_by_light", m_useLights);
		m_shader->setFloat4("ParticleColor", particle.getColor());
		m_shader->setMat4("Model", particle.transform.getTransformMatrix());

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
	}
}

ParticleSystem::ParticleSystem(GameObject* in_gameObject) : Component(in_gameObject)
{
    init(ComponentType::ParticleSystem);

    initGL();
    m_gameObject->m_sceneReference->m_rendererManager.m_particleSystemList[(int)m_gameObject->m_sceneReference->m_rendererManager.m_particleSystemList.size()] = this;
	pos.m_gameObject = in_gameObject;

	// Set Particle shader as default
	Resources::loadShader("Particle");
	m_shader = &ResourcesManager::instance()->m_shaderName_shader["Particle"];
	m_shaderName = "Particle";

	m_playTime = Timer();
}

void ParticleSystem::emit() 
{ 
	ParticleSpecs particleInfo =
	{
		 Maths::randRange(m_lifetimeRange.x, m_lifetimeRange.y),
		 Maths::randRange(m_startSize.x, m_startSize.y),
		 Maths::randRange(m_startSpeed.x, m_startSpeed.y),
		 Maths::randColor(m_startColorMin, m_startColorMax),
		 m_StartVelocity,
		 isCone,
		 Conesize,
		 spawnCircleRadius,
		 endCircleRadius,
		 m_gravityMultiplier,
		 m_coneDir,
	};
	particles.push_back({ &pos, particleInfo});
}

void ParticleSystem::destroyParticle(int i) 
{
	particles[i] = particles[particles.size() - 1];
	particles.pop_back();
}

void ParticleSystem::initGL()
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        -0.5f,  0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f,

       -0.5f,  0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 0.0f
    };

    //	Create VAO - Vertex Array Object
    glGenVertexArrays(1, &VAO);

    //	Create VBO - Vertex Buffer Object
    glGenBuffers(1, &VBO);

    //	Define VBO and VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //	Attach VBO to VAO / Bind attributes (texCoord) in VAO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //	Position & texCoord
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);
	
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleSystem::saveComponentInSCNFile(std::ofstream& file)
{
	file << "PARTICLESYSTEM\t";
	file << m_materialPath << " ";
	file << m_materialName << " ";
	file << m_shaderName << " ";;
	file << "/";
	FileWriter::writeVec2InFile(file, m_lifetimeRange);
	file << "/";
	FileWriter::writeVec2InFile(file, m_spawnrateRange);
	file << "/";
	FileWriter::writeVec2InFile(file, m_startSize);
	file << "/";
	FileWriter::writeVec2InFile(file, m_startSpeed);
	file << "/";
	file << m_maxParticles; file << "/";
	file << m_playTime.m_endTime; file << "/";
	file << m_gravityMultiplier; file << "/";
	file << m_active; file << "/";
	file << m_loop; file << "/";
	file << isCone; file << "/";
	FileWriter::writeVec4InFile(file, m_startColorMin);
	file << "/";
	FileWriter::writeVec4InFile(file, m_startColorMax);
	file << "/";
	FileWriter::writeVec3InFile(file, m_StartVelocity);
	file << "/";
	file << Conesize; file << "/";
	file << spawnCircleRadius; file << "/";
	file << endCircleRadius; file << "/";
	FileWriter::writeVec3InFile(file, m_coneDir);
	file << "\n";
}

void ParticleSystem::loadComponentFromSCNFile(std::istringstream& lineStream)
{
	Core::Log* _log = Core::Log::instance();
	_log->write("+\t Adding Particle System to a new gameObject");

	lineStream.ignore();
	using namespace FileParser;
	m_materialPath = getString(lineStream);
	Resources::stringList nonUsed;
	ResourcesManager::instance()->parseMTL(
		Extractor::ExtractDirectory(std::string(m_materialPath)),
		Extractor::ExtractFilename(std::string(m_materialPath)),
		nonUsed
	);
	m_materialName = getString(lineStream);
	m_material = &ResourcesManager::instance()->m_materialName_material[m_materialName];

	m_shaderName = getString(lineStream);
	Resources::loadShader(m_shaderName);
	m_shader = &ResourcesManager::instance()->m_shaderName_shader[m_shaderName];
	lineStream.ignore(); lineStream.ignore();
	m_lifetimeRange  = getVector2(lineStream);	lineStream.ignore();
	m_spawnrateRange = getVector2(lineStream);	lineStream.ignore();
	m_startSize      = getVector2(lineStream);	lineStream.ignore();
	m_startSpeed     = getVector2(lineStream);	lineStream.ignore();
	m_maxParticles   = getInt(lineStream); lineStream.ignore(); m_playTime.m_endTime = getFloat(lineStream);
	lineStream.ignore(); m_gravityMultiplier = getFloat(lineStream); lineStream.ignore();
	m_active = getBool(lineStream); lineStream.ignore(); m_loop = getBool(lineStream); 
	lineStream.ignore(); isCone = getBool(lineStream); lineStream.ignore();
	m_startColorMin  = getVector4(lineStream);  lineStream.ignore();
	m_startColorMax  = getVector4(lineStream);  lineStream.ignore();
	m_StartVelocity  = getVector3(lineStream);  lineStream.ignore();
	Conesize = getFloat(lineStream); lineStream.ignore(); spawnCircleRadius = getFloat(lineStream);  lineStream.ignore();
	endCircleRadius = getFloat(lineStream); lineStream.ignore();
	m_coneDir = getVector3(lineStream);
}