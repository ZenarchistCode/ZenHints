modded class PlayerBase
{
	override void OnPlayerLoaded()
	{
		super.OnPlayerLoaded();
		
		if (GetGame().IsClient() && IsControlledPlayer())
		{
			// The hints will only update after player logs in to the server at least once since last version change
			GetRPCManager().SendRPC("ZenRPC", "RPC_ServerReceive_ZenHintsConfigVersion", new Param1<ref string>(ZenHintsConfig.STATIC_VERSION), true, null);
			Print("[ZenSkills] Sending hints version to server for cross-check: " + ZenHintsConfig.STATIC_VERSION);
		}
	}
}