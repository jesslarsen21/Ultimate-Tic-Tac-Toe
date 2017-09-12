DIRS := \
	base64 fileno sha1 \
	signals \
	../../networking/websocket \
	../../messaging \
	../../Physics \
	../../accounts \
	../../ai \
	../../networking/classyServer

all install clean:
	for d in $(DIRS); do (cd $$d; make $@); done
