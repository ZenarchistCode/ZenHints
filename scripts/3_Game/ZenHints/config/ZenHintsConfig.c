modded class HintPage 
{
	void ZenSetHeadlineText(string s)
	{
		m_Headline = s;
	}
	
	void ZenSetDescriptionText(string s)
	{
		m_Description = s;
	}
	
	void ZenSetImagePath(string s)
	{
		m_ImagePath = s;
	}
}

class ZenHintsConfig
{
	static const string INIT_VERSION 			= "Edit this text to force client re-sync";
	
	// Config location
	private const static string ModFolder		= "$profile:\\Zenarchist\\";
	private const static string ConfigName		= "ZenHintsConfig.json";
	static string STATIC_VERSION;

	// Config data
	string CfgVersion;
	bool ReplaceVanillaInsteadOfMerge;
	int IngameChatHintsTimerSecs;
	ref array<ref HintPage> Hints;

	void Load()
	{
		SetDefaultValues();
		
		string serverIP = "";
		
		#ifndef SERVER
		GetCLIParam("connect", serverIP);
		serverIP.Replace(":", "");
		serverIP.Replace(".", "");
		
		if (serverIP == "")
		{
			Error("SERVER IP UNAVAILABLE!");
			return;
		}
		#endif
		
		string filePath = ModFolder + serverIP + "\\" + ConfigName;
		
		Print("[ZenHints] Loading config file for server IP: " + filePath);

		if (FileExist(filePath))
		{ 
			// If config exists, load file
			JsonFileLoader<ZenHintsConfig>.JsonLoadFile(filePath, this);	
			Print("[ZenHints] Successfully loaded config. Version: " + CfgVersion);
			STATIC_VERSION = CfgVersion;
			//return;
		}
		else 
		{
			Print("[ZenHints] Config file does not exist, creating a new one...");
		}

		Save();
	}

	void SetDefaultValues()
	{
		CfgVersion = INIT_VERSION;
		STATIC_VERSION = CfgVersion;
		
		ReplaceVanillaInsteadOfMerge = false;
		Hints = new array<ref HintPage>;
		
		HintPage exampleHint = new HintPage();
		exampleHint.ZenSetHeadlineText("Example Headline");
		exampleHint.ZenSetDescriptionText("Example hint description.");
		
		Hints.Insert(exampleHint);
		
		IngameChatHintsTimerSecs = 0; // 1800 = 30 minutes etc
	}
	
	void CheckSingleEntry()
	{
		if (Hints.Count() == 0)
		{
			// No hints present (error in JSON?) - force game to use vanilla.
			ReplaceVanillaInsteadOfMerge = false;
		}
		else 
		if (Hints.Count() == 1)
		{
			// If only 1 hint is present, it seems to break the game loading. I guess it hangs when cycling
			// to the next hint? Anyway, just duplicate the single hint so that game has 2 hints to swap between.
			HintPage singleHint = Hints.Get(0);
			
			HintPage extraHint = new HintPage();
			extraHint.ZenSetHeadlineText(singleHint.GetHeadlineText());
			extraHint.ZenSetDescriptionText(singleHint.GetDescriptionText());
			extraHint.ZenSetImagePath(singleHint.GetImagePath());
			
			Hints.Insert(extraHint);
			Print("[ZenHints] Only one hint was found - duplicated it to avoid hang on load. Count=" + Hints.Count());
		}
	}

	void Save()
	{
		if (!FileExist(ModFolder))
		{
			MakeDirectory(ModFolder);
		}

		string serverIP = "";
		
		#ifndef SERVER
		GetCLIParam("connect", serverIP);
		serverIP.Replace(":", "");
		serverIP.Replace(".", "");
		#endif
		
		string filePath = ModFolder + serverIP + "\\" + ConfigName;
		
		#ifndef SERVER
		if (!FileExist(ModFolder + serverIP))
		{
			MakeDirectory(ModFolder + serverIP);
		}
		#endif

		JsonFileLoader<ZenHintsConfig>.JsonSaveFile(filePath, this);
	}
}

static ZenHintsConfig GetZenHintsConfig()
{
	if (!m_ZenHintsConfig)
	{
		Print("[ZenHintsConfig] Init");
		m_ZenHintsConfig = new ZenHintsConfig();
		m_ZenHintsConfig.Load();
	}

	return m_ZenHintsConfig;
}

ref ZenHintsConfig m_ZenHintsConfig;