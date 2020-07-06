# Fronter 2.0
### Universal Paradox Game Converters Group Frontend for all currently supported game converters. 

Written on a shoestring "budget" of three bananas and tested for whole 2 hours before proclaimed "fantastic" and sent to production.

It *should* work. On Windows. With some modifications, it should even compile on Linux and Mac but some functions won't be usable,
namely anything dealing with windows registry and related file/folder paths.

---

Fronter expects to find fronter-configuration.txt and fronter-options.txt inside a folder named "Configuration/".
These two files need to be provided with bundled converters as they define converter-specific required folders, files, and options.

Why two files instead of a single one? Completely irrelevant! Feel free to dump all options into *either* of the two files,
but Fronter will whine if it can't find either. It's better to have them separate as options change often and the configuration changes basically never.

### Supported and required tags (with examples):
#### fronter-configuration.txt

```
name = CK2ToEU4
converterFolder = CK2ToEU4
displayName = DISPLAYNAME
sourceGame = SOURCEGAME
targetGame = TARGETGAME
autoGenerateModsFrom = CK2ModsDirectory

requiredFile = {
	name = SaveGame
	displayName = FILE2
	tooltip = FILE2TIP
	mandatory = true
	outputtable = true
	searchPathType = windowsUsersFolder
	searchPath = "Paradox Interactive\Crusader Kings II\save games"
	allowedExtension = "*.ck2"
}

requiredFile = {
	name = converterExe
	displayName = FILE1
	tooltip = FILE1TIP
	mandatory = true
	outputtable = false
	searchPathType = converterFolder
	searchPath = "CK2ToEU4"
	allowedExtension = "*.exe"
	fileName = "CK2ToEU4Converter.exe"
}

requiredFolder = {
	name = CK2directory
	displayName = FOLDER1
	tooltip = FOLDER1TIP
	mandatory = true
	searchPathType = steamFolder
	searchPathID = 203770
}

requiredFolder = {
	name = targetGameModPath
	displayName = FOLDER4
	tooltip = FOLDER4TIP
	mandatory = true
	searchPathType = windowsUsersFolder
	searchPath = "Paradox Interactive\Europa Universalis IV\mod"
}
```

searchPathType:
- converterFolder - looks in the provided converterFolder in current directory
- steamFolder - uses searchPathID to look for an "installation path" from windows/steam registry. If there's a match it will also append searchPath at the end so you can use this for Vic2installdir/mods.
- windowsUsersFolder - looks in $USERHOMEDIR$\Documents folder
- direct - copies over an absolute path from searchPath

mandatory:
- true - path will be checked and converter won't run without it.

outputtable (relevant for files only):
- true - will be sent to configuration.txt

Rest is self-explanatory! Shoestring Budget!

#### fronter-options.txt

```
option = {
	name = shatter_hre_level
	displayName = OPTION2
	tooltip = OPTION2TIP
	radioSelector = {
		radioOption = {
			name = 1
			displayName = OP2R1
			tooltip = OP2R1TIP
			default = true
		}
		radioOption = {
			name = 2
			displayName = OP2R2
			tooltip = OP2R2TIP
			default = false
		}
	}
}

option = {
	name = output_name
	displayName = OPTION8
	tooltip = OPTION8TIP
	textSelector = {	
		value = ""
		editable = true
		tooltip = OPTION8TEXTTIP
	}
}

option = {
	name = some_checkbox_control
	displayName = THENAME
	tooltip = THETOOLTIP
	checkBoxSelector = {
		checkBoxOption = {
			name = yes
			displayName = CBNAME
			tooltip = CBTIP
			default = true
		}
		checkBoxOption = {
			name = 2
			displayName = NAME2
			tooltip = TIP2
			default = false
		}
		checkBoxOption = {
			name = sausage
			displayName = SAUSAGE
			tooltip = SAUSAGETIP
			default = true
		}
	}
}
```

Entirely self-explanatory. Why waste words on such simplicity.

Multiple default values set to true on checkBoxOptions are perfectly fine.

#### Localization

Fronter expects to find appropriate yml files in Configuration/ directory. yml files look like this:

```l_french:
 DISPLAYNAME: "Crusader Kings II à Europa Universalis IV"
 SOURCEGAME: "Crusader Kings II"
 TARGETGAME: "Europa Universalis IV"
 FOLDER1: "Répertoire d'installation de Crusader Kings II"
 FOLDER1TIP: "Un chemin sur votre ordinateur où Crusader Kings 2 est installé"
```
or
```
l_english:
 OPTION1: "Which empire inherits EU4 HRE mechanics and shatters?"
 OPTION1TIP: "Only one empire can use HRE mechanics."
 OP1R1: "The HRE [obviously]"
 OP1R1TIP: "e_hre"
 OP1R2: "Byzantium! [Holy and Roman and Empire]"
```

Yml files need to be encoded in UTF-8, not UTF-8-BOM.

Fronter will load the files (regardless of their actual name) and use `l_language:` to file the key-value pairs under appropriate language.
Whenever configuration or options use these keys, it will attempt to load appropriate localization string to be displayed.

If it fails to find a key-pair under a specific language, it will default to english. If there's no english either, it will display a blank string. 
Look for these blank strings to see where you made a typo.

#### cofiguration.txt

Fronter will export a single configuration.txt in converterFolder/ with paired values, for all outputtable files/folders and all options:

```
SaveGame = "somepath/autosave.ck2"
CK2directory = "c:\Path\To\Game Installation\"
targetGameModPath = "d:\somePath\to\mods"
shatter_hre_level = "1"
output_name = "some user input"
some_checkbox_control = { "yes", "1", "sausage" }
selectedMods  = { "modfilename1.mod" "second mod filena.me.mod" "oddlynamed.mod.mod.mod" } # this one is done if autoGenerateModsFrom was used and valid
```

Slashes and backslashes depend on OS, so Converter is responsible for parsing them.

### Mod move after conversion

Fronter will attempt to move "\[output_name\].mod" and "\[output_name\]/" folder (or "\[SaveGame\].mod" and "\[SaveGame\]/" in case output_name was not set) from "\[converterFolder\]/output/" subdirectory to "\[targetGameModPath\]/".
Success will undoubtedly vary of the amount of effort invested in this program.
