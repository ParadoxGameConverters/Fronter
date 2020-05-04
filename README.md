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
supportedLocLanguages = { english french } # Keep this line at the top.

name = CK2ToEU4
converterFolder = CK2ToEU4
displayName_english = "Crusader Kings II To Europa Universalis IV"
displayName_french = "Crusader Kings II à Europa Universalis IV"
sourceGame_english = "Crusader Kings II"
sourceGame_french = "Crusader Kings II"
targetGame_english = "Europa Universalis IV"
targetGame_french = "Europa Universalis IV"

requiredFile = {
	name = SaveGame
	displayName_english = "Path to the CK2 Savegame"
	displayName_french = "Chemin vers le jeu de sauvegarde CK2"
	tooltip_english = "Path to your savegame from CK2"
	tooltip_french = "Chemin vers votre sauvegarde depuis CK2"
	mandatory = true
	outputtable = true
	searchPathType = windowsUsersFolder
	searchPath = "Paradox Interactive\Crusader Kings II\save games"
	allowedExtension = "*.ck2"
}

requiredFolder = {
	name = CK2directory
	displayName_english = "Crusader Kings II Install directory"
	displayName_french = "Répertoire d'installation de Crusader Kings II"
	tooltip_english = "A path on your computer where Crusader Kings 2 is installed"
	tooltip_french = "Un chemin sur votre ordinateur où Crusader Kings 2 est installé"
	mandatory = true
	searchPathType = steamFolder
	searchPathID = 203770
}

requiredFolder = {
	name = targetGameModPath
	displayName_english = "Europa Universalis IV Mods directory"
	displayName_french = "Répertoire des Mods Europa Universalis IV"
	tooltip_english = "A path on your computer where Europa Universalis IV keeps mods"
	tooltip_french = "Un chemin sur votre ordinateur où Europa Universalis IV conserve les mods"
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
- true - will appear in the Paths Tab of the Fronter

outputtable (relevant for files only):
- true - will be sent to configuration.txt

Rest is self-explanatory! Shoestring Budget!

#### fronter-options.txt

```
option = {
	name = shatter_hre_level
	displayName_english = "If we're shattering HRE, how far down do we go?"
	displayName_french = "Si nous brisons l'EDH, jusqu'où allons-nous?"
	tooltip_english = "HRE should be shattered to duchies as it increases prince number and thus stability."
	tooltip_french = "L'EDH devrait être brisée en duchés car elle augmente le nombre de prince et donc la stabilité."
	radioSelector = {
		radioOption = {
			name = 1
			displayName_english = "Down to duchies [default]"
			displayName_french = "Jusqu'aux duchés [par défaut]"
			tooltip_english = "Need Duchies for stability."
			tooltip_french = "Besoin de duchés pour la stabilité."
			default = true
		}
		radioOption = {
			name = 2
			displayName_english = "Keep kingdoms if any [not recommended]"
			displayName_french = "Gardez les royaumes le cas échéant [non recommandé]"
			tooltip_english = "Kingdoms remain."
			tooltip_french = "Les royaumes restent."
			default = false
		}
	}
}

option = {
	name = output_name
	displayName_english = "Mod Output Name (optional):"
	displayName_french = "Nom de sortie du module (facultatif):"
	tooltip_english = "Please don't use cyrillic or chinese..."
	tooltip_french = "Veuillez ne pas utiliser cyrillique ou chinois ..."
	textSelector = {	
		value = ""
		editable = true
		tooltip_english = "Optional name for the converted mod."
		tooltip_french = "Nom facultatif pour le mod converti."
	}
}

```

Entirely self-explanatory. Why waste words on such simplicity.

#### Localization

If the Fronter is running in some language not supported by configuration, it will load english lines. That's what that first line of first file is for.

Also, unless the frontend itself is taught how to speak Russian through converter_l_russian.yml and expanded converter_languages.yml, inserting Russian localizations here won't make a bit of difference.

Also also, the french displayed is totally not a product of googletranslate but carefully broken to demonstrate the need for professional help.

#### cofiguration.txt

Fronter will export a single configuration.txt in converterFolder/ with paired values, for all outputtable files/folders and all options:

```
SaveGame = "somepath/autosave.ck2"
CK2directory = "c:\Path\To\Game Installation\"
targetGameModPath = "d:\somePath\to\mods"
shatter_hre_level = "1"
output_name = "some user input"
```

Slashes and backslashes depend on OS, so Converter is responsible for parsing them.

### Mod move after conversion

Fronter will attempt to move "\[output_name\].mod" and "\[output_name\]/" folder (or "\[SaveGame\].mod" and "\[SaveGame\]/" in case output_name was not set) from "\[converterFolder\]/output/" subdirectory to "\[targetGameModPath\]/".
Success will undoubtedly vary of the amount of effort invested in this program.
