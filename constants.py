DEBUG = False

BUTTON_WIDTH = 150
BUTTON_HEIGHT = 30

WINDOW_WIDTH = 900
WINDOW_HEIGHT = 450

TEXTAREA_WIDTH = (WINDOW_WIDTH - 20) / 2
TEXTAREA_HEIGHT = 150

TEXTINPUT_WIDTH = (WINDOW_WIDTH - 20) / 2
TEXTINPUT_HEIGHT = 30

LABEL_WIDTH = TEXTAREA_WIDTH
LABEL_HEIGHT = 30

PATH_TO_FILTER = "Debug\profanities.exe" if DEBUG else "profanities.exe"
PATH_TO_WHITELIST = "./whitelist_debug.txt" if DEBUG else "./whitelist.txt"
PATH_TO_PROFANITIES = "./profanity_list_debug.txt" if DEBUG else "./profanity_list.txt"
PATH_TO_HELP = "help.txt"
PATH_TO_README = "README.md"
