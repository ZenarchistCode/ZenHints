modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();

		#ifdef ZENMODPACK 
		if (!ZenModEnabled("ZenHints"))
			return;
		#endif
		
		Print("[ZenHints] MissionServer::OnInit");

		// Load server-side config 
		GetZenHintsConfig();
	}
}