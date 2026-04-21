ref ZenHintsConfig g_ZenHintsConfig;

static ZenHintsConfig GetZenHintsConfig()
{
	if (!g_ZenHintsConfig) GetZenConfigRegister().RegisterConfig(ZenHintsConfig);
	return g_ZenHintsConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload(); 
		RegisterType(ZenHintsConfig);
	}
}

class ZenHintsConfig: ZenConfigBase
{
	override void OnRegistered()
	{
		g_ZenHintsConfig = this;
	}
	
	[NonSerialized()]
	static const string INIT_VERSION = "Edit this text to force client re-sync";
	
	[NonSerialized()]
	static string STATIC_VERSION;
	
	// Config location
	override string    		GetCurrentVersion()   		{ return "1.29.1"; }
	override bool			ShouldLoadOnServer() 		
	{
		#ifdef ZenModPack
		if (!ZenModEnabled("ZenHints"))
			return false;
		#endif
		
		return true; 
	}
	override bool			ShouldLoadOnClient() 		{ return true; }
	override bool			ShouldSyncToClient()		{ return true; }
	override string 		GetSyncVersion() 			{ return SyncVersion; }

	// Config data
	string SyncVersion;
	bool ReplaceVanillaInsteadOfMerge;
	int IngameChatHintsTimerSecs;
	ref array<ref HintPage> Hints;

	override void SetDefaults()
	{
		SyncVersion = INIT_VERSION;
		STATIC_VERSION = SyncVersion;
		
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
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenHintsConfig>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenHintsConfig>.SaveFile(path, this, err);
	}
	
	override bool BuildSyncPayload(out string payload, out string err)
	{
		return JsonFileLoader<ZenHintsConfig>.MakeData(this, payload, err, false);
	}
	
	override bool ApplySyncPayload(string payload, out string err)
	{
		return JsonFileLoader<ZenHintsConfig>.LoadData(payload, this, err);
	}
	
	override void AfterLoad()
	{
		super.AfterLoad();
		
		STATIC_VERSION = SyncVersion;
		CheckSingleEntry();
		
		foreach (HintPage hint : Hints)
		{
			hint.SetIsZenHint(true);
		}
	}
}