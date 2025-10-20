modded class PlayerBase
{
	protected ref Timer m_ZenHintsTimer;
	
	override void OnPlayerLoaded()
	{
		super.OnPlayerLoaded();
		
		#ifdef ZENMODPACK 
		if (!ZenModEnabled("ZenHints"))
			return;
		#endif
		
		if (GetGame().IsClient() && IsControlledPlayer())
		{
			// The hints will only update after player logs in to the server at least once since last version change
			GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ServerReceive_ZenHintsConfigVersion", new Param1<ref string>(ZenHintsConfig.STATIC_VERSION), true, null);
			Print("[ZenSkills] Sending hints version to server for cross-check: " + ZenHintsConfig.STATIC_VERSION);
			
			if (GetZenHintsConfig().IngameChatHintsTimerSecs > 0 && (!m_ZenHintsTimer || !m_ZenHintsTimer.IsRunning()))
			{
				m_ZenHintsTimer = new Timer(CALL_CATEGORY_SYSTEM);
				m_ZenHintsTimer.Run(GetZenHintsConfig().IngameChatHintsTimerSecs, this, "PrintZenChatHint", null, true);
			}
		}
	}
	
	override void OnDisconnect()
	{
		super.OnDisconnect();
		
		if (m_ZenHintsTimer)
			m_ZenHintsTimer.Stop();
	}
	
	protected void PrintZenChatHint()
	{
		HintPage hintCfg = GetZenHintsConfig().Hints.GetRandomElement();
		string hintStr = hintCfg.GetDescriptionText();
	
		int chunkSize = 150;
		array<string> hints = new array<string>;
	
		int i = 0;
		int total = hintStr.Length();
	
		// Skip any leading spaces
		while (i < total && hintStr.Substring(i, 1) == " ") 
		{ 
			i++; 
		}
	
		while (i < total)
		{
			int remain = total - i;
	
			// Last piece fits
			if (remain <= chunkSize)
			{
				if (remain > 0) 
				{ 
					hints.Insert(hintStr.Substring(i, remain)); 
				}
				
				break;
			}
	
			int end = i + chunkSize; // exclusive
			int cut = -1;
	
			// Find last space within [i, end)
			int k = end;
			while (k > i)
			{
				if (hintStr.Substring(k - 1, 1) == " ")
				{
					cut = k - 1;
					break;
				}
				
				k = k - 1;
			}
	
			int take = chunkSize;
			if (cut != -1) 
			{ 
				take = cut - i; 
			}
	
			if (take > 0) 
			{ 
				hints.Insert(hintStr.Substring(i, take)); 
			}
	
			// Advance and skip break space(s)
			i = i + take;
			while (i < total && hintStr.Substring(i, 1) == " ") 
			{ 
				i++; 
			}
		}
	
		if (hints.Count() == 0)
		{ 
			hints.Insert(hintStr); 
		}
	
		// Print each chunk on its own chat line
		for (int j = 0; j < hints.Count(); j++)
		{
			string part = hints.Get(j);
			ZenHintsChat(part);
			Print("Hint(part " + (j + 1) + "/" + hints.Count() + "): " + part);
		}
	}
	
	static void ZenHintsChat(string msg)
	{
		GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCSystem, "", msg, ""));
	}
}