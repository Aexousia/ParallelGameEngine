

inline Camera2D::Effect::Effect(Type type)
	: m_type(type)
	, m_enabled(false)
{
}

inline std::string Camera2D::Effect::getName() const
{
	return m_name;
}

inline void Camera2D::Effect::setName(const std::string & name)
{
	m_name = name;
}

inline Camera2D::Effect::Type Camera2D::Effect::getType() const
{
	return m_type;
}

inline bool Camera2D::Effect::getEnabled() const
{
	return m_enabled;
}

inline void Camera2D::Effect::setEnabled(bool enabled)
{
	m_enabled = enabled;
}