

inline void Camera2D::Influencer::setProps(const Vector2 & position, float affectRange)
{
	m_position = position;
	m_affectRange = affectRange;
}

inline void Camera2D::Influencer::setStrength(float strength)
{
	m_strength = strength;
}

inline std::string Camera2D::Influencer::getName() const
{
	return m_name;
}

inline void Camera2D::Influencer::setName(const std::string & name)
{
	m_name = name;
}

inline Camera2D::Influencer::Type Camera2D::Influencer::getType() const
{
	return m_type;
}

inline Camera2D::Vector2 Camera2D::Influencer::getPosition() const
{
	return m_position;
}

inline float Camera2D::Influencer::getRange() const
{
	return m_affectRange;
}

inline float Camera2D::Influencer::getStrength() const
{
	return m_strength;
}
