modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();

		Print("[ZenHints] MissionServer::OnInit");

		// Load server-side config 
		GetZenHintsConfig();
	}
}