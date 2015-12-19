{
	"targets": [
		{
			"target_name": "winnetinfo",
			"sources": [
				"NodeWinNetInfo/NodeWinNetInfo.cpp"
			],
			"include_dirs": [
				"<!(node -e \"require('nan')\")"
			]
		}
	]
}