modded class MissionBase 
{
	void MissionBase()
	{
		// add rpcs (removed til we figure out new sync behaviour)
	}
	
	// Server -> client
    void RPC_ClientReceive_ZenHintsConfig(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param1<ref ZenHintsConfig> data;
        if (!ctx.Read(data))
        {
            Error("Error sync'ing server-side data to client - RPC_ClientReceive_ZenHintsConfig");
            return;
        }
		
		string oldVersion = GetZenHintsConfig().SyncVersion;

        GetZenHintsConfig().Hints.Clear();

		GetZenHintsConfig().SyncVersion = data.param1.SyncVersion;
		GetZenHintsConfig().ReplaceVanillaInsteadOfMerge = data.param1.ReplaceVanillaInsteadOfMerge;
		GetZenHintsConfig().IngameChatHintsTimerSecs = data.param1.IngameChatHintsTimerSecs;
		ZenHintsConfig.STATIC_VERSION = GetZenHintsConfig().SyncVersion;
		
		foreach (HintPage hp : data.param1.Hints)
		{
			GetZenHintsConfig().Hints.Insert(hp);
		}
		
		GetZenHintsConfig().Save();
        Print("[ZenHints] Received & saved new config version: " + ZenHintsConfig.STATIC_VERSION + " oldVersion=" + oldVersion);
    }
	
	// Client -> server
    void RPC_ServerReceive_ZenHintsConfigVersion(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param1<ref string> data;
        if (!ctx.Read(data))
        {
            Error("Error sync'ing server-side data to client - RPC_ServerReceive_ZenHintsConfigVersion");
            return;
        }
		
		string serverVersion = GetZenHintsConfig().SyncVersion;
		string clientVersion = data.param1;
		
		if (clientVersion != serverVersion)
		{
			GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ClientReceive_ZenHintsConfig", new Param1<ref ZenHintsConfig>(GetZenHintsConfig()), true, sender);
			Print("[ZenHints] Client config version " + clientVersion + "/" + serverVersion + " mismatch - sent new config to " + sender.GetId());
		}
    }
}