modded class HintPage
{
	void ZenSetHeadlineText(string s)
	{
		m_IsZenHint = true;
		m_Headline = s;
	}
	
	void ZenSetDescriptionText(string s)
	{
		m_IsZenHint = true;
		m_Description = s;
	}
	
	void ZenSetImagePath(string s)
	{
		m_IsZenHint = true;
		m_ImagePath = s;
	}
	
	//! For internal use of loading screens - not needed for config
	[NonSerialized()]
	protected bool m_IsZenHint = false;
	
	void SetIsZenHint(bool state)
	{
		m_IsZenHint = state;
	}
	
	bool IsZenHint()
	{
		return m_IsZenHint;
	}
}
