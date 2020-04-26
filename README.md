# Fronter 2.0
### Universal Paradox Game Converters Group Frontend for all currently supported game converters. 

Written on a shoestring "budget" of three bananas and tested for whole 2 hours before proclaimed "fantastic" and sent to production.

It *should* work. On Windows. With some modifications, it should even compile on Linux and Mac but some functions won't be usable,
namely anything dealing with windows registry and related file/folder paths.

---

Fronter expects to find fronter-configuration.txt and fronter-options.txt inside a folder named "Configuration/".
These two files need to be provided with bundled converters as they define converter-specific required folders, files, and options.

### Supported and required tags (with examples):
#### fronter-configuration.txt

```
name = CK2ToEU4
converterFolder = CK2ToEU4
displayName = "Crusader Kings II To Europa Universalis IV"
optionsFile = "fronter-options.txt"
sourceGame = "Crusader Kings II"
targetGame = "Europa Universalis IV"

requiredFile = {
	name = faq
	internal = true
	displayName = "F.A.Q."
	tooltip = "The faq.txt"
	mandatory = false
	outputtable = false
	searchPathType = converterFolder
	searchPath = "CK2ToEU4"
	fileName = "FAQ.txt"
}
requiredFile = {
	name = SaveGame
	displayName = "Path to the CK2 Savegame"
	tooltip = "Path to your savegame from CK2"
	mandatory = true
	outputtable = true
	searchPathType = windowsUsersFolder
	searchPath = "Paradox Interactive\Crusader Kings II\save games"
	allowedExtension = "*.ck2"
}
requiredFolder = {
	name = CK2directory
	displayName = "Crusader Kings 2 Install directory"
	tooltip = "A path on your computer where Crusader Kings 2 is installed"
	mandatory = true
	searchPathType = steamFolder
	searchPathID = 203770
}
requiredFolder = {
	name = targetGameModPath
	displayName = "Europa Universalis 4 Mods directory"
	tooltip = "A path on your computer where Europa Universalis 4 keeps mods"
	mandatory = true
	searchPathType = windowsUsersFolder
	searchPath = "Paradox Interactive\Europa Universalis IV\mod"
}
```

searchPathType:
- converterFolder - looks in the profided converterFolder in current directory
- steamFolder - uses searchPathID to look for an "installation path" from windows/steam registry
- windowsUsersFolder - looks in USERHOMEDIR\Documents folder

mandatory:
- true - will appear in the Paths Tab of the Fronter

outputtable:
- true - will be sent to configuration.txt

Rest is self-explanatory! Shoestring Budget!

#### fronter-options.txt

```
option = {
	name = shatter_hre_level
	displayName = "If we're shattering HRE, how far down do we go?"
	tooltip = "HRE should be shattered to duchies as it increases prince number and thus stability."
	radioSelector = {
		radioOption = {
			name = 1
			displayName = "1 - Down to duchies [default]"
			tooltip = "Need Duchies for stability"
			default = true
		}
		radioOption = {
			name = 2
			displayName = "2 - Keep kingdoms if any [not recommended]"
			tooltip = "Kingdoms remain."
			default = false
		}
	}
}
option = {
	name = output_name
	displayName = "Mod Output Name (optional):"
	tooltip = "Please don't use cyrillic or chinese..."
	textSelector = {	
		value = ""
		editable = true
		tooltip = "Optional name for the converted mod"
	}
}
```

Entirely self-explanatory. Why waste words on such simplicity.
