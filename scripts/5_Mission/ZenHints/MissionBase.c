modded class MissionBase 
{
	void MissionBase()
	{
		#ifdef SERVER
		// SERVER RECEIVE RPCs
		GetRPCManager().AddRPC("ZenRPC", "RPC_ServerReceive_ZenHintsConfigVersion", this, SingeplayerExecutionType.Server);
        #else
		// CLIENT RECEIVE RPCs
        GetRPCManager().AddRPC("ZenRPC", "RPC_ClientReceive_ZenHintsConfig", this, SingeplayerExecutionType.Client);
		#endif
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
		
		string oldVersion = GetZenHintsConfig().CfgVersion;

        GetZenHintsConfig().Hints.Clear();

		GetZenHintsConfig().CfgVersion = data.param1.CfgVersion;
		GetZenHintsConfig().ReplaceVanillaInsteadOfMerge = data.param1.ReplaceVanillaInsteadOfMerge;
		ZenHintsConfig.STATIC_VERSION = GetZenHintsConfig().CfgVersion;
		
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
		
		string serverVersion = GetZenHintsConfig().CfgVersion;
		string clientVersion = data.param1;
		
		if (clientVersion != serverVersion)
		{
			GetRPCManager().SendRPC("ZenRPC", "RPC_ClientReceive_ZenHintsConfig", new Param1<ref ZenHintsConfig>(GetZenHintsConfig()), true, sender);
			Print("[ZenHints] Client config version " + clientVersion + "/" + serverVersion + " mismatch - sent new config to " + sender.GetId());
		}
    }
}