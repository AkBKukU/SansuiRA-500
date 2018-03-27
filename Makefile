.PHONY: all upload xbee

all: upload

upload:
	arduino --upload /home/akbkuku/git/hub/AkBKukU/SansuiRA-500/src/VUMeter/VUMeter.ino
