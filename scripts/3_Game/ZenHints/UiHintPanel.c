modded class UiHintPanel 
{
	override void LoadContentList()
	{
		super.LoadContentList();

		if (GetZenHintsConfig().CfgVersion != ZenHintsConfig.INIT_VERSION)
		{
			GetZenHintsConfig().CheckSingleEntry();
			
			if (GetZenHintsConfig().ReplaceVanillaInsteadOfMerge)
			{
				m_ContentList.Clear();
			}
			
			foreach (HintPage hp : GetZenHintsConfig().Hints)
			{
				m_ContentList.Insert(hp);
			}
			
			//Print("[ZenHints] Loaded custom hints. Total hints in array: " + m_ContentList.Count() + "/" + GetZenHintsConfig().Hints.Count());
		}
	}
}