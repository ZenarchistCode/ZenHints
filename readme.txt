[h1]What Is This?[/h1]

This mod adds a simple server-side JSON config file for allowing admins to add new game hints to the loading screens, or to replace the vanilla ones altogether.

Because the server must sync the hint config to the client, the hints will not apply until a player logs in to your server at least once, and any updates you make to the config won't apply until after their next login.

To add new hints simply copy what's in the default file, change the text, and then change the value of "CfgVersion" to a number or a date or any other value than the default value to force a resync to all players.

Don't forget to add a comma after the } when adding a new hint, but not on the final }. Use this website to verify your JSON file structure is correct when modifying the file: https://jsonlint.com/

After installation, run the server and the JSON config file will be automatically created in your server_profiles_folder/Zenarchist/ZenHints.json

[h1]Installation Instructions:[/h1]

Install this mod like any other mod - copy it into your server folder and add it to your mods list. Make sure to copy the .bikey into your server keys if you're not using a server management tool like OmegaManager which does that automatically.

This mod must be run on both client and server.

[h1]Repack & Source Code:[/h1]

You can repack this mod if you like, and do anything else you want with it for that matter. Just keep in mind my future updates won't be applied so make sure to check back for new versions if you notice any bugs. The source code is on my GitHub at www.zenarchist.com

[h1]Learn Modding[/h1]

Want to learn how to make your own mods? Check out my guides on YouTube: https://www.youtube.com/@Zenarchist

[h1]Buy Me A Coffee:[/h1]

All my mods are free and open source, but it takes an enormous amount of time to put some of these mods together. If I've helped you out, please consider helping me buy my next coffee! I don't expect it, but I very much appreciate it.

https://buymeacoffee.com/zenarchist

Enjoy